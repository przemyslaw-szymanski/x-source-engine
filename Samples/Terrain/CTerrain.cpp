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
		sprintf( buff, "xse_cloned_mesh_000000000%d", i );
		str.assign( buff );
		//XSE::Resources::CMesh* pMesh = xst_new XSE::Resources::CMesh( 0, 0, 0, 0, "" );
		XSE::MeshPtr pMesh = pMgr->CreateMesh( str );
	}
	f32 fTime = Timer.GetQPerfElapsedTime();

	this->CreateSceneManager( pEngine, 1250 );
	//this->m_pSceneMgr->GetCurrentCamera()->SetPosition( 0, 5, -10 );
	//this->m_pSceneMgr->GetCurrentCamera()->SetMoveSpeed( 1 );
	this->m_pViewCam->SetAngleX( 0.397f );
	this->m_pViewCam->SetAngleY( 0.580f );
	this->m_pViewCam->SetPosition( -18, 101, -56 );
	this->m_pViewCam->SetSpeed( 40, 1.1f, 1.1f );
	this->m_pViewCam->SetFar( 1000 );

	//this->m_pDbgCam->SetPosition( 0, 100, 0 );
	this->m_pDbgCam->SetPosition( this->m_pViewCam->GetPosition() );
	this->m_pDbgCam->SetAngleX( this->m_pViewCam->GetAngles().x );
	this->m_pDbgCam->SetAngleY( this->m_pViewCam->GetAngles().y );
	this->m_pDbgCam->SetFOV( XST::DegreesToRadians( 45 ), 0.1f, 1000.0f );
	this->m_pDbgCam->ShowFrustumMesh( true );

	this->EnableDbgCamera( false );

	//this->m_pSceneMgr->SetViewFrustumCullType( XSE::ViewFrustumCullTypes::NONE );

	//this->m_pMoveCam = this->m_pDbgCam;

	//this->m_pSceneMgr->SetCamera( this->m_pViewCam );

	XSE::CSceneManager::CameraIterator Itr = m_pSceneMgr->GetCameraIterator();
	while( Itr.HasMoreElements() )
	{
		bool b = Itr.GetValue().second->IsFrustumMeshVisible();
		Itr.MoveNext();
	}

	XSE::STerrainOptions Options;
	Options.Size = XSE::CPoint( 1000, 1000 );
	u32 uiCount = 11;
	Options.PageCount = XSE::CPoint( 1, 1 );
	Options.vHeightmaps.push_back( "heightmap04.jpg" );
	Options.PageVertexCount = XSE::CPoint( 32 * 6 + 1 );
	Options.TileVertexCount = XSE::CPoint( 16 + 1 );
	//Options.PageVertexCount = XSE::CPoint( 32 * 32 + 1 );
	//Options.TileVertexCount = XSE::CPoint( 16 + 16 + 1 );
	Options.uiLODCount = 2;
	Options.bColor = true;
	Options.bBinormal = Options.bNormal = Options.bTangent = Options.bTexCoord = false;

	{
		XSTSimpleProfiler2( "CreateTerrain");
	XSE::ITerrain* pTerrain = this->m_pSceneMgr->CreateTerrain( "Terrain", Options );
	}
	return XSE::RESULT::OK;
}

void CTerrain::OnUpdate()
{
	XSE::IKeyboard* pKeyboard = m_pRenderWindow->GetKeyboard( );
	if( pKeyboard->IsKeyPressed( XSE::KeyCodes::CAPITAL_Z ) )
	{
		this->m_pDbgCam->RotateX( -0.1 );
	}
	else if( pKeyboard->IsKeyPressed( XSE::KeyCodes::CAPITAL_X ) )
	{
		this->m_pDbgCam->RotateX( 0.1 );
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