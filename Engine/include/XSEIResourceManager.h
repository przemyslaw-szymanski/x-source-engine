#ifndef XSE_IRESOURCEMANAGER_H
#define XSE_IRESOURCEMANAGER_H

#include "XSEIResource.h"
#include "XSEIResourceGroup.h"

namespace XSE
{
    class IResourceGroup;

    class XST_API IResourceManager
    {
        friend class IResourceGroup;
        public:

            static xst_castring IResourceManager::ALL_GROUPS;
	        static xst_castring IResourceManager::DEFAULT_GROUP;

            typedef XST::TCObjectSmartPointer< IResourceGroup > GroupPtr;
            typedef XST::TCWeakPointer< IResourceGroup >        GroupWeakPtr;
			//typedef GroupPtr GroupWeakPtr;
            typedef IResourceGroup::ResourceHandle              ResourceHandle;
            typedef IResourceGroup::Handle                      GroupHandle;
            typedef xst_hash_table< GroupHandle, GroupPtr >     ResGroupMap;
            typedef ResGroupMap::iterator                       ResGroupMapItr;
            typedef ResGroupMap::const_iterator                 ResGroupMapConstItr;
            typedef std::function< void ( ResourcePtr, GroupWeakPtr ) >   ResourceCallback;
            typedef std::function< void ( GroupWeakPtr ) >     GroupCallback;

        public:

        IResourceManager();
        virtual                     ~IResourceManager();

        virtual     i32             Init();

        ResourcePtr				CreateResource(xst_castring& strName, xst_castring& strGroupName = DEFAULT_GROUP);

					ResourcePtr				CreateResource(xst_castring& strName, GroupWeakPtr pGroup);

        virtual i32         AddResource(xst_castring& strName, ResourcePtr pRes, xst_castring& strGroupName, bool bCreateGroup = true);
        virtual i32         AddResource(xst_castring& strName, ResourcePtr pRes, GroupWeakPtr pGr);
        virtual i32	AddResource(const ResourceHandle& ulResourceHandle, ResourcePtr pRes, xst_castring& strGroupName, bool bCreateGroup = true);
        virtual i32	AddResource(const ResourceHandle& ulResourceHandle, ResourcePtr pRes, const GroupHandle& ulGroupHandle);
        virtual i32	AddResource(const ResourceHandle& ulResourceHandle, ResourcePtr pRes, GroupWeakPtr pGroup);

        virtual GroupWeakPtr    CreateGroup(xst_castring& strName);
        virtual GroupWeakPtr    GetGroup(xst_castring& strName);
        virtual GroupWeakPtr    GetGroup(const GroupHandle& ulHandle);
        virtual GroupWeakPtr    GetOrCreateGroup(xst_castring& strName);

        virtual GroupPtr RemoveGroup(xst_castring& strName);
        virtual GroupPtr RemoveGroup(const GroupHandle& Handle);
        virtual GroupPtr RemoveGroup(const GroupWeakPtr pGroup);

        virtual i32 DestroyGroup(xst_castring& strName);
        virtual i32 DestroyGroup(const GroupHandle& Handle);
        virtual i32 DestroyGroup(const GroupWeakPtr pGroup);

        virtual i32 MoveResources(xst_castring& strSrcGroup, xst_castring& strDstGroup);

        virtual i32 MoveResource(xst_castring& strResName, xst_castring& strSrcGroup, xst_castring& strDstGroup);
        virtual i32 MoveResource(xst_castring& strResName, GroupWeakPtr pSrcGroup, GroupWeakPtr pDstGroup);
        virtual i32 MoveResource(ResourcePtr, GroupWeakPtr pSrcGroup, GroupWeakPtr pDstGroup);

        virtual ResourcePtr RemoveResource(xst_castring& strName);
        virtual ResourcePtr RemoveResource(const ResourceHandle& Handle);
        virtual ResourcePtr RemoveResource(ResourcePtr pRes);
        virtual ResourcePtr RemoveResource(xst_castring& strName, xst_castring& strGroup);
        virtual ResourcePtr RemoveResource(const ResourceHandle& Handle, const GroupHandle& GroupHandle);

        virtual i32 DestroyResource(xst_castring& strName);
        virtual i32 DestroyResource(const ResourceHandle& Handle);
        virtual i32 DestroyResource(ResourcePtr pRes);
        virtual i32 DestroyResource(xst_castring& strName, xst_castring& strGroup);
        virtual i32 DestroyResource(const ResourceHandle& Handle, const GroupHandle& GroupHandle);

		virtual void				DestroyResources() {}

        virtual	xst_fi ResourcePtr				CloneResource(ResourcePtr pSrcRes, xst_castring& strNewName = XST::StringUtil::EmptyAString, bool bFullClone = true)
											{ return CloneResource( pSrcRes.GetPtr(), strNewName, bFullClone ); }

			virtual ResourcePtr				CloneResource(const Resources::IResource* pSrcRes, xst_castring& strNewName = XST::StringUtil::EmptyAString, bool bFullClone = true);

        virtual	ResourcePtr				LoadResource(xst_castring& strName, xst_castring& strGroupName = ALL_GROUPS);

			virtual ResourcePtr				LoadResource(xst_castring &strFileName, xst_castring& strResName, xst_castring &strGroupName);

			virtual i32						LoadResource(ResourcePtr pRes, xst_castring& strGroupName = ALL_GROUPS);
			virtual i32						LoadResource(ResourcePtr pRes, xst_castring& strFileName, xst_castring& strGroupName);

			virtual ResourcePtr				PrepareResource(xst_castring& strName, xst_castring& strGroupName = DEFAULT_GROUP);
            virtual ResourcePtr				PrepareResource(xst_castring& strName, GroupWeakPtr pGroup);
            virtual i32						PrepareResource(ResourcePtr pRes) { return XST_OK; };

            ResourcePtr				GetResource(xst_castring& strName, GroupWeakPtr pGroup);
            ResourcePtr				GetResource(xst_castring& strName, xst_castring& strGroupName = ALL_GROUPS);

            ResourcePtr				GetOrCreateResource(xst_castring& strName, xst_castring& strGroupName = ALL_GROUPS, bool* pbCreatedOut = xst_null);

					ResourcePtr				GetOrCreateResource(xst_castring& strName, GroupWeakPtr pGroup, bool* pbCreatedOut = xst_null);

            virtual i32                 CreateMemoryPool(cul32& ulMemorySize);

            virtual void                ForEachResource(ResourceCallback Callback);

            virtual void                ForEachGroup(GroupCallback Callback);

        protected:

            virtual Resources::IResource*   _CreateResource(xst_castring& strName, const ResourceHandle& ResHandle, GroupWeakPtr pGroup) = 0;
            
            virtual IResourceGroup*         _CreateGroup(xst_castring& strName, const GroupHandle& Handle);

            virtual i32                     _OnGroupDestroy(GroupWeakPtr pGroup)
                                            { return XST_OK; }

            virtual XST::IAllocator*        _CreateMemoryManager(cul32& ulMemorySize);
            
            virtual void                    _DestroyMemoryManager(XST::IAllocator** ppMemMgr);

        protected:

            ResGroupMap         m_mGroups;
            xst_stringstream    m_ssTmpName;
            XST::IAllocator*    m_pMemoryMgr = xst_null;
            XST::CFileManager*	m_pFileMgr = xst_null;
            lpcastr             m_pStrTmp = "";
            GroupWeakPtr        m_pLastUsedGroup;
            ul32                m_LastUsedGroupHandle = XSE_INVALID_HANDLE;
    };

	class XST_API IResourceManager2 : public XST::TCResourceManager< ResourcePtr >
	{
		friend class CEngine;

		public:

			typedef IResourceListener						Listener;
			typedef XST::TCResourceManager< ResourcePtr >	BaseManager;
			typedef BaseManager::ResourceGroup				ResourceGroup;
			typedef BaseManager::ResourceIterator			ResourceIterator;
			//typedef BaseManager::GroupIterator				GroupIterator;
			typedef BaseManager::GroupPtr					GroupPtr;

			//typedef xst_list< Listener* >					ListenerList;
			xst_tlistx( Listener* )							ListenerList;

			static	xst_castring							DEFAULT_GROUP;
			static	xst_castring							ALL_GROUPS;

		public:

											IResourceManager2();
			virtual							~IResourceManager2();

					XST::IAllocator&		GetMemoryPool();

					ResourcePtr				CreateResource(xst_castring& strName, xst_castring& strGroupName = DEFAULT_GROUP);

					ResourcePtr				CreateResource(xst_castring& strName, GroupPtr pGroup);

					ResourcePtr				GetOrCreateResource(xst_castring& strName, xst_castring& strGroupName = ALL_GROUPS, bool* pbCreatedOut = xst_null);

					ResourcePtr				GetOrCreateResource(xst_castring& strName, GroupPtr pGroup, bool* pbCreatedOut = xst_null);

					ResourcePtr				GetResource(xst_castring& strName, xst_castring& strGroup = ALL_GROUPS);

			virtual	ResourcePtr				LoadResource(xst_castring& strName, xst_castring& strGroupName = ALL_GROUPS);

			virtual ResourcePtr				LoadResource(xst_castring &strFileName, xst_castring& strResName, xst_castring &strGroupName);

			virtual i32						LoadResource(ResourcePtr pRes, xst_castring& strGroupName = ALL_GROUPS);
			virtual i32						LoadResource(ResourcePtr pRes, xst_castring& strFileName, xst_castring& strGroupName);

			//virtual i32						LoadResource(ResourcePtr pRes);

			virtual ResourcePtr				PrepareResource(xst_castring& strName, xst_castring& strGroupName = DEFAULT_GROUP);
			virtual i32						PrepareResource(ResourcePtr pRes) = 0;

			xst_fi	void					AddListener(Listener* pListener)
											{ m_lListeners.push_back( pListener ); }

					void					DestroyListeners();

					void					RemoveListener(Listener* pListener, bool bDestroy = true);

					ResourcePtr				GetResource(xst_castring& strName, GroupPtr pGroup);

			xst_fi	bool					IsResourceExists(xst_castring& strName, xst_castring& strGroupName = ALL_GROUPS)
											{ return !GetResource( strName, strGroupName ).IsNull(); }

			virtual	ResourcePtr				CloneResource(const ResourcePtr pSrcRes, xst_castring& strNewName = XST::StringUtil::EmptyAString, bool bFullClone = true)
											{ return CloneResource( pSrcRes.GetPtr(), strNewName, bFullClone ); }

			virtual ResourcePtr				CloneResource(const Resources::IResource* pSrcRes, xst_castring& strNewName = XST::StringUtil::EmptyAString, bool bFullClone = true);

		protected:

			virtual	i32						_Init() { return XST_OK; }
			virtual	Resources::IResource*	_CreateResource(xst_castring& strName, cul32& ulHandle, GroupPtr pGroup) = 0;
			virtual	i32						_CreateMemoryPool(cul32& ulObjCount, XST::IAllocator* pAllocator = xst_null) = 0;

		protected:

			XST::IAllocator*	m_pMemoryMgr;
			ListenerList		m_lListeners;
			ResourcePtr			m_pNullRes;
			XST::CFileManager*	m_pFileMgr;
			xst_stringstream	m_ssTmpName;
	};

	#define XSE_DO_LISTENER_VOID_METHOD(_methodName, _listIteratorType, _listenerList, _arguments) \
		for( _listIteratorType ListenerItr = _listenerList.begin(); ListenerItr != _listenerList.end(); ++ListenerItr ) \
		{ \
			(*ListenerItr)->_methodName( _arguments ); \
		}

}//xse

#endif