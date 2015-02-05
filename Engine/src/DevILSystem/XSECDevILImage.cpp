#include "XSECDevILImage.h"
#include "XSECDevILSystem.h"
#include "XSEIResourceManager.h"

namespace XSE
{
	namespace DevIL
	{
		static const RS_FORMAT g_aRSFormats[ ImageFormats::_ENUM_COUNT ] = 
		{
			/*UNKNOWN = 0,
			RGB,
			RGBA,
			BGR,
			BGRA,
			LUMINANCE,*/
			RSFormats::UNKNOWN,
			RSFormats::R8G8B8U,
			RSFormats::R8G8B8A8U,
			RSFormats::B5G6R5U,
			RSFormats::B8G8R8A8,
			RSFormats::R8U
		};

		CImage::CImage(CImageSystem* pIS, XSE_IRESOURCE_DECL_PARAMS) : 
			IImage( XSE_IRESOURCE_PARAMS ),
			m_pImageSystem( pIS ), 
			m_uiWidth( 0 ),
			m_uiHeight( 0 ),
			m_uiBPP( 0 ),
			m_pData( xst_null ),
			m_ulDataSize( 0 ),
			m_uiImgId( 0 ),
			m_eDataType( ImageDataTypes::UINT_8 ),
			m_uiType( IL_UNSIGNED_BYTE ),
			_SetColor( &CImage::_SetColorRGB ),
			m_bDestroyed( false ),
			m_uiBytesPerChannel( 0 )
		{
		}

		CImage::~CImage() 
		{
			ClearResource();
		}

		i32 CImage::ClearResource()
		{
			if( !m_bDestroyed )
			{
				m_pImageSystem->DestroyImage( this );
				m_pImageSystem = xst_null;
				m_bDestroyed = true;
			}
			return XST_OK;
		}

		ImagePtr CImage::Clone() const
		{
			return this->m_pResourceCreator->CloneResource( this );
		}

		void CImage::GetColor(cu32& uiPosition, XST::CColor* pColorOut) const
		{
			xst_assert( m_ulDataSize > ( uiPosition * m_uiBytesPerPixel ), "(CImage::GetColor) Color position out of bounds" );
			xst_assert( m_uiBytesPerChannel > 0, "(CImage::GetColor) Image data not prepared" );
			//Calc position
			u8* pPtr = m_pData;
			pPtr += ( uiPosition * m_uiBytesPerPixel );

			pColorOut->r = *pPtr;
			pPtr += m_uiBytesPerChannel;
			pColorOut->g = *pPtr;
			pPtr += m_uiBytesPerChannel;
			pColorOut->b = *pPtr;
			pPtr += m_uiBytesPerChannel;
		}

		u8 CImage::GetChannelColor(cu32& uiPos, const IImage::CHANNEL& eChannel) const
		{
			xst_assert( m_ulDataSize > ( uiPos * m_uiBytesPerPixel ), "(CImage::GetChannelColor) Color position out of bounds" );
			xst_assert( m_uiBytesPerChannel > 0, "(CImage::GetChannelColor) Image data not prepared" );
			xst_assert( (u32)eChannel < m_uiChannelCount, "(CImage::GetChannelColor) Channel out of bounds" );

			//Calc offset position
			u8* pPtr = m_pData;
			pPtr += ( uiPos * m_uiBytesPerPixel );
			pPtr += (u32)eChannel * m_uiBytesPerChannel;
			return *pPtr;
		}

		i32 CImage::Save(xst_castring& strFileName)
		{
			return this->m_pImageSystem->SaveImage( strFileName, this );
		}

		void CImage::Scale(cu32& uiWidth, cu32& uiHeight)
		{
			this->m_pImageSystem->ScaleImage( this, uiWidth, uiHeight );
		}

		void CImage::SetColor(cu32& uiPosition, const XST::CColor& Color)
		{
			xst_assert2( m_ulDataSize > ( uiPosition * m_uiBytesPerPixel ) );
			//Calc position
			u8* pPtr = m_pData;
			pPtr += ( uiPosition * m_uiBytesPerPixel );

			XST_CALL_MEMBER_FN_PTR( this, _SetColor )( pPtr, Color );
		}

		void CImage::SetSize(cu32& uiWidth, cu32& uiHeight)
		{
			m_uiWidth = uiWidth;
			m_uiHeight = uiHeight;
		}

		void CImage::SetDataType(IMAGE_DATA_TYPE eType)
		{
			m_eDataType = eType;
			m_uiType = m_pImageSystem->m_aiDataTypes[ eType ];
		}

		void CImage::SetBitsPerPixel(IMAGE_BPP_TYPE eType)
		{
			m_uiBPP = eType;
			m_uiBytesPerPixel = m_uiBPP / 8;
		}

		void CImage::SetFormat(IMAGE_FORMAT eFormat)
		{
			m_eFormat = eFormat;
			m_uiFormat = m_pImageSystem->m_aiFormats[ eFormat ];
			//Set defaults
			if( m_uiBPP == 0 )
			{
				m_uiBytesPerPixel = m_pImageSystem->m_aiFormatSizes[ eFormat ];
				m_uiBPP = m_uiBytesPerPixel * 8;
			}
		}

		RS_FORMAT CImage::GetRenderSystemFormat() const
		{
			return g_aRSFormats[ m_eFormat ];
		}

		void CImage::_SetColorR(u8* pPtr, const XST::CColor& Color)
		{
			*pPtr = Color.r;
		}

		void CImage::_SetColorRGB(u8* pPtr, const XST::CColor& Color)
		{
			*pPtr = Color.r;
			pPtr += m_uiBytesPerChannel;
			*pPtr = Color.g;
			pPtr += m_uiBytesPerChannel;
			*pPtr = Color.b;
		}

		void CImage::_SetColorRGBA(u8* pPtr, const XST::CColor& Color)
		{
			*pPtr = Color.r;
			pPtr += m_uiBytesPerChannel;
			*pPtr = Color.g;
			pPtr += m_uiBytesPerChannel;
			*pPtr = Color.b;
			pPtr += m_uiBytesPerChannel;
			*pPtr = Color.a;
		}

		void CImage::_SetColorBGR(u8* pPtr, const XST::CColor& Color)
		{
			*pPtr = Color.b;
			pPtr += m_uiBytesPerChannel;
			*pPtr = Color.g;
			pPtr += m_uiBytesPerChannel;
			*pPtr = Color.r;
		}

		void CImage::_SetColorBGRA(u8* pPtr, const XST::CColor& Color)
		{
			*pPtr = Color.b;
			pPtr += m_uiBytesPerChannel;
			*pPtr = Color.g;
			pPtr += m_uiBytesPerChannel;
			*pPtr = Color.r;
			pPtr += m_uiBytesPerChannel;
			*pPtr = Color.a;
		}

	}//devil
}//xse