#include "CSampleMgr.h"
#include "ui\CRenderInfo.h"
#include "IncludeSamples.h"

CSampleMgr::CSampleMgr() : m_pCurrSample( NULL )
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

	XSE::CEngineOptions Opts;
	Opts.RSOptions.bFullScreen = false;
	Opts.RSOptions.uiHWND = m_pRenderWnd->GetHandle();
	Opts.RSOptions.uiResolutionWidth = m_pRenderWnd->GetWidth();
	Opts.RSOptions.uiResolutionHeight = m_pRenderWnd->GetHeight();
	Opts.RSOptions.uiRefreshRate = 60;
	Opts.RSOptions.eMinFeatureLevel = XSE::ShaderModels::SM_3_0;
	Opts.RSOptions.eMaxFeatureLevel = XSE::ShaderModels::SM_4_0;
	Opts.strRenderSystem = XSE::CEngine::BEST_RENDER_SYSTEM;
	Opts.RSOptions.bAntialiasing = true;
	Opts.RSOptions.bMultisampling = true;
	Opts.RSOptions.eFillMode = XSE::FillModes::SOLID;
	Opts.RSOptions.bVSync = false;
#if defined (XST_DEBUG)
	Opts.RSOptions.ulShaderFlags |= XSE::ShaderFlags::DEBUG;
	Opts.RSOptions.bDebugMode = true;
#else
	Opts.RSOptions.ulShaderFlags |= XSE::ShaderFlags::OPTIMIZED;
#endif
	Opts.RSOptions.ulShaderFlags |= XSE::ShaderFlags::BACKWARD_COMPAPILITY;

	if( XST_FAILED( m_pEngine->Init( Opts ) ) )
	{
		XST::Platform::MsgBoxError( m_pRenderWnd->GetHandle(), "Error", XST::CLastError::Get().data() );
		XSEDestroyEngine( &m_pEngine );
		return 0;
	}

	//Init resources
	XSE::CResourceFileManager* pFileMgr = XSE::CResourceFileManager::GetSingletonPtr();
	pFileMgr->AddLocation( "Data", "Sample", "FileSystem", true );
	if( XST_FAILED( pFileMgr->PrepareAllGroups() ) )
	{
		return XST_FAIL;
	}

	return XST_OK;
}

void CSampleMgr::DestroyEngine()
{
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
	RegisterSample( &CreateCCustomGeometry, &DestroyCCustomGeometry );
	RegisterSample( &CreateCCustomGeometryLOD, &DestroyCCustomGeometryLOD );
	RegisterSample( &CreateCTerrain, &DestroyCTerrain );
}

void CSampleMgr::UnregisterSample(ISample* pSample)
{
	if( m_pCurrSample == NULL )
	{
		return;
	}

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
	if( m_pCurrSample != xst_null )
	{
		m_pRenderWnd->BeginRenderFrame();
		m_pRenderWnd->EndRenderFrame();
		//bool b = m_pRenderWnd->GetKeyboard()->IsKeyPressed( XSE::KeyCodes::CAPITAL_W );
		m_pCurrSample->Update();
	}
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
	if( m_pCurrSample->Init( m_pEngine, m_pRenderWnd ) != XST_OK )
	{
		return false;
	}
	
	m_pRenderWnd->GetKeyboard()->AddListener( m_pCurrSample );
	m_pRenderWnd->GetMouse()->AddListener( m_pCurrSample );
	m_pRenderWnd->Show( true );
	return m_pCurrSample->Run() == XST_OK;
}

void CSampleMgr::RenderWireframe()
{
	m_pEngine->GetRenderSystem()->GetCurrentViewport()->SetFillMode( XSE::FillModes::WIREFRAME );
}

void CSampleMgr::RenderSolid()
{
	m_pEngine->GetRenderSystem()->GetCurrentViewport()->SetFillMode( XSE::FillModes::SOLID );
}

void CSampleMgr::ResetCamera()
{
}