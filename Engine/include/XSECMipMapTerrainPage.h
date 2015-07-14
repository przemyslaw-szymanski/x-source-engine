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
            u8 m_padding[ 12 ];
		};

		struct SInfo
		{
			Vec3 vecPagePosition; // position in 3d space. Top left corner position
			CPoint GridPosition; // position of this page in a global terrain grid
			CPoint VertexCount; // vertex count for this page
			CPoint TileCount; // tile count for this page for x and y
			CPoint TileVertexCount; // vertex count per one tile
			CPoint ImgPixelStartPosition;
			CPoint ImpostorVertexCount; // vertex count for impostor vertex buffer
			Vec2 vecHeightRange; // terrain vertex y position min-max
			Vec2 vecPageSize; // size x-z of the page
			const Resources::IImage* pImg = xst_null;
			const Resources::CTexture*	apTextures[5];
			const IInputLayout* pInputLayout = xst_null;
			CMipMapTerrainTile* aTiles = xst_null; // a begin pointer to the tile buffer
			IVertexBuffer* pVB = xst_null;
			const IIndexBuffer* pIB = xst_null;
			IVertexBuffer* pImpVB = xst_null; // impostor vertex buffer to render distant land
			const Resources::IImage* pImpImg = xst_null; // impostor image
			u32 uTileCount = 0; // a end indicator in the tile buffer
			u32	uPageId;
            bool bEachTileHasOwnTexCoords = false;
		};

		struct SImpostor
		{
			VertexBufferPtr pVB;
		};

		typedef xst_vector< CMipMapTerrainTile >	TileVec;
		typedef xst_vector< CMipMapTerrainTile >	ImpostorTileVec;
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

		i32				CalcVertexPositions(xst_vector<Vec3>* pvOut);

		i32				CalcVertexNormals(const xst_vector<Vec3>& vPositions, xst_vector<Vec3>* pvNormalsOut);
		i32				CalcVertexNormals(CVertexData& VData, const IIndexBuffer* pIB);

		xst_fi	u32		CalcVertexDataId(cu32& uiX, cu32& uiY) const
						{ return uiX + m_Info.TileCount.x * uiY; }


	protected:

		SInfo					m_Info;
		CMipMapPagingTerrain*	m_pTerrain = xst_null;
		CBoundingVolume			m_BoundingVolume;
		ImpostorTileVec			m_vImpTiles;
		//MeshPtr					m_pMesh; // Not drawable
		xst_vector<Vec3>		m_vVertices;
		xst_vector<Vec3>		m_vNormals;
        u8 m_padding2[ 12 ];
	};
}//xse

#endif