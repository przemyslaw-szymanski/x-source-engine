#include "XSECSceneManager.h"
#include "XSECSceneDebug.h"
#include "XSECSceneNode.h"
#include "XSEIRenderableObject.h"
#include "XSECModel.h"
#include "XSECCamera.h"
#include "XSECModelManager.h"
#include "XSECStaticGeometry.h"
#include "XSECDynamicGeometry.h"
#include "XSEITerrainSystem.h"
#include "XSECOctreeScenePartitionSystem.h"
#include "XSECNoScenePartitionSystem.h"
#include "XSECFPPCamera.h"
#include "XSEIRenderSystem.h"
#include "XSEIShaderSystem.h"
#include "XSECMeshManager.h"
#include "XSECMaterialManager.h"

namespace XSE
{
	xst_castring CSceneManager::DEFAULT_CAMERA( "Default" );
	xst_astring CSceneManager::DEFAULT_SHADER_COLOR( "" );

	CSceneManager::CSceneManager(xst_castring& strName, CModelManager* pModelMgr, f32 fSize) : 
		m_strName( strName ), 
		m_pViewCamera( xst_null ),
		m_pModelMgr( pModelMgr ),
		m_pTerrainSystem( xst_null ),
		m_pScenePartitionSystem( xst_null ),
		m_fSize( fSize )
	{
		m_pRootNode = xst_new CSceneNode( this, xst_null, "World", XST::CHash::GetCRC( "World" ) );
		m_pDbg = xst_new CSceneDebug( this );
		m_RenderQueue._SetRenderSystem( GetRenderSystem() );
		//m_pScenePartitionSystem = xst_new COctreeScenePartitionSystem( this );
	}

	CSceneManager::~CSceneManager()
	{
		xst_delete( m_pDbg );
		xst_delete( m_pScenePartitionSystem );

		if( m_pTerrainSystem && m_pTerrainSystem->IsAutoDestroy() )
		{
			m_pTerrainSystem->DestroyTerrains();
			xst_delete( m_pTerrainSystem );
		}

		DestroyCameras();
		DestroyStaticGeometries();
		DestroyDynamicGeometries();
		xst_delete( m_pRootNode );
	}

	i32 CSceneManager::Init()
	{
		DEFAULT_SHADER_COLOR = m_pModelMgr->GetRenderSystem()->GetShaderSystem()->CreateShaderCode( ILEs::COLOR, ILEs::COLOR );

		if( this->GetSize() > 0 )
		{
			SetScenePartitionSystem( xst_new COctreeScenePartitionSystem( this ) );
		}
		else
		{
			SetScenePartitionSystem( xst_new CNoScenePartitionSystem( this ) );
		}

		if( XST_FAILED( m_pScenePartitionSystem->Init() ) )
		{
			return XST_FAIL;
		}

		return XST_OK;
	}

	void CSceneManager::Update(cf32& fFrameTime)
	{
		//Update the camera
		m_pComputeCamera->Update( fFrameTime );
		if( m_pComputeCamera != m_pViewCamera )
		{
			m_pViewCamera->Update( fFrameTime );
		}

		m_pScenePartitionSystem->StartProcessing();
		m_pScenePartitionSystem->Update();

		m_RenderQueue.Render( this );
	}

	void CSceneManager::SetScenePartitionSystem(IScenePartitionSystem* pSystem)
	{
		xst_assert( pSystem != xst_null, "(CSceneManager::SetScenePartitionSystem)" );

		if( m_pScenePartitionSystem != xst_null )
		{
			pSystem->AddObjects( m_pScenePartitionSystem->GetObjects() );
			xst_delete( m_pScenePartitionSystem );
		}

		m_pScenePartitionSystem = pSystem;
	}

	IRenderSystem* CSceneManager::GetRenderSystem()
	{
		return m_pModelMgr->GetRenderSystem();
	}

	const IRenderSystem* CSceneManager::GetRenderSystem() const
	{
		return m_pModelMgr->GetRenderSystem();
	}

	CMeshManager*	CSceneManager::GetMeshManager()
	{
		return m_pModelMgr->GetMeshManager();
	}

	const	CMeshManager*	CSceneManager::GetMeshManager() const
	{
		return m_pModelMgr->GetMeshManager();
	}

	CMaterialManager*	CSceneManager::GetMaterialManager()
	{
		return m_pModelMgr->GetMeshManager()->GetMaterialManager();
	}

	const	CMaterialManager*	CSceneManager::GetMaterialManager() const
	{
		return m_pModelMgr->GetMeshManager()->GetMaterialManager();
	}

	CSceneNode* CSceneManager::CreateNode(xst_castring& strName, bool bAutoDestroy)
	{
		return m_pRootNode->CreateChildNode( strName, bAutoDestroy );
	}

	i32 CSceneManager::DestroyNode(xst_castring& strName, bool bSearch)
	{
		return DestroyNode( XST::CHash::GetCRC( strName ), bSearch );
	}

	i32 CSceneManager::DestroyNode(cul32& ulHandle, bool bSearch)
	{
		if( m_pRootNode->DestroyChildNode( ulHandle ) != RESULT::OK )
		{
			if( bSearch )
			{
				DestroyNode( m_pRootNode, ulHandle );
			}
			else
			{
				return RESULT::FAILED;
			}
		}

		return RESULT::OK;
	}

	i32	CSceneManager::DestroyNode(CSceneNode* pNode, bool bSearch)
	{
		return DestroyNode( pNode->GetObjectHandle(), bSearch );
	}

	CCamera* CSceneManager::CreateCamera(xst_castring &strName, CAMERA_TYPE eType)
	{
		_CameraIterator Itr;
		ul32 ulHandle = XST::CHash::GetCRC( strName );
		if( !XST_FAILED( XST::MapUtils::FindPlace( m_mCameras, ulHandle, &Itr ) ) )
		{
			return Itr->second;
		}

		CCamera* pCamera = xst_null;
		switch( eType )
		{
			case CameraTypes::FPP:
			{
				pCamera = xst_new CFPPCamera( this, strName, ulHandle );
			}
			break;

			case CameraTypes::TPP:
			{
				xst_assert( pCamera != xst_null, "(CSceneManager::CreateCamera) TPP camera type is not supported yet" );
			}
			break;

			case CameraTypes::RTS:
			{
				xst_assert( pCamera != xst_null, "(CSceneManager::CreateCamera) RTS camera type is not supported yet" );
			}
			break;

			case CameraTypes::FREE: default:
			{
				pCamera = xst_new CCamera( this, strName, ulHandle );
			}
			break;
		}
		
		pCamera->_CreateFrustumMesh();
		pCamera->ShowFrustumMesh( false );

		XST::MapUtils::InsertOnPlace( m_mCameras, ulHandle, pCamera, Itr );

		return pCamera;
	}

	i32 CSceneManager::DestroyCamera(CCamera* pCamera)
	{
		if( XST_FAILED( RemoveCamera( pCamera ) ) )
		{
			return XST_FAIL;
		}
		xst_delete( pCamera );
		return XST_OK;
	}

	i32 CSceneManager::RemoveCamera(CCamera* pCamera)
	{
		_CameraIterator Itr = m_mCameras.find( pCamera->GetObjectHandle() );
		if( Itr == m_mCameras.end() )
		{
			XST_LOG_ERR( "Camera: " << XST_GET_DBG_NAME( pCamera ) << " does not exists in scene manager: " << GetName() );
			return XST_FAIL;
		}

		m_mCameras.erase( Itr );

		return XST_OK;
	}

	void CSceneManager::DestroyCameras()
	{
		for(_CameraIterator Itr = m_mCameras.begin(); Itr != m_mCameras.end(); ++Itr)
		{
			xst_delete( Itr->second );
		}

		m_mCameras.clear();
	}

	void CSceneManager::RemoveCameras()
	{
		m_mCameras.end();
	}

	i32 CSceneManager::AddCamera(XSE::CCamera *pCamera)
	{
		return XST::MapUtils::Insert( m_mCameras, pCamera->GetObjectHandle(), pCamera );
	}

	CCamera* CSceneManager::GetCamera(xst_castring& strName)
	{
		return GetCamera( XST::CHash::GetCRC( strName ) );
	}

	i32 CSceneManager::SetCamera(XSE::CCamera *pCamera, bool bCompute, bool bView)
	{
		xst_assert( pCamera, "" );
		//Find this camera

		if( bView )
		{
			m_pViewCamera = pCamera;
			m_pViewCamera->_SetCompute( bCompute );
		}

		if( bCompute )
		{
			m_pComputeCamera = pCamera;
			m_pComputeCamera->_SetCompute( true );
		}

		return XST_OK;
	}

	CCamera* CSceneManager::GetCamera(cul32& ulHandle)
	{
		_CameraIterator Itr = m_mCameras.find( ulHandle );
		if( Itr == m_mCameras.end() )
		{
			return xst_null;
		}

		return Itr->second;
	}

	CSceneManager::CameraIterator CSceneManager::GetCameraIterator()
	{
		return CameraIterator( &m_mCameras );
	}

	void CSceneManager::_AddObject(IRenderableObject* pObj)
	{ 
		m_pDbg->_CreateObjectAABBMesh( pObj );
		m_pScenePartitionSystem->AddObject( pObj );
		m_RenderQueue.AddObject( pObj ); 
	}

	/*void CSceneManager::_SetRenderSystem(IRenderSystem* pRS)
	{
		m_pRenderSystem = pRS;
		m_RenderQueue._SetRenderSystem( pRS );
	}*/

	void DestroyNode(CSceneNode* pParentNode, cul32& ulHandle)
	{
		if( pParentNode )
		{
			CSceneNode::NodeIterator Itr = pParentNode->GetChildNodeIterator();
			while( Itr.HasMoreElements() )
			{
				DestroyNode( Itr.GetValue(), ulHandle );
				Itr.GetValue()->DestroyChildNode( ulHandle );
				Itr.MoveNext();
			}
		}
	}

	ModelPtr CSceneManager::CreateModel(xst_castring& strName)
	{
		ModelPtr pModel = m_pModelMgr->CreateModel( strName, this->m_strName );

		return pModel;
	}

	ModelPtr CSceneManager::CreateModel(xst_castring& strName, xst_castring& strNodeName)
	{
		ModelPtr pModel = m_pModelMgr->CreateModel( strName, this->m_strName );
		CSceneNode* pNode = this->CreateNode( strNodeName );
		pNode->AddObject( pModel );
		
		return pModel;
	}

	ModelPtr CSceneManager::CreateModel(xst_castring& strName, CSceneNode* pNode)
	{
		ModelPtr pModel = m_pModelMgr->CreateModel( strName, pNode, this->m_strName );
		
		return pModel;
	}

	ModelPtr CSceneManager::CreateModel(xst_castring& strName, xst_castring& strMeshName, xst_castring& strMeshGroup)
	{
		xst_assert( 0, "CSceneManager::CreateModel not implemented" );
		return ModelPtr();
	}

	ModelPtr CSceneManager::CreateModel(xst_castring& strName, xst_castring& strNodeName, xst_castring& strMeshName, xst_castring& strMeshGroup)
	{
		xst_assert( 0, "CSceneManager::CreateModel not implemented" );
		return ModelPtr();
	}

	ModelPtr CSceneManager::CreateModel(xst_castring& strName, CSceneNode* pNode, xst_castring& strMeshName, xst_castring& strMeshGroup)
	{
		xst_assert( 0, "CSceneManager::CreateModel not implemented" );
		return ModelPtr();
	}

	ModelPtr CSceneManager::CreateModel(xst_castring& strName, BASIC_SHAPE eShape, IInputLayout* pIL, xst_unknown pShapeOptions)
	{
		ModelPtr pModel = m_pModelMgr->CreateModel( strName, eShape, pIL, pShapeOptions, this->m_strName );

		return pModel;
	}

	ModelPtr CSceneManager::CreateModel(xst_castring& strName, CSceneNode* pNode, BASIC_SHAPE eShape, IInputLayout* pIL, xst_unknown pShapeOptions)
	{
		ModelPtr pModel = m_pModelMgr->CreateModel( strName, eShape, pIL, pShapeOptions, this->m_strName );
		pNode->AddObject( pModel );
		return pModel;
	}

	ModelPtr CSceneManager::CreateModel(xst_castring& strName, xst_castring& strNodeName, BASIC_SHAPE eShape, IInputLayout* pIL, xst_unknown pShapeOptions)
	{
		ModelPtr pModel = m_pModelMgr->CreateModel( strName, eShape, pIL, pShapeOptions, this->m_strName );
		CSceneNode* pNode = this->CreateNode( strNodeName );
		pNode->AddObject( pModel );
		
		return pModel;
	}


	ModelPtr CSceneManager::CreateModel(xst_castring& strName, xst_castring& strMeshName, xst_castring& strMeshGroup, BASIC_SHAPE eShape, IInputLayout* pIL, xst_unknown pShapeOptions)
	{
		xst_assert( 0, "CSceneManager::CreateModel not implemented" );
		return ModelPtr();
	}

	ModelPtr CSceneManager::CreateModel(xst_castring& strName, xst_castring& strNodeName, xst_castring& strMeshName, xst_castring& strMeshGroup, BASIC_SHAPE eShape, IInputLayout* pIL, xst_unknown pShapeOptions)
	{
		xst_assert( 0, "CSceneManager::CreateModel not implemented" );
		return ModelPtr();
	}

	ModelPtr CSceneManager::CreateModel(xst_castring& strName, CSceneNode* pNode, xst_castring& strMeshName, xst_castring& strMeshGroup, BASIC_SHAPE eShape, IInputLayout* pIL, xst_unknown pShapeOptions)
	{
		xst_assert( 0, "CSceneManager::CreateModel not implemented" );
		return ModelPtr();
	}

	ModelPtr CSceneManager::CreateModel(xst_castring& strName, xst_castring& strNodeName, MeshPtr& pMesh)
	{
		ModelPtr pModel = m_pModelMgr->CreateModel( strName, this->m_strName );
		CSceneNode* pNode = this->CreateNode( strNodeName );
		pModel->SetMesh( pMesh );
		pNode->AddObject( pModel );
		return pModel;
	}

	CSceneManager::StaticGeometryPtr CSceneManager::CreateStaticGeometry(xst_castring& strName, xst_castring& strGroup)
	{
		CStaticGeometry* pSG = xst_new CStaticGeometry( strName, this->GetRenderSystem(), this, this->m_pModelMgr->GetMeshManager(), strGroup );
		if( XST_FAILED( XST::MapUtils::Insert( m_mStaticGeometries, strName, pSG ) ) )
		{
			XST_LOG_ERR( "Static geometry: " << strName << " already exists in scene manager: " << this->GetName() );
			xst_delete( pSG );
			return xst_null;
		}

		return pSG;
	}

	DynamicGeometryPtr CSceneManager::CreateDynamicGeometry(xst_castring& strName, xst_castring& strGroup)
	{
		CDynamicGeometry* pSG = xst_new CDynamicGeometry( strName, this->GetRenderSystem(), this, this->m_pModelMgr->GetMeshManager(), strGroup );
		DynamicGeometryPtr pGeom( pSG );
		if( XST_FAILED( XST::MapUtils::Insert( m_mDynamicGeometries, strName, pGeom ) ) )
		{
			XST_LOG_ERR( "Static geometry: " << strName << " already exists in scene manager: " << this->GetName() );
					
			return DynamicGeometryPtr();
		}

		return pGeom;
	}

	void CSceneManager::DestroyStaticGeometry(xst_castring &strName)
	{
		StaticGeometryMap::iterator Itr = m_mStaticGeometries.find( strName );
		if( Itr == m_mStaticGeometries.end() )
		{
			XST_LOG_WRN( "Static geometry: " << strName << " not found in scene manager: " << this->GetName() );
		}

		//Remove from scene node
		CSceneNode* pNode = Itr->second->GetSceneNode();
		if( pNode != xst_null )
		{
			pNode->RemoveObject( Itr->second );
		}
		//Remove from render queue
		xst_release( Itr->second );
		m_mStaticGeometries.erase( Itr );
	}

	void CSceneManager::DestroyDynamicGeometry(xst_castring &strName)
	{

	}

	void CSceneManager::DestroyDynamicGeometry(CDynamicGeometry* pGeometry)
	{
	}

	void CSceneManager::DestroyStaticGeometries()
	{

	}

	void CSceneManager::DestroyDynamicGeometries()
	{

	}

	void CSceneManager::AddToRenderQueue(RENDER_QUEUE_TYPE eType, IRenderableObject* pObj)
	{
		m_RenderQueue.AddObject( eType, pObj );
	}

	//TERRAIN
	i32 CSceneManager::SetTerrainSystem(ITerrainSystem* pSystem)
	{
		if( m_pTerrainSystem )
		{
			if( !m_pTerrainSystem->IsAutoDestroy() )
			{
				XST_LOG_ERR( "Terrain system is not auto destroyable. You can't set new terrain system" );
				return XST_FAIL;
			}

			xst_delete( m_pTerrainSystem );
		}

		m_pTerrainSystem = pSystem;
		return XST_OK;
	}

	void CSceneManager::DestroyTerrain(xst_castring& strName)
	{
		if( m_pTerrainSystem )
		{
			m_pTerrainSystem->DestroyTerrain( strName );
		}
	}

	ITerrain* CSceneManager::CreateTerrain(xst_castring& strName, STerrainOptions& Options)
	{
		ITerrain* pTerrain = xst_null;
		//if( !Options.vHeightmaps.empty() )
		{
			//pTerrain = m_pTerrainSystem->CreateFromImage( strName, Options.pImage, Options );
		}
		//else
		{
			//pTerrain = m_pTerrainSystem->CreateTerrain( strName, Options );
			pTerrain = m_pTerrainSystem->CreateTerrain( strName, Options, false );
		}

		 
		if( pTerrain == xst_null )
		{
			return xst_null;
		}

		CSceneNode* pNode = CreateNode( strName );
		if( XST_FAILED( pNode->AddObject( pTerrain ) ) )
		{
			m_pTerrainSystem->DestroyTerrain( pTerrain );
			return xst_null;
		}

		m_vTerrains.push_back( pTerrain );
		return pTerrain;
	}

}//xse