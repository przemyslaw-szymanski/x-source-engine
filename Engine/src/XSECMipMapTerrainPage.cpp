#include "XSECMipMapTerrainPage.h"
#include <XSTCToString.h>

namespace XSE
{
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
		m_Info.pVB->SetInputLayout( m_Info.pInputLayout );
		m_Info.pVB->SetTopologyType( TopologyTypes::TRIANGLE_LIST );
		m_Info.pVB->SetUsage( BufferUsages::DYNAMIC );
		cul32 ulVertexCount = (m_Info.TileVertexCount.x * m_Info.TileVertexCount.y) * (m_Info.TileCount.x * m_Info.TileCount.y);
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
		return m_Info.pVB->Lock();
	}

	i32 CMipMapTerrainPage::UnlockVertexBuffer()
	{
		xst_assert2( m_Info.pVB );
		return m_Info.pVB->Unlock();
	}

	void CMipMapTerrainPage::CalcVertexPositions()
	{
		cu32 uImgWidth	= m_Info.pImg->GetWidth();
		cu32 uImgHeight = m_Info.pImg->GetHeight();
		const CPoint Range( uImgWidth - m_Info.ImgPixelStartPosition.x, uImgHeight - m_Info.ImgPixelStartPosition.y );
		xst_assert2( Range.x >= m_Info.VertexCount.x && Range.y >= m_Info.VertexCount.y );

		CVertexData& VData = m_Info.pVB->GetVertexData();

		Vec3 vecPos = m_Info.vecPagePosition;
		//const Vec2 vecStep( m_Info.vecPageSize.x / (m_Info.VertexCount.x-0), m_Info.vecPageSize.y / (m_Info.VertexCount.y-0) );
		CPoint PixelPos = m_Info.ImgPixelStartPosition;
		ul32 ulVertexOffsetStart = 0;
		ul32 ulVertexDataSize = 0;
		ul32 ulVertexId = 0;
		cul32 ulVertexCount = m_Info.VertexCount.x * m_Info.VertexCount.y;
		cul32 ulTileVertexCount = m_Info.TileVertexCount.x * m_Info.TileVertexCount.y;
		CMipMapTerrainTile::SInfo TileInfo;
		Vec3 vecTilePos = m_Info.vecPagePosition;
		Vec3 vecTilePosEnd = vecTilePos;
		TileInfo.ulVertexBufferDataSize = ulTileVertexCount * m_Info.pInputLayout->GetVertexSize();
		const Vec2 vecTileSize( m_Info.vecPageSize.x / m_Info.TileCount.x, m_Info.vecPageSize.y / m_Info.TileCount.y );
		const Vec2 vecStep( vecTileSize.x / m_Info.TileVertexCount.x, vecTileSize.y / m_Info.TileVertexCount.y );

        char t[256];

		for( u32 uTileY = 0; uTileY < m_Info.TileCount.y; ++uTileY )
		{
            XST::CDebug::PrintDebugLN("");
			vecTilePos.z = uTileY * vecTileSize.y;
			for( u32 uTileX = 0; uTileX < m_Info.TileCount.x; ++uTileX )
			{
                vecTilePos.x = uTileX * vecTileSize.x;
                sprintf( t, "[(%.2f,%.2f)", vecTilePos.x, vecTilePos.z ); XST::CDebug::PrintDebug( t );
				TileInfo.VertexRange.x = ulVertexId;
				vecPos.z = vecTilePos.z;
				for( u32 uVertexY = 0; uVertexY < m_Info.TileVertexCount.y; ++uVertexY )
				{
					//vecPos.x = m_Info.vecPagePosition.x + uTileX * ( (m_Info.TileVertexCount.x-1) * vecStep.x );
					vecPos.x = vecTilePos.x;
					PixelPos.x = m_Info.ImgPixelStartPosition.x + uTileX * (m_Info.TileVertexCount.x-1);
					for( u32 uVertexX = 0; uVertexX < m_Info.TileVertexCount.x; ++uVertexX )
					{
						if( PixelPos.x >= uImgWidth || PixelPos.y >= uImgHeight )
						{
							int a = 3;
							a = a;
						}
						u8 uR = m_Info.pImg->GetChannelColor( PixelPos.x, PixelPos.y, Resources::IImage::CHANNEL::RED );
						vecPos.y = CMipMapTerrainTile::ColorToHeight( m_Info.vecHeightRange, uR );
						vecPos.x += vecStep.x;

						//VData.SetPosition( ulVertexId, vecPos );

						++ulVertexId;
					}
					vecPos.z += vecStep.y;
					PixelPos.y++;

					vecTilePosEnd.x = vecPos.x;
					vecTilePosEnd.z = vecPos.z;
				}
				PixelPos.y = m_Info.ImgPixelStartPosition.y + uTileY * (m_Info.TileVertexCount.y-1);

				CMipMapTerrainTile* pTile = &m_Info.aTiles[ XST_ARRAY_2D_TO_1D( uTileX, uTileY, m_Info.TileCount.x ) ];
				TileInfo.ulVertexBufferOffset = ulVertexOffsetStart;
				TileInfo.TilePart = CPoint( uTileX, uTileY );
				TileInfo.VertexRange.y = ulVertexId - 1;
				pTile->SetInfo( TileInfo );
				ulVertexOffsetStart += TileInfo.ulVertexBufferDataSize;

				
				
				sprintf( t, "(%.2f,%.2f)] ", vecTilePosEnd.x, vecTilePosEnd.z ); XST::CDebug::PrintDebug( t );
			}
			vecPos.z = m_Info.vecPagePosition.y + uTileY * ( (m_Info.TileVertexCount.y-1) * vecStep.y );
			
		}
		xst_assert2( ulVertexId == ulVertexCount );
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



}//xse