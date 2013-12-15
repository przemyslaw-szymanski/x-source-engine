#ifndef XSE_CTERRAIN_TILE_H
#define XSE_CTERRAIN_TILE_H

#include "XSECBoundingSphere.h"
#include "XST/XSTCAABB.h"
#include "XSECMesh.h"

namespace XSE
{
	class CMeshManager;


	class XST_API CMipMapTerrainTile
	{
		friend class CMipMapPagingTerrain;
		friend class CTerrainSystem;

		public:

			struct SInfo
			{
				CPoint						TilePart;
				Vec3						vecTileSize;
				CPoint						VertexCount;
				CPoint						TerrainVertexCount;
				Vec2						vecHeightRange;
				Vec3						vecTerrainPosition;
				Vec3						vecVertexDistance;
				Vec3						vecDebugColor;
				const Resources::IImage*	pHeightmap;
			};

		public:

											CMipMapTerrainTile();
			virtual							~CMipMapTerrainTile();

			i32								Lock(MeshPtr pMesh, ul32 ulVertexCount);

			i32								CalcVertexData(const SInfo& Info);

			static	i32						CalcVertexData(const SInfo& Info, CVertexData* pData, CBoundingVolume* pVolumeOut = xst_null);

			static	xst_fi f32				ColorToHeight(const Vec2& vecHeightRange, cu8& byValue)
											{
												//Convert color from byte to [0,1] float
												cf32 fColor = (f32)byValue / 255.0f;
												return XST::Math::Lerp( vecHeightRange.x, vecHeightRange.y, fColor );
											}

			i32								Unlock();

			void							SetLOD(u32 uiLOD);

			const Vec3&						GetTranslation() const
											{ return m_vecTranslation; }

			const MeshPtr&					GetMesh() const
											{ return m_pMesh; }

			MeshPtr							GetMesh()
											{ return m_pMesh; }

			i32								SetVertexData(const CVertexData& Data);

			void							SetBoundingVolume(const CBoundingVolume& Volume);

		protected:

			/*i32					_CreateVertexData(XSE::CVertexData& VData, u32 uiWidth, u32 uiHeight);

			i32					_CreateIndexData(MeshPtr pMesh, u32 uiWidth, u32 uiHeight, u32 uiLOD);

			i32					_CreateIndexDataNoStitch(CIndexData& IData, u32 uiWidth, u32 uiHeight, u32 uiLOD);

			void				_CalcStripNoStitch(CIndexData& IData, u32 uiCurrHeight, u32 uiWidth, u32* puiCurrTri, u32 uiLodStep, bool bEvenStrip);

			void				_CalcStripStitchUp(CIndexData& IData, u32 uiCurrHeight, u32 uiWidth, u32* puiCurrTri, u32 uiLodStep, bool bEvenStrip);

			void				_CalcStripStitchLeft(CIndexData& IData, u32 uiCurrHeight, u32 uiWidth, u32* puiCurrTri, u32 uiLodStep, bool bEvenStrip);

			void				_CalcStripStitchUpLeft(CIndexData& IData, u32 uiCurrHeight, u32 uiWidth, u32* puiCurrTri, u32 uiLodStep, bool bEvenStrip);

			void				_CalcQuad(CIndexData& Data, u32 uiX, u32 uiY, u32 uiWidth, u32 uiLodStep, u32* puiCurrTri,  bool bBackslashTriangle);
*/

		protected:

			MeshPtr				m_pMesh;
			Vec3				m_vecTranslation;
			Vec2				m_vecSize;
			bool				m_bReady;
	};
}//xse
#endif