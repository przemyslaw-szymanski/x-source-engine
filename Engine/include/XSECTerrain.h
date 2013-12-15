#ifndef XSE_CTERRAIN_H
#define XSE_CTERRIAN_H

#include "XSEITerrain.h"
#include "XSECBoundingSphere.h"
#include "XST/XSTCAABB.h"

namespace XSE
{
	class CTerrain;
	class CTerrainSystem;

	class XST_API CTerrainTile
	{
		friend class CTerrain;
		friend class CTerrainSystem;

		typedef xst_vector< IIndexBuffer* >	IndexBufferVector;

		struct SLODInfo
		{
			SLODInfo(cu32& uiStartIndex, cu32& uiIndexCount) : uiStartIndex( uiStartIndex ), uiIndexCount( uiIndexCount ) {}
			u32	uiStartIndex;
			u32	uiIndexCount;
		};

		typedef xst_vector< SLODInfo >	LODInfoVector;

		public:

											CTerrainTile();
			virtual							~CTerrainTile();

			virtual	i32						Create(IVertexBuffer* pVB, const Vec3& vecTranslate, const Vec2& vecSize, const Vec2& vecVertexCount, bool bCreateIndexBuffers);
			virtual i32						Create(const CPoint& TilePart, const CPoint& TileSize, const CPoint& TerrainVertexCount, const CPoint& TileVertexCount);

			virtual void					SetLOD(u32 uiLOD)
											{ m_uiLOD = uiLOD; m_pCurrentIndexBuffer = m_vIndexBuffers[ uiLOD ]; }
											//{ m_uiLOD = uiLOD; m_pCurrentLODInfo = &m_vLODInfos[ uiLOD ]; }

			virtual xst_fi IVertexBuffer*	GetVertexBuffer() const
											{ return m_pVBuffer; }

			virtual const Vec3&				GetTranslation() const
											{ return m_vecTranslation; }

			virtual const CBoundingSphere&	GetBoundingSphere() const
											{ return m_Sphere; }

			virtual const XST::CAABB&		GetAABB() const
											{ return m_AABB; }

		protected:

		protected:

			IVertexBuffer*		m_pVBuffer;
			IndexBufferVector	m_vIndexBuffers;
			LODInfoVector		m_vLODInfos;
			const SLODInfo*		m_pCurrentLODInfo;
			IIndexBuffer*		m_pCurrentIndexBuffer;
			Vec3				m_vecTranslation;
			Vec2				m_vecSize;
			Vec2				m_vecVertexCount;
			u32					m_uiLOD;
			bool				m_bReady;
			bool				m_bDestroyIndexBuffers;
			ul32				m_ulStartVertex;
			//Test
			IVertexBuffer*		m_pVBNormals;
			Resources::IVertexShader*		m_pNormalVS;
			Resources::IPixelShader*		m_pNormalPS;
			CBoundingSphere		m_Sphere;
			XST::CAABB			m_AABB;
	};

	class XST_API CTerrain : public ITerrain
	{
		friend class CSceneManager;
		friend class CTerrainManager;
		friend class CTerrainSystem;
		friend class ITerrainSystem;

		public:

			typedef xst_vector< IIndexBuffer* >		IndexBufferVector;
			typedef xst_vector< CTerrainTile* >		TileVector;

		protected:

			typedef void (CTerrain::*pfnRender)(IRenderSystem* pRS);

		public:

									CTerrain(xst_castring& strName, IInputLayout* pIL, CSceneManager* pSceneMgr);
			virtual					~CTerrain();

			virtual void			DestroyData();

			virtual	void			Render(IRenderSystem* pRS)
									{ XST_CALL_MEMBER_FN_PTR( this, _Render )( pRS ); }

			virtual void			IsVisible(bool bIs);
			virtual bool			IsVisible()
									{ return _Render == &CTerrain::_RenderNormal; }

			virtual ul32			GetObjectHandle() const
									{ return 0; }

			virtual xst_castring&	GetObjectName() const
									{ return m_strName; }

			virtual void			Update();

			virtual CTerrainTile*	CreateTile();

			virtual i32				CreateTiles(u32 uiHorizontalCount, u32 uiVerticalCount);

			xst_fi CTerrainTile*	GetTile(u32 uiX, u32 uiY)
									{ xst_assert2( m_aTiles != xst_null && uiX + ( uiY * m_TileCount.y ) < m_TileCount.x * m_TileCount.y ); return &m_aTiles[ uiX + ( uiY * m_TileCount.y ) ]; }

			virtual void			SetLOD(u32 uiLOD);

			virtual void			SetLOD(u32 uiTileId, u32 uiLOD)
									{ m_vTiles[ uiTileId ]->SetLOD( uiLOD ); }

			virtual const STerrainOptions&	GetOptions() const
											{ return m_Options; }

		protected:

					pfnRender	_Render;
					void		_RenderEmpty(IRenderSystem* pRS) 
								{ return; }
					void		_RenderNormal(IRenderSystem* pRS);

		protected:

			IVertexBuffer*		m_pVBuffer;
			IndexBufferVector	m_vIndexBuffers;
			xst_astring			m_strName;
			TileVector			m_vTiles;
			TileVector			m_vRenderBuffer;
			XST::TCDynamicArray< CTerrainTile* >	m_aRenderBuffer;
			u32					m_uiLOD;
			STerrainOptions		m_Options;
			CTerrainTile		*m_aTiles;
			CPoint				m_TileCount;
			CSceneManager*		m_pSceneMgr;
	};


}//xse

#endif