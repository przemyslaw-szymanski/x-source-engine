#ifndef XST_IRESOURCE_GROUP_H
#define XST_IRESOURCE_GROUP_H

#include "XSEIResource.h"

namespace XSE
{

    class IResourceManager;

    class IResourceListener
	{
		public:

			virtual				~IResourceListener() {}

			virtual	void		ResourceCreated(ResourcePtr pRes) {}

		protected:
	};

    class IResourceGroupListener
    {
        public:

            virtual             ~IResourceGroupListener()
                                {}
    };

    class XST_API IResourceGroup : public XST::IObject
    {
        friend class CEngine;
        friend class IResourceManager;

        public:

        typedef ul32                                            Handle;
        typedef Resources::IResource::Handle                    ResourceHandle;
        typedef xst_hash_table< ResourceHandle, ResourcePtr >   ResourceMap;
        typedef ResourceMap::iterator                           ResMapItr;
        typedef ResourceMap::const_iterator                     ResMapConstItr;
        typedef xst_vector< IResourceGroupListener* >           ListenerVec;
        typedef std::function< void ( ResourcePtr ) >      ResourceCallback;

        public:

        IResourceGroup( xst_castring& strName, const Handle& Handle );
        virtual             ~IResourceGroup();

        virtual i32         AddResource( xst_castring& strName, ResourcePtr pRes );
        virtual i32         AddResource( const ResourceHandle& Handle, ResourcePtr pRes );

        virtual ResourcePtr   GetResource( xst_castring& strName );

        virtual ResourcePtr  GetResource( const ResourceHandle& ulResourceHandle );

        virtual ResourcePtr      RemoveResource( xst_castring& strName );
        virtual ResourcePtr      RemoveResource( const ResourceHandle& ulResourceHandle );
        virtual ResourcePtr      RemoveResource( ResourceWeakPtr pRes );

        i32      DestroyResource( xst_castring& strName );
        i32      DestroyResource( const ResourceHandle& ulResourceHandle );
        i32      DestroyResource( ResourceWeakPtr pRes );

        virtual xst_i ResourceHandle    GetHandle() const
                                        { return m_Handle; }

        virtual xst_i xst_castring&     GetName() const
                                        { return m_strName; }

        virtual void        ForEachResource(ResourceCallback Callback);

        protected:

        xst_astring     m_strName;
        ResourceMap     m_mResources;
        ul32            m_Handle = XSE_INVALID_HANDLE;
        lpcastr         m_pTmpStr = "";
        ResourcePtr m_pLastUsedResource;
        ResourceHandle  m_LastUsedHandle = XSE_INVALID_HANDLE;
    };

} // XSE

#endif // XST_IRESOURCE_GROUP_H