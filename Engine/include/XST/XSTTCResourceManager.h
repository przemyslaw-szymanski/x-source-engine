#ifndef XST_TCRESOURCE_MANAGER_H
#define XST_TCRESOURCE_MANAGER_H

#include "XSTCommon.h"
#include "XSTCHash.h"
#include "XSTTCSmartPointer.h"
#include "XSTCLogger.h"
#include "XSTIResource.h"

namespace XST
{

	template<class _RESOURCE_>
	class TIBaseResourceManager
	{
		public:

			virtual				~TIBaseResourceManager() {}

					u16			GetResourceManagerType() const
								{ return m_usResMgrType; }

			virtual void		RemoveResource(_RESOURCE_& pRes)
			{
			}

		protected:

			u16		m_usResMgrType;
	};

	class XST_API IBaseResourceManager
	{
		public:

			virtual				~IBaseResourceManager() {}

					u16			GetResourceManagerType() const
								{ return m_usResMgrType; }

			virtual void		RemoveResource(Resources::IResource* pRes)
			{
			}

		protected:

			u16		m_usResMgrType;
	};



	template<class _KEY_, class _HASH_ = ul32 >
	class TCDefaultResourceNameHasher
	{
		public:

			static const _HASH_&	GetHash(const _KEY_& key)
			{
				return XST::CHash::GetCRC( key );
			}
	};

	template<class _KEY_, class _HASH_ = _KEY_ >
	class TCNoResourceNameHasher
	{
		public:

			static const _HASH_&	GetHash(const _KEY_& key)
			{
				return key;
			}
	};


	#define XST_RESMGR_HASH		TCDefaultResourceNameHasher< _KEY_, _HASH_ >
	#define XST_RESMGR_RESOP	TCDefaultResourceOperator< _RESOURCE_ >


	class MapUtils
	{
		public:

			template<class _M_, class _K_, class _V_>
			static xst_fi i32	Insert(_M_& map, const _K_& key, const _V_& value, typename _M_::iterator* pItrOut = xst_null)
			{

				//ResourceIterator Itr = m_mResources.lower_bound(_ulID);
				typename _M_::iterator Itr = map.lower_bound( key );
				if( Itr != map.end() && !( map.key_comp()( key, Itr->first ) ) )
				{
					//Resource already exists, return failed
                    if( pItrOut ) *pItrOut = Itr;
					return RESULT::FAILED;
				}
				else
				{
					//Resource doesn't exists, add it to the map
					map.insert( Itr, _M_::value_type( key, value ) );
                    if( pItrOut ) *pItrOut = Itr;
					return RESULT::OK;
				}

				return RESULT::FAILED;

			}

			template<class _M_, class _K_, class _V_>
			static xst_fi void InsertOnPlace(_M_& map, const _K_& key, const _V_& value, typename _M_::iterator& Itr)
			{
				map.insert( Itr, _M_::value_type( key, value ) );
			}

			template<class _M_, class _K_>
			static xst_fi bool FindPlace(_M_& map, const _K_& key, typename _M_::iterator* pItrOut = xst_null)
			{

				//ResourceIterator Itr = m_mResources.lower_bound(_ulID);
				typename _M_::iterator Itr = map.lower_bound( key );
				if( Itr != map.end() && !( map.key_comp()( key, Itr->first ) ) )
				{
					//Resource already exists
                    if( pItrOut ) *pItrOut = map.end();
					return false;
				}

                if( pItrOut ) *pItrOut = Itr;
				return true;

			}

			template<class _M_, class _K_>
			static xst_fi bool IsKeyExists(const _M_& map, const _K_& key, const typename _M_::iterator& Itr)
			{
				return ( Itr != map.end() && !( map.key_comp()( key, Itr->first ) ) );
			}
	};


	#define XST_RESMGR_LOG(_strStream)	{ XST::CLogger::GetSingleton() << "[ERROR]: " << _strStream; XST::CLogger::GetSingleton().FlushStream( CLogger::ALL ); } //else { std::stringstream ss; ss << _strStream; CLastError::Set( ss.str() ); }

	//#define XST_RESMGR_TEMPLATE_PARAMS	_RESOURCE_, _OPERATOR_, _KEY_, _HASH_, _HASH_CLASS_
	#define XST_RESMGR_TEMPLATE_PARAMS	_RESOURCE_, _KEY_, _HASH_, _HASH_CLASS_

	//template< XST_RESMGR_TEMPLATE_PARAMS > class TCResourceManager

    template<
		class _RESOURCE_,
		//class _OPERATOR_ = TIBaseResourceListener< _RESOURCE_ >,
		class _KEY_ = xst_astring,
		class _HASH_ = ul32,
		class _HASH_CLASS_ = XST_RESMGR_HASH
	>
	class XST_API TCBaseResourceManager : public virtual IBaseResourceManager
    {
		//friend class TCResourceManager< XST_RESMGR_TEMPLATE_PARAMS >;
        public:

            typedef _HASH_CLASS_					HashClass;
			//typedef _OPERATOR_						OperatorClass;
            typedef _KEY_							Key;
			typedef _HASH_							Hash;
            typedef _RESOURCE_						Resource;
            xst_tmapx( Hash, Resource )				ResourceMap;
			typedef typename ResourceMap::value_type	ResourcePair;

			//typedef _OPERATOR_*						ResourceListenerPtr;
			//typedef TIBaseResourceListener< _RESOURCE_ >* ResourceListenerPtr;
			typedef fastdelegate::FastDelegate1< _RESOURCE_& >	Delegate;
#if( XST_VS_VER > 9 )

#else
			XST_TEMPLATE_CLASS fastdelegate::FastDelegate1< _RESOURCE_& >;
#endif

			class CIterator
			{
				public:

					typedef xst_map< Hash, Resource >	Map;

				public:

											CIterator() : m_pMap( xst_null ) {}
											CIterator(Map* pMap) : m_pMap( pMap ), m_Itr( pMap->begin() ) {}

					xst_fi	bool			HasMoreElements()
											{ xst_assert( !IsEmpty(), "Iterator is empty" ); return m_Itr != m_pMap->end(); }

					xst_fi	void			MoveNext()
											{ xst_assert( !IsEmpty(), "Iterator is empty" ); ++m_Itr; }

					xst_fi	const _HASH_&	GetKey()
											{ xst_assert( !IsEmpty(), "Iterator is empty" ); return m_Itr->first; }

					xst_fi	_RESOURCE_&		GetValue()
											{ xst_assert( !IsEmpty(), "Iterator is empty" ); return m_Itr->second; }

					xst_fi	bool			IsEmpty()
											{ return m_pMap == xst_null; }

					xst_fi	Delegate&		GetOnForEachResource()
											{ return m_OnResItr; }

					xst_fi	void			SetOnForEachResource(const Delegate& Event)
											{ m_OnResItr = Event; }

					xst_i	void			ForEachResource()
					{
						if( m_OnResItr )
						{
							for(_ResouceIterator Itr = m_mResources.begin(); Itr != m_mResources.end(); ++Itr)
							{
								m_OnResItr( Itr->second );
							}
						}
					}

				protected:

					Map*					m_pMap;
					typename Map::iterator	m_Itr;
					Delegate				m_OnResItr;
			};

			typedef CIterator	Iterator;

			typedef typename ResourceMap::iterator	_ResourceIterator;

        public:

								TCBaseResourceManager(bool bManualDestroy = false) : m_bManualResourceDestroy( bManualDestroy ), m_pUserData( xst_null )
								{
									m_OnResDestroyDelegate.bind( this, &TCBaseResourceManager::_OnResDestroyDefault );
									m_OnResRemoveDelegate.bind( this, &TCBaseResourceManager::_OnResRemoveDefault );
								}

            virtual             ~TCBaseResourceManager()
            {
				if( !m_bManualResourceDestroy )
				{
					DestroyResources();
				}
            }

			/*xst_fi	void SetListener(ResourceListenerPtr pListener)
			{
				m_pListener = pListener;
			}*/

			xst_fi	Iterator GetIterator()
					{ return Iterator( &this->m_mResources ); }

            xst_fi i32     AddResource(const Key& Id, const Resource& Res)
            {
				Hash resId = HashClass::GetHash( Id );

				return AddResourceByHandle( resId, Res );
            }

			xst_i i32	AddResourceByHandle(const _HASH_& resId, const Resource& Res)
			{
				_ResourceIterator Itr;
				if( !MapUtils::FindPlace( m_mResources, resId, &Itr ) )
				{
					return RESULT::FAILED;
				}

				m_mResources.insert( Itr, ResourcePair( resId, Res ) );

				return RESULT::OK;
			}

			i32 GetResource(const Key& Id, Resource* pResOut)
			{
				Hash ResId = HashClass::GetHash( Id );
                if( m_LastUsedHash == ResId )
                {
                    *pResOut = m_LastUsedResource;
                    return RESULT::OK;
                }
				//pResOut = 0;
				//Find resource
				_ResourceIterator Itr = m_mResources.find( ResId );
				if( Itr == m_mResources.end() )
				{
					//XST_RESMGR_LOG( "Resource: " << Id << " not found. " );
					return RESULT::FAILED;
				}

				*pResOut = Itr->second;
                m_LastUsedHash = ResId;
                m_LastUsedResource = Itr->second;
				return RESULT::OK;
			}

			xst_fi i32 RemoveResource(const Key& Id)
			{
				Hash ResId = HashClass::GetHash( Id );
				return RemoveResourceByHandle( ResId );
			}

			i32 RemoveResourceByHandle(const Hash& ResId)
			{
				_ResourceIterator Itr = m_mResources.find( ResId );
				if( Itr == m_mResources.end() )
				{
					return RESULT::FAILED;
				}

				if( m_OnResRemoveDelegate )
					m_OnResRemoveDelegate( Itr->second );
				m_mResources.erase( Itr );
				return RESULT::OK;
			}

			xst_fi i32 DestroyResource(const Key& Id)
			{
				Hash ResId = HashClass::GetHash( Id );
				return DestroyResourceByHandle( ResId );
			}

			i32 DestroyResourceByHandle(const Hash& ResId)
			{
				_ResourceIterator Itr = m_mResources.find( ResId );
				if( Itr == m_mResources.end() )
				{
					//XST_RESMGR_LOG( "Resource: " << ResId << " not found. " );
					return RESULT::FAILED;
				}

				if( m_OnResDestroyDelegate )
					m_OnResDestroyDelegate( Itr->second );
				m_mResources.erase( Itr );

				return RESULT::OK;
			}

			void RemoveResources()
			{
				//typename ResourceMap::iterator Itr = m_mResources.begin();
				//for(; Itr != m_mResources.end(); ++Itr)
                if( m_OnResRemoveDelegate )
                {
                    for( auto& Itr : m_mResources )
                    {
                        m_OnResRemoveDelegate( Itr.second );
                    }
                }
				m_mResources.clear();
                m_LastUsedHash = 0;
			}

			void DestroyResources()
			{
				//typename ResourceMap::iterator Itr = m_mResources.begin();
				//for(; Itr != m_mResources.end(); ++Itr)
                if( m_OnResDestroyDelegate )
                {
                    for( auto& Itr : m_mResources )
                    {
                        m_OnResDestroyDelegate( Itr.second );
                    }
                }
				m_mResources.clear();
                //m_LastUsedHash = 0;
			}

			void	_SetDelegates(const Delegate& OnResRemove, const Delegate& OnResDestroy)
			{
				m_OnResRemoveDelegate = OnResRemove;
				m_OnResDestroyDelegate = OnResDestroy;
			}

			void	_SetResourceIterationDelegate(const Delegate& OnResItr)
			{
				m_OnResItrDelegate = OnResItr;
			}

			/*void	_ForEachResource()
			{
				if( m_OnResItrDelegate )
				{
					//for(_ResourceIterator Itr = m_mResources.begin(); Itr != m_mResources.end(); ++Itr)
                    for( auto& Itr : m_mResources )
					{
						m_OnResItrDelegate( Itr->second );
					}
				}
			}*/

			xst_fi	void	SetUserData(xst_unknown pUserData)
							{ m_pUserData = pUserData; }

			xst_fi	xst_unknown	GetUserData()
								{ return m_pUserData; }

			xst_fi	_HASH_	GetHash(const _KEY_& key)
							{ return HashClass::GetHash( key ); }

		protected:

			void				_OnResRemoveDefault(_RESOURCE_& Res) {}
			void				_OnResDestroyDefault(_RESOURCE_& Res) {}

        protected:

            ResourceMap			m_mResources;
            Hash                m_LastUsedHash;
            Resource            m_LastUsedResource;
			Delegate			m_OnResDestroyDelegate;
			Delegate			m_OnResRemoveDelegate;
			Delegate			m_OnResItrDelegate;
			xst_unknown			m_pUserData;
            bool				m_bManualResourceDestroy;
    };



#define XST_RESMGR_RESGROP	TCDefaultResourceGroupOperator< _RESOURCE_, XST::IObject* >

    template<
        class _RESOURCE_,
		//class _OPERATOR_ = TIBaseResourceListener< _RESOURCE_ >, //TCDefaultResourceGroupOperator< _RESOURCE_, TCBaseResourceManager< XST_RESMGR_TEMPLATE_PARAMS > >,
		class _KEY_ = xst_astring,
		class _HASH_ = ul32,
		class _HASH_CLASS_ = XST_RESMGR_HASH
    >
	class XST_API TCResourceGroup : public TCBaseResourceManager< XST_RESMGR_TEMPLATE_PARAMS >, public IObject
    {
		//friend class TCResourceManager< XST_RESMGR_TEMPLATE_PARAMS >;
        public:

            //typedef _GROUP_KEY_     GroupKey;
            typedef TCBaseResourceManager< XST_RESMGR_TEMPLATE_PARAMS >   GroupResource;
			typedef typename GroupResource::ResourceMap	ResourceMap;
			//typedef _OPERATOR_	    GroupOperatorClass;
			typedef _KEY_           ResourceName;
			typedef _HASH_          ResourceHash;
			typedef _HASH_CLASS_    HashClass;
			typedef typename GroupResource::Iterator	ResourceIterator;
			typedef GroupResource::Delegate	Delegate;

        public:

								TCResourceGroup() : TCBaseResourceManager< XST_RESMGR_TEMPLATE_PARAMS >( false )
								{

								}

            virtual             ~TCResourceGroup()
            {

            }

			xst_fi	xst_castring&	GetGroupName() const
									{ return m_strGroupName; }

			xst_fi	ul32			GetGroupHandle() const
									{ return m_ulGroupHandle; }

			xst_fi	void			_SetGroupName(xst_castring& strName)
									{ m_strGroupName = strName; }

			xst_fi	void			_SetGroupHandle(ul32 ulHandle)
									{ m_ulGroupHandle = ulHandle; }

        protected:

			xst_astring		m_strGroupName;
			ul32			m_ulGroupHandle;

    };

#define XST_RES_GROUP	XST::TCResourceGroup< XST_RESMGR_TEMPLATE_PARAMS >
//#define XST_RESMGR_GRMGR XST::TCBaseResourceManager< TCObjectSmartPointer< XST_RES_GROUP >, TCDefaultResourceOperator< TCObjectSmartPointer< XST_RES_GROUP > >, xst_astring, ul32, TCDefaultResourceNameHasher< xst_astring, ul32 > >
#define XST_RESMGR_GRMGR XST::TCBaseResourceManager< TCObjectSmartPointer< XST_RES_GROUP >, xst_astring, ul32, TCDefaultResourceNameHasher< xst_astring, ul32 > >

    template<
        class _RESOURCE_,
		//class _OPERATOR_ = TIBaseResourceListener< _RESOURCE_ >, //XST_RESMGR_RESGROP,
		class _KEY_ = xst_astring,
		class _HASH_ = ul32,
		class _HASH_CLASS_ = XST_RESMGR_HASH
    >
    class XST_API TCResourceManager : public virtual XST_RESMGR_GRMGR, public virtual IBaseResourceManager
    {
        public:

            typedef _RESOURCE_      Resource;
            typedef XST_RES_GROUP	ResourceGroup;
            typedef TCObjectSmartPointer< ResourceGroup >   GroupPtr;
            typedef TCWeakPointer< ResourceGroup >          WeakGroupPtr;
            //typedef _OPERATOR_		OperatorClass;
            typedef _KEY_           ResourceKey;
			typedef _HASH_			ResourceHash;
            typedef _HASH_CLASS_    HashClass;
            typedef xst_astring		GroupKey;
			typedef ul32			GroupHash;
			typedef typename std::pair< GroupHash, GroupPtr >	GroupPair;

			typedef typename XST_RESMGR_GRMGR::ResourceMap      GroupMap;
			typedef typename XST_RESMGR_GRMGR::HashClass		GroupHashClass;
			typedef typename TCBaseResourceManager< XST_RESMGR_TEMPLATE_PARAMS >::ResourceMap	ResourceMap;
			typedef fastdelegate::FastDelegate1< _RESOURCE_& >	ResourceDelegate;
			typedef fastdelegate::FastDelegate1< GroupPtr& >	GroupDelegate;
			typedef typename ResourceGroup::ResourceIterator	ResourceIterator;
			typedef typename XST_RESMGR_GRMGR::Iterator			GroupIterator;

			typedef typename XST_RESMGR_GRMGR::_ResourceIterator	_GroupIterator;
			typedef typename ResourceGroup::_ResourceIterator		_ResourceIterator;

#if( XST_VS_VER < 10 )
			XST_TEMPLATE_CLASS fastdelegate::FastDelegate1< _RESOURCE_& >;
			XST_TEMPLATE_CLASS fastdelegate::FastDelegate1< GroupPtr& >;
#endif

        public:

			TCResourceManager(bool bManualDestroy = false) : XST_RESMGR_GRMGR( bManualDestroy )
			{
				_SetOnRemoveDestroyDelegates();
			}

            virtual ~TCResourceManager()
            {

            }

			xst_fi GroupIterator GetGroupIterator()
			{
				return this->GetIterator();
			}

			xst_fi	WeakGroupPtr	CreateGroup(const GroupKey& strGroupName, const bool& bReturnIfExists = true)
			{
				GroupHash groupId = GroupHashClass::GetHash( strGroupName );
                if( m_LastUsedGroupHash == groupId )
                    return m_pLastUsedGroup;

				//If resource found return null
				_GroupIterator Itr;
				if( !MapUtils::FindPlace< GroupMap, GroupHash >( this->m_mResources, groupId, &Itr ) )
				{
					if( bReturnIfExists )
					{
						return Itr->second;
					}
					else
					{
						return GroupPtr( 0 );
					}
				}

				//if not found create new
				GroupPtr pGr( xst_new ResourceGroup() );
				this->m_mResources.insert( Itr, GroupPair( groupId, pGr ) );
				pGr->_SetGroupName( strGroupName );
				pGr->_SetGroupHandle( groupId );
				pGr->_SetDelegates( this->m_OnResRemove, this->m_OnResDestroy );
                m_pLastUsedGroup = pGr;
                m_LastUsedGroupHash = groupId;
				return pGr;
			}

			xst_fi	WeakGroupPtr	GetOrCreateGroup(const GroupKey& strGroupName)
			{
				GroupHash groupId = GroupHashClass::GetHash( strGroupName );
                if( m_LastUsedGroupHash == groupId )
                    return m_pLastUsedGroup;

				//If resource found return it
				_GroupIterator Itr;
				if( !MapUtils::FindPlace< GroupMap, GroupHash >( this->m_mResources, groupId, &Itr ) )
				{
					return Itr->second;
				}

				//if not found create new
				GroupPtr pGr( xst_new ResourceGroup() );
				this->m_mResources.insert( Itr, GroupPair( groupId, pGr ) );
				pGr->_SetGroupName( strGroupName );
				pGr->_SetGroupHandle( groupId );
				pGr->_SetDelegates( this->m_OnResRemove, this->m_OnResDestroy );
                m_LastUsedGroupHash = groupId;
                m_pLastUsedGroup = pGr;
				return pGr;
			}

            xst_i  i32     AddResource(const ResourceKey& resourceName, const Resource& Res, const GroupKey& groupName, bool bCreateGroup = true)
            {
				//Get the group
				WeakGroupPtr pGr;
				if( bCreateGroup )
				{
					pGr = GetOrCreateGroup( groupName );
					if( pGr.IsNull() )
					{
						return RESULT::FAILED;
					}
				}
				else
				{
					pGr = GetGroup( groupName );
					if( pGr.IsNull() )
					{
						return RESULT::FAILED;
					}
				}

				return pGr->AddResource( resourceName, Res );
            }

			xst_fi  i32     AddResource(const ResourceKey& resourceName, const Resource& Res, const GroupPtr& pGr)
            {
				xst_assert( pGr != xst_null, "(XSTTCResourceManager::AddResource)" );
				return pGr->AddResource( resourceName, Res );
            }

			xst_i	i32	AddResourceByHandle(const ResourceHash& resourceHandle, const Resource& Res, const GroupKey& groupName, bool bCreateGroup = true)
			{
				//Get the group
				WeakGroupPtr pGr;
				if( bCreateGroup )
				{
					pGr = GetOrCreateGroup( groupName );
					if( pGr.IsNull() )
					{
						return RESULT::FAILED;
					}
				}
				else
				{
					pGr = GetGroup( groupName );
					if( pGr.IsNull() )
					{
						return RESULT::FAILED;
					}
				}

				return pGr->AddResourceByHandle( resourceHandle, Res );
			}

			xst_fi	i32	AddResourceByHandle(const ResourceHash& resourceHandle, const Resource& Res, const GroupPtr& pGr, bool bCreateGroup = true)
			{
				xst_assert( pGr != xst_null, "(XSTTCResourceManager::AddResourceByHandle)" );
				return pGr->AddResourceByHandle( resourceHandle, Res );
			}

            xst_fi  i32   GetResource(const ResourceKey& resourceName, Resource* pResOut, const GroupKey& groupName)
            {
                WeeakGroupPtr pGr = GetGroup( groupName );
				if( pGr.IsNull() )
				{
					return RESULT::FAILED;
				}

				return pGr->GetResource( resourceName, pResOut );
            }

			xst_i	WeakGroupPtr GetGroupByHandle(const GroupHash& groupHandle)
			{
                if( m_LastUsedGroupHash == groupHandle )
                    return m_pLastUsedGroup;
				_GroupIterator Itr = this->m_mResources.find( groupHandle );
				if( Itr == this->m_mResources.end() )
				{
					//XST_RESMGR_LOG( "Resource group: " << groupName << " not found." );
					return WeakGroupPtr( 0 );
				}
                m_LastUsedGroupHash = groupHandle;
                m_pLastUsedGroup = Itr->second;
				return m_pLastUsedGroup;
			}

            xst_fi  WeakGroupPtr GetGroup(const GroupKey& groupName)
            {
				GroupHash groupId = GroupHashClass::GetHash( groupName );
                return GetGroupByHandle( groupId );
            }

            xst_i i32 RemoveResource(const ResourceKey& resName, const GroupKey& groupName)
            {
                //Get the group
                WeakGroupPtr pGr = GetGroup( groupName );
				if( pGr.IsNull() )
				{
					return RESULT::FAILED;
				}

				return pGr->RemoveResource( resName );
            }

			xst_fi i32 RemoveResource(const ResourceKey& resName)
            {
				_HASH_ resId = HashClass::GetHash( resName );
				return RemoveResourceByHandle( resId );
            }

			xst_i i32 RemoveResourceByHandle(const _HASH_& resId)
            {
				/*_GroupIterator GrItr = this->m_mResources.begin();
				for(; GrItr != this->m_mResources.end(); ++GrItr)
				{
					GrItr->second->RemoveResourceByHandle( resId );
				}*/

                for( auto& GrItr : this->m_mResources )
                {
                    GrItr.second->RemoveResourceByHandle( resId );
                }

				return RESULT::OK;
            }

			xst_fi i32 DestroyResource(const ResourceKey& resName)
            {
				_HASH_ resId = HashClass::GetHash( resName );
				return DestroyResourceByHandle( resId );
            }

			i32 DestroyResourceByHandle(const _HASH_& resId)
            {
				_GroupIterator GrItr = this->m_mResources.begin();
				for(; GrItr != this->m_mResources.end(); ++GrItr)
				{
					GrItr->second->DestroyResourceByHandle( resId );
				}

				return RESULT::OK;
            }

			i32 DestroyResource(const ResourceKey& resName, const GroupKey& groupName)
            {
                //Get the group
                WeakGroupPtr pGr = GetGroup( groupName );
				if( pGr.IsNull() )
				{
					return RESULT::FAILED;
				}

				return pGr->DestroyResource( resName );
            }

			i32 DestroyResourceByHandle(const _HASH_& resId, const GroupKey& groupName)
            {
                //Get the group
                WeakGroupPtr pGr = GetGroup( groupName );
				if( pGr.IsNull() )
				{
					return RESULT::FAILED;
				}

				return pGr->DestroyResourceByHandle( resId );
            }

			i32 DestroyResourceByHandle(const _HASH_& resId, const GroupHash& groupHandle)
            {
                //Get the group
                WeakGroupPtr pGr = GetGroupByHandle( groupHandle );
				if( pGr.IsNull() )
				{
					return RESULT::FAILED;
				}

				return pGr->DestroyResourceByHandle( resId );
            }

			void DestroyResources()
			{
				_GroupIterator GrItr = this->m_mResources.begin();
				for(; GrItr != this->m_mResources.end(); ++GrItr)
				{
					GrItr->second->DestroyResources();
				}
			}

			void RemoveResources()
            {
				_GroupIterator GrItr = this->m_mResources.begin();
				for(; GrItr != this->m_mResources.end(); ++GrItr)
				{
					GrItr->second->RemoveResources();
				}
            }

			void DestroyGroups()
			{
				this->m_mResources.clear();
                m_LastUsedGroupHash = 0;
                m_pLastUsedGroup = xst_null;
			}

			void	RemoveResource(Resources::IResource* pRes)
			{

			}

		protected:

			/*void	_ForEachResource()
			{
				for(_GroupIterator GrItr = m_mResources.begin(); GrItr != m_mResources.end(); ++GrItr)
				{
					GrItr->second->SetOnForEachResource( m_OnResItr );
					GrItr->second->ForEachResource();
				}
			}

			void	_ForEachGroup()
			{
				if( m_OnGroupItr )
				{
					for(_GroupIterator GrItr = m_mResources.begin(); GrItr != m_mResources.end(); ++GrItr)
					{
						m_OnGroupItr( GrItr->second );
					}
				}
			}*/

			void _SetOnRemoveDestroyDelegates()
			{
				for(_GroupIterator GrItr = m_mResources.begin(); GrItr != m_mResources.end(); ++GrItr)
				{
					GrItr->second->_SetDelegates( m_OnResRemove, m_OnResDestroy );
				}
			}

			void _SetOnResourceIterationDelegate()
			{
				for(_GroupIterator GrItr = m_mResources.begin(); GrItr != m_mResources.end(); ++GrItr)
				{
					GrItr->second->_SetResourceIterationDelegate( m_OnResItr );
				}
			}

        protected:

            GroupHash           m_LastUsedGroupHash = 0;
            WeakGroupPtr        m_pLastUsedGroup = xst_null;
			ResourceDelegate	m_OnResRemove;
			ResourceDelegate	m_OnResDestroy;
			ResourceDelegate	m_OnResItr;
			GroupDelegate		m_OnGroupItr;
    };

}//xst

#endif
