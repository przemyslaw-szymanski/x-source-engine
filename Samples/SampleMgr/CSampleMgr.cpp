#include "CSampleMgr.h"
#include "IncludeSamples.h"
#include "CRenderThread.h"

CRenderThread g_RenderThread;

CSampleMgr::CSampleMgr()
{
}

CSampleMgr::~CSampleMgr()
{
	UnregisterSamples();
}

void CSampleMgr::UnregisterSamples()
{
	SampleMap::iterator Itr;
	for(Itr = m_mSamples.begin(); Itr != m_mSamples.end(); ++Itr)
	{
		UnregisterSample( Itr->second.pSample );
		if( Itr->second.pSample->AutoDestroy() )
		{
			Itr->second.DestroySample( Itr->second.pSample );
		}
	}

	m_mSamples.clear();
}

i32 CSampleMgr::InitEngine(u32 uiWindowHandle)
{
	m_pEngine = XSECreateEngine();
	m_pRenderWnd = m_pEngine->CreateRenderWindow( "MainWindow", uiWindowHandle );

	XSE::SEngineSettings Opts;
	Opts.RSSettings.bFullScreen = false;
	Opts.RSSettings.uiHWND = m_pRenderWnd->GetHandle();
	Opts.RSSettings.uiResolutionWidth = m_pRenderWnd->GetWidth();
	Opts.RSSettings.uiResolutionHeight = m_pRenderWnd->GetHeight();
	Opts.RSSettings.uiRefreshRate = 60;
	Opts.RSSettings.eMinFeatureLevel = XSE::ShaderModels::SM_3_0;
	Opts.RSSettings.eMaxFeatureLevel = XSE::ShaderModels::SM_4_0;
	Opts.strRenderSystem = XSE::CEngine::BEST_RENDER_SYSTEM;
	Opts.RSSettings.bAntialiasing = true;
	Opts.RSSettings.bMultisampling = true;
	Opts.RSSettings.eFillMode = XSE::FillModes::SOLID;
	Opts.RSSettings.bVSync = false;
#if defined (XST_DEBUG)
	Opts.RSSettings.ulShaderFlags |= XSE::ShaderFlags::DEBUG;
	Opts.RSSettings.bDebugMode = true;
#else
	Opts.RSSettings.ulShaderFlags |= XSE::ShaderFlags::OPTIMIZED;
#endif
	Opts.RSSettings.ulShaderFlags |= XSE::ShaderFlags::BACKWARD_COMPAPILITY;

	if( XST_FAILED( m_pEngine->Init( Opts ) ) )
	{
		XST::Platform::MsgBoxError( m_pRenderWnd->GetHandle(), "Error", XST::CLastError::Get().data() );
		XSEDestroyEngine( &m_pEngine );
		return 0;
	}

	//Init resources
	XSE::CResourceFileManager* pFileMgr = XSE::CResourceFileManager::GetSingletonPtr();
    if( XST_FAILED( pFileMgr->AddLocation( "Data", "Sample", "FileSystem", true ) ) )
    {
        return XST_FAIL;
    }
	if( XST_FAILED( pFileMgr->PrepareAllGroups() ) )
	{
		return XST_FAIL;
	}

	g_RenderThread.Init( m_pEngine );
	g_RenderThread.start();

	return XST_OK;
}

void CSampleMgr::DestroyEngine()
{
	g_RenderThread.StartRendering( false );
	g_RenderThread.terminate();
	XSEDestroyEngine( &m_pEngine );
}

bool CSampleMgr::RegisterSample(pfnCreateSample Create, pfnDestroySample Destroy)
{
	//Find sample
	ISample* pSample = Create();
	if( pSample == NULL )
	{
		return false;
	}

	if( GetSample( pSample->GetName() ) != NULL )
	{
		Destroy( pSample );
	}

	SSample Sample = { pSample, Create, Destroy };
	m_mSamples.insert( SampleMap::value_type( pSample->GetName(), Sample ) );

	return true;
}

void CSampleMgr::RegisterSamples()
{
	RegisterSample( &CreateCSimpleSample, &DestroyCSimpleSample );
	//RegisterSample( &CreateCCustomGeometry, &DestroyCCustomGeometry );
	//RegisterSample( &CreateCCustomGeometryLOD, &DestroyCCustomGeometryLOD );
	RegisterSample( &CreateCTerrain, &DestroyCTerrain );
}

void CSampleMgr::UnregisterSample(ISample* pSample)
{
	if( m_pCurrSample == NULL )
	{
		return;
	}

	g_RenderThread.StartRendering( false );
	m_pRenderWnd->GetKeyboard()->RemoveListener( m_pCurrSample, false );
	m_pRenderWnd->GetMouse()->RemoveListener( m_pCurrSample, false );
	m_pCurrSample->Destroy();
	m_pCurrSample = xst_null;
}

ISample* CSampleMgr::GetSample(xst_castring& strName)
{
	SampleMap::iterator Itr = m_mSamples.find( strName );
	if( Itr == m_mSamples.end() )
	{
		return NULL;
	}

	return Itr->second.pSample;
}

void CSampleMgr::RenderSample()
{
	//g_RenderThread.Lock();
	if( m_pCurrSample != xst_null/* && m_pCurrSample->IsReady()*/ )
	{
		//g_RenderThread.SendMessage( CRenderThread::MESSAGE::SET_RENDER_ONE_FRAME );
		//m_pRenderWnd->BeginRenderFrame();
		//m_pRenderWnd->EndRenderFrame();
		//bool b = m_pRenderWnd->GetKeyboard()->IsKeyPressed( XSE::KeyCodes::CAPITAL_W );
		//m_pCurrSample->Update();
	}
	//g_RenderThread.Unlock();
}

bool CSampleMgr::RunSample(xst_castring& strName)
{
	if( m_pCurrSample != xst_null && m_pCurrSample->GetName() == strName )
	{
		return true;
	}

	m_pRenderWnd->Show( true );

	UnregisterSample( m_pCurrSample );

	//
	ISample* pSample = GetSample( strName );
	if( pSample == NULL )
	{
		return false;
	}

	m_pCurrSample = pSample;
    m_pCurrSample->SetReady( false );
	if( m_pCurrSample->Init( m_pEngine, m_pRenderWnd ) != XST_OK )
	{
		return false;
	}
	
	m_pRenderWnd->GetKeyboard()->AddListener( m_pCurrSample );
	m_pRenderWnd->GetMouse()->AddListener( m_pCurrSample );
	m_pRenderWnd->Show( true );
    m_pCurrSample->SetReady( true );

	g_RenderThread.SetSample( m_pCurrSample );
	g_RenderThread.StartRendering( true );
	
	return m_pCurrSample->Run() == XST_OK;
}

void CSampleMgr::RenderWireframe()
{
	//m_pEngine->GetRenderSystem()->GetCurrentViewport()->SetFillMode( XSE::FillModes::WIREFRAME );
	g_RenderThread.SendMessage( CRenderThread::MESSAGE::SET_RENDER_TYPE_WIREFRAME );
}

void CSampleMgr::RenderSolid()
{
	//m_pEngine->GetRenderSystem()->GetCurrentViewport()->SetFillMode( XSE::FillModes::SOLID );
	g_RenderThread.SendMessage( CRenderThread::MESSAGE::SET_RENDER_TYPE_SOLID );
}

void CSampleMgr::ResetCamera()
{
}

void CSampleMgr::SetFrustumCullType(XSE::VIEW_FRUSTUM_CULL_TEST_TYPE eType)
{
	if( m_pEngine && m_pEngine->GetSceneManager() )
		m_pEngine->GetSceneManager()->SetViewFrustumCullType( eType );
}