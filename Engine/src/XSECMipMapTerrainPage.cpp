#include "XSECMipMapTerrainPage.h"

namespace XSE
{
	CMipMapTerrainPage::CMipMapTerrainPage(CMipMapPagingTerrain* pTerrain) : 
		m_pTerrain( pTerrain )/*,
		m_ulTileVertexCount( 0 ),
		m_pTileInputLayout( xst_null )*/
	{
	}

	CMipMapTerrainPage::~CMipMapTerrainPage()
	{
		/*VertexDataVec::iterator Itr;
		xst_stl_foreach( Itr, m_aVertexData )
		{
			xst_delete( (*Itr).pData );
		}*/
		for(u32 i = m_aVertexData.size(); i --> 0;)
		{
			xst_delete( m_aVertexData[ i ].pData );
		}

		m_aVertexData.clear();
		//xst_vector_clear( m_aVertexData, SVertexData );
	}

	i32 CMipMapTerrainPage::Init(const CMipMapTerrainPage::SInfo& Info)
	{
		m_Info = Info;
		u32 uiCount = m_Info.TileCount.x * m_Info.TileCount.y;
		m_aVertexData.reserve( uiCount );

		for(u32 i = 0; i < uiCount; ++i)
		{
			CVertexData* pData = xst_new CVertexData();
			m_aVertexData.push_back( SVertexData( pData ) );
		}

		return XST_OK;
	}

	i32 CMipMapTerrainPage::CreateVertexData()
	{
		xst_assert2( m_Info.pInputLayout != xst_null );
		//u32 ulTileVertexCount = ( m_Info.VertexCount.x / m_Info.TileCount.x ) * ( m_Info.VertexCount.y / m_Info.TileCount.y );
		for(u32 i = 0; i < m_aVertexData.size(); ++i)
		{
			if( XST_FAILED( m_aVertexData[ i ].pData->Create( m_Info.TileVertexCount.x * m_Info.TileVertexCount.y, m_Info.pInputLayout ) ) )
			{
				return XST_FAIL;
			}
		}
		return XST_OK;
	}

	i32 CMipMapTerrainPage::CalcVertexData(CMipMapTerrainTile::SInfo Info)
	{
		Info.pHeightmap = GetImage();

		for(u32 y = 0; y < m_Info.TileCount.y; ++y)
		{
			Info.TilePart.y = y;

			for(u32 x = 0; x < m_Info.TileCount.x; ++x)
			{
				Info.TilePart.x = x;

				if( XST_FAILED( CalcTileVertexData( Info ) ) )
				{
					return XST_FAIL;
				}
			}
		}

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
		//xst_assert( paTiles && *paTiles, "(CMipMapTerrainPage::SetTileData)" );
		//CMipMapTerrainTile *const *const aTiles = (*paTiles);

		u32 uCurrTile = uiStartTile;
		u32 uStartX = 0, uStartY = 0;

		for( u32 uTileY = 0; uTileY < m_Info.TileCount.y - 1; ++uTileY )
		{
			for( u32 uTileX = 0; uTileX < m_Info.TileCount.x - 1; ++uTileX )
			{
				//if( uCurrTile >= uiEndTile )
				//	break;

				//CMipMapTerrainTile* pTile = aTiles[ uCurrTile++ ];
				//VertexBufferWeakPtr pVB = pTile->GetMesh( )->GetVertexBuffer();
				//CVertexData& VData = pVB->GetVertexData();
				CVertexData* pVData = m_aVertexData[uCurrTile++].pData;
				u32 uCurrY = uStartY;

				for( u32 y = 0; y < m_Info.TileVertexCount.y; ++y )
				{
					u32 uCurrX = uStartX;
					for( u32 x = 0; x < m_Info.TileVertexCount.x; ++x )
					{
						u32 uId = XST_ARRAY_2D_TO_1D( uCurrX, uCurrY, m_Info.VertexCount.x );
						u32 uVSize = m_vVertices.size();
						if( uId >= uVSize )
						{
							int a;
							a = uVSize;
							a = a;
						}
						xst_assert( uId < m_vVertices.size(), "(CMipMapTerrainPage::SetTileData) Index out of bounds" );
						u32 uId2 = XST_ARRAY_2D_TO_1D( x, y, m_Info.TileVertexCount.x );
						u32 uVCount = pVData->GetVertexCount();
						xst_assert2( uId2 < uVCount );
						pVData->SetPosition( uId2, m_vVertices[ uId ] );
						uCurrX = x + uStartX;
					}
					uCurrY = uStartY + y;
				}

				uStartX += m_Info.TileVertexCount.x;
			}

			uStartX = 0;
			uStartY += m_Info.TileVertexCount.y;
		}

		return XST_OK;
	}

	void CMipMapTerrainPage::CalcVertexNormals()
	{
		//for(u32 i = 0; i < m_ )
	}

	i32 CMipMapTerrainPage::CalcTileVertexData(const CMipMapTerrainTile::SInfo& Info)
	{
		SVertexData& Data = GetVertexData( Info.TilePart.x, Info.TilePart.y );
		return CMipMapTerrainTile::CalcVertexData( Info, Data.pData, &Data.Volume );
	}

}//xse