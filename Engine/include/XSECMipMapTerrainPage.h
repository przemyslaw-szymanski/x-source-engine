#ifndef XSE_CTERRAIN_PAGE_H
#define XSE_CTERRAIN_PAGE_H

#include "XSECMipMapTerrainTile.h"
#include "XSEIImage.h"

namespace XSE
{
	class CMipMapPagingTerrain;

	class CMipMapTerrainPage
	{
	public:

		struct SVertexData
		{
			SVertexData() : pData( xst_null ) {}
			SVertexData(CVertexData* pVData) : pData( pVData ) {}

			CBoundingVolume		Volume;
			CVertexData*		pData;
		};

		struct SInfo
		{
			CPoint VertexCount; // vertex count for this page
			CPoint TileCount; // tile count for this page
			CPoint TileVertexCount; // vertex count per one tile
			Vec2 vecHeightRange; // terrain vertex y position min-max
			Vec3 vecPagePosition; // position in 3d space. Top left corner position
			Vec2 vecPageSize; // size x-z of the page
			const Resources::IImage* pImg = xst_null;
			const IInputLayout* pInputLayout = xst_null;
		};

		typedef xst_vector< CMipMapTerrainTile* >	TileVec;
		//typedef xst_vector< SVertexData >			VertexDataVec;
		typedef XST::TCDynamicArray< SVertexData >	VertexDataArray;
		//xst_vector< SVertexData, XST::STL::TCAlignedAllocator< SVertexData, 16 > > v;

	public:

		CMipMapTerrainPage(CMipMapPagingTerrain* pTerrain);
		virtual					~CMipMapTerrainPage();

		i32				Init(const SInfo& Info);

		xst_fi	const Resources::IImage*	GetImage() const
											{ return m_Info.pImg; }

		i32				CalcTileVertexData(const CMipMapTerrainTile::SInfo& Info);

		i32				CreateVertexData();

		i32				CalcVertexData(CMipMapTerrainTile::SInfo Info);

		i32				CalcVertexDataForPage();

		i32				SetTileData(CMipMapTerrainTile *const (*const *paTiles), u32 uiStartTile, u32 uiEndTile);

		void			CalcVertexNormals();

		xst_fi	u32				CalcVertexDataId(cu32& uiX, cu32& uiY) const
		{ return uiX + m_Info.TileCount.x * uiY; }

		xst_fi	SVertexData&	GetVertexData(cu32& uiX, cu32& uiY)
		{ return m_aVertexData[ CalcVertexDataId( uiX, uiY ) ]; }

		xst_fi	
			const	SVertexData&	GetVertexData(cu32& uiX, cu32& uiY) const
		{ return m_aVertexData[ CalcVertexDataId( uiX, uiY ) ]; }

		xst_fi	
			VertexDataArray&		GetVertexDataBuffer()
		{ return m_aVertexData; }

		xst_fi	
			const VertexDataArray&	GetVertexDataBuffer() const
		{ return m_aVertexData; }

	protected:

		CMipMapPagingTerrain*	m_pTerrain;
		//TileVec					m_vTiles;
		VertexDataArray			m_aVertexData;
		CBoundingVolume			m_BoundingVolume;
		//CPoint					m_TileCount;
		//ul32					m_ulTileVertexCount;
		//IInputLayout*			m_pTileInputLayout;
		SInfo					m_Info;
		xst_vector<Vec3>		m_vVertices;
		xst_vector<Vec3>		m_vNormals;
	};
}//xse

#endif