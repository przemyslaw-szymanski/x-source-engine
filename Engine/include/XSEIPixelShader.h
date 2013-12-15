#ifndef XSE_IPIXEL_SHADER_H
#define XSE_IPIXEL_SHADER_H

#include "XSEIShader.h"
#include "XSECommon.h"

namespace XSE
{
	namespace Resources
	{
		class IPixelShader : public IShader
		{
			public:

								IPixelShader(IShaderSystem* pSS, XSE_IRESOURCE_DECL_PARAMS) : IShader( pSS, ShaderTypes::PIXEL, XSE_IRESOURCE_PARAMS ) {}
				virtual			~IPixelShader() {}

			protected:
		};
	}//resources

	typedef XST::TCObjectSmartPointer< Resources::IPixelShader >	PixelShaderPtr;
	XST_TEMPLATE_CLASS XST::TCObjectSmartPointer< Resources::IPixelShader >;

}//xse

#endif