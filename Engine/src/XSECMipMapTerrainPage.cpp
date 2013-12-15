#include "XSECMipMapTerrainPage.h"

namespace XSE
{
	CMipMapTerrainPage::CMipMapTerrainPage(CMipMapPagingTerrain* pTerrain) : 
		m_pTerrain( pTerrain ),
		m_ulTileVertexCount( 0 ),
		m_pTileInputLayout( xst_null )
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

	i32 CMipMapTerrainPage::Init(cu32& uiWidth, cu32& uiHeight, cul32& ulTileVertexCount, IInputLayout* pTileInputLayout)
	{
		m_TileCount = CPoint( uiWidth, uiHeight );
		u32 uiCount = m_TileCount.x * m_TileCount.y;
		m_ulTileVertexCount = ulTileVertexCount;
		m_pTileInputLayout = pTileInputLayout;

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
		xst_assert2( m_ulTileVertexCount > 0 );
		xst_assert2( m_pTileInputLayout != xst_null );

		for(u32 i = 0; i < m_aVertexData.size(); ++i)
		{
			if( XST_FAILED( m_aVertexData[ i ].pData->Create( m_ulTileVertexCount, m_pTileInputLayout ) ) )
			{
				return XST_FAIL;
			}
		}
		return XST_OK;
	}

	i32 CMipMapTerrainPage::CalcVertexData(CMipMapTerrainTile::SInfo Info)
	{
		Info.pHeightmap = m_pImg.GetPointer();

		for(u32 y = 0; y < m_TileCount.y; ++y)
		{
			Info.TilePart.y = y;

			for(u32 x = 0; x < m_TileCount.x; ++x)
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

	i32 CMipMapTerrainPage::CalcTileVertexData(const CMipMapTerrainTile::SInfo& Info)
	{
		SVertexData& Data = GetVertexData( Info.TilePart.x, Info.TilePart.y );
		return CMipMapTerrainTile::CalcVertexData( Info, Data.pData, &Data.Volume );
	}

}//xse