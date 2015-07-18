#include "XSECTErrainSystem.h"
#include "XSECSceneManager.h"
#include "XSECImageManager.h"
#include "XSECMipMapPagingTerrain.h"
#include "XSEIRenderSystem.h"
#include "XSEIInputLayout.h"
#include "XSECMeshManager.h"
#include "XSECMesh.h"
#include "XSECShaderManager.h"
#include "XSECMeshManager.h"
#include "XSECModelManager.h"
#include "XSECSceneNode.h"
#include "XSECStaticGeometryGroup.h"

#pragma warning( disable: 4244 )
#pragma warning( disable: 4305 )
#pragma warning( disable: 4018 )
#pragma warning( disable: 4996 )

namespace XSE
{



    CTerrainSystem::CTerrainSystem(CSceneManager* pSceneMgr, CImageManager* pImgMgr, IRenderSystem* pRS) :
        m_pSceneMgr( pSceneMgr ),
        m_pImgMgr( pImgMgr ),
        m_pRenderSystem( pRS ),
        m_pDbgGeom( xst_null )
    {
    }

    CTerrainSystem::~CTerrainSystem()
    {
        DestroyTerrains();
    }

    void CTerrainSystem::DestroyTerrains()
    {
        //Destroy terrain
        TerrainVec::iterator Itr;
        xst_stl_foreach( Itr, m_vTerrains )
        {
            DestroyTerrain( (*Itr) );
        }

        m_vTerrains.clear();
    }

    void CTerrainSystem::Update()
    {
        for( auto pTerrain : m_vTerrains )
        {
            pTerrain->Update();
        }
    }

    ITerrain*	CTerrainSystem::CreateTerrain(xst_castring& strName, STerrainOptions& Options, bool bLockOnly)
    {
        XSTSimpleProfiler();

        //Validate options
        if( !ValidateOptions( Options ) )
        {
            return xst_null;
        }
        
        //Create input layout
        u32 uiEls = XSE::ILE::POSITION;
        //uiEls |= XSE::ILE::TEXCOORD0;

        if( Options.bNormal )	uiEls |= XSE::ILE::NORMAL;
        if( Options.bBinormal )	uiEls |= XSE::ILE::BINORMAL;
        if( Options.bTangent )	uiEls |= XSE::ILE::TANGENT;
        if( Options.bColor )	uiEls |= XSE::ILE::COLOR;
        if( Options.bTexCoord )	uiEls |= XSE::ILE::TEXCOORD0;

        IInputLayout* pIL = m_pRenderSystem->GetInputLayout( uiEls );
        xst_assert2( pIL );

        //Create terrain
        CMipMapPagingTerrain* pTerrain = xst_new CMipMapPagingTerrain( strName, pIL, m_pSceneMgr );
        m_vTerrains.push_back( pTerrain );
        
        if( XST_FAILED( pTerrain->Init( Options ) ) )
        {
            return xst_null;
        }

        if( XST_FAILED( pTerrain->CreateIndexBuffers() ) )
        {
            return xst_null;
        }

        if( XST_FAILED( pTerrain->LockIndexBuffers() ) )
        {
            return xst_null;
        }

        if( XST_FAILED( pTerrain->CalcIndexBufferData() ) )
        {
            return xst_null;
        }

        if( XST_FAILED( pTerrain->UnlockIndexBuffers() ) )
        {
            return xst_null;
        }

        if( XST_FAILED( pTerrain->CreateVertexBuffers() ) )
        {
            return xst_null;
        }

        if( XST_FAILED( pTerrain->LockVertexBuffers() ) )
        {
            return xst_null;
        }

        if( XST_FAILED( pTerrain->CalcVertexData() ) )
        {
            return xst_null;
        }

        if( XST_FAILED( pTerrain->UnlockVertexBuffers() ) )
        {
            return xst_null;
        }

        if( XST_FAILED( pTerrain->CreateImpostors() ) )
        {
            return xst_null;
        }

        return pTerrain;

    }

    ITerrain*	CTerrainSystem::CreateTerrain(xst_castring& strName, STerrainOptions& Options)
    {
        return CreateTerrain( strName, Options, false );
    }

            
    ITerrain*	CTerrainSystem::CreateFromImage(xst_castring& strName, const Resources::IImage* pImg2, STerrainOptions& Options)
    {
        CMipMapPagingTerrain* pTerrain = (CMipMapPagingTerrain*)CreateTerrain( strName, Options, true );
/*
        //Create threads
        xst_vector< CVertexThread* > vThreads;
        vThreads.reserve( pTerrain->m_vTiles.size() );

        //Scale image to terrain size
        ImagePtr pTmpImg = pImg2->Clone();
        if( pTmpImg == xst_null )
        {
            return xst_null;
        }

        CPoint Scale;
        Scale.x = (u32)std::min< u32 >( Options.Size.x, 5000 );
        Scale.y = (u32)std::min< u32 >( Options.Size.y, 5000 );
        pTmpImg->Scale( Scale.x, Scale.y );

        Resources::IImage* pImg = pTmpImg.GetPtr();

        Vec2 vecTexelSize( 1.0f / pImg->GetWidth(), 1.0f / pImg->GetHeight() );
        Vec3 vecPos;
        Vec2 vecTC;
        f32 fColTime = 0;
        f32 fNormalTime = 0;
        XST::CTimer Timer;
        f32 fMinHeight = 0;
        f32 fMaxHeight = 0;
        f32 fAvgHeight;

        CStaticGeometryGroup* pGeomGr = xst_null;
        MeshPtr pBox;
        //m_pDbgGeom = m_pSceneMgr->CreateStaticGeometry( "dbg" );
        if( m_pDbgGeom )
        {
            pGeomGr = m_pDbgGeom->CreateGroup();
            IInputLayout* pBoxIL = m_pRenderSystem->GetInputLayout( ILE::POSITION );
            SBoxOptions BoxOptions;
            BoxOptions.Size = 15;//Vec3( pTile->m_Sphere.GetRadius() );
            pBox = XSE::CMeshManager::GetSingleton().CreateMesh( "DbgBox", pBoxIL, BasicShapes::BOX, &BoxOptions ); 
        }

        //CPoint MinMax( 0, 3 );
        CPoint MinMax( 0, pTerrain->m_vTiles.size() );
        for(u32 i = MinMax.x; i < MinMax.y; ++ i)
        {

            //i = 13;
            CMipMapTerrainTile* pTile = pTerrain->m_vTiles[ i ];
            const STerrainOptions* pOptions = &Options;

            IVertexBuffer* pVB = pTile->GetVertexBuffer();
            CVertexData& VData = pVB->GetVertexData();

            ul32 ulVertex = 0;
            
            Timer.StartQPerf();

            //Get the first vertex
            VData.GetPosition( ulVertex, &vecPos );
            vecTC.x = vecPos.x / pOptions->Size.x;
            vecTC.y = vecPos.z / pOptions->Size.y;		
            XST::CColor Col = pImg->GetColor( std::min< i32 >( vecTC.x * pImg->GetWidth(), pImg->GetWidth() - 1 ), std::min< i32 >( vecTC.y * pImg->GetHeight(), pImg->GetHeight() - 1 ) );
            vecPos.y += Col.r;
            fMinHeight = fMaxHeight = vecPos.y;

            for(u32 y = 0; y < pOptions->TileVertexCount.y; ++y)
            {
                for(u32 x = 0; x < pOptions->TileVertexCount.x; ++x)
                {
                    //Calculate height
                    VData.GetPosition( ulVertex, &vecPos );
                    vecTC.x = vecPos.x / pOptions->Size.x;
                    vecTC.y = vecPos.z / pOptions->Size.y;
                    
                    //XST::CColor Col( ( Col1.r + Col2.r ) * 0.5f, ( Col1.g + Col2.g ) * 0.5f, ( Col1.b + Col2.b ) * 0.5f, ( Col1.a + Col2.a ) * 0.5f );
                    XST::CColor Col = pImg->GetColor( std::min< i32 >( vecTC.x * pImg->GetWidth(), pImg->GetWidth() - 1 ), std::min< i32 >( vecTC.y * pImg->GetHeight(), pImg->GetHeight() - 1 ) );
                    //VData.SetColor( ulVertex, Col.ToVector4() + 0.1f );
                    //Set the y value
                    vecPos.y += Col.r;
                    VData.SetPosition( ulVertex, vecPos );

                    if( vecPos.y < fMinHeight ) fMinHeight = vecPos.y;
                    if( vecPos.y > fMaxHeight ) fMaxHeight = vecPos.y;

                    ++ulVertex;
                    //XST::CDebug::PrintDebugLN( XST::StringUtil::ToString( uiX ) );
                }
            }

            //Calc avg height
            fAvgHeight = ( fMinHeight + fMaxHeight ) * 0.5f;
            //Update bounding sphere and box
            pTile->m_Sphere.SetCenterPoint( Vec3( pTile->m_Sphere.GetCenterPoint().x, fAvgHeight, pTile->m_Sphere.GetCenterPoint().z ) );

            //Dbg tmp
            //IInputLayout* pBoxIL = m_pRenderSystem->GetInputLayout( ILE::POSITION );
            //SBoxOptions Options;
            //Options.Size = 15;//Vec3( pTile->m_Sphere.GetRadius() );
            //ModelPtr pBox = XSE::CModelManager::GetSingleton().CreateModel( XST::StringUtil::ToString( i ) + "Box", BasicShapes::BOX, pBoxIL, &Options ); 
            //m_pSceneMgr->GetRootNode()->CreateChildNode( pBox->GetName() )->AddObject( pBox );
            //pBox->SetPosition( pTile->m_Sphere.GetCenterPoint() );
            const Vec3& vecBoxPos = pTile->m_Sphere.GetCenterPoint();
            if( pGeomGr ) pGeomGr->AddMesh( pBox, pBox->GetMaterial(), vecBoxPos );
            //////////////////////////////////////////////////////////////////////////////////////////////////
            
            fColTime += Timer.GetQPerfElapsedTime();
            Timer.StartQPerf();

            //Normals
            if( !pTerrain->m_pInputLayout->IsNormal() )
            {
                //Get index data with max lod
                CMipMapTerrainTile* pTile = pTerrain->m_vTiles[ i ];
                
                CIndexData& IData = pTile->m_vIndexBuffers[ 0 ]->GetIndexData();
                CVertexData& VData = pTile->m_pVBuffer->GetVertexData();

                u32 auiIndices[3]; 
                Vec3 vecTriangle[3];
                Vec3 vecTmps[2];
                Vec3 vecNormal;

                auiIndices[ 0 ] = IData.GetIndex( 0 );
                auiIndices[ 1 ] = IData.GetIndex( 1 );

                bool bVisualizeNormals = false;
                //Test
                if( bVisualizeNormals )
                {
                    pTile->m_pVBNormals = m_pRenderSystem->CreateVertexBuffer();
                    IInputLayout* pNormalIL = m_pRenderSystem->GetInputLayout( ILE::POSITION );
                    pTile->m_pVBNormals->SetInputLayout( pNormalIL );
                    pTile->m_pVBNormals->SetTopologyType( TopologyTypes::LINE_LIST );
                    pTile->m_pVBNormals->SetUsage( BufferUsages::STATIC );
                    pTile->m_pVBNormals->SetVertexCount( pTile->m_vecVertexCount.x * pTile->m_vecVertexCount.y * 2 );
                    if( XST_FAILED( pTile->m_pVBNormals->Lock() ) )
                    {
                        xst_release( pTile->m_pVBNormals );
                    }
                }

                CVertexData* pNormalData = xst_null;
                if( pTile->m_pVBNormals )
                {
                    pNormalData = &pTile->m_pVBNormals->GetVertexData();
                }

                pTile->m_pNormalVS = CShaderManager::GetSingletonPtr()->GetDefaultVertexShader().GetPtr();
                pTile->m_pNormalPS = CShaderManager::GetSingletonPtr()->GetDefaultPixelShader().GetPtr();
                
                for(u32 id = 2; id < IData.GetIndexCount() - 3; ++id)
                {
                    auiIndices[ 2 ] = IData.GetIndex( id );

                    VData.GetPosition( auiIndices[ 0 ], &vecTriangle[ 0 ] );
                    VData.GetPosition( auiIndices[ 1 ], &vecTriangle[ 1 ] );
                    VData.GetPosition( auiIndices[ 2 ], &vecTriangle[ 2 ] );

                    vecTmps[ 0 ] = vecTriangle[ 1 ] - vecTriangle[ 0 ];
                    vecTmps[ 1 ] = vecTriangle[ 2 ] - vecTriangle[ 0 ];
                    vecNormal = vecTmps[ 0 ].Cross( vecTmps[ 1 ] );
                    vecNormal.Normalize();

                    VData.SetNormal( auiIndices[ 0 ], vecNormal );
                    VData.SetNormal( auiIndices[ 1 ], vecNormal );
                    VData.SetNormal( auiIndices[ 2 ], vecNormal );

                    auiIndices[ 0 ] = auiIndices[ 1 ];
                    auiIndices[ 1 ] = auiIndices[ 2 ];
                }

                if( pTile->m_pVBNormals )
                {
                    Vec3 vecTmp;
                    Vec3 vecLine[2];
                    Vec3 vecNormal;
                    ul32 ulCurrVertex = 0;
                    for(u32 vId = 0; vId < VData.GetVertexCount(); ++vId)
                    {
                        const Vec3 vecNormalSize( 5, 5, 5 );
                        VData.GetPosition( vId, &vecTmp );
                        VData.GetNormal( vId, &vecNormal );
                        vecLine[ 0 ] = vecTmp;
                        vecLine[ 1 ] = vecTmp + ( vecNormal * vecNormalSize );
                        pNormalData->SetPosition( ulCurrVertex++, vecLine[ 0 ] );
                        pNormalData->SetPosition( ulCurrVertex++, vecLine[ 1 ] );
                    }

                    pTile->m_pVBNormals->Unlock();
                    if( XST_FAILED( pTile->m_pVBNormals->Create() ) )
                    {
                        xst_release( pTile->m_pVBNormals );
                    }
                }
            }
            fNormalTime += Timer.GetQPerfElapsedTime();

        }

        //Wait for threads
        for(u32 i = 0; i < vThreads.size(); ++i)
        {
            while( !vThreads[ i ]->bFinished ) {}
            xst_delete( vThreads[ i ] );
        }

        if( m_pDbgGeom ) 
        {
            m_pDbgGeom->Build();
            m_pSceneMgr->GetRootNode()->AddObject( m_pDbgGeom );
            XSE::CMeshManager::GetSingleton().DestroyMesh( pBox );
        }
        
        if( XST_FAILED( LockTerrain( pTerrain ) ) )
        {
            xst_delete( pTerrain );
        }
*/
        return pTerrain;
    }
            
    ITerrain*	CTerrainSystem::CreateFromBinary(xst_castring& strName, const XST::CBinaryData& Data, STerrainOptions& Options)
    {
        return 0;
    }

    i32 CTerrainSystem::LockTerrain(ITerrain* pTerrain)
    {
        return XST_OK;
    }
            
    i32			CTerrainSystem::SaveToImage(xst_castring& strImg, const ITerrain* pTerrain)
    {
        return 0;
    }
            
    i32			CTerrainSystem::SaveToBinary(xst_castring& strBinaryName, const ITerrain* pTerrain)
    {
        return 0;
    }

    void		CTerrainSystem::DestroyTerrain(ITerrain* pTerrain)
    {
        pTerrain->DestroyData();
        xst_delete( pTerrain );
    }

    void CTerrainSystem::DestroyTerrain(xst_castring& strName)
    {
        TerrainMap::iterator Itr = m_mTerrains.find( strName );
        if( Itr == m_mTerrains.end() )
        {
            return;
        }

        DestroyTerrain( Itr->second );
    }

    bool CTerrainSystem::ValidateOptions(STerrainOptions& Options)
    {
        //Check max vertex count
        u32 uiMaxVertCount = Options.PageVertexCount.x * Options.PageVertexCount.y;
        CPoint LODPageVertCount( ( Options.TileVertexCount.x * Options.Size.x ) / Options.PageVertexCount.x, ( Options.TileVertexCount.y * Options.Size.y ) / Options.PageVertexCount.y );
        ul32 usMaxVertCountPerLODPage = LODPageVertCount.x * LODPageVertCount.y;
        //Max vertex count is size of unsigned short
        ul32 usMax = -1; 

        //Calculate max lod count
        //If there is too few vertices for lod count max lod count should be set to correctly noumber
        Options.uiMaxLODCount = Options.uiLODCount;

        //Get max lod count
        for(i32 i = 0; i < Options.uiLODCount; ++i)
        {
            //Calc vertex step for max lod
            u32 uiLOD = (u32)pow( 2.0, i );
            //Check if it is divide with no rest
            if( ( (u32)( Options.TileVertexCount.x - 1 ) % uiLOD == 0 && (u32)( Options.TileVertexCount.y - 1 ) % uiLOD == 0 ) )
                //&& ( (u32)( Options.TileVertexCount.x - 1 ) / uiLOD > 1 && (u32)( Options.TileVertexCount.y - 1 ) / uiLOD > 1 ) )
            {
                //If it is fit the lod count is correctly
                //Set the max lod
                Options.uiMaxLODCount = i + 1;	
                //Continue to the next level
                continue;
            }
            else
            {
                //If this lod is not correctly for tile vertex count break it
                break;
            }
        }

        /*if( uiMaxVertCount > usMax )
        {
            XST_LOG_ERR( "Vertex count for the terrain is too high" );
            return false;
        }*/

        if( usMaxVertCountPerLODPage > usMax )
        {
            XST_LOG_ERR( "LOD page size has too many vertices" );
            return false;
        }

        return true;
    }

}//xse