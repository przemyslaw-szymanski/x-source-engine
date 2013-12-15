#include "../include/XSEIResourceManager.h"

namespace XSE
{
	xst_castring IResourceManager::ALL_GROUPS = "";
	xst_castring IResourceManager::DEFAULT_GROUP = "Default";

	namespace Resources
	{
		ResourcePtr IResource::Clone(xst_castring& strName /* = XST::StringUtil::EmptyAString */, bool bFullClone /* = true */)
		{
			return this->m_pResourceCreator->CloneResource( this, strName, bFullClone );
		}
	}//Resources

	IResourceManager::IResourceManager() : m_pMemoryMgr( xst_null ), m_pFileMgr( xst_null )
	{
		/*if( !XST::CFileManager::IsSingletonCreated() )
		{
			m_pFileMgr = xst_new XST::CFileManager();
		}
		else
		{
			m_pFileMgr = XST::CFileManager::GetSingletonPtr();
		}*/

		m_pFileMgr = XST::CFileManager::GetSingletonPtr();
	}

	IResourceManager::~IResourceManager()
	{
		DestroyListeners();
		xst_delete( this->m_pMemoryMgr );
		this->m_pMemoryMgr = xst_null;
	}

	void IResourceManager::DestroyListeners()
	{
		ListenerList::iterator Itr;
		for(Itr = m_lListeners.begin(); Itr != m_lListeners.end(); ++Itr)
		{
			xst_delete( (*Itr) );
		}

		m_lListeners.clear();
	}

	void IResourceManager::RemoveListener(XSE::IResourceManager::Listener* pListener, bool bDestroy)
	{
		m_lListeners.remove( pListener );
		if( bDestroy ) xst_delete( pListener );
	}

	XST::IAllocator& IResourceManager::GetMemoryPool()
	{
		xst_assert( m_pMemoryMgr, "(IResourceManager::GetMemoryPool) Memory Manager not created" );
		return *m_pMemoryMgr;
	}

	ResourcePtr IResourceManager::CloneResource(const Resources::IResource* pSrcRes, xst_castring& strNewName /* = XST::StringUtil::EmptyAString */, bool bFullClone /* = true */)
	{
		m_ssTmpName.str( "" );
		if( strNewName.empty() )
			m_ssTmpName << XST::CTime::GetQPerfTickCount() << "_cloned";
		else
			m_ssTmpName << strNewName;

		GroupPtr pGroup = this->GetGroupByHandle( pSrcRes->m_ulResourceGroupId );
		xst_assert( pGroup != xst_null, "(IResourceManager::CloneResource)" );
		ResourcePtr pNewRes = CreateResource( m_ssTmpName.str(), pGroup );
		if( pNewRes == xst_null )
		{
			return pNewRes; 
		}

		if( XST_FAILED( PrepareResource( pNewRes ) ) )
		{
			this->DestroyResourceByHandle( pNewRes->m_ulResourceHandle );
			return ResourcePtr();
		}



		return pNewRes;
	}

	ResourcePtr IResourceManager::CreateResource(xst_castring &strName, xst_castring &strGroupName)
	{
		GroupPtr pGr = this->GetOrCreateGroup( strGroupName );
		return CreateResource( strName, pGr );
	}

	ResourcePtr IResourceManager::CreateResource(xst_castring& strName, IResourceManager::GroupPtr pGroup)
	{
		ul32 ulHandle; 
		Resources::IResource* pBaseRes;
		{
			//XSTSimpleProfiler2("IResourceManager::GetHash"); //~0.000008sec in debug
			ulHandle = this->GetHash( strName );
		}
		{
			//XSTSimpleProfiler2("IResourceManager::_CreateResource"); //~0.003sec in debug
		pBaseRes = _CreateResource( strName, ulHandle, pGroup );
		if( pBaseRes == xst_null )
		{
			return m_pNullRes;
		}
		}

		pBaseRes->m_ulResourceGroupId = pGroup->GetGroupHandle();
		//pBaseRes->m_strResourceName = strName;
		pBaseRes->m_ulResourceHandle = ulHandle;

		ResourcePtr pRes( pBaseRes );
		{ 
			//XSTSimpleProfiler2( "CreateResource::AddResourceByHandle" ); //~0.0007sec in debug
		if( pGroup->AddResourceByHandle( ulHandle, pRes ) != RESULT::OK )
		{
			pRes = xst_null;
			XST_LOG_ERR( "Resource: " << strName << " already exists in group: " << pGroup->GetGroupName() );
			return m_pNullRes;
		}
		}
		//XSE_DO_LISTENER_VOID_METHOD( ResourceCreated, ListenerList::iterator, m_lListeners, pRes );

		return pRes;
	}

	ResourcePtr IResourceManager::GetOrCreateResource(xst_castring &strName, xst_castring &strGroupName, bool* pbCreatedOut)
	{
		GroupPtr pGr = this->GetOrCreateGroup( strGroupName );
		return GetOrCreateResource( strName, pGr, pbCreatedOut );
	}

	ResourcePtr IResourceManager::GetOrCreateResource(xst_castring &strName, IResourceManager::GroupPtr pGroup, bool* pbCreatedOut)
	{
		ResourcePtr pRes;
		bool bCreated = false;

		if( !XST_FAILED( pGroup->GetResource( strName, &pRes ) ) )
		{
			//If resource with given name already exists return it
			if( pbCreatedOut ) *pbCreatedOut = bCreated;
			return pRes;
		}
		
		//If not exists create it
		{ 
			//XSTSimpleProfiler2( "ResourceGroup::CreateResource" );
		pRes = CreateResource( strName, pGroup );
		}
		bCreated = true;
		
		if( pbCreatedOut ) *pbCreatedOut = bCreated;

		return pRes;
	}

	ResourcePtr IResourceManager::LoadResource(xst_castring &strName, xst_castring &strGroupName)
	{
		xst_assert( m_pFileMgr, "File manager not created" );
		XST::FilePtr pFile;
		if( strGroupName == ALL_GROUPS )
		{
			pFile = m_pFileMgr->LoadFile( strName );
		}
		else
		{
			pFile = m_pFileMgr->LoadFile( strName, strGroupName );
		}

		if( pFile.IsNull() )
		{
			return m_pNullRes;
		}

		ResourcePtr pRes = GetOrCreateResource( strName, strGroupName );
		if( pRes.IsNull() )
		{
			return m_pNullRes;
		}

		pRes->m_pResourceFile = pFile;
		pRes->_SetResourceFile( pFile );

		return pRes;
	}

	ResourcePtr IResourceManager::LoadResource(xst_castring &strFileName, xst_castring& strResName, xst_castring &strGroupName)
	{
		xst_assert( m_pFileMgr, "File manager not created" );

		ResourcePtr pRes = GetOrCreateResource( strResName, strGroupName );
		if( pRes.IsNull() )
		{
			return m_pNullRes;
		}

		XST::FilePtr pFile;
		if( strGroupName == ALL_GROUPS )
		{
			pFile = m_pFileMgr->LoadFile( strFileName );
		}
		else
		{
			pFile = m_pFileMgr->LoadFile( strFileName, strGroupName );
		}

		if( pFile.IsNull() )
		{
			return m_pNullRes;
		}

		pRes->m_pResourceFile = pFile;
		pRes->_SetResourceFile( pFile );

		return pRes;
	}

	i32	IResourceManager::LoadResource(ResourcePtr pRes, xst_castring& strGroupName)
	{
		return LoadResource( pRes, pRes->GetResourceName(), strGroupName );
	}

	i32	IResourceManager::LoadResource(ResourcePtr pRes, xst_castring& strFileName, xst_castring& strGroupName)
	{
		xst_assert( m_pFileMgr, "File manager not created" );
		xst_assert2( pRes != xst_null );

		if( pRes.IsNull() )
		{
			XST_LOG_ERR( "Resource is null" );
			return XST_FAIL;
		}

		XST::FilePtr pFile;
		if( strGroupName == ALL_GROUPS )
		{
			pFile = m_pFileMgr->LoadFile( strFileName );
		}
		else
		{
			pFile = m_pFileMgr->LoadFile( strFileName, strGroupName );
		}

		if( pFile.IsNull() )
		{
			return XST_FAIL;
		}

		pRes->m_pResourceFile = pFile;
		pRes->_SetResourceFile( pFile );

		return XST_OK;
	}


	/*i32 IResourceManager::LoadResource(ResourcePtr& pRes)
	{
		xst_assert( m_pFileMgr, "File manager not created" );
		xst_assert2( pRes != xst_null );

		GroupPtr pGr = GetOrCreateGroup( pRes->m_ulResourceGroupId );

		XST::FilePtr pFile;
		if( pGr->GetGroupName() == ALL_GROUPS )
		{
			pFile = m_pFileMgr->LoadFile( pGr->GetGroupName() );
		}
		else
		{
			pFile = m_pFileMgr->LoadFile( pRes->m_strResourceName, pGr->GetGroupName() );
		}

		if( pFile.IsNull() )
		{
			return XST_FAIL;
		}

		pRes->_SetResourceFile( pFile );

		return XST_OK;
	}*/

	ResourcePtr IResourceManager::GetResource(xst_castring &strName, xst_castring &strGroup)
	{
		ResourcePtr pRes;

		if( strGroup == ALL_GROUPS )
		{
			_GroupIterator GroupItr = this->m_mResources.begin();
			for(; GroupItr != this->m_mResources.end(); ++GroupItr)
			{
				if( GroupItr->second->GetResource( strName, &pRes ) == RESULT::OK )
				{
					return pRes;
				}
			}
		}
		else
		{
			GroupPtr pGr = this->GetGroup( strGroup );
			if( !pGr.IsNull() )
			{
				pGr->GetResource( strName, &pRes );
			}
		}

		return pRes;
	}


	ResourcePtr	IResourceManager::GetResource(xst_castring& strName, GroupPtr pGroup)
	{
		if( pGroup.IsNull() )
		{
			XST_LOG_ERR( "Group is null" );
			return ResourcePtr();
		}

		ResourcePtr pRes;
		if( XST_FAILED( pGroup->GetResource( strName, &pRes ) ) )
		{
			XST_LOG_ERR( "Resource: " << strName << " not found" );
			return ResourcePtr();
		}

		return pRes;
	}

	ResourcePtr IResourceManager::PrepareResource(XST::Types::xst_castring &strName, XST::Types::xst_castring &strGroupName)
	{
		ResourcePtr pRes = GetResource( strName, strGroupName );
		if( pRes.IsNull() ) 
		{
			return ResourcePtr();
		}
		if( XST_FAILED( PrepareResource( pRes ) ) )
		{
			return ResourcePtr();
		}

		return pRes;
	}

}//xse
