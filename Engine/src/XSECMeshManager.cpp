#include "XSECMeshManager.h"
#include "XSECMesh.h"
#include "XSEIRenderSystem.h"
#include "XSECMaterialManager.h"
#include "XSEIInputLayout.h"
#include "XSEIVertexBuffer.h"
#include "XSEIIndexBuffer.h"
#include "XSECEngine.h"
#include <XST/XSTCToString.h>

namespace XSE
{


    i32 CreateBox(Resources::CMesh* pMesh, const IInputLayout* pIL, const SBoxOptions& Options);
    i32 CreateBox(Resources::CMesh* pMesh, const IInputLayout* pIL, const SLineBoxOptions& Options);
    i32 CreatePlane(Resources::CMesh* pMesh, const IInputLayout* pIL, const SPlaneOptions& Opts);
    i32 CreateRect2D(Resources::CMesh* pMesh,  const IInputLayout* pIL, const SRect2DOptions& Opts);
    i32 CreateCircle(Resources::CMesh* pMesh, const  IInputLayout* pIL, const SCircleOptions& Opts );

    ch8 g_astrName[ 128 ];
    xst_astring g_strName;

    CMeshManager::CMeshManager(CMaterialManager* pMatMgr)
    {
        xst_assert( pMatMgr, "(CMeshManager::CMeshManager)" );
        m_pMatMgr = pMatMgr;
        m_pRenderSystem = xst_null;
        m_pDefaultIL = xst_null;

        /*this->m_OnResDestroy.bind( this, &CMeshManager::_OnResDestroy );
        this->m_OnResRemove.bind( this, &CMeshManager::_OnResRemove );
        this->_SetOnRemoveDestroyDelegates();

        this->m_OnResItr.bind( this, &CMeshManager::_DestroyMeshBuffers );
        this->_SetOnResourceIterationDelegate();*/

        g_strName.reserve( 256 );

        SetDefaultMeshLODCount( 5 );
    }

    CMeshManager::~CMeshManager()
    {		
    }


    void CMeshManager::_DestroyMeshBuffers(ResourcePtr pRes)
    {
        Resources::CMesh* pMesh = (Resources::CMesh*)pRes.GetPtr();
        pMesh->DestroyBuffers();
    }

    void CMeshManager::_OnResDestroy(ResourcePtr pRes)
    {
        MeshPtr pMesh = pRes;
        pMesh->DestroyBuffers();
    }
                        
    void CMeshManager::_OnResRemove(ResourcePtr pRes)
    {
        //MeshPtr pMesh = pRes;
        //xst_release( pMesh );
    }

    void CMeshManager::_OnDestroy()
    {
        m_pDefaultMesh = xst_null;
        TCFreeListAllocator< Resources::CMesh >::Destroy();
    }

    ResourceWeakPtr CMeshManager::CloneResource(const Resources::IResource* pSrcRes, xst_castring& strNewName /* = XST::StringUtil::EmptyAString */, bool bFullClone /* = true */)
    {
        XSTSimpleProfiler();
        const Resources::CMesh* pSrcMesh = (const Resources::CMesh*)pSrcRes;
        ul32 ulID = XST::CTime::GetQPerfTickCount();

        if( strNewName.empty() )
        {
            //this->m_ssTmpName.str( XST::StringUtil::EmptyAString );
            //this->m_ssTmpName << "xse_mesh_clone_" << ulID;
            xst_sprintf( g_astrName, 128, "xse_mesh_clone_%d", ulID );
            g_strName.assign( g_astrName );
        }

        ResourceWeakPtr pNewRes( this->CreateMesh( g_strName/*this->m_ssTmpName.str()*/, this->GetGroup( pSrcMesh->GetResourceGroupHandle() ) ) );
    
        if( pNewRes == xst_null )
        {
            return pNewRes;
        }

        if( XST_FAILED( PrepareResource( &pNewRes ) ) )
        {
            this->DestroyResource( pNewRes );
            return ResourcePtr();
        }

        { XSTSimpleProfiler2( "clone mesh: set data" );
        Resources::CMesh* pNewMesh = (Resources::CMesh*)pNewRes.GetPtr();
        
        //if( bFullClone )
        {
            for(int i = 0; i < pSrcMesh->m_vLODs.size(); ++i)
            {
                /*SMeshLOD* pCurrLOD = pSrcMesh->m_vLODs[ i ];
                SMeshLOD* pNewLOD = pNewMesh->CreateLOD();
                pNewLOD->byID = pCurrLOD->byID;
                pNewLOD->fDistance = pCurrLOD->fDistance;
                pNewLOD->pIndexBuffer = pCurrLOD->pIndexBuffer;
                pNewLOD->pMaterial = pCurrLOD->pMaterial;
                pNewLOD->pVertexBuffer = pCurrLOD->pVertexBuffer;
                pNewLOD->ulBeginIndexId = pCurrLOD->ulBeginIndexId;
                pNewLOD->ulBeginVertexId = pCurrLOD->ulBeginVertexId;
                pNewLOD->ulEndIndexId = pCurrLOD->ulEndIndexId;
                pNewLOD->ulEndVertexId = pCurrLOD->ulEndVertexId;
                pNewMesh->AddLOD( pNewLOD );*/

                const SMeshLOD* pCurrLOD = &pSrcMesh->m_vLODs[ i ];
                SMeshLOD& NewLOD = pNewMesh->AddLOD();
                NewLOD.byID = pCurrLOD->byID;
                NewLOD.fDistance = pCurrLOD->fDistance;
                NewLOD.pIndexBuffer = pCurrLOD->pIndexBuffer;
                NewLOD.pMaterial = pCurrLOD->pMaterial;
                NewLOD.pVertexBuffer = pCurrLOD->pVertexBuffer;
                NewLOD.ulBeginIndexId = pCurrLOD->ulBeginIndexId;
                NewLOD.ulBeginVertexId = pCurrLOD->ulBeginVertexId;
                NewLOD.ulEndIndexId = pCurrLOD->ulEndIndexId;
                NewLOD.ulEndVertexId = pCurrLOD->ulEndVertexId;
            }
        }
        /*else
        {
            SMeshLOD& LOD = pNewMesh->AddLOD();

        }*/

        pNewMesh->m_ulCloneId = ulID;
        pNewMesh->m_bIsCloned = true;
        pNewMesh->m_bIndexedGeometry = pSrcMesh->m_bIndexedGeometry;

        pNewMesh->SetLOD( pSrcMesh->GetCurrentLODID() );
        pNewMesh->SetResource( pSrcMesh );
        pNewMesh->SetRenderableObject( pSrcMesh );
        }
        return pNewRes;
    }

    i32	CMeshManager::PrepareResource(ResourceWeakPtr *const ppRes)
    {
        xst_assert2( ppRes && (*ppRes).IsValid() );
        auto pRes = *ppRes;
        Resources::CMesh* pMesh = (Resources::CMesh*)pRes.GetPtr();
        //Resource has to be in created state
        if( pMesh->GetResourceState() != Resources::ResourceStates::CREATED )
            return XST_FAIL;

        //pMesh->m_vLODs.reserve( GetDefaultMeshLODCount() );
        pMesh->_SetResourceState( Resources::ResourceStates::PREPARED );
        return XST_OK;
    }

    i32 CMeshManager::Init()
    {
        xst_assert( m_pRenderSystem ,"(CMeshManager::Init()" );
        m_pDefaultIL = m_pRenderSystem->GetInputLayout( ILE::POSITION );

        TCFreeListAllocator< Resources::CMesh >::Create( CEngine::GetSingletonPtr()->GetSettings().MemSettings.ulMeshCount );

        SBoxOptions Options;
        m_pDefaultMesh = CreateMesh( "xse_default_mesh", m_pDefaultIL, BasicShapes::BOX, &Options );
        if( m_pDefaultMesh == xst_null )
        {
            return XST_FAIL;
        }

        return XST_OK;
    }

    i32 CMeshManager::SetDefaultMesh(MeshWeakPtr pMesh)
    {
        m_pDefaultMesh->DestroyBuffers();
        m_pDefaultMesh = pMesh;
        m_pDefaultMesh->SetMaterial( m_pMatMgr->GetDefaultMaterial() );
        return XST_OK;
    }

    MeshWeakPtr	CMeshManager::CreateMesh(xst_castring& strName, ul32 ulInputLayout, xst_castring& strGroupName)
    {
        IInputLayout* pIL = m_pRenderSystem->GetInputLayout( ulInputLayout );
        return CreateMesh( strName, pIL, strGroupName );
    }

    MeshWeakPtr CMeshManager::CreateMesh(xst_castring& strName, const IResourceManager::GroupHandle& GrHandle)
    {
        GroupWeakPtr pGr = this->GetGroup( GrHandle );
        return CreateMesh( strName, pGr );
    }

    MeshWeakPtr	CMeshManager::CreateMesh(xst_castring& strName, ul32 ulInputLayout, BASIC_SHAPE eShape, xst_unknown pShapeOptions, xst_castring& strGroupName)
    {
        IInputLayout* pIL = m_pRenderSystem->GetInputLayout( ulInputLayout );
        return CreateMesh( strName, pIL, eShape, pShapeOptions, strGroupName );
    }

    MeshWeakPtr	CMeshManager::CreateMesh(xst_castring& strName, const IInputLayout* pIL, BASIC_SHAPE eShape, xst_unknown pShapeOptions, xst_castring& strGroupName)
    {
        MeshWeakPtr pMesh = CreateMesh( strName, pIL, strGroupName );
        if( pMesh->m_bIsCloned )
        {
            return pMesh;
        }

        switch( eShape )
        {
            case BasicShapes::BOX:
            {
                SBoxOptions Options;
                if( pShapeOptions != xst_null ) 
                    Options = *(SBoxOptions*)pShapeOptions;
                if( XST_FAILED( CreateBox( pMesh.GetPtr(), pIL, Options ) ) )
                {
                    this->DestroyResource( pMesh );
                    pMesh = xst_null;
                }
            }
            break;

            case BasicShapes::LINE_BOX:
            {
                SLineBoxOptions Options;
                if( pShapeOptions != xst_null ) 
                    Options = *(SLineBoxOptions*)pShapeOptions;
                if( XST_FAILED( CreateBox( pMesh.GetPtr(), pIL, Options ) ) )
                {
                    this->DestroyResource( pMesh );
                    pMesh = xst_null;
                }
            }
            break;

            case BasicShapes::CIRCLE:
            {
                SCircleOptions Options;
                if( pShapeOptions != xst_null )
                    Options = *(SCircleOptions*)pShapeOptions;
                if( XST_FAILED( CreateCircle( pMesh.GetPtr(), pIL, Options ) ) )
                {
                    this->DestroyResource( pMesh );
                    pMesh = xst_null;
                }
            }
            break;

            case BasicShapes::PLANE:
            {
                SPlaneOptions Options;
                if( pShapeOptions != xst_null ) Options = *(SPlaneOptions*)pShapeOptions;
                if( XST_FAILED( CreatePlane( pMesh.GetPtr(), pIL, Options ) ) )
                {
                    this->DestroyResource( pMesh );
                    pMesh = xst_null;
                }
            }
            break;

            case BasicShapes::RECT_2D:
            {
                SRect2DOptions Options;
                if( pShapeOptions != xst_null ) Options = *(SRect2DOptions*)pShapeOptions;
                if( XST_FAILED( CreateRect2D( pMesh.GetPtr(), pIL, Options ) ) )
                {
                    this->DestroyResource( pMesh );
                    pMesh = xst_null;
                }
            }
            break;

            default:
            {
                this->DestroyResource( pMesh );
                pMesh = xst_null;
            }
            break;
        };

        return pMesh;
    }

    MeshWeakPtr	CMeshManager::CreateMesh(xst_castring& strName, const IInputLayout* pIL,  xst_castring& strGroupName)
    {
        bool bCreated = true;
        ResourceWeakPtr pRes;
        {
            //XSTSimpleProfiler2( "--GetOrCreateResource" );
        pRes = this->GetOrCreateResource( strName, strGroupName, &bCreated );//this->CreateResource( strName, strGroupName );
        }
        if( pRes.IsNull() )
        {
            return MeshWeakPtr();
        }

        MeshWeakPtr pMesh( pRes );
        pMesh->SetInputLayout( pIL );

        if( !bCreated )
        {
            return this->CloneMesh( pMesh, strGroupName );
        }

        return pMesh;
    }


    MeshWeakPtr	CMeshManager::CreateMesh(xst_castring& strName, xst_castring& strGroupName)
    {
        return CreateMesh( strName, this->GetOrCreateGroup( strGroupName ) );
    }

    MeshWeakPtr	CMeshManager::CreateMesh(xst_castring& strName, GroupWeakPtr pGroup)
    {
        //XSTSimpleProfiler();
        xst_assert( pGroup != xst_null, "(CMeshManager::CreateMesh)" );
        
        bool bCreated = true;
        ResourceWeakPtr pRes;
        //{ XSTSimpleProfiler2( "CMeshManager: get or create resource" );
        pRes = this->GetOrCreateResource( strName, pGroup, &bCreated );
        //}
        if( pRes.IsNull() )
        {
            return MeshWeakPtr();
        }

        MeshWeakPtr pMesh( pRes );

        if( !bCreated )
        {
            return this->CloneMesh( pMesh );
        }

        return pMesh;
    }

    MeshWeakPtr CMeshManager::LoadMesh(xst_castring &strName, xst_castring &strGroup)
    {
        //Try to get the mesh
        bool bCreated = true;
        MeshWeakPtr pMesh = this->GetOrCreateResource( strName, strGroup, &bCreated );
        if( pMesh.IsNull() )
        {
            //Load
        }

        if( !bCreated )
        {
            return this->CloneMesh( pMesh, strGroup );
        }

        return pMesh;
    }

    /*i32 CMeshManager::DestroyMesh(MeshPtr& pMesh, xst_castring& strGroupName)
    {
        if( strGroupName == ALL_GROUPS )
        {
            i32 iResult = 0;
            IResourceManager::_GroupIterator Itr;
            ul32 ulHandle = pMesh->GetResourceHandle();
            for( Itr = this->m_mResources.begin(); Itr != this->m_mResources.end(); ++Itr )
            {
                Itr->second->DestroyResourceByHandle( ulHandle );
            }

            return XST_OK;
        }
        else
        {
            return this->DestroyResourceByHandle( pMesh->GetResourceHandle(), strGroupName );
        }
        return XST_FAIL;
    }*/

    class Tmp : public XSE::IRenderableObject, public XSE::Resources::IResource, public XST::TIAllocableObject< TCFreeListAllocator< Tmp > >
    {
        public:

        ul32 GetObjectHandle() const
        { return 0;}

        void Render( XSE::IRenderSystem * )
        {}
        typedef XST::TCConstantArray< SMeshLOD, 20 >	LODVec;
        typedef i32 (Resources::CMesh::*pfnRender)();
        //LODVec			m_vLODs;
        SMeshLOD arr[ 200 ];
        SMeshLOD*		m_pCurrentLOD;
        IRenderSystem*	m_pRS;
        //MeshVector		m_vSubMeshes;
        pfnRender		m_RenderMethod;

        ul32			m_ulCloneId;
        bool			m_bIndexedGeometry;
        bool			m_bIsCloned;
    };
    bool bCreated = false;

    Resources::IResource* CMeshManager::_CreateResource(xst_castring &strName, cul32 &ulHandle, IResourceManager::GroupWeakPtr pGroup)
    {
        //XSTSimpleProfiler();
        xst_assert( m_pRenderSystem, "(CMeshManager::_CreateResource) Render system is not set/created or engine is not initialized" );
        Resources::CMesh* pMesh;
        {
            //XSTSimpleProfiler2("CMeshManager::_CreateResource"); //0.002 - 0.005 sec in debug
            pMesh = xst_new Resources::CMesh( m_pRenderSystem, m_pDefaultIL, this, ulHandle, strName, XST::ResourceType::MESH, XST::ResourceStates::CREATED/*, this->m_pMemoryMgr*/ );
            if( !pMesh )
            {
                XST_LOG_ERR( "Unable to create Mesh object: " << strName << ". No memory." );
                return xst_null;
            }
        }
        pMesh->SetMaterial( m_pMatMgr->GetDefaultMaterial() );
        // MEMORY ALLOCATION DEBUG
        /*if( !bCreated ){
            TCFreeListAllocator< Tmp >::Create( 2000 );
            bCreated = true;
        }
        //Set default material
        {
            XSTSimpleProfiler2("CMeshManager::_CreateResource2"); //0.002 - 0.005 sec in debug
            //pMesh = xst_new Resources::CMesh( m_pRenderSystem, m_pDefaultIL, this, ulHandle, strName, XST::ResourceType::MESH, XST::ResourceStates::CREATED, this->m_pMemoryMgr );
            //IResource* p = xst_new IResource();
            Tmp* p = xst_new Tmp();
        }*/
        return pMesh;
    }

    i32 CreateCircle( Resources::CMesh* pMesh, const IInputLayout* pIL, const SCircleOptions& Options )
    {
        xst_vector< Vec3 > vPoints;
        f32 fAngle = 0.0f;
        for( u32 i = 0; i <= 360; i += Options.uStep )
        {
            fAngle = XST::Math::DegreesToRadians( (f32)i );
            vPoints.push_back( Vec3( XST::Math::Cos( fAngle ) * Options.fRadius, XST::Math::Sin( fAngle ) * Options.fRadius, 0.0f ) );
        }

        bool bIsNormal = pIL->IsNormal();
        ul32 ulVertCount = vPoints.size();
        VertexBufferPtr pVB = pMesh->CreateVertexBuffer();

        pVB->SetTopologyType( TopologyTypes::LINE_STRIP );
        pVB->SetUsage( BufferUsages::DEFAULT );
        pVB->SetVertexCount( ulVertCount );
        pVB->SetInputLayout( pIL );

        if( XST_FAILED( pVB->Lock() ) )
        {
            return XST_FAIL;
        }

        CVertexData& Data = pVB->GetVertexData();

        ul32 vs = pIL->GetVertexSize();

        for( u32 i = 0; i < vPoints.size(); ++i )
        {
            if( pIL->IsPosition() )
            {
                Data.SetPosition( i, vPoints[ i ] + Options.vecPos );
            }
        }

        if( XST_FAILED( pVB->Unlock() ) || XST_FAILED( pVB->Create() ) )
        {
            return XST_FAIL;
        }

        return XST_OK;
    }

    i32 CreateBox(Resources::CMesh* pMesh, const IInputLayout* pIL, const SBoxOptions& Options)
    {
        bool bIsNormal = pIL->IsNormal();
        ul32 ulVertCount = ( bIsNormal )? 24 : 8; //if there are normals use more vertices
        ul32 ulIndexCount = 36;
        VertexBufferPtr pVB = pMesh->CreateVertexBuffer();
        
        pVB->SetTopologyType( TopologyTypes::TRIANGLE_LIST );
        pVB->SetUsage( BufferUsages::DEFAULT );
        pVB->SetVertexCount( ulVertCount );
        pVB->SetInputLayout( pIL );

        pVB->Lock();

        CVertexData& Data = pVB->GetVertexData();

        cf32 fUnit = 1.0f;
        ul32 vs = pIL->GetVertexSize();
        const Vec3 vecSize( Options.vecSize * 0.5f );
        const Vec3 vecPos( vecSize + Options.vecPos );

        if( pIL->IsPosition() )
        {
            if( bIsNormal )
            {
                Data.SetPosition( 0, Vec3( -vecSize.x, vecSize.y, -vecSize.z ) + Options.vecPos );
                Data.SetPosition( 1, Vec3( vecSize.x, vecSize.y, -vecSize.z ) + Options.vecPos ); 
                Data.SetPosition( 2, Vec3( vecSize.x, vecSize.y, vecSize.z ) + Options.vecPos  ); 
                Data.SetPosition( 3, Vec3( -vecSize.x, vecSize.y, vecSize.z ) + Options.vecPos );

                Data.SetPosition( 4, Vec3( -vecSize.x, -vecSize.y, vecSize.z ) + Options.vecPos );
                Data.SetPosition( 5, Vec3( -vecSize.x, -vecSize.y, -vecSize.z ) + Options.vecPos ); 
                Data.SetPosition( 6, Vec3( -vecSize.x, vecSize.y, -vecSize.z ) + Options.vecPos ); 
                Data.SetPosition( 7, Vec3( -vecSize.x, vecSize.y, vecSize.z ) + Options.vecPos );

                Data.SetPosition( 8, Vec3( vecSize.x, -vecSize.y, vecSize.z ) + Options.vecPos ); 
                Data.SetPosition( 9, Vec3( vecSize.x, -vecSize.y, -vecSize.z ) + Options.vecPos ); 
                Data.SetPosition( 10, Vec3( vecSize.x, vecSize.y, -vecSize.z ) + Options.vecPos ); 
                Data.SetPosition( 11, Vec3( vecSize.x, vecSize.y, vecSize.z ) + Options.vecPos ); 

                Data.SetPosition( 12, Vec3( -vecSize.x, -vecSize.y, -vecSize.z ) + Options.vecPos ); 
                Data.SetPosition( 13, Vec3( vecSize.x, -vecSize.y, -vecSize.z ) + Options.vecPos );
                Data.SetPosition( 14, Vec3( vecSize.x, vecSize.y, -vecSize.z ) + Options.vecPos ); 
                Data.SetPosition( 15, Vec3( -vecSize.x, vecSize.y, -vecSize.z ) + Options.vecPos );

                Data.SetPosition( 16, Vec3( -vecSize.x, -vecSize.y, vecSize.z ) + Options.vecPos );
                Data.SetPosition( 17, Vec3( vecSize.x, -vecSize.y, vecSize.z ) + Options.vecPos );
                Data.SetPosition( 18, Vec3( vecSize.x, vecSize.y, vecSize.z ) + Options.vecPos  ); 
                Data.SetPosition( 19, Vec3( -vecSize.x, vecSize.y, vecSize.z ) + Options.vecPos ); 

                Data.SetPosition( 20, Vec3( -vecSize.x, -vecSize.y, -vecSize.z ) + Options.vecPos ); 
                Data.SetPosition( 21, Vec3( vecSize.x, -vecSize.y, -vecSize.z ) + Options.vecPos ); 
                Data.SetPosition( 22, Vec3( vecSize.x, -vecSize.y, vecSize.z ) + Options.vecPos ); 
                Data.SetPosition( 23, Vec3( -vecSize.x, -vecSize.y, vecSize.z ) + Options.vecPos ); 
                
            }
            else
            {
                Data.SetPosition( 0, Vec3( -vecSize.x, vecSize.y, -vecSize.z ) + Options.vecPos ); 
                Data.SetPosition( 1, Vec3( vecSize.x, vecSize.y, -vecSize.z ) + Options.vecPos ); 
                Data.SetPosition( 2, Vec3( vecSize.x, vecSize.y, vecSize.z ) + Options.vecPos ); 
                Data.SetPosition( 3, Vec3( -vecSize.x, vecSize.y, vecSize.z ) + Options.vecPos ); 
                Data.SetPosition( 4, Vec3( -vecSize.x, -vecSize.y, -vecSize.z ) + Options.vecPos ); 
                Data.SetPosition( 5, Vec3( vecSize.x, -vecSize.y, -vecSize.z ) + Options.vecPos ); 
                Data.SetPosition( 6, Vec3( vecSize.x, -vecSize.y, vecSize.z ) + Options.vecPos ); 
                Data.SetPosition( 7, Vec3( -vecSize.x, -vecSize.y, vecSize.z ) + Options.vecPos ); 
            }
        }

        if( pIL->IsColor() )
        {
            f32 fc = 0.05f;
            for(ul32 i = 0; i < ulVertCount; ++i)
            {
                /*Data.SetColor( 0, Vec4( 1, 0, 0, 1 ) ); 
                Data.SetColor( 1, Vec4( 1, 1, 1, 1 ) ); 
                Data.SetColor( 2, Vec4( 0, 1, 0, 1 ) ); 
                Data.SetColor( 3, Vec4( 0.5, 0.3, 1, 1 ) ); */
                Data.SetColor( i, Options.colColor.ToVector4() ); 
            }
        }

        if( bIsNormal )
        {
            Data.SetNormal( 0, Vec3( -1, 1, -1 ) ); 
            Data.SetNormal( 1, Vec3( 1, 1, -1 ) ); 
            Data.SetNormal( 2, Vec3( 1, 1, 1 ) ); 
            Data.SetNormal( 3, Vec3( -1, 1, 1 ) ); 
            Data.SetNormal( 4, Vec3( -1, -1, -1 ) ); 
            Data.SetNormal( 5, Vec3( 1, -1, -1 ) ); 
            Data.SetNormal( 6, Vec3( 1, -1, 1 ) ); 
            Data.SetNormal( 7, Vec3( -1, -1, 1 ) ); 
        }

        pVB->Unlock();
        pVB->Create();

        IndexBufferPtr pIB = pMesh->CreateIndexBuffer();
        pIB->SetUsage( BufferUsages::DEFAULT );
        pIB->SetIndexCount( ulIndexCount );
        pIB->Lock();

        CIndexData& IndexData = pIB->GetIndexData();

        if( bIsNormal )
        {
            //Front
            IndexData.AddTriangle( 3, 1, 0 );
            IndexData.AddTriangle( 2, 1, 3 );
            //Back
            IndexData.AddTriangle( 6, 4, 5 );
            IndexData.AddTriangle( 7, 4, 6 );
            //Top
            IndexData.AddTriangle( 11,9,8 );
            IndexData.AddTriangle( 10,9,11 );
            //Bottom
            IndexData.AddTriangle( 14,12,13 );
            IndexData.AddTriangle( 15,12,14 );
            //Left
            IndexData.AddTriangle( 19,17,16 );
            IndexData.AddTriangle( 18,17,19 );
            //Right
            IndexData.AddTriangle( 22,20,21 );
            IndexData.AddTriangle( 23,20,22 );
        }
        else
        {
            IndexData.AddTriangle( 3, 1, 0 );
            IndexData.AddTriangle( 2, 1, 3 );

            IndexData.AddTriangle( 0, 5, 4 );
            IndexData.AddTriangle( 1, 5, 0 );

            IndexData.AddTriangle( 3, 4, 7 );
            IndexData.AddTriangle( 0, 4, 3 );

            IndexData.AddTriangle( 1, 6, 5 );
            IndexData.AddTriangle( 2, 6, 1 );

            IndexData.AddTriangle( 2, 7, 6 );
            IndexData.AddTriangle( 3, 7, 2 );

            IndexData.AddTriangle( 6, 4, 5 );
            IndexData.AddTriangle( 7, 4, 6 );

        }

        XST_RET_FAIL( pIB->Unlock() );
        XST_RET_FAIL( pIB->Create() );

        CBoundingVolume Vol;
        Vol.BuildFromMinMax( -vecSize + Options.vecPos, vecSize + Options.vecPos );
        pMesh->SetBoundingVolume( Vol );

        return XST_OK;
    }


    i32 CreateBox(Resources::CMesh* pMesh, const IInputLayout* pIL, const SLineBoxOptions& Options)
    {

        ul32 ulVertCount = 8; 
        ul32 ulIndexCount = 24; //8 * 2 + 2 * 4; 8 per front/back, 4 per left, right
        VertexBufferPtr pVB = pMesh->CreateVertexBuffer();
        
        pVB->SetTopologyType( TopologyTypes::LINE_LIST );
        pVB->SetUsage( BufferUsages::DEFAULT );
        pVB->SetVertexCount( ulVertCount );
        pVB->SetInputLayout( pIL );

        if( XST_FAILED( pVB->Lock() ) )
        {
            return XST_FAIL;
        }

        CVertexData& Data = pVB->GetVertexData();

        cf32 fUnit = 1.0f;
        ul32 vs = pIL->GetVertexSize();
        const Vec3 vecSize( Options.vecSize * 0.5f );
        const Vec3 vecPos( vecSize + Options.vecPos );

        Vec3 aCorners[ Resources::BoxCorners::_ENUM_COUNT ];
        aCorners[ Resources::BoxCorners::LEFT_BOTTOM_BACK ]	= Vec3( -vecSize.x, -vecSize.y, -vecSize.z ) + Options.vecPos;
        aCorners[ Resources::BoxCorners::LEFT_BOTTOM_FRONT ]	= Vec3( -vecSize.x, -vecSize.y, vecSize.z ) + Options.vecPos;
        aCorners[ Resources::BoxCorners::LEFT_TOP_BACK ]		= Vec3( -vecSize.x, vecSize.y, -vecSize.z ) + Options.vecPos;
        aCorners[ Resources::BoxCorners::LEFT_TOP_FRONT ]		= Vec3( -vecSize.x, vecSize.y, vecSize.z ) + Options.vecPos;
        aCorners[ Resources::BoxCorners::RIGHT_BOTTOM_BACK ]	= Vec3( vecSize.x, -vecSize.y, -vecSize.z ) + Options.vecPos;
        aCorners[ Resources::BoxCorners::RIGHT_BOTTOM_FRONT ]	= Vec3( vecSize.x, -vecSize.y, vecSize.z ) + Options.vecPos;
        aCorners[ Resources::BoxCorners::RIGHT_TOP_FRONT ]		= Vec3( vecSize.x, vecSize.y, vecSize.z ) + Options.vecPos;
        aCorners[ Resources::BoxCorners::RIGHT_TOP_BACK ]		= Vec3( vecSize.x, vecSize.y, -vecSize.z ) + Options.vecPos;

        /*for(i32 i = 0; i < BoxCorners::_ENUM_COUNT; ++i)
        {
            XST::CDebug::PrintDebugLN( XST::ToStr() << XST_GET_DBG_NAME( pMesh ) << (BOX_CORNER)i << aCorners[ i ] );
        }*/

        if( pIL->IsPosition() )
        {
            for(u32 i = 0; i < Resources::BoxCorners::_ENUM_COUNT; ++i)
            {
                Data.SetPosition( i, aCorners[ i ] );
            }
        }

        if( pIL->IsColor() )
        {
            f32 fc = 0.05f;
            for(ul32 i = 0; i < ulVertCount; ++i)
            {
                Data.SetColor( i, Options.colColor.ToVector4() ); 
            }
        }

        if( XST_FAILED( pVB->Unlock() ) )
        {
            return XST_FAIL;
        }

        IndexBufferPtr pIB = pMesh->CreateIndexBuffer();
        pIB->SetUsage( BufferUsages::DEFAULT );
        pIB->SetIndexCount( ulIndexCount );
        
        if( XST_FAILED( pIB->Lock() ) )
        {
            return XST_FAIL;
        }

        CIndexData& IData = pIB->GetIndexData();

        //Front
        IData.SetIndex( 0, Resources::BoxCorners::LEFT_BOTTOM_FRONT );
        IData.SetIndex( 1, Resources::BoxCorners::LEFT_TOP_FRONT );

        IData.SetIndex( 2, Resources::BoxCorners::LEFT_TOP_FRONT );
        IData.SetIndex( 3, Resources::BoxCorners::RIGHT_TOP_FRONT );

        IData.SetIndex( 4, Resources::BoxCorners::RIGHT_TOP_FRONT );
        IData.SetIndex( 5, Resources::BoxCorners::RIGHT_BOTTOM_FRONT );

        IData.SetIndex( 6, Resources::BoxCorners::RIGHT_BOTTOM_FRONT );
        IData.SetIndex( 7, Resources::BoxCorners::LEFT_BOTTOM_FRONT );
        
        //Back
        IData.SetIndex( 8, Resources::BoxCorners::LEFT_BOTTOM_BACK );
        IData.SetIndex( 9, Resources::BoxCorners::LEFT_TOP_BACK );

        IData.SetIndex( 10, Resources::BoxCorners::LEFT_TOP_BACK );
        IData.SetIndex( 11, Resources::BoxCorners::RIGHT_TOP_BACK );

        IData.SetIndex( 12, Resources::BoxCorners::RIGHT_TOP_BACK );
        IData.SetIndex( 13, Resources::BoxCorners::RIGHT_BOTTOM_BACK );

        IData.SetIndex( 14, Resources::BoxCorners::RIGHT_BOTTOM_BACK );
        IData.SetIndex( 15, Resources::BoxCorners::LEFT_BOTTOM_BACK );

        //Left
        IData.SetIndex( 16, Resources::BoxCorners::LEFT_BOTTOM_FRONT );
        IData.SetIndex( 17, Resources::BoxCorners::LEFT_BOTTOM_BACK );

        IData.SetIndex( 18, Resources::BoxCorners::LEFT_TOP_FRONT );
        IData.SetIndex( 19, Resources::BoxCorners::LEFT_TOP_BACK );

        //Right
        IData.SetIndex( 20, Resources::BoxCorners::RIGHT_BOTTOM_FRONT );
        IData.SetIndex( 21, Resources::BoxCorners::RIGHT_BOTTOM_BACK );

        IData.SetIndex( 22, Resources::BoxCorners::RIGHT_TOP_FRONT );
        IData.SetIndex( 23, Resources::BoxCorners::RIGHT_TOP_BACK );

        if( XST_FAILED( pIB->Unlock() ) )
        {
            return XST_FAIL;
        }

        CBoundingVolume Vol;
        Vol.BuildFromMinMax( aCorners[ Resources::BoxCorners::LEFT_BOTTOM_BACK ], aCorners[ Resources::BoxCorners::RIGHT_TOP_FRONT ] );
        pMesh->SetBoundingVolume( Vol );

        return XST_OK;
    }

    i32 CreatePlane(Resources::CMesh* pMesh, const IInputLayout* pIL, const SPlaneOptions& Opts)
    {
        bool bIsNormal = pIL->IsNormal();
        u16 ulVVertCount = (u16)Opts.vecVertexCount.x;
        u16 ulHVertCount = (u16)Opts.vecVertexCount.y;
        ul32 ulVertCount = ulVVertCount * ulHVertCount;
        //ul32 ulIndexCount = 36;
        VertexBufferPtr pVB = pMesh->CreateVertexBuffer();
        pVB->SetInputLayout( pIL );
        
        pVB->SetTopologyType( TopologyTypes::TRIANGLE_STRIP );
        pVB->SetUsage( BufferUsages::DEFAULT );
        pVB->SetVertexCount( ulVertCount );
        pVB->SetInputLayout( pIL );

        pVB->Lock();

        CVertexData& Data = pVB->GetVertexData();

        cf32 fUnit = 1.0f;
        ul32 vs = pIL->GetVertexSize();
        Vec2 vecTC( Vec2::ZERO );
        Vec3 vecTmpPos;

        if( pIL->IsPosition() )
        {
            ul32 ulVertId = 0;
            Vec2 vecVertDist( Opts.vecSize.x / ( Opts.vecVertexCount.x - 1 ), Opts.vecSize.y / ( Opts.vecVertexCount.y - 1 ) );
            Vec2 vecPos( Vec2::ZERO );

            for(ul32 y = 0; y < ulVVertCount; ++y, vecPos.y += vecVertDist.y)
            {
                for(ul32 x = 0; x < ulHVertCount; ++x, vecPos.x += vecVertDist.x)
                {
                    vecTmpPos = Vec3( vecPos.x, 0, vecPos.y );
                    Data.SetPosition( ulVertId, vecTmpPos );
                    if( pIL->IsTexCoord0() )
                    {
                        vecTC.x = vecTmpPos.x / Opts.vecSize.x;
                        vecTC.y = vecTmpPos.z / Opts.vecSize.y;
                        Data.SetTexCoord0( ulVertId, vecTC );
                    }

                    ++ulVertId;
                }

                vecPos.x = 0.0f;
            }
        }

        if( pIL->IsColor() )
        {
            f32 fc = 0.05f;
            for(ul32 i = 0; i < ulVertCount; ++i)
            {
                Data.SetColor( i, XST::CColor::Random().ToVector4() + Vec4( 0.2f, 0.2f, 0.2f, 0.0f ) ); 
            }
        }

        if( bIsNormal )
        {
            for(u32 i = 0; i < ulVertCount; ++i)
            {
                Data.SetNormal( i, Opts.vecNormal );
            }
        }

        pVB->Unlock();
        pVB->Create();


        IndexBufferPtr pIB = pMesh->CreateIndexBuffer();
        pIB->SetUsage( BufferUsages::DEFAULT );
        u16 ulIndexCount = ( ulHVertCount * 2 ) * ( ulVVertCount - 1 ) + ( ulVVertCount - 2 );
        pIB->SetIndexCount( ulIndexCount );
        pIB->Lock();

        CIndexData& IndexData = pIB->GetIndexData();

        ul32 ulIndex = 0;
        for(u16 z = 0; z < ulVVertCount - 1; ++z)
        {
            if( z % 2 == 0 )
            {
                i16 x;
                for(x = 0; x < ulHVertCount; ++x )
                {
                    IndexData.SetIndex( ulIndex++, x + ( z * ulHVertCount ) );
                    IndexData.SetIndex( ulIndex++, x + ( z * ulHVertCount ) + ulHVertCount );
                }

                if( z != ulVVertCount - 2 )
                {
                    IndexData.SetIndex( ulIndex++, --x + ( z * ulHVertCount ) );
                }
            }
            else
            {
                i16 x;
                for(x = ulHVertCount - 1; x >= 0; --x )
                {
                    IndexData.SetIndex( ulIndex++, x + ( z * ulHVertCount ) );
                    IndexData.SetIndex( ulIndex++, x + ( z * ulHVertCount ) + ulHVertCount );
                }

                if( z != ulVVertCount - 2 )
                {
                    IndexData.SetIndex( ulIndex++, ++x + ( z * ulHVertCount ) );
                }
            }
        }

        pIB->Unlock();
        pIB->Create();

        return XST_OK;
    }

    i32 CreateRect2D(Resources::CMesh* pMesh, const IInputLayout* pIL, const SRect2DOptions& Opts)
    {
        Vec3 vecScale( 100 ); 
        Vec2 vecWidth( 0, 100 );
        Vec2 vecHeight( 0, 100 );

        IndexBufferPtr pIB = pMesh->CreateIndexBuffer();
        VertexBufferPtr pVB = pMesh->CreateVertexBuffer();

        pVB->SetVertexCount( 4 );
        pVB->SetInputLayout( pIL );
        pVB->SetTopologyType( TopologyTypes::TRIANGLE_LIST );
        pVB->SetUsage( Opts.eUsage );

        pIB->SetIndexCount( 6 );
        pIB->SetUsage( BufferUsages::STATIC );

        pVB->Lock();
        CVertexData& VData = pVB->GetVertexData();

        if( pIL->IsPosition() )
        {
            VData.SetPosition( 0, Vec3( 0,			0,				0 ) );
            VData.SetPosition( 1, Vec3( vecScale.x, 0,				0 ) );
            VData.SetPosition( 2, Vec3( vecScale.x, -vecScale.y,	0 ) );
            VData.SetPosition( 3, Vec3( 0,			-vecScale.y,	0) );
        }

        if( pIL->IsColor() )
        {
            VData.SetColor( 0, Vec4( 1, 1, 1, 1 ) );
            VData.SetColor( 1, Vec4( 1, 1, 1, 1 ) );
            VData.SetColor( 2, Vec4( 1, 1, 1, 1 ) );
            VData.SetColor( 3, Vec4( 1, 1, 1, 1 ) );
        }

        if( pIL->IsTexCoord0() )
        {
            VData.SetTexCoord( 0, 0, Vec2( 0, 1 ) );
            VData.SetTexCoord( 1, 0, Vec2( 1, 1 ) );
            VData.SetTexCoord( 2, 0, Vec2( 0, 1 ) );
            VData.SetTexCoord( 3, 0, Vec2( 0, 0 ) );
        }

        pVB->Unlock();
        pVB->Create();

        pIB->Lock();
        CIndexData& IData = pIB->GetIndexData();

        IData.SetTriangle( 0, 0, 2, 3 );
        IData.SetTriangle( 1, 1, 2, 0 );

        return XST_OK;
    }

}//xse