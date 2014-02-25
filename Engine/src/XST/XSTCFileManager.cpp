#include "../../include/XST/XSTCFileManager.h"
#include "../../include/XST/XSTCFileSystemLoader.h"
#include "../../include/XST/XSTCVirtualFileSystemLoader.h"
#include "../../include/XST/XSTIFile.h"
#include "../../include/XST/XSTCSimpleProfiler.h"

namespace XST
{
	

	CFileLocationManager::CFileLocationManager(CFileManager* pMgr)
	{
		this->m_bManualResourceDestroy = true;
		m_pMgr = pMgr;

	}

	CFileLocationManager::~CFileLocationManager()
	{
		DestroyResources();
	}

	void CFileLocationManager::DestroyResources()
	{
		//XST::TCBaseResourceManager< xst_vector< CFileLocation >* >::ResourceIterator Itr;
		BaseManager::_ResourceIterator Itr;
		for(Itr = this->m_mResources.begin(); Itr != this->m_mResources.end(); ++Itr)
		{
			for(u32 i = 0; i < Itr->second->size(); ++i)
			{
				//Itr->second->at( i ).pLoader->DestroyInstance( &Itr->second->at( i ).pLoader );
			}
			xst_delete( Itr->second );
		}
		this->m_mResources.clear();
	}

	i32 CFileLocationManager::AddLocation(xst_castring& strLocation, xst_castring& strGroupName, IFileLoader* pLoader, bool bRecursive)
	{
		if( pLoader == xst_null )
		{
			return RESULT::FAILED;
		}

		xst_vector< CFileLocation >* pvLocs = xst_null;
		if( this->GetResource( strGroupName, &pvLocs ) == RESULT::OK )
		{
			pvLocs->push_back( CFileLocation( strLocation, pLoader, bRecursive ) );
		}
		else
		{
			pvLocs = xst_new xst_vector< CFileLocation >();
			pvLocs->push_back( CFileLocation( strLocation, pLoader, bRecursive ) );
			if( this->AddResource( strGroupName, pvLocs ) != RESULT::OK )
			{
				xst_delete( pvLocs );
				return RESULT::FAILED;
			}
		}

		return RESULT::OK;

	}

	i32 CFileLocationManager::AddLocation(const CFileLocation& Loc, xst_castring& strGroupName)
	{
		xst_assert( Loc.pLoader, "(CFileLocationManager::AddLocation) Loader not set" );

		xst_vector< CFileLocation >* pvLocs = xst_null;
		if( this->GetResource( strGroupName, &pvLocs ) == RESULT::OK )
		{
			pvLocs->push_back( Loc );
		}
		else
		{
			pvLocs = xst_new xst_vector< CFileLocation >();
			pvLocs->push_back( Loc );
			if( this->AddResource( strGroupName, pvLocs ) != RESULT::OK )
			{
				xst_delete( pvLocs );
				return RESULT::FAILED;
			}
		}

		return RESULT::OK;
	}

	CFileManager::CFileManager()
	{
		m_pLocMgr = xst_new CFileLocationManager( this );
		this->RegisterLoader( "FileSystem", xst_new CFileSystemLoader() );
		m_bAllFilesSupported = true;
		m_bExtensionFilter = false;
		//m_OnResRemove.bind( this, &CFileManager::_OnRemoveFile );
		//m_OnResDestroy.bind( this, &CFileManager::_OnDestroyFile );
		//this->_SetDelegates( _OnResRemove, _OnResDestroy );
		//this->m_OnResDestroyDelegate.bind( this, &CFileManager::_OnDestroyFile );
	}

	CFileManager::~CFileManager()
	{
		xst_delete( m_pLocMgr );
		LoaderMap::iterator Itr;
		
		for(Itr = m_mLoaderInstances.begin(); Itr != m_mLoaderInstances.end(); ++Itr)
		{
			xst_delete( Itr->second );
		}

		for(Itr = m_mLoaders.begin(); Itr != m_mLoaders.end(); ++Itr)
		{
			xst_delete( Itr->second );
		}
		m_mLoaders.clear();
	}

	/*void CFileManager::_OnDestroyFile(ResourcePtr pRes)
	{
		xst_assert2( pRes != xst_null );
		FilePtr& pFile = pRes;
		pFile->m_Data.Destroy();
		pFile->m_ulSize = 0;
		pFile->m_ulCompSize = 0;
		pFile->m_ulOffset = 0;
	}*/

	IFileLoader* CFileManager::_GetLoader(xst_castring& strName)
	{
		LoaderMap::iterator Itr = m_mLoaders.find( strName );
		if( Itr == m_mLoaders.end() ) return xst_null;
		return Itr->second;
	}

	i32 CFileManager::AddLocation(xst_castring &strDirectory, xst_castring &strGroupName, xst_castring& strLoaderName, bool bRecursive)
	{
		IFileLoader* pLoader = _GetLoader( strLoaderName );
		if( !pLoader )
		{
			return RESULT::FAILED;
		}

		//IFileLoader* pInstance = pLoader->CreateInstance();
		//return m_pLocMgr->AddLocation( strDirectory, strGroupName, pInstance, bRecursive );
		IFileLoader* pInstance = xst_null;

		CFileLocation Loc;
		Loc.bRecursive = bRecursive;

		// TODO: the FileLoader should check directory!!!
		if( XST_FAILED( _GetDefaultDir( strDirectory, &Loc.strLocation, &Loc.strLocationPath, &Loc.bPhysical ) ) )
		{
			return XST_FAIL;
		}

		LoaderMap::iterator Itr = m_mLoaderInstances.find( Loc.strLocation );
		if( Itr == m_mLoaderInstances.end() )
		{
			pInstance = pLoader->CreateInstance();
			m_mLoaderInstances.insert( LoaderMap::value_type( Loc.strLocation, pInstance ) );
		}
		else
		{
			pInstance = Itr->second;
		}

		Loc.pLoader = pInstance;

		if( m_pLocMgr->AddLocation( Loc, strGroupName ) != RESULT::OK )
		{
			return RESULT::FAILED;
		}

		return RESULT::OK;
	}

	i32 CFileManager::AddFileExtension(xst_castring& strExt)
	{
		m_bAllFilesSupported = ( strExt == "*" )? true : false;
		m_mExts.insert( xst_map< xst_astring, i32 >::value_type( strExt, 0 ) );
		return RESULT::OK;
	}

	i32 CFileManager::PrepareAllGroups()
	{
		/*XST::TCBaseResourceManager< xst_vector< CFileLocation >* >::*/
		xst_astring strExt;
		strExt.reserve( 10 );

		CFileLocationManager::BaseManager::_ResourceIterator LocItr = m_pLocMgr->m_mResources.begin();
		for(; LocItr != m_pLocMgr->m_mResources.end(); ++LocItr)
		{
			
			for(u32 i = 0; i < LocItr->second->size(); ++i)
			{
				//Get file location info
				CFileLocation& Loc = LocItr->second->at( i );

				if( !Loc.pLoader->IsReady() )
				{
					//xst_astring strDefaultDir;
					//bool bIsDir = true;
					//_GetDefaultDir( Loc.strLocation, &strDefaultDir, &Loc.strLocationPath, &bIsDir );
					i32 iResult = Loc.pLoader->Init( Loc.strLocation );
				}

				if( Loc.pLoader->IsReady() )
				{
					const xst_astring& s = LocItr->first;
		
					GroupPtr pGroup = this->GetOrCreateGroup( s/*LocItr->first*/ );
					
					Resources::CDirectory::FileMap mFiles;
					if( Loc.bPhysical )
					{
						mFiles = Loc.pLoader->GetFileInfos( Loc.strLocation, Loc.bRecursive );
					}
					else
					{
						mFiles = Loc.pLoader->GetFileInfos( Loc.strLocationPath, Loc.bRecursive );
					}

					Resources::CDirectory::FileMap::iterator Itr = mFiles.begin();
					for(; Itr != mFiles.end(); ++Itr)
					{
						Res File = { Loc.pLoader, Itr->second };
						//Check if this file extension is supported
						strExt = XST::IFile::GetFileExtension( File.pFile->GetName() );
						if( IsExtensionSupported( strExt ) )
						{
							pGroup->AddResource( Itr->first, File );
						}
					}
				}
			}
		}

		return RESULT::OK;
	}

	bool CFileManager::IsExtensionSupported(xst_castring& strExt)
	{
		if( !m_bAllFilesSupported && m_bExtensionFilter )
		{
			xst_map< xst_astring, i32 >::iterator Itr = m_mExts.find( strExt );
			return Itr != m_mExts.end();
		}

		return true;
	}

	bool CFileManager::IsFileTypeSupported(xst_castring& strFileName)
	{
		return IsExtensionSupported( XST::IFile::GetFileExtension( strFileName ) );
	}

	i32 CFileManager::_GetDefaultDir(xst_castring& strLocation, xst_astring* strDirOut, xst_astring* strPathOut, bool* bDirOut)
	{
		*strDirOut = *strPathOut = "";

		// If physical directory does not exists on the hard disk search it in the virtual file system
		if( XST::IFile::IsDirectoryExists( strLocation ) )
		{
			*strDirOut = strLocation;
			*bDirOut = true;
		}
		else
		{
			//Find first '.' character
			//Note that any virtual file systems should have an extension
			u32 uiDotPos = strLocation.find_first_of( "." );
			if( uiDotPos == xst_astring::npos )
			{
				XST_LOG_ERR( "This is no virtual file system directory/file" );
				return RESULT::FAILED;
			}

			//Now find the end of the extension
			u32 uiSlashPos = strLocation.find_first_of( "\\/", uiDotPos );
			//If no slash/backslash found
			if( uiSlashPos == xst_astring::npos )
			{
				*strDirOut = strLocation;
			}
			else
			{
				*strDirOut = strLocation.substr( 0, uiSlashPos );
				*strPathOut = strLocation.substr( uiSlashPos + 1 );
			}

			*bDirOut = false;
		}

		return RESULT::OK;
	}

	i32 CFileManager::PrepareGroup(xst_castring& strGroupName)
	{
		GroupPtr pGroup = GetGroup( strGroupName );
		if( pGroup.IsNull() )
		{
			XST_LOG( "Group name: " << strGroupName << " does not exists" );
			return RESULT::FAILED;
		}

		///*XST::TCResourceManager< Resources::FilePtr >::*/GroupIterator Itr = this->m_mResources.begin();
		//for(; Itr != this->m_mResources.end(); ++Itr)
		//{

		//}

		return RESULT::OK;
	}

	i32 CFileManager::RegisterLoader(xst_castring &strLoaderName, XST::IFileLoader *pLoader)
	{
		if( pLoader == xst_null )
		{
			XST_LOG( "Loader named: " << strLoaderName << " is null" );
			return RESULT::FAILED;
		}


		LoaderMap::iterator Itr = m_mLoaders.find( strLoaderName );
		if( Itr != m_mLoaders.end() )
		{
			XST_LOG( "Loader named: " << strLoaderName << " already exists" );
			return RESULT::FAILED;
		}

		pLoader->m_strLoaderName = strLoaderName;
		m_mLoaders.insert( LoaderMap::value_type( strLoaderName, pLoader ) );

		return RESULT::OK;
	}

	XST::IFileLoader* CFileManager::GetFileLoader(xst_castring& strName) const
	{
		LoaderMap::const_iterator Itr = m_mLoaders.find( strName );
		if( Itr == m_mLoaders.end() )
		{
			XST_LOG_ERR( "No file loader named: " << strName << " found in the file manager" );
			return xst_null;
		}

		return Itr->second;
	}

	Resources::FilePtr CFileManager::LoadFile(xst_castring& strDirPath, xst_castring& strFileName, xst_castring& strLoaderName, xst_castring& strGroupName)
	{
		Resources::FilePtr pNull;

		i32 iListenerResult = 0;
		for(u32 i = 0; i < m_vListeners.size(); ++i )
		{
			xst_assert( m_vListeners[ i ], "(CFileManager::LoadFile) Listener is null" );
			iListenerResult += m_vListeners[ i ]->OnBeforeLoad( strFileName, strGroupName );
		}

		if( iListenerResult > 0 )
		{
			return pNull;
		}

		GroupPtr pGr = this->GetOrCreateGroup( strGroupName );
		if( pGr.IsNull() )
		{
			XST_LOG_ERR( "No group: " << strGroupName << " found in the file manager" );
			return pNull;
		}

		Resource File;
		if( pGr->GetResource( strFileName, &File ) == RESULT::OK )
		{
			//If this file is in resource group load it
			if( File.pFile.IsNull() )
			{
				XST_LOG_ERR( "File: " << strFileName << " does not exists in group: " << strGroupName << " in the file manager" );
				return pNull;
			}

			if( File.pFile->GetData().IsLoaded() )
			{
				return File.pFile;
			}

			File.pFile = File.pLoader->Load( File.pFile->GetPath(), File.pFile->GetName() );

			return File.pFile;
		}

		//If this file is not present in a resource group get loader and try to load it
		XST::IFileLoader* pLoader = GetFileLoader( strLoaderName );
		if( pLoader == xst_null )
		{
			return pNull;
		}

		xst_stringstream ssFullPath;
		ssFullPath << strDirPath << "/" << strFileName;

		File.pFile = pLoader->Load( ssFullPath.str() );
		if( File.pFile != xst_null )
		{
			File.pLoader = pLoader;
			File.pFile->m_strName = strFileName;
			File.pFile->m_strPath = strDirPath;
			pGr->AddResource( strFileName, File );
		}

		for(u32 i = 0; i < m_vListeners.size(); ++i )
		{
			m_vListeners[ i ]->OnAfterLoad( File.pFile );
		}

		return File.pFile;
	}

	Resources::FilePtr CFileManager::LoadFile(xst_castring &strFileName, xst_castring &strGroupName)
	{
		Resources::FilePtr pNull;

		i32 iListenerResult = 0;
		for(u32 i = 0; i < m_vListeners.size(); ++i )
		{
			xst_assert( m_vListeners[ i ], "(CFileManager::LoadFile) Listener is null" );
			iListenerResult += m_vListeners[ i ]->OnBeforeLoad( strFileName, strGroupName );
		}

		if( iListenerResult > 0 )
		{
			return pNull;
		}

		GroupPtr pGr = this->GetGroup( strGroupName );
		if( pGr.IsNull() )
		{
			XST_LOG_ERR( "No group: " << strGroupName << " found in the file manager" );
			return pNull;
		}

		Resource File;
		if( pGr->GetResource( strFileName, &File ) != RESULT::OK )
		{
			XST_LOG_ERR( "File: " << strFileName << " does not exists in group: " << strGroupName << " in the file manager" );
			return pNull;
		}

		if( File.pFile.IsNull() )
		{
			XST_LOG_ERR( "File: " << strFileName << " does not exists in group: " << strGroupName << " in the file manager" );
			return pNull;
		}

		if( File.pFile->GetData().IsLoaded() )
		{
			return File.pFile;
		}

		File.pFile = File.pLoader->Load( File.pFile->GetPath(), File.pFile->GetName() );

		for(u32 i = 0; i < m_vListeners.size(); ++i )
		{
			m_vListeners[ i ]->OnAfterLoad( File.pFile );
		}

		return File.pFile;
	}

	Resources::FilePtr CFileManager::LoadFile(xst_castring &strFileName)
	{
		Resources::FilePtr pNull;

		i32 iListenerResult = 0;
		xst_castring strGroupName( "" );
		for(u32 i = 0; i < m_vListeners.size(); ++i )
		{
			xst_assert( m_vListeners[ i ], "(CFileManager::LoadFile) Listener is null" );
			iListenerResult += m_vListeners[ i ]->OnBeforeLoad( strFileName, strGroupName );
		}

		if( iListenerResult > 0 )
		{
			return pNull;
		}

		GroupPtr pGr;
		Resource File;
		for(_GroupIterator Itr = this->m_mResources.begin(); Itr != this->m_mResources.end(); ++Itr)
		{
			pGr = Itr->second;
			if( pGr->GetResource( strFileName, &File ) == RESULT::OK )
			{
				break;
			}
		}

		if( File.pFile.IsNull() )
		{
			XST_LOG_ERR( "File: " << strFileName << " does not exists in any group" );
			return pNull;
		}

		if( File.pFile->GetData().IsLoaded() )
		{
			return File.pFile;
		}

		File.pFile = File.pLoader->Load( File.pFile->GetPath(), File.pFile->GetName() );

		for(u32 i = 0; i < m_vListeners.size(); ++i )
		{
			m_vListeners[ i ]->OnAfterLoad( File.pFile );
		}

		return File.pFile;
	}

	i32 CFileManager::RemoveListener(XST::IFileListener *pListener)
	{
		ListenerVector::iterator Itr = std::find( m_vListeners.begin(), m_vListeners.end(), pListener );
		if( Itr == m_vListeners.end() )
		{
			return RESULT::FAILED;
		}

		m_vListeners.erase( Itr );
		return RESULT::OK;
	}

}//xst
