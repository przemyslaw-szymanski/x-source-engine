#include "XSECMipMapTerrainPage.h"
#include <XSTCToString.h>
#include "XSECMeshManager.h"
#include "XSECMipMapPagingTerrain.h"
#include "XSECSceneManager.h"
#include "XSECSceneNode.h"

namespace XSE
{
#define XSE_TERRAIN_NORMAL_DEBUG 0

	CMipMapTerrainPage::CMipMapTerrainPage(CMipMapPagingTerrain* pTerrain) : 
		m_pTerrain( pTerrain )
	{
	}

	CMipMapTerrainPage::~CMipMapTerrainPage()
	{

	}

	i32 CMipMapTerrainPage::Init(const CMipMapTerrainPage::SInfo& Info)
	{
		m_Info = Info;
		xst_assert2( m_Info.aTiles );
		xst_assert2( m_Info.pImg );
		xst_assert2( m_Info.pInputLayout );
		xst_assert2( m_Info.pVB );
		xst_assert2( m_Info.pImpVB );
		
		m_Info.pVB->SetInputLayout( m_Info.pInputLayout );
		m_Info.pVB->SetTopologyType( TopologyTypes::TRIANGLE_LIST );
		m_Info.pVB->SetUsage( BufferUsages::DEFAULT );
		ul32 ulVertexCount = (m_Info.TileVertexCount.x * m_Info.TileVertexCount.y) * (m_Info.TileCount.x * m_Info.TileCount.y);
		m_Info.pVB->SetVertexCount( ulVertexCount );

		return XST_OK;
	}

	void CMipMapTerrainPage::Update(const CCamera* pCam)
	{

	}

	i32 CMipMapTerrainPage::CreateVertexBuffer()
	{
		return XST_OK;
	}

	void CMipMapTerrainPage::DestroyVertexBuffer()
	{

	}

	i32 CMipMapTerrainPage::LockVertexBuffer()
	{
		xst_assert2( m_Info.pVB );
		if( XST_FAILED( m_Info.pVB->Lock() ) )
		{
			return XST_FAIL;
		}

		/*if( XST_FAILED( m_Info.pImpVB->Lock() ) )
		{
			return XST_FAIL;
		}*/
		return XST_OK;
	}

	i32 CMipMapTerrainPage::UnlockVertexBuffer()
	{
		xst_assert2( m_Info.pVB );
		if( XST_FAILED( m_Info.pVB->Unlock() ) )
		{
			return XST_FAIL;
		}

		/*if( XST_FAILED( m_Info.pImpVB->Unlock() ) )
		{
			return XST_FAIL;
		}*/
		return XST_OK;
	}

	void CMipMapTerrainPage::CalcVertexPositions()
	{
		cu32 uImgWidth	= m_Info.pImg->GetWidth();
		cu32 uImgHeight = m_Info.pImg->GetHeight();
		const CPoint Range( uImgWidth - m_Info.ImgPixelStartPosition.x, uImgHeight - m_Info.ImgPixelStartPosition.y );
		xst_assert2( Range.x >= m_Info.VertexCount.x && Range.y >= m_Info.VertexCount.y );

		CVertexData& VData = m_Info.pVB->GetVertexData();
	
		Vec3 vecPos = m_Info.vecPagePosition;
		CPoint PixelPos = m_Info.ImgPixelStartPosition;
		ul32 ulVertexOffsetStart = 0;
		ul32 ulVertexDataSize = 0;
		ul32 ulVertexId = 0;
		cul32 ulVertexCount = VData.GetVertexCount();
		cul32 ulTileVertexCount = m_Info.TileVertexCount.x * m_Info.TileVertexCount.y;
		CMipMapTerrainTile::SInfo TileInfo;
		Vec3 vecTilePos = m_Info.vecPagePosition;
		Vec3 vecTilePosEnd = vecTilePos;
		TileInfo.ulStartVertex = 0;
		const Vec2 vecTileSize( m_Info.vecPageSize.x / m_Info.TileCount.x, m_Info.vecPageSize.y / m_Info.TileCount.y );
		const Vec2 vecTileHalfSize( vecTileSize * 0.5f );
		Vec2 vecStep( vecTileSize.x / (m_Info.TileVertexCount.x-1), vecTileSize.y / (m_Info.TileVertexCount.y-1) );
		const IInputLayout* pIL = m_Info.pInputLayout;
		u32 uCurrTileId = 0;
		Vec4 vecCol(1,1,1,1);
		Vec3 vecTileMin( XST_MAX_F32 ), vecTileMax( XST_MIN_F32 );
		ul32 ulCurrVertex = 0;
		Vec3 vecNormal;

		CPoint VertexPos;
		char t[256 ];
		xst_vector<Vec3> vPositions;
		xst_vector<Vec3> vNormals;
		CalcVertexPositions( &vPositions );
		CalcVertexNormals( vPositions, &vNormals );
#if (XSE_TERRAIN_NORMAL_DEBUG)
		xst_vector<Vec3> vNormPos;
		vNormPos.reserve( ulVertexCount *2 );
		xst_vector<u16> vNormIds;
		MeshPtr pNormMesh = CMeshManager::GetSingletonPtr()->CreateMesh("tmp_normal_mesh");
		m_pTerrain->m_pSceneMgr->GetRootNode()->AddObject(pNormMesh);
		VertexBufferPtr pVB = pNormMesh->CreateVertexBuffer();
		IndexBufferPtr pIB = pNormMesh->CreateIndexBuffer();

		pVB->SetInputLayout(m_Info.pInputLayout);
		pVB->SetTopologyType(TopologyTypes::LINE_LIST);
		pVB->SetUsage(BufferUsages::STATIC);
		pVB->SetVertexCount( ulVertexCount * 2 );
		pIB->SetIndexCount( ulVertexCount * 2 );
		pIB->SetUsage(BufferUsages::STATIC);
		pIB->SetIndexSize( IndexElementSizes::UNSIGNED_32 );
		pVB->Lock();
		pIB->Lock();
		ul32 ulCurrNormVid = 0;
		ul32 ulCurrNormId = 0;
		CVertexData& VNData = pVB->GetVertexData();
		CIndexData& INData = pIB->GetIndexData();
#endif
		
		for( u32 uTileY = 0; uTileY < m_Info.TileCount.y; ++uTileY )
		{
			for( u32 uTileX = 0; uTileX < m_Info.TileCount.x; ++uTileX )
			{
				VertexPos.y = uTileY * ( m_Info.TileVertexCount.y-1 );
				
				vecTileMin = XST_MAX_F32;
				vecTileMax = XST_MIN_F32;

				TileInfo.VertexRange.x = ulCurrVertex;

				for( u32 uVertexY = 0; uVertexY < m_Info.TileVertexCount.y; ++uVertexY )
				{
					VertexPos.x = uTileX * ( m_Info.TileVertexCount.x-1 );
		
					for( u32 uVertexX = 0; uVertexX < m_Info.TileVertexCount.x; ++uVertexX )
					{
						ulVertexId = XST_ARRAY_2D_TO_1D( VertexPos.x, VertexPos.y, m_Info.VertexCount.x );
						vecPos = vPositions[ ulVertexId ];
						vecNormal = vNormals[ ulVertexId ];
						vecTileMin.Min( vecPos );
						vecTileMax.Max( vecPos );
#if (XSE_TERRAIN_NORMAL_DEBUG)
						VNData.SetPosition( ulCurrNormVid++, vecPos );
						VNData.SetPosition( ulCurrNormVid++, vecPos + vecNormal * 10 );
						INData.SetIndex( ulCurrNormId++, ulCurrNormVid - 2 );
						INData.SetIndex( ulCurrNormId++, ulCurrNormVid - 1 );
#endif
						/*vecTileMin.y = std::min( vecTileMin.y, vecPos.y );
						vecTileMin.x = std::min( vecTileMin.x, vecPos.x );
						vecTileMin.z = std::min( vecTileMin.z, vecPos.z );
						vecTileMax.y = std::max( vecTileMax.y, vecPos.y );
						vecTileMax.x = std::max( vecTileMin.x, vecPos.x );
						vecTileMax.z = std::max( vecTileMax.z, vecPos.z );*/
						

						VData.SetPosition( ulCurrVertex, vecPos );

						if( pIL->IsColor() )
						{
							VData.SetColor( ulCurrVertex, vecCol );
						}

						if( pIL->IsNormal() )
						{
							VData.SetNormal( ulCurrVertex, vecNormal );
						}

						VertexPos.x++;
						ulCurrVertex++;
					}
					
					VertexPos.y++;

				}
				//sprintf(t, "-(%d,%d)]", VertexPos.x, VertexPos.y); XST::CDebug::PrintDebug(t);
				// Calculate tile info
				u32 uTileId = XST_ARRAY_2D_TO_1D( uTileX, uTileY, m_Info.TileCount.x );
				xst_assert2( uTileId < m_Info.uTileCount );
				CMipMapTerrainTile* pTile = &m_Info.aTiles[ uTileId ];
				TileInfo.ulStartVertex = uCurrTileId * ulTileVertexCount;
				TileInfo.ulVertexBufferOffset = TileInfo.ulStartVertex;
				TileInfo.TilePart = CPoint( uTileX, uTileY );
				TileInfo.VertexRange.y = ulCurrVertex - 1;
				TileInfo.pVB = m_Info.pVB;
				pTile->Init( TileInfo );
				pTile->SetPosition( vecTilePos + Vec3(vecTileHalfSize.x, 0, vecTileHalfSize.y) ); // calculate center: right_top_corner + half_size
				vecTileMax.x = vecTileMin.x + vecTileSize.x;
				vecTileMax.z = vecTileMin.z + vecTileSize.y;
				CBoundingVolume Vol;
				Vol.BuildFromMinMax( vecTileMin, vecTileMax );
				//sprintf( t, "[(%.2f,%.2f, %.2f)-(%.2f,%.2f, %.2f)]", vecTileMin.x, vecTileMin.y, vecTileMin.z, vecTileMax.x, vecTileMax.y, vecTileMax.z ); XST::CDebug::PrintDebugLN( t );
				pTile->SetBoundingVolume( Vol );
				pTile->SetPosition( Vol.GetAABB( ).CalcCenter() );
#if (XSE_TERRAIN_NORMAL_DEBUG)
				xst_sprintf( t, sizeof(t), "terr_p%d_t%dx%d", m_Info.uPageId, TileInfo.TilePart.x, TileInfo.TilePart.y );
				pTile->_SetDbgName( t );
#endif
				*pTile->m_pbIsVisible = true;
		
				uCurrTileId++;
				//sprintf( t, "vertex range: (%d-%d) ", TileInfo.ulVertexBufferOffset, TileInfo.ulStartVertex ); XST::CDebug::PrintDebug( t );
			}
			
			//XST::CDebug::PrintDebugLN( "" );
			vecPos.z = m_Info.vecPagePosition.y + uTileY * ( (m_Info.TileVertexCount.y-1) * vecStep.y );	
		}
		//xst_assert2( ulVertexId == ulVertexCount );

#if (XSE_TERRAIN_NORMAL_DEBUG)
		pVB->Unlock();
		pIB->Unlock();
#endif
	}

	void CMipMapTerrainPage::CalcVertexNormals()
	{

	}

	i32 CMipMapTerrainPage::FillVertexBuffer()
	{
		return XST_OK;
	}

	i32 CMipMapTerrainPage::CreateVertexData()
	{
		xst_assert2( m_Info.pInputLayout != xst_null );
		xst_assert2( m_Info.pVB );
		
		return XST_OK;
	}


	i32 CMipMapTerrainPage::CalcVertexDataForPage()
	{
		xst_assert( m_Info.pImg, "(CMipMapTerrainPage::CalcVertexDataForPage) Heightmap is not set" );
		xst_assert( m_Info.pImg->GetWidth() == m_Info.VertexCount.x && m_Info.pImg->GetHeight() == m_Info.VertexCount.y, "(CMipMapTerrainPage::CalcVertexDataForPage) Heightmap image has wrong scale" );

		Vec3 vecPos = m_Info.vecPagePosition;
		const Vec2 vecVertexDistance( m_Info.vecPageSize.x / m_Info.VertexCount.x, m_Info.vecPageSize.y / m_Info.VertexCount.y );
		u8 byHeightColor;

		for( ul32 y = 0; y < m_Info.VertexCount.y; ++y )
		{
			for( ul32 x = 0; x < m_Info.VertexCount.x; ++x )
			{
				byHeightColor = m_Info.pImg->GetChannelColor( x, y, Resources::IImage::CHANNEL::RED );
				vecPos.x += vecVertexDistance.x;
				vecPos.y += CMipMapTerrainTile::ColorToHeight( m_Info.vecHeightRange, byHeightColor );

				m_vVertices.push_back( vecPos );
			}

			vecPos.x = m_Info.vecPagePosition.x;
			vecPos.z += vecVertexDistance.y;
		}
		return XST_OK;
	}

	i32 CMipMapTerrainPage::SetTileData(CMipMapTerrainTile *const *const *paTiles, u32 uiStartTile, u32 uiEndTile)
	{
		return XST_OK;
	}

	i32 CMipMapTerrainPage::CalcVertexPositions(xst_vector<Vec3>* pvOut)
	{
		XSTSimpleProfiler2("CMipMapTerrainPage::CalcVertexPositions(xst_vector<Vec3>* pvOut)");
		ul32 uVertexCount = m_Info.VertexCount.x * m_Info.VertexCount.y;
		pvOut->resize( uVertexCount );

		Vec3 vecPos = m_Info.vecPagePosition;
		CPoint PixelPos = m_Info.ImgPixelStartPosition;
		ul32 ulVertexOffsetStart = 0;
		ul32 ulVertexDataSize = 0;
		ul32 ulVertexId = 0;
		cul32 ulTileVertexCount = m_Info.TileVertexCount.x * m_Info.TileVertexCount.y;
		CMipMapTerrainTile::SInfo TileInfo;
		Vec3 vecTilePos = m_Info.vecPagePosition;
		Vec3 vecTilePosEnd = vecTilePos;
		TileInfo.ulStartVertex = 0;
		const Vec2 vecTileSize( m_Info.vecPageSize.x / m_Info.TileCount.x, m_Info.vecPageSize.y / m_Info.TileCount.y );
		const Vec2 vecTileHalfSize( vecTileSize * 0.5f );
		Vec2 vecStep( vecTileSize.x / (m_Info.TileVertexCount.x-1), vecTileSize.y / (m_Info.TileVertexCount.y-1) );
		const IInputLayout* pIL = m_Info.pInputLayout;
		u32 uCurrTileId = 0;
		Vec4 vecCol(1,1,1,1);
		Vec3 vecTileMin( XST_MAX_F32 ), vecTileMax( XST_MIN_F32 );

		for( u32 uVertexY = 0; uVertexY < m_Info.VertexCount.y; ++uVertexY )
		{
			vecPos.x = m_Info.vecPagePosition.x;
			PixelPos.x = m_Info.ImgPixelStartPosition.x;
			for( u32 uVertexX = 0; uVertexX < m_Info.VertexCount.x; ++uVertexX )
			{
				u8 uR = m_Info.pImg->GetChannelColor( PixelPos.x, PixelPos.y, Resources::IImage::CHANNEL::RED );
				vecPos.y = CMipMapTerrainTile::ColorToHeight( m_Info.vecHeightRange, uR );
						
				( *pvOut )[ulVertexId] = vecPos;

				vecPos.x += vecStep.x;

				ulVertexId++;
				PixelPos.x++;
			}
			vecPos.z += vecStep.y;
			PixelPos.y++;
		}
		return XST_OK;
	}

	i32 CMipMapTerrainPage::CalcVertexNormals(const xst_vector<Vec3>& vPositions, xst_vector<Vec3>* pvNormalsOut)
	{
		XSTSimpleProfiler();
		xst_assert2( vPositions.size() == m_Info.VertexCount.x * m_Info.VertexCount.y );

		pvNormalsOut->resize( vPositions.size(), Vec3::ZERO );
		xst_vector<Vec3> vTmp( vPositions.size() );

		Vec3 avecTriLeft[3];
		Vec3 avecTriRight[3];
		Vec3 avecNormals[2];
		ul32 uIDs[4];
		Vec3 vecU, vecV;

		enum CORNER
		{
			TOP_LEFT,
			TOP_RIGHT,
			BOTTOM_LEFT,
			BOTTOM_RIGHT
		};

		for( u32 y = 0; y < m_Info.VertexCount.y-1; ++y )
		{
			for( u32 x = 0; x < m_Info.VertexCount.x-1; ++x )
			{
				uIDs[ CORNER::TOP_LEFT ] = XST_ARRAY_2D_TO_1D( x, y, m_Info.VertexCount.x );
				uIDs[ CORNER::TOP_RIGHT ] = XST_ARRAY_2D_TO_1D( x+1, y, m_Info.VertexCount.x );
				uIDs[ CORNER::BOTTOM_RIGHT ] = XST_ARRAY_2D_TO_1D( x + 1, y + 1, m_Info.VertexCount.x );
				uIDs[ CORNER::BOTTOM_LEFT ] = XST_ARRAY_2D_TO_1D( x, y + 1, m_Info.VertexCount.x );

				// Create quad
				// Left triangle |\.
				avecTriLeft[ 0 ] = vPositions[ uIDs[ CORNER::TOP_LEFT ] ]; // top left
				avecTriLeft[ 1 ] = vPositions[ uIDs[ CORNER::BOTTOM_LEFT ] ];  // bottom left
				avecTriLeft[ 2 ] = vPositions[ uIDs[ CORNER::BOTTOM_RIGHT] ]; // botton right

				vecU = avecTriLeft[ 1 ] - avecTriLeft[ 0 ];
				vecV = avecTriLeft[ 2 ] - avecTriLeft[ 0 ];
				avecNormals[ 0 ].x = ( vecU.y * vecV.z - vecU.z * vecV.y );
				avecNormals[ 0 ].y = ( vecU.z * vecV.x - vecU.x * vecV.z );
				avecNormals[ 0 ].z = ( vecU.x * vecV.y - vecU.y * vecV.x );

				pvNormalsOut->at( uIDs[ CORNER::TOP_LEFT ] ) += avecNormals[0];
				pvNormalsOut->at( uIDs[ CORNER::BOTTOM_LEFT ] ) += avecNormals[0];
				pvNormalsOut->at( uIDs[ CORNER::BOTTOM_RIGHT ] ) += avecNormals[0];

				// Right triangle \|
				avecTriRight[ 0 ] = vPositions[ uIDs[ CORNER::TOP_LEFT ] ]; // top left
				avecTriRight[ 1 ] = vPositions[ uIDs[ CORNER::BOTTOM_RIGHT ] ]; // bottom right
				avecTriRight[ 2 ] = vPositions[ uIDs[ CORNER::TOP_RIGHT ] ]; // top right

				vecU = avecTriRight[ 1 ] - avecTriRight[ 0 ];
				vecV = avecTriRight[ 2 ] - avecTriRight[ 0 ];
				avecNormals[ 1 ].x = ( vecU.y * vecV.z - vecU.z * vecV.y );
				avecNormals[ 1 ].y = ( vecU.z * vecV.x - vecU.x * vecV.z );
				avecNormals[ 1 ].z = ( vecU.x * vecV.y - vecU.y * vecV.x );

				(*pvNormalsOut)[ uIDs[ CORNER::TOP_LEFT ] ] += avecNormals[1];
				(*pvNormalsOut)[ uIDs[ CORNER::BOTTOM_RIGHT ] ] += avecNormals[1];
				(*pvNormalsOut)[ uIDs[ CORNER::TOP_RIGHT ] ] += avecNormals[1];
			}
		}

		// TODO: speed up this. Implement a function: NormalizeVectors and use SSE
		for( u32 i = pvNormalsOut->size(); i-- > 0; )
		{
			pvNormalsOut->at( i ).Normalize();
			//Vec3& v = ( *pvNormalsOut )[i];
			//v.Normalize();
			//Vec3 vecNormal = pvNormalsOut->at( i );
			//XST::CDebug::PrintDebugLN( XST::ToStr() << vecNormal.x << " " << vecNormal.y << " " << vecNormal.z );
		}

		return XST_OK;
	}

}//xse