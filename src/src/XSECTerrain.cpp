#include "XSECTerrain.h"
#include "XSEIVertexBuffer.h"
#include "XSEIIndexBuffer.h"
#include "XSEIRenderSystem.h"
#include "XSECCamera.h"
#include "XSECSceneManager.h"

namespace XSE
{
	CTerrainTile::CTerrainTile() :
		m_uiLOD( 1 ),
		m_pCurrentIndexBuffer( xst_null ),
		m_pVBuffer( xst_null ),
		m_vecTranslation( Vec3::ZERO ),
		m_vecSize( Vec2::ZERO ),
		m_vecVertexCount( Vec2::ZERO ),
		m_bReady( false ),
		m_pVBNormals( xst_null ),
		m_pNormalVS( xst_null ),
		m_pNormalPS( xst_null ),
		m_bDestroyIndexBuffers( false )
	{
	}

	CTerrainTile::~CTerrainTile()
	{
		xst_release( m_pVBuffer );
		xst_release( m_pVBNormals );
		IndexBufferVector::iterator Itr;
		if( m_bDestroyIndexBuffers )
		{
			xst_stl_foreach( Itr, m_vIndexBuffers )
			{
				xst_release( (*Itr) );
			}
		}

		m_vIndexBuffers.clear();
	}

	i32	CTerrainTile::Create(IVertexBuffer* pVB, const Vec3& vecTranslate, const Vec2& vecSize, const Vec2& vecVertexCount, bool bCreateIndexBuffers)
	{
		xst_assert2( pVB != xst_null );

		m_pVBuffer = pVB;

		//Calculate bounding sphere and box
		Vec3 vecSize3( vecSize.x, 0.0f, vecSize.y );
		m_Sphere.SetCenterPoint( vecTranslate + vecSize3 * 0.5f );
		//vecTranslate is a position of the left upper vertex - the first in the vertex buffer
		m_Sphere.SetRadius( m_Sphere.GetCenterPoint().Distance( vecTranslate ) );
		//Calc min and max of the x,y,z 
		m_AABB.Create( vecSize3, m_Sphere.GetCenterPoint() );

		ul32 ulVertexCount = m_pVBuffer->GetVertexCount();

		m_vecTranslation = vecTranslate;
		m_vecSize = vecSize;
		m_vecVertexCount = vecVertexCount;
		Vec2 vecTexelSize( 1.0f / 257, 1.0f / 257 ); //total vertex count x,y

		CVertexData& VData = m_pVBuffer->GetVertexData();

		Vec2 vecDistance( vecSize / vecVertexCount );
		Vec3 vecPos( Vec3::ZERO );
		Vec3 vecTmp;
		Vec2 vecTmp2;
		ul32 ulVertexId = 0;

		//std::stringstream ss;
		//ss << "tile_" << (u32)m_vecTranslation.x << "_" << (u32)m_vecTranslation.z << ".xls";
		//FILE* pFile = fopen( ss.str().data(), "w" );

		for(ul32 y = 0; y < vecVertexCount.y; ++y, vecPos.z += vecDistance.y)
		{
			for(ul32 x = 0; x < vecVertexCount.x; ++x, vecPos.x += vecDistance.x)
			{
				vecTmp = vecPos + vecTranslate;
				VData.SetPosition( ulVertexId, vecTmp );
				//vecTmp2 = Vec2( vecTmp.x, vecTmp.z ) / 514;
				//VData.SetTexCoord( ulVertexId, 0, vecTmp2 );
				++ulVertexId;
				//if( pFile ) fprintf( pFile, "(%.2f,%.2f)\t", vecTmp.x, vecTmp.z );
			}
			//if( pFile ) fprintf( pFile, "\n" );
			vecPos.x = 0.0f;
		}

		//if( pFile ) fclose( pFile );
		Vec4 vecCol = ( XST::CColor::Random().ToVector4() ) ;
		XST::CRandom Rand;
		vecCol.x = Rand.GetNextFloatInRange( 0.2f, 1 );
		vecCol.y = Rand.GetNextFloatInRange( 0.2f, 1 );
		vecCol.z = Rand.GetNextFloatInRange( 0.2f, 1 );
		vecCol.w = 1;

		if( pVB->GetInputLayout()->IsColor() )
		{
			for(ul32 i = VData.GetVertexCount(); i -->0;)
			{
				VData.SetColor( i, vecCol );
			}
		}

		if( bCreateIndexBuffers )
		{
		}

		/*for(ul32 i = 0; i < VData.GetVertexCount(); ++i)
		{
			std::stringstream ss;
			Vec3 vecPos;
			VData.GetPosition( i , &vecPos );
			ss<< vecPos.x << " " << vecPos.z;
			XST::CDebug::PrintDebugLN( ss.str() );
		}*/

		m_bReady = true;
		return XST_OK;
	}

	i32	CTerrainTile::Create(const CPoint& TilePart, const CPoint& TileSize, const CPoint& TerrainVertexCount, const CPoint& TileVertexCount)
	{
		//In this method create only the index buffers

		for(u32 i = 0; i < m_vIndexBuffers.size(); ++i)
		{
			IIndexBuffer* pIB = m_vIndexBuffers[ i ];
			CIndexData& IData = pIB->GetIndexData();

			if( !pIB->IsTriangleStripDegenerationSupported() )
			{
				XST_LOG_ERR( "No triangle strip degeneration supported." );
				return XST_FAIL;
			}

			//Horizontal and vertical vertex count for this tile
			ul32 ulHVertCount = TerrainVertexCount.x;
			ul32 ulVVertCount = TerrainVertexCount.y;
			u32 uiLOD = i + 1; //current lod
			ul32 ulIndex = 0; //current index in the loop
			CPoint Offset( TilePart ); //inde offset for this tile
			CPoint BeginEndVVertex( TilePart.y * TileVertexCount.y, TilePart.y * TileVertexCount.y + TileVertexCount.y );
			CPoint BeginEndHVertex( TilePart.x * TileVertexCount.x, TilePart.x * TileVertexCount.x + TileVertexCount.x );

			if( TilePart.y > 0 )
			{
				BeginEndVVertex = CPoint( TilePart.y * ( TileVertexCount.y - 1 ), TilePart.y * ( TileVertexCount.y - 1 ) + TileVertexCount.y );
			}

			if( TilePart.x > 0 )
			{
				BeginEndHVertex = CPoint( TilePart.x * ( TileVertexCount.x - 1), TilePart.x * ( TileVertexCount.x - 1 ) + TileVertexCount.x );
			}
FILE *pFile = fopen( "indeksy.xls", "w" );
			//Degenerate strip
			//for(ul32 z = 0, z1 = 0; z < ulVVertCount - 1; ++z1, z += uiLOD)
			for(ul32 z = BeginEndVVertex.x; z < BeginEndVVertex.y - 1; z += uiLOD)
			{
				i32 x;
				//for(x = 0; x < ulHVertCount; x += uiLOD )
				for(x = BeginEndHVertex.x; x < BeginEndHVertex.y; x += uiLOD)
				{
				//ret.push_back(WORD(x + m_x* z ));
				//ret.push_back(WORD(x + m_x*(z+1)));
					u32 uiVertex = x + ulHVertCount * z;
					IData.SetIndex( ulIndex++, uiVertex );
fprintf( pFile, "%d\n", IData.GetIndex( ulIndex - 1 ) );
					uiVertex = uiVertex + ulHVertCount * uiLOD;
					//uiVertex = x + ulHVertCount * (z + 1 );
					IData.SetIndex( ulIndex++, uiVertex );
fprintf( pFile, "%d\n", IData.GetIndex( ulIndex - 1 ) );
				}

				//Degenerate strip:
				//if( ulIndex < ulIndexCount )
				//	IData.SetIndex( ulIndex++, 0xffffffff );
				if( ulIndex < IData.GetIndexCount() )
				{
					pIB->DegenerateTriangleStrip( ulIndex++ );
fprintf( pFile, "%d\n", IData.GetIndex( ulIndex - 1 ) );
				}
				//ret.push_back(0xffffffff);
			}
fclose( pFile );

		}

		m_bReady = true;
		return XST_OK;
	}




	CTerrain::CTerrain(xst_castring& strName, IInputLayout* pIL, CSceneManager* pSceneMgr) :
		ITerrain( pIL, "Terrain", true ),
		_Render( &CTerrain::_RenderNormal ),
		m_strName( strName ),
		m_pVBuffer( xst_null ),
		m_uiLOD( 0 ),
		m_pSceneMgr( pSceneMgr )
	{
	}

	CTerrain::~CTerrain()
	{
		DestroyData();
	}

	void CTerrain::DestroyData()
	{
		xst_release( m_pVBuffer );
		for(u32 i = m_vIndexBuffers.size(); i --> 0;)
		{
			xst_release( m_vIndexBuffers[ i ] );
		}

		m_vIndexBuffers.clear();

		//Destroy all tiles
		if( m_aTiles )
		{
			xst_deletea( m_aTiles );
		}
		else
		{
			for(u32 i = 0; i < m_vTiles.size(); ++i)
			{
				xst_delete( m_vTiles[ i ] );
			}
		}

		m_vTiles.clear();
		m_vRenderBuffer.clear();
	}

	void CTerrain::Update()
	{
		TileVector::iterator Itr;
		CCamera* pCam = this->m_pSceneMgr->GetCurrentCamera();
		m_vRenderBuffer.clear();
		m_aRenderBuffer.clear();
		xst_stl_foreach( Itr, m_vTiles )
		{
			if( pCam->IsSphereInFrustum( (*Itr)->GetBoundingSphere() ) )
			{
				//m_vRenderBuffer.push_back( (*Itr) );
				m_aRenderBuffer.push_back( (*Itr) );
				//break;
			}
		}
	}

	void CTerrain::SetLOD(u32 uiLOD)
	{
		m_uiLOD = uiLOD;
		TileVector::iterator Itr;
		xst_stl_foreach( Itr, m_vTiles )
		{
			(*Itr)->SetLOD( uiLOD );
		}
	}

	CTerrainTile* CTerrain::CreateTile()
	{
		CTerrainTile* pTile = xst_new CTerrainTile();
		m_vTiles.push_back( pTile );
		return pTile;
	}

	void CTerrain::IsVisible(bool bIs)
	{
		if( bIs )
		{
			_Render = &CTerrain::_RenderNormal;
		}
		else
		{
			_Render = &CTerrain::_RenderEmpty;
		}
	}

	void CTerrain::_RenderNormal(XSE::IRenderSystem *pRS)
	{
		//pRS->SetVertexBuffer( this->m_pVBuffer );
		/*pRS->SetInputLayout( this->m_pInputLayout );
		pRS->SetVertexShader( this->m_pMaterial->GetVertexShader().GetPointer() );
		pRS->SetPixelShader( this->m_pMaterial->GetPixelShader().GetPointer() );
		pRS->SetTranslation( 0, 0, 0 );
		pRS->SetTopology( TopologyTypes::TRIANGLE_STRIP );
		pRS->UpdateObjectInputs();
		for(u32 i = 0; i < this->m_vIndexBuffers.size(); ++i)
		{
			pRS->SetVertexBuffer( this->m_pVBuffer );
			pRS->SetIndexBuffer( this->m_vIndexBuffers[ i ] );
			pRS->DrawVertexBuffer( this->m_pVBuffer, this->m_vIndexBuffers[ i ] );
		}*/

		TileVector::iterator Itr;
		pRS->SetInputLayout( this->m_pInputLayout );
		pRS->SetVertexShader( this->m_pMaterial->GetVertexShader().GetPointer() );
		pRS->SetPixelShader( this->m_pMaterial->GetPixelShader().GetPointer() );
		pRS->SetTopology( TopologyTypes::TRIANGLE_STRIP );

		pRS->SetTranslation( 0, 0, 0 );

		pRS->UpdateObjectInputs();

		bool bTest = false;
		ul32 ulSize = (bTest)? 1 : m_aRenderBuffer.size(); //m_vTiles.size();
		CTerrainTile* pTile;
		CPoint MinMax( 0, ulSize );
		//CPoint MinMax( 0, 0 );

		if( this->m_pVBuffer )
			pRS->SetVertexBufferWithCheck( this->m_pVBuffer );
		if( this->m_vIndexBuffers.size() == 1 )
			pRS->SetIndexBuffer( this->m_vIndexBuffers[ 0 ] );
		if( this->m_pVBuffer && this->m_vIndexBuffers.size() == 1 )
			pRS->DrawVertexBuffer( this->m_pVBuffer, this->m_vIndexBuffers[ 0 ] );
		
		for(u32 i = MinMax.x; i < MinMax.y; ++i)
		{

			//pTile = m_vTiles[ i ];
			pTile = m_aRenderBuffer[ i ];
			if( pTile->m_pVBuffer )
			{
				pRS->SetVertexBuffer( pTile->m_pVBuffer );
				pRS->SetIndexBuffer( pTile->m_pCurrentIndexBuffer );
				pRS->DrawVertexBuffer( pTile->m_pVBuffer, pTile->m_pCurrentIndexBuffer );
				//Draw normals - TEST
				if( pTile->m_pVBNormals )
				{
					pRS->SetVertexBuffer( pTile->m_pVBNormals );
					pRS->SetTopology( pTile->m_pVBNormals->GetTopologyType() );
					pRS->SetInputLayout( pTile->m_pVBNormals->GetInputLayout() );
					pRS->SetVertexShader( pTile->m_pNormalVS );
					pRS->SetPixelShader( pTile->m_pNormalPS );
					pRS->DrawVertexBuffer( pTile->m_pVBNormals );

					pRS->SetInputLayout( this->m_pInputLayout );
					pRS->SetVertexShader( this->m_pMaterial->GetVertexShader().GetPointer() );
					pRS->SetPixelShader( this->m_pMaterial->GetPixelShader().GetPointer() );
					pRS->SetTopology( TopologyTypes::TRIANGLE_STRIP );
				}
			}
			else
			{
				//pRS->SetIndexBuffer( pTile->m_pCurrentIndexBuffer );
				//pRS->DrawVertexBuffer( this->m_pVBuffer, pTile->m_pCurrentIndexBuffer );
				//pRS->SetTopology( TopologyTypes::POINT_LIST );
				//pRS->DrawVertexBuffer( this->m_pVBuffer );
			}
		}
	}

	i32 CTerrain::CreateTiles(u32 uiHorizontalCount, u32 uiVerticalCount)
	{
		m_TileCount.x = uiHorizontalCount;
		m_TileCount.y = uiVerticalCount;

		m_aTiles = xst_new CTerrainTile[ m_TileCount.x * m_TileCount.y ];
		if( m_aTiles == xst_null )
		{
			XST_LOG_ERR( "Failed to create tile count array. Memory error." );
			return XST_FAIL;
		}

		return XST_OK;
	}

}//xse