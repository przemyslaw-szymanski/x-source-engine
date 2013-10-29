#include "../include/XSECResourceFileManager.h"

namespace XSE
{
	CResourceFileManager::CResourceFileManager(XST::CFileManager* pFileMgr)
	{
		m_pFileMgr = pFileMgr;//XST::CFileManager::GetSingletonPtr();
		m_aResTypes.reserve( 20 );
	}

	CResourceFileManager::~CResourceFileManager()
	{
	}

	i32	CResourceFileManager::AddExtension(xst_castring& strExt, i32 iResourceType, i32 iLoadingOrder, IResourceManager* pMgr)
	{
		SExt Ext = { strExt, pMgr, iLoadingOrder };

		//ExtList::iterator Itr = m_lExts.begin();

		/*if( iLoadingOrder > (i32)m_lExts.size() || iLoadingOrder < 0 )
		{
			Itr = m_lExts.end();
		}
		else
		{
			for(i32 i = 0; i < iLoadingOrder; ++i, ++Itr);
		}

		m_lExts.insert( Itr, Ext );*/
		m_aResTypes.push_back( Ext );

		m_pFileMgr->AddFileExtension( strExt );

		return RESULT::OK;
	}

	i32	CResourceFileManager::AddLocation(xst_castring& strDirectory, xst_castring& strGroupName, xst_castring& strLoaderName, bool bRecursive)
	{
		return m_pFileMgr->AddLocation( strDirectory, strGroupName, strLoaderName, bRecursive );
	}

	i32	CResourceFileManager::PrepareGroup(xst_castring& strName)
	{
		return m_pFileMgr->PrepareGroup( strName );
	}

	i32	CResourceFileManager::PrepareAllGroups()
	{
		if( m_pFileMgr->PrepareAllGroups() != RESULT::OK )
		{
			return RESULT::FAILED;
		}

		//Get all shader groups
		

		return RESULT::OK;
	}

	i32	CResourceFileManager::RegisterLoader(xst_castring& strLoaderName, XST::IFileLoader* pLoader)
	{
		return m_pFileMgr->RegisterLoader( strLoaderName, pLoader );
	}

	XST::FilePtr	CResourceFileManager::LoadFile(xst_castring& strFileName, xst_castring& strGroupName)
	{
		return m_pFileMgr->LoadFile( strFileName, strGroupName );
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


}//xse