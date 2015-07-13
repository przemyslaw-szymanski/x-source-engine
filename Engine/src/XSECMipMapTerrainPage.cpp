#include "XSECMipMapTerrainPage.h"
#include <XSTCToString.h>
#include "XSECMeshManager.h"
#include "XSECMipMapPagingTerrain.h"
#include "XSECSceneManager.h"
#include "XSECSceneNode.h"

namespace XSE
{
#define XSE_TERRAIN_NORMAL_DEBUG 1

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

	u32 GetId( i32 x, i32 y, u32 w, u32 h )
	{
		x = (x<0? 0 : (x>=w)? w-1 : x);
		y = (y<0? 0 : (y>=h)? h-1 : y);
		return XST_ARRAY_2D_TO_1D( x, y, w );
	}

#define CENTER 0
#define LEFT 1
#define RIGHT 2
#define TOP 3
#define BOTTOM 4

	void CalcTriangleNormal( Vec3* pvecOut, const Vec3& vecV1, const Vec3& vecV2, const Vec3& vecV3 )
	{
		const Vec3 vecU = vecV2 - vecV1;
		const Vec3 vecV = vecV3 - vecV1;
		pvecOut->x = ( vecU.y * vecV.z - vecU.z * vecV.y );
		pvecOut->y = ( vecU.z * vecV.x - vecU.x * vecV.z );
		pvecOut->z = ( vecU.x * vecV.y - vecU.y * vecV.x );
	}

	void CalcNormal(Vec3 aVertices[5], Vec3* pN)
	{
		/*Vec3 vecL = aVertices[LEFT] - aVertices[CENTER];
		Vec3 vecR = aVertices[RIGHT] - aVertices[CENTER];
		Vec3 vecT = aVertices[TOP] - aVertices[CENTER];
		Vec3 vecB = aVertices[BOTTOM] - aVertices[CENTER];
		
		Vec3 vecNormalLT = vecL.Cross(vecT).Normalize();
		Vec3 vecNormalRT = vecR.Cross(vecT).Normalize();
		Vec3 vecNormalLB = vecL.Cross(vecB).Normalize();
		Vec3 vecNormalRB = vecR.Cross(vecB).Normalize();

		Vec3 vecAVGNormal = (vecNormalLT + vecNormalRT + vecNormalLB + vecNormalRB) / 4.0f;
		vecAVGNormal.Normalize();*/
		/*vecAVGNormal.x = ( vecNormalACB.x + vecNormalBCE.x + vecNormalECD.x + vecNormalDCA.x ) / 4.0f;
		vecAVGNormal.x = ( vecNormalACB.x + vecNormalBCE.x + vecNormalECD.x + vecNormalDCA.x ) / 4.0f;
		vecAVGNormal.x = ( vecNormalACB.x + vecNormalBCE.x + vecNormalECD.x + vecNormalDCA.x ) / 4.0f;*/
		//pN->Set( vecAVGNormal );
		Vec3 aNs[4];
		CalcTriangleNormal( &aNs[0], aVertices[CENTER], aVertices[TOP], aVertices[LEFT] );
		CalcTriangleNormal( &aNs[1], aVertices[CENTER], aVertices[RIGHT], aVertices[TOP] );
		CalcTriangleNormal( &aNs[2], aVertices[CENTER], aVertices[BOTTOM], aVertices[RIGHT] );
		CalcTriangleNormal( &aNs[3], aVertices[CENTER], aVertices[LEFT], aVertices[BOTTOM] );

		pN->Set( aNs[0] + aNs[1] + aNs[2] + aNs[3] );
		pN->Normalize();
	}

	i32 CalcVertexNormals(const xst_vector<Vec3>& vPositions, const CPoint& VertexCount, xst_vector<Vec3>* pvNormalsOut)
	{
		XSTSimpleProfiler();
		xst_assert2( vPositions.size() == VertexCount.x * VertexCount.y );

		pvNormalsOut->resize( vPositions.size(), Vec3::ZERO );
		//xst_vector<u32> vTmp( vPositions.size() );

		Vec3 avecVertices[5];

		enum CORNER
		{
			TOP_LEFT,
			TOP_RIGHT,
			BOTTOM_LEFT,
			BOTTOM_RIGHT
		};

		for( u32 y = 0; y < VertexCount.y; ++y )
		{
			for( u32 x = 0; x < VertexCount.x; ++x )
			{
				i32 ids[5]; 
				ids[CENTER] = GetId( x, y, VertexCount.x, VertexCount.y );
				ids[LEFT] = GetId( x-1, y, VertexCount.x, VertexCount.y );
				ids[RIGHT] = GetId( x+1, y, VertexCount.x, VertexCount.y );
				ids[TOP] = GetId( x, y-1, VertexCount.x, VertexCount.y );
				ids[BOTTOM] = GetId( x, y+1, VertexCount.x, VertexCount.y );
				
				avecVertices[0] = vPositions[ ids[0] ];
				avecVertices[1] = vPositions[ ids[1] ];
				avecVertices[2] = vPositions[ ids[2] ];
				avecVertices[3] = vPositions[ ids[3] ];
				avecVertices[4] = vPositions[ ids[4] ];
				Vec3 vecN;
				CalcNormal(avecVertices, &vecN);
				pvNormalsOut->at(ids[CENTER]) = vecN;
			}
		}
		return XST_OK;
	}

	i32 CalcImpostorVertexPositions(const xst_vector<Vec3>& vPositions, const xst_vector<Vec3>& vNormals, 
		xst_vector<CMipMapTerrainTile::SInfo>* pvTileOut, const CPoint& TileCount, const CPoint& TileVertexCount,
		CVertexData* pDataOut)
	{
		CPoint VertexPos;
		Vec3 vecTileMin(XST_MAX_F32), vecTileMax(XST_MIN_F32);
		ul32 ulCurrVertex = 0;
		Vec3 vecNormal;
		CPoint VertexRange;
		ul32 ulVertexId;
		Vec3 vecPos;
		auto& VData = *pDataOut;

		for (u32 uTileY = 0; uTileY < TileCount.y; ++uTileY)
		{
			for (u32 uTileX = 0; uTileX < TileCount.y; ++uTileX)
			{
				VertexPos.y = uTileY * (TileVertexCount.y - 1);
				vecTileMin = XST_MAX_F32;
				vecTileMax = XST_MIN_F32;
				VertexRange.x = ulCurrVertex;
				for (u32 uVertexY = 0; uVertexY < TileVertexCount.y; ++uVertexY)
				{
					VertexPos.x = uTileX * (TileVertexCount.x - 1);

					for (u32 uVertexX = 0; uVertexX < TileVertexCount.x; ++uVertexX)
					{
						ulVertexId = XST_ARRAY_2D_TO_1D( VertexPos.x, VertexPos.y, TileVertexCount.x );
						vecPos = vPositions[ ulVertexId ];
						vecNormal = vNormals[ ulVertexId ];
						vecTileMin.Min( vecPos );
						vecTileMax.Max( vecPos );

						VData.SetPosition( ulCurrVertex, vecPos );

						VertexPos.x++;
						ulCurrVertex++;
					}
					VertexPos.y++;
				}
			}
		}
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
		const Vec2 vecTILEize( m_Info.vecPageSize.x / m_Info.TileCount.x, m_Info.vecPageSize.y / m_Info.TileCount.y );
		const Vec2 vecTileHalfSize( vecTILEize * 0.5f );
		Vec2 vecStep( vecTILEize.x / (m_Info.TileVertexCount.x-1), vecTILEize.y / (m_Info.TileVertexCount.y-1) );
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
		if( !m_Info.pIB )
			//XSE::CalcVertexNormals( vPositions, m_Info.VertexCount, &vNormals );
			CalcVertexNormals( vPositions, &vNormals );
		else
			vNormals.resize(vPositions.size(), Vec3::ZERO);
#if (XSE_TERRAIN_NORMAL_DEBUG)
		xst_vector<Vec3> vNormPos;
		vNormPos.reserve( ulVertexCount *2 );
		xst_vector<u16> vNormIds;
		MeshPtr pNormMesh = CMeshManager::GetSingletonPtr()->CreateMesh("tmp_normal_mesh");
		pNormMesh->SetInputLayout( ILE::POSITION | ILE::COLOR );
		m_pTerrain->m_pSceneMgr->GetRootNode()->AddObject(pNormMesh);
		VertexBufferPtr pVB = pNormMesh->CreateVertexBuffer();
		IndexBufferPtr pIB = pNormMesh->CreateIndexBuffer();

		//pVB->SetInputLayout(/*m_Info.pInputLayout*/ILE::POSITION | ILE::COLOR);
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
		const Vec4 vecCol1 = { 1,0,0,1 };
		const Vec4 vecCol2 = { 0,1,0,1 };
		const bool bIsColor = pIL->IsColor();
		const bool bIsNormal = pIL->IsNormal();
		const bool bIsTexcoord0 = pIL->IsTexCoord0();
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
						VNData.SetPosition( ulCurrNormVid, vecPos );
						VNData.SetColor( ulCurrNormVid++, vecCol1 );
						VNData.SetPosition( ulCurrNormVid, vecPos + vecNormal * 2 );
						VNData.SetColor( ulCurrNormVid++, vecCol2 );
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

						if( bIsColor )
						{
							VData.SetColor( ulCurrVertex, vecCol );
						}

						if( bIsNormal )
						{
							VData.SetNormal( ulCurrVertex, vecNormal );
						}

						if( bIsTexcoord0 )
						{
							const Vec2 vecUV(	(f32)( uTileX * ( m_Info.TileVertexCount.x - 1 ) + uVertexX ) / m_Info.VertexCount.x,
												(f32)( uTileY * ( m_Info.TileVertexCount.y - 1 ) + uVertexY ) / m_Info.VertexCount.y);
							VData.SetTexCoord0( ulCurrVertex, vecUV );
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
				TileInfo.TilePart = CPoint( m_Info.GridPosition.x + uTileX, m_Info.GridPosition.y + uTileY );
				TileInfo.VertexRange.y = ulCurrVertex - 1;
				TileInfo.pVB = m_Info.pVB;
				pTile->Init( TileInfo );
				pTile->SetPosition( vecTilePos + Vec3(vecTileHalfSize.x, 0, vecTileHalfSize.y) ); // calculate center: right_top_corner + half_size
				vecTileMax.x = vecTileMin.x + vecTILEize.x;
				vecTileMax.z = vecTileMin.z + vecTILEize.y;
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

		if( m_Info.pIB )
			CalcVertexNormals( VData, m_Info.pIB );

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

	i32 CMipMapTerrainPage::SetTileData(CMipMapTerrainTile *const *const *paTILE, u32 uiStartTile, u32 uiEndTile)
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
		const Vec2 vecTILEize( m_Info.vecPageSize.x / m_Info.TileCount.x, m_Info.vecPageSize.y / m_Info.TileCount.y );
		const Vec2 vecTileHalfSize( vecTILEize * 0.5f );
		Vec2 vecStep( vecTILEize.x / (m_Info.TileVertexCount.x-1), vecTILEize.y / (m_Info.TileVertexCount.y-1) );
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
		//xst_vector<u32> vTmp( vPositions.size() );

		Vec3 avecTriLeft[3];
		Vec3 avecTriRight[3];
		Vec3 avecNormals[2];
		ul32 uIDs[5];
		ul32 uTmpIds[6];
		Vec3 vecU, vecV;

		enum CORNER
		{
			MID,
			TOP_LEFT,
			TOP_RIGHT,
			BOTTOM_LEFT,
			BOTTOM_RIGHT,
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
				{
					uTmpIds[0] = uIDs[ CORNER::TOP_LEFT ];
					uTmpIds[1] = uIDs[ CORNER::BOTTOM_LEFT ];
					uTmpIds[2] = uIDs[ CORNER::BOTTOM_RIGHT ];
					uTmpIds[3] = uIDs[ CORNER::TOP_LEFT ];
					uTmpIds[4] = uIDs[ CORNER::BOTTOM_RIGHT ];
					uTmpIds[5] = uIDs[ CORNER::TOP_RIGHT ];

					// Left triangle |\.
					avecTriLeft[ 0 ] = vPositions[ uTmpIds[0] ]; // top left
					avecTriLeft[ 1 ] = vPositions[ uTmpIds[1] ];  // bottom left
					avecTriLeft[ 2 ] = vPositions[ uTmpIds[2] ]; // botton right
					// Right triangle \|
					avecTriRight[ 0 ] = vPositions[ uTmpIds[3] ]; // top left
					avecTriRight[ 1 ] = vPositions[ uTmpIds[4] ]; // bottom right
					avecTriRight[ 2 ] = vPositions[ uTmpIds[5] ]; // top right
				}
				CalcTriangleNormal(&avecNormals[0], avecTriLeft[0], avecTriLeft[1], avecTriLeft[2]);
				pvNormalsOut->at( uTmpIds[0] ) += avecNormals[0];
				pvNormalsOut->at( uTmpIds[1] ) += avecNormals[0];
				pvNormalsOut->at( uTmpIds[2] ) += avecNormals[0];			

				CalcTriangleNormal(&avecNormals[1], avecTriRight[0], avecTriRight[1], avecTriRight[2]);
				pvNormalsOut->at( uTmpIds[3] ) += avecNormals[1];
				pvNormalsOut->at( uTmpIds[4] ) += avecNormals[1];
				pvNormalsOut->at( uTmpIds[5] ) += avecNormals[1];
			}
		}

		/*u32 w = m_Info.VertexCount.x;
		u32 h = m_Info.VertexCount.y;
		u32 uCurrFaceX = 0;
		u32 uCurrFaceY = 0;
		u32 fw = (w-1) * 2;
		u32 fh = (h-1);
		struct SFace
		{
			SFace()
			{ uIds[0] = uIds[1] = uIds[2] = 0; }
			Vec3 vecNormal;
			u32 uIds[3];
		};
		xst_vector<SFace> vFaceNormals( fw * fh );
		for( u32 y = 0; y < h-1; ++y )
		{
			uCurrFaceX = 0;
			for( u32 x = 0; x < h-1; ++x )
			{
				uIDs[ CORNER::TOP_LEFT ] = XST_ARRAY_2D_TO_1D( x, y, w );
				uIDs[ CORNER::TOP_RIGHT ] = XST_ARRAY_2D_TO_1D( x+1, y, w );
				uIDs[ CORNER::BOTTOM_RIGHT ] = XST_ARRAY_2D_TO_1D( x + 1, y + 1, w );
				uIDs[ CORNER::BOTTOM_LEFT ] = XST_ARRAY_2D_TO_1D( x, y + 1, w );

				// Create quad
				{
					uTmpIds[0] = uIDs[ CORNER::TOP_LEFT ];
					uTmpIds[1] = uIDs[ CORNER::BOTTOM_LEFT ];
					uTmpIds[2] = uIDs[ CORNER::BOTTOM_RIGHT ];
					uTmpIds[3] = uIDs[ CORNER::TOP_LEFT ];
					uTmpIds[4] = uIDs[ CORNER::BOTTOM_RIGHT ];
					uTmpIds[5] = uIDs[ CORNER::TOP_RIGHT ];

					// Left triangle |\.
					avecTriLeft[ 0 ] = vPositions[ uTmpIds[0] ]; // top left
					avecTriLeft[ 1 ] = vPositions[ uTmpIds[1] ];  // bottom left
					avecTriLeft[ 2 ] = vPositions[ uTmpIds[2] ]; // botton right
					// Right triangle \|
					avecTriRight[ 0 ] = vPositions[ uTmpIds[3] ]; // top left
					avecTriRight[ 1 ] = vPositions[ uTmpIds[4] ]; // bottom right
					avecTriRight[ 2 ] = vPositions[ uTmpIds[5] ]; // top right
				}
				CalcTriangleNormal(&avecNormals[0], avecTriLeft[1], avecTriLeft[2], avecTriLeft[0]);
				u32 uId = XST_ARRAY_2D_TO_1D(uCurrFaceX++,y,fw);
				vFaceNormals[uId].vecNormal = Vec3::Normalize(avecNormals[0]);
				vFaceNormals[uId].uIds[0] = uTmpIds[0];
				vFaceNormals[uId].uIds[1] = uTmpIds[1];
				vFaceNormals[uId].uIds[2] = uTmpIds[2];

				CalcTriangleNormal(&avecNormals[1], avecTriRight[2], avecTriRight[0], avecTriRight[1]);
				uId = XST_ARRAY_2D_TO_1D(uCurrFaceX++,y,fw);
				vFaceNormals[uId].vecNormal = Vec3::Normalize(avecNormals[1]);
				vFaceNormals[uId].uIds[0] = uTmpIds[3];
				vFaceNormals[uId].uIds[1] = uTmpIds[4];
				vFaceNormals[uId].uIds[2] = uTmpIds[5];
			}
		}

		u32 uCurrVertexNormal = 0;
		auto& vNormals = *pvNormalsOut;
		for( u32 y = 0; y < fh; ++y )
		{
			u32 uMid = 0, uUp = 0, uLeft = 0, uRight = 0, uDown = 0;
			for( u32 x = 0; x < fw; ++x )
			{
				// For current face x,y get 4 neighbours
				uMid = XST_ARRAY_2D_TO_1D(x,y,fw);
				uUp = (y > 0)? XST_ARRAY_2D_TO_1D(x,y-1,fw) : 0;
				uDown = (y < fh-1)? XST_ARRAY_2D_TO_1D(x,y+1,fw) : fh-1;
				uLeft = (x > 0)? XST_ARRAY_2D_TO_1D(x-1,y,fw) : 0;
				uRight = (x < fw-1)? XST_ARRAY_2D_TO_1D(x+1,y,fw) : fw-1;

				Vec3 vecAvgNormal = vFaceNormals[uMid].vecNormal + 
					vFaceNormals[uDown].vecNormal +
					vFaceNormals[uUp].vecNormal +
					vFaceNormals[uLeft].vecNormal +
					vFaceNormals[uRight].vecNormal;
				vecAvgNormal /= 5.0f;
				vecAvgNormal.Normalize();
				u32 uIds[] = {vFaceNormals[uMid].uIds[0], vFaceNormals[uMid].uIds[1], vFaceNormals[uMid].uIds[2]};
				vNormals[uIds[0]] += vecAvgNormal;
				vNormals[uIds[1]] += vecAvgNormal;
				vNormals[uIds[2]] += vecAvgNormal;

			}
		}*/

		// TODO: speed up this. Implement a function: NormalizeVectors and use SSE
		for( u32 i = pvNormalsOut->size(); i-- > 0; )
		{
			pvNormalsOut->at( i ).Normalize();
		}

		return XST_OK;
	}

	i32 CMipMapTerrainPage::CalcVertexNormals(CVertexData& VData, const IIndexBuffer* pIB)
	{
		XSTSimpleProfiler();

		xst_vector<Vec3> vNormals( VData.GetVertexCount(), Vec3::ZERO );
		//xst_vector<u32> vTmp( vPositions.size() );

		Vec3 avecTri[3];
		u32 auTri[3];

		const CIndexData& IData = pIB->GetIndexData();
		u32 uTriCount = IData.GetTriangleCount();
		ul32 uIdCount = IData.GetIndexCount();
		u32 uTileCount = m_Info.TileCount.x * m_Info.TileCount.y;

		for( u32 t = 0; t < uTileCount; ++t )
		{
			u32 uOffset = t * uIdCount;
			for( u32 i = 0; i < uIdCount; i += 3 )
			{
				auTri[ 0 ] = IData.GetIndex( i + 0 ) + uOffset;
				auTri[ 1 ] = IData.GetIndex( i + 1 ) + uOffset;
				auTri[ 2 ] = IData.GetIndex( i + 2 ) + uOffset;

				VData.GetPosition( auTri[ 0 ], &avecTri[ 0 ] );
				VData.GetPosition( auTri[ 1 ], &avecTri[ 1 ] );
				VData.GetPosition( auTri[ 2 ], &avecTri[ 2 ] );
				Vec3 vecNormal;
				CalcTriangleNormal( &vecNormal, avecTri[ 0 ], avecTri[ 1 ], avecTri[ 2 ] );

				vNormals[auTri[0]] += vecNormal;
				vNormals[auTri[1]] += vecNormal;
				vNormals[auTri[2]] += vecNormal;
			}
		}
		for( u32 i = vNormals.size(); i-- > 0; )
		{
			vNormals[ i ].Normalize();
			VData.SetNormal( i, vNormals[ i ] );
			//Vec3& v = ( *pvNormalsOut )[i];
			//v.Normalize();
			//Vec3 vecNormal = vNormals[ i ];
			//XST::CDebug::PrintDebugLN( XST::ToStr() << vecNormal.x << " " << vecNormal.y << " " << vecNormal.z );
		}

		return XST_OK;
	}

}//xse