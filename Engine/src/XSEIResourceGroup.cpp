#include "XSEIResourceGroup.h"

namespace XSE
{
    #define XSE_HASH(_string) XST::CHash::GetCRC( (_string).c_str(), (_string).length() )

    IResourceGroup::IResourceGroup(xst_castring& strName, const Handle& Handle) :
        m_strName( strName )
        ,m_Handle( Handle )
    {

    }

    IResourceGroup::~IResourceGroup()
    {
        for( auto& Itr : m_mResources )
        {
            XST::CDebug::PrintDebugLN( Itr.second->GetResourceName() + ", " + std::to_string( Itr.second->GetRefCount() ) );
        }
        m_mResources.clear();
    }

    void IResourceGroup::ForEachResource(ResourceCallback Callback)
    {
        for( auto& Itr : m_mResources )
        {
            Callback( Itr.second );
        }
    }

    i32 IResourceGroup::AddResource(xst_castring& strName, ResourcePtr pRes)
    {
        m_pTmpStr = strName.c_str();
        ResourceHandle Handle = XSE_HASH( strName );
        return AddResource( Handle, pRes );
    }

    i32 IResourceGroup::AddResource(const ResourceHandle& Handle, ResourcePtr pRes)
    {
        if( m_LastUsedHandle == Handle )
        {
            XST_LOG_ERR( "Resource: " << m_pTmpStr << " already exists in group: " << GetName() );
            m_pTmpStr = "";
            return XST_FAIL;
        }

        ResMapItr Itr;
        if( !XST::MapUtils::FindPlace( m_mResources, Handle, &Itr ) )
        {
            XST_LOG_ERR( "Resource: " << m_pTmpStr << " already exists in group: " << GetName() );
            m_pTmpStr = "";
            return XST_FAIL;
        }

        XST::MapUtils::InsertOnPlace( m_mResources, Handle, pRes, Itr );
        pRes->m_ResourceGroupHandle = GetHandle();
        pRes->m_ResourceHandle = Handle;
        m_pTmpStr = "";
        return XST_OK;
    }
    
    ResourcePtr IResourceGroup::GetResource(xst_castring& strName)
    {
        m_pTmpStr = strName.c_str();
        ResourceHandle Handle = XSE_HASH( strName );
        return GetResource( Handle );
    }

    ResourcePtr IResourceGroup::GetResource(const ResourceHandle& Handle)
    {
        if( m_LastUsedHandle == Handle )
            return m_pLastUsedResource;
        ResMapItr Itr = m_mResources.find( Handle );
        if( Itr == m_mResources.end() )
        {
            XST_LOG_ERR( "Resource: " << m_pTmpStr << " does not exists in group: " << GetName() );
            m_pTmpStr = "";
            return ResourcePtr();
        }
        m_LastUsedHandle = Handle;
        m_pLastUsedResource = Itr->second;
        return m_pLastUsedResource;
    }

    ResourcePtr IResourceGroup::RemoveResource(xst_castring& strName)
    {
        m_pTmpStr = strName.c_str();
        ResourceHandle Handle = XSE_HASH( strName );
        return RemoveResource( Handle );
    }

    ResourcePtr IResourceGroup::RemoveResource(const ResourcePtr pRes)
    {
        m_pTmpStr = pRes->GetResourceName().c_str();
        return RemoveResource( pRes->GetResourceHandle() );
    }
    
    ResourcePtr IResourceGroup::RemoveResource(const ResourceHandle& Handle)
    {
        ResMapItr Itr = m_mResources.find( Handle );
        if( Itr == m_mResources.end() )
        {
            XST_LOG_ERR( "Resource: " << m_pTmpStr << " does not exists in group: " << GetName() );
            m_pTmpStr = "";
            return ResourcePtr();
        }

        ResourcePtr pRes( Itr->second );
        m_mResources.erase( Itr );

        m_pTmpStr = "";
        return pRes;
    }

    i32 IResourceGroup::DestroyResource(xst_castring& strName)
    {
        m_pTmpStr = strName.c_str();
        ResourceHandle Handle = XSE_HASH( strName );
        return DestroyResource( Handle );
    }

    i32 IResourceGroup::DestroyResource(ResourcePtr pRes)
    {
        m_pTmpStr = pRes->GetResourceName().c_str();
        return DestroyResource( pRes->GetResourceHandle() );
    }

    i32 IResourceGroup::DestroyResource(const ResourceHandle& Handle)
    {
        ResourcePtr pRes = RemoveResource( Handle );
        if( !pRes )
            return XST_FAIL;
        return XST_OK;
    }

} // XSE
