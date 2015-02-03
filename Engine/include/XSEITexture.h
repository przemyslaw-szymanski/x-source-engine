#ifndef XSE_ITEXTURE_H
#define XSE_ITEXTURE_H

#include "XSEIImage.h"

namespace XSE
{
	/*struct TextureTypes
	{
		enum TYPE
		{
			UNKNOWN,
			TEXTURE_1D,
			TEXTURE_2D,
			TEXTURE_3D,
			TEXTURE_CUBE,
			_ENUM_COUNT
		};
	};
	typedef TextureTypes::TYPE TEXTURE_TYPE;*/

	namespace Resources
	{
		// Deprecated
		class ITexture : public Resources::IResource
		{
			public:

									ITexture(XSE_IRESOURCE_DECL_PARAMS_DEFAULT) : IResource(XSE_IRESOURCE_PARAMS) {}
				virtual				~ITexture() {}

				virtual xst_fi
				IRenderSystem*		GetRenderSystem() const = 0;

				virtual xst_fi
				ImageWeakPtr		GetImage() const
									{ return m_pImg; }

			protected:

				ImagePtr		m_pImg;
		};

	}//resources

	typedef XST::TCObjectSmartPointer< Resources::ITexture >	TexturePtr;
	XST_TEMPLATE_CLASS XST::TCObjectSmartPointer< Resources::ITexture >;

}//xse

#endif