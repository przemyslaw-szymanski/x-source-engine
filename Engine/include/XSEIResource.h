#ifndef XSE_IRESOURCE_H
#define XSE_IRESOURCE_H

#include "XSECommon.h"

namespace XSE
{

	namespace Resources
	{
		using namespace XST;

		#define XSE_INVALID_HANDLE	0

		#define XSE_IRESOURCE_DECL_PARAMS_DEFAULT	XSE::IResourceManager* pCreator, XST::Types::cul32& ulHandle = XSE_INVALID_HANDLE, XST_IRESOURCE_DECL_PARAMS_DEFAULT
		#define XSE_IRESOURCE_DECL_PARAMS			XSE::IResourceManager* pCreator, XST::Types::cul32& ulHandle, XST_IRESOURCE_DECL_PARAMS
		#define	XSE_IRESOURCE_PARAMS				pCreator, ulHandle, XST_IRESOURCE_PARAMS
		#define	XSE_IRESOURCE_CTOR					XSE::Resources::IResource( XSE_IRESOURCE_PARAMS )

		class XST_API IResource : public XST::Resources::IResource
		{
			friend class XSE::IResourceManager;
			
			public:

				typedef XST::TCObjectSmartPointer< IResource >	ResourcePtr;
			
			public:
				
										IResource() { xst_assert( 0, "(IResource::IResource) Do not use this constructor" ); }
										IResource(XSE_IRESOURCE_DECL_PARAMS_DEFAULT) : m_ulResourceHandle( ulHandle ), m_pResourceCreator( pCreator ), XST_IRESOURCE_CTOR {}
				
				virtual					~IResource() 
										{
											m_ulResourceHandle = 0;
											m_pResourceCreator = xst_null;
											m_ulResourceGroupId = 0;
										}

				xst_fi	ul32			GetResourceHandle()
										{ return m_ulResourceHandle; }

				xst_fi	const FilePtr	GetResourceFile() const
										{ return this->m_pResourceFile; }

				xst_fi	IResourceManager*	GetResourceCreator()
											{ return m_pResourceCreator; }

				virtual ResourcePtr		Clone(xst_castring& strName = XST::StringUtil::EmptyAString, bool bFullClone = true);

				xst_i	void			SetResource(const IResource* pOther)
										{
											this->m_pResourceFile = pOther->m_pResourceFile;
											this->m_iResourceState = pOther->m_iResourceState;
											this->m_iResourceType = pOther->m_iResourceType;
										}

				xst_fi	void			SetResource(const ResourcePtr& pOther)
										{ SetResource( pOther.GetPointer() ); }

            protected:

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
				ul32				m_ulResourceHandle = 0;
				ul32				m_ulResourceGroupId = 0;
		};

	}//resources

	typedef Resources::IResource::ResourcePtr	ResourcePtr;
	XST_TEMPLATE_CLASS XST::TCObjectSmartPointer< Resources::IResource >;

}//xse

#endif
