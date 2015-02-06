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

	i32 CResourceFileManager::CGroup::AddFileInfo(xst_castring& strFullPath)
	{
		u32 uPos = strFullPath.find_last_of("\\/");
		xst_astring strDirPath = strFullPath.substr(0, uPos + 1);
		u32 uPos2 = strFullPath.find_last_of(".");
		xst_astring strFileName = strFullPath.substr( uPos+1, strFullPath.length() - uPos );
		xst_astring strExt = strFullPath.substr( uPos2+1, uPos2 - strFullPath.length() );
		return AddFileInfo( strDirPath, strFileName, strExt );
	}

	i32 CResourceFileManager::CGroup::AddFileInfo(xst_castring& strDirPath, xst_castring& strName, xst_castring& strExt)
	{
		SFileInfo Info;
		Info.uNameHash = XST::CHash::GetCRC( strName );
		auto& Itr = m_mFileInfos.find( Info.uNameHash );
		if( Itr == m_mFileInfos.end() )
		{
			Info.uPathHash = XST::CHash::GetCRC( strDirPath );
			Info.uExtHash = XST::CHash::GetCRC( strExt );
			u32 uOffset = (m_aNames.size() == 0) ? 0 : m_aNames.size() - 1;
			Info.pBuff = &m_aNames[ uOffset ];
			Info.uPathLen = strDirPath.length();
			Info.uNameLen = strName.length();
			Info.uExtLen = strExt.length();
			m_aNames.push_back_range( strDirPath.c_str(), strDirPath.length() + 1 );
			m_aNames.push_back_range( strName.c_str(), strName.length() + 1 );
			m_aNames.push_back_range( strExt.c_str(), strExt.length() + 1 );
			m_mFileInfos[ Info.uNameHash ] = Info;
			return XST_OK;
		}
		return XST_FAIL;
	}

	lpcastr	CResourceFileManager::CGroup::GetFileDirPath(const SFileInfo& Info)
	{
		return Info.pBuff;
	}

	lpcastr	CResourceFileManager::CGroup::GetFileName(const SFileInfo& Info)
	{
		return Info.pBuff + 1 + Info.uPathLen;
	}

	lpcastr	CResourceFileManager::CGroup::GetFileExt(const SFileInfo& Info)
	{
		return Info.pBuff + 1 + Info.uPathLen + 1 + Info.uNameLen;
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

	CResourceFileManager::GroupWeakPtr CResourceFileManager::GetOrCreateGroup( xst_castring& strName )
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

	i32	CResourceFileManager::AddLocation(xst_castring& strDirectory, xst_castring& strGroupName, xst_castring& strLoaderName, bool bRecursive)
	{
		GroupWeakPtr pGr = GetOrCreateGroup( strGroupName );
		pGr->AddFileInfo( strDirectory );
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