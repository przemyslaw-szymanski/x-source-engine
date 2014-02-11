#ifndef XST_IRESOURCE_H
#define XST_IRESOURCE_H

#include "XSTTCData.h"
#include "XSTIObject.h"
#include "XSTIFileLoader.h"
#include "XSTIAllocator.h"
#include "XSTTIAllocableObject.h"

namespace XST
{
	class IBaseResourceManager;

	struct XST_API ResourceStates
	{
		enum
		{
			UNKNOWN = 0,
			CREATED,
			LOADED,
			PREPARED
		};
	};

	struct XST_API ResourcePriority
	{
		enum
		{
			UNKNOWN = 0,
			LOW,
			MEDIUM,
			HIGH
		};
	};

	struct XST_API ResourceType
	{
		enum TYPE
		{
			UNKNOWN = 0,
			LUA_SCRIPT,
			SOUND,
			TEXTURE,
			MESH,
			MATERIAL,
			SHADER
		};
	};

	namespace Resources
	{
		#define XST_IRESOURCE_DECL_PARAMS_DEFAULT XST::Types::xst_castring& strName = XST::StringUtil::EmptyAString, XST::Types::ci32& iType = 0, XST::Types::ci32& iState = 0, XST::IAllocator* pAllocator = xst_null
		#define XST_IRESOURCE_DECL_PARAMS XST::Types::xst_castring& strName, XST::Types::ci32& iType, XST::Types::ci32& iState, XST::IAllocator* pAllocator
		#define XST_IRESOURCE_PARAMS strName, iType, iState, pAllocator
		#define XST_IRESOURCE_CTOR XST::Resources::IResource( strName, iType, iState, pAllocator ) 

		class XST_API IResource : public virtual IObject
		{
			public:

				typedef xst_castring	Name;

			public:

									IResource() : m_iResourceType(0), m_iResourceState(0), m_pResourceAllocator(xst_null) {}
									/*IResource(ul32 _ulID) : m_ulID(_ulID) {}
									IResource(const xst_string &_strName) : m_ulID(0), m_strName(_strName) {}
									IResource(ul32 _ulID, const xst_string &_strName) : m_ulID(_ulID), m_strName(_strName) {}*/
									IResource( XST_IRESOURCE_DECL_PARAMS ) : m_strResourceName( strName ), m_iResourceType( iType ), m_iResourceState( iState ), m_pResourceAllocator( pAllocator ) 
									{
										XST_SET_DBG_NAME( this, strName.c_str() );
									}

			virtual					~IResource() 
									{
										m_pResourceFile = xst_null;
										m_iResourceType = 0;
										m_iResourceState = 0;
										m_pResourceAllocator = xst_null;
									}

			virtual
			xst_fi const Name&		GetResourceName() const
									{ return m_strResourceName; }

			xst_fi FilePtr			GetResourceFile() const
									{ return m_pResourceFile; }

			xst_fi i32				GetResourceType() const
									{ return m_iResourceType; }

			xst_fi i32				GetResourceState() const
									{ return m_iResourceState; }

			virtual	void			DestroyData() {}

			protected:

				Name				m_strResourceName;
				FilePtr				m_pResourceFile;
				i32					m_iResourceType;
				i32					m_iResourceState;
				IAllocator*			m_pResourceAllocator;
		};
	}//resources

}//XST

#endif
