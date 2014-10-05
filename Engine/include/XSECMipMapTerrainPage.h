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

		typedef xst_vector< CMipMapTerrainTile* >	TileVec;
		//typedef xst_vector< SVertexData >			VertexDataVec;
		typedef XST::TCDynamicArray< SVertexData >	VertexDataArray;
		//xst_vector< SVertexData, XST::STL::TCAlignedAllocator< SVertexData, 16 > > v;

	public:

		CMipMapTerrainPage(CMipMapPagingTerrain* pTerrain);
		virtual					~CMipMapTerrainPage();

		i32				Init(cu32& uiWidth, cu32& uiHeight, cul32& ulTileVertexCount, IInputLayout* pTileInputLayout);

		xst_fi	void			SetImage(ImagePtr pImg)
		{ m_pImg = pImg; }

		xst_fi	ImagePtr		GetImage()
		{ return m_pImg; }

		xst_fi	const ImagePtr&	GetImage() const
		{ return m_pImg; }

		i32				CalcTileVertexData(const CMipMapTerrainTile::SInfo& Info, const Vec3* aNormals, cul32& ulNormalCount);

		i32				CreateVertexData();

		i32				CalcVertexData(CMipMapTerrainTile::SInfo Info);

		void			CalcVertexNormalsForTiles();

		void			CalcTileVertexNormals(const CMipMapTerrainTile::SInfo& Info);

		xst_fi	u32				CalcVertexDataId(cu32& uiX, cu32& uiY) const
		{ return uiX + m_TileCount.x * uiY; }

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
		ImagePtr				m_pImg;
		CPoint					m_TileCount;
		ul32					m_ulTileVertexCount;
		IInputLayout*			m_pTileInputLayout;
	};
}//xse

#endif