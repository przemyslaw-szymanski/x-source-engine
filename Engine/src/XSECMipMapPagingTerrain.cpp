#include "XSECMipMapPagingTerrain.h"
#include "XSEIVertexBuffer.h"
#include "XSEIIndexBuffer.h"
#include "XSEIRenderSystem.h"
#include "XSECCamera.h"
#include "XSECSceneManager.h"
#include "XSECRenderQueueElement.h"
#include "XSECMeshManager.h"
#include "XSECModelManager.h"
#include "XSECMaterialManager.h"
#include "XSECSceneNode.h"
#include "XSECImageManager.h"
#include "XSEIPass.h"
#include "XSEITechnique.h"
#include <XSTCToString.h>
namespace XSE
{
	#define CALC_XY(_x, _y, _width) ( (_x) + (_y) * (_width) )

	Vec3* g_avecNormals = xst_null;
	bool g_bCCW = true;
	typedef xst_vector< CMipMapTerrainTile* > TileVec;
	xst_vector< TileVec > g_vTileGrid;
	TileVec g_vVisibleTiles;

	CPoint CalcImpostorVertexCount( const CPoint& PageVertexCount, u32 uLODCount )
	{
		CPoint vc;
		vc.x = (PageVertexCount.x >> uLODCount) + 1;
		vc.y = (PageVertexCount.y >> uLODCount) + 1;
		return vc;
	}

	i32 CreateImpostorVertexBufferData( IVertexBuffer* *const *const pppVBs, u32 uVBCount, const CPoint& VertexCount, const Vec2& vecImpostorSize,
									 const IInputLayout* pIL )
	{
		xst_assert2( uVBCount > 0 );
		IVertexBuffer* *const apVBs = ( *pppVBs );
		IVertexBuffer* pVB;
		cul32 ulVertexCount = VertexCount.x * VertexCount.y;
		const Vec2 vecStep( vecImpostorSize.x / VertexCount.x, vecImpostorSize.y / VertexCount.y );

		for( u32 i = 0; i < uVBCount; ++i )
		{
			pVB = apVBs[ i ];
			pVB->SetInputLayout( pIL );
			pVB->SetTopologyType( TopologyTypes::TRIANGLE_LIST );
			pVB->SetUsage( BufferUsages::DEFAULT );
			pVB->SetVertexCount( ulVertexCount );
		}

		// Create data for first vertex buffer only
		// And then copy that data to the rest
		ul32 ulVertexId = 0;

		for( u32 i = 0; i < 1; ++i )
		{
			pVB = apVBs[ i ];
			if( XST_FAILED( pVB->Lock() ) )
				return XST_FAIL;

			CVertexData& VData = pVB->GetVertexData();
			Vec3 vecPos = Vec3::ZERO;

			for( u32 z = 0; z < VertexCount.y; ++z )
			{
				for( u32 x = 0; x < VertexCount.x; ++x )
				{
					VData.SetPosition( ulVertexId, vecPos );
					vecPos.x += vecStep.x;
					ulVertexId++;
				}
				vecPos.x = 0;
				vecPos.z += vecStep.y;
			}

			if( XST_FAILED( pVB->Unlock() ) )
				return XST_FAIL;
		}

		for( u32 i = 1; i < uVBCount; ++i )
		{
			pVB = apVBs[ i ];
			XST_RET_FAIL( pVB->Lock() );
			XST_RET_FAIL( pVB->GetVertexData( ).SetData( apVBs[ 0 ]->GetVertexData() ) );
			XST_RET_FAIL( pVB->Unlock() );
		}

		return XST_OK;
	}

	i32 SetImpostorVertexDataHeights(IVertexBuffer** ppVBOut, const CPoint& VertexCount, const Vec2& vecHeightRange, const Resources::IImage* pImg)
	{
		// Image must be scaled
		if( pImg->GetWidth() < VertexCount.x || pImg->GetHeight() < VertexCount.y )
		{
			return XST_FAIL;
		}

		IVertexBuffer* pVB = ( *ppVBOut );
		CVertexData& VData = pVB->GetVertexData();
		Vec3 vecPos;
		ul32 ulVertexId = 0;

		for( u32 y = 0; y < VertexCount.y; ++y )
		{
			for( u32 x = 0; x < VertexCount.x; ++x )
			{
				u8 r = pImg->GetChannelColor( x, y, COLOR_CHANNEL::RED );
				VData.GetPosition( ulVertexId, &vecPos );
				vecPos.y = CMipMapTerrainTile::ColorToHeight( vecHeightRange, r );
				VData.SetPosition( ulVertexId++, vecPos );
			}
		}

		pVB->Update();
		return XST_OK;
	}

	void DebugPrintVB(MeshPtr& pMesh)
	{
		CVertexData& Data = pMesh->GetVertexBuffer()->GetVertexData();
		Vec3 vecPos;
		ul32 ulVertexSize = Data.GetSize();
		XST::CDebug::PrintDebugLN( XST::StringUtil::ToString( ulVertexSize ) );

		for(u32 i = 0; i < Data.GetVertexCount(); ++i)
		{
			Data.GetPosition( i, &vecPos );
			XST::CDebug::PrintDebugLN( XST::StringUtil::ToString< Vec3 >( vecPos ) );
		}
	}

	CMipMapPagingTerrain::CMipMapPagingTerrain(xst_castring& strName, IInputLayout* pIL, CSceneManager* pSceneMgr) :
		ITerrain( pIL, strName.c_str(), false ),
		_Render( &CMipMapPagingTerrain::_RenderNormal ),
		//m_strName( strName ),
		m_pSceneMgr( pSceneMgr ),
		m_bTileLocked( false )
	{
		this->m_bManualRendering = true;
	}

	CMipMapPagingTerrain::~CMipMapPagingTerrain()
	{
		DestroyData();
	}

	void CMipMapPagingTerrain::DestroyData()
	{
		m_vPages.clear();
		m_vIndexBuffers.clear();
		if( !m_vpVertexBuffers.empty() )
			m_pSceneMgr->GetRenderSystem( )->DestroyVertexBuffers( &m_vpVertexBuffers[0], m_vpVertexBuffers.size() );
		if( !m_vpImpostorVertexBuffers.empty() )
			m_pSceneMgr->GetRenderSystem( )->DestroyVertexBuffers( &m_vpImpostorVertexBuffers[ 0 ], m_vpImpostorVertexBuffers.size() );
		m_vpVertexBuffers.clear();
		m_vpImpostorVertexBuffers.clear();
	}

	void CMipMapPagingTerrain::Disable(cu32& uiReason)
	{
		this->m_uiObjDisableReason = uiReason;
	}

	xst_fi u32 CalcLOD(cu32& uiLOD, const MIPMAP_TERRAIN_STITCH_TYPE& eType)
	{
		return uiLOD * MipMapTerrainStitchTypes::_MAX_COUNT + eType;
	}

	void CMipMapPagingTerrain::Update()
	{
		CMesh* pCurrMesh;
		CMipMapTerrainTile* pCurrTile;
        f32 fConstDist = 100;
        f32 fLODDist = fConstDist;
        f32 fPrevDist;
		u32 uMaxLOD = m_Options.uiLODCount;
        CCamera* pCam = m_pSceneMgr->GetComputeCamera();

		g_vVisibleTiles.clear();
		for( u32 i = 0; i < m_vTileVisibility.size(); ++i )
		{
			if( m_vTileVisibility[ i ] )
				g_vVisibleTiles.push_back( &m_vTiles[ i ] );
		}

		// Sort from nearest from the camera
		std::sort( g_vVisibleTiles.begin(), g_vVisibleTiles.end(), [] (const CMipMapTerrainTile* l, const CMipMapTerrainTile* r){ return l->GetDistanceToCamera() < r->GetDisableReason();  } );

		for( u32 i = 0; i < m_vPages.size(); ++i )
		{
			m_vPages[ i ].Update( pCam );
		}

		//for( u32 t = 0; t < g_vVisibleTiles.size(); ++t )
		for( u32 t = 0; t < m_vTiles.size(); ++t )
		{
			//auto& Tile = g_vVisibleTiles[ t ];
			auto* Tile = &m_vTiles[ t ];
			f32 fDist = Tile->GetDistanceToCamera();
			Tile->m_uiLOD = uMaxLOD-1;
			for( u32 i = uMaxLOD; i-->0; )
			{
				if( fDist < fConstDist * ( i + 1 ) )
				{
					Tile->m_uiLOD = i;
					Tile->m_eStitchType = MipMapTerrainStitchTypes::NONE;
				}
			}
		}

		for( u32 i = 0; i < m_vTileVisibility.size(); ++i )
		{
			if( !m_vTileVisibility[ i ] )
				continue;

			CMipMapTerrainTile* pTile = &m_vTiles[ i ];
			u32 uTileId;
			const CPoint& Pos = pTile->m_Info.TilePart;
			bool bNeedLeft = false, bNeedRight = false, bNeedUp = false, bNeedDown = false;
			if( Pos.x > 0 )
			{
				//uTileId = XST_ARRAY_2D_TO_1D( pTile->m_Info.TilePart.x - 1, pTile->m_Info.TilePart.y, m_TileCount.x * m_Options.PageCount.x );
				CMipMapTerrainTile* pLeft = g_vTileGrid[ Pos.y ][ Pos.x - 1 ];
				bNeedLeft = pLeft->GetLOD() > pTile->GetLOD();
			}
			//else
			if( Pos.x < m_TileCount.x-1 )
			{
				//uTileId = XST_ARRAY_2D_TO_1D( pTile->m_Info.TilePart.x + 1, pTile->m_Info.TilePart.y, m_TileCount.x * m_Options.PageCount.x );
				CMipMapTerrainTile* pRight = g_vTileGrid[ Pos.y ][ Pos.x + 1 ];
				bNeedRight = pRight->GetLOD() > pTile->GetLOD();
			}

			if( Pos.y > 0 )
			{
				//uTileId = XST_ARRAY_2D_TO_1D( pTile->m_Info.TilePart.x, pTile->m_Info.TilePart.y - 1, m_TileCount.x * m_Options.PageCount.x );
				CMipMapTerrainTile* pUp = g_vTileGrid[ Pos.y - 1 ][ Pos.x ];
				bNeedUp = pUp->GetLOD() > pTile->GetLOD();
			}
			//else
			if( Pos.y < m_TileCount.y-1 )
			{
				//uTileId = XST_ARRAY_2D_TO_1D( pTile->m_Info.TilePart.x, pTile->m_Info.TilePart.y + 1, m_TileCount.x * m_Options.PageCount.x );
				CMipMapTerrainTile* pDown = g_vTileGrid[ Pos.y + 1 ][ Pos.x ];
				bNeedDown = pDown->GetLOD() > pTile->GetLOD();
			}

			MIPMAP_TERRAIN_STITCH_TYPE eStitchType = MipMapTerrainStitchTypes::NONE;
			if( bNeedLeft && bNeedUp )
			{
				eStitchType = MipMapTerrainStitchTypes::LEFT_UP;
			}
			else if( bNeedLeft && bNeedDown )
			{
				eStitchType = MipMapTerrainStitchTypes::LEFT_DOWN;
			}
			else if( bNeedRight && bNeedDown )
			{
				eStitchType = MipMapTerrainStitchTypes::RIGHT_DOWN;
			}
			else if( bNeedRight && bNeedUp )
			{
				eStitchType = MipMapTerrainStitchTypes::RIGHT_UP;
			}
			else if( bNeedUp )
			{
				eStitchType = MipMapTerrainStitchTypes::UP;
			}
			else if( bNeedDown )
			{
				eStitchType = MipMapTerrainStitchTypes::DOWN;
			}
			else if( bNeedLeft )
			{
				eStitchType = MipMapTerrainStitchTypes::LEFT;
			}
			else if( bNeedRight )
			{
				eStitchType = MipMapTerrainStitchTypes::RIGHT;
			}

			pTile->SetLOD( pTile->GetLOD(), eStitchType );
		}
	}

	i32 CMipMapPagingTerrain::Init(const STerrainOptions& Options)
	{
		m_Options = Options;
		
		ValidateOptions( &m_Options );

		cu32 uPageCount = m_Options.PageCount.x * m_Options.PageCount.y;
		m_TileCount = CPoint( ceilf( (f32)(m_Options.PageVertexCount.x-1) / (m_Options.TileVertexCount.x-1) ), 
							  ceilf( (f32)(m_Options.PageVertexCount.y-1) / (m_Options.TileVertexCount.y-1) ) );
		m_vTiles.resize( ( m_TileCount.x * m_TileCount.y ) * uPageCount );
		m_vTileVisibility.resize( m_vTiles.size(), false );
		m_vPageVisibility.resize( uPageCount, false );
		m_vpVertexBuffers.resize( uPageCount );
		m_vpImpostorVertexBuffers.resize( uPageCount );

		// Create tile grid
		g_vTileGrid.resize( m_TileCount.y * m_Options.PageCount.y );
		std::for_each( g_vTileGrid.begin(), g_vTileGrid.end(), [&] (TileVec& v){ v.resize( m_TileCount.x * m_Options.PageCount.x, xst_null ); } );

		// Set visible pointers
		for( u32 i = 0; i < m_vTiles.size(); ++i )
		{
			auto it = m_vTileVisibility.begin() + i;
			m_vTiles[ i ].m_pbIsVisible = it;
		}

		this->m_pMaterial = CMaterialManager::GetSingletonPtr( )->GetDefaultMaterial();

		//Load heightmap images
		if( XST_FAILED( LoadImages( Options.vHeightmaps ) ) )
		{
			return XST_FAIL;
		}

		if( XST_FAILED( CreatePages() ) )
		{
			return XST_FAIL;
		}

		return XST_OK;
	}

	static xst_fi u32 CalcFirstTileIdForPage(cu32& uPageId, cu32& uTileCountForPage)
	{
		u32 uResult = (uPageId) * uTileCountForPage;
		return uResult;
	}

	i32 CMipMapPagingTerrain::CreatePages()
	{
		if( m_vPages.size() > 0 )
			return XST_FAIL;
		//Create pages
		//u32 uiPageCount = m_Options.PageCount.x * m_Options.PageCount.y;
		const Vec3 vecRightTopCorner = ( -m_Options.Size.x * 0.5f, 0.0f, -m_Options.Size.y * 0.5f );
		Vec3 vecPos = vecRightTopCorner;
		const Vec2 vecPageSize( m_Options.Size.x / m_Options.PageCount.x, m_Options.Size.y / m_Options.PageCount.y );
		u32 uPageCount = m_Options.PageCount.x * m_Options.PageCount.y;
		m_vPages.resize( uPageCount, CMipMapTerrainPage( this ) );
		u32 uTileCount = (m_TileCount.x * m_TileCount.y);
		
		if( XST_FAILED( m_pSceneMgr->GetRenderSystem()->CreateVertexBuffers( &m_vpVertexBuffers[ 0 ], m_vpVertexBuffers.size() ) ) )
		{
			return XST_FAIL;
		}

		if( XST_FAILED( m_pSceneMgr->GetRenderSystem()->CreateVertexBuffers( &m_vpImpostorVertexBuffers[ 0 ], m_vpImpostorVertexBuffers.size() ) ) )
		{
			return XST_FAIL;
		}

		const CPoint ImpVertCount = CalcImpostorVertexCount( m_Options.PageVertexCount, m_Options.uiLODCount );
		IVertexBuffer* *const pTmpVBs = &m_vpImpostorVertexBuffers[ 0 ];
		XST_RET_FAIL( CreateImpostorVertexBufferData( &pTmpVBs, m_vpImpostorVertexBuffers.size(), ImpVertCount, vecPageSize, m_pInputLayout ) );
		XST_RET_FAIL( SetImpostorVertexDataHeights( &(pTmpVBs[0]), ImpVertCount, m_Options.vecHeightRange, m_vpImages[1].GetPtr() ) );

		for(u32 y = 0; y < m_Options.PageCount.y; ++y)
		{
			vecPos.x = vecRightTopCorner.x;

			for( u32 x = 0; x < m_Options.PageCount.x; ++x )
			{
				u32 uCurrPageId = XST_ARRAY_2D_TO_1D( x, y, m_Options.PageCount.x );
				CMipMapTerrainPage* pPage = &m_vPages[ uCurrPageId ];
				CMipMapTerrainPage::SInfo Info;
				
				Info.vecPageSize = vecPageSize;
				Info.TileCount = m_TileCount;
				Info.pInputLayout = m_pInputLayout;
				Info.vecHeightRange = m_Options.vecHeightRange;
				Info.vecPagePosition = vecPos;
				Info.VertexCount = m_Options.PageVertexCount;
				Info.TileVertexCount = m_Options.TileVertexCount;
				Info.pImg = m_vpImages[0].GetPtr(); // TEMP use only one heightmap at this moment
				Info.uPageId = m_vPages.size() - 1;
				Info.ImgPixelStartPosition = CPoint( x, y );
				Info.GridPosition = CPoint( x, y );
				//Info.apTiles = &m_vTiles[ CalcFirstTileIdForPage( uCurrPageId, uTileCount ) ];
				u32 uTileId = CalcFirstTileIdForPage( uCurrPageId, uTileCount );
				xst_assert2( uTileId < m_vTiles.size() );
				Info.aTiles = &m_vTiles[ uTileId ];
				Info.uTileCount = uTileCount;
				Info.pVB = m_vpVertexBuffers[ uCurrPageId ];
				Info.pImpVB = m_vpImpostorVertexBuffers[ uCurrPageId ];
				Info.ImpostorVertexCount = ImpVertCount;
				Info.pImpImg = m_vpImages[1].GetPtr(); // TEMP
				
				XST_RET_FAIL( pPage->Init( Info ) );

				vecPos.x += vecPageSize.x;
			}

			vecPos.z += vecPageSize.y;
		}
		
		return XST_OK;
	}

	i32 CMipMapPagingTerrain::LoadImages(const StringVector& vHeightmapNames)
	{
		CImageManager* pImgMgr = CImageManager::GetSingletonPtr();
		xst_castring strImgGroup( DEFAULT_GROUP );
		m_vpImages.clear();

		for(u32 i = 0; i < vHeightmapNames.size(); ++i)
		{
            ImageWeakPtr pImg = pImgMgr->CreateResource( vHeightmapNames[ i ] );
			if( pImg == xst_null )
			{
				return XST_FAIL;
			}

			if( XST_FAILED( pImgMgr->LoadResource( pImg ) ) )
			{
				return XST_FAIL;
			}

			if( XST_FAILED( pImgMgr->PrepareResource( pImg ) ) )
			{
				return XST_FAIL;
			}

			ImageWeakPtr pImpImg = pImgMgr->CloneResource( pImg, vHeightmapNames[ i ] + "/impostor" );

			//If image has more pixels than terrain, resize it
			u32 uiHeight = m_Options.PageVertexCount.y; //Math::Max( (u32)m_Options.PageVertexCount.y, pImg->GetHeight() ) - m_TileCount.y + 1; //leave first tile
			u32 uiWidth = m_Options.PageVertexCount.x; //Math::Max( (u32)m_Options.PageVertexCount.x, pImg->GetWidth() ) - m_TileCount.x + 1;
			pImg->Scale( uiWidth, uiHeight );
			m_vpImages.push_back( pImg );
			
			CPoint VC = CalcImpostorVertexCount( m_Options.PageVertexCount, m_Options.uiLODCount );
			pImpImg->Scale( VC.x, VC.y );
			m_vpImages.push_back( pImpImg );
		}
		return XST_OK;
	}

	/*CMipMapTerrainTile* CMipMapPagingTerrain::CreateTile()
	{
		//CMipMapTerrainTile* pTile = xst_new CMipMapTerrainTile();
		u32 uiId = m_vTiles.size();
		CMipMapTerrainTile* pTile = &m_vTilePool[ uiId ];
        pTile->SetPoolId( uiId );
		m_vTiles.push_back( pTile );
        
		return pTile;
	}*/

	void CMipMapPagingTerrain::SetVisible(bool bVisible)
	{
		if( bVisible && !this->IsDisabled() )
		{
			_Render = &CMipMapPagingTerrain::_RenderNormal;
		}
		else
		{
			_Render = &CMipMapPagingTerrain::_RenderEmpty;
		}
	}

	void CMipMapPagingTerrain::_RenderNormal(XSE::IRenderSystem *pRS)
	{
		IIndexBuffer* pIB = m_vIndexBuffers[0].pIndexBuffer.GetPtr();
		Mtx4 mtxTransform = Mtx4::IDENTITY;

		pRS->SetInputLayoutWithCheck( m_pInputLayout );
		pRS->SetTopology( TopologyTypes::TRIANGLE_LIST );

		CMaterial* pMat = m_pMaterial.GetPtr();
		ITechnique* pTech = pMat->GetCurrentTechnique();
		IPass* pPass;
		IVertexShader* pVS;
		IPixelShader* pPS;
		for(u32 i = 0; i < pTech->GetPassCount(); ++i)
		{
			pPass = pTech->GetPass( i );
			pVS = pPass->GetVertexShader().GetPtr();
			pPS = pPass->GetPixelShader().GetPtr();

			//Set shaders
			pRS->SetVertexShaderWithCheck( pVS );
			pRS->SetPixelShaderWithCheck( pPS );
				
			//Do transformations
			pRS->SetMatrix( MatrixTypes::WORLD, mtxTransform );
			//Update shaders input
			pRS->UpdateObjectInputs();
				
			//Draw object
			/*for( u32 t = 0; t < m_vTiles.size(); ++t )
			{
				if( m_vTileVisibility[t] )
				{
					// TODO: probably cache miss here. Use array of pVB,ulStartVertex structures
					const auto& Tile = m_vTiles[ t ];
					const auto& TileInfo = Tile.m_Info;
					pIB = GetIndexBuffer( Tile.m_uiLOD, Tile.m_eStitchType ).pIndexBuffer.GetPtr();
					//pIB = GetIndexBuffer(0, 1 ).pIndexBuffer.GetPtr();
					lpcastr n = pIB->_GetDbgName();
					pRS->SetVertexBufferWithCheck( TileInfo.pVB );
					pRS->SetIndexBufferWithCheck( pIB );
					pRS->DrawIndexed( pIB->GetIndexCount(), 0, TileInfo.ulStartVertex );
				}
			}*/
			for( u32 t = 0; t < g_vVisibleTiles.size(); ++t )
			{
				auto* Tile = g_vVisibleTiles[ t ];
				// TODO: probably cache miss here. Use array of pVB,ulStartVertex structures
				const auto& TileInfo = Tile->m_Info;
				pIB = GetIndexBuffer( Tile->m_uiLOD, Tile->m_eStitchType ).pIndexBuffer.GetPtr();
				//lpcastr n = pIB->_GetDbgName();
				pRS->SetVertexBufferWithCheck( TileInfo.pVB );
				pRS->SetIndexBufferWithCheck( pIB );
				pRS->DrawIndexed( pIB->GetIndexCount(), 0, TileInfo.ulStartVertex );
			}
		}
	}

	void CMipMapPagingTerrain::SetMaterial(MaterialPtr pMat)
	{
		ITerrain::SetMaterial( pMat );
	}


	void CMipMapPagingTerrain::SetTileInfo(CMipMapTerrainTile::SInfo* pInfoOut)
	{
	}

	i32 CMipMapPagingTerrain::CreateVertexBuffers()
	{
		XSTSimpleProfiler();
		for( auto& Page : m_vPages )
		{
			XST_RET_FAIL( Page.CreateVertexBuffer() );
		}
		
		return XST_OK;
	}

	i32 CMipMapPagingTerrain::LockVertexBuffers()
	{
		XSTSimpleProfiler();
		for( auto& Page : m_vPages )
		{
			XST_RET_FAIL( Page.LockVertexBuffer() );
		}
		return XST_OK;
	}

	i32 CMipMapPagingTerrain::UnlockVertexBuffers()
	{
		XSTSimpleProfiler();
		for( auto& Page : m_vPages )
		{
			if( XST_FAILED( Page.UnlockVertexBuffer() ) )
			{
				return XST_FAIL;
			}
		}
		return XST_OK;
	}

	void CMipMapPagingTerrain::DestroyVertexBuffers()
	{
		for( auto& Page : m_vPages )
		{
			Page.DestroyVertexBuffer();
		}
	}

	i32 CMipMapPagingTerrain::CalcVertexData()
	{
		XSTSimpleProfiler();
		for( auto& Page : m_vPages )
		{
			Page.CalcVertexPositions();
			Page.CalcVertexNormals();
			if( XST_FAILED( Page.FillVertexBuffer() ) )
			{
				return XST_FAIL;
			}

			// Update grid
			for( u32 i = 0; i < Page.m_Info.uTileCount; ++i )
			{
				CMipMapTerrainTile& Tile = Page.m_Info.aTiles[ i ];
				//u32 uTileId = XST_ARRAY_2D_TO_1D( Tile.m_Info.TilePart.x, Tile.m_Info.TilePart.y, m_TileCount.x * m_Options.PageCount.x ); 
				//u32 uSize = g_vTileGrid.size();
				//g_vTileGrid[ uTileId ] = &Tile;
				g_vTileGrid[ Tile.m_Info.TilePart.y ][ Tile.m_Info.TilePart.x ] = &Tile;
			}
			
		}

		// After vertex positions are calculated
		// All tile bounding boxes are set
		// Add tile objects to scene partition system
		u32 uTileCount = m_TileCount.x * m_TileCount.y;
		for( u32 i = uTileCount; i-- > 0; )
		{
			CMipMapTerrainTile* pTile = &m_vTiles[ i ];
			m_pSceneMgr->AddToPartitionSystem( pTile );
			
			/*SLineBoxOptions o;
			o.vecPos = m_vTiles[ i ].GetPosition();
			o.vecSize = m_vTiles[ i ].GetBoundingVolume( ).GetAABB( ).CalcSize();
			o.colColor = CColor::GREEN;
			char name[128];
			xst_sprintf( name, sizeof( name ), "tile%d_aabb", i );
			MeshPtr pM = CMeshManager::GetSingletonPtr( )->CreateMesh( name, ILEs::POSITION | ILEs::COLOR, BasicShapes::LINE_BOX, &o, "terrain" );
			m_pSceneMgr->GetRootNode( )->AddObject( pM );*/
		}

		return XST_OK;
	}

    i32 CMipMapPagingTerrain::CreateImpostors()
    {
        return XST_OK;
    }

	void CMipMapPagingTerrain::DestroyIndexBuffers()
	{
		IBVec::iterator Itr;
		xst_stl_foreach( Itr, m_vIndexBuffers )
		{
			//xst_release( (*Itr).pIndexBuffer );
			( *Itr ).pIndexBuffer.Release();
		}

		xst_vector_clear( m_vIndexBuffers, IBVec::value_type );

		m_pImpostorIB->Release();
	}

	i32	CMipMapPagingTerrain::CreateIndexBuffers()
	{
		XSTSimpleProfiler();
		if( m_pImpostorIB.IsValid() || m_vIndexBuffers.size() > 0 )
		{
			return XST_OK;
		}

		const u32 uiIBCount = m_Options.uiLODCount * MipMapTerrainStitchTypes::_MAX_COUNT;

		m_vIndexBuffers.reserve( uiIBCount );
		ch8 tmp[32];
		for(u32 l = 0; l < m_Options.uiLODCount; ++l)
		{
			for(u32 s = 0; s < MipMapTerrainStitchTypes::_MAX_COUNT; ++s)
			{
				u16 usIndexCount = CalcIndexCount( l );
				IndexBufferPtr pIB = IndexBufferPtr( m_pSceneMgr->GetRenderSystem()->CreateIndexBuffer() );
				xst_sprintf( tmp, 32, "terr_%d_%d", l, s );
				pIB->_SetDbgName( tmp );
				pIB->SetUsage( BufferUsages::STATIC );
				pIB->SetIndexCount( usIndexCount );
				SMipMapIndexBuffer IB = { (MIPMAP_TERRAIN_STITCH_TYPE)s, pIB, l };
				m_vIndexBuffers.push_back( IB );
			}
		}

		m_pImpostorIB = IndexBufferPtr( m_pSceneMgr->GetRenderSystem( )->CreateIndexBuffer() );
		if( m_pImpostorIB.IsNull() )
		{
			return XST_FAIL;
		}

		m_pImpostorIB->SetUsage( BufferUsages::STATIC );
		CPoint VC = CalcImpostorVertexCount( m_Options.PageVertexCount, m_Options.uiLODCount );
		u32 usIndexCount = ( ( VC.x-1 ) * ( VC.y-1 ) * 2 ) * 3; // triangle count * 3 vertices per each for triangle list
		m_pImpostorIB->SetIndexCount( usIndexCount );

		return XST_OK;
	}

	u16 CMipMapPagingTerrain::CalcTriangleCount(u32 uiLOD)
	{
		//lod0 = 1
		//lod1 = 2
		//lod2 = 4
		//lod3 = 8
		//lod4 = 16
		//lodn = 2^( uiLOD )
		uiLOD = CalcLODStep( uiLOD );
		cu16 usCount = ( ( m_Options.TileVertexCount.x - 1 ) / uiLOD ) * ( ( m_Options.TileVertexCount.y - 1 ) / uiLOD ) * 2;
		return usCount;
	}

	u16 CMipMapPagingTerrain::CalcIndexCount(u32 uiLOD)
	{
		cu16 usTriCount = CalcTriangleCount( uiLOD );
		return usTriCount * 3;
	}

	u32 CMipMapPagingTerrain::CalcLODStep(u32 uiLOD)
	{
		//uiLOD = pow< u32, u32 >( 2, uiLOD );
		uiLOD = pow( 2.0, (f64)uiLOD );
		return uiLOD;
	}

	i32	CMipMapPagingTerrain::LockIndexBuffers()
	{
		XSTSimpleProfiler();
		IBVec::iterator Itr;
		for( auto& Data : m_vIndexBuffers )
		{
			if( XST_FAILED( Data.pIndexBuffer->Lock() ) )
			{
				return XST_FAIL;
			}
		}
		XST_RET_FAIL( m_pImpostorIB->Lock() );
		return XST_OK;
	}

	/*i32 CMipMapPagingTerrain::CalcVertexNormalData()
	{
		if( !m_Options.bNormal )
			return XST_OK;

		XSTSimpleProfiler();
		// Get index data for default lod 0
		const IIndexBuffer* pIB = GetIndexBuffer( 0, MipMapTerrainStitchTypes::NONE ).pIndexBuffer.GetPtr();
		xst_assert( pIB, "(CMipMapPagingTerrain::CalcVertexNormalData) Index buffer not created" );
		const CIndexData& IData = pIB->GetIndexData();
		cu32 uTriCount = IData.GetTriangleCount();
		
		// One normal per each vertex
		m_vTerrNormals.clear();
		ul32 ulCount = m_Options.PageVertexCount.x * m_Options.PageVertexCount.y;
		m_vTerrNormals.resize( ulCount, Vec3::ZERO );

		u32 auTriangle[3];
		Vec3 avecTriangleVertices[3];
		CColor aColors[3];
		int auRs[4];
		Vec3 avecTriLeft[3];
		Vec3 avecTriRight[3];
		Vec3 vecU, vecV;
		Vec3 avecNormals[2];

		for( auto& pPage : m_vPages )
		{
			const IImage* pImg = pPage->GetImage();
			// For each vertex/texel
			for( u32 y = 0; y < pImg->GetHeight() - 1; y += 1 )
			{
				for( u32 x = 0; x < pImg->GetWidth() - 1; x += 1 )
				{
					auRs[ 0 ] = pImg->GetChannelColor( x, y, IImage::CHANNEL::RED ); // top left
					auRs[ 1 ] = pImg->GetChannelColor( x, y+1, IImage::CHANNEL::RED ); // bottom left
					auRs[ 2 ] = pImg->GetChannelColor( x+1, y, IImage::CHANNEL::RED ); // top right
					auRs[ 3 ] = pImg->GetChannelColor( x+1, y+1, IImage::CHANNEL::RED ); // bottom right
					// Create quad
					// Left triangle |\.
					avecTriLeft[ 0 ].Set( x, CMipMapTerrainTile::ColorToHeight( m_Options.vecHeightRange, auRs[0] ), y ); // top left
					avecTriLeft[ 1 ].Set( x, CMipMapTerrainTile::ColorToHeight( m_Options.vecHeightRange, auRs[1] ), y + 1 ); // bottom left
					avecTriLeft[ 2 ].Set( x + 1, CMipMapTerrainTile::ColorToHeight( m_Options.vecHeightRange, auRs[3] ), y + 1 ); // botton right

					vecU = avecTriLeft[ 1 ];
					vecV = avecTriLeft[ 2 ];
					vecU.Sub( avecTriLeft[ 0 ] );
					vecV.Sub( avecTriLeft[ 0 ] );
					avecNormals[ 0 ].x = ( vecU.y * vecV.z - vecU.z * vecV.y );
					avecNormals[ 0 ].y = ( vecU.z * vecV.x - vecU.x * vecV.z );
					avecNormals[ 0 ].z = ( vecU.x * vecV.y - vecU.y * vecV.x );
					m_vTerrNormals[XST_ARRAY_2D_TO_1D(x, y, m_Options.PageVertexCount.x)] += avecNormals[0];
					m_vTerrNormals[XST_ARRAY_2D_TO_1D(x, y+1, m_Options.PageVertexCount.x)] += avecNormals[0];
					m_vTerrNormals[XST_ARRAY_2D_TO_1D(x+1, y+1, m_Options.PageVertexCount.x)] += avecNormals[0];

					// Right triangle \|
					avecTriRight[ 0 ].Set( x, CMipMapTerrainTile::ColorToHeight( m_Options.vecHeightRange, auRs[0] ), y ); // top left
					avecTriRight[ 1 ].Set( x + 1, CMipMapTerrainTile::ColorToHeight( m_Options.vecHeightRange, auRs[3] ), y + 1 ); // bottom right
					avecTriRight[ 2 ].Set( x + 1, CMipMapTerrainTile::ColorToHeight( m_Options.vecHeightRange, auRs[2] ), y ); // top right

					vecU = avecTriRight[ 1 ];
					vecV = avecTriRight[ 2 ];
					vecU.Sub( avecTriRight[ 0 ] );
					vecV.Sub( avecTriRight[ 0 ] );
					avecNormals[ 1 ].x = ( vecU.y * vecV.z - vecU.z * vecV.y );
					avecNormals[ 1 ].y = ( vecU.z * vecV.x - vecU.x * vecV.z );
					avecNormals[ 1 ].z = ( vecU.x * vecV.y - vecU.y * vecV.x );
					m_vTerrNormals[XST_ARRAY_2D_TO_1D(x, y, m_Options.PageVertexCount.x)] += avecNormals[1];
					m_vTerrNormals[XST_ARRAY_2D_TO_1D(x+1, y+1, m_Options.PageVertexCount.x)] += avecNormals[1];
					m_vTerrNormals[XST_ARRAY_2D_TO_1D(x+1, y, m_Options.PageVertexCount.x)] += avecNormals[1];

					//CDebug::PrintDebugLN( ToStr( avecNormals[0].a, 3 ).Array( avecNormals[1].a, 3 ) );
				}
			}

			for( u32 i = 0; i < m_vTerrNormals.size(); ++i )
			{	
				m_vTerrNormals[ i ].Normalize();
				//CDebug::PrintDebugLN( ToStr( vecNormal.a, 3 ) );
			}
			
		}

		return XST_OK;
	}*/

	i32	CMipMapPagingTerrain::CalcIndexBufferData(u32 uiLOD)
	{
		cu32 uiIBID = GetIndexBuffersID( uiLOD );
		if( g_bCCW )
		{
			_CalcIBStitchDownCCW( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::DOWN ].pIndexBuffer );
			_CalcIBStitchDownLeftCCW( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::DOWN_LEFT ].pIndexBuffer );
			_CalcIBStitchLeftCCW( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::LEFT ].pIndexBuffer );
			_CalcIBStitchNoneCCW( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::NONE ].pIndexBuffer );
			_CalcIBStitchRightCCW( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::RIGHT ].pIndexBuffer );
			_CalcIBStitchRightDownCCW( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::RIGHT_DOWN ].pIndexBuffer );
			_CalcIBStitchUpCCW( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::UP ].pIndexBuffer );
			_CalcIBStitchUpLeftCCW( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::UP_LEFT ].pIndexBuffer );
			_CalcIBStitchUpRightCCW( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::UP_RIGHT ].pIndexBuffer );
		}
		else
		{
			_CalcIBStitchDownCW( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::DOWN ].pIndexBuffer );
			_CalcIBStitchDownLeftCW( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::DOWN_LEFT ].pIndexBuffer );
			_CalcIBStitchLeftCW( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::LEFT ].pIndexBuffer );
			_CalcIBStitchNoneCW( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::NONE ].pIndexBuffer );
			_CalcIBStitchRightCW( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::RIGHT ].pIndexBuffer );
			_CalcIBStitchRightDownCW( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::RIGHT_DOWN ].pIndexBuffer );
			_CalcIBStitchUpCW( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::UP ].pIndexBuffer );
			_CalcIBStitchUpLeftCW( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::UP_LEFT ].pIndexBuffer );
			_CalcIBStitchUpRightCW( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::UP_RIGHT ].pIndexBuffer );
		}
	
		return XST_OK;
	}

	#define LEFT_UP		0
	#define RIGHT_UP	1
	#define RIGHT_DOWN	2
	#define LEFT_DOWN	3


	i32 CalcImpostorIndexBufferData(IIndexBuffer* pIB, const CPoint& PageVertexCount, u32 uiLODCount)
	{
		CPoint VC = CalcImpostorVertexCount( PageVertexCount, uiLODCount );
		bool bBackslashTriangle = true;
		u32 uiCurrTri = 0;
		u32 uWidth = VC.x-1;
		u32 uHeight = VC.y-1;

		CIndexData& IData = pIB->GetIndexData();

		if( g_bCCW )
		{
			for( u32 y = 0; y < uHeight; ++y )
			{
				bBackslashTriangle = y % 2 == 0;
				for( u32 x = 0; x < uWidth; ++x )
				{
					cu32 uiIds[ 4 ] = 
					{
						XST_ARRAY_2D_TO_1D( x, y, VC.x ), //LEFT_UP
						XST_ARRAY_2D_TO_1D( x + 1, y, VC.x ), //RIGHT_UP
						XST_ARRAY_2D_TO_1D( x + 1, y + 1, VC.x ), //RIGHT_DOWN
						XST_ARRAY_2D_TO_1D( x, y + 1, VC.x ) //LEFT_DOWN
					};

					if( bBackslashTriangle )
					{
						//Set the backslash triangle 
						/* |\ */
						//CW
						//pData->SetTriangle( uiCurrTri++, uiIds[ LEFT_DOWN ], uiIds[ LEFT_UP ], uiIds[ RIGHT_DOWN ] );
						//CCW
						IData.SetTriangle( uiCurrTri++, uiIds[ LEFT_UP ], uiIds[ LEFT_DOWN ], uiIds[ RIGHT_DOWN ] );
						//The second part 
						/* \| */
						//CW
						//pData->SetTriangle( uiCurrTri++, uiIds[ LEFT_UP ], uiIds[ RIGHT_UP ], uiIds[ RIGHT_DOWN ] );
						//CCW
						IData.SetTriangle( uiCurrTri++, uiIds[ RIGHT_UP ], uiIds[ LEFT_UP ], uiIds[ RIGHT_DOWN ] );
					}
					else
					{
						//Set the slash triangle 
						/* |/ */
						//CW
						//pData->SetTriangle( uiCurrTri++, uiIds[ LEFT_DOWN ], uiIds[ LEFT_UP ], uiIds[ RIGHT_UP ] );
						//CCW
						IData.SetTriangle( uiCurrTri++, uiIds[ LEFT_UP ], uiIds[ LEFT_DOWN ], uiIds[ RIGHT_UP ] );
						//The second part 
						/* /| */
						//CW
						//pData->SetTriangle( uiCurrTri++, uiIds[ RIGHT_UP ], uiIds[ RIGHT_DOWN ], uiIds[ LEFT_DOWN ] );
						//CCW
						IData.SetTriangle( uiCurrTri++, uiIds[ RIGHT_DOWN ], uiIds[ RIGHT_UP ], uiIds[ LEFT_DOWN ] );
					}

					bBackslashTriangle = !bBackslashTriangle;
				}
			}
		}
		else
		{
			for( u32 y = 0; y < uHeight; ++y )
			{
				bBackslashTriangle = y % 2 == 0;
				for( u32 x = 0; x < uWidth; ++x )
				{
					cu32 uiIds[ 4 ] = 
					{
						XST_ARRAY_2D_TO_1D( x, y, uWidth ), //LEFT_UP
						XST_ARRAY_2D_TO_1D( x + 1, y, uWidth ), //RIGHT_UP
						XST_ARRAY_2D_TO_1D( x + 1, y + 1, uWidth ), //RIGHT_DOWN
						XST_ARRAY_2D_TO_1D( x, y + 1, uWidth ) //LEFT_DOWN
					};

					if( bBackslashTriangle )
					{
						//Set the backslash triangle 
						/* |\ */
						//CW
						IData.SetTriangle( uiCurrTri++, uiIds[ LEFT_DOWN ], uiIds[ LEFT_UP ], uiIds[ RIGHT_DOWN ] );
						//CCW
						//IData.SetTriangle( uiCurrTri++, uiIds[ LEFT_UP ], uiIds[ LEFT_DOWN ], uiIds[ RIGHT_DOWN ] );
						//The second part 
						/* \| */
						//CW
						IData.SetTriangle( uiCurrTri++, uiIds[ LEFT_UP ], uiIds[ RIGHT_UP ], uiIds[ RIGHT_DOWN ] );
						//CCW
						//IData.SetTriangle( uiCurrTri++, uiIds[ RIGHT_UP ], uiIds[ LEFT_UP ], uiIds[ RIGHT_DOWN ] );
					}
					else
					{
						//Set the slash triangle 
						/* |/ */
						//CW
						IData.SetTriangle( uiCurrTri++, uiIds[ LEFT_DOWN ], uiIds[ LEFT_UP ], uiIds[ RIGHT_UP ] );
						//CCW
						//IData.SetTriangle( uiCurrTri++, uiIds[ LEFT_UP ], uiIds[ LEFT_DOWN ], uiIds[ RIGHT_UP ] );
						//The second part 
						/* /| */
						//CW
						IData.SetTriangle( uiCurrTri++, uiIds[ RIGHT_UP ], uiIds[ RIGHT_DOWN ], uiIds[ LEFT_DOWN ] );
						//CCW
						//IData.SetTriangle( uiCurrTri++, uiIds[ RIGHT_DOWN ], uiIds[ RIGHT_UP ], uiIds[ LEFT_DOWN ] );
					}

					bBackslashTriangle = !bBackslashTriangle;
				}
			}
		}

		return XST_OK;
	}

	i32	CMipMapPagingTerrain::CalcIndexBufferData()
	{
		XSTSimpleProfiler();
		for(u32 i = 0; i < m_Options.uiLODCount; ++i)
		{
			if( XST_FAILED( CalcIndexBufferData( i ) ) )
			{
				return XST_FAIL;
			}
		}
		XST_RET_FAIL( CalcImpostorIndexBufferData( m_pImpostorIB.GetPtr(), m_Options.PageVertexCount, m_Options.uiLODCount ) );
		return XST_OK;
	}

	i32	CMipMapPagingTerrain::UnlockIndexBuffers()
	{
		XSTSimpleProfiler();
		IBVec::iterator Itr;
		u32 i = 0;
		for( auto& Data : m_vIndexBuffers )
		{
			if( XST_FAILED( Data.pIndexBuffer->Unlock() ) )
			{
				return XST_FAIL;
			}
			++i;
		}
		XST_RET_FAIL( m_pImpostorIB->Unlock() );
		return XST_OK;
	}

	i32 CMipMapPagingTerrain::CreateTilesLODs()
	{
		{
			//XSTSimpleProfiler(); //~32sec in debug for 31x31 tiles

		/*const int iMaxLODCount = m_Options.uiLODCount * MipMapTerrainStitchTypes::_MAX_COUNT;
		SMeshLOD LOD;
		int iCurrLOD = 0;
		CMipMapTerrainTile* pCurrTile;
		CMesh* pCurrMesh;

		for(u32 t = 0; t < m_vTiles.size(); ++t)
		{
			pCurrTile = m_vTiles[t];
			pCurrMesh = pCurrTile->m_pMesh.GetPtr();
            pCurrMesh->SetLODCount( iMaxLODCount );
			const SMeshLOD* pFirstLOD = pCurrMesh->GetLOD( 0 );
			//pCurrMesh->SetLODCount( iMaxLODCount );
			iCurrLOD = 0;

			for(u32 i = 0; i < m_Options.uiLODCount; ++i)
			{
				for(u32 j = 0; j < MipMapTerrainStitchTypes::_MAX_COUNT; ++j)
				{
					SMipMapIndexBuffer IBuffer = GetIndexBuffer( i, j );
					if( j == 0 && i == 0 ) //first, default lod
					{
						pCurrMesh->SetIndexBuffer( IBuffer.pIndexBuffer );
					}
					else
					{
						// LOD levels are created in a batch it is need to fill them properly now
						SMeshLOD* pLOD = pCurrMesh->GetLOD( iCurrLOD );
						pLOD->pIndexBuffer = IBuffer.pIndexBuffer;
						pLOD->pVertexBuffer = pFirstLOD->pVertexBuffer; // Empty lods was created during mesh creation now it is time to set vertex buffer for each lod
					}

					iCurrLOD++;
				}
			}

			m_vTiles[ t ]->SetLOD( 0, 0, MipMapTerrainStitchTypes::NONE );
		}*/
		}
		return XST_OK;
	}

	void CMipMapPagingTerrain::SetLOD(u32 uiLOD)
	{
	}

	i32 CMipMapPagingTerrain::ValidateOptions(STerrainOptions* pOptionsOut)
	{
		i32 iResult = XST_OK;

		if( pOptionsOut->TileVertexCount.x % 2 == 0 )
		{
			pOptionsOut->TileVertexCount.x += 1;
		}

		if( pOptionsOut->TileVertexCount.y % 2 == 0 )
		{
			pOptionsOut->TileVertexCount.y += 1;
		}

		if( pOptionsOut->PageVertexCount.x % 2 == 0 )
		{
			pOptionsOut->TileVertexCount.x += 1;
		}

		if( pOptionsOut->PageVertexCount.y % 2 == 0 )
		{
			pOptionsOut->TileVertexCount.y += 1;
		}

		CPoint CurrVertexCount = pOptionsOut->PageVertexCount - 1;
		cu32 uiLODCount = pOptionsOut->uiLODCount;
		for( u32 i = 0; i <= uiLODCount; ++i )
		{
			CurrVertexCount /= 2;
			if( CurrVertexCount.x < 2 || CurrVertexCount.y < 2 )
			{
				pOptionsOut->uiLODCount = i;
				break;
			}
		}

		return iResult;
	}

	void CMipMapPagingTerrain::_OnAddToRenderQueue(CRenderQueueElement* pQueue)
	{
		XSTSimpleProfiler();
	}

	void CMipMapPagingTerrain::_SetSceneNode(CSceneNode* pNode)
	{
		XSTSimpleProfiler();
		/*IRenderableObject::_SetSceneNode( pNode );
		pNode->ReserveObjects( m_vTiles.size() + 16 );
		for(u32 i = 0; i < m_vTiles.size(); ++i)
		{
			//m_vTiles[ i ]->m_pMesh->_SetSceneNode( pNode );
			pNode->AddUniqueObject( m_vTiles[ i ]->m_pMesh );
		}*/
		/*pNode->ReserveObjects( m_vPages.size() + 16 );
		for( auto& Page : m_vPages )
		{
			pNode->AddUniqueObject( Page.m_pMesh );
		}*/
	}


	void CMipMapPagingTerrain::_CalcQuadCW(CIndexData* pData, cu32& uiX, cu32& uiY, u32* puiCurrTri, cu32& uiLODStep, bool bBackslashTriangle)
	{
		u32 uiCurrTri = *puiCurrTri;
		cu32 uiWidth = m_Options.TileVertexCount.x;

		cu32 uiIds[ 4 ] = 
		{
			CALC_XY( uiX, uiY, uiWidth ), //LEFT_UP
			CALC_XY( uiX + uiLODStep, uiY, uiWidth ), //RIGHT_UP
			CALC_XY( uiX + uiLODStep, uiY + uiLODStep, uiWidth ), //RIGHT_DOWN
			CALC_XY( uiX, uiY + uiLODStep, uiWidth ) //LEFT_DOWN
		};

		/*uiIds[ LEFT_UP ]	= CALC_XY( uiX, uiY, uiWidth );
		uiIds[ LEFT_DOWN ]	= CALC_XY( uiX, uiY + uiLODStep, uiWidth );
		uiIds[ RIGHT_UP ]	= CALC_XY( uiX + uiLODStep, uiY, uiWidth );
		uiIds[ RIGHT_DOWN ] = CALC_XY( uiX + uiLODStep, uiY + uiLODStep, uiWidth );*/

		if( bBackslashTriangle )
		{
			//Set the backslash triangle 
			/* |\ */
			//CW
			pData->SetTriangle( uiCurrTri++, uiIds[ LEFT_DOWN ], uiIds[ LEFT_UP ], uiIds[ RIGHT_DOWN ] );
			//CCW
			//pData->SetTriangle( uiCurrTri++, uiIds[ LEFT_UP ], uiIds[ LEFT_DOWN ], uiIds[ RIGHT_DOWN ] );
			//The second part 
			/* \| */
			//CW
			pData->SetTriangle( uiCurrTri++, uiIds[ LEFT_UP ], uiIds[ RIGHT_UP ], uiIds[ RIGHT_DOWN ] );
			//CCW
			//pData->SetTriangle( uiCurrTri++, uiIds[ RIGHT_UP ], uiIds[ LEFT_UP ], uiIds[ RIGHT_DOWN ] );
		}
		else
		{
			//Set the slash triangle 
			/* |/ */
			//CW
			pData->SetTriangle( uiCurrTri++, uiIds[ LEFT_DOWN ], uiIds[ LEFT_UP ], uiIds[ RIGHT_UP ] );
			//CCW
			//pData->SetTriangle( uiCurrTri++, uiIds[ LEFT_UP ], uiIds[ LEFT_DOWN ], uiIds[ RIGHT_UP ] );
			//The second part 
			/* /| */
			//CW
			pData->SetTriangle( uiCurrTri++, uiIds[ LEFT_DOWN ], uiIds[ RIGHT_UP ], uiIds[ RIGHT_DOWN ] );
			//CCW
			//pData->SetTriangle( uiCurrTri++, uiIds[ RIGHT_DOWN ], uiIds[ RIGHT_UP ], uiIds[ LEFT_DOWN ] );
		}

		*puiCurrTri = uiCurrTri;
	}

	void CMipMapPagingTerrain::_CalcQuadCCW(CIndexData* pData, cu32& uiX, cu32& uiY, u32* puiCurrTri, cu32& uiLODStep, bool bBackslashTriangle)
	{
		u32 uiCurrTri = *puiCurrTri;
		cu32 uiWidth = m_Options.TileVertexCount.x;

		cu32 uiIds[ 4 ] = 
		{
			CALC_XY( uiX, uiY, uiWidth ), //LEFT_UP
			CALC_XY( uiX + uiLODStep, uiY, uiWidth ), //RIGHT_UP
			CALC_XY( uiX + uiLODStep, uiY + uiLODStep, uiWidth ), //RIGHT_DOWN
			CALC_XY( uiX, uiY + uiLODStep, uiWidth ) //LEFT_DOWN
		};

		/*uiIds[ LEFT_UP ]	= CALC_XY( uiX, uiY, uiWidth );
		uiIds[ LEFT_DOWN ]	= CALC_XY( uiX, uiY + uiLODStep, uiWidth );
		uiIds[ RIGHT_UP ]	= CALC_XY( uiX + uiLODStep, uiY, uiWidth );
		uiIds[ RIGHT_DOWN ] = CALC_XY( uiX + uiLODStep, uiY + uiLODStep, uiWidth );*/

		if( bBackslashTriangle )
		{
			//Set the backslash triangle 
			/* |\ */
			//CW
			//pData->SetTriangle( uiCurrTri++, uiIds[ LEFT_DOWN ], uiIds[ LEFT_UP ], uiIds[ RIGHT_DOWN ] );
			//CCW
			pData->SetTriangle( uiCurrTri++, uiIds[ LEFT_UP ], uiIds[ LEFT_DOWN ], uiIds[ RIGHT_DOWN ] );
			//The second part 
			/* \| */
			//CW
			//pData->SetTriangle( uiCurrTri++, uiIds[ LEFT_UP ], uiIds[ RIGHT_UP ], uiIds[ RIGHT_DOWN ] );
			//CCW
			pData->SetTriangle( uiCurrTri++, uiIds[ RIGHT_UP ], uiIds[ LEFT_UP ], uiIds[ RIGHT_DOWN ] );
		}
		else
		{
			//Set the slash triangle 
			/* |/ */
			//CW
			//pData->SetTriangle( uiCurrTri++, uiIds[ LEFT_DOWN ], uiIds[ LEFT_UP ], uiIds[ RIGHT_UP ] );
			//CCW
			pData->SetTriangle( uiCurrTri++, uiIds[ LEFT_UP ], uiIds[ LEFT_DOWN ], uiIds[ RIGHT_UP ] );
			//The second part 
			/* /| */
			//CW
			//pData->SetTriangle( uiCurrTri++, uiIds[ RIGHT_UP ], uiIds[ RIGHT_DOWN ], uiIds[ LEFT_DOWN ] );
			//CCW
			pData->SetTriangle( uiCurrTri++, uiIds[ RIGHT_DOWN ], uiIds[ RIGHT_UP ], uiIds[ LEFT_DOWN ] );
		}

		*puiCurrTri = uiCurrTri;
	}

	void CMipMapPagingTerrain::_CalcBaseIBStitch(u32 uiLOD, const CPoint& VertexCount, IndexBufferPtr& pIB, pfnCalcIBStitch Func1,  pfnCalcIBStitch Func2)
	{
		//XSTSimpleProfiler();
		u32 ulQuad = 0;
		cu32 uiLODStep = CalcLODStep( uiLOD );
		bool bIsLastLOD = (uiLOD == m_Options.uiMaxLODCount - 1);
		// If this lod is the las lod there is no next lod step
		cu32 uiNextLODStep = ( bIsLastLOD ) ? uiLODStep : CalcLODStep( uiLOD + 1 );
		/*cu32 uiHeight = m_Options.TileVertexCount.y - uiLODStep;
		cu32 uiWidth = m_Options.TileVertexCount.x - uiLODStep;
		cu32 uiLastX = uiWidth - uiLODStep;
		cu32 uiLastY = uiHeight - uiLODStep;*/
		cu32 uiHeight = VertexCount.y - 1;
		cu32 uiWidth = VertexCount.x - 1;
		cu32 uiLastX = uiWidth - uiLODStep;
		cu32 uiLastY = uiHeight - uiLODStep;
		u32 uiCurrId = 0;
		u32 uiCurrTri = 0;
		u32 uiPos = 0;
		u32 uiId = 0;
		bool bBackslash;

		CIndexData& IData = pIB->GetIndexData();

		SCalcIBStitchInfo Info;
		Info.uiHeight = uiHeight;
		Info.uiWidth = uiWidth;
		Info.uiLastX = uiLastX;
		Info.uiLastY = uiLastY;
		Info.uiLODStep = uiLODStep;
		Info.uiNextLODStep = uiNextLODStep;
		Info.uiVertCountX = m_Options.TileVertexCount.x;
		Info.uiVertCountY = m_Options.TileVertexCount.y;
		Info.uiCurrItrX = Info.uiCurrItrY = 0;

		if( bIsLastLOD )
		{
			Func1 = &CMipMapPagingTerrain::_CalcBaseIBStitchEmpty;
			Func2 = &CMipMapPagingTerrain::_CalcBaseIBStitchEmpty;
		}

		for(u32 z = 0; z < uiHeight; z += uiLODStep)
		{
			Info.uiCurrY = z;
			Info.uiCurrItrX = 0;
			bBackslash = ulQuad % 2 == 0;

			for(u32 x = 0; x < uiWidth; x += uiLODStep)
			{
				if( g_bCCW )
					_CalcQuadCCW( &IData, x, z, &uiCurrTri, uiLODStep, bBackslash );
				else
					_CalcQuadCW( &IData, x, z, &uiCurrTri, uiLODStep, bBackslash );
				bBackslash = !bBackslash;
				Info.uiCurrID = uiCurrTri * 3;
				Info.uiCurrX = x;
				XST_CALL_MEMBER_FN_PTR( this, Func1 )( &IData, Info );
				XST_CALL_MEMBER_FN_PTR( this, Func2 )( &IData, Info );
				Info.uiCurrItrX++;
			}
			ulQuad++;
			Info.uiCurrItrY++;
		}
	}

	void CMipMapPagingTerrain::_CalcIBStitchNoneCCW(u32 uiLOD, IndexBufferPtr& pIB)
	{
		u32 ulQuad = 0;
		cu32 uiHeight = m_Options.TileVertexCount.y;
		cu32 uiWidth = m_Options.TileVertexCount.x;
		cu32 uiLODStep = CalcLODStep( uiLOD );
		u32 uiCurrHeight = 0;
		u32 uiCurrTri = 0;

		CIndexData& IData = pIB->GetIndexData();

		for(u32 z = 0; z < uiHeight - uiLODStep; z += uiLODStep)
		{
			//CalcStripStitchUpLeft( IData4, z, uiWidth, &uiCurrTri, uiLodStep, ulQuad % 2 == 0 );
			bool bBackslash = ulQuad % 2 == 0;
			for(u32 x = 0; x < uiWidth - uiLODStep; x += uiLODStep)
			{
				_CalcQuadCCW( &IData, x, z, &uiCurrTri, uiLODStep, bBackslash );
				bBackslash = !bBackslash;
			}
			ulQuad++;
		}

		//XST::CDebug::PrintDebugLN( XST::StringUtil::ToString( uiLOD ) );
		//DebugPrintIndexData( IData );
	}

	void CMipMapPagingTerrain::_CalcBaseIBStitchRightCCW(CIndexData* pData, const SCalcIBStitchInfo& Info)
	{
		if( Info.uiCurrX == Info.uiLastX  )
		{
			u32 uiPos;
			u32 uiId;
			//For even strip
			//if( Info.uiCurrY % 2 == 0 )
			if( Info.uiCurrItrY % 2 == 0 )
			{

					// Quad calculations: 
					// Left triangle: uiIds[ LEFT_UP ], uiIds[ LEFT_DOWN ], uiIds[ RIGHT_UP ]
					// Right triangle: uiIds[ RIGHT_DOWN ], uiIds[ RIGHT_UP ], uiIds[ LEFT_DOWN ]
					// Right down vertex is at position -3
					uiPos = Info.uiCurrID - 3;

				uiId = CALC_XY( Info.uiCurrX + Info.uiLODStep, Info.uiCurrY + Info.uiNextLODStep, Info.uiVertCountX );
			}
			else
			{
					// Quad calculations: 
					// Left triangle: uiIds[ LEFT_UP ], uiIds[ LEFT_DOWN ], uiIds[ RIGHT_DOWN ]
					// Right triangle: uiIds[ RIGHT_UP ], uiIds[ LEFT_UP ], uiIds[ RIGHT_DOWN ]
					// Right up vertex is at position -3
					uiPos = Info.uiCurrID - 3;
				uiId = CALC_XY( Info.uiCurrX + Info.uiLODStep, Info.uiCurrY + Info.uiLODStep, Info.uiVertCountX );
				
			}
			pData->SetIndex( uiPos, uiId );
		}
	}

	void CMipMapPagingTerrain::_CalcBaseIBStitchLeftCCW(CIndexData* pData, const SCalcIBStitchInfo& Info)
	{
		if( Info.uiCurrX == 0 )
		{
			u32 uiPos, uiId;
			//For even strip
			//if( Info.uiCurrY % 2 == 0 )
			if( Info.uiCurrItrY % 2 == 0 )
			{
				/* |\ */
					// Quad calculations: 
					// Left triangle: leftUp, leftDown, rightDown
					// Right triangle: rightUp, leftUp, rightDown
					// Left down vertex for left triangle is at index -4
					uiPos = Info.uiCurrID - 4 - 1; // sub -1 because we are on a next working index

				uiId = CALC_XY( Info.uiCurrX, Info.uiCurrY + Info.uiNextLODStep, Info.uiVertCountX );
			}
			else
			{
				/* |/ */

					// Quad calculations: 
					// Left triangle: leftUp, leftDown, rightUp
					// Right triangle: rightDown, rightUp, leftDown
					// Left up vertex for left triangle is at index -5
					uiPos = Info.uiCurrID - 5 - 1;

				uiId = CALC_XY( Info.uiCurrX + Info.uiLODStep, Info.uiCurrY, Info.uiVertCountX );
			}
			pData->SetIndex( uiPos, uiId );
		}
	}

	void CMipMapPagingTerrain::_CalcBaseIBStitchUpCCW(CIndexData* pData, const SCalcIBStitchInfo& Info)
	{
		if( Info.uiCurrY == 0 ) //if this is the first row
		{
			u32 uiPos, uiId;
			//For even quads change right up vertex of the second triangle
			if( Info.uiCurrItrX % 2 == 0 )
			{
				/* \| change to \/ */
					// Quad calculations: 
					// Left triangle: leftUp, leftDown, rightDown
					// Right triangle: rightUp, leftUp, rightDown
					// Right up vertex for left triangle is at index -3
					uiPos = Info.uiCurrID - 3;

				uiId = CALC_XY( Info.uiCurrX + Info.uiNextLODStep, Info.uiCurrY, Info.uiVertCountX );
			}
			else //for odd quads degenerate first triangle
			{
				/* |/ change to / */
					// Quad calculations: 
					// Left triangle: leftUp, leftDown, rightUp
					// Right triangle: rightDown, rightUp, leftDown
					// Left up vertex for left triangle is at index -6
					uiPos = Info.uiCurrID - 6;

				uiId = CALC_XY( Info.uiCurrX + Info.uiLODStep, Info.uiCurrY, Info.uiVertCountX );
			}
			pData->SetIndex( uiPos, uiId );
		}
	}

	void CMipMapPagingTerrain::_CalcBaseIBStitchDownCCW(CIndexData* pData, const SCalcIBStitchInfo& Info)
	{
		if( Info.uiCurrY == Info.uiLastY ) //if this is the last row
		{
			// Index position in the buffer
			// Vertex id in the vertex buffer
			u32 uiPos, uiId;
			//For even quads change right up vertex of the second triangle
			//if( Info.uiCurrX % 2 == 0 )
			if( Info.uiCurrItrX % 2 == 0 )
			{
				/* /| to /\ */
					// Quad calculations: 
					// Left triangle: leftUp, leftDown, rightUp
					// Right triangle: rightDown, rightUp, leftDown
					// Right down vertex for left triangle is at index -3
					uiPos = Info.uiCurrID - 3;

				uiId = CALC_XY( Info.uiCurrX + Info.uiNextLODStep, Info.uiCurrY + Info.uiLODStep, Info.uiVertCountX );
			}
			else //for odd quads degenerate first triangle
			{
				/* |\ change to \ */
					// Quad calculations: 
					// Left triangle: leftUp, leftDown, rightDown
					// Right triangle: rightUp, leftUp, rightDown
					// Left down vertex for left triangle is at index -5
					uiPos = Info.uiCurrID - 5;

				uiId = CALC_XY( Info.uiCurrX, Info.uiCurrY, Info.uiVertCountX );
			}
			pData->SetIndex( uiPos, uiId );
		}
	}

	void CMipMapPagingTerrain::_CalcIBStitchDownCCW(u32 uiLOD, IndexBufferPtr& pIB)
	{
		pIB->_SetDbgName("terr_ib_st_d");
		_CalcBaseIBStitch( uiLOD, m_Options.TileVertexCount, pIB, &CMipMapPagingTerrain::_CalcBaseIBStitchDownCCW );
	}

	void CMipMapPagingTerrain::_CalcIBStitchDownLeftCCW(u32 uiLOD, IndexBufferPtr& pIB)
	{
		pIB->_SetDbgName("terr_ib_st_dl");
		_CalcBaseIBStitch( uiLOD, m_Options.TileVertexCount, pIB, &CMipMapPagingTerrain::_CalcBaseIBStitchDownCCW, &CMipMapPagingTerrain::_CalcBaseIBStitchLeftCCW );
	}

	void CMipMapPagingTerrain::_CalcIBStitchLeftCCW(u32 uiLOD, IndexBufferPtr& pIB)
	{
		pIB->_SetDbgName("terr_ib_st_l");
		_CalcBaseIBStitch( uiLOD, m_Options.TileVertexCount, pIB, &CMipMapPagingTerrain::_CalcBaseIBStitchLeftCCW );
	}

	void CMipMapPagingTerrain::_CalcIBStitchRightCCW(u32 uiLOD, IndexBufferPtr& pIB)
	{
		pIB->_SetDbgName("terr_ib_st_r");
		_CalcBaseIBStitch( uiLOD, m_Options.TileVertexCount, pIB, &CMipMapPagingTerrain::_CalcBaseIBStitchRightCCW );
	}

	void CMipMapPagingTerrain::_CalcIBStitchRightDownCCW(u32 uiLOD, IndexBufferPtr& pIB)
	{
		pIB->_SetDbgName("terr_ib_st_rd");
		_CalcBaseIBStitch( uiLOD, m_Options.TileVertexCount, pIB, &CMipMapPagingTerrain::_CalcBaseIBStitchDownCCW, &CMipMapPagingTerrain::_CalcBaseIBStitchRightCCW );
	}

	void CMipMapPagingTerrain::_CalcIBStitchUpCCW(u32 uiLOD, IndexBufferPtr& pIB)
	{
		pIB->_SetDbgName("terr_ib_st_u");
		_CalcBaseIBStitch( uiLOD, m_Options.TileVertexCount, pIB, &CMipMapPagingTerrain::_CalcBaseIBStitchUpCCW );
	}

	void CMipMapPagingTerrain::_CalcIBStitchUpLeftCCW(u32 uiLOD, IndexBufferPtr& pIB)
	{
		pIB->_SetDbgName("terr_ib_st_ul");
		_CalcBaseIBStitch( uiLOD, m_Options.TileVertexCount, pIB, &CMipMapPagingTerrain::_CalcBaseIBStitchUpCCW, &CMipMapPagingTerrain::_CalcBaseIBStitchLeftCCW );
	}

	void CMipMapPagingTerrain::_CalcIBStitchUpRightCCW(u32 uiLOD, IndexBufferPtr& pIB)
	{
		pIB->_SetDbgName("terr_ib_st_ur");
		_CalcBaseIBStitch( uiLOD, m_Options.TileVertexCount, pIB, &CMipMapPagingTerrain::_CalcBaseIBStitchUpCCW, &CMipMapPagingTerrain::_CalcBaseIBStitchRightCCW );
	}


	void CMipMapPagingTerrain::_CalcIBStitchNoneCW(u32 uiLOD, IndexBufferPtr& pIB)
	{
		u32 ulQuad = 0;
		cu32 uiHeight = m_Options.TileVertexCount.y;
		cu32 uiWidth = m_Options.TileVertexCount.x;
		cu32 uiLODStep = CalcLODStep( uiLOD );
		u32 uiCurrHeight = 0;
		u32 uiCurrTri = 0;

		CIndexData& IData = pIB->GetIndexData();

		for(u32 z = 0; z < uiHeight - uiLODStep; z += uiLODStep)
		{
			//CalcStripStitchUpLeft( IData4, z, uiWidth, &uiCurrTri, uiLodStep, ulQuad % 2 == 0 );
			bool bBackslash = ulQuad % 2 == 0;
			for(u32 x = 0; x < uiWidth - uiLODStep; x += uiLODStep)
			{
				_CalcQuadCW( &IData, x, z, &uiCurrTri, uiLODStep, bBackslash );
				bBackslash = !bBackslash;
			}
			ulQuad++;
		}

		//XST::CDebug::PrintDebugLN( XST::StringUtil::ToString( uiLOD ) );
		//DebugPrintIndexData( IData );
	}

	void CMipMapPagingTerrain::_CalcBaseIBStitchRightCW(CIndexData* pData, const SCalcIBStitchInfo& Info)
	{
		if( Info.uiCurrX == Info.uiLastX  )
		{
			u32 uiPos;
			u32 uiId;
			//For even strip
			//if( Info.uiCurrY % 2 == 0 )
			if( Info.uiCurrItrY % 2 == 0 )
			{
				//Left down vertex is at index -5
				/* /| */
				uiPos = Info.uiCurrID - 1;
				uiId = CALC_XY( Info.uiCurrX + Info.uiLODStep, Info.uiCurrY + Info.uiNextLODStep, Info.uiVertCountX );
			}
			else
			{
				//Left up vertex is at index -2
				/* \| */
				uiPos = Info.uiCurrID - 2;
				uiId = CALC_XY( Info.uiCurrX + Info.uiLODStep, Info.uiCurrY + Info.uiLODStep, Info.uiVertCountX );
				
			}
			pData->SetIndex( uiPos, uiId );
		}
	}

	void CMipMapPagingTerrain::_CalcBaseIBStitchLeftCW(CIndexData* pData, const SCalcIBStitchInfo& Info)
	{
		if( Info.uiCurrX == 0 )
		{
			u32 uiPos, uiId;
			//For even strip
			//if( Info.uiCurrY % 2 == 0 )
			if( Info.uiCurrItrY % 2 == 0 )
			{
				/* |\ */
				//Left down vertex is at index -5
				uiPos = Info.uiCurrID - 6;
				uiId = CALC_XY( Info.uiCurrX, Info.uiCurrY + Info.uiNextLODStep, Info.uiVertCountX );
			}
			else
			{
				/* |/ */
				//Left up vertex is at index -5
				uiPos = Info.uiCurrID - 5;	
				uiId = CALC_XY( Info.uiCurrX + Info.uiLODStep, Info.uiCurrY, Info.uiVertCountX );
			}
			pData->SetIndex( uiPos, uiId );
		}
	}

	void CMipMapPagingTerrain::_CalcBaseIBStitchUpCW(CIndexData* pData, const SCalcIBStitchInfo& Info)
	{
		if( Info.uiCurrY == 0 ) //if this is the first row
		{
			u32 uiPos, uiId;
			//For even quads change right up vertex of the second triangle
			if( Info.uiCurrItrX % 2 == 0 )
			{
				/* \| change to \/ */
				//Right up vertex of the second triangle is at position -2
				uiPos = Info.uiCurrID - 2;
				uiId = CALC_XY( Info.uiCurrX + Info.uiNextLODStep, Info.uiCurrY, Info.uiVertCountX );
			}
			else //for odd quads degenerate first triangle
			{
				/* |/ change to / */
				//Left up vertex of the first triangle is at position -5
				uiPos = Info.uiCurrID - 5;
				uiId = CALC_XY( Info.uiCurrX + Info.uiLODStep, Info.uiCurrY, Info.uiVertCountX );
			}
			pData->SetIndex( uiPos, uiId );
		}
	}

	void CMipMapPagingTerrain::_CalcBaseIBStitchDownCW(CIndexData* pData, const SCalcIBStitchInfo& Info)
	{
		if( Info.uiCurrY == Info.uiLastY ) //if this is the last row
		{
			// Index position in the buffer
			// Vertex id in the vertex buffer
			u32 uiPos, uiId;
			//For even quads change right up vertex of the second triangle
			//if( Info.uiCurrX % 2 == 0 )
			if( Info.uiCurrItrX % 2 == 0 )
			{
				/* /| to /\ */
				//Right down vertex of the second triangle is at position -2
				uiPos = Info.uiCurrID - 1;
				uiId = CALC_XY( Info.uiCurrX + Info.uiNextLODStep, Info.uiCurrY + Info.uiLODStep, Info.uiVertCountX );
			}
			else //for odd quads degenerate first triangle
			{
				/* |\ change to \ */
				//Left up vertex of the first triangle is at position -5
				uiPos = Info.uiCurrID - 6;
				uiId = CALC_XY( Info.uiCurrX, Info.uiCurrY, Info.uiVertCountX );
			}
			pData->SetIndex( uiPos, uiId );
		}
	}

	void CMipMapPagingTerrain::_CalcIBStitchDownCW(u32 uiLOD, IndexBufferPtr& pIB)
	{
		_CalcBaseIBStitch( uiLOD, m_Options.TileVertexCount, pIB, &CMipMapPagingTerrain::_CalcBaseIBStitchDownCW );
	}

	void CMipMapPagingTerrain::_CalcIBStitchDownLeftCW(u32 uiLOD, IndexBufferPtr& pIB)
	{
		_CalcBaseIBStitch( uiLOD, m_Options.TileVertexCount, pIB, &CMipMapPagingTerrain::_CalcBaseIBStitchDownCW, &CMipMapPagingTerrain::_CalcBaseIBStitchLeftCW );
	}

	void CMipMapPagingTerrain::_CalcIBStitchLeftCW(u32 uiLOD, IndexBufferPtr& pIB)
	{
		_CalcBaseIBStitch( uiLOD, m_Options.TileVertexCount, pIB, &CMipMapPagingTerrain::_CalcBaseIBStitchLeftCW );
	}

	void CMipMapPagingTerrain::_CalcIBStitchRightCW(u32 uiLOD, IndexBufferPtr& pIB)
	{
		_CalcBaseIBStitch( uiLOD, m_Options.TileVertexCount, pIB, &CMipMapPagingTerrain::_CalcBaseIBStitchRightCW );
	}

	void CMipMapPagingTerrain::_CalcIBStitchRightDownCW(u32 uiLOD, IndexBufferPtr& pIB)
	{
		_CalcBaseIBStitch( uiLOD, m_Options.TileVertexCount, pIB, &CMipMapPagingTerrain::_CalcBaseIBStitchDownCW, &CMipMapPagingTerrain::_CalcBaseIBStitchRightCW );
	}

	void CMipMapPagingTerrain::_CalcIBStitchUpCW(u32 uiLOD, IndexBufferPtr& pIB)
	{
		_CalcBaseIBStitch( uiLOD, m_Options.TileVertexCount, pIB, &CMipMapPagingTerrain::_CalcBaseIBStitchUpCW );
	}

	void CMipMapPagingTerrain::_CalcIBStitchUpLeftCW(u32 uiLOD, IndexBufferPtr& pIB)
	{
		_CalcBaseIBStitch( uiLOD, m_Options.TileVertexCount, pIB, &CMipMapPagingTerrain::_CalcBaseIBStitchUpCW, &CMipMapPagingTerrain::_CalcBaseIBStitchLeftCW );
	}

	void CMipMapPagingTerrain::_CalcIBStitchUpRightCW(u32 uiLOD, IndexBufferPtr& pIB)
	{
		_CalcBaseIBStitch( uiLOD, m_Options.TileVertexCount, pIB, &CMipMapPagingTerrain::_CalcBaseIBStitchUpCW, &CMipMapPagingTerrain::_CalcBaseIBStitchRightCW );
	}

}//xse