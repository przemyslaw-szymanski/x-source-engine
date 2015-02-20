#ifndef XSE_IRESOURCE_H
#define XSE_IRESOURCE_H

#include "XSECommon.h"
#include "XSECFile.h"

namespace XSE
{
	namespace Resources
	{
		using namespace XST;

		#define XSE_INVALID_HANDLE	0

		#define XSE_IRESOURCE_DECL_PARAMS_DEFAULT	XSE::IResourceManager* pCreator, const Handle& Handle = XSE_INVALID_HANDLE, XST_IRESOURCE_DECL_PARAMS_DEFAULT
		#define XSE_IRESOURCE_DECL_PARAMS			XSE::IResourceManager* pCreator, const Handle& Handle, XST_IRESOURCE_DECL_PARAMS
		#define	XSE_IRESOURCE_PARAMS				pCreator, Handle, XST_IRESOURCE_PARAMS
		#define	XSE_IRESOURCE_CTOR					XSE::Resources::IResource( XSE_IRESOURCE_PARAMS )

		class XST_API IResource : public XST::Resources::IResource
		{
			friend class XSE::IResourceManager;
            friend class XSE::IResourceGroup;
            friend class XSE::IResourceManager2;
			
			public:

				typedef XST::TCObjectSmartPointer< IResource >	ResourcePtr;
                typedef XST::TCWeakPointer< IResource >         ResourceWeakPtr;
                typedef ul32                                    Handle;
			
			public:
				
										IResource() { /*xst_assert( 0, "(IResource::IResource) Do not use this constructor" );*/ }
										IResource(XSE_IRESOURCE_DECL_PARAMS_DEFAULT) : m_ResourceHandle( Handle ), m_pResourceCreator( pCreator ), XST_IRESOURCE_CTOR {}
				
				virtual					~IResource() 
										{
											m_ResourceHandle = 0;
											m_pResourceCreator = xst_null;
											m_ResourceGroupHandle = 0;
										}

				xst_fi	Handle			GetResourceHandle() const
										{ return m_ResourceHandle; }

                xst_fi  Handle          GetResourceGroupHandle() const
                                        { return m_ResourceGroupHandle; }

				xst_fi	
				const XST::FilePtr		GetResourceFile() const
										{ return this->m_pResourceFile; }

				xst_fi	IResourceManager*	GetResourceCreator() const
											{ return m_pResourceCreator; }

				virtual ResourcePtr		CloneResource(xst_castring& strName = XST::StringUtil::EmptyAString, bool bFullClone = true);

				xst_i	void			SetResource(const IResource* pOther)
										{
											this->m_pResourceFile = pOther->m_pResourceFile;
											this->m_iResourceState = pOther->m_iResourceState;
											this->m_iResourceType = pOther->m_iResourceType;
										}

				xst_fi	void			SetResource(const ResourcePtr pOther)
										{ SetResource( pOther.GetPtr() ); }

				virtual	i32				ClearResource() xst_implement;

            protected:

				xst_fi	void			_Init(XSE::IResourceManager* pCreator, const Handle& Handle, const Name& strName, XST::Types::ci32& iType, XST::Types::ci32& iState)
				{
					XST::Resources::IResource::_Init( strName, iType, iState );
					m_pResourceCreator = pCreator;
					m_ResourceHandle = Handle;
				}

                xst_fi  void            _SetResourceState(ci32& iState)
                {
                     this->m_iResourceState = iState;
                }

                xst_fi  void            _SetResourceName(xst_castring& strName)
                {
                    //this->m_strResourceName = strName;
					XST_SET_DBG_NAME( this, strName.c_str() );
                }

                xst_fi  void            _SetResourceFile(FilePtr pFile)
                {
                    this->m_pResourceFile = pFile;
                }

                xst_fi  void            _SetResourceType(ci32& iType)
                {
                    this->m_iResourceType = iType;
                }

			protected:

				IResourceManager*	m_pResourceCreator = xst_null;
				ResFilePtr			m_pFile;
				Handle				m_ResourceHandle = 0;
				Handle				m_ResourceGroupHandle = 0;
		};

	}//resources

	typedef Resources::IResource::ResourcePtr	    ResourcePtr;
    typedef Resources::IResource::ResourceWeakPtr	ResourceWeakPtr;
	XST_TEMPLATE_CLASS XST::TCObjectSmartPointer< Resources::IResource >;
    XST_TEMPLATE_CLASS XST::TCWeakPointer< Resources::IResource >;

}//xse

#endif
