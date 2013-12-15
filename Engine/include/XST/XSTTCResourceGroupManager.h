#ifndef XST_RESOURCE_GROUP_MANAGER_H
#define XST_RESOURCE_GROUP_MANAGER_H

#include "XSTTCResourceManager.h"

namespace XST
{
	

	template<class _T_, class _MGR_CLASS_>
	class TCResourceGroupManager;

	template<class _T_, class _MGR_CLASS_>
	class TCResourceGroup //: public TCResourceManager<_T_, TCResourceGroup<_T_, _MGR_CLASS_> >
	{
		friend class TCResourceGroupManager<_T_, _MGR_CLASS_>;
		friend _MGR_CLASS_;

		public:

			//typedef TCResourceManager<_T_>::ResourceIterator	ResourceIterator;

			typedef void (_MGR_CLASS_::*FuncPtr)(	TCResourceGroup<_T_, _MGR_CLASS_>* _pGroup, _T_*	_pResource);
			typedef void (_MGR_CLASS_::*FuncPtr2)(	TCResourceGroup<_T_, _MGR_CLASS_>* _pGroup, _T_*&	_pResource, const ul32& _ulID);
			typedef void (_MGR_CLASS_::*FuncPtr3)(	TCResourceGroup<_T_, _MGR_CLASS_>* _pGroup, const ul32& _ulID, _T_*	_pResource);
			typedef _T_* (_MGR_CLASS_::*FuncPtr4)(	TCResourceGroup<_T_, _MGR_CLASS_>* _pGroup, const ul32& _ulID);

			typedef _T_									Resource;
			typedef _T_*								ResourcePtr;
			typedef const ResourcePtr					ConstResPtr;
			typedef Resource&							ResourceRef;
			typedef const Resource&						ConstResRef;

			typedef xst_map<ul32, ResourcePtr>			ResourceMap;
			typedef xst_pair<ul32, ResourcePtr>			ResourcePair;
			typedef typename ResourceMap::iterator		ResourceIterator;
			typedef typename ResourceMap::const_iterator ResourceConstIterator;
			
			typedef xst_stack<ResourcePtr>				ResourceStack;

		public:

							TCResourceGroup(_MGR_CLASS_* _pResGroupMgr, FuncPtr _OnResRemPtr, FuncPtr _OnResDesPtr, const ul32& _ulID, ResourceStack* _pResStack) 
							{
								m_pResGroupMgr = _pResGroupMgr;

								m_OnResourceDestroyPtr = _OnResDesPtr;
								m_OnResourceRemovePtr = _OnResRemPtr;

								m_ulID = _ulID;

								m_uiResourceCount = 0;
								m_pLastUsedResource = 0;
								m_ulLastUsedID = 0;

								xst_assert(_pResStack != 0, "(TCResourceGroup) _pResStack is null. Application will crash.");
								m_psFreeResources = _pResStack;
							}

			virtual			~TCResourceGroup()
							{
								_DestroyAllResources();
							}

			

			xst_fi	ul32	GetID() const
			{ 
				return m_ulID;
			}
		

		public:


			i32		AddResource(const ul32& _ulID, _T_* _pResource)
			{
				ResourceIterator Itr = m_mResources.lower_bound(_ulID);
				if( Itr != m_mResources.end() && !(m_mResources.key_comp()(_ulID, Itr->first)) )
				{
					//Resource already exists, return failed
					return RESULT::FAILED;
				}
				else
				{
					//Resource doesn't exists, add it to the map
					m_mResources.insert(Itr, ResourceMap::value_type(_ulID, _pResource));
					return RESULT::OK;
				}

				return RESULT::FAILED;
			}

			xst_fi void	AddResource(ResourceIterator _Itr, ul32 _ulID, _T_* _pResource)
			{
				m_mResources.insert(_Itr, ResourceMap::value_type(_ulID, _pResource));
			}

			//Returns number of the removed resources - 0 or 1
			i32	RemoveResource(const ul32& _ulID)
			{
				//If removed resource is the last used
				if(m_ulLastUsedID == _ulID)
				{
					m_uiResourceCount--;
					
					m_mResources.erase(m_ulLastUsedID);
					_OnResourceRemove(m_pLastUsedResource);
					m_pLastUsedResource = 0;
					m_ulLastUsedID = 0;

					return 1;
				}

				//If it was not last used resource, search if exists this one
				ResourceIterator Itr = GetResourceItr(_ulID);
				if( ResourceExists(Itr, _ulID) )
				{
					m_uiResourceCount--;
					
					_T_* pResource = Itr->second;
					m_mResources.erase(Itr);
					_OnResourceRemove(pResource);
					return 1;
				}

				return 0;
			}

			//Danger! No _OnResourceRemove
			ResourcePtr	SimpleRemoveResource(const ul32& _ulID)
			{
				ResourcePtr pRes = 0;
				//If removed resource is the last used
				if(m_ulLastUsedID == _ulID)
				{
					m_uiResourceCount--;
					
					pRes = m_pLastUsedResource;

					m_mResources.erase(m_ulLastUsedID);
					m_pLastUsedResource = 0;
					m_ulLastUsedID = 0;

					return pRes;
				}

				//If it was not last used resource, search if exists this one
				ResourceIterator Itr = GetResourceItr(_ulID);
				if( ResourceExists(Itr, _ulID) )
				{
					m_uiResourceCount--;
					
					pRes = Itr->second;
					m_mResources.erase(Itr);
					return pRes;
				}

				return 0;
			}

			xst_i _T_*	GetResource(const ul32& _ulID) 
			{
				//If it is the same resource that was used as last
				if(m_ulLastUsedID == _ulID)
				{
					return m_pLastUsedResource;
				}

				ResourceIterator Itr = m_mResources.lower_bound(_ulID);
				//If resource exists
				if( Itr != m_mResources.end() && !(m_mResources.key_comp()(_ulID, Itr->first)) )
				{
					m_pLastUsedResource = Itr->second;
					m_ulLastUsedID = _ulID;
					return Itr->second;
				}

				return 0;
			}

			xst_i _T_*	GetGroup(const ul32& _ulID) 
			{
				//If it is the same resource that was used as last
				if(m_ulLastUsedID == _ulID)
				{
					return m_pLastUsedResource;
				}

				ResourceIterator Itr = m_mResources.lower_bound(_ulID);
				//If resource exists
				if( Itr != m_mResources.end() && !(m_mResources.key_comp()(_ulID, Itr->first)) )
				{
					m_pLastUsedResource = Itr->second;
					m_ulLastUsedID = _ulID;
					return Itr->second;
				}

				return 0;
			}

			xst_fi ResourceIterator	GetResourceItr(const ul32& _ulID)
			{
				return m_mResources.lower_bound(_ulID);
			}

			xst_fi bool	ResourceExists(ResourceIterator _Itr, const ul32& _ulID)
			{
				return ( _Itr != m_mResources.end() && !(m_mResources.key_comp()(_ulID, _Itr->first)) );
			}

			xst_fi void	RemoveAllResources()
			{
			
				ResourceIterator Itr = m_mResources.begin();
				//The first resource is on 1st position, 0 is reserved for null
				for(Itr; Itr != m_mResources.end(); ++Itr)
				{
					_OnResourceRemove(Itr->second);
				}

				//Clears all resources
				m_mResources.clear();
				//And add the null
				//AddResource(0,0);
				m_uiResourceCount = 0;
				m_ulLastUsedID = 0;
				m_pLastUsedResource = 0;
			}

			xst_fi u32	GetResourceCount() const
			{
				return m_uiResourceCount;
			}

			xst_fi ResourceIterator GetBeginIterator()
									{ return m_mResources.begin(); }

			xst_fi ResourceIterator	GetEndIterator()
									{ return m_mResources.end(); }

		protected:

			

			xst_fi	void	_SetID(const ul32& _ulID)
			{ 
				m_ulID = _ulID;
			}

			void	_OnResourceRemove(_T_* _pResource)
			{ 
				XST_CALL_MEMBER_FN_PTR(m_pResGroupMgr, m_OnResourceRemovePtr)(this, _pResource);  
			}

			void	_OnResourceDestroy(_T_* _pResource)
			{ 
				XST_CALL_MEMBER_FN_PTR(m_pResGroupMgr, m_OnResourceDestroyPtr)(this, _pResource); 
			}


			i32		_BeginResourceCreate(const ul32& _ulID, _T_** _ppOutRes)
			{
				(*_ppOutRes) = 0;

				m_BeginCreateItr = m_mResources.lower_bound(_ulID);
				//If resource exists
				if( m_BeginCreateItr != m_mResources.end() && !(m_mResources.key_comp()(_ulID, m_BeginCreateItr->first)) )
				{
					m_bCanEndCreate = false;
					return RESULT::FAILED;
				}

				m_ulBeginCreateID = _ulID;
				m_bCanEndCreate = true;

				//If available on free stack
				if( !m_psFreeResources->empty() )
				{
					(*_ppOutRes) = m_psFreeResources->top();
					m_psFreeResources->pop();
				}

				return RESULT::OK;
			}

			xst_fi void _EndResourceCreate(_T_* _pRes)
			{
				//xst_assert(m_BeginCreateItr != m_mResources.end(), "(TCResourceManager::_EndResourceCreate) m_BeginCreateItr is invalid. Did you forget to use _BeginResourceCreate method?");
				//xst_assert(_pRes != 0, "(TCResourceManager::_EndResourceCreate) m_pBeginCreateRes is null. Did you forget to create resource?");
				//xst_assert(m_ulBeginCreateID != 0, "(TCResourceManager::_EndResourceCreate) m_ulBeginCreateID is invalid. Did you forget to use _BeginResourceCreate method?");
				xst_assert(m_bCanEndCreate == true, "(TCResourceManager::_EndResourceCreate) Cannot end the resource create. Did you forget to use _BeginResourceCreate method?");
				m_mResources.insert(m_BeginCreateItr, ResourceMap::value_type(m_ulBeginCreateID, _pRes));
				m_BeginCreateItr = m_mResources.end();
				m_ulBeginCreateID = 0;
				m_bCanEndCreate = false;
			}

			xst_fi	ResourceMap& _GetResourceMap()
			{ 
					return m_mResources; 
			}

					void	_DestroyAllResources()
					{
						//Remove all resources from free stack
						while(!m_psFreeResources->empty())
						{
							_OnResourceDestroy(m_psFreeResources->top());
							m_psFreeResources->pop();
						}

						ResourceIterator Itr = m_mResources.begin();
						//i32 i = 0;
						//The first resource is on 1st position, 0 is reserved for null
						for(Itr; Itr != m_mResources.end(); ++Itr)
						{
							//XST::CConsole::WriteLN( XST::CStringUtil::ToString(++i).data() );
							_OnResourceDestroy(Itr->second);
						}

						m_mResources.clear();

						//And add the null
						//AddResource(0,0);
						m_uiResourceCount = 0;
						m_pLastUsedResource = 0;
						m_ulLastUsedID = 0;
					}

					void _DestroyResource(_T_* _pResource)
					{
						if(m_pLastUsedResource == _pResource)
						{
							m_pLastUsedResource = 0;
							m_ulLastUsedID = 0;
						}

						_OnResourceDestroy(_pResource);
					}

					void _DestroyResource(const ul32& _ulID)
					{
						if(m_ulLastUsedID == _ulID)
						{
							m_ulLastUsedID = 0;
							_OnResourceDestroy(m_pLastUsedResource);
							m_pLastUsedResource = 0;
						}
					}


			xst_fi	ResourceStack& _GetFreeResourceStack()
			{ 
				return *m_psFreeResources; 
			}

			xst_fi	void _AddFreeResource(_T_* _pRes)
			{
				m_psFreeResources->push(_pRes);
			}

			xst_fi _T_*	_GetFirstFreeResource() const
			{ 
				return m_psFreeResources->top();
			}

			xst_fi void _RemoveFreeResource()
			{
				m_psFreeResources->pop();
			}

		protected:

			ResourcePtr			m_pLastUsedResource; //resource that was last used (get)
			ResourceIterator	m_BeginCreateItr; //iterator using in _BeginResourceCreating - position of resource to insert
			ul32				m_ulBeginCreateID;
			bool				m_bCanEndCreate;
			ul32				m_ulLastUsedID; //resource id thtat was last used (get)

			ResourceMap			m_mResources;

			ResourceStack*		m_psFreeResources;
			u32					m_uiResourceCount;

		private:

			_MGR_CLASS_*	m_pResGroupMgr;
			FuncPtr			m_OnResourceRemovePtr;
			FuncPtr			m_OnResourceDestroyPtr;
			FuncPtr2		m_OnResourceCreatePtr;
			FuncPtr3		m_OnResourceCreateNewPtr;
			FuncPtr4		m_OnResourceCreateFromStackPtr;
			ul32			m_ulID;

	};


	template<class _T_, class _MGR_CLASS_ >
	class TCResourceGroupManager : public virtual TCResourceManager< TCResourceGroup<_T_, _MGR_CLASS_> >
	{
		public:

			typedef _T_										Resource;
			typedef TCResourceGroup<_T_, _MGR_CLASS_>		ResourceGroup;
			typedef ResourceIterator						GroupIterator;

		protected:		

			typedef typename ResourceGroup::ResourceStack	BaseResourceStack;

		public:

					TCResourceGroupManager() 
					{
					}

					virtual ~TCResourceGroupManager()
					{
						//All groups are destroyed in TCResourceManager destructor
					}

			xst_fi	i32 AddGroup(const ul32& _ulGroupID, ResourceGroup* _pResGroup)
			{
				return AddResource(_ulGroupID, _pResGroup);
			}

			xst_fi	i32 RemoveGroup(const ul32& _ulGroupID)
			{
				ResourceIterator Itr = m_mResources.find( _ulGroupID );
				if( Itr == m_mResources.end() )
					return RESULT::FAILED;

				_OnGroupRemove( Itr->second );
				m_mResources.erase( Itr );

				return RESULT::OK;
			}

			xst_i	ResourceGroup* GetGroup(const ul32& _ulGroupID)
			{
				//If it is the same resource that was used as last
				if(m_ulLastUsedID == _ulGroupID)
				{
					return m_pLastUsedResource;
				}

				ResourceIterator Itr = m_mResources.lower_bound(_ulGroupID);
				//If resource exists
				if( Itr != m_mResources.end() && !(m_mResources.key_comp()(_ulGroupID, Itr->first)) )
				{
					m_pLastUsedResource = Itr->second;
					m_ulLastUsedID = _ulGroupID;
					return Itr->second;
				}

				return 0;
			}

			xst_fi	i32 RemoveResource(const ul32& _ulResourceID, const ul32& _ulGroupID)
			{
				ResourceGroup* pGroup = GetGroup(_ulGroupID);
				xst_assert( pGroup != 0, "(TCResourceGroupManager::RemoveResource) Resource group doesn't exists");

				return pGroup->RemoveResource(_ulResourceID);
			}

			//Returns number of removed resources
			xst_i	i32 RemoveResource(const ul32& _ulResourceID)
			{
				i32 iResult = 0;
				for(GroupIterator Itr = m_mResources.begin(); Itr != m_mResources.end(); ++Itr)
				{
					iResult += Itr->second->RemoveResource( _ulResourceID );
				}

				return iResult;
			}

			//Get resource; search all groups
			_T_* GetResource(const ul32& _ulResourceID)
			{
				ResourceIterator Itr = m_mResources.begin();
				_T_* pRes = 0;
				for(Itr; Itr != m_mResources.end(); ++Itr)
				{
					pRes = Itr->second->GetResource( _ulResourceID );
					if( pRes )
						return pRes;
				}

				return 0;
			}

			_T_* GetResource(const ul32& _ulResourceID, const ul32& _ulGroupID)
			{
				//Get the group
				//If it is the same resource that was used as last
				if(m_ulLastUsedID == _ulGroupID)
				{
					return m_pLastUsedResource->GetResource( _ulResourceID );
				}

				ResourceIterator Itr = m_mResources.lower_bound(_ulGroupID);
				//If resource exists
				if( Itr != m_mResources.end() && !(m_mResources.key_comp()(_ulGroupID, Itr->first)) )
				{
					m_pLastUsedResource = Itr->second;
					m_ulLastUsedID = _ulGroupID;
					return Itr->second->GetResource( _ulResourceID );
				}

				/*ResourceGroup* pGroup = GetGroup( _ulGroupID );
				if( pGroup )
				{
					return pGroup->GetResource( _ulResourceID );
				}*/

				return 0;
			}

			i32 MoveResource(const ul32& _ulResourceID, const ul32& _ulDstGroupID, const ul32& _ulSrcGroupID)
			{
				//Get the groups
				ResourceGroup* pDstGroup = GetResource(_ulDstGroupID);
				xst_assert( pGroup != 0, "(TCResourceGroupManager::MoveResource) destination resource group doesn't exists");
				
				ResourceGroup* pSrcGroup = GetResource(_ulSrcGroupID);
				xst_assert( pGroup != 0, "(TCResourceGroupManager::MoveResource) source resource group doesn't exists");

				ResourceGroup::ResourcePtr _pRes = pSrcGroup->SimpleRemoveResource(_ulResourceID);
				xst_assert( _pRes != 0, "(TCResourceGroupManager::MoveResource) resource doesn't exists");

				return _pDstGroup->AddResource(_ulResourceID, _pRes);
			}

			i32 MoveResources(const ul32& _ulDstGroupID, const ul32& _ulSrcGroupID)
			{
				ResourceGroup* pDstGroup = GetResource(_ulDstGroupID);
				xst_assert( pGroup != 0, "(TCResourceGroupManager::MoveResource) destination resource group doesn't exists");
				
				ResourceGroup* pSrcGroup = GetResource(_ulSrcGroupID);
				xst_assert( pGroup != 0, "(TCResourceGroupManager::MoveResource) source resource group doesn't exists");

				TCResourceManager<_T_>::ResourceIterator Itr;
				for(Itr = pSrcGroup->GetBeginIterator(); Itr != pSrcGroup->GetEndIterator(); ++Itr)
				{
				}

				return RESULT::FAILED;
			}

			void RemoveAllResources()
			{
				ResourceIterator Itr = GetBeginIterator();
				ResourceIterator EndItr = GetEndIterator();

				for(Itr; Itr != EndItr; ++Itr)
				{
					Itr->second->RemoveAllResources();
				}
			}
			

		protected:

			xst_fi void _AddFreeResource(_T_* _pRes)
			{
				m_sFreeBaseResources.push( _pRes );
			}

			xst_fi void _AddFreeGroup(ResourceGroup* _pGroup)
			{
				m_sFreeResources.push( _pGroup );
			}

			i32 _MoveResource(const ul32& _ulResourceID, ResourceGroup* _pDstGroup, ResourceGroup* _pSrcGroup)
			{
				xst_assert( _pDstGroup != 0, "(TCResourceGroupManager::MoveResource) destination resource group doesn't exists");
				xst_assert( _pSrcGroup != 0, "(TCResourceGroupManager::MoveResource) source resource group doesn't exists");

				ResourceGroup::ResourcePtr _pRes = _pSrcGroup->SimpleRemoveResource(_ulResourceID);
				xst_assert( _pRes != 0, "(TCResourceGroupManager::MoveResource) resource doesn't exists");

				return _pDstGroup->AddResource(_ulResourceID, _pRes);
			}

			xst_fi GroupIterator _GetGroup(cul32& _ulGroupID)
			{
				return m_mResources.lower_bound( _ulGroupID );
			}

			xst_fi bool _GroupExists(const GroupIterator& _Itr, cul32& _ulGroupID)
			{
				return _Itr != m_mResources.end() && ( m_mResources.key_comp()( _ulGroupID, _Itr->first ) );
			}

			//Unsafe method
			xst_fi void _AddGroup(const GroupIterator& _Itr, cul32& _ulGroupID, ResourceGroup* _pGroup)
			{
				m_mResources.insert( _Itr, ResourceMap::value_type( _ulGroupID, _pGroup ) );
			}
	
			virtual void _OnGroupRemove(ResourceGroup* _pResGroup)
			{ 
				xst_delete(_pResGroup);
			}

			virtual	void _OnGroupDestroy(ResourceGroup* _pResGroup)
			{ 
				xst_delete(_pResGroup); 
			}

		protected:

		private:


					void _OnResourceRemove(ResourceGroup* _pResGroup)
					{
						_OnGroupRemove(_pResGroup);
					}

					void _OnResourceDestroy(ResourceGroup* _pResGroup)
					{
						_OnGroupDestroy(_pResGroup);
					}

		protected:

			BaseResourceStack	m_sFreeBaseResources;

	};

	#define XST_RESOURCE_GROUP_CONSTRUCTOR( _className, _ulGroupID ) ResourceGroup( this, &_className::_OnResourceRemove, &_className::_OnResourceDestroy, (_ulGroupID), &m_sFreeBaseResources )
	#define XST_DEFAULT_GROUP "Default"

	#define XST_RGM_CREATE_GROUP( _className ) \
		ResourceGroup*	_CreateGroup(const ul32& _ulID)\
		{\
			ResourceGroup* pGroup = 0;\
			if( this->_BeginResourceCreate( _ulID, &pGroup ) == RESULT::OK )\
			{\
				if( !pGroup ) pGroup = xst_new XST_RESOURCE_GROUP_CONSTRUCTOR( _className, _ulID );\
				this->_EndResourceCreate( pGroup );\
				return pGroup;\
			}\
			return 0;\
		}

	#define XST_RGM_GET_OR_CREATE_GROUP( _className) \
			ResourceGroup*	_GetOrCreateGroup(const ul32& _ulGroupID)\
			{\
				if(m_ulLastUsedID == _ulGroupID) return m_pLastUsedResource;\
				ResourceIterator Itr = m_mResources.lower_bound(_ulGroupID);\
				if( Itr != m_mResources.end() && !(m_mResources.key_comp()(_ulGroupID, Itr->first)) ) \
				{\
					m_pLastUsedResource = Itr->second;\
					m_ulLastUsedID = _ulGroupID;\
					return m_pLastUsedResource;\
				}\
				ResourceGroup* pGroup = 0;\
				return _CreateGroup( _ulGroupID ); \
			}

}//XST

#endif