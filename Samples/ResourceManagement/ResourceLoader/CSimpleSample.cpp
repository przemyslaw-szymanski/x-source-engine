#include "CSimpleSample.h"

i32 CSimpleSample::Init(XSE::CEngine* pEngine, XSE::IRenderWindow* pWnd)
{
	m_pWnd = pWnd;
	m_pEngine = pEngine;

	XSE::CSceneManager* pSceneMgr = pEngine->CreateSceneManager( this->GetName() );
	pEngine->SetSceneManager( pSceneMgr );
	pSceneMgr->GetCurrentCamera()->SetFOV( XST_HALF_PI, 0.1f, 1000.0f );
	pSceneMgr->GetCurrentCamera()->SetPosition( 300.0f, 250.0f, 300.0f );
	pSceneMgr->GetCurrentCamera()->SetLookAt( XSE::Vec3( 0, 0, 0 ) );
	return XST_OK;
}

i32 CSimpleSample::Run()
{
	while( !m_pWnd->IsClosing() )
	{
		m_pWnd->RenderFrame();
	}
	
	return XST_OK;
}

void CSimpleSample::Destroy()
{
	m_pEngine->DestroySceneManager( this->GetName() );
}