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

	this->CreateSceneManager( pEngine, 500 );
	//this->m_pSceneMgr->GetCurrentCamera()->SetPosition( 0, 5, -10 );
	//this->m_pSceneMgr->GetCurrentCamera()->SetMoveSpeed( 1 );
	this->m_pViewCam->SetAngleX( 0.397f );
	this->m_pViewCam->SetAngleY( 0.580f );
	this->m_pViewCam->SetPosition( -18, 101, -56 );

	this->m_pDbgCam->SetPosition( 0, 0, 0 );
	this->m_pDbgCam->SetAngleX( 45 );
	this->m_pDbgCam->SetFar( 100 );

	this->m_pDbgCam->ShowFrustumMesh( false );
	//this->m_pMoveCam = this->m_pDbgCam;
	this->m_pMoveCam->SetMoveSpeed( 400 );
	this->m_pSceneMgr->SetCamera( this->m_pViewCam );

	XSE::CSceneManager::CameraIterator Itr = m_pSceneMgr->GetCameraIterator();
	while( Itr.HasMoreElements() )
	{
		bool b = Itr.GetValue().second->IsFrustumMeshVisible();
		Itr.MoveNext();
	}

	XSE::STerrainOptions Options;
	Options.Size = XSE::CPoint( 100, 100 );
	u32 uiCount = 11;
	Options.PageCount = XSE::CPoint( 1, 1 );
	Options.vHeightmaps.push_back( "heightmap04.jpg" );
	Options.PageVertexCount = XSE::CPoint( 1025 );
	Options.TileVertexCount = XSE::CPoint( 33 );
	Options.uiLODCount = 2;
	Options.bColor = true;
	Options.bBinormal = Options.bNormal = Options.bTangent = Options.bTexCoord = false;

	{
		XSTSimpleProfiler2( "CreateTerrain");
	XSE::ITerrain* pTerrain = this->m_pSceneMgr->CreateTerrain( "Terrain", Options );
	}
	return XST_OK;
}

i32 CTerrain::Run()
{
	return XST_OK;
}

void CTerrain::Destroy()
{
	m_pEngine->DestroySceneManager( this->GetName() );
}