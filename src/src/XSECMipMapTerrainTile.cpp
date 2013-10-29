#include "XSECMipMapTerrainTile.h"
#include "XSECMeshManager.h"
#include "XSEIImage.h"
#include "XST/XSTCToString.h"

#define TERRAIN_GROUP	"TERRAIN"

namespace XSE
{
	using namespace XST;

	CMipMapTerrainTile::CMipMapTerrainTile() : m_pMesh( xst_null )
	{
	}

	CMipMapTerrainTile::~CMipMapTerrainTile()
	{
		//xst_release( m_pMesh );
	}

	i32	CMipMapTerrainTile::Lock(MeshPtr pMesh, ul32 ulVertexCount)
	{
		m_pMesh = pMesh;
		VertexBufferPtr pVB;
		{
			//XSTSimpleProfiler2("-CreateVertexBuffer"); //~0.002sec in debug
			pVB = m_pMesh->CreateVertexBuffer();
		}
		pVB->SetVertexCount( ulVertexCount );
		pVB->SetTopologyType( XSE::TopologyTypes::TRIANGLE_LIST );
		pVB->SetUsage( XSE::BufferUsages::STATIC );
		{
			//XSTSimpleProfiler2("-Lock"); //~0.001sec in debug
		if( XST_FAILED( m_pMesh->GetVertexBuffer()->Lock() ) )
		{
			return XST_FAIL;
		}
		}
		return XST_OK;
	}	

	i32 CMipMapTerrainTile::SetVertexData(const CVertexData& SrcData)
	{
		xst_assert2( m_pMesh != xst_null );
		xst_assert2( m_pMesh->GetVertexBuffer() != xst_null );
		xst_assert2( m_pMesh->GetVertexBuffer()->IsLocked() );

		CVertexData& Data = m_pMesh->GetVertexBuffer()->GetVertexData();

		return Data.SetData( SrcData );
	}

	void CMipMapTerrainTile::SetBoundingVolume(const CBoundingVolume& Volume)
	{
		m_pMesh->SetObjectBoundingVolume( Volume );
	}

	i32 CMipMapTerrainTile::CalcVertexData(const SInfo& Info, CVertexData* pData, CBoundingVolume* pVolumeOut)
	{
		xst_assert( pData != xst_null, "(CMipMapTerrainTile::CalcVertexData)" );
		ul32 ulCurrVertex = 0;
		f32 uiWidth = Info.vecTileSize.x;
		f32 uiHeight = Info.vecTileSize.y;

		const Vec3 vecTilePos( Info.TilePart.x * Info.vecTileSize.x, 0, -Info.TilePart.y * Info.vecTileSize.y );
		const Vec3 vecBeginPos( Info.vecTerrainPosition + vecTilePos );
		const Vec3 vecEndPos( vecBeginPos + Vec3( Info.vecTileSize.x, 0, -Info.vecTileSize.y ) );
		Vec3 vecPos( vecBeginPos );
		CPoint ImgBeginPos( Info.TilePart.x * ( Info.VertexCount.x - 1 ), Info.TilePart.y * ( Info.VertexCount.y - 1 ) );
		XST::CColor Color( XST::CColor::BLACK );
		Vec4 vecCol( 1, 1, 1, 1 );
		Vec3 vecMin ( vecBeginPos.x, XST_MAX_F32, vecEndPos.z ), vecMax( vecEndPos.x, XST_MIN_F32, vecBeginPos.z );
		const IInputLayout* pIL = pData->GetInputLayout();

		for(u32 z = 0; z < Info.VertexCount.y; ++z)
		{

			for(u32 x = 0; x < Info.VertexCount.x; ++x)
			{
				if( Info.pHeightmap )
				{
					//Get color from heightmap
					Color = Info.pHeightmap->GetColor( ImgBeginPos.x + x, ImgBeginPos.y + z );
					vecCol = Vec4( (f32)Color.r / 255.0f, (f32)Color.g / 255.0f, (f32)Color.b / 255.0f, 1 );
				}

				vecPos.y = ColorToHeight( Info.vecHeightRange, Color.r );
				pData->SetPosition( ulCurrVertex, vecPos );
				//XST::CDebug::PrintDebugLN( XST::ToStr() << vecPos );
				
				//Calc min/max vertex position for bounding box
				vecMin.y = Math::Min( vecMin.y, vecPos.y );
				vecMax.y = Math::Max( vecMax.y, vecPos.y );

				vecPos.x += Info.vecVertexDistance.x;

				if( pIL->IsColor() )
				{
					pData->SetColor( ulCurrVertex, vecCol );
				}
				//

				++ulCurrVertex;
			}

			vecPos.x = vecBeginPos.x;
			vecPos.z -= Info.vecVertexDistance.y;
		}
		
		xst_assert( ulCurrVertex == pData->GetVertexCount(), "(CMipMapTerrainTile::CalculateVertexData)" );

		if( pVolumeOut != xst_null )
		{
			//const Vec3 vecSize( vecMax - vecMin );
			//pVolumeOut->GetAABB().CreateFromLeftBottomFrontCorner( vecMin, vecSize );
			//pVolumeOut->GetAABB().GetCenter
			pVolumeOut->BuildFromMinMax( vecMin, vecMax );
			//Vec3 v = pVolumeOut->GetAABB().CalcSize();
			//Vec3 p = pVolumeOut->GetAABB().CalcCenter();
		}

		//XST::CDebug::PrintDebugLN( XST::ToStr() << "Tile data calculated: " << Info.TilePart.x << " " << Info.TilePart.y );
		//DebugPrintVertexData( *pData );

		return XST_OK;
	}

	i32	CMipMapTerrainTile::CalcVertexData(const CMipMapTerrainTile::SInfo& Info)
	{
		CVertexData& Data = m_pMesh->GetVertexBuffer()->GetVertexData();
		return CalcVertexData( Info, &Data );
	}

	i32 CMipMapTerrainTile::Unlock()
	{
		return m_pMesh->GetVertexBuffer()->Unlock();
	}
		
	void CMipMapTerrainTile::SetLOD(u32 uiLOD)
	{
		m_pMesh->SetLOD( uiLOD );
	}

	/*
	CMipMapTerrainTile::CMipMapTerrainTile() :
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

	CMipMapTerrainTile::~CMipMapTerrainTile()
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

	i32	CMipMapTerrainTile::Create(IVertexBuffer* pVB, const Vec3& vecTranslate, const Vec2& vecSize, const Vec2& vecVertexCount, bool bCreateIndexBuffers)
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

		m_bReady = true;
		return XST_OK;
	}

	i32	CMipMapTerrainTile::Create(const CPoint& TilePart, const CPoint& TileSize, const CPoint& TerrainVertexCount, const CPoint& TileVertexCount)
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

	*/

	//i32	CMipMapTerrainTile::CreateVertexData(XSE::CVertexData& VData, u32 uiWidth, u32 uiHeight)
	//{
	//	ul32 ulCurrVert = 0;
	//	XSE::Vec2 vecBeginPos( (f32)uiWidth * -0.5f, (f32)uiHeight * 0.5f );
	//	XSE::Vec3 vecPos( vecBeginPos.x, 0, vecBeginPos.y );
	//	f32 fStep = 5.0f;

	//	for(u32 z = 0; z < uiHeight; ++z)
	//	{
	//		for(u32 x = 0; x < uiWidth; ++x)
	//		{
	//			VData.SetPosition( ulCurrVert, vecPos );
	//			vecPos.x += fStep;
	//			++ulCurrVert;
	//		}

	//		vecPos.x = vecBeginPos.x;
	//		vecPos.z -= fStep;
	//	}

	//	//CreatePlane( VData, uiWidth, uiHeight );

	//	return XST_OK;
	//}


	//i32	CMipMapTerrainTile::CreateIndexData(XSE::MeshPtr pMesh, u32 uiWidth, u32 uiHeight, u32 uiLOD)
	//{
	//	XSE::IndexBufferPtr pIB;
	//	XSE::SMeshLOD LOD;

	//	ul32 ulTriCount = ( uiWidth - 1 ) * ( uiHeight - 1 ) * 2;
	//	ul32 ulIdCount = ulTriCount * 3;

	//	//First lod
	//	pIB = pMesh->CreateIndexBuffer();
	//	pIB->SetUsage( XSE::BufferUsages::STATIC );
	//	pIB->SetIndexCount( ulIdCount );

	//	if( XST_FAILED( pIB->Lock() ) )
	//	{
	//		return XST_FAIL;
	//	}

	//	XSE::CIndexData& IData1 = pIB->GetIndexData();

	//	u32 uiCurrTri = 0;
	//	ul32 ulQuad = 0;
	//	u32 uiLodStep = uiLOD + 1;

	//	for(u32 z = 0; z < uiHeight - uiLodStep; ++z)
	//	{
	//		CalcStripNoStitch( IData1, z, uiWidth, &uiCurrTri, uiLodStep, ulQuad % 2 == 0 );
	//		ulQuad++;
	//	}

	//	uiCurrTri = ulQuad = 0;

	//	if( XST_FAILED( pIB->Unlock() ) )
	//	{
	//		return XST_FAIL;
	//	}

	//	//Second lod
	//	pIB = pMesh->CreateIndexBuffer();
	//	pIB->SetUsage( XSE::BufferUsages::STATIC );
	//	pIB->SetIndexCount( ulIdCount );

	//	if( XST_FAILED( pIB->Lock() ) )
	//	{
	//		return XST_FAIL;
	//	}

	//	XSE::CIndexData& IData2 = pIB->GetIndexData();

	//	for(u32 z = 0; z < uiHeight - uiLodStep; ++z)
	//	{
	//		CalcStripStitchUp( IData2, z, uiWidth, &uiCurrTri, uiLodStep, ulQuad % 2 == 0 );
	//		ulQuad++;
	//	}

	//	uiCurrTri = ulQuad = 0;

	//	if( XST_FAILED( pIB->Unlock() ) )
	//	{
	//		return XST_FAIL;
	//	}

	//	LOD.pVertexBuffer = pMesh->GetVertexBuffer();
	//	LOD.pIndexBuffer = pIB;
	//	pMesh->AddLOD( LOD );

	//	//Third lod
	//	pIB = pMesh->CreateIndexBuffer();
	//	pIB->SetUsage( XSE::BufferUsages::STATIC );
	//	pIB->SetIndexCount( ulIdCount );

	//	if( XST_FAILED( pIB->Lock() ) )
	//	{
	//		return XST_FAIL;
	//	}

	//	XSE::CIndexData& IData3 = pIB->GetIndexData();

	//	for(u32 z = 0; z < uiHeight - uiLodStep; ++z)
	//	{
	//		CalcStripStitchLeft( IData3, z, uiWidth, &uiCurrTri, uiLodStep, ulQuad % 2 == 0 );
	//		ulQuad++;
	//	}

	//	uiCurrTri = ulQuad = 0;

	//	if( XST_FAILED( pIB->Unlock() ) )
	//	{
	//		return XST_FAIL;
	//	}

	//	LOD.pVertexBuffer = pMesh->GetVertexBuffer();
	//	LOD.pIndexBuffer = pIB;
	//	pMesh->AddLOD( LOD );

	//	//Fourth lod
	//	pIB = pMesh->CreateIndexBuffer();
	//	pIB->SetUsage( XSE::BufferUsages::STATIC );
	//	pIB->SetIndexCount( ulIdCount );

	//	if( XST_FAILED( pIB->Lock() ) )
	//	{
	//		return XST_FAIL;
	//	}

	//	XSE::CIndexData& IData4 = pIB->GetIndexData();

	//	for(u32 z = 0; z < uiHeight - uiLodStep; ++z)
	//	{
	//		CalcStripStitchUpLeft( IData4, z, uiWidth, &uiCurrTri, uiLodStep, ulQuad % 2 == 0 );
	//		ulQuad++;
	//	}

	//	uiCurrTri = ulQuad = 0;

	//	if( XST_FAILED( pIB->Unlock() ) )
	//	{
	//		return XST_FAIL;
	//	}

	//	LOD.pVertexBuffer = pMesh->GetVertexBuffer();
	//	LOD.pIndexBuffer = pIB;
	//	pMesh->AddLOD( LOD );
	//
	//	return XST_OK;
	//}


	#define CALC_XY(_x, _y, _width) ( (_x) + (_y) * (_width) )

	#define LEFT_UP		0
	#define RIGHT_UP	1
	#define RIGHT_DOWN	2
	#define LEFT_DOWN	3
	#define LOG_IDS(_a, _b, _c) { char buff[10]; sprintf_s( buff, 10, "%d %d %d", (_a), (_b), (_c) ); XSE::CDebug::PrintDebugLN( buff ); }

	//void CMipMapTerrainTile::CalcQuad(XSE::CIndexData& Data, u32 uiX, u32 uiY, u32 uiWidth, u32 uiLodStep, u32* puiCurrTri,  bool bBackslashTriangle)
	//{
	//	u32 uiIds[ 4 ];
	//	u32 uiCurrTri = *puiCurrTri;

	//	uiIds[ LEFT_UP ]	= CALC_XY( uiX, uiY, uiWidth );
	//	uiIds[ LEFT_DOWN ]	= CALC_XY( uiX, uiY + uiLodStep, uiWidth );
	//	uiIds[ RIGHT_UP ]	= CALC_XY( uiX + uiLodStep, uiY, uiWidth );
	//	uiIds[ RIGHT_DOWN ] = CALC_XY( uiX + uiLodStep, uiY + uiLodStep, uiWidth );

	//	if( bBackslashTriangle )
	//	{
	//		//Set the backslash triangle 
	//		/* |\ */
	//		Data.SetTriangle( uiCurrTri++, uiIds[ LEFT_DOWN ], uiIds[ LEFT_UP ], uiIds[ RIGHT_DOWN ] );
	//		//The second part 
	//		/* \| */
	//		Data.SetTriangle( uiCurrTri++, uiIds[ LEFT_UP ], uiIds[ RIGHT_UP ], uiIds[ RIGHT_DOWN ] );
	//	}
	//	else
	//	{
	//		//Set the slash triangle 
	//		/* |/ */
	//		Data.SetTriangle( uiCurrTri++, uiIds[ LEFT_DOWN ], uiIds[ LEFT_UP ], uiIds[ RIGHT_UP ] );
	//		//The second part 
	//		/* /| */
	//		Data.SetTriangle( uiCurrTri++, uiIds[ RIGHT_UP ], uiIds[ RIGHT_DOWN ], uiIds[ LEFT_DOWN ] );
	//	}

	//	*puiCurrTri = uiCurrTri;
	//}

	//void CMipMapTerrainTile::CalcStripNoStitch(XSE::CIndexData& IData, u32 uiCurrHeight, u32 uiWidth, u32* puiCurrTri, u32 uiLodStep, bool bEvenStrip)
	//{
	//	bool bBackslash = bEvenStrip;
	//	for(u32 x = 0; x < uiWidth - uiLodStep; ++x)
	//	{
	//		CalcQuad( IData, x, uiCurrHeight, uiWidth, uiLodStep, puiCurrTri, bBackslash );
	//		bBackslash = !bBackslash;
	//	}
	//}

	//void CMipMapTerrainTile::CalcStripStitchUp(XSE::CIndexData& IData, u32 uiCurrHeight, u32 uiWidth, u32* puiCurrTri, u32 uiLodStep, bool bEvenStrip)
	//{
	//	bool bBackslash = bEvenStrip;
	//	u32 uiPos, uiId, uiCurrId;
	//	u32 uiNextLodStep = uiLodStep * 2;

	//	for(u32 x = 0; x < uiWidth - uiLodStep; ++x)
	//	{
	//		CalcQuad( IData, x, uiCurrHeight, uiWidth, uiLodStep, puiCurrTri, bBackslash );
	//		bBackslash = !bBackslash;
	//		//Move up odd vertices to even vertices only for first strip
	//		if( uiCurrHeight == 0 )
	//		{
	//			uiCurrId = *puiCurrTri * 3;
	//			//For even quads change right up vertex of the second triangle
	//			if( x % 2 == 0 )
	//			{
	//				/* \| change to \/ */
	//				//Right up vertex of the second triangle is at position -2
	//				uiPos = uiCurrId - 2;
	//				uiId = CALC_XY( x + uiNextLodStep, uiCurrHeight, uiWidth );
	//				IData.SetIndex( uiPos, uiId );
	//			}
	//			else //for odd quads degenerate first triangle
	//			{
	//				/* |/ change to / */
	//				//Left up vertex of the first triangle is at position -5
	//				uiPos = uiCurrId - 5;
	//				uiId = CALC_XY( x + uiLodStep, uiCurrHeight, uiWidth );
	//				IData.SetIndex( uiPos, uiId );
	//			}
	//		}
	//	}
	//}

	//void CMipMapTerrainTile::CalcStripStitchLeft(XSE::CIndexData& IData, u32 uiCurrHeight, u32 uiWidth, u32* puiCurrTri, u32 uiLodStep, bool bEvenStrip)
	//{
	//	bool bBackslash = bEvenStrip;
	//	u32 uiPos, uiId, uiCurrId;
	//	u32 uiNextLodStep = uiLodStep * 2;

	//	for(u32 x = 0; x < uiWidth - uiLodStep; ++x)
	//	{
	//		CalcQuad( IData, x, uiCurrHeight, uiWidth, uiLodStep, puiCurrTri, bBackslash );
	//		bBackslash = !bBackslash;
	//		//Move left down vertex of the first triangle
	//		if( x == 0 )
	//		{
	//			uiCurrId = *puiCurrTri * 3;
	//			//For even strip
	//			if( bEvenStrip )
	//			{
	//				//Left down vertex is at index -5
	//				/* |\ */
	//				uiPos = uiCurrId - 6;
	//				uiId = CALC_XY( x, uiCurrHeight + uiNextLodStep, uiWidth );
	//				IData.SetIndex( uiPos, uiId );
	//			}
	//			else
	//			{
	//				//Left up vertex is at index -5
	//				/* |/ */
	//				uiPos = uiCurrId - 5;
	//				uiId = CALC_XY( x + uiLodStep, uiCurrHeight, uiWidth );
	//				IData.SetIndex( uiPos, uiId );
	//			}
	//		}
	//	}
	//}

	//void CMipMapTerrainTile::CalcStripStitchUpLeft(XSE::CIndexData& IData, u32 uiCurrHeight, u32 uiWidth, u32* puiCurrTri, u32 uiLodStep, bool bEvenStrip)
	//{
	//	bool bBackslash = bEvenStrip;
	//	u32 uiPos, uiId, uiCurrId;
	//	u32 uiNextLodStep = uiLodStep * 2;

	//	for(u32 x = 0; x < uiWidth - uiLodStep; ++x)
	//	{
	//		CalcQuad( IData, x, uiCurrHeight, uiWidth, uiLodStep, puiCurrTri, bBackslash );
	//		bBackslash = !bBackslash;
	//		uiCurrId = *puiCurrTri * 3;
	//		//Move up odd vertices to even vertices only for first strip
	//		if( uiCurrHeight == 0 )
	//		{
	//			//For even quads change right up vertex of the second triangle
	//			if( x % 2 == 0 )
	//			{
	//				/* \| change to \/ */
	//				//Right up vertex of the second triangle is at position -2
	//				uiPos = uiCurrId - 2;
	//				uiId = CALC_XY( x + uiNextLodStep, uiCurrHeight, uiWidth );
	//				IData.SetIndex( uiPos, uiId );
	//			}
	//			else //for odd quads degenerate first triangle
	//			{
	//				/* |/ change to / */
	//				//Left up vertex of the first triangle is at position -5
	//				uiPos = uiCurrId - 5;
	//				uiId = CALC_XY( x + uiLodStep, uiCurrHeight, uiWidth );
	//				IData.SetIndex( uiPos, uiId );
	//			}
	//		}

	//		//Move left down vertex of the first triangle
	//		if( x == 0 )
	//		{
	//			//For even strip
	//			if( bEvenStrip )
	//			{
	//				//Left down vertex is at index -5
	//				/* |\ */
	//				uiPos = uiCurrId - 6;
	//				uiId = CALC_XY( x, uiCurrHeight + uiNextLodStep, uiWidth );
	//				IData.SetIndex( uiPos, uiId );
	//			}
	//			else
	//			{
	//				//Left up vertex is at index -5
	//				/* |/ */
	//				uiPos = uiCurrId - 5;
	//				uiId = CALC_XY( x + uiLodStep, uiCurrHeight, uiWidth );
	//				IData.SetIndex( uiPos, uiId );
	//			}
	//		}
	//	}
	//}


}//xse