#ifndef XSE_ITEXTURE_H
#define XSE_ITEXTURE_H

#include "XSECommon.h"

namespace XSE
{
	namespace Resources
	{
		class ITexture : public IResource
		{
			public:

								ITexture(IRenderSystem* pRS, XSE_IRESOURCE_DECL_PARAMS_DEFAULT) : XSE_IRESOURCE_CTOR {}
				virtual			~ITexture() {}
		};

	}//resources

	typedef XST::TCObjectSmartPointer< Resources::ITexture >	TexturePtr;
	XST_TEMPLATE_CLASS XST::TCObjectSmartPointer< Resources::ITexture >;

}//xse

#endif