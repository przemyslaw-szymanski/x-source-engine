#include "../include/XSECResourceFileManager.h"
#include "FileSystems/XSECFileSystem.h"
#include "FileSystems/XSECVirtualFileSystem.h"
#include "XSECFile.h"

namespace XSE
{
	xst_map<i32, IResourceManager*> g_mResTypeMgr;
	xst_map<xst_castring, i32> g_mExtResType;
	xst_map<i32, i32> g_mResTypeLoadingOrder;

	IResourceManager* GetMgrByResType(i32 iResType)
	{ return g_mResTypeMgr[ iResType ]; }

	i32 GetResTypeByExt( xst_castring& strExt )
	{ return g_mExtResType[ strExt ]; }

	IResourceManager* GetMgrByExt( xst_castring& strExt )
	{
		i32 iResType = GetResTypeByExt( strExt );
		return GetMgrByResType( iResType );
	}

	i32 CResourceFileManager::CGroup::AddFileInfo(xst_castring& strFullPath, ul32 uFileSize)
	{
		u32 uPos = strFullPath.find_last_of("\\/");
		xst_astring strDirPath = strFullPath.substr(0, uPos + 1);
		u32 uPos2 = strFullPath.find_last_of(".");
		xst_astring strFileName = strFullPath.substr( uPos+1, strFullPath.length() - uPos );
		xst_astring strExt = strFullPath.substr( uPos2+1, uPos2 - strFullPath.length() );
		return AddFileInfo( strDirPath.c_str(), strDirPath.length(), strFileName.c_str(), strFileName.length(),
							strExt.c_str(), strExt.length(), uFileSize );
	}

	i32 CResourceFileManager::CGroup::AddFileInfo(const IFileSystem::SFileInfo& Info)
	{
		u32 uLen = Info.uPathLength - Info.uNameLength;
		lpcastr strName = Info.strPath + Info.uPathLength - Info.uNameLength;
		lpcastr strExt = Info.strPath + Info.uPathLength - Info.uExtLength + 1;
		return AddFileInfo(	Info.strPath, Info.uPathLength, strName, Info.uNameLength, strExt, Info.uExtLength, Info.uFileSize );
	}

	i32 CResourceFileManager::CGroup::AddFileInfo(	lpcastr strPath, u32 uPathSize, lpcastr strName, u32 uNameSize,
													lpcastr strExt, u32 uExtSize, ul32 uFileSize)
	{
		SFileInfo Info;
		xst_zero( &Info, sizeof( SFileInfo ) );
		Info.uNameHash = XST::CHash::GetCRC( strName );
		auto& Itr = m_mFileInfos.find( Info.uNameHash );
		if( Itr == m_mFileInfos.end() )
		{
			ch8 aExt[ 10 ];
			lpastr pTmp = aExt;
			XST::StringUtil::ToLower( strExt, uExtSize, &pTmp );
			Info.uPathHash = XST::CHash::GetCRC( strPath, uPathSize );
			Info.uExtHash = XST::CHash::GetCRC( aExt, uExtSize );
			u32 uOffset = (m_aNames.size() == 0) ? 0 : m_aNames.size() - 1;
			Info.uBuffHandle = m_aNames.size();
			Info.uPathLen = uPathSize;
			Info.uNameLen = uNameSize;
			Info.uExtLen = uExtSize;
			if( uFileSize == 0 )
				uFileSize = m_pFS->GetFileSize( strPath, uPathSize );
			Info.uFileSize = uFileSize;
			m_aNames.push_back_range( strPath, uPathSize );
			m_aNames.push_back( 0 );
			m_aNames.push_back_range( strName, uNameSize );
			m_aNames.push_back( 0 );
			m_aNames.push_back_range( strExt, uExtSize );
			m_aNames.push_back( 0 );
			m_mFileInfos.insert( std::make_pair( Info.uNameHash, Info ) );
			return XST_OK;
		}
		return XST_FAIL;
	}

	i32 CResourceFileManager::CGroup::GetFileInfo(ResFileWeakPtr pFile, SFileInfo* pOut) const
	{
		auto& Itr = m_mFileInfos.find( pFile->m_ulHandle );
		if( Itr != m_mFileInfos.end() )
			*pOut = Itr->second;
		else
			return XST_FAIL;
		return XST_OK;
	}

	i32 CResourceFileManager::CGroup::Prepare()
	{
		// Get file infos from a file system
		xst_assert2( m_pFS );
		IFileSystem::FileInfoVec vFiles;
		vFiles.reserve( 100 );

		for( u32 i = 0; i < m_vLocations.size(); ++i )
		{
			auto& Loc = m_vLocations[ i ];
			m_pFS->GetFileInfos( Loc.strDir, Loc.bRecursive, &vFiles );
		}

		for( u32 i = 0; i < vFiles.size(); ++i )
		{
			auto& Info = vFiles[ i ];
			AddFileInfo( Info );
		}
		m_bPrepared = true;
		return XST_OK;
	}

	lpcastr	CResourceFileManager::CGroup::GetFileName(const SFileInfo& Info) const
	{
		lpcastr s = &m_aNames[ Info.uBuffHandle + Info.uPathLen + 1 ];
		return s;
	}

	lpcastr	CResourceFileManager::CGroup::GetFileExt(const SFileInfo& Info) const
	{
		lpcastr s = &m_aNames[ Info.uBuffHandle + Info.uPathLen + Info.uNameLen + 2 ];
		return s;
	}

	lpcastr CResourceFileManager::CGroup::GetFilePath( const SFileInfo& Info ) const
	{
		lpcastr s = &m_aNames[ Info.uBuffHandle ];
		return s;
	}

	ResFileWeakPtr CResourceFileManager::CGroup::LoadFile(ul32 uHandle, u8** ppOut)
	{
		auto& Itr = m_mFiles.find( uHandle );
		if( Itr == m_mFiles.end() )
		{
			auto& InfoItr = m_mFileInfos.find( uHandle );
			if( InfoItr != m_mFileInfos.end() )
			{
				bool bSharedData;
				auto& Info = InfoItr->second;
				u8* pData = xst_null;
				if( ppOut && *ppOut )
				{
					pData = *ppOut;
					bSharedData = true;
				}
				else
				{
					pData = xst_new u8[ Info.uFileSize + 1 ]; // +1 for null termination
					bSharedData = false;
				}
				lpcastr pFilePath = GetFilePath( Info );
				if( XST_FAILED( m_pFS->LoadFile( pFilePath, Info.uPathLen, Info.uFileSize, &pData ) ) )
				{
					return ResFilePtr();
				}
				//XST::FilePtr pFile( xst_new XST::Resources::CFile( strName, pFilePath, &pData, Info.uFileSize, false ) );
				ResFilePtr pFile( xst_new Resources::CFile() );
				if( pFile.IsValid() )
				{
					Resources::CFile* pTmp = pFile.GetPtr();
					if( bSharedData )
						pTmp->m_Data.SetSharedData( pData, Info.uFileSize, true );
					else
						pTmp->m_Data.Move(&pData, Info.uFileSize, true );
					
					pTmp->m_ulInfoHandle = uHandle;
					pTmp->m_ulHandle = uHandle;
					pTmp->m_ulGroupHandle = m_uHash;
					m_mFiles.insert( std::make_pair( uHandle, pFile ) );
					return pFile;
				}
				else
				{
					if( !m_pTmpName ) m_pTmpName = "";
					XST_LOG_ERR( "Unable to create file pointer for file: " << m_pTmpName );
					m_pTmpName = xst_null;
					return ResFileWeakPtr();
				}
			}
			else
			{
				if( !m_pTmpName ) m_pTmpName = "";
				XST_LOG_ERR( "Unable to find file info for file: " << m_pTmpName );
				m_pTmpName = xst_null;
				return ResFileWeakPtr();
			}
		}
		else
		{
			return Itr->second;
		}
		return ResFileWeakPtr();
	}

	ResFileWeakPtr CResourceFileManager::CGroup::LoadFile(xst_castring& strName, u8** ppOut)
	{
		u32 uHandle = XST::CHash::GetCRC( strName );
		m_pTmpName = strName.c_str();
		return LoadFile( uHandle, ppOut );
	}
			
	i32	CResourceFileManager::CGroup::Load(FileVec* pOut, bool bSharedMemory, u8** ppOut)
	{
		return XST_FAIL;
	}

	void CResourceFileManager::CGroup::_DestroyFileData(Resources::CFile* pFile)
	{
		xst_assert2( pFile );
		pFile->m_Data.Delete();
	}

	void CResourceFileManager::CGroup::Destroy()
	{
		for( auto& Pair : m_mFiles )
		{
			//_DestroyFileData( Pair.second.GetPtr() );
		}

		m_mFiles.clear();
		m_mFileInfos.clear();
	}

	i32	CResourceFileManager::CGroup::DestroyFile(xst_castring& strName)
	{
		ul32 uHash = XST::CHash::GetCRC( strName );
		//return DestroyFile( uHash );
		return 0;
	}
			
	i32	CResourceFileManager::CGroup::DestoryFileByHandle(ul32 uHandle)
	{
		auto& Itr = m_mFiles.find( uHandle );
		if( Itr != m_mFiles.end() )
		{
			//Resources::CFile* pFile = Itr->second.GetPtr();
			//_DestroyFileData( pFile );
			m_mFiles.erase( Itr );
			return XST_OK;
		}
		XST_LOG_ERR( "Unable to find file with handle: " << uHandle );
		return XST_FAIL;
	}
	
	i32	CResourceFileManager::CGroup::DestroyFile(ResFilePtr pFile)
	{
		//return DestroyFile( pFile->GetHandle() );
		return 0;
	}

	CResourceFileManager::CResourceFileManager(XST::CFileManager* pFileMgr)
	{
		RegisterFileSystem( "FileSystem", xst_new CFileSystem(), true );
		m_pFileMgr = pFileMgr;//XST::CFileManager::GetSingletonPtr();
		m_aResTypes.reserve( 20 );
	}

	CResourceFileManager::~CResourceFileManager()
	{
		for( auto& Pair : m_mFileSystems )
		{
			if( Pair.second.bAutoDestroy )
				xst_delete( Pair.second.pSys );
		}
		m_mFileSystems.clear();
	}

	i32	CResourceFileManager::SetManager(i32 iResourceType, IResourceManager* pMgr)
	{
		g_mResTypeMgr[ iResourceType ] = pMgr;
		return XST_OK;
	}
					
	i32	CResourceFileManager::AddExtension(xst_castring& strExt, i32 iResourceType)
	{
		g_mExtResType[ strExt ] = iResourceType;
		return XST_OK;
	}
	
	i32	CResourceFileManager::SetLoadingOrder(i32 iResourceType, i32 iLoadingOrder)
	{
		g_mResTypeLoadingOrder[ iResourceType ] = iLoadingOrder;
		return XST_OK;
	}

	IFileSystem* CResourceFileManager::RegisterFileSystem(xst_castring& strName, IFileSystem* pSys, bool bAutoDestroy)
	{
		u32 uHash = XST::CHash::GetCRC( strName );
		auto& Itr = m_mFileSystems.find( uHash );
		if( Itr == m_mFileSystems.end() )
		{
			SFileSystem FS = { pSys, bAutoDestroy };
			m_mFileSystems[ uHash ] = FS;
			return FS.pSys;
		}
		return Itr->second.pSys;
	}

	IFileSystem* CResourceFileManager::GetFileSystem(xst_castring& strName) const
	{
		u32 uHash = XST::CHash::GetCRC( strName );
		auto& Itr = m_mFileSystems.find( uHash );
		if( Itr != m_mFileSystems.end() )
			return Itr->second.pSys;
		return xst_null;
	}

	i32	CResourceFileManager::AddExtension(xst_castring& strExt, i32 iResourceType, i32 iLoadingOrder, IResourceManager* pMgr)
	{
		SExt Ext = { strExt, pMgr, iLoadingOrder };
		m_aResTypes.push_back( Ext );

		AddExtension( strExt, iResourceType );
		SetLoadingOrder( iResourceType, iLoadingOrder );
		SetManager( iResourceType, pMgr );

		m_pFileMgr->AddFileExtension( strExt );

		return RESULT::OK;
	}

	CResourceFileManager::GroupWeakPtr CResourceFileManager::GetOrCreateGroup(xst_castring& strName)
	{
		ul32 uHash = XST::CHash::GetCRC( strName );
		auto& Itr = m_mGroups.find( uHash );
		if( Itr != m_mGroups.end() )
		{
			return Itr->second;
		}
		GroupPtr pGr( xst_new CGroup( strName, uHash ) );
		m_mGroups[ uHash ] = pGr;
		return pGr;
	}

	CResourceFileManager::GroupWeakPtr CResourceFileManager::GetGroup(xst_castring& strName) const
	{
		ul32 uHash = XST::CHash::GetCRC( strName );
		auto& Itr = m_mGroups.find( uHash );
		if( Itr != m_mGroups.end() )
		{
			return Itr->second;
		}
		XST_LOG_ERR( "Group: '" << strName << "' does not exits" );
		return GroupWeakPtr();
	}

	CResourceFileManager::GroupWeakPtr CResourceFileManager::GetGroup(ul32 uHandle) const
	{
		auto& Itr = m_mGroups.find( uHandle );
		if( Itr != m_mGroups.end() )
		{
			return Itr->second;
		}
		XST_LOG_ERR( "Group with handle: '" << uHandle << "' does not exits" );
		return GroupWeakPtr();
	}

	XST::IFileLoader* CResourceFileManager::_GetLoader( xst_castring& strName )
	{
		u32 uHash = XST::CHash::GetCRC( strName );
		return m_mLoaders[ uHash ];
	}

	i32	CResourceFileManager::AddLocation(xst_castring& strDirectory, xst_castring& strGroupName, xst_castring& strLoaderName, bool bRecursive)
	{
		IFileSystem* pFS = GetFileSystem( strLoaderName );
		xst_assert2( pFS );

		GroupWeakPtr pGr = GetOrCreateGroup( strGroupName );
		pGr->SetFileSystem( pFS );
		pGr->AddLocation( strDirectory, true );

		return m_pFileMgr->AddLocation( strDirectory, strGroupName, strLoaderName, bRecursive );
	}

	i32	CResourceFileManager::PrepareGroup(xst_castring& strName)
	{
		GroupWeakPtr pGr = GetOrCreateGroup( strName );
		i32 iResult = pGr->Prepare();
		return m_pFileMgr->PrepareGroup( strName );
	}

	i32	CResourceFileManager::PrepareAllGroups()
	{
		if( m_pFileMgr->PrepareAllGroups() != RESULT::OK )
		{
			return RESULT::FAILED;
		}

		// TODO: make prepare async
		for( auto& Pair : m_mGroups )
		{
			Pair.second->Prepare();
		}

		// Wait for prepares
		bool bAllPrepared = false;
		while( !bAllPrepared )
		{
			bAllPrepared = true;
			for( auto& Pair : m_mGroups )
			{
				if( !Pair.second->IsPrepared() )
					bAllPrepared = false;
			}
		}
		//Get all shader groups

		return RESULT::OK;
	}

	i32	CResourceFileManager::RegisterLoader(xst_castring& strLoaderName, XST::IFileLoader* pLoader)
	{
		u32 uHash = XST::CHash::GetCRC( strLoaderName );
		auto& Itr = m_mLoaders.find( uHash );
		if( Itr == m_mLoaders.end() )
			m_mLoaders[ uHash ] = pLoader;
		else
		{
			XST_LOG_ERR( "File loader: '" << strLoaderName << "' already registered." );
			return XST_FAIL;
		}
		return m_pFileMgr->RegisterLoader( strLoaderName, pLoader );
	}

	XST::FilePtr CResourceFileManager::LoadFile(xst_castring& strFileName, ul32 uGroupHandle)
	{
		GroupWeakPtr pGr = GetGroup( uGroupHandle );
		ResFilePtr pFile = pGr->LoadFile( strFileName, xst_null );
		//return pFile;
		return XST::FilePtr();
	}

	XST::FilePtr CResourceFileManager::LoadFile(xst_castring& strFileName, xst_castring& strGroupName)
	{
		return m_pFileMgr->LoadFile( strFileName, strGroupName );
	}

	ResFileWeakPtr CResourceFileManager::LoadFile2(xst_castring& strFileName, ul32 uGroupHandle)
	{
		GroupWeakPtr pGr = GetGroup( uGroupHandle );
		ResFileWeakPtr pFile = pGr->LoadFile( strFileName, xst_null );
		//return pFile;
		return pFile;
	}

	ResFileWeakPtr CResourceFileManager::LoadFile2(xst_castring& strFileName, xst_castring& strGroupName)
	{
		ul32 uHash = XST::CHash::GetCRC( strFileName );
		if( strGroupName == ALL_GROUPS )
		{
			for( auto& Pair : m_mGroups )
			{
				ResFileWeakPtr pFile = Pair.second->LoadFile( uHash, xst_null );
				if( pFile.IsValid() )
					return pFile;
			}
		}
		else
		{
			GroupWeakPtr pGr = GetGroup( strGroupName );
			ResFileWeakPtr pFile = pGr->LoadFile( strFileName, xst_null );
			return pFile;
		}
		return ResFileWeakPtr();;
	}

	XST::FilePtr CResourceFileManager::LoadFromFileSystem(xst_castring& strDirPath, xst_castring& strFileName, xst_castring& strGroupName)
	{
		return m_pFileMgr->LoadFile( strDirPath, strFileName, "FileSystem", strGroupName );
	}

	void CResourceFileManager::AddListener(XST::IFileListener* pListener) 
	{ 
		m_pFileMgr->AddListener( pListener );
	}

	i32	CResourceFileManager::RemoveListener(XST::IFileListener* pListener)
	{
		return m_pFileMgr->RemoveListener( pListener );
	}

	i32	CResourceFileManager::DestroyFile(ResFilePtr pFile)
	{
		xst_assert2( pFile.IsValid() );
		ul32 uHandle = pFile->m_ulGroupHandle;
		GroupWeakPtr pGr = GetGroup( uHandle );
		xst_assert2( pGr.IsValid() );
		return pGr->DestroyFile( pFile );
	}

	i32 CResourceFileManager::DestroyFile(XST::FilePtr pFile)
	{
		return m_pFileMgr->DestroyResource( pFile->GetName() );
	}
					
	i32	CResourceFileManager::DestroyFile(ul32 uHandle, xst_castring& strGroupName)
	{
		GroupWeakPtr pGr = GetGroup( strGroupName );
		xst_assert2( pGr.IsValid() );
		return pGr->DestoryFileByHandle( uHandle );
	}
		
	i32	CResourceFileManager::DestroyFile(ul32 uHandle, ul32 uGroupHandle)
	{
		GroupWeakPtr pGr = GetGroup( uGroupHandle );
		xst_assert2( pGr.IsValid() );
		return pGr->DestoryFileByHandle( uHandle );
	}
		
	i32	CResourceFileManager::DestroyFile(xst_castring& strName, xst_castring& strGroup)
	{
		GroupWeakPtr pGr = GetGroup( strGroup );
		xst_assert2( pGr.IsValid() );
		return pGr->DestroyFile( strName );
	}

	i32	CResourceFileManager::GetFileInfo(ResFileWeakPtr pFile, SFileInfo* pOut)
	{
		xst_assert2( pFile.IsValid() );
		xst_assert2( pOut );
		GroupWeakPtr pGr = GetGroup( pFile->m_ulGroupHandle );
		xst_assert2( pGr.IsValid() );
		return pGr->GetFileInfo( pFile, pOut );
	}

}//xse