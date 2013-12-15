#ifndef XSE_CD3D11_RENDER_SYSTEM_H
#define XSE_CD3D11_RENDER_SYSTEM_H

#include "XSEIRenderSystem.h"

#if defined (XST_WINDOWS) && defined (XSE_DX11)
#include <d3d11.h>
#include <d3dx11.h>

namespace XSE
{
	#if defined (XST_VISUAL_STUDIO)
	#	define XSE_RS_API	__declspec(dllexport)
	#else
	#	error "Support gcc"
	#endif

	namespace D3D11
	{
		class XSE_RS_API CRenderSystem : public XSE::IRenderSystem
		{
			enum
			{
				D3D11,
				D3DX11,
				LIB_COUNT
			};

			public:

									CRenderSystem(xst_castring& strName);
				virtual				~CRenderSystem();

						i32			Init(const SRenderSystemOptions& Options);

			protected:

						void		_UnloadLibraries();

			protected:

				xst_unknown				m_ahDlls[ LIB_COUNT ];
				D3D_DRIVER_TYPE			m_eDriverType;
				D3D_FEATURE_LEVEL		m_eFeatureLevel;
				ID3D11Device*			m_pDevice;
				ID3D11DeviceContext*	m_pDeviceContext;
				IDXGISwapChain*			m_pSwapChain;
				ID3D11RenderTargetView*	m_pRenderTargetView;
		};

	}//d3d11
}//xse
#endif //XST_VISUAL_STUDIO
#endif //XSE_CD3D11_RENDER_SYSTEM_H