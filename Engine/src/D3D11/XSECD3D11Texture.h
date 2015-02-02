#ifndef XSE_CD3D11_TEXTURE_H
#define XSE_CD3D11_TEXTURE_H

#include "XSEITexture.h"
#include "XSED3D11Common.h"

#if defined (XST_WINDOWS)
namespace XSE
{
	namespace D3D11
	{
		class CRenderSystem;

		class CTexture : public Resources::ITexture
		{
			public:
									CTexture(CRenderSystem* pRS, XSE_IRESOURCE_DECL_PARAMS_DEFAULT);
				virtual				~CTexture();

				virtual xst_fi
				IRenderSystem*		GetRenderSystem() const xst_implement
									{ return m_pRS; }

			protected:

				CRenderSystem*		m_pRS		= xst_null;
				ID3D11ShaderResourceView*	m_pResView = xst_null;
				ID3D11Texture1D*	m_pTex1D	= xst_null;
				ID3D11Texture2D*	m_pTex2D	= xst_null;
				ID3D11Texture3D*	m_pTex3D	= xst_null;
		};
	} // D3D11
} // XSE
#endif // XST_WINDOWS

#endif // XSE_CD3D11_TEXTURE_H