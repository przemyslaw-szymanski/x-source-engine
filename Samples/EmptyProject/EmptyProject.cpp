//
#include <XSourceEngine.h>
#include <XSECRenderArray.h>
#include <iostream>
#include <fstream>

using namespace XST::Types;

XSE::CCamera*	pCamera;
XSE::ITerrain*	pTerr;
XSE::IViewport*	pViewport;
f32 g_fCamSpeed = 1.0f;

class CListener : public XSE::IKeyboardListener, public XSE::IRenderWindowListener, public XSE::IMouseListener
{
	public:
		f32 fTheta;
		f32 fPhi;
		i32 iLOD;

		CListener()
		{
			fTheta = 0;
			fPhi = 0;
			iLOD = 1;
		}

		void		OnKeyDown(const XSE::SKeyboardEvent& Event)
		{
			if( Event.eKeyCode == XSE::KeyCodes::ESCAPE )
				Event.pWnd->Close();


			if( Event.eKeyCode == XSE::KeyCodes::ADD )
			{
				if( iLOD + 1 < pTerr->GetOptions().uiMaxLODCount )
					pTerr->SetLOD( ++iLOD );
			}

			if( Event.eKeyCode == XSE::KeyCodes::SUBTRACT )
			{
				if( iLOD - 1 >= 0 )
					pTerr->SetLOD( --iLOD );
			}

			if( Event.eKeyCode == XSE::KeyCodes::NUM_8 )
			{
				g_fCamSpeed += 0.02f;
			}

			if( Event.eKeyCode == XSE::KeyCodes::NUM_2 )
			{
				g_fCamSpeed = std::max< f32 >( 0.0f, g_fCamSpeed - 0.02f );
			}

			if( Event.eKeyCode == XSE::KeyCodes::CAPITAL_V )
			{
				XSE::FILL_MODE eMode = pViewport->GetFillMode();
				if( eMode == XSE::FillModes::WIREFRAME )
				{
					pViewport->SetFillMode( XSE::FillModes::SOLID );
				}
				else
				{
					pViewport->SetFillMode( XSE::FillModes::WIREFRAME );
				}
			}

		}

		void	OnMouseMove(const XSE::SMouseEvent& Event)
		{
			fPhi += (f32)Event.Delta.x * 0.00001f;
			fTheta += (f32)Event.Delta.y * 0.00001f;

			if( Event.uiButtonDown & XSE::MouseButtons::LEFT )
			{
				pCamera->RotateX( (f32)Event.Delta.x * 0.01f );
				pCamera->RotateY( (f32)Event.Delta.y * 0.01f );
			}

			/*std::stringstream ss;
			ss<< pCamera->GetAngles().x << " " << pCamera->GetAngles().y;
			XSE::CEngine::GetSingleton().GetRenderWindow( "MainWindow" )->SetTitle( ss.str() );*/

		}

		i32			OnMove(const XSE::SWindowEvent& Event, cu32& uiNewXPos, cu32& uiNewYPos)
		{
			return XST_FAIL;
		}

	protected:
};

void MoveCamera(XSE::CCamera* pCamera, XSE::IRenderWindow* pWnd)
{
	XSE::IKeyboard* pKeyboard = pWnd->GetKeyboard();

	if( pKeyboard->IsKeyPressed( XSE::KeyCodes::CAPITAL_W ) )
	{
		pCamera->Move( g_fCamSpeed, pCamera->GetDirection()  );
	}

	if( pKeyboard->IsKeyPressed( XSE::KeyCodes::CAPITAL_S ) )
	{
		pCamera->Move( g_fCamSpeed, -pCamera->GetDirection() );
	}
}

int xse_main()
{
	XST::CLogger::GetSingleton().SetMode( XST::CLogger::CONSOLE );
	XSE::CEngine* pEngine = XSECreateEngine();
	XSE::IRenderWindow* pWnd = pEngine->CreateRenderWindow( "MainWindow" );
	CListener* pListener = xst_new CListener();
	pWnd->GetKeyboard()->AddListener( pListener );
	pWnd->GetMouse()->AddListener( pListener );
	pWnd->AddListener( pListener );
	pWnd->GetMouse()->IsCapturedOutsideTheWindow( true );

	XSE::CEngineOptions Opts;
	Opts.RSOptions.bFullScreen = false;
	Opts.RSOptions.uiHWND = pWnd->GetHandle();
	Opts.RSOptions.uiResolutionWidth = 800;
	Opts.RSOptions.uiResolutionHeight = 600;
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


	if( XST_FAILED( pEngine->Init( Opts ) ) )
	{
		XST::Platform::MsgBoxError( pWnd->GetHandle(), "Error", XST::CLastError::Get().data() );
		XSEDestroyEngine( &pEngine );
		return 0;
	}

	pViewport = pEngine->GetRenderSystem()->GetCurrentViewport();

	/*if( XST_FAILED( pEngine->RegisterShaderSystem( XSE::ShaderLanguages::CG ) ) )
	{
		XST::Platform::MsgBoxError( pWnd->GetHandle(), "Error", XST::CLastError::Get().data() );
		XSEDestroyEngine( &pEngine );
		return 0;
	}*/

	if( XST_FAILED( pEngine->SetShaderSystem( XSE::ShaderLanguages::HLSL ) ) )
	{
		XSEDestroyEngine( &pEngine );
		return 0;
	}

	XSE::CSceneManager* pSceneMgr = pEngine->CreateSceneManager( "Test" );
	pEngine->SetSceneManager( pSceneMgr );
	pSceneMgr->GetCurrentCamera()->SetFOV( XST_HALF_PI, 0.1f, 10000.0f );
	pSceneMgr->GetCurrentCamera()->SetPosition( 300.0f, 250.0f, 300.0f );
	pSceneMgr->GetCurrentCamera()->SetLookAt( XSE::Vec3( 0, 0, 0 ) );
	pCamera = pSceneMgr->GetCurrentCamera();

	pWnd->Show( true );

	XSE::CResourceFileManager* pRFMgr = XSE::CResourceFileManager::GetSingletonPtr();
	XSE::CResourceFileManager::GetSingletonPtr()->AddLocation( "Data/Shaders", "Shaders", "FileSystem" );
	XSE::CResourceFileManager::GetSingletonPtr()->AddLocation( "Data/Images", "Images", "FileSystem" );
	if( XST_FAILED( XSE::CResourceFileManager::GetSingletonPtr()->PrepareAllGroups() ) )
	{
		XST::Platform::MsgBoxError( pWnd->GetHandle(), "Error", XST::CLastError::Get().data() );
	}

	//Create material
	i32 iResult;
	XSE::MaterialPtr pMat = XSE::CMaterialManager::GetSingletonPtr()->CreateMaterial( "Test", "Test" );
	XSE::IInputLayout* pInputLayout = pEngine->GetRenderSystem()->GetInputLayout( XSE::InputLayoutElements::POSITION | /*XSE::InputLayoutElements::NORMAL |*/ XSE::InputLayoutElements::COLOR );
	if( !pMat.IsNull() )
	{
		XSE::IPass* pPass = pMat->GetCurrentTechnique()->CreatePass( "pass0" );
		XSE::VertexShaderPtr pVS = XSE::CShaderManager::GetSingleton().LoadVertexShader( "Test.fx", "VS" );
		pVS->Compile( "VS", XSE::ShaderProfiles::VS_3_0 );
		pPass->SetVertexShader( pVS );
		XSE::PixelShaderPtr pPS = XSE::CShaderManager::GetSingleton().LoadPixelShader( "Test.fx", "PS" );
		pPS->Compile( "PS", XSE::ShaderProfiles::PS_3_0 );
		pPass->SetPixelShader( pPS );
	}
	XSE::MaterialPtr pMat2 = XSE::CMaterialManager::GetSingletonPtr()->CreateMaterial( "Test2", "Test" );
	//XSE::IInputLayout* pInputLayout = pEngine->GetRenderSystem()->GetInputLayout( XSE::InputLayoutElements::POSITION | /*XSE::InputLayoutElements::NORMAL |*/ XSE::InputLayoutElements::COLOR );
	if( !pMat2.IsNull() )
	{
		XSE::IPass* pPass = pMat2->GetCurrentTechnique()->CreatePass( "pass0" );
		XSE::VertexShaderPtr pVS = XSE::CShaderManager::GetSingleton().LoadVertexShader( "Test.fx", "VS" );
		pVS->Compile( "VS", XSE::ShaderProfiles::VS_3_0 );
		pPass->SetVertexShader( pVS );
		XSE::PixelShaderPtr pPS = XSE::CShaderManager::GetSingleton().LoadPixelShader( "Test.fx", "PS2" );
		pPS->Compile( "PS2", XSE::ShaderProfiles::PS_3_0 );
		pPass->SetPixelShader( pPS );
	}
	XSE::MaterialPtr pMat3 = XSE::CMaterialManager::GetSingletonPtr()->CreateMaterial( "Test3", "Test" );
	if( !pMat3.IsNull() )
	{
		XSE::IPass* pPass = pMat3->GetCurrentTechnique()->GetPass( 0 );
		XSE::VertexShaderPtr pVS = XSE::CShaderManager::GetSingleton().LoadVertexShader( "Test.fx", "VS_Screen" );
		pVS->Compile( "VS_Screen", XSE::ShaderProfiles::VS_3_0 );
		pPass->SetVertexShader( pVS );
		XSE::PixelShaderPtr pPS = XSE::CShaderManager::GetSingleton().LoadPixelShader( "Test.fx", "PS_Screen" );
		pPS->Compile( "PS_Screen", XSE::ShaderProfiles::PS_3_0 );
		pPass->SetPixelShader( pPS );
	}
	XSE::MaterialPtr pMat4 = XSE::CMaterialManager::GetSingletonPtr()->CreateMaterial( "Test4", "Test" );
	if( !pMat3.IsNull() )
	{
		XSE::IPass* pPass = pMat4->GetCurrentTechnique()->GetPass( 0 );
		XSE::VertexShaderPtr pVS = XSE::CShaderManager::GetSingleton().LoadVertexShader( "Test.fx", "VS_Screen" );
		pVS->Compile( "VS_Screen", XSE::ShaderProfiles::VS_3_0 );
		pPass->SetVertexShader( pVS );
		XSE::PixelShaderPtr pPS = XSE::CShaderManager::GetSingleton().LoadPixelShader( "Test.fx", "PS_Screen" );
		pPS->Compile( "PS_Screen2", XSE::ShaderProfiles::PS_3_0 );
		pPass->SetPixelShader( pPS );
	}
	XSE::MaterialPtr pMat5 = XSE::CMaterialManager::GetSingletonPtr()->CreateMaterial( "Test5", "Test" );
	if( !pMat5.IsNull() )
	{
		XSE::IPass* pPass = pMat5->GetCurrentTechnique()->GetPass( 0 );
		XSE::VertexShaderPtr pVS = XSE::CShaderManager::GetSingleton().LoadVertexShader( "Test.fx", "VS_Terrain" );
		pVS->Compile( "VS_Terrain", XSE::ShaderProfiles::VS_3_0 );
		pPass->SetVertexShader( pVS );
		XSE::PixelShaderPtr pPS = XSE::CShaderManager::GetSingleton().LoadPixelShader( "Test.fx", "PS_Terrain" );
		pPS->Compile( "PS_Terrain", XSE::ShaderProfiles::PS_3_0 );
		pPass->SetPixelShader( pPS );
	}

	//Create box
	XSE::CSceneNode* pNode = pSceneMgr->GetRootNode()->CreateChildNode( "Box" );
	
	//XSE::ModelPtr pModel = pSceneMgr->CreateModel( "Box", pNode, XSE::BasicShapes::BOX, pInputLayout, xst_null );
	XSE::ModelPtr pModel = XSE::CModelManager::GetSingletonPtr()->CreateModel( "Box", XSE::BasicShapes::BOX, pInputLayout );
	//pModel->SetMaterial( pMat );
	////pNode->AddObject( pModel );
	//pModel->SetPosition( -3, 5, 2 );

	f32 fCMTime = 0, fCNTime = 0;
	{
		xst_astring strName;
		XST::CTimer Timer;
		XST::CSimpleProfiler Prof( "m" );
	for(u32 i = 0; i < 0; ++i)
	{
		strName = XST::StringUtil::ToString( i ) + "Box";
		Timer.StartQPerf();
		XSE::ModelPtr pModel2 = XSE::CModelManager::GetSingletonPtr()->CreateModel( strName, "Box", XSE::DEFAULT_GROUP, XSE::DEFAULT_GROUP );// ( pSceneMgr, XST::StringUtil::ToString( i ) + "Box", "Box", XST::StringUtil::ToString( i ) + "Box", XSE::BasicShapes::BOX, pInputLayout );
		fCMTime += Timer.GetQPerfElapsedTime();
		Timer.StopQPerf();

		pModel2->SetMaterial( pMat );
		XSE::IVertexBuffer* pVB = pModel2->GetMesh()->GetVertexBuffer();
		XSE::IIndexBuffer* pIB = pModel2->GetMesh()->GetIndexBuffer();

		Timer.StartQPerf();
		pSceneMgr->GetRootNode()->CreateChildNode( strName )->AddObject( pModel2 );
		fCNTime += Timer.GetQPerfElapsedTime();
		Timer.StopQPerf();

		pModel2->SetPosition(XST::Random::GetNextFloatInRange( -20, 20 ), 
							XST::Random::GetNextFloatInRange( 0, 20 ), 
							XST::Random::GetNextFloatInRange( -20, 20 ));
		//pModel2->GetMesh()->IsVisible( true );
	}
	}

	XSE::CStaticGeometry* pSG = pSceneMgr->CreateStaticGeometry( "test" );
	XSE::CStaticGeometryGroup* pSGG = pSG->CreateGroup(  );
	XSE::CStaticGeometryGroup* pSGG2 = pSG->CreateGroup(  );
	for(u32 i = 0; i < 1; ++i)
	{
		pSGG->AddMesh( pModel->GetMesh(), pMat, XSE::Vec3( XST::Random::GetNextFloatInRange( -20, 20 ), 
							XST::Random::GetNextFloatInRange( 0, 20 ), 
							XST::Random::GetNextFloatInRange( -20, 20 ) ) );

		pSGG2->AddMesh( pModel->GetMesh(), pMat2, XSE::Vec3( XST::Random::GetNextFloatInRange( -40, 40 ), 
							XST::Random::GetNextFloatInRange( 0, 40 ), 
							XST::Random::GetNextFloatInRange( -40, 40 ) ) );

	}

	pSG->Build();
	XSE::CSceneNode* pSGNode = pSceneMgr->GetRootNode()->CreateChildNode( "sg" );
	//pSGNode->AddObject( pSG );
	//pSGG2->SetPosition( 20, 0, 20 );

	//Terrain
	XSE::ImagePtr pImg = XSE::CImageManager::GetSingletonPtr()->CreateResource( "heightmap01.jpg", "Images" );
	XSE::CImageManager::GetSingletonPtr()->LoadResource( pImg, "Images" );
	XSE::CImageManager::GetSingletonPtr()->PrepareImage( pImg );
	XSE::STerrainOptions TerrOptions;
	TerrOptions.bColor = true;
	TerrOptions.bTexCoord = true;
	TerrOptions.uiLODCount = 6;
	TerrOptions.bNormal = true;
	TerrOptions.bBinormal = TerrOptions.bTangent = false;
	TerrOptions.vecVertexCount = XSE::Vec2( 1025, 1025 );
	TerrOptions.vecSize = XSE::Vec2( 1000, 1000 );
	TerrOptions.vecTileVertexCount = XSE::Vec2( 33, 33 );
	/*TerrOptions.vecVertexCount = XSE::Vec2( 265, 265 );
	TerrOptions.vecSize = XSE::Vec2( 400, 400 );
	TerrOptions.vecTileVertexCount = XSE::Vec2( 33, 33 );*/
	TerrOptions.pImage = pImg.GetPointer();
	{ XST::CSimpleProfiler Prof( "terrain" );
	pTerr = pSceneMgr->CreateTerrain( "test", TerrOptions );
	}
	pTerr->SetMaterial( pMat5 );

	//pTerr->SetLOD( 3 );
	
	XSE::SPlaneOptions PlaneOpts;
	PlaneOpts.vecNormal = XSE::Vec3( 0, 1, 0 );
	PlaneOpts.vecSize = XSE::Vec2( 20, 20 );
	PlaneOpts.vecVertexCount = XSE::Vec2( 2, 2 );
	XSE::IInputLayout* pPlaneIL = pEngine->GetRenderSystem()->GetInputLayout( XSE::ILEs::POSITION | XSE::ILEs::COLOR | XSE::ILEs::TEXCOORD0 );
	XSE::ModelPtr pPlaneModel = XSE::CModelManager::GetSingleton().CreateModel( "Plane", XSE::BasicShapes::PLANE, pPlaneIL, &PlaneOpts );
	XSE::CSceneNode* pPlaneNode = pSceneMgr->GetRootNode()->CreateChildNode( "Plane" );

	//pPlaneNode->AddObject( pPlaneModel );
	//pPlaneModel->SetMaterial( pMat5 );
	//pPlaneModel->SetPosition( -10, 0, -10 );
	//for(u32 i = 0; i < pPlaneModel->GetMesh()->GetVertexBuffer()->GetVertexCount(); ++i)
	//{
	//	std::stringstream ss;
	//	XSE::Vec3 vecPos;
	//	XSE::Vec4 vecCol;
	//	XSE::Vec2 vecTC;
	//	pPlaneModel->GetMesh()->GetVertexBuffer()->GetVertexData().GetPosition( i, &vecPos );
	//	pPlaneModel->GetMesh()->GetVertexBuffer()->GetVertexData().GetTexCoord0( i, &vecTC );
	//	pPlaneModel->GetMesh()->GetVertexBuffer()->GetVertexData().GetColor( i, &vecCol );
	//	ss<< vecPos.x << " " << vecPos.z << " tc " << vecTC.x << " " << vecTC.y << " col " << vecCol.x;
	//	XST::CDebug::PrintDebugLN( ss.str() );
	//}

	pEngine->GetRenderSystem()->GetCurrentViewport()->SetBackgroundColor( XST::CColor( 0, 0, 1, 1 ) );
	XSE::IRenderSystem* pRS = pEngine->GetRenderSystem();

	XST::CTimer Timer;
	Timer.StartQPerf();
	cf32 fTimeStep = 0.03f;
	f32 fAccum = 0.0f;

	///////////////////////////////////////////////////////////////////////////////

	/*XSE::GUI::CGUIManager GuiMgr;
	GuiMgr.Init( pEngine->GetRenderSystem(), pSceneMgr, XSE::CMeshManager::GetSingletonPtr() );
	XSE::MaterialPtr aMats[] = { pMat3, pMat4 };
	for(u32 i = 0; i < 5; ++i)
	{
		XSE::GUI::CComponent* pCmp = GuiMgr.GetMainComponent()->CreateComponent();
		i32 m = XST::CRandom::GetNextIntRange( 0, 1 );
		pCmp->SetMaterial( aMats[ m ] );
		pCmp->SetPosition( XST::CRandom::GetNextIntRange( 0, 700 ), XST::CRandom::GetNextIntRange( 0, 500 ) );
	}
	XSE::GUI::CComponent* pCmp = GuiMgr.GetMainComponent()->CreateComponent();
	pCmp->SetPosition( 0, 0 );
	pCmp->SetMaterial( pMat3 );
	GuiMgr.Build( pInputLayout, "gui" );*/
	
	/////////////////////////////////////////////////////////////////////////////////

	while( !pWnd->IsClosing() )
	{
		//Get input
		fAccum += pWnd->GetFrameTime();
		//Update game
		//while( fAccum > fTimeStep )
		{
			fAccum -= fTimeStep;
		}
		f32 fa = pWnd->GetFrameTime() * 0.001f; 
		//pRS->SetRotationY( fa );

		//pCmp->SetPosition( XST::CRandom::GetNextIntRange( 0, 700 ), XST::CRandom::GetNextIntRange( 0, 500 ) );
		//GuiMgr.GetRenderer()->RenderGUI();

		pWnd->Update();
		//pEngine->GetRenderSystem()->GetCurrentViewport()->DisableZBuffer();
		pWnd->BeginRenderFrame();
		pCamera->Update( 0 );
		pWnd->EndRenderFrame();
		pWnd->SetTitle( XST::StringUtil::ToString( pEngine->GetRenderSystem()->GetDiagnostics().ulDrawIndexedCallCount ) );
		MoveCamera( pCamera, pWnd );
		//pWnd->SetTitle( XST::StringUtil::ToString( fa, 10 ) );
		//XST::Platform::Delay( 1 );
	}


	//pModel = xst_null;
	XSEDestroyEngine( &pEngine );
	xst_delete( pListener );

	return 0;
}
//#include <windows.h>
//int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR l, int cmd)
//{
//	return 0;
//}