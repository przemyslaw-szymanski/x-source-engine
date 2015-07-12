#define XST_ENABLE_PROFILER 1
#include "CTerrain.h"
#include <QtCore\qdebug.h>

template<class _T_, u32 _COUNT_>
xst_fi xst_ostream& operator<<(xst_ostream& o, const XST::TCConstantArray< _T_, _COUNT_ >& Right)
{
	o << "( ";
	for(int i = 0; i < Right.size(); ++i)
	{
		o << Right[ i ] << " ";
	}
	o << ")";
	return o;
}

class CToString
{
	public:

		//static xst_castring SEPARATOR;
		static const ch8 SEPARATOR = ' ';

	public:

							CToString() {}
							
							template<class _T_>
							CToString(const _T_* pArray, cu32& uiElemCount)
							{
								Array( pArray, uiElemCount );
							}

		virtual				~CToString() {}

		template<class _T_>
		xst_fi	CToString&	operator<<(const _T_& right)
		{
			m_ssStr << right << SEPARATOR;
			return *this;
		}

		xst_fi xst_astring	operator<<(const CToString& ToStr)
		{
			return ToStr.BuildString();
		}

		//template<class _T_, u32 _ELEM_COUNT_>
		//xst_fi	CToString&	operator<<(const XST::TCConstantArray< _T_, _ELEM_COUNT_ >& Right)
		//{
		//	return Array( Right.data(), Right.size() );
		//}

		xst_fi	void		Clear()
							{ m_ssStr.str( xst_astring( "" ) ); }

		xst_fi	xst_astring	BuildString() const
							{ return m_ssStr.str(); }

		xst_fi	operator	xst_astring() const
							{ return m_ssStr.str(); }	

		template<class _T_>
		xst_fi	CToString&	operator()(const _T_* pArray, cu32& uiElemCount)
							{
								return Array( pArray, uiElemCount );
							}

		template<class _T_>
		xst_fi	CToString&	Array(const _T_* pArray, u32 uiElemCount)
		{
			m_ssStr << "( ";
			for(int i = 0; i < uiElemCount; ++i)
			{
				m_ssStr << pArray[ i ] << SEPARATOR;
			}
			m_ssStr << ")";
			return *this;
		}
		

	protected:

		xst_stringstream	m_ssStr;
};

//xst_castring CToString::SEPARATOR( " " );

xst_fi CToString ToString() { return CToString(); }

#define DEBUG_CAMERA 1
XSE::CLight* pLight;

i32 CTerrain::Init(XSE::CEngine* pEngine, XSE::IRenderWindow* pWnd)
{
	m_pWnd = pWnd;
	m_pEngine = pEngine;

	XST::CTimer Timer;
	Timer.StartQPerf();
	ch8 buff[ 64 ];
	xst_astring str; str.reserve( 128 );
	XSE::CMeshManager* pMgr = XSE::CMeshManager::GetSingletonPtr();
	for(u32 i = 0; i < 000; ++i)
	{
		//XSTSimpleProfiler();
        xst_sprintf( buff, sizeof( buff ), "xse_cloned_mesh_000000000%d", i );
		str.assign( buff );
		//XSE::Resources::CMesh* pMesh = xst_new XSE::Resources::CMesh( 0, 0, 0, 0, "" );
		XSE::MeshPtr pMesh = pMgr->CreateMesh( str );
	}
	f32 fTime = Timer.GetQPerfElapsedTime();

	this->CreateSceneManager( pEngine, 1250 );
    this->m_pSceneMgr->GetRenderSystem( )->GetCurrentViewport()->SetFillMode( XSE::FillModes::WIREFRAME );
	this->m_pViewCam->SetAngleX( 0.397f );
	this->m_pViewCam->SetAngleY( 0.580f );
	this->m_pViewCam->SetPosition( 32, 100, 24 );
	//this->m_pViewCam->SetPosition( 0, 0, 0 );
	this->m_pViewCam->SetSpeed( 2000, 1.1f, 1.1f );
	this->m_pViewCam->SetFar( 1000 );

	// DEBUG CAMERA
#if DEBUG_CAMERA
	this->m_pDbgCam->SetPosition( this->m_pViewCam->GetPosition() );
	this->m_pDbgCam->SetAngleX( this->m_pViewCam->GetAngles().x );
	this->m_pDbgCam->SetAngleY( this->m_pViewCam->GetAngles().y );
	this->m_pDbgCam->SetFOV( XST::DegreesToRadians( 45 ), 0.1f, 1000.0f );
	this->m_pDbgCam->ShowFrustumMesh( true );
	this->EnableDbgCamera( true );
	this->m_pViewCam->SetPosition( -389, 450, 230 );
	this->m_pViewCam->SetAngleX( 1.879f );
	this->m_pViewCam->SetAngleY( 0.701f );
    this->m_pDbgCam->SetSpeed(10000, 100, 100);
#endif
	/////////////////////////////////////////

	XSE::CSceneManager::CameraIterator Itr = m_pSceneMgr->GetCameraIterator();
	while( Itr.HasMoreElements() )
	{
		bool b = Itr.GetValue().second->IsFrustumMeshVisible();
		Itr.MoveNext();
	}

	XSE::STerrainOptions Options;
	Options.Size = XSE::CPoint( 1000 );
	u32 uiCount = 11;
	Options.PageCount = XSE::CPoint( 1, 1 );
	Options.vHeightmaps.push_back( "heightmap04.jpg" );
	//Options.PageVertexCount = XSE::CPoint( 32 * 32 + 1 );
	//Options.TileVertexCount = XSE::CPoint( 32 * 1 + 1 );
	Options.PageVertexCount = XSE::CPoint( 32 * 6 + 1 );
	Options.TileVertexCount = XSE::CPoint( 16 + 1 );
	//Options.PageVertexCount = XSE::CPoint( 32 + 1 );
	//Options.TileVertexCount = XSE::CPoint( 32 + 1 );
	Options.uiLODCount = 4;
	//Options.bColor = true;
	Options.bBinormal = Options.bNormal = Options.bTangent = false;
	Options.bNormal = true;
	Options.bColor = false;
	Options.bTexCoord = true;
    Options.bEachTileHasOwnTexCoords = false;

	XSE::MaterialWeakPtr pMat;
	{
		XSTSimpleProfiler2( "CreateTerrain");
		pMat = XSE::CMaterialManager::GetSingletonPtr()->CreateMaterial( "terrain" );
        XSE::VertexShaderPtr pVS;
        XSE::PixelShaderPtr pPS;
	pVS = XSE::CShaderManager::GetSingletonPtr()->LoadVertexShader("terrain.shader", "terr.vs", "vs", XSE::ShaderProfiles::VS_BEST);
	xst_assert2(pVS.IsValid());
	pPS = XSE::CShaderManager::GetSingletonPtr()->LoadPixelShader("terrain.shader", "terr.ps", "ps", XSE::ShaderProfiles::PS_BEST);
    /*XSE::CShaderManager::GetSingletonPtr()->LoadPixelShaders("terrain.shader", "terrain", "vs", "ps", 
                                                             XSE::ShaderProfiles::VS_BEST, XSE::ShaderProfiles::PS_BEST,
                                                             &pVS, &pPS);*/
	pMat->CreateTechnique("tech", pVS, pPS);
	pMat->SetTexture( XSE::MaterialTextureTypes::DIFFUSE, "terrtex01.jpg", XSE::ALL_GROUPS );
	pMat->SetTextureSamplingMode( XSE::MaterialTextureTypes::DIFFUSE, XSE::STextureSamplingMode() );
	
	
	xst_assert2(pVS.IsValid() && pPS.IsValid());
	XSE::ITerrain* pTerrain = this->m_pSceneMgr->CreateTerrain( "Terrain", Options );
    xst_assert( pTerrain != xst_null, "(CTerrain::Init) Terrain sample creation failed. Engine error!!!" );
	pTerrain->SetMaterial( pMat );
	}

    XSE::Vec3 vecPos =m_pDbgCam->GetPosition() + XSE::Vec3(0,500,0);
	XSE::ModelPtr pSphere = XSE::CModelManager::GetSingleton().LoadResource("untitled.obj", XSE::ALL_GROUPS);
	if( pSphere.IsValid() )
	{
		pSphere->SetMaterial( pMat );
		//pSphere->SetPosition( m_pDbgCam->GetPosition() + XSE::Vec3(0,500,0) );
		m_pSceneMgr->GetRootNode()->CreateChildNode()->AddObject( pSphere, false );
		pSphere->GetSceneNode()->SetPosition( m_pDbgCam->GetPosition() );
	}

	pLight = m_pSceneMgr->CreateLight("terrain");
	pLight->SetColor( XSE::Vec4( 0.3f, 0.6f, 0.9f, 1.0f ) );
	pLight->SetPosition( 1000,500,0 );
	//pSphere->GetSceneNode()->AddObject( pLight );
	m_pSceneMgr->SetLight( pLight );

	return XSE::RESULT::OK;
}

void CTerrain::OnUpdate()
{
	XSE::IKeyboard* pKeyboard = m_pRenderWindow->GetKeyboard( );
	if( pKeyboard->IsKeyPressed( XSE::KeyCodes::CAPITAL_Z ) )
	{
		this->m_pDbgCam->RotateX( -0.1f );
	}
	else if( pKeyboard->IsKeyPressed( XSE::KeyCodes::CAPITAL_X ) )
	{
		this->m_pDbgCam->RotateX( 0.1f );
	}
	else if( pKeyboard->IsKeyPressed( XSE::KeyCodes::CAPITAL_R ) )
	{
		this->m_pDbgCam->Move( 0.5f, this->m_pDbgCam->GetDirection() );
	}
	else if( pKeyboard->IsKeyPressed( XSE::KeyCodes::CAPITAL_F ) )
	{
		this->m_pDbgCam->Move( -0.5f, this->m_pDbgCam->GetDirection() );
	}
	this->m_pDbgCam->Update(0);

	if (pKeyboard->IsKeyPressed(XSE::KeyCodes::UP))
	{
		pLight->MoveObject(XSE::Vec3(0,100,0));
	}
	else if (pKeyboard->IsKeyPressed(XSE::KeyCodes::DOWN))
	{
		pLight->MoveObject(XSE::Vec3(0, -100, 0));
	}
	else if (pKeyboard->IsKeyPressed(XSE::KeyCodes::LEFT))
	{
		pLight->MoveObject(XSE::Vec3(-100, 0, 0));
	}
	else if (pKeyboard->IsKeyPressed(XSE::KeyCodes::RIGHT))
	{
		pLight->MoveObject(XSE::Vec3(100, 0, 0));
	}

	if (pKeyboard->IsKeyPressed(XSE::KeyCodes::CAPITAL_N))
	{
		XSE::MeshWeakPtr pNormals = XSE::CMeshManager::GetSingletonPtr()->GetResource("tmp_normal_mesh");
		if (pNormals.IsValid())
		{
			pNormals->SetVisible(!pNormals->IsVisible());
		}
	}

	if (pKeyboard->IsKeyPressed(XSE::KeyCodes::CAPITAL_C))
	{
		this->m_pViewCam->SetPosition(this->m_pDbgCam->GetPosition());
		this->m_pViewCam->SetAngleX(this->m_pDbgCam->GetAngles().x);
		this->m_pViewCam->SetAngleY(this->m_pDbgCam->GetAngles().y);
	}
}

void CTerrain::OnKeyPressEvent( int iKey )
{

}

i32 CTerrain::Run()
{
	return XST_OK;
}

void CTerrain::Destroy()
{
	m_pEngine->DestroySceneManager( this->GetName() );
}