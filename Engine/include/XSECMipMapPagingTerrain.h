#ifndef XSE_CTERRAIN_H
#define XSE_CTERRIAN_H

#include "XSEITerrain.h"
#include "XSECMipMapTerrainPage.h"
#if defined( XSE_RENDERER_DEBUG )
#	include "XSECModel.h"
#endif // XSE_RENDERER_DEBUG

namespace XSE
{
	class CMipMapTerrainTile;
	class CMipMapTerrainPage;
	class CMipMapPagingTerrain;
	class CTerrainSystem;

	class XST_API CMipMapPagingTerrain : public ITerrain
	{
		friend class CSceneManager;
		friend class CTerrainManager;
		friend class CTerrainSystem;
		friend class ITerrainSystem;

		public:

			struct SMipMapIndexBuffer
			{
				MIPMAP_TERRAIN_STITCH_TYPE	eType;
				IndexBufferPtr		pIndexBuffer;
				u8					byLOD;
			};

            struct STileInfo
            {
                bool bVisible = true;
            };

			typedef xst_vector< SMipMapIndexBuffer >	IBVec;
			typedef xst_vector< CMipMapTerrainTile* >	TileVec;
			typedef xst_vector< CMipMapTerrainPage* >	PageVec;
			typedef xst_vector< CMipMapTerrainTile >	TilePoolVec;
			typedef xst_vector< u32 >					IntVec;
			typedef xst_vector< IntVec >				TileIdVec;
            typedef xst_vector< TileVec >               TileGrid;
            using TileInfoVec = xst_vector< STileInfo >;
            using TileInfoGrid = xst_vector< TileInfoVec >;

		protected:

			struct SCalcIBStitchInfo
			{
				u32 uiCurrX;
				u32 uiCurrY;
				u32 uiWidth;
				u32 uiHeight;
				u32 uiVertCountX;
				u32 uiVertCountY;
				u32 uiLastX;
				u32 uiLastY;
				u32 uiLODStep;
				u32 uiNextLODStep;
				u32 uiCurrID;
				u32 uiCurrItrX;
				u32 uiCurrItrY;
			};

			typedef void (CMipMapPagingTerrain::*pfnRender)(IRenderSystem* pRS);
			typedef void (CMipMapPagingTerrain::*pfnCalcIBStitch)(CIndexData*, const SCalcIBStitchInfo&);

		public:

										CMipMapPagingTerrain(xst_castring& strName, IInputLayout* pIL, CSceneManager* pSceneMgr);
			virtual						~CMipMapPagingTerrain();

					i32					Init(const STerrainOptions& Options);

			virtual	i32					LoadImages(const StringVector& vHeightmapNames);

			virtual void				DestroyData();

					void				Disable(cu32& uiReason);

			virtual	xst_fi void			Render(IRenderSystem* pRS)
										{ XST_CALL_MEMBER_FN_PTR( this, _Render )( pRS ); }

			virtual void				SetVisible(bool bVisible);
			virtual bool				IsVisible()
										{ return _Render == &CMipMapPagingTerrain::_RenderNormal; }

			virtual ul32				GetObjectHandle() const
										{ return 0; }

			/*virtual xst_castring&		GetObjectName() const
										{ return m_strName; }*/

			virtual void				Update();

			virtual void				SetTileInfo(CMipMapTerrainTile::SInfo* pInfoOut);

			virtual i32					CreatePages(); 

			virtual i32					CreatePagesVertexData(); //vertex data for all page tiles

			virtual i32					SetPagesData(); 

			virtual CMipMapTerrainTile*	CreateTile();

			virtual i32					CreateTiles();

			virtual i32					LockTiles();

			virtual i32					SetTileData();

			virtual i32					SetTileDataFromPages();

			virtual i32					UnlockTiles();

			virtual void				SetLOD(u32 uiLOD);

					i32					ValidateOptions(STerrainOptions* pOptionsOut);

			virtual 
			const STerrainOptions&		GetOptions() const
										{ return m_Options; }

			virtual i32					CreateIndexBuffers();

			virtual i32					LockIndexBuffers();

			virtual i32					CalcIndexBufferData(u32 uiLOD);
			virtual i32					CalcIndexBufferData();

			virtual i32					CalcVertexNormalData();

			virtual i32					UnlockIndexBuffers();

			virtual void				DestroyIndexBuffers();

			virtual i32					CreateTilesLODs();

			virtual u16					CalcIndexCount(u32 uiLOD);

			virtual u16					CalcTriangleCount(u32 uiLOD);

			virtual xst_fi
			SMipMapIndexBuffer&			GetIndexBuffer(u32 uiLOD, u32 uiStitchType)
										{ return m_vIndexBuffers[ GetIndexBufferID( uiLOD, uiStitchType ) ]; }

			virtual xst_fi
			const SMipMapIndexBuffer&	GetIndexBuffer(u32 uiLOD, u32 uiStitchType) const
										{ return m_vIndexBuffers[ GetIndexBufferID( uiLOD, uiStitchType ) ]; }

			virtual xst_fi	u32			GetIndexBufferID(u32 uiLOD, u32 uiStitchType) const
										{ return uiStitchType + uiLOD * MipMapTerrainStitchTypes::_MAX_COUNT; }

			virtual xst_fi	u32			GetIndexBuffersID(u32 uiLOD) const
										{ return uiLOD * MipMapTerrainStitchTypes::_MAX_COUNT; }

			virtual u32					CalcLODStep(u32 uiLOD);

			virtual void				SetMaterial(MaterialPtr pMat);

		protected:

					pfnRender	_Render;

					void		_RenderEmpty(IRenderSystem* pRS) 
								{ return; }
					void		_RenderNormal(IRenderSystem* pRS);

					void		_OnAddToRenderQueue(CRenderQueueElement* pQueue);

					void		_SetSceneNode(CSceneNode* pNode);
	
					void		_CalcBaseIBStitch(u32 uiLOD, IndexBufferPtr& pIB, pfnCalcIBStitch Func1 = &CMipMapPagingTerrain::_CalcBaseIBStitchEmpty, pfnCalcIBStitch Func2 = &CMipMapPagingTerrain::_CalcBaseIBStitchEmpty);

					void		_CalcBaseIBStitchEmpty(CIndexData* pData, const SCalcIBStitchInfo& Info) {}
					void		_CalcBaseIBStitchRight(CIndexData* pData, const SCalcIBStitchInfo& Info);
					void		_CalcBaseIBStitchLeft(CIndexData* pData, const SCalcIBStitchInfo& Info);
					void		_CalcBaseIBStitchUp(CIndexData* pData, const SCalcIBStitchInfo& Info);
					void		_CalcBaseIBStitchDown(CIndexData* pData, const SCalcIBStitchInfo& Info);
					
					void		_CalcIBStitchDown(u32 uiLOD, IndexBufferPtr& pIB);
					void		_CalcIBStitchDownLeft(u32 uiLOD, IndexBufferPtr& pIB);
					void		_CalcIBStitchLeft(u32 uiLOD, IndexBufferPtr& pIB);
					void		_CalcIBStitchNone(u32 uiLOD, IndexBufferPtr& pIB);
					void		_CalcIBStitchRight(u32 uiLOD, IndexBufferPtr& pIB);
					void		_CalcIBStitchRightDown(u32 uiLOD, IndexBufferPtr& pIB);
					void		_CalcIBStitchUp(u32 uiLOD, IndexBufferPtr& pIB);
					void		_CalcIBStitchUpLeft(u32 uiLOD, IndexBufferPtr& pIB);
					void		_CalcIBStitchUpRight(u32 uiLOD, IndexBufferPtr& pIB);

					void		_CalcQuadCW(CIndexData* pData, cu32& uiX, cu32& uiY, u32* puiCurrTri, cu32& uiLODStep, bool bBackslashTriangle);

					void		_CalcQuadCCW(CIndexData* pData, cu32& uiX, cu32& uiY, u32* puiCurrTri, cu32& uiLODStep, bool bBackslashTriangle);

		protected:

			CSceneManager*	m_pSceneMgr;
			//xst_astring		m_strName;
			TileVec			m_vTiles; //shared tiles among all pages
			TilePoolVec		m_vTilePool;
			TileIdVec		m_vTileIdGrid; // grid of the tile ids
            TileGrid        m_vTileGrid;
            TileInfoGrid    m_vTileInfoGrid;
			PageVec			m_vPages;
			IBVec			m_vIndexBuffers;
			CPoint			m_TileCount;
			xst_vector<Vec3>	m_vTerrNormals;
#if defined( XSE_RENDERER_DEBUG )
			ModelPtr		m_pBoundingSphereModel; // Used for each tile
			ModelPtr		m_pAABBModel; // Used for each tile
			bool			m_bBoundingObjectsCreated = false;
#endif // XSE_RENDERER_DEBUG
			bool			m_bTileLocked;
	};


}//xse

#endif