#ifndef XSE_CTEXTURE_H
#define XSE_CTEXTURE_H

#include "XSEIResource.h"
#include "XSEIImage.h"
#include "XSERenderSystemCommon.h"

namespace XSE
{
	namespace Resources
	{
		class XST_API CTexture : public IResource
		{
			friend class IRenderSystem;
			friend class IResourceManager;
			friend class CTextureManager;

			public:

											CTexture(IRenderSystem* pRS, XSE_IRESOURCE_DECL_PARAMS);
					virtual					~CTexture();

					xst_fi	TEXTURE_TYPE	GetTextureType() const
											{ return m_eTexType; }

					xst_fi 
					const ImageWeakPtr		GetImage() const
											{ return m_pImg; }

					xst_fi	RS_FORMAT		GetRenderSystemFormat() const
											{ return m_eRSFormat; }

			protected:

				xst_fi	void				_SetRenderSystemHandle(RSHandleRef Handle)
											{ m_RSHandle = Handle; }

				xst_fi	void				_SetTextureType(TEXTURE_TYPE eType)
											{ m_eTexType = eType; }

				xst_fi	void				_SetRenderSystemFormat(RS_FORMAT eFmt)
											{ m_eRSFormat = eFmt; }

				xst_fi	void				_SetImage(ImageWeakPtr pImg)
											{ m_pImg = pImg; }

			protected:

				RSHandle		m_RSHandle;
				ImagePtr		m_pImg;
				IRenderSystem*	m_pRS;
				TEXTURE_TYPE	m_eTexType = TextureTypes::UNKNOWN;
				RS_FORMAT		m_eRSFormat = RSFormats::UNKNOWN;
		};
	} // Resources

	typedef XST::TCObjectSmartPointer< Resources::CTexture >	TexturePtr;
	typedef XST::TCWeakPointer< Resources::CTexture >			TextureWeakPtr;

} // xse

#endif // XSE_CTEXTURE_H