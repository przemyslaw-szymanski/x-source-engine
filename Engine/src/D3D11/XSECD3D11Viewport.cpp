#include "XSECD3D11Viewport.h"
#include "XSECD3D11RenderSystem.h"

#if defined (XST_WINDOWS)
namespace XSE
{
	namespace D3D11
	{

		//Matrix for 2D rendering
		//XMMATRIX	g_mtxOrtho;

		CViewport::CViewport(IRenderSystem* pRS) : 
			IViewport( pRS ), 
			m_pD3DRTView( xst_null ),
			m_pD3DDSView( xst_null ),
			m_pD3DRState( xst_null ),
			m_pRS( (CRenderSystem*)pRS )
		{
			
		}

		CViewport::~CViewport()
		{
			_Destroy();
		}

		void CViewport::SetFillMode(FILL_MODE eMode)
		{
			m_eFillMode = eMode;
			ID3D11RasterizerState* pRState;
			D3D11_RASTERIZER_DESC RDesc;
			m_pRS->m_pDeviceContext->RSGetState( &pRState );
			pRState->GetDesc( &RDesc );
			RDesc.FillMode = m_pRS->m_aeFillModes[ eMode ];
			m_pRS->m_pDevice->CreateRasterizerState( &RDesc, &pRState );
			m_pRS->m_pDeviceContext->RSSetState( pRState );
			pRState->Release();
		}

		i32 CViewport::_Create(ID3D11Device* pDevice, IDXGISwapChain* pSwapChain, const SViewportOptions& Options)
		{
			if( this->m_bViewportCreated )
			{
				return XST_FAIL;
			}

			m_Options = Options;

			ID3D11Texture2D* pTexBackBuffer = xst_null;
			HRESULT hr = pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&pTexBackBuffer );
			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "[D3D11]: Create back buffer texture failed: " << ::GetLastError() );
				this->m_bViewportCreated = false;
				return RESULT::FAILED;
			}

			hr = pDevice->CreateRenderTargetView( pTexBackBuffer, xst_null, &m_pD3DRTView );
			pTexBackBuffer->Release();
			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "[D3D11]: Create render target view failed: " << m_pRS->_ErrorToString( hr ) );
				this->m_bViewportCreated = false;
				return RESULT::FAILED;
			}

			//Create depth stencil
			ID3D11Texture2D* pTexDepthStencil = xst_null;
			D3D11_TEXTURE2D_DESC descDepth;
			xst_zero( &descDepth, sizeof( descDepth ) );
			descDepth.Width = m_Options.uiResolutionWidth;
			descDepth.Height = m_Options.uiResolutionHeight;
			descDepth.MipLevels = 1;
			descDepth.ArraySize = 1;
			descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			descDepth.SampleDesc.Count = 1;
			descDepth.SampleDesc.Quality = 0;
			descDepth.Usage = D3D11_USAGE_DEFAULT;
			descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			descDepth.CPUAccessFlags = 0;
			descDepth.MiscFlags = 0;
			hr = m_pRS->m_pDevice->CreateTexture2D( &descDepth, NULL, &pTexDepthStencil );
			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "[D3D11]: Create depth stencil texture failed: " << m_pRS->_ErrorToString( hr ) );
				this->m_bViewportCreated = false;
				return RESULT::FAILED;
			}

			//Create depth stencil states
			D3D11_DEPTH_STENCIL_DESC DSDesc;
			xst_zero( &DSDesc, sizeof( DSDesc ) );
			D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
			xst_zero( &DSVDesc, sizeof( DSVDesc ) );

			DSDesc.DepthEnable = true;
			DSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			DSDesc.DepthFunc = D3D11_COMPARISON_LESS;

			DSDesc.StencilEnable = true;
			DSDesc.StencilReadMask = 0xFF;
			DSDesc.StencilWriteMask = 0xFF;

			// Stencil operations if pixel is front-facing.
			DSDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			DSDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
			DSDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			DSDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			// Stencil operations if pixel is back-facing.
			DSDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			DSDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
			DSDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			DSDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			hr = m_pRS->m_pDevice->CreateDepthStencilState( &DSDesc, &m_apDSStates[ 0 ] );
			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "[D3D11]: Create depth stencil state failed: " << m_pRS->_ErrorToString( hr ) );
				return XST_FAIL;
			}

			// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
			// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
			xst_zero( &DSDesc, sizeof( DSDesc ) );
			DSDesc.DepthEnable = false;
			DSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			DSDesc.DepthFunc = D3D11_COMPARISON_LESS;
			DSDesc.StencilEnable = true;
			DSDesc.StencilReadMask = 0xFF;
			DSDesc.StencilWriteMask = 0xFF;
			DSDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			DSDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
			DSDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			DSDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			DSDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			DSDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
			DSDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			DSDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			hr = m_pRS->m_pDevice->CreateDepthStencilState( &DSDesc, &m_apDSStates[ 1 ] );
			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "[D3D11]: Create depth stencil state failed: " << m_pRS->_ErrorToString( hr ) );
				return XST_FAIL;
			}

			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			xst_zero( &descDSV, sizeof( descDSV ) );
			descDSV.Format = descDepth.Format;
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDSV.Texture2D.MipSlice = 0;
			hr = m_pRS->m_pDevice->CreateDepthStencilView( pTexDepthStencil, &descDSV, &m_pD3DDSView );
			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "[D3D11]: Create depth stencil view failed: " << m_pRS->_ErrorToString( hr ) );
				this->m_bViewportCreated = false;
				return RESULT::FAILED;
			}

			//Create rasterizer state
			D3D11_RASTERIZER_DESC RSDesc;
			xst_zero( &RSDesc, sizeof( RSDesc ) );
			// Setup the raster description which will determine how and what polygons will be drawn.
			RSDesc.AntialiasedLineEnable = m_Options.bAntialiasing;
			RSDesc.CullMode = m_pRS->m_aeCullModes[ m_Options.eCullMode ];
			RSDesc.DepthBias = 0;
			RSDesc.DepthBiasClamp = 0.0f;
			RSDesc.DepthClipEnable = true;
			RSDesc.FillMode = m_pRS->m_aeFillModes[ m_Options.eFillMode ];
			RSDesc.FrontCounterClockwise = false;
			RSDesc.MultisampleEnable = m_Options.bMultisampling;
			RSDesc.ScissorEnable = m_Options.bScissor;
			RSDesc.SlopeScaledDepthBias = 0.0f;

			hr = m_pRS->m_pDevice->CreateRasterizerState( &RSDesc, &m_pD3DRState );
			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "[D3D11] Create of rasterizer state failed: " << m_pRS->_ErrorToString( hr ) );
				return XST_FAIL;
			}

			m_pRS->m_pDeviceContext->OMSetDepthStencilState( m_apDSStates[ 0 ], 1 );
			m_pRS->m_pDeviceContext->RSSetState( m_pD3DRState );

			m_D3DViewport.Width = (f32) m_Options.uiResolutionWidth;
			m_D3DViewport.Height = (f32) m_Options.uiResolutionHeight;
			m_D3DViewport.MinDepth = 0;
			m_D3DViewport.MaxDepth = 1;
			m_D3DViewport.TopLeftX = 0;
			m_D3DViewport.TopLeftY = 0;

			//g_mtxOrtho = XMMatrixOrthographicLH( m_D3DViewport.Width, m_D3DViewport.Height, 0.01f, 1000.0f );

			this->m_bViewportCreated = true;
			return RESULT::OK;
		}

		void CViewport::_Destroy()
		{
			this->m_bViewportCreated = false;
			xst_release( this->m_apDSStates[ 0 ] );
			xst_release( this->m_apDSStates[ 1 ] );
			xst_release( this->m_pD3DRState );
			xst_release( this->m_pD3DRTView );
			xst_release( this->m_pD3DDSView );
		}

		void CViewport::EnableZBuffer()
		{
			m_pRS->m_pDeviceContext->OMSetDepthStencilState( m_apDSStates[ 0 ], 1 );
		}

		void CViewport::DisableZBuffer()
		{
			m_pRS->m_pDeviceContext->OMSetDepthStencilState( m_apDSStates[ 1 ], 1 );
		}

		void CViewport::Clear()
		{
			m_pRS->m_pDeviceContext->ClearRenderTargetView( m_pD3DRTView, m_afBGColor );
			m_pRS->m_pDeviceContext->ClearDepthStencilView( m_pD3DDSView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
		}

	}//d3d11
}//xse
#endif //XST_WINDOWS