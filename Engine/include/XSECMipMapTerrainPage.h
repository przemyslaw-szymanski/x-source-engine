#ifndef XSE_CTERRAIN_PAGE_H
#define XSE_CTERRAIN_PAGE_H

#include "XSECMipMapTerrainTile.h"
#include "XSEIImage.h"

namespace XSE
{
	class CMipMapPagingTerrain;

	class CMipMapTerrainPage
	{
		friend class CMipMapPagingTerrain;
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
			Vec3 vecPagePosition; // position in 3d space. Top left corner position
			CPoint VertexCount; // vertex count for this page
			CPoint TileCount; // tile count for this page for x and y
			CPoint TileVertexCount; // vertex count per one tile
			CPoint ImgPixelStartPosition;
			CPoint ImpostorVertexCount; // vertex count for impostor vertex buffer
			Vec2 vecHeightRange; // terrain vertex y position min-max
			Vec2 vecPageSize; // size x-z of the page
			const Resources::IImage* pImg = xst_null;
			const IInputLayout* pInputLayout = xst_null;
			CMipMapTerrainTile* aTiles = xst_null; // a begin pointer to the tile buffer
			IVertexBuffer* pVB = xst_null;
			IVertexBuffer* pImpVB = xst_null; // impostor vertex buffer to render distant land
			const Resources::IImage* pImpImg = xst_null; // impostor image
			u32 uTileCount = 0; // a end indicator in the tile buffer
			u32	uPageId;
		};

		typedef xst_vector< CMipMapTerrainTile >	TileVec;
		//typedef xst_vector< SVertexData >			VertexDataVec;
		typedef XST::TCDynamicArray< SVertexData >	VertexDataArray;
		//xst_vector< SVertexData, XST::STL::TCAlignedAllocator< SVertexData, 16 > > v;

	public:

		CMipMapTerrainPage(CMipMapPagingTerrain* pTerrain);
		virtual					~CMipMapTerrainPage();

		i32				Init(const SInfo& Info);

		void			Update(const CCamera* pCam);

		xst_fi	const Resources::IImage*	GetImage() const
											{ return m_Info.pImg; }

		i32				CreateVertexData();

		i32				CreateVertexBuffer();

		void			DestroyVertexBuffer();

		i32				LockVertexBuffer();

		i32				UnlockVertexBuffer();

		void			CalcVertexPositions();

		void			CalcVertexNormals();

		i32				FillVertexBuffer();

		i32				CalcVertexDataForPage();

		i32				SetTileData(CMipMapTerrainTile *const (*const *paTiles), u32 uiStartTile, u32 uiEndTile);

		xst_fi	u32		CalcVertexDataId(cu32& uiX, cu32& uiY) const
						{ return uiX + m_Info.TileCount.x * uiY; }


	protected:

		CMipMapPagingTerrain*	m_pTerrain = xst_null;
		CBoundingVolume			m_BoundingVolume;
		SInfo					m_Info;
		//MeshPtr					m_pMesh; // Not drawable
		xst_vector<Vec3>		m_vVertices;
		xst_vector<Vec3>		m_vNormals;
	};
}//xse

#endif