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
#include "XSEIViewport.h"
#include "XSECLight.h"

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
		m_fSize( fSize ),
		m_vecAmbientColor( 0.3f, 0.3f, 0.3f, 1.0f )
	{
		m_pRootNode = xst_new CSceneNode( this, xst_null, "World", XST::CHash::GetCRC( "World" ) );
		m_pDbg = xst_new CSceneDebug( this );
		m_RenderQueue._SetRenderSystem( GetRenderSystem() );
		xst_castring& strLName( "xse_default_light" );
		m_pDefaultLight = xst_new CLight( strLName, XST::CHash::GetCRC( strLName ), m_pRootNode );
		xst_assert2( m_pDefaultLight );
		m_pCurrLight = m_pDefaultLight;
		//m_pScenePartitionSystem = xst_new COctreeScenePartitionSystem( this );
	}

	CSceneManager::~CSceneManager()
	{
		xst_release( m_pDefaultLight );
		
		xst_delete( m_pDbg );
		xst_delete( m_pScenePartitionSystem );

		if( m_pTerrainSystem && m_pTerrainSystem->IsAutoDestroy() )
		{
			m_pTerrainSystem->DestroyTerrains();
			xst_delete( m_pTerrainSystem );
		}

		DestroyLights();
		xst_delete( m_pDefaultLight );
		DestroyCameras();
		DestroyStaticGeometries();
		DestroyDynamicGeometries();
		xst_delete( m_pRootNode );
	}

	i32 CSceneManager::Init()
	{
		DEFAULT_SHADER_COLOR = m_pModelMgr->GetRenderSystem()->GetShaderSystem()->CreateShaderCode( ILE::COLOR, ILE::COLOR );

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

		for( LightPtr pL : m_vLights )
		{
			pL->Update( fFrameTime );
		}

		Vec3 vecTmp;
		m_pModelMgr->GetRenderSystem()->GetShaderSystem()->SetConstantValue( ShaderConstants::SCENE_AMBIENT_COLOR, m_vecAmbientColor );
		m_pModelMgr->GetRenderSystem()->GetShaderSystem()->SetConstantValue( ShaderConstants::LIGHT_COLOR, m_pCurrLight->GetColor() );
		m_pModelMgr->GetRenderSystem()->GetShaderSystem()->SetConstantValue( ShaderConstants::LIGHT_SPECULAR_POWER, m_pCurrLight->GetSpecularPower() );
		m_pModelMgr->GetRenderSystem()->GetShaderSystem()->SetConstantValue( ShaderConstants::LIGHT_SPECULAR_COLOR, m_pCurrLight->GetSpecularColor() );
		m_pModelMgr->GetRenderSystem()->GetShaderSystem()->SetConstantValue( ShaderConstants::LIGHT_POWER, m_pCurrLight->GetPower() );
		m_pCurrLight->CalcWorldPosition( &vecTmp );
		m_pModelMgr->GetRenderSystem()->GetShaderSystem()->SetConstantValue( ShaderConstants::LIGHT_POSITION, vecTmp );

		m_pComputeCamera->CalcWorldPosition( &vecTmp );
		m_pModelMgr->GetRenderSystem()->GetShaderSystem()->SetConstantValue( ShaderConstants::CAMERA_POSITION, Vec3(0.0f) );

		m_pScenePartitionSystem->StartProcessing();
		m_pScenePartitionSystem->Update();
		m_pTerrainSystem->Update();
	}

	void CSceneManager::Render()
	{
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
        // If camera exists (no free place on this handle)
		if( !XST::MapUtils::FindPlace( m_mCameras, ulHandle, &Itr ) )
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

		pCamera->SetAspectRatio( GetRenderSystem()->GetOptions().uiResolutionWidth, GetRenderSystem()->GetOptions().uiResolutionWidth );
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

		pCamera->Init();

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

	CSceneManager::CameraMap& CSceneManager::GetCameras()
	{
		return m_mCameras;
	}

	void CSceneManager::_AddObject(IRenderableObject* pObj, bool bAddToScenePartition)
	{ 
		m_pDbg->_CreateObjectAABBMesh( pObj );
		if( bAddToScenePartition )
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

	CSceneManager::LightPtr FindLight(const CSceneManager::LightVec& vLights, u32 uHandle, CSceneManager::LightPtr* ppLightOut, u32* puLightPos)
	{
		for( u32 i = vLights.size(); i-- > 0; )
		{
			if( vLights[ i ]->GetObjectHandle() == uHandle )
			{
				*ppLightOut = vLights[ i ];
				*puLightPos = i;
				return *ppLightOut;
			}
		}
		return xst_null;
	}

	CSceneManager::LightPtr RemoveLight(CSceneManager::LightVec& vLights, u32 uPos)
	{
		auto* pLight = vLights[uPos];
		std::swap(vLights[uPos], vLights[vLights.size()-1]);
		vLights.pop_back();
		return pLight;
	}

	CSceneManager::LightPtr FindLight(const CSceneManager::LightVec& vLights, xst_castring& strName, CSceneManager::LightPtr* ppLightOut, u32* puLightPos)
	{
		ul32 uHandle = XST::CHash::GetCRC( strName );
		return FindLight( vLights, uHandle, ppLightOut, puLightPos );
	}

	CSceneManager::LightPtr	CSceneManager::CreateLight(xst_castring& strName)
	{
		LightPtr pL;
		u32 uPos;
		ul32 uHandle = XST::CHash::GetCRC( strName );
		if( FindLight( m_vLights, uHandle, &pL, &uPos ) )
			return pL;
		pL = xst_new CLight( strName, uHandle, m_pRootNode );
		if( !pL )
			return xst_null;
		m_vLights.push_back( pL );
		if( XST_FAILED( m_pRootNode->AddObject( pL ) ) )
		{
			DestroyLight( &pL );
			return xst_null;
		}
		return pL;
	}
				
	i32	CSceneManager::DestroyLight(LightPtr* ppLight)
	{
		xst_assert2( ppLight && *ppLight );
		LightPtr pLight = *ppLight;
		LightPtr pL;
		u32 uPos;
		if( !FindLight( m_vLights, pLight->GetObjectHandle(), &pL, &uPos ) )
			return XST_FAIL;
		RemoveLight( m_vLights, uPos );
		if( m_pCurrLight == pLight )
			m_pCurrLight = m_pDefaultLight;
		xst_delete( *ppLight );
		*ppLight = xst_null;
        return XST_OK;
	}
				
	i32	CSceneManager::DestroyLight(xst_castring& strName)
	{
		LightPtr pL;
		u32 uPos;
		if( !FindLight( m_vLights, strName, &pL, &uPos ) )
			return XST_FAIL;
		RemoveLight( m_vLights, uPos );
		if( m_pCurrLight == pL )
			m_pCurrLight = m_pDefaultLight;
		xst_delete( pL );
		return XST_OK;
	}
				
	void CSceneManager::DestroyLights()
	{
		for( u32 i = m_vLights.size(); i-- > 0; )
		{
			xst_delete( m_vLights[ i ] );
		}
		m_vLights.clear();
		m_pCurrLight = m_pDefaultLight;
	}
				
	CSceneManager::LightPtr	CSceneManager::GetLight(xst_castring& strName) const
	{
		LightPtr pLight;
		u32 uPos;
		return FindLight( m_vLights, strName, &pLight, &uPos );
	}

	i32 CSceneManager::SetLight(xst_castring& strName)
	{
		u32 uHandle = XST::CHash::GetCRC( strName );
		return SetLight( uHandle );
	}
				
	i32	CSceneManager::SetLight(ul32 uHandle)
	{
		u32 uPos;
		LightPtr pL;
		if( !FindLight( m_vLights, uHandle, &pL, &uPos ) )
			return XST_FAIL;
		return SetLight( pL );
	}
	
	i32	CSceneManager::SetLight(LightPtr pLight)
	{
		xst_assert2( pLight );
		m_pCurrLight = pLight;
		return XST_OK;
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

	void CSceneManager::AddToPartitionSystem(CObject* pObj)
	{
		GetScenePartitionSystem( )->AddObject( pObj );
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

	void CSceneManager::SetViewFrustumCullType(const VIEW_FRUSTUM_CULL_TEST_TYPE& eType)
	{
		this->m_pScenePartitionSystem->SetViewFrustumCullTestType( eType );
	}

	VIEW_FRUSTUM_CULL_TEST_TYPE	CSceneManager::GetViewFrustumCullType() const
	{
		return this->m_pScenePartitionSystem->GetViewFrustumCullTestType();
	}

}//xse