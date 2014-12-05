#include "XSECMipMapTerrainTile.h"
#include "XSECMeshManager.h"
#include "XSEIImage.h"
#include "XST/XSTCToString.h"

#define TERRAIN_GROUP	"TERRAIN"

namespace XSE
{
	using namespace XST;

	CMipMapTerrainTile::CMipMapTerrainTile() 
	{
	}

	CMipMapTerrainTile::~CMipMapTerrainTile()
	{
		//xst_release( m_pMesh );
	}

	i32 CMipMapTerrainTile::Init(const SInfo& Info)
	{
		m_Info = Info;
		return XST_OK;
	}

	i32	CMipMapTerrainTile::Lock(MeshWeakPtr pMesh, ul32 ulVertexCount)
	{
		/*m_pMesh = pMesh;
		IVertexBuffer* pVB;
		{
			//XSTSimpleProfiler2("-CreateVertexBuffer"); //~0.0007sec in debug
			VertexBufferPtr pBuff( pMesh->CreateVertexBuffer( false ) );
			pMesh->SetVertexBuffer( pBuff, 0 );
			pMesh->SetLOD( 0 );
			pVB = pBuff.GetPtr();
		}
		pVB->SetVertexCount( ulVertexCount );
		pVB->SetTopologyType( XSE::TopologyTypes::TRIANGLE_LIST );
		pVB->SetUsage( XSE::BufferUsages::STATIC );

		return pMesh->GetVertexBuffer()->Lock();*/
		return 0;
	}	

	i32 CMipMapTerrainTile::SetVertexData(const CVertexData& SrcData)
	{
		/*xst_assert2( m_pMesh != xst_null );
		xst_assert2( m_pMesh->GetVertexBuffer() != xst_null );
		xst_assert2( m_pMesh->GetVertexBuffer()->IsLocked() );

		CVertexData& Data = m_pMesh->GetVertexBuffer()->GetVertexData();

		return Data.SetData( SrcData );*/
		return 0;
	}

	i32 CMipMapTerrainTile::CalcVertexData(const SInfo& Info, CVertexData* pData, CBoundingVolume* pVolumeOut)
	{
		/*xst_assert( pData != xst_null, "(CMipMapTerrainTile::CalcVertexData)" );
		ul32 ulCurrVertex = 0;
		f32 uiWidth = Info.vecTileSize.x;
		f32 uiHeight = Info.vecTileSize.y;

		const Vec3 vecTilePos( Info.TilePart.x * Info.vecTileSize.x, 0, -Info.TilePart.y * Info.vecTileSize.y );
		const Vec3 vecBeginPos( Info.vecTerrainPosition + vecTilePos );
		const Vec3 vecEndPos( vecBeginPos + Vec3( Info.vecTileSize.x, 0, -Info.vecTileSize.y ) );
		Vec3 vecPos( vecBeginPos );
		Vec3 vecNormal;
		const Vec2 vecImgBeginPos( Info.TilePart.x * ( Info.VertexCount.x - 1 ), Info.TilePart.y * ( Info.VertexCount.y - 1 ) );
		Vec2 vecCurrPos;
		XST::CColor Color( XST::CColor::BLACK );
		Vec4 vecCol( 1, 1, 1, 1 );
		Vec3 vecMin ( vecBeginPos.x, XST_MAX_F32, vecEndPos.z ), vecMax( vecEndPos.x, XST_MIN_F32, vecBeginPos.z );
		const IInputLayout* pIL = pData->GetInputLayout();

		for(u32 z = 0; z < Info.VertexCount.y; ++z)
		{
			for(u32 x = 0; x < Info.VertexCount.x; ++x)
			{
				vecCurrPos.Set( x, z );
				if( Info.pHeightmap )
				{
					//Get color from heightmap
					vecCurrPos += vecImgBeginPos;
					Color = Info.pHeightmap->GetColor( vecCurrPos.x, vecCurrPos.y );
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
				if( pIL->IsNormal() )
				{
					ul32 ulPos = XST_ARRAY_2D_TO_1D( vecCurrPos.x, vecCurrPos.y, Info.TerrainVertexCount.x );
					xst_assert( ulPos < Info.pvTerrainNormals->size(), "(CMipMapTerrainTile::CalcVertexData) Array index out of bounds" );
					vecNormal = Info.pvTerrainNormals->at( ulPos );
					pData->SetNormal( ulCurrVertex, vecNormal );
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
		*/
		return XST_OK;
	}

	i32 CMipMapTerrainTile::Unlock()
	{
		//return m_pMesh->GetVertexBuffer()->Unlock();
		return 0;
	}
		
	void CMipMapTerrainTile::SetLOD(u32 uiLOD, MIPMAP_TERRAIN_STITCH_TYPE eType)
	{
		m_uiLOD = uiLOD;
		m_eStitchType = eType;
		//m_pMesh->SetLOD( uiMeshLOD );
	}

	void CMipMapTerrainTile::_OnObjectDisable(cu32& uReason)
	{
		( *m_pbIsVisible ) = !this->IsDisabled();
	}


}//xse