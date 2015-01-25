#include "CSimpleSample.h"

i32 CSimpleSample::Init(XSE::CEngine* pEngine, XSE::IRenderWindow* pWnd)
{
	m_pWnd = pWnd;
	m_pEngine = pEngine;

	XSE::CSceneManager* pSceneMgr = pEngine->CreateSceneManager( this->GetName(), 0, true );
	pSceneMgr->GetCurrentCamera()->SetFOV( XST_HALF_PI, 0.1f, 1000.0f );
	pSceneMgr->GetCurrentCamera()->SetPosition( 00.0f, 00.0f, -10.0f );
	pSceneMgr->GetCurrentCamera()->SetLookAt( XSE::Vec3( 0, 0, 0 ) );
	XSE::SBoxOptions Options;
	Options.vecSize = XST::Vec2( 10, 10 );
	XSE::MeshPtr pMesh = pSceneMgr->GetMeshManager()->CreateMesh( "boxMesh", XSE::ILE::POSITION | XSE::ILE::COLOR, XSE::BasicShapes::BOX, &Options );
	XSE::ModelPtr pModel = pSceneMgr->CreateModel( "boxModel", "boxNode" );
	pModel->AddMesh( pMesh );
	return XST_OK;
}

i32 CSimpleSample::Run()
{
	return XST_OK;
}

void CSimpleSample::Destroy()
{
	m_pEngine->DestroySceneManager( this->GetName() );
}