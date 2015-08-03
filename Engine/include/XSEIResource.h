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

        #define XSE_IRESOURCE_DECL_PARAMS_DEFAULT	XSE::IResourceManager* pCreator = xst_null, const Handle& Handle = 0, const Name& strName = "", XST::Types::ci32& iType = 0, XST::Types::ci32& iState = 0
        #define XSE_IRESOURCE_DECL_PARAMS			XSE::IResourceManager* pCreator, const Handle& Handle, const Name& strName, XST::Types::ci32& iType, XST::Types::ci32& iState
        #define	XSE_IRESOURCE_PARAMS				pCreator, Handle, strName, iType, iState
        #define	XSE_IRESOURCE_CTOR					XSE::Resources::IResource( XSE_IRESOURCE_PARAMS )

        class XST_API IResource : public virtual XST::IObject //XST::Resources::IResource
        {
            friend class XSE::IResourceManager;
            friend class XSE::IResourceGroup;
            friend class XSE::IResourceManager2;
            
            public:

                typedef XST::TCObjectSmartPointer< IResource >	ResourcePtr;
                typedef XST::TCWeakPointer< IResource >         ResourceWeakPtr;
                typedef ul32                                    Handle;
                typedef xst_astr32                              Name;
            
            public:
                
                                        IResource() { /*xst_assert( 0, "(IResource::IResource) Do not use this constructor" );*/ }
                                        IResource(XSE::IResourceManager* pCreator, const Handle& Handle, const Name& strName, 
                                                  XST::Types::ci32& iType, XST::Types::ci32& iState) :
#if (XST_OBJ_DEBUG)
                                                  m_strName(strName)
#endif
                                                  , m_ResourceHandle( Handle )
                                                  , m_ResourceGroupHandle( Handle )
                                                  , m_pResourceCreator( pCreator )
                                                  , m_iResourceType( iType )
                                                  , m_iResourceState( iState )
                                        {}
                
                virtual					~IResource() 
                                        {
                                            ClearResource();
                                        }

                xst_fi	Handle			GetResourceHandle() const
                                        { return m_ResourceHandle; }

                xst_fi  Handle          GetResourceGroupHandle() const
                                        { return m_ResourceGroupHandle; }

                xst_fi  const Name&     GetResourceName() const 
                                        { return m_strName; }

                xst_fi  i32             GetResourceState() const 
                                        { return m_iResourceState; }

                /*xst_fi	
                const XST::FilePtr		GetResourceFile() const
                                        { return this->m_pResourceFile; }*/

                xst_fi	
                const ResFileWeakPtr	GetResourceFile() const
                                        { return m_pResourceFile; }

                xst_fi	IResourceManager*	GetResourceCreator() const
                                            { return m_pResourceCreator; }

                virtual ResourcePtr		CloneResource(xst_castring& strName = XST::StringUtil::EmptyAString, bool bFullClone = true);

                xst_i	void			SetResource(const IResource* pOther)
                                        {
                                            m_pResourceFile = pOther->m_pResourceFile;
                                            m_iResourceState = pOther->m_iResourceState;
                                            m_iResourceType = pOther->m_iResourceType;
                                        }

                xst_fi	void			SetResource(const ResourcePtr pOther)
                                        { SetResource( pOther.GetPtr() ); }

                virtual	i32				ClearResource();

            protected:

                xst_fi	void			_Init(XSE::IResourceManager* pCreator, const Handle& Handle, const Name& strName, XST::Types::ci32& iType, XST::Types::ci32& iState)
                {
                    m_pResourceCreator = pCreator;
                    m_ResourceHandle = Handle;
                    m_ResourceGroupHandle = 0;
                    m_strName = strName;
                    m_iResourceType = iType;
                    m_iResourceState = iState;
                }

                xst_fi  void            _SetResourceState(ci32& iState)
                {
                     m_iResourceState = iState;
                }

                xst_fi  void            _SetResourceName(const Name& strName)
                {
                    //this->m_strResourceName = strName;
                    XST_SET_DBG_NAME( this, strName.c_str() );
                }

                xst_fi  void            _SetResourceFile(ResFileWeakPtr pFile)
                {
                    m_pResourceFile = pFile;
                }

                xst_fi  void            _SetResourceType(ci32& iType)
                {
                    this->m_iResourceType = iType;
                }

            protected:
#if (XST_OBJ_DEBUG)
                Name                m_strName;
#endif
                IResourceManager*	m_pResourceCreator = xst_null;
                ResFilePtr			m_pResourceFile;
                Handle				m_ResourceHandle = 0;
                Handle				m_ResourceGroupHandle = 0;
                i32                 m_iResourceState;
                i32                 m_iResourceType;
        };

    }//resources

    typedef Resources::IResource::ResourcePtr	    ResourcePtr;
    typedef Resources::IResource::ResourceWeakPtr	ResourceWeakPtr;
    XST_TEMPLATE_CLASS XST::TCObjectSmartPointer< Resources::IResource >;
    XST_TEMPLATE_CLASS XST::TCWeakPointer< Resources::IResource >;

}//xse

#endif
