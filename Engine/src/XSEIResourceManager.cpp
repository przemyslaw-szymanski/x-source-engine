#include "XSEIResourceManager.h"
#include "XSTCHash.h"
#include "XSEIResourceLoader.h"
#include "XSECResourceFileManager.h"

namespace XSE
{

#define XSE_HASH(_string) XST::CHash::GetCRC( (_string).c_str(), (_string).length() )
#define XSE_NULLRES ResourcePtr()

    IResourceManager::GroupHandle g_DefaultGroupHandle = XSE_HASH( DEFAULT_GROUP );

    namespace Resources
    {
        ResourcePtr IResource::CloneResource(xst_castring& strName /* = XST::StringUtil::EmptyAString */, bool bFullClone /* = true */)
        {
            return this->m_pResourceCreator->CloneResource( this, strName, bFullClone );
        }

        i32 IResource::ClearResource()
        {
            m_ResourceHandle = 0;
            m_ResourceGroupHandle = 0;
#if (XST_OBJ_DEBUG)
            m_strName.clear();
#endif
            m_pResourceCreator = xst_null;
            m_iResourceState = XST::ResourceStates::UNKNOWN;
            m_iResourceType = XST::ResourceType::UNKNOWN;
            return XST_OK;
        }
    }//Resources

    IResourceManager::IResourceManager()
    {
        //m_pFileMgr = CFileManager::GetSingletonPtr();
        m_pResFileMgr = CResourceFileManager::GetSingletonPtr();
    }

    IResourceManager::~IResourceManager()
    {
        m_pResFileMgr = xst_null;
    }

    void IResourceManager::Destroy()
    {
        _OnBeforeResourcesDestroyed();
        m_mGroups.clear();
        _OnDestroy();
        for( auto& Pair : m_mLoaders )
        {
            if( !Pair.second->IsManualDestroy() )
                xst_delete( Pair.second );
        }
        m_mLoaders.clear();
        m_bDestroyed = true;
    }

    i32 IResourceManager::Init()
    {
        m_bDestroyed = false;
        xst_assert( m_pResFileMgr, "(IResourceManager::Init) File manager singleton not created" );
        return XST_OK;
    }

    void IResourceManager::ForEachResource(IResourceManager::ResourceCallback Callback)
    {
        for( auto& GroupItr : m_mGroups )
        {
            for( auto& ResItr : GroupItr.second->m_mResources )
            {
                Callback( ResItr.second, GroupItr.second );
            }
        }
    }

    void IResourceManager::ForEachGroup(IResourceManager::GroupCallback Callback)
    {
        for( auto& GroupItr : m_mGroups )
        {
            //Callback( GroupItr.second );
        }
    }

    IResourceManager::GroupWeakPtr IResourceManager::GetGroup(const GroupHandle& Handle)
    {
        if( m_LastUsedGroupHandle == Handle )
            return m_pLastUsedGroup;

        auto Itr = m_mGroups.find( Handle );
        if( Itr == m_mGroups.end() ) // if not found this group return null
        {
            XST_LOG_ERR( "Requested resource group: " << m_pStrTmp << "(" << Handle << ") is not created" );
            m_LastUsedGroupHandle = 0;
            return GroupWeakPtr();
        }
        
        m_LastUsedGroupHandle = Handle;
        m_pLastUsedGroup = Itr->second;
        m_pStrTmp = "";
        return m_pLastUsedGroup;
    }

    IResourceManager::GroupWeakPtr IResourceManager::GetGroup(xst_castring& strName)
    {
        GroupHandle Handle = XSE_HASH( strName );
        m_pStrTmp = strName.c_str();
        return GetGroup( Handle );
    }

    IResourceManager::GroupWeakPtr IResourceManager::CreateGroup(xst_castring& strName)
    {
        // Check for existing
        GroupHandle Handle = XSE_HASH( strName );
        if( m_LastUsedGroupHandle == Handle )
        {
            XST_LOG_ERR( "Group with name: " << strName << " already exists" );
            return GroupWeakPtr();
        }

        ResGroupMapItr Itr;
        XST::MapUtils::FindPlace( m_mGroups, Handle, &Itr );
        if( Itr != m_mGroups.end() ) // group exists
        {
            XST_LOG_ERR( "Group with name: " << strName << " already exists" );
            return GroupWeakPtr();
        }

        GroupPtr pGroup( _CreateGroup( strName, Handle ) );
        if( !pGroup )
        {
            XST_LOG_ERR( "Unable to allocate memory for resource group: " << strName );
            return GroupWeakPtr();
        }

        XST::MapUtils::InsertOnPlace( m_mGroups, Handle, pGroup, Itr );
        m_LastUsedGroupHandle = Handle;
        m_pLastUsedGroup = pGroup;
        return m_pLastUsedGroup;
    }

    IResourceManager::GroupWeakPtr IResourceManager::GetOrCreateGroup(xst_castring& strName)
    {
        GroupHandle Handle = XSE_HASH( strName );
        if( m_LastUsedGroupHandle == Handle && m_pLastUsedGroup.IsValid() )
            return m_pLastUsedGroup;

        m_LastUsedGroupHandle = Handle;
        ResGroupMapItr Itr;
        if( !XST::MapUtils::FindPlace( m_mGroups, Handle, &Itr ) ) // if resource exists
        {
            m_pLastUsedGroup = Itr->second; // Return already found group pointer
            return m_pLastUsedGroup;
        }

        GroupPtr pGroup( _CreateGroup( strName, Handle ) );
        if( pGroup.IsNull() )
        {
            XST_LOG_ERR( "Unable to allocate memory for resource group: " << strName );
            return GroupWeakPtr();
        }

        //XST::MapUtils::InsertOnPlace( m_mGroups, Handle, pGroup, Itr );
        m_mGroups.insert( Itr, ResGroupMap::value_type( Handle, pGroup ) );

        m_pLastUsedGroup = pGroup;
        m_LastUsedGroupHandle = Handle;
        return m_pLastUsedGroup;
    }

    IResourceGroup* IResourceManager::_CreateGroup(xst_castring& strName, const IResourceManager::GroupHandle& Handle)
    {
        return xst_new IResourceGroup( strName, Handle );
    }

    ResourceWeakPtr IResourceManager::CreateResource(xst_castring& strName, xst_castring& strGroupName)
    {
        GroupWeakPtr pGr = this->GetOrCreateGroup( strGroupName );
        m_pStrTmp = strGroupName.c_str();
        return CreateResource( strName, pGr );
    }

    ResourceWeakPtr IResourceManager::CreateResource(xst_castring& strName, GroupWeakPtr pGroup)
    {
        xst_assert( pGroup.IsValid(), "(IResourceManager::CreateResource) Group is null" );
        ResourceHandle Handle; 
        Resources::IResource* pBaseRes;
        //ResourcePtr pRes;
        {
            //XSTSimpleProfiler2("IResourceManager2::GetHash"); //~0.000008sec in debug
            Handle = XSE_HASH( strName );
        }
        {
            //XSTSimpleProfiler2("IResourceManager2::_CreateResource"); //~0.003sec in debug
        pBaseRes = _CreateResource( strName, Handle, pGroup );
       // pRes = ResourcePtr( pBaseRes );
        if( pBaseRes == xst_null )
        {
            return XSE_NULLRES;
        }
        }

        pBaseRes->m_ResourceGroupHandle = pGroup->GetHandle();
        //pBaseRes->m_strResourceName = strName;
        pBaseRes->m_ResourceHandle = Handle;
        ResourceWeakPtr pRes( pBaseRes );
        pBaseRes->RemoveRef();
        { 
            //XSTSimpleProfiler2( "CreateResource::AddResourceByHandle" ); //~0.0007sec in debug
        if( pGroup->AddResource( Handle, pRes ) != RESULT::OK )
        {
            XST_LOG_ERR( "Resource: '" << strName << "' already exists in group: " << pGroup->GetName() );
            return XSE_NULLRES;
        }
        }
        //XSE_DO_LISTENER_VOID_METHOD( ResourceCreated, ListenerList::iterator, m_lListeners, pRes );

        return pRes;
    }

    ResourceWeakPtr IResourceManager::GetOrCreateResource(xst_castring &strName, xst_castring &strGroupName, bool* pbCreatedOut)
    {
        GroupPtr pGr;
        {
            //XSTSimpleProfiler2( "---GetOrCreateGroup" );
        pGr	= this->GetOrCreateGroup( strGroupName );
        }
        GroupWeakPtr p( pGr );
        return GetOrCreateResource( strName, pGr, pbCreatedOut );
    }

    ResourceWeakPtr IResourceManager::GetOrCreateResource(xst_castring &strName, GroupWeakPtr pGroup, bool* pbCreatedOut)
    {
        // Disable log warnings
        XST_LOGGER_DISABLE();
        ResourceWeakPtr pRes;
        {
            //XSTSimpleProfiler2( "---GetResource" ); // ~0.0000250415957326 in debug, map find ~0.0000180627903645
        pRes	= pGroup->GetResource( strName );
        }
        XST_LOGGER_ENABLE();
        if( pRes.IsValid() )
        {
            // If resource with given name already exists return it
            if( pbCreatedOut ) *pbCreatedOut = false;
            return pRes;
        }
        
        //If not exists create it
        { 
            //XSTSimpleProfiler2( "---CreateResource" ); // ~0.0047267038240159 in debug
        pRes = CreateResource( strName, pGroup );
        }
        
        if( pbCreatedOut ) *pbCreatedOut = true;

        return pRes;
    }

    ResourceWeakPtr	IResourceManager::GetResource(const ResourceHandle& ResHandle, const IResourceManager::GroupHandle& GroupHandle)
    {
        GroupWeakPtr pGr = GetGroup( GroupHandle );
        if( pGr.IsNull() )
            return ResourceWeakPtr();
        return pGr->GetResource( ResHandle );
    }

    ResourceWeakPtr IResourceManager::GetResource(xst_castring &strName, xst_castring &strGroup)
    {
        ResourcePtr pRes;
        ResourceHandle Handle = XSE_HASH( strName );

        if( strGroup == ALL_GROUPS )
        {
            for( auto& Itr : m_mGroups )
            {
                pRes = Itr.second->GetResource( Handle );
                if( pRes.IsValid() )
                    return pRes;
            }
        }
        else
        {
            GroupWeakPtr pGr = GetGroup( strGroup );
            if( pGr.IsValid() )
            {
                pRes = pGr->GetResource( Handle );
            }
        }

        return pRes;
    }

    ResourceWeakPtr	IResourceManager::GetResource(xst_castring& strName, GroupWeakPtr pGroup)
    {
        xst_assert( pGroup.IsValid(), "(IResourceManager::GetResource) Group is null" );
        return pGroup->GetResource( strName );
    }

    ResourceWeakPtr IResourceManager::PrepareResource(xst_castring &strName, GroupWeakPtr pGroup)
    {
        xst_assert( pGroup.IsValid(), "(IResourceManager::PrepareResource) Group is null" );
        ResourceWeakPtr pRes = pGroup->GetResource( strName );
        if( !pRes ) 
        {
            return XSE_NULLRES;
        }

        if( XST_FAILED( PrepareResource( &pRes ) ) )
        {
            return XSE_NULLRES;
        }

        return pRes;
    }

    ResourceWeakPtr IResourceManager::PrepareResource(xst_castring &strName, xst_castring &strGroupName)
    {
        return PrepareResource( strName, GetGroup( strGroupName ) );
    }

    ResourceWeakPtr IResourceManager::LoadResource(xst_castring &strName, xst_castring &strGroupName, bool bFullLoad)
    {
        return LoadResource( strName, strGroupName, strName, strGroupName, bFullLoad );
    }

    ResourceWeakPtr IResourceManager::LoadResource(xst_castring& strResName, xst_castring& strResGroupName, xst_castring &strFileName, xst_castring &strFileGroupName, bool bFullLoad)
    {
        xst_assert( m_pResFileMgr, "(IResourceManager::LoadResource) File manager not created" );
        
        ResourceHandle ResHandle = CalcHandle( strResName.c_str(), strResName.length() );
        GroupHandle GrHandle = CalcHandle( strResGroupName.c_str(), strResName.length() );
        ResourceWeakPtr pRes = GetResource( ResHandle, GrHandle );
        if( pRes.IsValid() )
        {
            return pRes;
        }

        pRes = CreateResource( strResName, strResGroupName );
        if( XST_FAILED( LoadResource( &pRes, strFileName, strFileGroupName, bFullLoad ) ) )
        {
            DestroyResource( pRes );
            pRes = XSE_NULLRES;
        }
        return pRes;
    }

    i32	IResourceManager::LoadResource(ResourceWeakPtr *const ppRes, xst_castring& strGroupName, bool bFullLoad)
    {
        xst_assert2( ppRes && ( *ppRes ).IsValid() );
        return LoadResource( ppRes, (*ppRes)->GetResourceName(), strGroupName, bFullLoad );
    }

    i32	IResourceManager::LoadResource(ResourceWeakPtr *const ppRes, xst_castring& strFileName, xst_castring& strGroupName, bool bFullLoad)
    {
        xst_assert( m_pResFileMgr, "(IResourceManager::LoadResource) File manager not created" );
        xst_assert2( (*ppRes).IsValid() );

        Resources::IResource* pRes = (*ppRes).GetPtr();
        
        //XST::FilePtr pFile = m_pResFileMgr->LoadFile( strFileName, strGroupName );
        ResFileWeakPtr pFile2 = m_pResFileMgr->LoadFile2( strFileName, strGroupName );
        if( pFile2.IsNull() /*&& pFile.IsNull()*/ )
        {
            XST_LOG_ERR("Unable to load resource: " << strFileName << " in group: " << strGroupName);
            return XST_FAIL;
        }
        pRes->m_pResourceFile = pFile2;

        //pRes->_SetResourceFile( pFile );
        //pRes->_SetResourceFile( pFile2 );
        pRes->_SetResourceState( XST::ResourceStates::LOADED );

        if( bFullLoad )
        {
            if( XST_FAILED( PrepareResource( ppRes ) ) )
            {
                return XST_FAIL;
            }
        }

        return XST_OK;
    }

    ResourceWeakPtr IResourceManager::CloneResource(const Resources::IResource* pSrcRes, xst_castring& strNewName, bool bFullClone)
    {
        m_ssTmpName.str( "" );
        if( strNewName.empty() )
            m_ssTmpName << XST::CTime::GetQPerfTickCount() << "_cloned";
        else
            m_ssTmpName << strNewName;

        GroupWeakPtr pGroup = this->GetGroup( pSrcRes->GetResourceGroupHandle() );
        xst_assert( pGroup != xst_null, "(IResourceManager::CloneResource)" );
        ResourcePtr pNewRes = CreateResource( m_ssTmpName.str(), pGroup );
        if( pNewRes == xst_null )
        {
            return pNewRes; 
        }

        /*if( XST_FAILED( PrepareResource( pNewRes ) ) )
        {
            this->DestroyResource( pNewRes->GetResourceGroupHandle() );
            return ResourcePtr();
        }*/

        Resources::IResource* pTmp = pNewRes.GetPtr();
        if( XST_FAILED( _CloneResource( &pTmp, pSrcRes, bFullClone ) ) )
        {
            this->DestroyResource( pNewRes->GetResourceGroupHandle() );
            return ResourceWeakPtr();
        }

        return pNewRes;
    }


    /*i32 IResourceManager::CreateMemoryPool(void* pMemMgr)
    {
        xst_assert( m_pMemoryMgr == xst_null, "(IResourceManager::CreateMemoryManager) Memory manager is already created" );
        m_pMemoryMgr = _CreateMemoryManager( pMemMgr );
        if( !m_pMemoryMgr )
        {
            XST_LOG_ERR( "Memory manager creation failed" );
            return XST_FAIL;
        }
        return XST_OK;
    }

    XST::IAllocator* IResourceManager::_CreateMemoryManager(void* pMemMgr)
    {
        return xst_new XST::CDefaultAllocator();
    }

    void IResourceManager::_DestroyMemoryManager(XST::IAllocator** ppMemMgr)
    {
        xst_delete( *ppMemMgr );
    }*/

    i32 IResourceManager::AddResource( xst_castring& strName, ResourceWeakPtr pRes, xst_castring& strGroupName, bool bCreateGroup )
    {
        ResourceHandle Handle = XSE_HASH( strName );
        return AddResource( Handle, pRes, strGroupName, bCreateGroup );
    }
     
    i32 IResourceManager::AddResource( xst_castring& strName, ResourceWeakPtr pRes, GroupWeakPtr pGr )
    {
        ResourceHandle Handle = XSE_HASH( strName );
        return AddResource( Handle, pRes, pGr );
    }
     
    i32 IResourceManager::AddResource(const ResourceHandle& Handle, ResourceWeakPtr pRes, xst_castring& strGroupName, bool bCreateGroup)
    {
        GroupWeakPtr pGroup;
        if( bCreateGroup )
            pGroup = GetOrCreateGroup( strGroupName );
        else
            pGroup = GetGroup( strGroupName );

        if( !pGroup )
            return XST_FAIL;

        return AddResource( Handle, pRes, pGroup );
    }
     
    i32 IResourceManager::AddResource(const ResourceHandle& Handle, ResourceWeakPtr pRes, const GroupHandle& GroupHandle )
    {
        return AddResource( Handle, pRes, GetGroup( GroupHandle ) );
    }

    i32 IResourceManager::AddResource(const ResourceHandle& Handle, ResourceWeakPtr pRes, IResourceManager::GroupWeakPtr pGroup)
    {
        return pGroup->AddResource( Handle, pRes );
    }


    IResourceManager::GroupPtr IResourceManager::RemoveGroup( xst_castring& strName )
    {
        GroupHandle Handle = XSE_HASH( strName );
        return RemoveGroup( Handle );
    }
        
    IResourceManager::GroupPtr IResourceManager::RemoveGroup(const GroupHandle& Handle)
    {
        ResGroupMapItr Itr = m_mGroups.find( Handle );
        if( Itr == m_mGroups.end() )
        {
            return GroupPtr();
        }

        GroupPtr pGroup( Itr->second );
        m_mGroups.erase( Itr );
        return pGroup;
    }
        
    IResourceManager::GroupPtr IResourceManager::RemoveGroup( const GroupWeakPtr pGroup )
    {
        return RemoveGroup( pGroup->GetHandle() );
    }

    i32 IResourceManager::DestroyGroup( xst_castring& strName )
    {
        GroupHandle Handle = XSE_HASH( strName );
        return DestroyGroup( Handle );
    }
       
    i32 IResourceManager::DestroyGroup( const GroupHandle& Handle )
    {
        GroupPtr pGroup = RemoveGroup( Handle );
        if( !pGroup )
            return XST_FAIL;
        return XST_OK;
    }
        
    i32 IResourceManager::DestroyGroup( const GroupWeakPtr pGroup )
    {
        return DestroyGroup( pGroup->GetHandle() );
    }

    i32 IResourceManager::MoveResources( xst_castring& strSrcGroup, xst_castring& strDstGroup )
    {
        GroupWeakPtr pSrc = GetGroup( strSrcGroup );
        if( !pSrc )
            return XST_FAIL;
        
        GroupWeakPtr pDst = GetOrCreateGroup( strDstGroup );
        if( !pDst )
            return XST_FAIL;

        pDst->m_mResources.insert( pSrc->m_mResources.begin(), pSrc->m_mResources.end() );
        pSrc->m_mResources.clear();
        return XST_OK;
    }

    i32 IResourceManager::MoveResource(xst_castring& strResName, xst_castring& strSrcGroup, xst_castring& strDstGroup)
    {
        GroupWeakPtr pSrc = GetGroup( strSrcGroup );
        if( !pSrc )
            return XST_FAIL;
        
        ResourcePtr pRes = pSrc->GetResource( strResName );
        if( !pRes )
            return XST_FAIL;

        GroupWeakPtr pDst = GetOrCreateGroup( strDstGroup );
        if( !pDst )
            return XST_FAIL;

        return MoveResource( pRes, pSrc, pDst );
    }
        
    i32 IResourceManager::MoveResource(xst_castring& strResName, GroupWeakPtr pSrc, GroupWeakPtr pDst)
    {
        xst_assert( pSrc != xst_null, "(IResourceManager::MoveResource) Should not be null" );
        ResourcePtr pRes = pSrc->GetResource( strResName );
        if( !pRes )
            return XST_FAIL;
        return MoveResource( pRes, pSrc, pDst );
    }
        
    i32 IResourceManager::MoveResource(ResourcePtr pRes, GroupWeakPtr pSrc, GroupWeakPtr pDst)
    {
        xst_assert( pRes != xst_null, "(IResourceManager::MoveResource) Should not be null" );
        xst_assert( pSrc != xst_null, "(IResourceManager::MoveResource) Should not be null" );
        xst_assert( pDst != xst_null, "(IResourceManager::MoveResource) Should not be null" );

        ResourcePtr pResource = pSrc->RemoveResource( pRes );
        if( !pResource )
            return XST_FAIL;
        if( XST_FAILED( pDst->AddResource( pResource->GetResourceHandle(), pResource ) ) )
            return XST_FAIL;
        return XST_OK;
    }

    ResourcePtr IResourceManager::RemoveResource(xst_castring& strName)
    {
        ResourceHandle Handle = XSE_HASH( strName );
        return RemoveResource( Handle );
    }
        
    ResourcePtr IResourceManager::RemoveResource(const ResourceHandle& Handle)
    {
        for( auto& GrItr : m_mGroups )
        {
            auto ResItr = GrItr.second->m_mResources.find( Handle );
            if( ResItr != GrItr.second->m_mResources.end() )
            {
                ResourcePtr pRes( ResItr->second );
                GrItr.second->m_mResources.erase( ResItr );
                return pRes;
            }
        }

        return XSE_NULLRES;
    }

    ResourcePtr IResourceManager::RemoveResource( xst_castring& strName, xst_castring& strGroup )
    {
        ResourceHandle Handle = XSE_HASH( strName );
        GroupHandle GrHandle = XSE_HASH( strGroup );
        return RemoveResource( Handle, GrHandle );
    }

    ResourcePtr IResourceManager::RemoveResource(const ResourceHandle& Handle, const GroupHandle& GroupHandle)
    {
        GroupWeakPtr pGr = GetGroup( GroupHandle );
        if( pGr.IsValid() )
            return pGr->RemoveResource( Handle );
        return XSE_NULLRES;
    }
        
    ResourcePtr IResourceManager::RemoveResource(ResourceWeakPtr pRes)
    {
        xst_assert( pRes.IsValid(), "(IResourceManager::RemoveResource) Resource is null" );
        GroupWeakPtr pGroup = GetGroup( pRes->m_ResourceGroupHandle );
        xst_assert( pGroup.IsValid(), "(IResourceManager::RemoveResource) Invalid resource group handle" );
        return pGroup->RemoveResource( pRes );
    }
        
    i32 IResourceManager::DestroyResource(xst_castring& strName)
    {
        if( RemoveResource( strName ).IsValid() )
            return XST_OK;
        return XST_FAIL;
    }
        
    i32 IResourceManager::DestroyResource(const ResourceHandle& Handle)
    {
        if( RemoveResource( Handle ).IsValid() )
            return XST_OK;
        return XST_FAIL;
    }
        
    i32 IResourceManager::DestroyResource(ResourceWeakPtr pRes)
    {
        if( RemoveResource( pRes ).IsValid() )
            return XST_OK;
        return XST_FAIL;
    }

    i32 IResourceManager::DestroyResource( xst_castring& strName, xst_castring& strGroup )
    {
        if( RemoveResource( strName, strGroup ).IsValid() )
            return XST_OK;
        return XST_FAIL;
    }
       
    i32 IResourceManager::DestroyResource( const ResourceHandle& Handle, const GroupHandle& GroupHandle )
    {
        if( RemoveResource( Handle, GroupHandle ).IsValid() )
            return XST_OK;
        return XST_FAIL;
    }

    i32 IResourceManager::ManageUnusedResources()
    {
        // Clear resources and move them to unused buffer
        i32 iResult = XST_OK;
        ForEachResource( [&] (ResourceWeakPtr pRes, GroupWeakPtr pGroup) 
        {
            if( pRes->GetRefCount() > 1 ) // branch prediction, most of resources should be used (ref count > 1 )
                return;
            else // if this is the last reference = it exists and is used only in its group
            {
                ul32 ulHandle = pRes->GetResourceHandle();
                if( pRes->ClearResource() == XST_OK )
                    m_sUnusedResources.push( pGroup->RemoveResource( ulHandle ) );
                else
                    iResult = XST_FAIL;
            }
        });
        return iResult;
    }

    ResourcePtr IResourceManager::GrabUnusedResource()
    {
        if( m_sUnusedResources.size() > 0 )
            return m_sUnusedResources.top_pop();
        return ResourcePtr();
    }

    bool IResourceManager::IsUnusedResourceAvailable()
    {
        return m_sUnusedResources.size() > 0;
    }

    IResourceManager::Hash IResourceManager::CalcHandle(lpcastr strString, u32 uLen) const
    {
        return XST::CHash::GetCRC( strString, uLen );
    }

    i32 IResourceManager::RegisterLoader(xst_castring& strFileExtension, LoaderPtr pLoader)
    {
        ul32 uId = XST::CHash::GetCRC( strFileExtension );
        ResLoaderMap::iterator Itr = m_mLoaders.find( uId );
        if( Itr != m_mLoaders.end() )
        {
            if( Itr->second && !Itr->second->IsManualDestroy() )
            {
                xst_delete( Itr->second );
                m_mLoaders.erase( Itr );
            }
            else
            {
                XST_LOG_ERR( "Unable to register the resource loader. Resource loader for files: " << strFileExtension << " is already registered and is set for manual destroy" );
                return XST_FAIL;
            }
        }
        m_mLoaders[uId] = pLoader;
        return XST_OK;
    }

    IResourceManager::LoaderPtr IResourceManager::GetLoader(xst_castring& strFileExtension)
    {
        ul32 uId = XST::CHash::GetCRC( strFileExtension );
        ResLoaderMap::iterator Itr = m_mLoaders.find( uId );
        if( Itr == m_mLoaders.end() )
            return xst_null;
        return Itr->second;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////

}//xse
