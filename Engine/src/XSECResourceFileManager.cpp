#include "../include/XSECResourceFileManager.h"

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

	CResourceFileManager::CResourceFileManager(XST::CFileManager* pFileMgr)
	{
		m_pFileMgr = pFileMgr;//XST::CFileManager::GetSingletonPtr();
		m_aResTypes.reserve( 20 );
	}

	CResourceFileManager::~CResourceFileManager()
	{
	}

	i32	CResourceFileManager::SetManager( i32 iResourceType, IResourceManager* pMgr )
	{
		g_mResTypeMgr[ iResourceType ] = pMgr;
		return XST_OK;
	}
					
	i32	CResourceFileManager::AddExtension( xst_castring& strExt, i32 iResourceType )
	{
		g_mExtResType[ strExt ] = iResourceType;
		return XST_OK;
	}
	
	i32	CResourceFileManager::SetLoadingOrder( i32 iResourceType, i32 iLoadingOrder )
	{
		g_mResTypeLoadingOrder[ iResourceType ] = iLoadingOrder;
		return XST_OK;
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