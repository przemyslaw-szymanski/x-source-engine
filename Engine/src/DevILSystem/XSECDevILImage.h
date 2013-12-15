#ifndef XSE_CDEVIL_IMAGE_H
#define XSE_CDEVIL_IMAGE_H

#include "XSEIImage.h"
#include "../DevIL/ilu.h"

namespace XSE
{
	namespace DevIL
	{
		using namespace XSE::Resources;

		class CImageSystem;

		class CImage : public IImage
		{
			friend class CImageSystem;

			typedef void (CImage::*pfnSetColor)(u8* pPtr, const XST::CColor& Color);
			public:

											CImage(CImageSystem* pIS, XSE_IRESOURCE_DECL_PARAMS);
				virtual						~CImage();

						void				DestroyData();

						ImagePtr			Clone() const;

						void				GetColor(cu32& uiPosition, XST::CColor* pColorOut) const;
						XST::CColor			GetColor(cu32& uiX, cu32& uiY) const
											{ return GetColor( m_uiHeight * uiY + uiX ); }
						XST::CColor			GetColor(cu32& uiPosition) const
											{ XST::CColor Color; GetColor( uiPosition, &Color ); return Color; }
						void				GetColor(cu32& uiX, cu32& uiY, XST::CColor* pColorOut) const
											{ GetColor( m_uiHeight * uiY + uiX, pColorOut ); }

						void				SetColor(cu32& uiPosition, const XST::CColor& Color);
						void				SetColor(cu32& uiX, cu32& uiY, const XST::CColor& Color)
											{ SetColor( m_uiHeight * uiY + uiX, Color ); }

						void				Scale(cu32& uiWidth, cu32& uiHeight);

						void				SetSize(cu32& uiWidth, cu32& uiHeight);

						i32					Save(xst_castring& strFileName);

						u32					GetPixelCount() const
											{ return m_uiPixelCount; }
						
						u32					GetWidth() const
											{ return m_uiWidth; }

						u32					GetHeight() const
											{ return m_uiHeight; }
						
						u32					GetBitsPerPixel() const
											{ return m_uiBPP; }

						void				SetBitsPerPixel(IMAGE_BPP_TYPE eType);
						
						cu8*				GetData() const
											{ return m_pData; }

						ul32				GetDataSize() const
											{ return m_ulDataSize; }

						bool				IsAutoDestroy() const
											{ return true; }

						IMAGE_FORMAT		GetFormat() const
											{ return m_eFormat; }

						void				SetFormat(IMAGE_FORMAT eFormat);

						bool				IsManual() const
											{ return m_bManual; }

						void				SetDataType(IMAGE_DATA_TYPE eType);

			protected:

						void				_SetColorR(u8* pPtr, const XST::CColor& Color);
						void				_SetColorRGB(u8* pPtr, const XST::CColor& Color);
						void				_SetColorRGBA(u8* pPtr, const XST::CColor& Color);
						void				_SetColorBGR(u8* pPtr, const XST::CColor& Color);
						void				_SetColorBGRA(u8* pPtr, const XST::CColor& Color);
						pfnSetColor			_SetColor;

			protected:

				u8*				m_pData;
				u8*				m_pTmpData;
				ul32			m_ulDataSize;
				ILuint			m_uiImgId;
				CImageSystem*	m_pImageSystem;
				u32				m_uiWidth;
				u32				m_uiHeight;
				u32				m_uiBPP;
				u32				m_uiBytesPerPixel;
				ILenum			m_uiFormat;
				ILenum			m_uiType;
				IMAGE_FORMAT	m_eFormat;
				IMAGE_DATA_TYPE	m_eDataType;
				u32				m_uiChannelCount;
				u32				m_uiBytesPerChannel;
				u32				m_uiPixelCount;
				bool			m_bManual;
				bool			m_bDestroyed;
		};
	}//devil
}//xse

#endif