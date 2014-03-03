#include "XSECEngine.h"

#include "XSERenderSystemCommon.h"
#include "XSECLuaScriptManager.h"
#include "XSECMaterialManager.h"
#include "XSECResourceFileManager.h"
#include "XSECModelManager.h"
#include "XSECTextureManager.h"
#include "XSECShaderManager.h"
#include "Win32/XSECWin32RenderWindow.h"
#include "XSECRenderSystemFactory.h"
#include "D3D11/XSECD3D11RenderSystem.h"
#include "D3D9/XSECD3D9RenderSystem.h"
#include "XSECSceneManager.h"
#include "XSECMeshManager.h"
#include "XSECModelManager.h"
#include "XSEIShaderSystem.h"
#include "XSECImageManager.h"
#include "XSECTerrainSystem.h"
#include "XSECCamera.h"

#define XSE_LUA_COUNT	256

#define XST_CREATE_SINGLETON(_ptr, _type) \
	if( !_type::IsSingletonCreated() ) \
		_ptr = xst_new _type(); \
	else \
		_ptr = _type::GetSingletonPtr();

#if defined (XST_WINDOWS)
#	if defined (XST_VISUAL_STUDIO)
//#		pragma comment( lib, "winmm.lib" )
//#		pragma comment( lib, "comctl32.lib" )
#	else //if not XST_VISUAL_STUDIO
#	endif //XST_VISUAL_STUDIO
//WtsApi32.lib msi.lib shlwapi.lib
//#	pragma comment( lib, "WtsApi32.lib" )
//#	pragma comment( lib, "msi.lib" )
//#	pragma comment( lib, "shlwapi.lib" )
#else //if not XST_WINDOWS
#endif //XST_WINDOWS

namespace XSE
{

	IRenderSystem* CreateD3D11RenderSystem()
	{
		return xst_new D3D11::CRenderSystem( CEngine::DIRECT3D11 );
	}

	IRenderSystem* CreateD3D9RenderSystem()
	{
		//return xst_new D3D9::CRenderSystem( CEngine::DIRECT3D9 );
		return xst_null;
	}

	XSE_PFN_CREATERENDERSYSTEM	CreateRenderSystem = xst_null;
	XSE_PFN_DESTROYRENDERSYSTEM	DestroyRenderSystem = xst_null;
	XSE_PFN_GETRENDERSYSTEMNAME	GetRenderSystemName = xst_null;

	xst_castring CEngine::DIRECT3D11 = "Direct3D11";
	xst_castring CEngine::DIRECT3D9 = "Direct3D9";
	xst_castring CEngine::BEST_RENDER_SYSTEM = "BEST";
	CEngineOptions CEngine::DEFAULT_OPTIONS = CEngineOptions();

	

	CEngine::CEngine()
	{
		m_pRenderSystem		= xst_null;
		m_pCurrentScene		= xst_null;
		m_pCurrRenderWindow = xst_null;

		if( !XST::CFileManager::IsSingletonCreated() )
			m_pFileMgr = xst_new XST::CFileManager();
		else
			m_pFileMgr = XST::CFileManager::GetSingletonPtr();

		if( !CLuaScriptManager::IsSingletonCreated() )
			m_pLuaScriptMgr = xst_new CLuaScriptManager();
		else
			m_pLuaScriptMgr = CLuaScriptManager::GetSingletonPtr();

		m_pShaderMgr = xst_new CShaderManager();

		if( !CMaterialManager::IsSingletonCreated() )
			m_pMatMgr = xst_new CMaterialManager( m_pShaderMgr );
		else
			m_pMatMgr = CMaterialManager::GetSingletonPtr();

		m_pResFileMgr = xst_new XSE::CResourceFileManager( m_pFileMgr );

		XST_CREATE_SINGLETON( m_pImgMgr, XSE::CImageManager );
		XST_CREATE_SINGLETON( m_pTexMgr, XSE::CTextureManager );
		XST_CREATE_SINGLETON( m_pRenderSystemFactory, XSE::CRenderSystemFactory );
		
		m_pMeshMgr = xst_new XSE::CMeshManager( m_pMatMgr );
		m_pModelMgr = xst_new XSE::CModelManager( m_pMeshMgr );

		m_pRenderSystemFactory->RegisterRenderSystem( CEngine::DIRECT3D11, &CreateD3D11RenderSystem ); 
		m_pRenderSystemFactory->RegisterRenderSystem( CEngine::DIRECT3D9, &CreateD3D9RenderSystem );

		//Set default options
		DEFAULT_OPTIONS.RSOptions.bFullScreen = false;
		DEFAULT_OPTIONS.RSOptions.uiBitsPerPixel = 24;
		DEFAULT_OPTIONS.RSOptions.uiRefreshRate = 60;
		DEFAULT_OPTIONS.RSOptions.uiResolutionWidth = 800;
		DEFAULT_OPTIONS.RSOptions.uiResolutionHeight = 600;

		xst_zero( &m_Features, sizeof( SFeatures ) );

		//Randomize
		XST::Random::Randomize();
	}

	CEngine::~CEngine()
	{
		//Destroy scene managers
		for(SceneMgrMap::iterator Itr = m_mScenes.begin(); Itr != m_mScenes.end(); ++Itr)
		{
			DestroySceneManager( Itr->second );
		}

		m_mScenes.clear();

		m_pModelMgr->Destroy();
		xst_delete( m_pModelMgr );
		
		m_pMeshMgr->Destroy();
		xst_delete( m_pMeshMgr );

		//Destroy windows
		DestroyWindows();
		
		m_pShaderMgr->Destroy();
		xst_delete( m_pShaderMgr );
		
		m_pTexMgr->Destroy();
		xst_delete( m_pTexMgr );
		
		m_pImgMgr->Destroy();
		xst_delete( m_pImgMgr );
		
		m_pMatMgr->Destroy();
		xst_delete( m_pMatMgr );

		m_pLuaScriptMgr->Destroy();
		xst_delete( m_pLuaScriptMgr );
		
		//m_pResFileMgr->Destroy();
		xst_delete( m_pResFileMgr );

		if( XSE::DestroyRenderSystem && m_hRenderSystemDll )
		{	
			XSE::DestroyRenderSystem( &m_pRenderSystem );
			XST::Platform::CloseLibrary( m_hRenderSystemDll );
		}
		else
		{
			xst_delete( m_pRenderSystem );
		}

		xst_delete( m_pRenderSystemFactory );

		xst_delete( m_pFileMgr );
	}

	void CEngine::DestroyWindows()
	{
		for(WindowMap::iterator Itr = m_mWindows.begin(); Itr != m_mWindows.end(); ++Itr)
		{
			xst_delete( Itr->second );
		}

		m_mWindows.clear();
	}

	i32 CEngine::Init(const CEngineOptions& Options)
	{
		//Check capabilities
		if( !CheckCaps() )
		{
			return RESULT::FAILED;
		}

		//Enable sse
		//XST::CMath::EnableFullSSE2();
		IResourceManager* pTmpMgr = xst_null;
		pTmpMgr = m_pLuaScriptMgr;
		if( pTmpMgr->CreateMemoryPool( XSE_LUA_COUNT ) != RESULT::OK )
		{
			return RESULT::FAILED;
		}
		pTmpMgr = m_pMeshMgr;
		if( pTmpMgr->CreateMemoryPool( XSE_LUA_COUNT ) != RESULT::OK )
		{
			return RESULT::FAILED;
		}
		pTmpMgr = m_pMatMgr;
		if( XST_FAILED( pTmpMgr->CreateMemoryPool( XSE_LUA_COUNT ) ) )
		{
			return XST_FAIL;
		}
		pTmpMgr = m_pModelMgr;
		if( XST_FAILED( pTmpMgr->CreateMemoryPool( XSE_LUA_COUNT ) ) )
		{
			return XST_FAIL;
		}
		pTmpMgr = m_pImgMgr;
		if( XST_FAILED( pTmpMgr->CreateMemoryPool( XSE_LUA_COUNT ) ) )
		{
			return XST_FAIL;
		}
		pTmpMgr = m_pTexMgr;
		if( XST_FAILED( pTmpMgr->CreateMemoryPool( XSE_LUA_COUNT ) ) )
		{
			return XST_FAIL;
		}
		

		//Init render system
		//If no render system created, create default best render system for this machine
		
		//Create render system
		//Use render system plugin ONLY if render system from engine is not set
		if( Options.strRenderSystemPlugin.length() > 0 && Options.strRenderSystem.length() == 0 )
		{
			m_pRenderSystem = CreateRenderSystemFromPlugin( Options.strRenderSystemPlugin );
		}
		else if( Options.strRenderSystem.length() > 0 )
		{
			m_pRenderSystem = CreateRenderSystem( Options.strRenderSystem );
		}
		else
		{
			m_pRenderSystem = CreateRenderSystem( CEngine::BEST_RENDER_SYSTEM );
		}

		if( m_pRenderSystem == xst_null )
		{
			return RESULT::FAILED;
		}

		if( m_pRenderSystem->Init( Options.RSOptions ) != RESULT::OK )
		{
			return RESULT::FAILED;
		}

		for(WindowMap::iterator Itr = m_mWindows.begin(); Itr != m_mWindows.end(); ++Itr)
		{
			Itr->second->_SetRenderSystem( m_pRenderSystem );

			if( !Options.RSOptions.bFullScreen )
			{
				Itr->second->SetSize( Options.RSOptions.uiResolutionWidth, Options.RSOptions.uiResolutionHeight + 1 );
			}
		}

		m_pShaderMgr->_SetRenderSystem( m_pRenderSystem );
		m_pMeshMgr->_SetRenderSystem( m_pRenderSystem );
		m_pModelMgr->_SetRenderSystem( m_pRenderSystem );
		/*for(SceneIterator Itr = m_mScenes.begin(); Itr != m_mScenes.end(); ++Itr)
		{
			Itr->second->_SetRenderSystem( m_pRenderSystem );
		}*/

		//Shader manager uses render system to create memory pools for different shader types
		pTmpMgr = m_pShaderMgr;
		if( XST_FAILED( pTmpMgr->CreateMemoryPool( XSE_LUA_COUNT ) ) )
		{
			return XST_FAIL;
		}

		//Create input layouts
		typedef InputLayoutElements ILE;
		m_pRenderSystem->BuildInputLayout( ILE::POSITION );
		m_pRenderSystem->BuildInputLayout( ILE::POSITION | ILE::COLOR );
		m_pRenderSystem->BuildInputLayout( ILE::POSITION | ILE::NORMAL );
		//m_pRenderSystem->BuildInputLayout( ILE::POSITION | ILE::NORMAL | ILE::TEXCOORD0 );
		//m_pRenderSystem->BuildInputLayout( ILE::POSITION | ILE::NORMAL | ILE::COLOR );
		/*m_pRenderSystem->BuildInputLayout( ILE::NORMAL | ILE::TANGENT );
		m_pRenderSystem->BuildInputLayout( ILE::NORMAL | ILE::SPECULAR );
		m_pRenderSystem->BuildInputLayout( ILE::NORMAL | ILE::DIFFUSE );
		m_pRenderSystem->BuildInputLayout( ILE::NORMAL | ILE::TEXCOORD1 );
		m_pRenderSystem->BuildInputLayout( ILE::NORMAL | ILE::TEXCOORD0 | ILE::BINORMAL );
		m_pRenderSystem->BuildInputLayout( ILE::NORMAL | ILE::TEXCOORD0 | ILE::TANGENT );
		m_pRenderSystem->BuildInputLayout( ILE::NORMAL | ILE::TEXCOORD0 | ILE::BINORMAL | ILE::TANGENT );
		m_pRenderSystem->BuildInputLayout( ILE::NORMAL | ILE::TEXCOORD0 | ILE::SPECULAR | ILE::TEXCOORD1 );
		m_pRenderSystem->BuildInputLayout( ILE::NORMAL | ILE::TEXCOORD0 | ILE::DIFFUSE | ILE::TEXCOORD1 );
		m_pRenderSystem->BuildInputLayout( ILE::NORMAL | ILE::TEXCOORD0 | ILE::SPECULAR | ILE::DIFFUSE | ILE::TEXCOORD1 );
		m_pRenderSystem->BuildInputLayout( ILE::NORMAL | ILE::TEXCOORD0 | ILE::SPECULAR | ILE::DIFFUSE | ILE::TEXCOORD1 | ILE::BINORMAL );
		m_pRenderSystem->BuildInputLayout( ILE::NORMAL | ILE::TEXCOORD0 | ILE::SPECULAR | ILE::DIFFUSE | ILE::TEXCOORD1 | ILE::TANGENT );
		m_pRenderSystem->BuildInputLayout( ILE::NORMAL | ILE::TEXCOORD0 | ILE::SPECULAR | ILE::DIFFUSE | ILE::TEXCOORD1 | ILE::TANGENT | ILE::BINORMAL );
		*///m_pRenderSystem->BuildInputLayout( ILE:: | ILE:: );

		//Get features
		m_Features.RSFeatures = m_pRenderSystem->GetFeatures();


		//Init managers
		pTmpMgr = m_pLuaScriptMgr;
		if( XST_FAILED( pTmpMgr->Init() ) )
		{
			return XST_FAIL;
		}
		pTmpMgr = m_pShaderMgr;
		if( XST_FAILED( pTmpMgr->Init() ) )
		{
			return XST_FAIL;
		}
		pTmpMgr = m_pImgMgr;
		if( XST_FAILED( pTmpMgr->Init() ) )
		{
			return XST_FAIL;
		}
		pTmpMgr = m_pTexMgr;
		if( XST_FAILED( pTmpMgr->Init() ) )
		{
			return XST_FAIL;
		}
		pTmpMgr = m_pMatMgr;
		if( XST_FAILED( pTmpMgr->Init() ) )
		{
			return XST_FAIL;
		}
		pTmpMgr = m_pMeshMgr;
		if( XST_FAILED( pTmpMgr->Init() ) )
		{
			return XST_FAIL;
		}
		pTmpMgr = m_pModelMgr;
		if( XST_FAILED( pTmpMgr->Init() ) )
		{
			return XST_FAIL;
		}

		i32 iLoadingOrder = 0;
		m_pResFileMgr->AddExtension( "dds",		XSE::ResourceTypes::IMAGE, iLoadingOrder, m_pImgMgr );
		m_pResFileMgr->AddExtension( "bmp",		XSE::ResourceTypes::IMAGE, iLoadingOrder, m_pImgMgr );
		m_pResFileMgr->AddExtension( "tga",		XSE::ResourceTypes::IMAGE, iLoadingOrder, m_pImgMgr );
		m_pResFileMgr->AddExtension( "jpg",		XSE::ResourceTypes::IMAGE, iLoadingOrder, m_pImgMgr );
		m_pResFileMgr->AddExtension( "jpeg",	XSE::ResourceTypes::IMAGE, iLoadingOrder, m_pImgMgr );
		m_pResFileMgr->AddExtension( "raw",		XSE::ResourceTypes::IMAGE, iLoadingOrder, m_pImgMgr );
		++iLoadingOrder;
		m_pResFileMgr->AddExtension( "fx",		XST::ResourceType::SHADER, iLoadingOrder, m_pShaderMgr );
		m_pResFileMgr->AddExtension( "vs",		XST::ResourceType::SHADER, iLoadingOrder, m_pShaderMgr );
		m_pResFileMgr->AddExtension( "ps",		XST::ResourceType::SHADER, iLoadingOrder, m_pShaderMgr );
		m_pResFileMgr->AddExtension( "gs",		XST::ResourceType::SHADER, iLoadingOrder, m_pShaderMgr );
		++iLoadingOrder;
		m_pResFileMgr->AddExtension( "material",XST::ResourceType::MATERIAL, iLoadingOrder, m_pMatMgr );
		m_pResFileMgr->AddExtension( "mat",		XST::ResourceType::MATERIAL, iLoadingOrder, m_pMatMgr );
		++iLoadingOrder;
		m_pResFileMgr->AddExtension( "model", XST::ResourceType::MESH, iLoadingOrder, m_pModelMgr );
		++iLoadingOrder;
		m_pResFileMgr->AddExtension( "lua", XST::ResourceType::LUA_SCRIPT, iLoadingOrder, m_pLuaScriptMgr );
		++iLoadingOrder;
		

		return RESULT::OK;
	}

	i32 CEngine::RegisterShaderSystem(xst_castring& strName)
	{
		return m_pRenderSystem->CreateShaderSystem( strName );
	}

	i32 CEngine::RegisterShaderSystem(SHADER_LANGUAGE eLang)
	{
		return m_pRenderSystem->CreateShaderSystem( eLang );
	}

	i32 CEngine::SetShaderSystem(SHADER_LANGUAGE eLang)
	{
		return m_pRenderSystem->SetShaderSystem( eLang );
	}

	i32 CEngine::SetShaderSystem(xst_castring& strName)
	{
		return m_pRenderSystem->SetShaderSystem( strName );
	}

	void BuildInputLayouts(IRenderSystem* pRS)
	{
		ul32 aElements[ InputLayoutElements::_ENUM_COUNT ];
		aElements[0] = InputLayoutElements::NORMAL;
		aElements[1] = InputLayoutElements::TEXCOORD0;
		aElements[2] = InputLayoutElements::BINORMAL;
		aElements[3] = InputLayoutElements::TANGENT;
		aElements[4] = InputLayoutElements::SPECULAR;
		aElements[5] = InputLayoutElements::DIFFUSE;
		aElements[6] = InputLayoutElements::TEXCOORD1;

		xst_vector< ul32 > vElements;

		for(ul32 i = 0; i < InputLayoutElements::_ENUM_COUNT; ++i)
		{
			for(ul32 j = 0; j < InputLayoutElements::_ENUM_COUNT; ++j)
			{
				if( i != j )
				{
					vElements.push_back( aElements[ i ] | aElements[ j ] );
				}
			}
		}
	}

	bool CEngine::CheckCaps()
	{
		SFeatures Caps = GetCaps();
		
		if( !Caps.bSSE )
		{
			XST_LOG_ERR( "SSE is not supported on this machine" );
			return false;
		}

		if( !Caps.bSSE2 )
		{
			XST_LOG_ERR( "SSE2 is not supported on this machine" );
			return false;
		}

		//Check directx 
		#if defined (XST_WINDOWS)

		#endif

		return true;
	}

	SFeatures CEngine::GetCaps()
	{
		//Check simd
		XST::SCPUInfo CPUInfo;
		xst_zero( &CPUInfo, sizeof( XST::SCPUInfo ) );
		XST::Platform::GetCPUInfo( &CPUInfo );
		
		SFeatures Caps;
		xst_zero( &Caps, sizeof( SFeatures ) );
		
		Caps.b3DNow		= CPUInfo.bFeature3DNow;
		Caps.b3DNowExt	= CPUInfo.bFeature3DNowExt;
		Caps.bSSE		= CPUInfo.bFeatureSSE;
		Caps.bSSE2		= CPUInfo.bFeatureSSE2;
		Caps.bSSE3		= CPUInfo.bFeatureSSE3;
		Caps.bSSE4		= CPUInfo.bFeatureSSE4a;
		Caps.bSSE41		= CPUInfo.bFeatureSSE41;
		Caps.bSSE42		= CPUInfo.bFeatureSSE42;
		Caps.bSSE5		= CPUInfo.bFeatureSSE5;
		Caps.bMMX		= CPUInfo.bFeatureMMX;


		
		return Caps;
	}

	IRenderWindow* CEngine::GetRenderWindow(xst_castring& strName)
	{
		if( strName.empty() )
		{
			return m_pCurrRenderWindow;
		}

		WindowMap::iterator Itr = m_mWindows.find( strName );
		if( Itr == m_mWindows.end() )
		{
			return xst_null;
		}

		return Itr->second;
	}

	IRenderWindow* CEngine::GetRenderWindow(cul32& ulHWND)
	{
		if( ulHWND == 0 )
		{
			return m_pCurrRenderWindow;
		}

		WindowMap::iterator Itr = m_mWindows.begin();
		for(; Itr != m_mWindows.end(); ++Itr)
		{
			if( Itr->second->GetHandle() == ulHWND )
			{
				return Itr->second;
			}
		}

		return xst_null;
	}

	IRenderWindow* CEngine::CreateRenderWindow(xst_castring& strName, u32 uiExternalHandle)
	{
		IRenderWindow* pRenderWnd = GetRenderWindow( strName );
		if( pRenderWnd != xst_null )
		{
			XST_LOG_ERR( "Window: " << strName << " already exists" );
			return xst_null;
		}

		#if defined (XST_WINDOWS)
			
			pRenderWnd = xst_new Win32::CRenderWindow( this );
			pRenderWnd->SetTitle( strName );
			if( pRenderWnd->Create( uiExternalHandle ) != RESULT::OK )
			{
				xst_delete( pRenderWnd );
				return xst_null;
			}

			m_mWindows.insert( WindowMap::value_type( strName, pRenderWnd ) );

			m_pCurrRenderWindow = pRenderWnd;

		#else
		#error "This platform is not supported"
		#endif

		return m_pCurrRenderWindow;
	}

	CEngine::WindowIterator CEngine::GetWindowIterator()
	{
		return WindowIterator( &m_mWindows );
	}

	IRenderSystem* CEngine::CreateRenderSystem(xst_castring& strName)
	{
		xst_assert( m_pRenderSystem == xst_null, "(CEngine::CreateRenderSystem) Render system already created" );
		if( strName == "BEST" )
		{
			xst_astring strRSName = "";

			SFeatures Caps = GetCaps();
			XST::SPlatformInfo PlatformInfo;
			if( XST::Platform::GetPlatformInfo( &PlatformInfo ) != RESULT::OK )
			{
				XST_LOG_ERR( "Could not detect best render system for this machine" );
				return xst_null;
			}

			if( PlatformInfo.uiSystemType == XST::SystemType::WINDOWS_2000 || 
				PlatformInfo.uiSystemType == XST::SystemType::WINDOWS_XP || 
				PlatformInfo.uiSystemType == XST::SystemType::WINDOWS_XP_64 ||
				PlatformInfo.uiSystemType == XST::SystemType::WINDOWS_SERVER_2003 ||
				PlatformInfo.uiSystemType == XST::SystemType::WINDOWS_SERVER_2003_R2 )
			{
				strRSName = CEngine::DIRECT3D9;
			}
			else
			if( PlatformInfo.uiSystemType == XST::SystemType::WINDOWS_7 || 
				PlatformInfo.uiSystemType == XST::SystemType::WINDOWS_SERVER_2008 || 
				PlatformInfo.uiSystemType == XST::SystemType::WINDOWS_SERVER_2008_R2 ||
				PlatformInfo.uiSystemType == XST::SystemType::WINDOWS_VISTA )
			{
				strRSName = CEngine::DIRECT3D11;
			}
			else
			{
			}

			m_pRenderSystem = m_pRenderSystemFactory->CreateRenderSystem( strRSName );
		}
		else
		{
			m_pRenderSystem = m_pRenderSystemFactory->CreateRenderSystem( strName );
		}

		return m_pRenderSystem;
	}

	IRenderSystem* CEngine::CreateRenderSystemFromPlugin(xst_castring& strName)
	{
		m_hRenderSystemDll = XST::Platform::LoadLibrary( strName.data() );
		if( m_hRenderSystemDll == xst_null )
		{
			XST_LOG_ERR( "Unable to load render system plugin: " << strName );
			return xst_null;
		}

		XSE::CreateRenderSystem	= (XSE_PFN_CREATERENDERSYSTEM)XST::Platform::GetProcAddress( m_hRenderSystemDll, XST_TOSTRING( CreateRenderSystem ) );
		XSE::DestroyRenderSystem = (XSE_PFN_DESTROYRENDERSYSTEM)XST::Platform::GetProcAddress( m_hRenderSystemDll, XST_TOSTRING( DestroyRenderSystem ) );
		XSE::GetRenderSystemName = (XSE_PFN_GETRENDERSYSTEMNAME)XST::Platform::GetProcAddress( m_hRenderSystemDll, XST_TOSTRING( GetRenderSystemName ) );
		
		if( !XSE::CreateRenderSystem || !XSE::DestroyRenderSystem || !XSE::GetRenderSystemName )
		{
			XST_LOG_ERR( "Unable to load one of the render system functions" );
			return xst_null;
		}

		m_pRenderSystem = XSE::CreateRenderSystem();
		if( m_pRenderSystem == xst_null )
		{
			XST_LOG_ERR( "Unable to create render system" );
			return xst_null;
		}

		return m_pRenderSystem;
	}

	CSceneManager*	CEngine::CreateSceneManager(xst_castring& strName, f32 fSceneSize, bool bSetAsCurrent)
	{
		xst_assert2( m_pImgMgr );
		xst_assert2( m_pRenderSystem );

		CSceneManager* pScene = xst_new CSceneManager( strName, m_pModelMgr, fSceneSize );
		if( pScene == xst_null )
		{
			XST_LOG_ERR( "Unable to create scene manager: " << strName << ". Memory error" );
			return xst_null;
		}

		if( XST_FAILED( pScene->Init() ) )
		{
			xst_delete( pScene );
		}

		if( XST_FAILED( XST::MapUtils::Insert( m_mScenes, strName, pScene ) ) )
		{
			xst_delete( pScene );
			XST_LOG_ERR( "Scene: " << strName << " already exists" );
			return xst_null;
		}

		//pScene->_SetRenderSystem( m_pRenderSystem );
		CCamera* pCam = pScene->CreateCamera( CSceneManager::DEFAULT_CAMERA );
		pScene->SetCamera( pCam );

		pScene->SetTerrainSystem( xst_new CTerrainSystem( pScene, m_pImgMgr, m_pRenderSystem ) );

		if( bSetAsCurrent )
		{
			SetSceneManager( pScene );
		}

		return pScene;
	}
					
	i32	CEngine::SetSceneManager(xst_castring& strName)
	{
		return SetSceneManager( m_mScenes[ strName ] );
	}

	i32	CEngine::SetSceneManager(CSceneManager* pSceneMgr)
	{
		m_pCurrentScene = pSceneMgr;
		return XST_OK;
	}

	void CEngine::DestroySceneManager(xst_castring& strName)
	{
		SceneMgrMap::iterator Itr = m_mScenes.find( strName );
		CSceneManager* pMgr = Itr->second;
		if( Itr != m_mScenes.end() )
		{
			m_mScenes.erase( Itr );
		}

		return _DestroySceneManager( pMgr );
	}

	void CEngine::DestroySceneManager(CSceneManager* pSceneMgr)
	{
		//Find this scene manager
		SceneMgrMap::iterator Itr = m_mScenes.find( pSceneMgr->GetName() );
		if( Itr != m_mScenes.end() )
		{
			m_mScenes.erase( Itr );
		}
		_DestroySceneManager( pSceneMgr );
	}

	void CEngine::_DestroySceneManager(CSceneManager* pMgr)
	{
		xst_delete( pMgr );
	}

	void CEngine::Update(cf32& fFrameTime)
	{
		m_pCurrentScene->Update( fFrameTime );
	}

	// OPTIONS
	i32 CEngine::SetScreenResolution(u32 uiWidth, u32 uiHeight, u32 uiRefreshRate)
	{
		if( m_pRenderSystem )
		{
			if( XST_FAILED( m_pRenderSystem->ResizeBuffers( uiWidth, uiHeight ) ) )
			{
				return RESULT::FAILED;
			}
		}

		for( auto& SceneItr : m_mScenes )
		{
			for( auto& CamItr : SceneItr.second->GetCameras() )
			{
				CamItr.second->SetAspectRatio( uiWidth, uiHeight );
			}
		}
		return RESULT::OK;
	}

}//xse