#ifndef XSE_IIMAGE_H
#define XSE_IIMAGE_H

#include "XSEIResource.h"
#include "XSERenderSystemCommon.h"

namespace XSE
{
	namespace Resources
	{
		class XST_API IImage : public IResource
		{
			friend class CImageManager;

			public:

				enum class CHANNEL : u32
				{
					RED,
					GREEN,
					BLUE,
					ALPHA,
					_MAX
				};

				typedef XST::TCObjectSmartPointer< IImage >	ImagePtr;
                typedef XST::TCWeakPointer< IImage >	    ImageWeakPtr;

			public:

											IImage(XSE_IRESOURCE_DECL_PARAMS) : XSE_IRESOURCE_CTOR {}
				virtual						~IImage() {}

				virtual ImagePtr			Clone() const = 0;
				virtual	XST::CColor			GetColor(cu32& uiX, cu32& uiY) const = 0;
				virtual XST::CColor			GetColor(cu32& uiPosition) const = 0;
				virtual void				GetColor(cu32& uiPosition, XST::CColor* pColorOut) const = 0;
				virtual void				GetColor(cu32& uiX, cu32& uiY, XST::CColor* pColorOut) const = 0;
				virtual u8					GetChannelColor(cu32& uiPosition, const IImage::CHANNEL& eChannel) const = 0;
				virtual u8					GetChannelColor(cu32& uiX, cu32& uiY, const IImage::CHANNEL& eChannel) const = 0;
				virtual u32					GetPixelCount() const = 0;
				virtual void				SetColor(cu32& uiX, cu32& uiY, const XST::CColor& Color) = 0;
				virtual void				SetColor(cu32& uiPosition, const XST::CColor& Color) = 0;
				virtual void				SetSize(cu32& uiWidth, cu32& uiHeight) = 0;
				virtual u32					GetWidth() const = 0;
				virtual u32					GetHeight() const = 0;
				virtual u32					GetBitsPerPixel() const = 0;
				virtual void				SetBitsPerPixel(IMAGE_BPP_TYPE eType) = 0;
				virtual cu8*				GetData() const = 0;
				virtual ul32				GetDataSize() const = 0;
				virtual IMAGE_FORMAT		GetFormat() const = 0;
				virtual RS_FORMAT			GetRenderSystemFormat() const = 0;
				virtual void				SetFormat(IMAGE_FORMAT eFormat) = 0;
				virtual bool				IsManual() const = 0;
				virtual void				SetDataType(IMAGE_DATA_TYPE eType) = 0;
				virtual void				Scale(cu32& uiWidth, cu32& uiHeight) = 0;
				virtual i32					Save(xst_castring& strFileName) = 0;

		};
	}//resources

	typedef Resources::IImage::ImagePtr	    ImagePtr;
    typedef Resources::IImage::ImageWeakPtr	ImageWeakPtr;
	typedef Resources::IImage::CHANNEL	COLOR_CHANNEL;

}//xse

#endif