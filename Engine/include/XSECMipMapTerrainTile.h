#ifndef XSE_CTERRAIN_TILE_H
#define XSE_CTERRAIN_TILE_H

#include "XSECBoundingSphere.h"
#include "XST/XSTCAABB.h"
#include "XSECMesh.h"

namespace XSE
{
	class CMeshManager;

	struct MipMapTerrainStitchTypes
	{
		enum TYPE
		{
			NONE = 0,
			UP, // -
			RIGHT, // |
			DOWN, // _
			LEFT, // |
			UP_RIGHT, // -|
			UP_LEFT, // |-
			RIGHT_DOWN, // _|
			DOWN_LEFT, // |_
			/*UP_RIGHT_DOWN,
			UP_LEFT_DOWN,
			RIGHT_DOWN_LEFT,
			UP_LEFT_RIGHT,
			UP_RIGHT_DOWN_LEFT,*/
			_MAX_COUNT,
			DOWN_RIGHT	= RIGHT_DOWN,
			RIGHT_UP	= UP_RIGHT,
			LEFT_UP		= UP_LEFT,
			LEFT_DOWN	= DOWN_LEFT
		};
	};

	typedef MipMapTerrainStitchTypes::TYPE	MIPMAP_TERRAIN_STITCH_TYPE;

	class XST_API CMipMapTerrainTile : public CObject
	{
		friend class CMipMapPagingTerrain;
		friend class CMipMapTerrainPage;
		friend class CTerrainSystem;

		public:

			struct SInfo
			{
				CPoint					TilePart;
				CPoint					VertexRange; // for debug only
				ul32					ulVertexBufferOffset; // offset bytes in global (page) vertex buffer
				ul32					ulStartVertex; // start vertex for this tile
				const IVertexBuffer*	pVB;
			};

		public:

											CMipMapTerrainTile();
			virtual							~CMipMapTerrainTile();

			i32								Init(const SInfo& Info);

			ul32							GetObjectHandle() const xst_implement { return (ul32)this; }

			i32								Lock(MeshWeakPtr pMesh, ul32 ulVertexCount);

			i32								CalcVertexData(const SInfo& Info);

			static	i32						CalcVertexData(const SInfo& Info, CVertexData* pData, CBoundingVolume* pVolumeOut = xst_null);

			static	xst_fi f32				ColorToHeight(const Vec2& vecHeightRange, cu8& byValue)
											{
												//Convert color from byte to [0,1] float
												cf32 fColor = (f32)byValue / 255.0f;
												return XST::Math::Lerp( vecHeightRange.x, vecHeightRange.y, fColor );
											}

			i32								Unlock();

			void							SetLOD(u32 uiLOD, MIPMAP_TERRAIN_STITCH_TYPE eType);

			i32								SetVertexData(const CVertexData& Data);

			void							SetNeighbours(CMipMapTerrainTile* apNeigbours[ 4 ])
											{ m_apNeighbours[ 0 ] = apNeigbours[ 0 ]; m_apNeighbours[ 1 ] = apNeigbours[ 1 ]; m_apNeighbours[ 2 ] = apNeigbours[ 2 ]; m_apNeighbours[ 3 ] = apNeigbours[ 3 ]; }

			CMipMapTerrainTile*				GetNeighbour(u32 uiId) { return m_apNeighbours[ uiId ]; }

			xst_fi	u32						GetLOD() const { return m_uiLOD; }

			xst_fi 
			MIPMAP_TERRAIN_STITCH_TYPE		GetStitchType() const { return m_eStitchType; }

		protected:

			void							_OnObjectDisable(cu32& uDisableReason);

		protected:

			SInfo						m_Info;
			CMipMapTerrainTile*			m_apNeighbours[ 4 ];
			u32							m_uiLOD = 0;
			MIPMAP_TERRAIN_STITCH_TYPE	m_eStitchType = MipMapTerrainStitchTypes::NONE;
			xst_vector<bool>::iterator	m_pbIsVisible; // a pointer to a bool array from MipMapPagingTerrain (set in Init method)
	};
}//xse
#endif