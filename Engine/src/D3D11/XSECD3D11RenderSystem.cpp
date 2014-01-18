#include "XSECD3D11RenderSystem.h"
#include "XSECD3D11Viewport.h"
#include "XSECD3D11VertexShader.h"
#include "XSECD3D11PixelShader.h"
#include "XSED3D11InputLayouts.h"
#include "XSECD3D11InputLayout.h"
#include "XSECD3D11VertexBuffer.h"
#include "XSECD3D11IndexBuffer.h"
#include "XSECD3D11CGShaderSystem.h"
#include "XSECD3D11HLSLShaderSystem.h"

//#include <DirectXCollision.h>

#if defined (XST_WINDOWS)

//Typedef for function pointer
#define XSE_D3D11_TD(_name, _params) \
	typedef HRESULT (WINAPI * XST_ADD( pfn, _name ) ) _params; \
	XST_ADD( pfn, _name ) _name = xst_null

#define XSE_XMMTX_TO_MTX(_mtx, _mtxXM) \
	_mtx.r[ 0 ] = _mtxXM.r[ 0 ]; \
	_mtx.r[ 1 ] = _mtxXM.r[ 1 ]; \
	_mtx.r[ 2 ] = _mtxXM.r[ 2 ]; \
	_mtx.r[ 3 ] = _mtxXM.r[ 3 ];

#define XSE_XMMTX_TO_MTX_PTR(_mtxPtr, _mtxXM) \
	_mtxPtr->r[ 0 ] = _mtxXM.r[ 0 ]; \
	_mtxPtr->r[ 1 ] = _mtxXM.r[ 1 ]; \
	_mtxPtr->r[ 2 ] = _mtxXM.r[ 2 ]; \
	_mtxPtr->r[ 3 ] = _mtxXM.r[ 3 ];

#define XSE_MTX_TO_XMMTX(_mtxXM, _mtx) \
	_mtxXM.r[ 0 ] = _mtx.r[ 0 ]; \
	_mtxXM.r[ 1 ] = _mtx.r[ 1 ]; \
	_mtxXM.r[ 2 ] = _mtx.r[ 2 ]; \
	_mtxXM.r[ 3 ] = _mtx.r[ 3 ];

#define XSE_MTX_TO_XMMTX_PTR(_mtxXMPtr, _mtx) \
	(_mtxXMPtr)->r[ 0 ] = _mtx.r[ 0 ]; \
	(_mtxXMPtr)->r[ 1 ] = _mtx.r[ 1 ]; \
	(_mtxXMPtr)->r[ 2 ] = _mtx.r[ 2 ]; \
	( _mtxXMPtr )->r[ 3 ] = _mtx.r[ 3 ];

#define XSE_XMVEC4_TO_XST_VEC4(_xstVec, _xmVec) { (_xstVec.x) = (_xmVec.m128_f32[0]); (_xstVec.y) = (_xmVec.m128_f32[1]); (_xstVec.z) = (_xmVec.m128_f32[2]); (_xstVec.w) = (_xmVec.m128_f32[3]); }
#define XSE_XMVEC3_TO_XST_VEC3(_xstVec, _xmVec) { (_xstVec.x) = (_xmVec.x); (_xstVec.y) = (_xmVec.y); (_xstVec.z) = (_xmVec.z);  }
#define XSE_VEC4_TO_XMVEC(_xmVec, _xstVec) xst_memcpy( _xmVec.m_f )

#if defined (XST_VISUAL_STUDIO)
#	pragma warning( push )
#	pragma warning( disable : 4005 ) //macro redefinition
#endif

namespace XSE
{
	namespace D3D11
	{
		using namespace DirectX;

		SRSDiagnostics g_Diagnostics;

		using namespace XSE::Resources;

		typedef HRESULT (WINAPI * pfnD3D11CreateDeviceAndSwapChain)(IDXGIAdapter*,D3D_DRIVER_TYPE,HMODULE,UINT, CONST D3D_FEATURE_LEVEL*,UINT,UINT,CONST DXGI_SWAP_CHAIN_DESC*,IDXGISwapChain**,ID3D11Device**,D3D_FEATURE_LEVEL*,ID3D11DeviceContext** );
		typedef HRESULT (WINAPI * pfnD3DX11CompileFromFileA)(LPCSTR pSrcFile,CONST D3D10_SHADER_MACRO* pDefines, LPD3D10INCLUDE pInclude, LPCSTR pFunctionName, LPCSTR pProfile, UINT Flags1, UINT Flags2, ID3DX11ThreadPump* pPump, ID3D10Blob** ppShader, ID3D10Blob** ppErrorMsgs, HRESULT* pHResult);
		typedef HRESULT (WINAPI * pfnD3DX11CompileFromFileW)(LPCWSTR pSrcFile, CONST D3D10_SHADER_MACRO* pDefines, LPD3D10INCLUDE pInclude, LPCSTR pFunctionName, LPCSTR pProfile, UINT Flags1, UINT Flags2, ID3DX11ThreadPump* pPump, ID3D10Blob** ppShader, ID3D10Blob** ppErrorMsgs, HRESULT* pHResult);
		typedef HRESULT (WINAPI * pfnD3DX11CompileFromMemory)(LPCSTR pSrcData, SIZE_T SrcDataLen, LPCSTR pFileName, CONST D3D10_SHADER_MACRO* pDefines, LPD3D10INCLUDE pInclude, LPCSTR pFunctionName, LPCSTR pProfile, UINT Flags1, UINT Flags2, ID3DX11ThreadPump* pPump, ID3D10Blob** ppShader, ID3D10Blob** ppErrorMsgs, HRESULT* pHResult);
		typedef HRESULT (WINAPI * pfnCreateDXGIFactory)(REFIID riid, void **ppFactory);
		typedef HRESULT (WINAPI * pfnCreateDXGIFactory1)(REFIID riid, void **ppFactory);
		typedef HRESULT (WINAPI * pfnD3D11CreateDevice)(__in_opt IDXGIAdapter* pAdapter,D3D_DRIVER_TYPE DriverType,HMODULE Software,UINT Flags,__in_ecount_opt( FeatureLevels ) CONST D3D_FEATURE_LEVEL* pFeatureLevels,UINT FeatureLevels,UINT SDKVersion,__out_opt ID3D11Device** ppDevice,__out_opt D3D_FEATURE_LEVEL* pFeatureLevel,__out_opt ID3D11DeviceContext** ppImmediateContext );

		pfnD3D11CreateDeviceAndSwapChain	D3D11CreateDeviceAndSwapChain = xst_null;
		pfnD3DX11CompileFromFileA			D3DX11CompileFromFileA = xst_null;
		pfnD3DX11CompileFromFileW			D3DX11CompileFromFileW = xst_null;
		pfnD3DX11CompileFromMemory			D3DX11CompileFromMemory = xst_null;
		pfnCreateDXGIFactory				CreateDXGIFactory = xst_null;
		pfnD3D11CreateDevice				D3D11CreateDevice = xst_null;
		pfnCreateDXGIFactory1				CreateDXGIFactory1 = xst_null;

		#define XSE_D3D11_DEFAULT_FORMAT	DXGI_FORMAT_R8G8B8A8_UNORM


		XMMATRIX g_aMatrices[ MatrixTypes::_ENUM_COUNT ];
		XMMATRIX g_aTmpMatrices[ MatrixTypes::_ENUM_COUNT ];
		XMVECTOR g_vecDeterminant;

		D3D_FEATURE_LEVEL	g_aFeatureLevels[ ShaderModels::_ENUM_COUNT ] = 
		{
			(D3D_FEATURE_LEVEL)0, //unknown
			(D3D_FEATURE_LEVEL)0, //SM 1.0 is not supported
			D3D_FEATURE_LEVEL_9_3, //2.0
			D3D_FEATURE_LEVEL_9_3, //3.0
			D3D_FEATURE_LEVEL_10_0, //4.0
			D3D_FEATURE_LEVEL_11_0 //5.0
		};

		CRenderSystem::CRenderSystem(xst_castring& strName) : XSE::IRenderSystem( strName )
		{
			m_pViewport = xst_null;
			m_pDevice = xst_null;
			m_pDeviceContext = xst_null;
			m_pSwapChain = xst_null;
			m_pCurrentShaderSystem = xst_null;
			m_pD3DAdapter = xst_null;
			m_pD3DFactory = xst_null;

			m_pVSMemMgr = xst_null;
			m_pPSMemMgr = xst_null;

			//Set matrices to default
			g_aMatrices[ MatrixTypes::WORLD ] = XMMatrixIdentity();

			m_aFormats[ RSFormats::R16F ]						= DXGI_FORMAT_R16_FLOAT;
			m_aFormats[ RSFormats::R16G16B16A16U ]				= DXGI_FORMAT_R16G16B16A16_UNORM;
			m_aFormats[ RSFormats::R16G16B16A16F ]				= DXGI_FORMAT_R16G16B16A16_FLOAT;
			m_aFormats[ RSFormats::R16G16F ]					= DXGI_FORMAT_R16G16_FLOAT;
			m_aFormats[ RSFormats::R16G16U ]					= DXGI_FORMAT_R16G16_UNORM;
			m_aFormats[ RSFormats::R16U ]						= DXGI_FORMAT_R16_UNORM;
			m_aFormats[ RSFormats::R32F ]						= DXGI_FORMAT_R16_FLOAT;
			m_aFormats[ RSFormats::R32G32B32A32F ]				= DXGI_FORMAT_R32G32B32A32_FLOAT;
			m_aFormats[ RSFormats::R32G32B32F ]					= DXGI_FORMAT_R32G32B32_FLOAT;
			m_aFormats[ RSFormats::R32G32F ]					= DXGI_FORMAT_R32G32_FLOAT;
			m_aFormats[ RSFormats::R8G8B8A8U ]					= DXGI_FORMAT_R8G8B8A8_UNORM;
			m_aFormats[ RSFormats::R8U ]						= DXGI_FORMAT_R8_UNORM;
			m_aFormats[ RSFormats::UNKNOWN ]					= DXGI_FORMAT_UNKNOWN;

			m_aeTopologies[ TopologyTypes::LINE_LIST ]			= D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
			m_aeTopologies[ TopologyTypes::LINE_STRIP ]			= D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
			m_aeTopologies[ TopologyTypes::POINT_LIST ]			= D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
			m_aeTopologies[ TopologyTypes::TRIANGLE_LIST ]		= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			m_aeTopologies[ TopologyTypes::TRIANGLE_STRIP ]		= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			m_aeTopologies[ TopologyTypes::UNDEFINED ]			= D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;

			m_aeFillModes[ FillModes::SOLID ]					= D3D11_FILL_SOLID;
			m_aeFillModes[ FillModes::WIREFRAME ]				= D3D11_FILL_WIREFRAME;

			m_aeCullModes[ CullModes::BACK ]					= D3D11_CULL_BACK;
			m_aeCullModes[ CullModes::FRONT ]					= D3D11_CULL_FRONT;
			m_aeCullModes[ CullModes::NONE ]					= D3D11_CULL_NONE;

			this->SetFeatureLevel( ShaderModels::SM_2_0, ShaderModels::SM_5_0 );



			xst_zero( &m_Current, sizeof( SCurrent ) );
			xst_zero( &g_Diagnostics, sizeof( SRSDiagnostics ) );
		}

		CRenderSystem::~CRenderSystem()
		{
			//Destroy all input layouts
			for(ILMap::iterator Itr = m_mInputLayouts.begin(); Itr != m_mInputLayouts.end(); ++Itr)
			{
				xst_delete( Itr->second );
			}

			for(IRenderSystem::_ShaderSystemIterator Itr = this->m_mpShaderSystems.begin(); Itr != this->m_mpShaderSystems.end(); ++Itr)
			{
				Itr->second->Destroy();
				xst_delete( Itr->second );
			}

			if( m_pDeviceContext )
				m_pDeviceContext->ClearState();
		
			xst_delete( m_pVSMemMgr );
			xst_delete( m_pPSMemMgr );
			DestroyViewport( m_pViewport );
			xst_release( m_pDeviceContext );
			xst_release( m_pSwapChain );
			xst_release( m_pD3DAdapter );
			xst_release( m_pDevice );
			xst_release( m_pD3DFactory );

			_UnloadLibraries();
		}

		void CRenderSystem::_UnloadLibraries()
		{
			for(u32 i = 0; i < LIB_COUNT; ++i)
			{
				XST::Platform::CloseLibrary( m_ahDlls[ i ] );
			}
		}

		i32 CRenderSystem::_LoadLibraries()
		{
			#if defined (XST_ENGINE_DEBUG)
				m_ahDlls[ D3DX11 ] = XST::Platform::LoadLibrary( "D3DX11d_43.dll" );
			#else
				m_ahDlls[ D3DX11 ] = XST::Platform::LoadLibrary( "D3DX11d_43.dll" );
			#endif

			m_ahDlls[ D3D11 ] = XST::Platform::LoadLibrary( "d3d11.dll" );
			m_ahDlls[ DXGI ] = XST::Platform::LoadLibrary( "dxgi.dll" );

			for(u32 i = 0; i < LIB_COUNT; ++i)
			{
				if( !m_ahDlls[ i ] )
				{
					XST_LOG_ERR( "Unable to load one or more Direct3D11 dlls" );
					return RESULT::FAILED;
				}
			}

			XSE_LOAD_FUNC( D3D11CreateDeviceAndSwapChain, pfnD3D11CreateDeviceAndSwapChain, m_ahDlls[ D3D11 ], XST_TOSTRING( D3D11CreateDeviceAndSwapChain ) );
			XSE_LOAD_FUNC( D3DX11CompileFromFileA, pfnD3DX11CompileFromFileA, m_ahDlls[ D3DX11 ], XST_TOSTRING( D3DX11CompileFromFileA ) );
			XSE_LOAD_FUNC( D3DX11CompileFromFileW, pfnD3DX11CompileFromFileW, m_ahDlls[ D3DX11 ], XST_TOSTRING( D3DX11CompileFromFileW ) );
			XSE_LOAD_FUNC( D3DX11CompileFromMemory, pfnD3DX11CompileFromMemory, m_ahDlls[ D3DX11 ], XST_TOSTRING( D3DX11CompileFromMemory ) );
			XSE_LOAD_FUNC3( CreateDXGIFactory,	m_ahDlls[ DXGI ] );
			XSE_LOAD_FUNC3( CreateDXGIFactory1, m_ahDlls[ DXGI ] );
			XSE_LOAD_FUNC3( D3D11CreateDevice,	m_ahDlls[ D3D11 ] );

			return RESULT::OK;
		}

		i32 CRenderSystem::Init(const SRenderSystemOptions& Options)
		{
			//Init base class
			IRenderSystem::_Init( Options );

			_UnloadLibraries();
			if( XST_FAILED( _LoadLibraries() ) )
			{
				return RESULT::FAILED;
			}

			HRESULT hr = CreateDXGIFactory( __uuidof( IDXGIFactory ), (void**)&m_pD3DFactory );
			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "[D3D11]: Failed to create dxgi factory: " << _ErrorToString( hr ) );
				return XST_FAIL;
			}

			//Enumerate primary video card
			hr = m_pD3DFactory->EnumAdapters( 0, &m_pD3DAdapter );
			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "[D3D11]: Failed to enumerate dxgi factory adapters: " << _ErrorToString( hr ) );
				return XST_FAIL;
			}

			if( XST_FAILED( SetFeatureLevel( Options.eMinFeatureLevel, Options.eMaxFeatureLevel ) ) )
			{
				return XST_FAIL;
			}

			RECT rc;
			HWND hWnd = (HWND)Options.uiHWND;
			
			GetClientRect( hWnd, &rc );
			u32 uiWidth = rc.right - rc.left;
			u32 uiHeight = rc.bottom - rc.top;

			if( m_Options.uiResolutionWidth != uiWidth )
			{
				m_Options.uiResolutionWidth = uiWidth;
			}

			if( m_Options.uiResolutionHeight != uiHeight )
			{
				m_Options.uiResolutionHeight = uiHeight;
			}

			u32 uiDeviceFlags = 0;

			if( m_Options.bDebugMode )
			{
				uiDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
			}

			D3D_DRIVER_TYPE eDriverType = D3D_DRIVER_TYPE_HARDWARE;
			if( m_pD3DAdapter )
			{
				eDriverType = D3D_DRIVER_TYPE_UNKNOWN;
			}

			xst_vector< D3D_FEATURE_LEVEL > vFeatureLevels;
			cu32 uiLevelCount = (u32)m_eMaxFeatureLevel - (u32)m_eMinFeatureLevel;
			for(u32 i = (u32)m_eMaxFeatureLevel; i >= (u32)m_eMinFeatureLevel; --i)
			{
				vFeatureLevels.push_back( g_aFeatureLevels[ (SHADER_MODEL)i ] );
			}

			//Create the device
			hr = D3D11CreateDevice(	m_pD3DAdapter, eDriverType, 0, uiDeviceFlags, &vFeatureLevels[ 0 ], vFeatureLevels.size(), 
									D3D11_SDK_VERSION, &m_pDevice, &m_eFeatureLevel, &m_pDeviceContext );
			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "[D3D11] Unable to create Direct3D11 device: " << _ErrorToString( hr ) );
				return XST_FAIL;
			}
	
			//Get caps
			if( XST_FAILED( _GetCaps( &this->m_Caps ) ) )
			{
				return XST_FAIL;
			}

			//Create swap chain

			DXGI_SWAP_CHAIN_DESC SwapChainDesc;
			xst_zero( &SwapChainDesc, sizeof( DXGI_SWAP_CHAIN_DESC ) );
			
			u32 uiRefRate = Options.uiRefreshRate;
			if( !m_Caps.IsResolutionAvailable( uiWidth, uiHeight, Options.uiRefreshRate ) && m_Options.bFullScreen )
			{
				XST_LOG_WRN( "Resolution: " << uiWidth << "x" << uiHeight << "@" << Options.uiRefreshRate << " is not available on this machine. Set to minimal." );
				m_Options.uiResolutionWidth = this->m_Caps.vResolutions[ 0 ].uiWidth;
				m_Options.uiResolutionHeight = this->m_Caps.vResolutions[ 0 ].uiHeight;
				m_Options.uiRefreshRate = this->m_Caps.vResolutions[ 0 ].vRefreshRates[ 0 ];
			}

			SwapChainDesc.BufferCount = 1;
			SwapChainDesc.BufferDesc.Width = m_Options.uiResolutionWidth;
			SwapChainDesc.BufferDesc.Height = m_Options.uiResolutionHeight;
			SwapChainDesc.BufferDesc.RefreshRate.Numerator = m_Options.uiRefreshRate;
			SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			SwapChainDesc.OutputWindow = hWnd;
			SwapChainDesc.SampleDesc.Count = 1;
			SwapChainDesc.SampleDesc.Quality = 0;
			SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			SwapChainDesc.Windowed = !Options.bFullScreen;

			IDXGIDevice* pDevice;
			IDXGIAdapter* pAdapter;
			IDXGIFactory* pFactory = m_pD3DFactory;
			if( eDriverType == D3D_DRIVER_TYPE_UNKNOWN )
			{
				hr = m_pDevice->QueryInterface( __uuidof( IDXGIDevice ), (void**)&pDevice );
				hr = pDevice->GetParent( __uuidof( IDXGIAdapter ), (void**)&pAdapter );
				hr = pAdapter->GetParent( __uuidof( IDXGIFactory ), (void**)&pFactory );
			}
			hr = pFactory->CreateSwapChain( m_pDevice, &SwapChainDesc, &m_pSwapChain );
			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "[D3D11]: Failed to create swap chain: " << _ErrorToString( hr ) );
				return XST_FAIL;
			}
			/*hr = D3D11CreateDeviceAndSwapChain( 0, eDriverType, 0, 
												uiDeviceFlags, &vFeatureLevels[ 0 ], vFeatureLevels.size(), 
												D3D11_SDK_VERSION, &SwapChainDesc, &m_pSwapChain, 
												&m_pDevice, &m_eFeatureLevel, &m_pDeviceContext );*/

			//Create default viewport
			CViewport* pVP = (CViewport*)CreateViewport( uiWidth, uiHeight );
			if( pVP == xst_null )
			{
				return RESULT::FAILED;
			}

			SetViewport( pVP, true );	

			//Create shader systems
			if( XST_FAILED( this->CreateShaderSystem( ShaderLanguages::HLSL ) ) )
			{
				return XST_FAIL;
			}

			m_pCurrentShaderSystem = GetShaderSystem( );
			return RESULT::OK;
		}

		i32 CRenderSystem::ResizeBuffers( cu32 &uiWidth, cu32 &uiHeight )
		{
			//Do not resize to the same values
			if( ( m_Options.uiResolutionWidth == uiWidth && m_Options.uiResolutionHeight == uiHeight ) )
				return RESULT::OK;
			//Relase current render target view
			m_pViewport->_Destroy();
			//Remember old values
			u32 uiOldWidth = m_Options.uiResolutionWidth;
			u32 uiOldHeight = m_Options.uiResolutionHeight;
			m_Options.uiResolutionWidth = uiWidth;
			m_Options.uiResolutionHeight = uiHeight;

			ul32 ulFlags = 0;
			if( m_Options.bFullScreen )
			{
				ulFlags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			}

			m_pSwapChain->ResizeBuffers( 1, uiWidth, uiHeight, DXGI_FORMAT_R8G8B8A8_UNORM, ulFlags );

			//Create new rendertarget view
			if( XST_FAILED( m_pViewport->_Create( m_pDevice, m_pSwapChain, m_Options ) ) )
			{
				m_Options.uiResolutionWidth = uiOldWidth;
				m_Options.uiResolutionHeight = uiOldHeight;
				return RESULT::FAILED;
			}

			SetViewport( m_pViewport, false );
			return RESULT::OK;
		}

		i32 CRenderSystem::SetFeatureLevel(SHADER_MODEL eMinLevel, SHADER_MODEL eMaxLevel)
		{
			m_eMinFeatureLevel = eMinLevel;
			m_eMaxFeatureLevel = eMaxLevel;

			if( eMaxLevel == ShaderModels::UNKNOWN )
			{
				m_eD3DMaxFeatureLevel = D3D_FEATURE_LEVEL_11_0;
				m_eMinFeatureLevel = ShaderModels::SM_5_0;
			}

			if( eMinLevel == ShaderModels::UNKNOWN )
			{
				m_eD3DMinFeatureLevel = D3D_FEATURE_LEVEL_9_3;
				m_eMaxFeatureLevel = ShaderModels::SM_2_0;
			}

			if( ( m_eD3DMinFeatureLevel = g_aFeatureLevels[ eMinLevel ] ) == 0 )
			{
				m_eD3DMinFeatureLevel = D3D_FEATURE_LEVEL_9_3;
				m_eMinFeatureLevel = ShaderModels::SM_2_0;
				XST_LOG_ERR( "Feature level: " << eMinLevel << " is not supported" );
				return XST_FAIL;
			}

			u32 u = (u32)eMaxLevel;
			m_eD3DMaxFeatureLevel = g_aFeatureLevels[ u ];
			if( ( m_eD3DMaxFeatureLevel ) == (D3D_FEATURE_LEVEL)0 )
			{
				m_eD3DMaxFeatureLevel = D3D_FEATURE_LEVEL_9_3;
				m_eMaxFeatureLevel = ShaderModels::SM_2_0;
				XST_LOG_ERR( "Feature level: " << eMaxLevel << " is not supported" );
				return XST_FAIL;
			}	

			return XST_OK;
		}

		i32 CRenderSystem::ApplyShaderConstantNames()
		{
			i32 iResult = 0;
			for(ShaderSystemMap::iterator Itr = m_mpShaderSystems.begin(); Itr != m_mpShaderSystems.end(); ++Itr)
			{
				iResult += Itr->second->ApplyShaderConstantNames();
			}

			return iResult;
		}

		//EACH FRAME
		void CRenderSystem::Enable2DRendering()
		{
			//Store projection matrix
			g_aTmpMatrices[ MatrixTypes::PROJECTION ] = g_aMatrices[ MatrixTypes::PROJECTION ];
			g_aMatrices[ MatrixTypes::PROJECTION ] = XMMatrixOrthographicOffCenterRH( 0, (f32)m_Options.uiResolutionWidth, (f32)m_Options.uiResolutionHeight, 0, 0.0f, 1.0f );
			
			g_aMatrices[ MatrixTypes::TRANSPOSED_PROJ ]			= XMMatrixTranspose( g_aMatrices[ MatrixTypes::PROJECTION ] );
			g_aMatrices[ MatrixTypes::TRANSPOSED_VIEW ]			= XMMatrixTranspose( g_aMatrices[ MatrixTypes::VIEW ] );
			g_aMatrices[ MatrixTypes::TRANSPOSED_WORLD ]		= XMMatrixTranspose( g_aMatrices[ MatrixTypes::WORLD ] );
			g_aMatrices[ MatrixTypes::TRANSPOSED_VIEW_PROJ ]	= XMMatrixTranspose( XMMatrixMultiply( g_aMatrices[ MatrixTypes::VIEW ], g_aMatrices[ MatrixTypes::PROJECTION ] ) );
			//g_aMatrices[ MTX_PROJ ] = g_aMatrices[ MTX_T_PROJ ];
			m_pCurrentShaderSystem->UpdateFrameInputs();
		}

		//EACH FRAME
		void CRenderSystem::Enable3DRendering()
		{
			//Retreive projection matrix
			g_aMatrices[ MatrixTypes::PROJECTION ] = g_aTmpMatrices[ MatrixTypes::PROJECTION ];

			g_aMatrices[ MatrixTypes::TRANSPOSED_PROJ ]			= XMMatrixTranspose( g_aMatrices[ MatrixTypes::PROJECTION ] );
			g_aMatrices[ MatrixTypes::TRANSPOSED_VIEW ]			= XMMatrixTranspose( g_aMatrices[ MatrixTypes::VIEW ] );
			g_aMatrices[ MatrixTypes::TRANSPOSED_WORLD ]		= XMMatrixTranspose( g_aMatrices[ MatrixTypes::WORLD ] );
			g_aMatrices[ MatrixTypes::TRANSPOSED_VIEW_PROJ ]	= XMMatrixTranspose( XMMatrixMultiply( g_aMatrices[ MatrixTypes::VIEW ], g_aMatrices[ MatrixTypes::PROJECTION ] ) );

			m_pCurrentShaderSystem->UpdateFrameInputs();
		}

		//EACH FRAME
		i32 CRenderSystem::BeginRender()
		{
			xst_assert( m_pViewport, "(CD3D11RenderSystem::BeginRender) No viewport set" );
			m_pViewport->Clear();

			//Update constant buffers

			g_aMatrices[ MatrixTypes::TRANSPOSED_PROJ ]			= XMMatrixTranspose( g_aMatrices[ MatrixTypes::PROJECTION ] );
			g_aMatrices[ MatrixTypes::TRANSPOSED_VIEW ]			= XMMatrixTranspose( g_aMatrices[ MatrixTypes::VIEW ] );
			g_aMatrices[ MatrixTypes::TRANSPOSED_WORLD ]		= XMMatrixTranspose( g_aMatrices[ MatrixTypes::WORLD ] );
			g_aMatrices[ MatrixTypes::VIEW_PROJ ]				= XMMatrixMultiply( g_aMatrices[ MatrixTypes::VIEW ], g_aMatrices[ MatrixTypes::PROJECTION ] );
			g_aMatrices[ MatrixTypes::TRANSPOSED_VIEW_PROJ ]	= XMMatrixTranspose( g_aMatrices[ MatrixTypes::VIEW_PROJ ] );
			g_aMatrices[ MatrixTypes::INVERTED_VIEW_PROJ ]		= XMMatrixInverse( &g_vecDeterminant, g_aMatrices[ MatrixTypes::VIEW_PROJ ] );

			m_pCurrentShaderSystem->UpdateFrameInputs();

			///////////////////////////////////////////////////////////////////////////

			///////////////////////////////////////////////////////////////////////////

			return XST_OK;
		}

		//EACH FRAME
		i32 CRenderSystem::EndRender()
		{
#if defined (XST_ENGINE_DEBUG)
			xst_memcpy( &m_Diagnostics, sizeof( SRSDiagnostics ), &g_Diagnostics, sizeof( SRSDiagnostics ) );
			xst_zero( &g_Diagnostics, sizeof( SRSDiagnostics ) );
#endif
			return m_pSwapChain->Present( m_Options.bVSync, 0 );
		}

		//EACH FRAME
		i32 CRenderSystem::ClearScene(const XST::CColor& Color)
		{
			m_pViewport->SetBackgroundColor( Color );
			m_pViewport->Clear();
			return RESULT::OK;
		}

		CFrustum CRenderSystem::CreateViewFrustum() const
		{
			CFrustum Frustum;
			/*DirectX::BoundingFrustum XMFrustum;
			Frustum.m_fBottomSlope = XMFrustum.BottomSlope;
			Frustum.m_fFar = XMFrustum.Far;
			Frustum.m_fLeftSlope = XMFrustum.LeftSlope;
			Frustum.m_fNear = XMFrustum.Near;
			Frustum.m_fRightSlope = XMFrustum.RightSlope;
			Frustum.m_fTopSlope = XMFrustum.TopSlope;
			Frustum.m_quatOrientation*/
			UpdateViewFrustum( &Frustum );
			return Frustum;
		}

		void CRenderSystem::UpdateViewFrustum(CFrustum* pFrustum) const
		{
			static const Vec4 HomogenousPoints[ 6 ] =
			{
				{ 1.0f, 0.0f, 1.0f, 1.0f }, // right
				{ -1.0f, 0.0f, 1.0f, 1.0f }, // left
				{ 0.0f, 1.0f, 1.0f, 1.0f }, // top
				{ 0.0f, -1.0f, 1.0f, 1.0f }, // bottom

				{ 0.0f, 0.0f, 0.0f, 1.0f }, // near
				{ 0.0f, 0.0f, 1.0f, 1.0f }, // far
			};

			DirectX::XMVECTOR vecDet; // determinant
			DirectX::XMMATRIX mtxInverse = DirectX::XMMatrixInverse( &vecDet, g_aMatrices[ MatrixTypes::PROJECTION ] );
			DirectX::XMVECTOR avecPoints[ 6 ]; // frustum corners in world space

			for( u32 i = 0; i < 6; ++i )
			{
				avecPoints[ i ] = DirectX::XMVector4Transform( HomogenousPoints[ i ], mtxInverse );
			}

			for( u32 i = 0; i < 4; ++i )
			{
				avecPoints[ i ] = avecPoints[ i ] * DirectX::XMVectorReciprocal( DirectX::XMVectorSplatZ( avecPoints[ i ] ) );
			}

			avecPoints[ 4 ] = avecPoints[ 4 ] * DirectX::XMVectorReciprocal( DirectX::XMVectorSplatW( avecPoints[ 4 ] ) );
			avecPoints[ 5 ] = avecPoints[ 5 ] * DirectX::XMVectorReciprocal( DirectX::XMVectorSplatW( avecPoints[ 5 ] ) );

			pFrustum->m_fRightSlope = DirectX::XMVectorGetX( avecPoints[ 0 ] );
			pFrustum->m_fLeftSlope = DirectX::XMVectorGetX( avecPoints[ 1 ] );
			pFrustum->m_fTopSlope = DirectX::XMVectorGetX( avecPoints[ 2 ] );
			pFrustum->m_fBottomSlope = DirectX::XMVectorGetX( avecPoints[ 3 ] );
			pFrustum->m_fNear = DirectX::XMVectorGetZ( avecPoints[ 4 ] );
			pFrustum->m_fFar = DirectX::XMVectorGetZ( avecPoints[ 5 ] );


			//Build the frustum planes
			XMVECTOR Zero = XMVectorZero( );

			// Build the frustum planes.
			XMVECTOR Planes[ 6 ];
			Planes[ 0 ] = XMVectorSet( 0.0f, 0.0f, -1.0f, pFrustum->m_fNear );
			Planes[ 1 ] = XMVectorSet( 0.0f, 0.0f, 1.0f, -pFrustum->m_fFar );
			Planes[ 2 ] = XMVectorSet( 1.0f, 0.0f, -pFrustum->m_fRightSlope, 0.0f );
			Planes[ 3 ] = XMVectorSet( -1.0f, 0.0f, pFrustum->m_fLeftSlope, 0.0f );
			Planes[ 4 ] = XMVectorSet( 0.0f, 1.0f, -pFrustum->m_fTopSlope, 0.0f );
			Planes[ 5 ] = XMVectorSet( 0.0f, -1.0f, pFrustum->m_fBottomSlope, 0.0f );

			// Normalize the planes so we can compare to the sphere radius.
			Planes[ 2 ] = XMVector3Normalize( Planes[ 2 ] );
			Planes[ 3 ] = XMVector3Normalize( Planes[ 3 ] );
			Planes[ 4 ] = XMVector3Normalize( Planes[ 4 ] );
			Planes[ 5 ] = XMVector3Normalize( Planes[ 5 ] );

			XSE_XMVEC4_TO_XST_VEC4( pFrustum->RightPlane.vecPlane, Planes[ 2 ] );
			XSE_XMVEC4_TO_XST_VEC4( pFrustum->LeftPlane.vecPlane, Planes[ 3 ] );
			XSE_XMVEC4_TO_XST_VEC4( pFrustum->TopPlane.vecPlane, Planes[ 4 ] );
			XSE_XMVEC4_TO_XST_VEC4( pFrustum->BottomPlane.vecPlane, Planes[ 5 ] );
			XSE_XMVEC4_TO_XST_VEC4( pFrustum->FarPlane.vecPlane, Planes[ 1 ] );
			XSE_XMVEC4_TO_XST_VEC4( pFrustum->NearPlane.vecPlane, Planes[ 0 ] );
		}

		IViewport* CRenderSystem::CreateViewport(cu32& uiWidth, cu32& uiHeight)
		{
			CViewport* pVP = xst_new CViewport( this );
			if( XST_FAILED( pVP->_Create( m_pDevice, m_pSwapChain, m_Options ) ) )
			{
				xst_delete( pVP );
				return xst_null;
			}

			pVP->m_D3DViewport.Width = (f32)uiWidth;
			pVP->m_D3DViewport.Height = (f32)uiHeight;

			return pVP;
		}

		void CRenderSystem::DestroyViewport(XSE::IViewport *pViewport)
		{
			xst_delete( pViewport );
		}

		i32 CRenderSystem::SetViewport(IViewport* pViewport, bool bDeleteOld)
		{
			if( bDeleteOld )
			{
				xst_delete( m_pViewport );
			}

			CViewport* pVP = (CViewport*)pViewport;
			xst_assert( pVP, "(CRenderSystem::SetViewport) Viewport is null" );

			m_pDeviceContext->OMSetRenderTargets( 1, &pVP->m_pD3DRTView, pVP->m_pD3DDSView );
			m_pDeviceContext->RSSetViewports( 1, &pVP->m_D3DViewport );

			m_pViewport = pVP;

			return RESULT::OK;
		}

		IInputLayout*	CRenderSystem::GetInputLayout(ul32 ulElements)
		{
			ILMap::iterator Itr = m_mInputLayouts.find( ulElements );
			if( Itr == m_mInputLayouts.end() )
			{
				return BuildInputLayout( ulElements );
			}

			return Itr->second;
		}

		IInputLayout*	CRenderSystem::BuildInputLayout(ul32 ulElements)
		{
			CInputLayout* pIL = xst_null;
			ILMap::iterator Itr = m_mInputLayouts.find( ulElements );
			if( Itr == m_mInputLayouts.end() )
			{
				pIL = xst_new CInputLayout( this );

				if( XST_FAILED( pIL->Create( ulElements ) ) )
				{
					xst_delete( pIL );
					return xst_null;
				}
				m_mInputLayouts.insert( ILMap::value_type( ulElements, pIL ) );
			}
			else
			{
				pIL = Itr->second;
			}

			return pIL;
		}

		XSE::IVertexBuffer* CRenderSystem::CreateVertexBuffer()
		{
			CVertexBuffer* pVB = xst_new CVertexBuffer( this );
			return pVB;
		}

		void CRenderSystem::UpdateVertexBuffer(IVertexBuffer* pVB, cul32& ulStartVertex, cul32& ulVertexCount, const CVertexData& DstData)
		{
			xst_assert2( pVB != xst_null );
			const CVertexBuffer* pBuff = (CVertexBuffer*)pVB;

			m_pDeviceContext->UpdateSubresource( pBuff->m_pD3DBuffer, 0, xst_null, DstData.GetData(), DstData.GetSize(), 0 );
		}

		i32	CRenderSystem::SetVertexBuffer(const IVertexBuffer* pVB, const ul32& ulVertexSize)
		{
			xst_assert2( ulVertexSize > 0 );
			const CVertexBuffer* pBuff = (CVertexBuffer*)pVB;
			
			/*if( m_Current.pVertexBuffer == pBuff )
			{
				return XST_OK;
			}*/
			m_Current.pVertexBuffer = pBuff;

			m_pDeviceContext->IASetPrimitiveTopology( pBuff->m_eD3DTopologyType );
			u32 uiStrides = ulVertexSize;
			u32 uiOffsets = 0;
			m_pDeviceContext->IASetVertexBuffers( 0, 1, &pBuff->m_pD3DBuffer, &uiStrides, &uiOffsets );
#if defined (XST_ENGINE_DEBUG)
			++g_Diagnostics.ulSetVertexBufferCount;
#endif
			return XST_OK;
		}

		i32 CRenderSystem::SetVertexBuffer(const IVertexBuffer* pVB)
		{
			xst_assert( pVB, "(CRenderSystem::SetVertexBuffer) VertexBuffer not created" );
			const CVertexBuffer* pBuff = (CVertexBuffer*)pVB;
			
			/*if( m_Current.pVertexBuffer == pBuff )
			{
				return XST_OK;
			}*/
			m_Current.pVertexBuffer = pBuff;

			const CInputLayout* pIL = (CInputLayout*)pBuff->m_pInputLayout;
			xst_assert( pIL, "(CRenderSystem::SetVertexBuffer) Input layout not created" );
			//m_pDeviceContext->IASetInputLayout( pIL->m_pD3DInputLayout );
			//m_pDeviceContext->IASetPrimitiveTopology( pBuff->m_eD3DTopologyType );
			u32 uiStrides = pIL->GetVertexSize();
			u32 uiOffsets = 0;
			m_pDeviceContext->IASetVertexBuffers( 0, 1, &pBuff->m_pD3DBuffer, &uiStrides, &uiOffsets );
#if defined (XST_ENGINE_DEBUG)
			++g_Diagnostics.ulSetVertexBufferCount;
#endif
			return XST_OK;
		}

		i32 CRenderSystem::SetInputLayout(const IInputLayout* pIL)
		{
			xst_assert( pIL, "" );
			const CInputLayout* pILayout = (CInputLayout*)pIL;
			if( m_Current.pInputLayout == pILayout )
			{
				return XST_OK;
			}
			m_Current.pInputLayout = pILayout;
			m_pDeviceContext->IASetInputLayout( pILayout->m_pD3DInputLayout );
#if defined (XST_ENGINE_DEBUG)
			++g_Diagnostics.ulSetInputLayoutCount;
#endif
			return XST_OK;
		}

		i32 CRenderSystem::SetVertexShader(const Resources::IVertexShader* pVS)
		{
			xst_assert( pVS, "" );
			const CVertexShader* pShader = (CVertexShader*)pVS;
			if( m_Current.pVertexShader == pShader )
			{
				return XST_OK;
			}
			m_Current.pVertexShader = pShader;
#if defined (XST_ENGINE_DEBUG)
			++g_Diagnostics.ulSetVertexShaderCount;
#endif
			return pShader->m_pShaderSystem->SetVertexShader( pVS );
		}

		i32 CRenderSystem::SetPixelShader(const IPixelShader* pPS)
		{
			xst_assert( pPS, "" );
			const CPixelShader* pShader = (CPixelShader*)pPS;
			if( m_Current.pPixelShader == pShader )
			{
				return XST_OK;
			}
			m_Current.pPixelShader = pShader;
#if defined (XST_ENGINE_DEBUG)
			++g_Diagnostics.ulSetPixelShaderCount;
#endif
			return pShader->m_pShaderSystem->SetPixelShader( pPS );
		}

		i32 CRenderSystem::SetVertexBufferWithCheck(const IVertexBuffer* pVB)
		{
			xst_assert( pVB, "(CRenderSystem::SetVertexBufferWithCheck) VertexBuffer not created" );
			const CVertexBuffer* pBuff = (CVertexBuffer*)pVB;
			
			if( m_Current.pVertexBuffer == pBuff )
			{
				return XST_OK;
			}
			m_Current.pVertexBuffer = pBuff;

			CInputLayout* pIL = (CInputLayout*)pBuff->m_pInputLayout;
			xst_assert( pIL, "(CRenderSystem::SetVertexBufferWithCheck) Input layout not created" );
			//m_pDeviceContext->IASetInputLayout( pIL->m_pD3DInputLayout );
			m_pDeviceContext->IASetPrimitiveTopology( pBuff->m_eD3DTopologyType );
			u32 uiStrides = pIL->GetVertexSize();
			u32 uiOffsets = 0;
			m_pDeviceContext->IASetVertexBuffers( 0, 1, &pBuff->m_pD3DBuffer, &uiStrides, &uiOffsets );
#if defined (XST_ENGINE_DEBUG)
			++g_Diagnostics.ulSetVertexBufferCount;
#endif
			return XST_OK;
		}

		i32 CRenderSystem::SetInputLayoutWithCheck(const IInputLayout* pIL)
		{
			xst_assert( pIL, "" );
			const CInputLayout* pILayout = (CInputLayout*)pIL;
			if( m_Current.pInputLayout == pILayout )
			{
				return XST_OK;
			}
			m_Current.pInputLayout = pILayout;
			m_pDeviceContext->IASetInputLayout( pILayout->m_pD3DInputLayout );
#if defined (XST_ENGINE_DEBUG)
			++g_Diagnostics.ulSetInputLayoutCount;
#endif
			return XST_OK;
		}

		i32 CRenderSystem::SetVertexShaderWithCheck(const Resources::IVertexShader* pVS)
		{
			xst_assert( pVS, "" );
			const CVertexShader* pShader = (CVertexShader*)pVS;
			if( m_Current.pVertexShader == pShader )
			{
				return XST_OK;
			}
			m_Current.pVertexShader = pShader;
#if defined (XST_ENGINE_DEBUG)
			++g_Diagnostics.ulSetVertexShaderCount;
#endif
			return pShader->m_pShaderSystem->SetVertexShader( pVS );
		}

		i32 CRenderSystem::SetPixelShaderWithCheck(const IPixelShader* pPS)
		{
			xst_assert( pPS, "" );
			const CPixelShader* pShader = (CPixelShader*)pPS;
			if( m_Current.pPixelShader == pShader )
			{
				return XST_OK;
			}
			m_Current.pPixelShader = pShader;
#if defined (XST_ENGINE_DEBUG)
			++g_Diagnostics.ulSetPixelShaderCount;
#endif
			return pShader->m_pShaderSystem->SetPixelShader( pPS );
		}

		i32	CRenderSystem::SetIndexBuffer(const IIndexBuffer* pBuff)
		{
			xst_assert( pBuff, "(CRenderSystem::SetIndexBuffer)" );
			const CIndexBuffer* pIB = (CIndexBuffer*)pBuff;
			m_Current.pIndexBuffer = pIB;
			xst_assert( pIB->m_pD3DIndexBuffer, "(CRenderSystem::SetIndexBuffer)" );
			m_pDeviceContext->IASetIndexBuffer( pIB->m_pD3DIndexBuffer, pIB->m_eD3DDataFormat, 0 );
#if defined (XST_ENGINE_DEBUG)
			++g_Diagnostics.ulSetIndexBufferCount;
#endif
			return XST_OK;
		}

		i32	CRenderSystem::SetIndexBufferWithCheck(const IIndexBuffer* pBuff)
		{
			xst_assert( pBuff, "(CRenderSystem::SetIndexBuffer)" );
			const CIndexBuffer* pIB = (CIndexBuffer*)pBuff;
			if( m_Current.pIndexBuffer == pIB )
			{
				return XST_OK;
			}
			m_Current.pIndexBuffer = pIB;
			xst_assert( pIB->m_pD3DIndexBuffer, "(CRenderSystem::SetIndexBuffer)" );
			m_pDeviceContext->IASetIndexBuffer( pIB->m_pD3DIndexBuffer, pIB->m_eD3DDataFormat, 0 );
#if defined (XST_ENGINE_DEBUG)
			++g_Diagnostics.ulSetIndexBufferCount;
#endif
			return XST_OK;
		}

		i32 CRenderSystem::DrawVertexBuffer(const IVertexBuffer* pVB)
		{
			xst_assert( pVB, "(CRenderSystem::DrawVertexBuffer)" );

			CVertexBuffer* pVBuff = (CVertexBuffer*)pVB;
			cul32 u = pVB->GetVertexCount();
			m_pDeviceContext->Draw( u, 0 );

#if defined (XST_ENGINE_DEBUG)
			++g_Diagnostics.ulDrawCallCount;
#endif
			return XST_OK;
		}

		i32	CRenderSystem::DrawVertexBuffer(const IVertexBuffer* pVB, const IIndexBuffer* pIB)
		{
			xst_assert( pVB, "(CRenderSystem::DrawVertexBuffer)" );
			xst_assert( pIB, "(CRenderSystem::DrawVertexBuffer)" );

			CVertexBuffer* pVBuff = (CVertexBuffer*)pVB;
			CIndexBuffer* pIBuff = (CIndexBuffer*)pIB;
		
			cul32 u = pIB->GetIndexCount();
			m_pDeviceContext->DrawIndexed( u, 0, 0 );

#if defined (XST_ENGINE_DEBUG)
			++g_Diagnostics.ulDrawIndexedCallCount;
#endif
			return XST_OK;
		}

		Resources::IVertexShader*	CRenderSystem::CreateVertexShader(IInputLayout* pIL, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator)
		{
			IShaderSystem* pSS = m_pCurrentShaderSystem;
			if( pSS == xst_null )
			{
				return xst_null;
			}

			IVertexShader* pVS = pSS->CreateVertexShader( pIL, pResourceMgr, ulHandle, strName, iType, iState, pAllocator );
			this->_SetShaderSystem( pVS, pSS );
			return pVS;
		}

		Resources::IPixelShader*	CRenderSystem::CreatePixelShader(XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator)
		{
			IShaderSystem* pSS = m_pCurrentShaderSystem;
			if( pSS == xst_null )
			{
				return xst_null;
			}

			IPixelShader* pPS = pSS->CreatePixelShader( pResourceMgr, ulHandle, strName, iType, iState, pAllocator );
			this->_SetShaderSystem( pPS, pSS );
			return pPS;
		}

		Resources::IVertexShader*	CRenderSystem::CreateDefaultVertexShader(xst_unknown pOptions, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator)
		{
			/*IShaderSystem* pSS = m_pCurrentShaderSystem;
			if( pSS == xst_null )
			{
				return xst_null;
			}

			IVertexShader* pVS = pSS->CreateDefaultVertexShader( pOptions, pResourceMgr, ulHandle, strName, iType, iState, pAllocator );
			if( pVS == xst_null )
			{
				return xst_null;
			}
			this->_SetShaderSystem( pVS, pSS );
			return pVS;*/
			xst_assert2( 0 );
			return xst_null;
		}

		Resources::IPixelShader*	CRenderSystem::CreateDefaultPixelShader(xst_unknown pOptions, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator)
		{
			IShaderSystem* pSS = m_pCurrentShaderSystem;
			if( pSS == xst_null )
			{
				return xst_null;
			}

			IPixelShader* pPS = pSS->CreateDefaultPixelShader( pOptions, pResourceMgr, ulHandle, strName, iType, iState, pAllocator );
			this->_SetShaderSystem( pPS, pSS );
			return pPS;
		}

		ul32 CRenderSystem::GetShaderMaxSize()
		{
			return XST::Math::Max( sizeof( CVertexShader ), sizeof( CPixelShader ) );
		}

		i32	CRenderSystem::AddSupportableVertexShaderProfile(xst_castring& strProfile)
		{
			//Find profile
			StringVector::iterator Itr = std::find( m_avSupportableProfiles[ SHADER_TYPE_VERTEX ].begin(), m_avSupportableProfiles[ SHADER_TYPE_VERTEX ].end(), strProfile );
			if( Itr == m_avSupportableProfiles[ SHADER_TYPE_VERTEX ].end() )
			{
				return XST_FAIL;
			}

			m_avUserProfiles[ SHADER_TYPE_VERTEX ].push_back( strProfile );

			return XST_OK;
		}

		i32	CRenderSystem::AddSupportablePixelShaderProfile(xst_castring& strProfile)
		{
			//Find profile
			StringVector::iterator Itr = std::find( m_avSupportableProfiles[ SHADER_TYPE_PIXEL ].begin(), m_avSupportableProfiles[ SHADER_TYPE_PIXEL ].end(), strProfile );
			if( Itr == m_avSupportableProfiles[ SHADER_TYPE_PIXEL ].end() )
			{
				return XST_FAIL;
			}

			m_avUserProfiles[ SHADER_TYPE_PIXEL ].push_back( strProfile );

			return XST_OK;
		}

		void CRenderSystem::SetMatrix(const MATRIX_TYPE& eType, const Mtx4& mtx)
		{
			XSE_MTX_TO_XMMTX( g_aMatrices[ eType ], mtx );
		}

		void CRenderSystem::SetLookAt(const Vec3& vecPosition, const Vec3& vecPoint, const Vec3& vecUp)
		{
			g_aMatrices[ MatrixTypes::VIEW ] = XMMatrixLookAtLH( VecToXMVec( vecPosition ), VecToXMVec( vecPoint ), VecToXMVec( vecUp ) );
		}

		IRenderSystem::UPDATE_OPTIONS_RESULT CRenderSystem::UpdateOptions(const SRenderSystemOptions& Options)
		{
			return IRenderSystem::UpdateOptionsResults::OK;
		}

		IRenderSystem::UPDATE_OPTIONS_RESULT CRenderSystem::UpdateOptions()
		{
			return IRenderSystem::UpdateOptionsResults::OK;
		}
		
		void CRenderSystem::SetPerspectiveFOV(cf32& fAngle, cf32& fAspectRatio, cf32& fNear, cf32& fFar)
		{
			g_aMatrices[ MatrixTypes::PROJECTION ] = XMMatrixPerspectiveFovLH( fAngle, fAspectRatio, fNear, fFar );
		}

		void CRenderSystem::SetRotationX(cf32& fAngle)
		{
			g_aMatrices[ MatrixTypes::WORLD ] = XMMatrixRotationX( fAngle );
		}

		void CRenderSystem::SetRotationY(cf32& fAngle)
		{
			g_aMatrices[ MatrixTypes::WORLD ] = XMMatrixRotationY( fAngle );
		}

		void CRenderSystem::SetRotationZ(cf32& fAngle)
		{
			g_aMatrices[ MatrixTypes::WORLD ] = XMMatrixRotationZ( fAngle );
		}

		void CRenderSystem::SetRotation(cf32& fAngle, const Vec3& vecAxis)
		{
			g_aMatrices[ MatrixTypes::WORLD ] = XMMatrixRotationAxis( VecToXMVec( vecAxis ), fAngle );
		}

		void CRenderSystem::SetTranslationX(cf32& fOffset)
		{
			g_aMatrices[ MatrixTypes::WORLD ] = XMMatrixTranslation( fOffset, 0.0f, 0.0f ); 
		}

		void CRenderSystem::SetTranslationY(cf32& fOffset)
		{
			g_aMatrices[ MatrixTypes::WORLD ] = XMMatrixTranslation( 0.0f, fOffset, 0.0f );
		}

		void CRenderSystem::SetTranslationZ(cf32& fOffset)
		{
			g_aMatrices[ MatrixTypes::WORLD ] = XMMatrixTranslation( 0.0f, 0.0f, fOffset );
		}

		void CRenderSystem::SetTranslation(cf32& fX, cf32& fY, cf32& fZ)
		{
			g_aMatrices[ MatrixTypes::WORLD ] = XMMatrixTranslation( fX, fY, fZ );
		}

		void CRenderSystem::SetTranslation(const Vec3& vecOffset)
		{
			return SetTranslation( vecOffset.x, vecOffset.y, vecOffset.z );
		}

		void CRenderSystem::SetRotationX(Mtx4* pMtxOut, cf32& fAngle)
		{
			XSE_XMMTX_TO_MTX_PTR( pMtxOut, XMMatrixRotationX( fAngle ) );
		}

		void CRenderSystem::SetRotationY(Mtx4* pMtxOut, cf32& fAngle)
		{
			XSE_XMMTX_TO_MTX_PTR( pMtxOut, XMMatrixRotationY( fAngle ) );
		}

		void CRenderSystem::SetRotationZ(Mtx4* pMtxOut, cf32& fAngle)
		{
			XSE_XMMTX_TO_MTX_PTR( pMtxOut, XMMatrixRotationZ( fAngle ) );
		}

		void CRenderSystem::SetRotation(Mtx4* pMtxOut, cf32& fAngle, const Vec3& vecAxis)
		{
			XSE_XMMTX_TO_MTX_PTR( pMtxOut, XMMatrixRotationAxis( VecToXMVec( vecAxis ), fAngle ) );
		}

		void CRenderSystem::SetTranslationX(Mtx4* pMtxOut, cf32& fOffset)
		{
			XSE_XMMTX_TO_MTX_PTR( pMtxOut, XMMatrixTranslation( fOffset, 0.0f, 0.0f ) ); 
		}

		void CRenderSystem::SetTranslationY(Mtx4* pMtxOut, cf32& fOffset)
		{
			XSE_XMMTX_TO_MTX_PTR( pMtxOut, XMMatrixTranslation( 0.0f, fOffset, 0.0f ) );
		}

		void CRenderSystem::SetTranslationZ(Mtx4* pMtxOut, cf32& fOffset)
		{
			XSE_XMMTX_TO_MTX_PTR( pMtxOut, XMMatrixTranslation( 0.0f, 0.0f, fOffset ) );
		}

		void CRenderSystem::SetTranslation(Mtx4* pMtxOut, cf32& fX, cf32& fY, cf32& fZ)
		{
			XSE_XMMTX_TO_MTX_PTR( pMtxOut, XMMatrixTranslation( fX, fY, fZ ) );
		}

		void CRenderSystem::SetTranslation(Mtx4* pMtxOut, const Vec3& vecOffset)
		{
			return SetTranslation( pMtxOut, vecOffset.x, vecOffset.y, vecOffset.z );
		}

		void CRenderSystem::SetScale(Mtx4* pMtxOut, cf32& fX, cf32& fY, cf32& fZ)
		{
			XSE_XMMTX_TO_MTX_PTR( pMtxOut, XMMatrixScaling( fX, fY, fZ ) );
		}

		void CRenderSystem::SetScale(cf32& fX, cf32& fY, cf32& fZ)
		{
			g_aMatrices[ MatrixTypes::WORLD ] = XMMatrixScaling( fX, fY, fZ );
		}

		void CRenderSystem::GetMatrix(const MATRIX_TYPE& eMtx, XMMATRIX* pMtxOut) const
		{
			*pMtxOut = g_aMatrices[ eMtx ];
		}

		void CRenderSystem::GetMatrix(const MATRIX_TYPE& eMtx, Mtx4* pMtxOut) const
		{
			xst_assert2( pMtxOut );
			const XMMATRIX& mtx = g_aMatrices[ eMtx ];
			pMtxOut->SetRow( 0, mtx.r[ 0 ] );
			pMtxOut->SetRow( 1, mtx.r[ 1 ] );
			pMtxOut->SetRow( 2, mtx.r[ 2 ] );
			pMtxOut->SetRow( 3, mtx.r[ 3 ] );
			//XSE_XMMTX_TO_MTX_PTR( pMtxOut, g_aMatrices[ eMtx ] );
		}

		const	XMMATRIX* CRenderSystem::GetMatrix(const MATRIX_TYPE& eMtx) const
		{
			return &g_aMatrices[ eMtx ];
		}

		void CRenderSystem::UpdateObjectInputs()
		{
			//Update constant buffer
			m_pCurrentShaderSystem->UpdateObjectInputs();
		}

		IIndexBuffer* CRenderSystem::CreateIndexBuffer()
		{
			CIndexBuffer* pIB = xst_new CIndexBuffer( this );
			return pIB;
		}

		i32	CRenderSystem::_CreateIndexBuffer(CIndexBuffer* pIB)
		{
			xst_assert( pIB, "(CRenderSystem::_CreateIndexBuffer)" );
			HRESULT hr = m_pDevice->CreateBuffer( &pIB->m_D3DBufferDesc, &pIB->m_D3DInitData, &pIB->m_pD3DIndexBuffer );
			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "[D3D11]: Failed to create index buffer: " << _ErrorToString( hr ) );
				return XST_FAIL;
			}

			return XST_OK;
		}

		i32 CRenderSystem::CreateShaderSystem(xst_castring& strName)
		{
			return CreateShaderSystem( XST::CHash::GetCRC( strName ) );
		}

		i32 CRenderSystem::CreateShaderSystem(SHADER_LANGUAGE eLanguage)
		{
			//Try to get shader system
			ul32 ulId = eLanguage;
			IRenderSystem::_ShaderSystemIterator Itr = this->m_mpShaderSystems.find( ulId );
			if( Itr != this->m_mpShaderSystems.end() )
			{
				return XST_OK;
			}

			IShaderSystem* pSS = xst_null;
			if( eLanguage == ShaderLanguages::HLSL )
			{
				pSS = xst_new CHLSLShaderSystem();
			}
			else 
			if( eLanguage == ShaderLanguages::CG )
			{
				pSS = xst_new CCGShaderSystem( this );
			}
			else
			{
				XST_LOG_ERR( "[D3D11]: This render system supports only CG and HLSL shaders" );
				return XST_FAIL;
			}

			if( XST_FAILED( pSS->Init( this ) ) )
			{
				xst_delete( pSS );
				return XST_FAIL;
			}

			XST::MapUtils::Insert( this->m_mpShaderSystems, ulId, pSS );
			if( m_pCurrentShaderSystem == xst_null )
			{
				m_pCurrentShaderSystem = pSS;
			}

			return XST_OK;
		}

		IShaderSystem* CRenderSystem::GetShaderSystem(xst_castring &strName)
		{
			return GetShaderSystem( XST::CHash::GetCRC( strName ) );
		}

		IShaderSystem* CRenderSystem::GetShaderSystem(cul32& ulId)
		{
			//Check if shader system is current system
			if( m_pCurrentShaderSystem->GetType() == ulId )
			{
				return m_pCurrentShaderSystem;
			}

			IRenderSystem::_ShaderSystemIterator Itr = this->m_mpShaderSystems.find( ulId );
			if( Itr == this->m_mpShaderSystems.end() )
			{
				return xst_null;
			}
			m_pCurrentShaderSystem = Itr->second;
			return m_pCurrentShaderSystem;
		}

		i32 CRenderSystem::SetShaderSystem(xst_castring& strName)
		{
			xst_assert( GetShaderSystem( strName ) != xst_null, "(CRenderSystem::SetShaderSystem)" );
			return XST_OK;
		}

		i32 CRenderSystem::SetShaderSystem(SHADER_LANGUAGE eLang)
		{
			xst_assert( GetShaderSystem( eLang ) != xst_null, "(CRenderSystem::SetShaderSystem)" );
			return XST_OK;
		}

		void CRenderSystem::DrawIndexed(cu32& uiIndexCount, cu32& uiStartLocation, ci32& iBaseVertexLocation)
		{
			m_pDeviceContext->DrawIndexed( uiIndexCount, uiStartLocation, iBaseVertexLocation );
		}

		void CRenderSystem::SetTopology(TOPOLOGY_TYPE eType)
		{
			m_pDeviceContext->IASetPrimitiveTopology( m_aeTopologies[ eType ] );
		}

		//PROTECTED////////////////////////////////////////////////////////////////////////////////////////////////////

		ul32 CRenderSystem::_GetShaderFlags(cul32& ulFlags)
		{
			ul32 ulShaderFlags = 0;

			if( ulFlags & ShaderFlags::DEBUG )
			{
				ulShaderFlags |= D3D10_SHADER_DEBUG;
			}

			if( ulFlags & ShaderFlags::BACKWARD_COMPAPILITY )
			{
				ulShaderFlags |= D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;
				//ulShaderFlags |= D3D10_SHADER_ENABLE_STRICTNESS;
			}

			return ulShaderFlags;
		}

		ul32 CRenderSystem::_GetShaderFlags()
		{
			return _GetShaderFlags( this->m_Options.ulShaderFlags );
		}

		i32 CRenderSystem::_CompileShaderFromFile(xst_castring& strFileName, IShaderBase* pShader)
		{
			ID3DBlob* pErrorBlob, *pBlob = xst_null;
	
			HRESULT hr = D3DX11CompileFromFileA(	strFileName.data(), xst_null, xst_null, pShader->_GetEntryPoint(), 
													pShader->m_strProfile.data(), pShader->m_ulFlags, 0, xst_null, &pShader->m_pBlob, &pErrorBlob, xst_null );
			if( FAILED( hr ) )
			{
				if( pErrorBlob != xst_null )
				{
					XST_LOG_ERR( "[D3D11]: Compile shader: " << strFileName << " failed: " << (ch8*)pErrorBlob->GetBufferPointer() );
					pErrorBlob->Release();
				}
				else
				{
					XST_LOG_ERR( "[D3D11]: Compile shader: " << strFileName << " failed" );
				}
				return XST_FAIL;
			}

			return XST_OK;
		}

		i32	CRenderSystem::_CompileShader(IShaderBase* pShader, lpcastr lpszProfile)
		{
			return _CompileShaderFromMemory( (cch8*)pShader->_GetShaderData(), pShader->_GetShaderDataSize(), lpszProfile, pShader );
		}

		i32	CRenderSystem::_CompileShaderFromMemory(cch8* pData, ul32 ulDataSize, lpcastr lpszProfile, IShaderBase* pShader)
		{
			if( pShader->GetShaderName().length() == 0 )
			{
				XST_LOG_ERR( "[D3D11]: HLSL shader has no name so it could not be compiled" );
				return XST_FAIL;
			}

			return _CompileShaderFromMemory(pData, ulDataSize, pShader->GetShaderName().data(), xst_null, xst_null, pShader->_GetEntryPoint(), 
											lpszProfile, pShader->m_ulFlags, 0, xst_null, &pShader->m_pBlob, xst_null );
		}

		i32	CRenderSystem::_CompileShaderFromMemory(lpcastr pData, ul32 ulDataSize, lpcastr lpszShaderName, 
													const D3D10_SHADER_MACRO* pMacro, LPD3D10INCLUDE pInclude, 
													lpcastr lpszEntryPoint, lpcastr lpszProfile, u32 uiFlags1, 
													u32 uiFlags2, ID3DX11ThreadPump* pPump, ID3D10Blob** pBlob, 
													HRESULT* pHR)
		{
			ID3DBlob* pError = xst_null;
			HRESULT hr = D3DX11CompileFromMemory(	pData, ulDataSize, lpszShaderName, pMacro, pInclude, lpszEntryPoint, 
													lpszProfile, uiFlags1, uiFlags2, pPump, pBlob, &pError, pHR );
			if( FAILED( hr ) )
			{
				if( pError )
				{
					lpcastr strErr = (lpcastr)pError->GetBufferPointer();
					XST_LOG_ERR( "[D3D11]: Failed to compile shader from memory: " << strErr );
					pError->Release();
				}
				else
				{
					XST_LOG_ERR( "[D3D11]: Failed to compile shader from memory: " << _ErrorToString( hr ) );
				}
				XST_LOG_ERR( pData );
				return XST_FAIL;
			}
			
			return XST_OK;
		}

		i32	CRenderSystem::_CreateVertexShader(CVertexShader* pShader)
		{
			ul32 ulSize = pShader->m_pBlob->GetBufferSize();
			HRESULT hr = m_pDevice->CreateVertexShader( pShader->m_pBlob->GetBufferPointer(), pShader->m_pBlob->GetBufferSize(), xst_null, &pShader->m_pD3DVertexShader );
			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "[D3D11]: VertexShader creation failed: " << _ErrorToString( hr ) );
				return XST_FAIL;
			}
			pShader->m_bIsCompiled = true;
			return XST_OK;
		}

		i32	CRenderSystem::_CreateVertexShader(ID3DBlob* pVSBlob, ID3D11VertexShader** pShader)
		{
			HRESULT hr = m_pDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), xst_null, pShader );
			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "[D3D11]: VertexShader creation failed: " << _ErrorToString( hr ) );
				return XST_FAIL;
			}
			return XST_OK;
		}

		i32	CRenderSystem::_CreatePixelShader(CPixelShader* pShader)
		{
			HRESULT hr = m_pDevice->CreatePixelShader( pShader->m_pBlob->GetBufferPointer(), pShader->m_pBlob->GetBufferSize(), xst_null, &pShader->m_pD3DPixelShader );
			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "[D3D11]: PixelShader creation failed: " << ::GetLastError() );
				return XST_FAIL;
			}
			pShader->m_bIsCompiled = true;
			return XST_OK;
		}

		XST::IAllocator*	CRenderSystem::_CreateVertexShaderMemoryPool(cul32& ulObjCount, XST::IAllocator* pAllocator) 
		{
			xst_assert2( m_pVSMemMgr == xst_null );
			XST::IAllocator* pAlloc = xst_null;

			if( pAllocator )
			{
				pAlloc = pAllocator;
				if( !pAlloc->AllocatePool( sizeof( CVertexShader ), ulObjCount ) )
				{
					XST_LOG_ERR( "Create memory pool failed in CLuaScriptManager" );
					return xst_null;
				}
			}
			else
			{
				pAlloc = xst_new XST::TCFreeListMemoryManager< CVertexShader >( ulObjCount );
			}

			m_pVSMemMgr = pAlloc;
			return pAlloc;
		}

		XST::IAllocator*	CRenderSystem::_CreatePixelShaderMemoryPool(cul32& ulObjCount, XST::IAllocator* pAllocator) 
		{
			xst_assert2( m_pPSMemMgr == xst_null );
			XST::IAllocator* pAlloc = xst_null;

			if( pAllocator )
			{
				pAlloc = pAllocator;
				if( !pAlloc->AllocatePool( sizeof( CPixelShader ), ulObjCount ) )
				{
					XST_LOG_ERR( "Create memory pool failed in CLuaScriptManager" );
					return xst_null;
				}
			}
			else
			{
				pAlloc = xst_new XST::TCFreeListMemoryManager< CPixelShader >( ulObjCount );
			}

			m_pPSMemMgr = pAlloc;
			return pAlloc;
		}

		i32 CRenderSystem::_CreateVertexBuffer(CVertexBuffer* pVB)
		{
			HRESULT hr = m_pDevice->CreateBuffer( &pVB->m_D3DBufferDesc, &pVB->m_D3DInitData, &pVB->m_pD3DBuffer );
			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "[D3D11]: Failed to create vertex buffer: " << _ErrorToString( hr ) );
				return XST_FAIL;
			}

			return XST_OK;
		}

		i32 CRenderSystem::_CreateInputLayout(CInputLayout* pIL)
		{
			u32 uiElementCount = pIL->GetElementCount();

			HRESULT hr = m_pDevice->CreateInputLayout(	pIL->m_aInputElements, uiElementCount, 
														pIL->m_pD3DVertexShaderBlob->GetBufferPointer(), pIL->m_pD3DVertexShaderBlob->GetBufferSize(), 
														&pIL->m_pD3DInputLayout );

			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "[D3D11]: Failed to create input layout: " << _ErrorToString( hr ) );
				//MessageBoxA( 0, XST::CLastError::Get().data(), 0, 0 );
				return XST_FAIL;
			}

			return XST_OK;
		}

		i32 CRenderSystem::_GetCaps(SRSCapapiblities* pCaps)
		{
			xst_assert2( m_pD3DAdapter );

			DXGI_ADAPTER_DESC AdapterDesc;
			m_pD3DAdapter->GetDesc( &AdapterDesc );
			//Get primary monitor
			IDXGIOutput* pAdapterOutput;
			HRESULT hr = m_pD3DAdapter->EnumOutputs( 0, &pAdapterOutput );
			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "[D3D11]: Failed to enumerate dxgi factory adapter outputs: " << _ErrorToString( hr ) );
				return XST_FAIL;
			}

			//Get number of modes that the primary monitor supports for chosen format
			u32 uiModeCount = 0;
			hr = pAdapterOutput->GetDisplayModeList( XSE_D3D11_DEFAULT_FORMAT, DXGI_ENUM_MODES_INTERLACED, &uiModeCount, xst_null );
			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "[D3D11]: Failed to get display modes: " << _ErrorToString( hr ) );
				return XST_FAIL;
			}

			DXGI_MODE_DESC* aModeDescs = xst_new DXGI_MODE_DESC[ uiModeCount ];

			hr = pAdapterOutput->GetDisplayModeList( XSE_D3D11_DEFAULT_FORMAT, DXGI_ENUM_MODES_INTERLACED, &uiModeCount, aModeDescs );
			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "[D3D11]: Failed to get display modes: " << _ErrorToString( hr ) );
				return XST_FAIL;
			}

			for(u32 i = 0; i < uiModeCount; ++i)
			{
				
				SResolution* pRes = xst_null;

				//Find resolution
				SRSCapapiblities::ResolutionVector::iterator Itr;
				xst_stl_foreach( Itr, pCaps->vResolutions )
				{
					if( (*Itr).uiWidth == aModeDescs[ i ].Width && (*Itr).uiHeight == aModeDescs[ i ].Height )
					{
						pRes = &(*Itr);
						break;
					}
				}
				
				//If resolution not found
				if( pRes == xst_null )
				{
					//Create new
					SResolution Res;
					pCaps->vResolutions.push_back( Res );
					pRes = &pCaps->vResolutions.back();
				}
				
				pRes->uiWidth = aModeDescs[ i ].Width;
				pRes->uiHeight = aModeDescs[ i ].Height;
				cu16 cusRefRate = aModeDescs[ i ].RefreshRate.Numerator / aModeDescs[ i ].RefreshRate.Denominator;
				bool bRefRatePresent = false;
				//Add refresh rate if is not present
				SResolution::RefreshVector::iterator RItr;
				xst_stl_foreach( RItr, pRes->vRefreshRates )
				{
					//Do not add new refresh rate
					if( (*RItr) == cusRefRate )
					{
						bRefRatePresent = true;
						break;
					}
				}

				if( !bRefRatePresent )
				{
					pRes->vRefreshRates.push_back( cusRefRate );
				}

			}

			xst_release( pAdapterOutput );
			xst_deletea( aModeDescs );

			//Multisample count
			if( this->m_pDevice )
			{
				u32 uiQuality = 0;
				for(u32 i = 0; i < 32; ++i)
				{
					m_pDevice->CheckMultisampleQualityLevels( XSE_D3D11_DEFAULT_FORMAT, i, &uiQuality );
				}
			}

			//Features
			//Get render system features
			xst_zero( &pCaps->Features, sizeof( RSFeatures ) );
			//Shader model
			switch( this->m_eFeatureLevel )
			{
				case D3D_FEATURE_LEVEL_9_1:
				{
					pCaps->Features.eShaderModel = ShaderModels::SM_2_0;
					pCaps->Features.bTextureArrays = false;
					pCaps->Features.bCubemapArrays = false;
					pCaps->Features.bOcclusionQueries = false;
					pCaps->Features.bInstancing = false;
					pCaps->Features.bNonPowerOfTwoDimTextures = true;
					pCaps->Features.ulMaxTextureDimmension = 2048;
					pCaps->Features.ulMaxCubemapDimmension = 512;
					pCaps->Features.ulMaxVolumeExtent = 256;
					pCaps->Features.ulMaxTextureRepeat = 128;
					pCaps->Features.ulMaxAnisotropy = 2;
					pCaps->Features.ulMaxPrimitiveCount = 65535;
					pCaps->Features.ulMaxInputSlots = 16;
					pCaps->Features.ulMaxRenderTargets = 1;

					//Available shader models
					pCaps->abShaderModels[ ShaderModels::SM_2_0 ] = true;

					//Shader profiles
					pCaps->abShaderProfiles[ ShaderProfiles::VS_2_0 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::PS_2_0 ] = true;
				}
				break;

				case D3D_FEATURE_LEVEL_9_2:
				{
					pCaps->Features.eShaderModel = ShaderModels::SM_2_0;
					pCaps->Features.bTextureArrays = false;
					pCaps->Features.bCubemapArrays = false;
					pCaps->Features.bOcclusionQueries = true;
					pCaps->Features.bInstancing = false;
					pCaps->Features.bNonPowerOfTwoDimTextures = true;
					pCaps->Features.ulMaxTextureDimmension = 2048;
					pCaps->Features.ulMaxCubemapDimmension = 512;
					pCaps->Features.ulMaxVolumeExtent = 256;
					pCaps->Features.ulMaxTextureRepeat = 2048;
					pCaps->Features.ulMaxAnisotropy = 16;
					pCaps->Features.ulMaxPrimitiveCount = 1048575;
					pCaps->Features.ulMaxInputSlots = 16;
					pCaps->Features.ulMaxRenderTargets = 1;

					//Available shader models
					pCaps->abShaderModels[ ShaderModels::SM_2_0 ] = true;

					//Shader profiles
					pCaps->abShaderProfiles[ ShaderProfiles::VS_2_0 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::PS_2_0 ] = true;
				}
				break;

				case D3D_FEATURE_LEVEL_9_3:
				{
					pCaps->Features.eShaderModel = ShaderModels::SM_2_0;
					pCaps->Features.bTextureArrays = false;
					pCaps->Features.bCubemapArrays = false;
					pCaps->Features.bOcclusionQueries = true;
					pCaps->Features.bInstancing = true;
					pCaps->Features.bNonPowerOfTwoDimTextures = true;
					pCaps->Features.ulMaxTextureDimmension = 4096;
					pCaps->Features.ulMaxCubemapDimmension = 4096;
					pCaps->Features.ulMaxVolumeExtent = 256;
					pCaps->Features.ulMaxTextureRepeat = 8192;
					pCaps->Features.ulMaxAnisotropy = 16;
					pCaps->Features.ulMaxPrimitiveCount = 1048575;
					pCaps->Features.ulMaxInputSlots = 16;
					pCaps->Features.ulMaxRenderTargets = 4;

					//Available shader models
					pCaps->abShaderModels[ ShaderModels::SM_2_0 ] = true;

					//Shader profiles
					pCaps->abShaderProfiles[ ShaderProfiles::VS_2_0 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::PS_2_0 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::VS_2_X ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::PS_2_X ] = true;
				}
				break;

				case D3D_FEATURE_LEVEL_10_0:
				{
					pCaps->Features.eShaderModel = ShaderModels::SM_4_0;
					pCaps->Features.bTextureArrays = true;
					pCaps->Features.bCubemapArrays = false;
					pCaps->Features.bOcclusionQueries = true;
					pCaps->Features.bInstancing = true;
					pCaps->Features.bNonPowerOfTwoDimTextures = true;
					pCaps->Features.ulMaxTextureDimmension = 8192;
					pCaps->Features.ulMaxCubemapDimmension = 8192;
					pCaps->Features.ulMaxVolumeExtent = 2048;
					pCaps->Features.ulMaxTextureRepeat = 8192;
					pCaps->Features.ulMaxAnisotropy = 16;
					pCaps->Features.ulMaxPrimitiveCount = 4294967296UL;
					pCaps->Features.ulMaxInputSlots = 16;
					pCaps->Features.ulMaxRenderTargets = 8;

					//Available shader models
					pCaps->abShaderModels[ ShaderModels::SM_2_0 ] = true;
					pCaps->abShaderModels[ ShaderModels::SM_4_0 ] = true;

					//Shader profiles
					pCaps->abShaderProfiles[ ShaderProfiles::VS_2_0 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::PS_2_0 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::VS_2_X ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::PS_2_X ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::VS_4_0 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::PS_4_0 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::GS_4_0 ] = true;
				}
				break;

				case D3D_FEATURE_LEVEL_10_1:
				{
					pCaps->Features.eShaderModel = ShaderModels::SM_4_0;
					pCaps->Features.bTextureArrays = true;
					pCaps->Features.bCubemapArrays = true;
					pCaps->Features.bOcclusionQueries = true;
					pCaps->Features.bInstancing = true;
					pCaps->Features.bNonPowerOfTwoDimTextures = true;
					pCaps->Features.ulMaxTextureDimmension = 8192;
					pCaps->Features.ulMaxCubemapDimmension = 8192;
					pCaps->Features.ulMaxVolumeExtent = 2048;
					pCaps->Features.ulMaxTextureRepeat = 8192;
					pCaps->Features.ulMaxAnisotropy = 16;
					pCaps->Features.ulMaxPrimitiveCount = 4294967296UL;
					pCaps->Features.ulMaxInputSlots = 32;
					pCaps->Features.ulMaxRenderTargets = 8;

					pCaps->abShaderModels[ ShaderModels::SM_2_0 ] = true;
					pCaps->abShaderModels[ ShaderModels::SM_4_0 ] = true;
					//Shader profiles
					pCaps->abShaderProfiles[ ShaderProfiles::VS_2_0 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::PS_2_0 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::VS_2_X ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::PS_2_X ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::VS_4_0 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::PS_4_0 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::GS_4_0 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::GS_4_1 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::VS_4_1 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::PS_4_1 ] = true;
				}
				break;

				case D3D_FEATURE_LEVEL_11_0:
				{
					pCaps->Features.eShaderModel = ShaderModels::SM_5_0;
					pCaps->Features.bTextureArrays = true;
					pCaps->Features.bCubemapArrays = true;
					pCaps->Features.bOcclusionQueries = true;
					pCaps->Features.bInstancing = true;
					pCaps->Features.bNonPowerOfTwoDimTextures = true;
					pCaps->Features.ulMaxTextureDimmension = 16384;
					pCaps->Features.ulMaxCubemapDimmension = 16384;
					pCaps->Features.ulMaxVolumeExtent = 2048;
					pCaps->Features.ulMaxTextureRepeat = 16384;
					pCaps->Features.ulMaxAnisotropy = 16;
					pCaps->Features.ulMaxPrimitiveCount = 4294967296UL;
					pCaps->Features.ulMaxInputSlots = 32;
					pCaps->Features.ulMaxRenderTargets = 8;

					pCaps->abShaderModels[ ShaderModels::SM_2_0 ] = true;
					pCaps->abShaderModels[ ShaderModels::SM_4_0 ] = true;
					pCaps->abShaderModels[ ShaderModels::SM_5_0 ] = true;
					//Shader profiles
					pCaps->abShaderProfiles[ ShaderProfiles::VS_2_0 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::PS_2_0 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::VS_2_X ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::PS_2_X ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::VS_4_0 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::PS_4_0 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::VS_5_0 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::PS_5_0 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::GS_5_0 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::GS_4_0 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::CS_5_0 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::DS_5_0 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::HS_5_0 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::GS_4_1 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::VS_4_1 ] = true;
					pCaps->abShaderProfiles[ ShaderProfiles::PS_4_1 ] = true;
				}
				break;
			};

			return XST_OK;
		}

		lpcastr CRenderSystem::_ErrorToString(HRESULT ulErr)
		{
			int i = 0;
			i = D3D11_ERROR_FILE_NOT_FOUND;
			i = D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS;
			i = D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS;
			i = D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD;
			i = D3DERR_INVALIDCALL;
			i = D3DERR_WASSTILLDRAWING;
			i = E_FAIL;
			i = E_INVALIDARG;
			i = E_OUTOFMEMORY;
			i = S_FALSE;
			i = S_OK;

			switch( ulErr )
			{
				case D3D11_ERROR_FILE_NOT_FOUND: return XST_TOSTRING( D3D11_ERROR_FILE_NOT_FOUND ); break;
				case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS: return XST_TOSTRING( D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS ); break;
				case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS: return XST_TOSTRING( D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS ); break;
				case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD: return XST_TOSTRING( D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD ); break;
				case D3DERR_INVALIDCALL: return XST_TOSTRING( D3DERR_INVALIDCALL ); break;
				case D3DERR_WASSTILLDRAWING: return XST_TOSTRING( D3DERR_WASSTILLDRAWING ); break;
				case E_FAIL: return XST_TOSTRING( E_FAIL ); break;
				case E_INVALIDARG: return XST_TOSTRING( E_INVALIDARG ); break;
				case E_OUTOFMEMORY: return XST_TOSTRING( E_OUTOFMEMORY ); break;
				case S_FALSE: return XST_TOSTRING( S_FALSE ); break;
				case S_OK: return XST_TOSTRING( S_OK ); break;
			}

			return "UNKNOWN ERROR";
		}

	}//d3d11
}//xse

#if defined (XST_VISUAL_STUDIO)
#	pragma warning( pop ) //macro redefinition
#endif

#endif //XST_WINDOWS