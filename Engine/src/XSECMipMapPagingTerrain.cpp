#include "XSECMipMapPagingTerrain.h"
#include "XSEIVertexBuffer.h"
#include "XSEIIndexBuffer.h"
#include "XSEIRenderSystem.h"
#include "XSECCamera.h"
#include "XSECSceneManager.h"
#include "XSECRenderQueueElement.h"
#include "XSECMeshManager.h"
#include "XSECModelManager.h"
#include "XSECSceneNode.h"
#include "XSECImageManager.h"

namespace XSE
{
	#define CALC_XY(_x, _y, _width) ( (_x) + (_y) * (_width) )

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
	}

	CMipMapPagingTerrain::~CMipMapPagingTerrain()
	{
		DestroyData();
	}

	void CMipMapPagingTerrain::DestroyData()
	{

		IBVec::iterator IBItr;
		xst_stl_foreach( IBItr, m_vIndexBuffers )
		{
			//xst_release( IBItr->pIndexBuffer );
		}

		m_vIndexBuffers.clear();

		PageVec::iterator PageItr;
		xst_stl_foreach( PageItr, m_vPages )
		{
			xst_delete( (*PageItr) );
		}

		m_vPages.clear();

		for(u32 i = 0; i < m_vTiles.size(); ++i)
		{
			CMipMapTerrainTile* pTile = m_vTiles[ i ];
			//xst_delete( pTile );
		}

		xst_vector_clear( m_vTiles, TileVec::value_type );
		xst_vector_clear( m_vTilePool, TilePoolVec::value_type );
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
        CCamera* pCam = m_pSceneMgr->GetComputeCamera();

        for( u32 i = 0; i < m_vTiles.size(); ++i )
        {
            pCurrTile = m_vTiles[ i ];
            pCurrMesh = pCurrTile->m_pMesh.GetPointer();
            if( !pCurrMesh->IsVisible() || pCurrMesh->IsDisabled() )
                continue;
            const CAABB& AABB = pCurrMesh->GetBoundingVolume().GetAABB();
            Vec3 vecSize = AABB.CalcSize();
            fConstDist = vecSize.x * 1;
			f32 fDist = pCurrMesh->GetDistanceToCamera();
            fLODDist = 0;
			// Set the max lod by default
			//pCurrTile->SetLOD( CalcLOD( 3, MipMapTerrainStitchTypes::LEFT ), m_Options.uiLODCount - 1, MipMapTerrainStitchTypes::NONE );
			pCurrTile->SetLOD( CalcLOD( m_Options.uiLODCount - 1, MipMapTerrainStitchTypes::NONE ), m_Options.uiLODCount - 1, MipMapTerrainStitchTypes::NONE );
            lpcastr pName = pCurrTile->GetMesh()->_GetDbgName();
			for( u32 l = 0; l < m_Options.uiLODCount; ++l )
			{
                //strcmp( pName, "Terrain_5_4" ) == 0 || strcmp( pName, "Terrain_6_4" ) == 0
				// Get lod of all neighbours
				// This block costs 2fps
                fPrevDist = fLODDist;
				fLODDist += fConstDist; // multiply by l begins from 1
				//if( fDist < fLODDist + 0 )
				{
                    const CAABB ViewAABB(   Vec3( pCam->GetPosition().x - fLODDist, pCam->GetPosition().y - fLODDist, pCam->GetPosition().z - fLODDist ), 
                                            Vec3( Vec3( pCam->GetPosition().x + fLODDist, pCam->GetPosition().y + fLODDist, pCam->GetPosition().z + fLODDist ) ) );
                    f32 fMaxLenX = ViewAABB.vecMax.x - ViewAABB.vecMin.x + AABB.vecMax.x - AABB.vecMin.x;
                    f32 fMaxLenZ = ViewAABB.vecMax.z - ViewAABB.vecMin.z + AABB.vecMax.z - AABB.vecMin.z;
                    // Check if this tile intersects the view square (not only view sphere)

                    f32 fDistX = ViewAABB.vecMax.x - AABB.vecMin.x;
                    f32 fDistZ = ViewAABB.vecMax.z - AABB.vecMin.z;
                    if( ( fDistX < fMaxLenX && fDistX > 0.0f ) &&
                        ( fDistZ < fMaxLenZ && fDistZ > 0.0f ) )
                    {
                        pCurrTile->SetLOD( CalcLOD( l, MipMapTerrainStitchTypes::NONE ), l, MipMapTerrainStitchTypes::NONE );
                        break;
                    }
				}
			}
		}

        CMipMapTerrainTile* pLeft, *pRight, *pUp, *pDown;
        u32 uiLOD;
        bool bLeftLOD, bRightLOD, bUpLOD, bDownLOD;
        MIPMAP_TERRAIN_STITCH_TYPE eStitchType = MipMapTerrainStitchTypes::NONE;
        // TODO: first and last tiles also should be computed
        for( u32 y = 1; y < m_TileCount.y - 1; ++y )
        {
            for( u32 x = 1; x < m_TileCount.x - 1; ++x )
            {
                eStitchType = MipMapTerrainStitchTypes::NONE;
                pCurrTile = m_vTileGrid[ x ][ y ];
                if( !pCurrTile->GetMesh()->IsVisible() /*|| pCurrTile->GetMesh()->IsDisabled()*/ )
                    continue;
                uiLOD = pCurrTile->GetLOD();
                pLeft = m_vTileGrid[ x - 1 ][ y ];
                pRight = m_vTileGrid[ x + 1 ][ y ];
                pUp = m_vTileGrid[ x ][ y - 1 ];
                pDown = m_vTileGrid[ x ][ y + 1 ];

                bLeftLOD = pLeft->GetLOD() > uiLOD;
                bRightLOD = pRight->GetLOD() > uiLOD;
                bUpLOD = pUp->GetLOD() > uiLOD;
                bDownLOD = pDown->GetLOD() > uiLOD;

                if( bLeftLOD && bUpLOD )
                    eStitchType = MipMapTerrainStitchTypes::LEFT_UP;
                else if( bLeftLOD && bDownLOD )
                    eStitchType = MipMapTerrainStitchTypes::LEFT_DOWN;
                else if( bRightLOD && bUpLOD )
                    eStitchType = MipMapTerrainStitchTypes::RIGHT_UP;
                else if( bRightLOD && bDownLOD )
                    eStitchType = MipMapTerrainStitchTypes::RIGHT_DOWN;
                else if( bLeftLOD )
                    eStitchType = MipMapTerrainStitchTypes::LEFT;
                else if( bRightLOD )
                    eStitchType = MipMapTerrainStitchTypes::RIGHT;
                else if( bUpLOD )
                    eStitchType = MipMapTerrainStitchTypes::UP;
                else if( bDownLOD )
                    eStitchType = MipMapTerrainStitchTypes::DOWN;

                pCurrTile->SetLOD( CalcLOD( uiLOD, eStitchType ), uiLOD, eStitchType );
            }
        }
	}

	i32 CMipMapPagingTerrain::Init(const STerrainOptions& Options)
	{
		m_Options = Options;
		
		ValidateOptions( &m_Options );

		m_TileCount = CPoint( ceilf( (f32)m_Options.PageVertexCount.x / m_Options.TileVertexCount.x ), ceilf( (f32)m_Options.PageVertexCount.y / m_Options.TileVertexCount.y ) );
		
		if( XST_FAILED( CreatePages() ) )
		{
			return XST_FAIL;
		}

		//Load heightmap images
		if( XST_FAILED( LoadImages( Options.vHeightmaps ) ) )
		{
			return XST_FAIL;
		}

		return XST_OK;
	}

	i32 CMipMapPagingTerrain::CreatePages()
	{
		//Create pages
		u32 uiPageCount = m_Options.PageCount.x * m_Options.PageCount.y;
		for(u32 i = 0; i < uiPageCount; ++i)
		{
			CMipMapTerrainPage* pPage = xst_new CMipMapTerrainPage( this );
			m_vPages.push_back( pPage );

			if( XST_FAILED( pPage->Init( m_TileCount.x, m_TileCount.y, m_Options.TileVertexCount.x * m_Options.TileVertexCount.y, m_pInputLayout ) ) )
			{
				return XST_FAIL;
			}
		}
		
		return XST_OK;
	}

	i32 CMipMapPagingTerrain::LoadImages(const StringVector& vHeightmapNames)
	{
		CImageManager* pImgMgr = CImageManager::GetSingletonPtr();
		u32 uiCount = Math::Min( vHeightmapNames.size(), m_vPages.size() );
		xst_castring strImgGroup( DEFAULT_GROUP );

		for(u32 i = 0; i < uiCount; ++i)
		{
			ImagePtr pImg = pImgMgr->CreateImage( vHeightmapNames[ i ] );
			if( pImg == xst_null )
			{
				return XST_FAIL;
			}

			if( XST_FAILED( pImgMgr->LoadImage( pImg ) ) )
			{
				return XST_FAIL;
			}

			if( XST_FAILED( pImgMgr->PrepareImage( pImg ) ) )
			{
				return XST_FAIL;
			}

			//If image has more pixels than terrain, resize it
			u32 uiHeight = m_Options.PageVertexCount.y; //Math::Max( (u32)m_Options.PageVertexCount.y, pImg->GetHeight() ) - m_TileCount.y + 1; //leave first tile
			u32 uiWidth = m_Options.PageVertexCount.x; //Math::Max( (u32)m_Options.PageVertexCount.x, pImg->GetWidth() ) - m_TileCount.x + 1;
			//u32 uiHeight = Math::Max( (u32)m_Options.PageVertexCount.y, pImg->GetHeight() ) - m_TileCount.y + 1; //leave first tile
			//u32 uiWidth = Math::Max( (u32)m_Options.PageVertexCount.x, pImg->GetWidth() ) - m_TileCount.x + 1;
			pImg->Scale( uiWidth, uiHeight );

			m_vPages[ i ]->SetImage( pImg );
		}
		return XST_OK;
	}

	CMipMapTerrainTile* CMipMapPagingTerrain::CreateTile()
	{
		//CMipMapTerrainTile* pTile = xst_new CMipMapTerrainTile();
		u32 uiId = m_vTiles.size();
		CMipMapTerrainTile* pTile = &m_vTilePool[ uiId ];
        pTile->SetPoolId( uiId );
		m_vTiles.push_back( pTile );
        
		return pTile;
	}

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
		
	}

	void CMipMapPagingTerrain::SetMaterial(MaterialPtr pMat)
	{
		ITerrain::SetMaterial( pMat );

		TileVec::iterator Itr;
		xst_stl_foreach( Itr, m_vTiles )
		{
			(*Itr)->GetMesh()->SetMaterial( pMat );
		}
	}

	i32 CMipMapPagingTerrain::CreateTiles()
	{
		u32 uiTileCount = m_TileCount.x * m_TileCount.y;
		
		m_vTileGrid.resize( m_TileCount.x );
        m_vTileInfoGrid.resize( m_TileCount.x );
        for( u32 i = 0; i < m_TileCount.x; ++i )
        {
            m_vTileGrid[ i ].resize( m_TileCount.y );
            m_vTileInfoGrid[ i ].resize( m_TileCount.y );
        }
		m_vTiles.reserve( uiTileCount );
		m_vTilePool.reserve( uiTileCount );
		m_vTilePool.resize( uiTileCount );
		//m_vTiles.resize( uiTileCount );

		/*for(u32 i = 0; i < uiTileCount; ++i)
		{
			 CreateTile();
		}*/

		// Create tiles in the grid
		for( u32 y = 0; y < m_TileCount.y; ++y )
		{
			for( u32 x = 0; x < m_TileCount.x; ++x )
			{
				CMipMapTerrainTile* pTile = CreateTile();
				pTile->Init( CPoint( x, y ) );
                m_vTileGrid[ x ][ y ] = pTile;
			}
		}

		// Set tile neighbours
		CMipMapTerrainTile* pTmpTile;
		for( u32 y = 0; y < m_TileCount.y; ++y )
		{
			for( u32 x = 0; x < m_TileCount.x; ++x )
			{
				CMipMapTerrainTile* pTile = m_vTiles[ CALC_XY( x, y, m_TileCount.x ) ];
				// Prepare neighbours
				CMipMapTerrainTile* pLeftTile = xst_null;
				CMipMapTerrainTile* pRightTile = xst_null;
				CMipMapTerrainTile* pUpTile = xst_null;
				CMipMapTerrainTile* pDownTile = xst_null;
				// Get left neighbour
				if( x > 0 )
				{
					pLeftTile = m_vTiles[ CALC_XY( x - 1, y, m_TileCount.x ) ];
				}
				// Get up neighbour
				if( y > 0 )
				{
					pUpTile = m_vTiles[ CALC_XY( x, y - 1, m_TileCount.x ) ];
				}
				// Get right neighbour
				if( x < m_TileCount.x - 1 )
				{
					pRightTile = m_vTiles[ CALC_XY( x + 1, y, m_TileCount.x ) ];
				}
				// Get down neighbour
				if( y < m_TileCount.y - 1 )
				{
					pDownTile = m_vTiles[ CALC_XY( x, y + 1, m_TileCount.x ) ];
				}

				CMipMapTerrainTile* aTiles[ 4 ] = { pLeftTile, pUpTile, pRightTile, pDownTile };
				pTile->SetNeighbours( aTiles );
			}
		}

		return XST_OK;
	}

	i32 CMipMapPagingTerrain::LockTiles()
	{
		if( m_bTileLocked )
		{
			return XST_OK;
		}

		//Create memory buffers for vertices and indices

		//Vertex count per one tile
		ul32 ulCurrTile = 0;

		CMeshManager* pMeshMgr = CMeshManager::GetSingletonPtr();
		ul32 ulVertexCount = m_Options.TileVertexCount.x * m_Options.TileVertexCount.y;
		//Calculate tile count
		//m_TileCount = CPoint( m_Options.PageVertexCount / m_Options.TileVertexCount );
		xst_stringstream ssName;
		
		ch8 strTileName[32];

		xst_string strFullName;
		strFullName.reserve(64);

		xst_astring strTerrName = XST_GET_DBG_NAME( this );
		if( strTerrName.empty() ) 
		{
			strTerrName = "Terrain";
			//strTerrName += XST::ToStr() << XST::CTime::GetQPerfTickCount();
		}

		//int iBaseNameLen = xst_sprintf( strTileName, 32, "%s_", strTerrName.c_str() );
		//int iTmpLen = 0;
        {
            XSTSimpleProfiler2( "Create and lock tile meshes" );
		for(i32 y = 0; y < m_TileCount.y; ++y)
		{
			for(i32 x = 0; x < m_TileCount.x; ++x)
			{
				xst_sprintf( strTileName, "%s_%d_%d", strTerrName.data(), x, y);
				strFullName = strTileName;
			
				MeshPtr pMesh; 
				{
					//XSTSimpleProfiler2("Create Tile Mesh"); //~0.003sec in debug
					pMesh = pMeshMgr->CreateMesh( strFullName, m_pInputLayout, m_pSceneMgr->GetName() );
				}
				CMipMapTerrainTile* pTile = m_vTiles[ ulCurrTile++ ];

				//m_pSceneMgr->CreateNode( ssName.str() )->AddObject( pTile );

				{
					//XSTSimpleProfiler2("Lock Tile"); //~0.003 sec in debug
				if( XST_FAILED( pTile->Lock( pMesh, ulVertexCount ) ) )
				{
					return XST_FAIL;
				}
				}
				
			}
		}
        }
		m_bTileLocked = true;

		return XST_OK;
	}

	void CMipMapPagingTerrain::SetTileInfo(CMipMapTerrainTile::SInfo* pInfoOut)
	{
		//Center terrain position
		Vec3 vecTerrainPos( (f32)m_Options.Size.x * -0.5f, 0, (f32)m_Options.Size.y * 0.5f );
		
		//Calculate size of one tile
		Vec2 vecTileSize( (f32)m_Options.Size.x / m_TileCount.x, (f32)m_Options.Size.y / m_TileCount.y );
		Vec2 vecDistance( vecTileSize.x / ( m_Options.TileVertexCount.x - 1 ), vecTileSize.y / ( m_Options.TileVertexCount.y - 1 ) );
		
		xst_zero( pInfoOut, sizeof( CMipMapTerrainTile::SInfo ) );
		pInfoOut->TerrainVertexCount = m_Options.PageVertexCount;
		pInfoOut->vecTileSize = vecTileSize;
		pInfoOut->vecTerrainPosition = vecTerrainPos;
		pInfoOut->vecVertexDistance = vecDistance;
		pInfoOut->VertexCount = m_Options.TileVertexCount;
		pInfoOut->vecHeightRange = m_Options.vecHeightRange;
	}

	i32 CMipMapPagingTerrain::CreatePagesVertexData()
	{
		//For each visible page set its data
		PageVec::iterator Itr;
		xst_stl_foreach( Itr, m_vPages )
		{
			if( XST_FAILED( (*Itr)->CreateVertexData() ) )
			{
				return XST_OK;
			}
		}
		return XST_OK;
	}

	i32 CMipMapPagingTerrain::SetPagesData()
	{
		CMipMapTerrainTile::SInfo Info;
		SetTileInfo( &Info );

		//For each visible page set its data
		PageVec::iterator Itr;
		xst_stl_foreach( Itr, m_vPages )
		{
			if( XST_FAILED( (*Itr)->CalcVertexData( Info ) ) )
			{
				return XST_FAIL;
			}
		}

		return XST_OK;
	}

	i32 CMipMapPagingTerrain::SetTileDataFromPages()
	{

		xst_assert( m_bTileLocked, "(CMipMapPagingTerrain::SetTileData) Tiles not locked for set data" );
		xst_assert( m_vTiles.size() > 0, "(CMipMapPagingTerrain::SetTileData) Tiles not created" );

		//If tile count is more than page count, set tile data from main page (center) to tiles
		//Choose next pages to set rest of the tiles

#if defined( XSE_RENDERER_DEBUG )
		m_bBoundingObjectsCreated = true;
		CMeshManager* pMeshMgr = CMeshManager::GetSingletonPtr();
		CModelManager* pModelMgr = CModelManager::GetSingletonPtr();
		if( !m_bBoundingObjectsCreated )
		{
			m_pBoundingSphereModel = CModelManager::GetSingletonPtr( )->CreateModel( "Terrain/BoundingSphere", "Debug" );
			m_pAABBModel = CModelManager::GetSingletonPtr( )->CreateModel( "Terrain/AABB", "Debug" );
			CSceneNode* pNode = m_pSceneMgr->CreateNode( "Terrain/BoundingObjects" );
			pNode->AddObject( m_pBoundingSphereModel );
			pNode->AddObject( m_pAABBModel );
			m_pBoundingSphereModel->SetVisible( false );
		}
#endif // XSE_RENDERER_DEBUG

		u32 uiCurrTileID = 0;
		PageVec::iterator PageItr;
		xst_stl_foreach( PageItr, m_vPages )
		{
			//const CMipMapTerrainPage::VertexDataVec& vVertexDataBuffer = (*PageItr)->GetVertexDataBuffer();
			const CMipMapTerrainPage::VertexDataArray& vVertexDataBuffer = (*PageItr)->GetVertexDataBuffer();
			CVertexData* pCurrVertexData;
			CMipMapTerrainTile* pCurrTile;

			for(u32 i = 0; i < vVertexDataBuffer.size(); ++i)
			{
				pCurrTile = m_vTiles[ uiCurrTileID ];
				pCurrVertexData = vVertexDataBuffer[ i ].pData;
				if( XST_FAILED( pCurrTile->SetVertexData( *pCurrVertexData ) ) )
				{
					return XST_OK;
				}
				
				pCurrTile->SetBoundingVolume( vVertexDataBuffer[ i ].Volume );

#if defined( XSE_RENDERER_DEBUG )
				if( !m_bBoundingObjectsCreated )
				{
					SCircleOptions SphereOptions;
					const CBoundingVolume& Volume = pCurrTile->GetMesh( )->GetBoundingVolume( );
					SphereOptions.fRadius = Volume.GetSphere().fRadius;
					SphereOptions.vecPos = Volume.GetSphere().vecCenter;
					SphereOptions.uStep = 20;
					MeshPtr pSphereMesh = pMeshMgr->CreateMesh( pCurrTile->GetMesh()->GetResourceName() + "/BoundingSphere", ILEs::POSITION | ILEs::COLOR, XSE::BasicShapes::CIRCLE, &SphereOptions, "Debug" );
					m_pBoundingSphereModel->AddMesh( pSphereMesh );
				}
#endif // XSE_RENDERER_DEBUG

				++uiCurrTileID;
			}
		}

#if defined( XSE_RENDERER_DEBUG )
		m_bBoundingObjectsCreated = true;
#endif // XSE_RENDERER_DEBUG

		return XST_OK;
	}

	i32 CMipMapPagingTerrain::SetTileData()
	{
		xst_assert( m_bTileLocked, "(CMipMapPagingTerrain::SetTileData) Tiles not locked for set data" );
		xst_assert( m_vTiles.size() > 0, "(CMipMapPagingTerrain::SetTileData) Tiles not created" );

		//Set vertex positions
		u32 uiCurrTile = 0;
		//Calculate distance between vertices
		//Vec2 vecDistance( (f32)m_Options.Size.x / ( m_Options.PageVertexCount.x - 1 ), (f32)m_Options.Size.y / ( m_Options.PageVertexCount.y - 1 ) );
		//Center terrain position
		Vec3 vecTerrainPos( (f32)m_Options.Size.x * -0.5f, 0, (f32)m_Options.Size.y * 0.5f );
		
		//Calculate size of one tile
		Vec2 vecTileSize( (f32)m_Options.Size.x / m_TileCount.x, (f32)m_Options.Size.y / m_TileCount.y );
		Vec2 vecDistance( vecTileSize.x / ( m_Options.TileVertexCount.x - 1 ), vecTileSize.y / ( m_Options.TileVertexCount.y - 1 ) );
		
		CMipMapTerrainTile::SInfo TileInfo;
		xst_zero( &TileInfo, sizeof( CMipMapTerrainTile::SInfo ) );
		TileInfo.TerrainVertexCount = m_Options.PageVertexCount;
		TileInfo.vecTileSize = vecTileSize;
		TileInfo.vecTerrainPosition = vecTerrainPos;
		TileInfo.vecVertexDistance = vecDistance;
		TileInfo.VertexCount = m_Options.TileVertexCount;
		TileInfo.vecHeightRange = m_Options.vecHeightRange;
		//Temp
		TileInfo.pHeightmap = m_vPages[ 0 ]->GetImage().GetPointer();

		for(i32 y = 0; y < m_TileCount.y; ++y)
		{
			for(i32 x = 0; x < m_TileCount.x; ++x)
			{
				TileInfo.TilePart = CPoint( x, y );
				m_vTiles[ uiCurrTile++ ]->CalcVertexData( TileInfo );
				DebugPrintVB( m_vTiles[ uiCurrTile - 1]->m_pMesh );
			}
			//DebugPrintVB( m_vTiles[ uiCurrTile - 1]->m_pMesh );
		}

	
		return XST_OK;
	}

	i32 CMipMapPagingTerrain::UnlockTiles()
	{
		if( !m_bTileLocked )
		{
			return XST_OK;
		}

		//Create Renderer vertex and index buffers for tile data

		TileVec::iterator Itr;
		xst_stl_foreach( Itr, m_vTiles )
		{
			if( XST_FAILED( (*Itr)->Unlock() ) )
			{
				return XST_FAIL;
			}
		}

		m_bTileLocked = false;
		
		return XST_OK;
	}

	void CMipMapPagingTerrain::DestroyIndexBuffers()
	{
		IBVec::iterator Itr;
		xst_stl_foreach( Itr, m_vIndexBuffers )
		{
			xst_release( (*Itr).pIndexBuffer );
		}

		xst_vector_clear( m_vIndexBuffers, IBVec::value_type );
	}

	i32	CMipMapPagingTerrain::CreateIndexBuffers()
	{
		const u32 uiIBCount = m_Options.uiLODCount * MipMapTerrainStitchTypes::_MAX_COUNT;

		m_vIndexBuffers.reserve( uiIBCount );

		for(u32 l = 0; l < m_Options.uiLODCount; ++l)
		{
			for(u32 s = 0; s < MipMapTerrainStitchTypes::_MAX_COUNT; ++s)
			{
				u16 usIndexCount = CalcIndexCount( l );
				IndexBufferPtr pIB = IndexBufferPtr( m_pSceneMgr->GetRenderSystem()->CreateIndexBuffer() );
				pIB->SetUsage( BufferUsages::STATIC );
				pIB->SetIndexCount( usIndexCount );
				SMipMapIndexBuffer IB = { (MIPMAP_TERRAIN_STITCH_TYPE)s, pIB, l };
				m_vIndexBuffers.push_back( IB );
			}
		}
	
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
		IBVec::iterator Itr;
		xst_stl_foreach( Itr, m_vIndexBuffers )
		{
			if( XST_FAILED( (*Itr).pIndexBuffer->Lock() ) )
			{
				return XST_FAIL;
			}
		}
		return XST_OK;
	}

	i32	CMipMapPagingTerrain::SetIndexBufferData(u32 uiLOD)
	{

		cu32 uiIBID = GetIndexBuffersID( uiLOD );

		_CalcIBStitchDown( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::DOWN ].pIndexBuffer );
		_CalcIBStitchDownLeft( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::DOWN_LEFT ].pIndexBuffer );
		_CalcIBStitchLeft( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::LEFT ].pIndexBuffer );
		_CalcIBStitchNone( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::NONE ].pIndexBuffer );
		_CalcIBStitchRight( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::RIGHT ].pIndexBuffer );
		_CalcIBStitchRightDown( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::RIGHT_DOWN ].pIndexBuffer );
		_CalcIBStitchUp( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::UP ].pIndexBuffer );
		_CalcIBStitchUpLeft( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::UP_LEFT ].pIndexBuffer );
		_CalcIBStitchUpRight( uiLOD, m_vIndexBuffers[ uiIBID + MipMapTerrainStitchTypes::UP_RIGHT ].pIndexBuffer );
	
		return XST_OK;
	}

	i32	CMipMapPagingTerrain::SetIndexBufferData()
	{

		for(u32 i = 0; i < m_Options.uiLODCount; ++i)
		{
			if( XST_FAILED( SetIndexBufferData( i ) ) )
			{
				return XST_FAIL;
			}
		}
		return XST_OK;
	}

	i32	CMipMapPagingTerrain::UnlockIndexBuffers()
	{
		IBVec::iterator Itr;
		u32 i = 0;
		xst_stl_foreach( Itr, m_vIndexBuffers )
		{
			if( XST_FAILED( (*Itr).pIndexBuffer->Unlock() ) )
			{
				return XST_FAIL;
			}
			++i;
		}

		return XST_OK;
	}

	i32 CMipMapPagingTerrain::CreateTilesLODs()
	{
		{
			//XSTSimpleProfiler(); //~32sec in debug for 31x31 tiles

		const int iMaxLODCount = m_Options.uiLODCount * MipMapTerrainStitchTypes::_MAX_COUNT;
		SMeshLOD LOD;
		int iCurrLOD = 0;
		CMipMapTerrainTile* pCurrTile;
		CMesh* pCurrMesh;

		for(u32 t = 0; t < m_vTiles.size(); ++t)
		{
			pCurrTile = m_vTiles[t];
			pCurrMesh = pCurrTile->m_pMesh.GetPointer();
			pCurrMesh->SetLODCount( iMaxLODCount );
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

						//SMeshLOD& LOD = m_vTiles[ t ]->m_pMesh->AddLOD( iCurrLOD++ );
						//LOD.pIndexBuffer = IBuffer.pIndexBuffer;
						//LOD.pIndexBuffer = IBuffer.pIndexBuffer;
						//LOD.pVertexBuffer = m_vTiles[ t ]->m_pMesh->GetVertexBuffer();
						//m_vTiles[ t ]->m_pMesh->SetLOD( iCurrLOD++, LOD );
					}

					iCurrLOD++;
				}
			}

			m_vTiles[ t ]->SetLOD( 0, 0, MipMapTerrainStitchTypes::NONE );
			//DebugPrintIndexData( m_vTiles[ t ]->m_pMesh->GetIndexBuffer()->GetIndexData() );
			//DebugPrintVB( m_vTiles[ t ]->m_pMesh );
			//DebugPrintIB( m_vTiles[ t ]->m_pMesh );
		}
		}
		return XST_OK;
	}

	void CMipMapPagingTerrain::SetLOD(u32 uiLOD)
	{
		TileVec::iterator Itr;
		xst_stl_foreach( Itr, m_vTiles )
		{
			(*Itr)->SetLOD( uiLOD, uiLOD, MipMapTerrainStitchTypes::NONE );
		}
	}

	i32 CMipMapPagingTerrain::ValidateOptions(STerrainOptions* pOptionsOut)
	{
		i32 iResult = XST_OK;

		if( pOptionsOut->TileVertexCount.x % 2 == 0 )
		{
			pOptionsOut->TileVertexCount.x -= 1;
		}

		if( pOptionsOut->TileVertexCount.y % 2 == 0 )
		{
			pOptionsOut->TileVertexCount.y -= 1;
		}

		if( pOptionsOut->PageVertexCount.x % 2 == 0 )
		{
			pOptionsOut->TileVertexCount.x -= 1;
		}

		if( pOptionsOut->PageVertexCount.y % 2 == 0 )
		{
			pOptionsOut->TileVertexCount.y -= 1;
		}

		u32 uiVertCount = pOptionsOut->PageVertexCount.x - 1 * pOptionsOut->PageVertexCount.y - 1; //convert to even values
		u32 uiTileVertCount = pOptionsOut->TileVertexCount.x - 1 * pOptionsOut->TileVertexCount.y - 1; //convert to even values



		return iResult;
	}

	void CMipMapPagingTerrain::_OnAddToRenderQueue(CRenderQueueElement* pQueue)
	{
		XSTSimpleProfiler();
		for(u32 i = 0; i < m_vTiles.size(); ++i)
		{
			pQueue->AddObject( m_vTiles[ i ]->m_pMesh );
		}
	}

	void CMipMapPagingTerrain::_SetSceneNode(CSceneNode* pNode)
	{
		XSTSimpleProfiler();
		IRenderableObject::_SetSceneNode( pNode );
		for(u32 i = 0; i < m_vTiles.size(); ++i)
		{
			//m_vTiles[ i ]->m_pMesh->_SetSceneNode( pNode );
			pNode->AddObject( m_vTiles[ i ]->m_pMesh );
		}
	}

	#define LEFT_UP		0
	#define RIGHT_UP	1
	#define RIGHT_DOWN	2
	#define LEFT_DOWN	3

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

	void CMipMapPagingTerrain::_CalcBaseIBStitch(u32 uiLOD, IndexBufferPtr& pIB, pfnCalcIBStitch Func1,  pfnCalcIBStitch Func2)
	{
		u32 ulQuad = 0;
		cu32 uiLODStep = CalcLODStep( uiLOD );
		bool bIsLadLOD = (uiLOD == m_Options.uiMaxLODCount - 1);
		// If this lod is the las lod there is no next lod step
		cu32 uiNextLODStep = ( bIsLadLOD ) ? uiLODStep : CalcLODStep( uiLOD + 1 );
		/*cu32 uiHeight = m_Options.TileVertexCount.y - uiLODStep;
		cu32 uiWidth = m_Options.TileVertexCount.x - uiLODStep;
		cu32 uiLastX = uiWidth - uiLODStep;
		cu32 uiLastY = uiHeight - uiLODStep;*/
		cu32 uiHeight = m_Options.TileVertexCount.y - 1;
		cu32 uiWidth = m_Options.TileVertexCount.x - 1;
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

		if( bIsLadLOD )
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

	void CMipMapPagingTerrain::_CalcIBStitchNone(u32 uiLOD, IndexBufferPtr& pIB)
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

	void CMipMapPagingTerrain::_CalcBaseIBStitchRight(CIndexData* pData, const SCalcIBStitchInfo& Info)
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
				pData->SetIndex( uiPos, uiId );
			}
			else
			{
				//Left up vertex is at index -2
				/* \| */
				uiPos = Info.uiCurrID - 2;
				uiId = CALC_XY( Info.uiCurrX + Info.uiLODStep, Info.uiCurrY + Info.uiLODStep, Info.uiVertCountX );
				pData->SetIndex( uiPos, uiId );
			}
		}
	}

	void CMipMapPagingTerrain::_CalcBaseIBStitchLeft(CIndexData* pData, const SCalcIBStitchInfo& Info)
	{
		if( Info.uiCurrX == 0 )
		{
			u32 uiPos, uiId;
			//For even strip
			//if( Info.uiCurrY % 2 == 0 )
			if( Info.uiCurrItrY % 2 == 0 )
			{
				//Left down vertex is at index -5
				/* |\ */
				uiPos = Info.uiCurrID - 6;
				uiId = CALC_XY( Info.uiCurrX, Info.uiCurrY + Info.uiNextLODStep, Info.uiVertCountX );
				pData->SetIndex( uiPos, uiId );
			}
			else
			{
				//Left up vertex is at index -5
				/* |/ */
				uiPos = Info.uiCurrID - 5;
				uiId = CALC_XY( Info.uiCurrX + Info.uiLODStep, Info.uiCurrY, Info.uiVertCountX );
				pData->SetIndex( uiPos, uiId );
			}
		}
	}

	void CMipMapPagingTerrain::_CalcBaseIBStitchUp(CIndexData* pData, const SCalcIBStitchInfo& Info)
	{
		if( Info.uiCurrY == 0 ) //if this is the first row
		{
			u32 uiPos, uiId;
			//For even quads change right up vertex of the second triangle
			//if( Info.uiCurrX % 2 == 0 )
			if( Info.uiCurrItrX % 2 == 0 )
			{
				/* \| change to \/ */
				//Right up vertex of the second triangle is at position -2
				uiPos = Info.uiCurrID - 2;
				uiId = CALC_XY( Info.uiCurrX + Info.uiNextLODStep, Info.uiCurrY, Info.uiVertCountX );
				pData->SetIndex( uiPos, uiId );
			}
			else //for odd quads degenerate first triangle
			{
				/* |/ change to / */
				//Left up vertex of the first triangle is at position -5
				uiPos = Info.uiCurrID - 5;
				uiId = CALC_XY( Info.uiCurrX + Info.uiLODStep, Info.uiCurrY, Info.uiVertCountX );
				pData->SetIndex( uiPos, uiId );
			}
		}
	}

	void CMipMapPagingTerrain::_CalcBaseIBStitchDown(CIndexData* pData, const SCalcIBStitchInfo& Info)
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
				pData->SetIndex( uiPos, uiId );
			}
			else //for odd quads degenerate first triangle
			{
				/* |\ change to \ */
				//Left up vertex of the first triangle is at position -5
				uiPos = Info.uiCurrID - 6;
				uiId = CALC_XY( Info.uiCurrX, Info.uiCurrY, Info.uiVertCountX );
				pData->SetIndex( uiPos, uiId );
			}
		}
	}

	void CMipMapPagingTerrain::_CalcIBStitchDown(u32 uiLOD, IndexBufferPtr& pIB)
	{
		_CalcBaseIBStitch( uiLOD, pIB, &CMipMapPagingTerrain::_CalcBaseIBStitchDown );
	}

	void CMipMapPagingTerrain::_CalcIBStitchDownLeft(u32 uiLOD, IndexBufferPtr& pIB)
	{
		_CalcBaseIBStitch( uiLOD, pIB, &CMipMapPagingTerrain::_CalcBaseIBStitchDown, &CMipMapPagingTerrain::_CalcBaseIBStitchLeft );
	}

	void CMipMapPagingTerrain::_CalcIBStitchLeft(u32 uiLOD, IndexBufferPtr& pIB)
	{
		_CalcBaseIBStitch( uiLOD, pIB, &CMipMapPagingTerrain::_CalcBaseIBStitchLeft );
	}

	void CMipMapPagingTerrain::_CalcIBStitchRight(u32 uiLOD, IndexBufferPtr& pIB)
	{
		_CalcBaseIBStitch( uiLOD, pIB, &CMipMapPagingTerrain::_CalcBaseIBStitchRight );
	}

	void CMipMapPagingTerrain::_CalcIBStitchRightDown(u32 uiLOD, IndexBufferPtr& pIB)
	{
		_CalcBaseIBStitch( uiLOD, pIB, &CMipMapPagingTerrain::_CalcBaseIBStitchDown, &CMipMapPagingTerrain::_CalcBaseIBStitchRight );
	}

	void CMipMapPagingTerrain::_CalcIBStitchUp(u32 uiLOD, IndexBufferPtr& pIB)
	{
		_CalcBaseIBStitch( uiLOD, pIB, &CMipMapPagingTerrain::_CalcBaseIBStitchUp );
	}

	void CMipMapPagingTerrain::_CalcIBStitchUpLeft(u32 uiLOD, IndexBufferPtr& pIB)
	{
		_CalcBaseIBStitch( uiLOD, pIB, &CMipMapPagingTerrain::_CalcBaseIBStitchUp, &CMipMapPagingTerrain::_CalcBaseIBStitchLeft );
	}

	void CMipMapPagingTerrain::_CalcIBStitchUpRight(u32 uiLOD, IndexBufferPtr& pIB)
	{
		_CalcBaseIBStitch( uiLOD, pIB, &CMipMapPagingTerrain::_CalcBaseIBStitchUp, &CMipMapPagingTerrain::_CalcBaseIBStitchRight );
	}

}//xse