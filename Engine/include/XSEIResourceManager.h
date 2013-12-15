#ifndef XSE_IRESOURCEMANAGER_H
#define XSE_IRESOURCEMANAGER_H

#include "XSECommon.h"
#include "XSEIResource.h"

namespace XSE
{
	class IResourceListener
	{
		public:

			virtual				~IResourceListener() {}

			virtual	void		ResourceCreated(ResourcePtr pRes) {}

		protected:
	};


	class XST_API IResourceManager : public virtual XST::TCResourceManager< ResourcePtr >
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

											IResourceManager();
			virtual							~IResourceManager();

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

			//virtual i32						LoadResource(ResourcePtr& pRes);

			virtual ResourcePtr				PrepareResource(xst_castring& strName, xst_castring& strGroupName = DEFAULT_GROUP);
			virtual i32						PrepareResource(ResourcePtr pRes) = 0;

			xst_fi	void					AddListener(Listener* pListener)
											{ m_lListeners.push_back( pListener ); }

					void					DestroyListeners();

					void					RemoveListener(Listener* pListener, bool bDestroy = true);

					ResourcePtr				GetResource(xst_castring& strName, GroupPtr pGroup);

			xst_fi	bool					IsResourceExists(xst_castring& strName, xst_castring& strGroupName = ALL_GROUPS)
											{ return !GetResource( strName, strGroupName ).IsNull(); }

			virtual	ResourcePtr				CloneResource(const ResourcePtr& pSrcRes, xst_castring& strNewName = XST::StringUtil::EmptyAString, bool bFullClone = true)
											{ return CloneResource( pSrcRes.GetPointer(), strNewName, bFullClone ); }

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