#include "XSECD3D11RenderSystemPlugin.h"
#if defined (XST_WINDOWS) && defined (XSE_DX11)

//#if defined (XST_DEBUG)
//#	pragma comment( lib, "d3dcompiler.lib" )
//#	pragma comment( lib, "d3dx11d.lib" )
//#	pragma comment( lib, "dxerr.lib" )
//#	pragma comment( lib, "dxguid.lib" )
//#else
//#	pragma comment( lib, "d3dcompiler.lib" )
//#	pragma comment( lib, "d3dx11.lib" )
//#	pragma comment( lib, "dxerr.lib" )
//#	pragma comment( lib, "dxguid.lib" )
//#endif
//#	pragma comment( lib, "d3d11.lib" )
/*
d3dcompiler.lib
d3dx11d.lib
d3dx9d.lib
dxerr.lib
dxguid.lib
winmm.lib
comctl32.lib
*/

namespace XSE
{
	namespace D3D11
	{
		CRenderSystem::CRenderSystem(xst_castring& strName) : XSE::IRenderSystem( strName )
		{
		}

		CRenderSystem::~CRenderSystem()
		{
			_UnloadLibraries();
		}

		void CRenderSystem::_UnloadLibraries()
		{
			for(u32 i = 0; i < LIB_COUNT; ++i)
			{
				XST::Platform::CloseLibrary( m_ahDlls[ i ] );
			}
		}

		i32 CRenderSystem::Init(const SRenderSystemSettings& Options)
		{
			
			HRESULT hr = S_OK;
			RECT rc;
			HWND hWnd = (HWND)Options.uiHWND;
			
			GetClientRect( hWnd, &rc );
			u32 uiWidth = rc.right - rc.left;
			u32 uiHeight = rc.bottom - rc.top;

			u32 uiDeviceFlags = 0;

			#if defined (XST_DEBUG)

				uiDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

			#else

			#endif

			D3D_DRIVER_TYPE eDriverType = D3D_DRIVER_TYPE_HARDWARE;

			D3D_FEATURE_LEVEL aeFeatureLevels[] =
			{
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1,
			};
			u32 uiFeatureLevelCount = ARRAYSIZE( aeFeatureLevels );

			DXGI_SWAP_CHAIN_DESC SwapChainDesc;
			xst_zero( &SwapChainDesc, sizeof( DXGI_SWAP_CHAIN_DESC ) );
			
			SwapChainDesc.BufferCount = 1;
			SwapChainDesc.BufferDesc.Width = uiWidth;
			SwapChainDesc.BufferDesc.Height = uiHeight;
			SwapChainDesc.BufferDesc.RefreshRate.Numerator = Options.uiRefreshRate;
			SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			SwapChainDesc.OutputWindow = hWnd;
			SwapChainDesc.SampleDesc.Count = 1;
			SwapChainDesc.SampleDesc.Quality = 0;
			SwapChainDesc.Windowed = !Options.bFullScreen;

			hr = D3D11CreateDeviceAndSwapChain( 0, eDriverType, 0, 
												uiDeviceFlags, aeFeatureLevels, uiFeatureLevelCount, 
												D3D11_SDK_VERSION, &SwapChainDesc, &m_pSwapChain, 
												&m_pDevice, &m_eFeatureLevel, &m_pDeviceContext );
			u32 uiCodeError = 0;
			if( FAILED( hr ) )
			{
				switch( hr )
				{
				case D3D11_ERROR_FILE_NOT_FOUND: uiCodeError = 1; break;
				case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS: uiCodeError = 2; break;
				case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS: uiCodeError = 3; break;
				case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD: uiCodeError = 4; break;
				case D3DERR_INVALIDCALL: uiCodeError = 5;  break;
				case D3DERR_WASSTILLDRAWING: uiCodeError = 6; break;
				case E_FAIL: uiCodeError = 7; break;
				case E_INVALIDARG: uiCodeError = 8; break;
				case E_OUTOFMEMORY: uiCodeError = 9; break;
				case S_FALSE: uiCodeError = 10; break;
				case S_OK: uiCodeError = 11; break;
				}

				XST_LOG_ERR( "Unable to create D3D11 device: " << GetLastError() );
				return RESULT::FAILED;
			}

			XSE::Vec3 v( XSE::Vec3::ZERO );

			return RESULT::OK;
		}

	}//d3d11
}//xse

#endif //XST_WINDOWS