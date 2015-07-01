#ifndef XSE_CTERRAIN_SYSTEM_H
#define XSE_CTERRAIN_SYSTEM_H

#include "XSEITerrainSystem.h"
#include "XST/XSTCBinaryData.h"
#include "XST/XSTTCOctree.h"

namespace XSE
{
	class XST_API CTerrainSystem : public ITerrainSystem
	{
		protected:

			typedef xst_map< xst_astring, ITerrain* >	TerrainMap;
			typedef xst_vector< ITerrain* >				TerrainVec;

		public:

			typedef XST::TCPairIterator< TerrainMap >	TerrainIterator;

		public:

									CTerrainSystem(CSceneManager* pSceneMgr, CImageManager* pImgMgr, IRenderSystem* pRS);
			virtual					~CTerrainSystem();

			virtual ITerrain*		CreateTerrain(xst_castring& strName, STerrainOptions& Options, bool bLockOnly);
			virtual ITerrain*		CreateTerrain(xst_castring& strName, STerrainOptions& Options);
			virtual ITerrain*		CreateFromImage(xst_castring& strName, const Resources::IImage* pImg, STerrainOptions& Options);
			virtual ITerrain*		CreateFromBinary(xst_castring& strName, const XST::CBinaryData& Data, STerrainOptions& Options);
			virtual i32				LockTerrain(ITerrain* pTerrain);
			virtual i32				SaveToImage(xst_castring& strImg, const ITerrain* pTerrain);
			virtual i32				SaveToBinary(xst_castring& strBinaryName, const ITerrain* pTerrain);
			virtual void			DestroyTerrain(ITerrain* pTerrain);
			virtual void			DestroyTerrain(xst_castring& strName);
			virtual bool			ValidateOptions(STerrainOptions& Options);
			virtual void			DestroyTerrains();
			virtual void			Update();
			virtual bool			IsAutoDestroy() const
									{ return true; }

		protected:

			CSceneManager*		m_pSceneMgr;
			CImageManager*		m_pImgMgr;
			IRenderSystem*		m_pRenderSystem;
			TerrainMap			m_mTerrains;
			XST::TCOctree		m_Octree;
            u8 m_padding[ 8 ];
			CStaticGeometry*	m_pDbgGeom;
			TerrainVec			m_vTerrains;
            u8 m_padding2[ 12 ];
	};

}//xse

#endif