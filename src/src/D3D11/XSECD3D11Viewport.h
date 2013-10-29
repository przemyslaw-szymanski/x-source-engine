#ifndef XSE_CD3D11_VIEWPORT_H
#define XSE_CD3D11_VIEWPORT_H

#include "XSEIViewport.h"
#include "XSED3D11Common.h"

#if defined (XST_WINDOWS)

namespace XSE
{
	namespace D3D11
	{
		class CViewport : public IViewport
		{
			friend class CRenderSystem;

			public:

								CViewport(IRenderSystem* pRS);
				virtual			~CViewport();

				void			Clear();

				void			EnableZBuffer();

				void			DisableZBuffer();

				FILL_MODE		GetFillMode() const
								{ return m_eFillMode; }

				void			SetFillMode(FILL_MODE eMode);

				void			SetBackgroundColor(const XST::CColor& Color)
				{
					m_BGColor = Color;
					m_afBGColor[ 0 ] = (f32)m_BGColor.r / UCHAR_MAX;
					m_afBGColor[ 1 ] = (f32)m_BGColor.g / UCHAR_MAX;
					m_afBGColor[ 2 ] = (f32)m_BGColor.b / UCHAR_MAX;
					m_afBGColor[ 3 ] = (f32)m_BGColor.a / UCHAR_MAX;
				}

			protected:

					i32			_Create(ID3D11Device* pDevice, IDXGISwapChain* pSwapChain, const SViewportOptions& Options);

					void		_Destroy();

			protected:

				//Viewport
				D3D11_VIEWPORT				m_D3DViewport;
				//Render target view
				ID3D11RenderTargetView*		m_pD3DRTView;
				//Depth stencil view
				ID3D11DepthStencilView*		m_pD3DDSView;
				//Rasterizer state
				ID3D11RasterizerState*		m_pD3DRState;
				//Depth stencil states
				//0 - enabled
				//1 - disabled
				ID3D11DepthStencilState*	m_apDSStates[2];
				//Render system
				CRenderSystem*				m_pRS;
				//Background color
				f32							m_afBGColor[4];
				//Viewport options
				SViewportOptions			m_Options;
				//Fill mode
				FILL_MODE					m_eFillMode;
		};

	}//d3d11
}//xse

#endif //XST_WINDOWS

#endif //XSE_CD3D11_VIEWPORT_H