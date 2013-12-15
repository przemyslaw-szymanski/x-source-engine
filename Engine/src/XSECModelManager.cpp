#include "XSECModelManager.h"
#include "XSECMeshManager.h"
#include "XSEIVertexBuffer.h"
#include "XSEIIndexBuffer.h"
#include "XSECMaterialManager.h"
#include "XSECSceneNode.h"
#include "XSECSceneManager.h"
#include "XSEIRenderSystem.h"

namespace XSE
{
	using namespace Resources;

	CModelManager::CModelManager(CMeshManager* pMeshMgr)
	{
		m_pRenderSystem = xst_null;
		m_pMeshMgr = pMeshMgr;
	}

	CModelManager::~CModelManager()
	{
		
	}

	i32	CModelManager::_CreateMemoryPool(cul32& ulObjCount, XST::IAllocator* pAllocator)
	{ 
		if( pAllocator )
		{
			xst_delete( this->m_pMemoryMgr );
			this->m_pMemoryMgr = pAllocator;
			if( !this->m_pMemoryMgr->AllocatePool( sizeof( Resources::CModel ), ulObjCount ) )
			{
				XST_LOG_ERR( "Create memory pool failed in CLuaScriptManager" );
				return RESULT::FAILED;
			}
		}
		else
		{
			xst_delete( this->m_pMemoryMgr );
			this->m_pMemoryMgr = xst_new XST::TCFreeListMemoryManager< Resources::CModel >( ulObjCount );
		}

		return RESULT::OK; 
	}

	i32 CModelManager::DestroyModel(XSE::ModelPtr &pModel, bool bDestroySceneNodeIfEmpty, xst_castring& strGroupName)
	{
		if( pModel == xst_null )
		{
			XST_LOG_ERR( "Model is null" );
			return XST_FAIL;
		}

		/*if( XST_FAILED( m_pMeshMgr->DestroyMesh( pModel->GetMesh(), strGroupName ) ) )
		{
			return XST_FAIL;
		}*/
		CModel::MeshVecItr Itr;
		xst_stl_foreach( Itr, pModel->GetMeshes() )
		{
			m_pMeshMgr->DestroyMesh( (*Itr), strGroupName );
		}

		if( bDestroySceneNodeIfEmpty )
		{
			CSceneNode* pNode = pModel->GetSceneNode();
			if( pNode && pNode->IsEmpty() ) pNode->Destroy();
		}

		this->DestroyResourceByHandle( pModel->GetResourceHandle() );
		pModel = xst_null;

		return XST_OK;
	}

	ResourcePtr CModelManager::CloneResource(const Resources::IResource* pSrcRes, xst_castring& strNewName /* = XST::StringUtil::EmptyAString */, bool bFullClone /* = true */)
	{
		ResourcePtr pNewRes = IResourceManager::CloneResource( pSrcRes, strNewName, bFullClone );
		if( pNewRes == xst_null )
		{
			return pNewRes;
		}

		Resources::CModel* pSrcModel = (Resources::CModel*)pSrcRes;
		Resources::CModel* pModel = (Resources::CModel*)pNewRes.GetPointer();
		
		if( !bFullClone )
		{
			Resources::CModel::MeshVec& vMeshes = pSrcModel->GetMeshes();
			for(u32 i = 0; i < vMeshes.size(); ++i)
			{
				pModel->AddMesh( vMeshes[ i ] );
			}

			pModel->SetMaterial( pSrcModel->GetMaterial() );
			pModel->SetTransformMatrix( pSrcModel->GetTransformMatrix() );
			pModel->SetVisible( pSrcModel->IsVisible() );
			pModel->DisableObject( pSrcModel->GetObjectDisableReason() );
			pModel->SetMaxLOD( pSrcModel->GetMaxLOD() );
			pModel->SetMinLOD( pSrcModel->GetMinLOD() );
			pModel->SetLOD( pSrcModel->GetCurrentLODId() );
			pModel->SetObjectBoundingVolume( pSrcModel->GetObjectBoundingVolume() );
			pModel->SetObjectDirection( pSrcModel->GetObjectDirection() );
		}
		else
		{
			xst_assert( 0, "(CModelManager::CloneResource) Full clone not implemented" );
		}

		return pNewRes;
	}

	ModelPtr CModelManager::CreateModel(xst_castring& strName, xst_castring& strGroup)
	{
		ResourcePtr pRes = this->CreateResource( strName, strGroup );
		//CModel* pModel = (CModel*)pRes.GetPointer();
		ModelPtr pModel = ModelPtr( pRes );

		return pModel;
	}

	ModelPtr CModelManager::CreateModel(xst_castring& strName, CSceneNode* pNode, xst_castring& strGroup)
	{
		ResourcePtr pRes = this->CreateResource( strName, strGroup );
		//CModel* pModel = (CModel*)pRes.GetPointer();
		ModelPtr pModel = ModelPtr( pRes );

		if( pNode )
		{
			pNode->AddObject( pModel );
		}

		return pModel;
	}

	ModelPtr CModelManager::CreateModel(CSceneManager* pSceneMgr, xst_castring& strName, xst_castring& strNodeName, xst_castring& strGroup)
	{
		ResourcePtr pRes = this->CreateResource( strName, strGroup );
		//CModel* pModel = (CModel*)pRes.GetPointer();
		ModelPtr pModel = ModelPtr( pRes );
		
		xst_assert( pSceneMgr, "(CModelManager::CreateModel)" );
		CSceneNode* pNode = pSceneMgr->CreateNode( strNodeName );
		pNode->AddObject( pModel );

		return pModel;
	}

	ModelPtr CModelManager::CreateModel(xst_castring& strName, BASIC_SHAPE eShape, IInputLayout* pLayout, xst_unknown pOptions, xst_castring& strGroup)
	{
		ModelPtr pModel = CreateModel( strName, strGroup );
		MeshPtr pMesh = m_pMeshMgr->CreateMesh( strName, pLayout, eShape, pOptions, strGroup );
		if( pMesh == xst_null )
		{
			DestroyResource( strName );
			return ModelPtr();
		}

		pModel->SetMesh( pMesh );
		
		return pModel;
	}

	ModelPtr CModelManager::CreateModel(xst_castring& strName, CSceneNode* pNode, BASIC_SHAPE eShape, IInputLayout* pLayout, xst_unknown pOptions, xst_castring& strGroup)
	{
		ModelPtr pModel = CreateModel( strName, strGroup );
		MeshPtr pMesh = m_pMeshMgr->CreateMesh( strName, pLayout, eShape, pOptions, strGroup );
		if( pMesh == xst_null )
		{
			DestroyResource( strName );
			return ModelPtr();
		}

		pModel->SetMesh( pMesh );
		
		if( pNode )
		{
			pNode->AddObject( pModel );
		}

		return pModel;
	}

	ModelPtr CModelManager::CreateModel(CSceneManager* pSceneMgr, xst_castring& strName, xst_castring& strNodeName, BASIC_SHAPE eShape, IInputLayout* pLayout, xst_unknown pOptions, xst_castring& strGroup)
	{
		xst_assert( pSceneMgr, "(CModelManager::CreateModel)" );
		ModelPtr pModel = CreateModel( strName, strGroup );

		MeshPtr pMesh = m_pMeshMgr->CreateMesh( strName, pLayout, eShape, pOptions, strGroup );
		if( pMesh == xst_null )
		{
			DestroyResource( strName );
			return ModelPtr();
		}

		pModel->SetMesh( pMesh );
		
		CSceneNode* pNode = pSceneMgr->CreateNode( strNodeName );
		pNode->AddObject( pModel );

		return pModel;
	}

	ModelPtr CModelManager::CreateModel(xst_castring& strName, xst_castring& strMeshName, xst_castring& strModelGroup, xst_castring& strMeshGroup)
	{
		//xst_assert( pSceneMgr, "(CModelManager::CreateModel)" );
		ModelPtr pModel = CreateModel( strName, strModelGroup );

		MeshPtr pMesh = m_pMeshMgr->CreateMesh( strMeshName, strMeshGroup );
		if( pMesh == xst_null )
		{
			DestroyResource( strName );
			return ModelPtr();
		}

		pModel->SetMesh( pMesh );

		return pModel;
	}
	
	ModelPtr CModelManager::CreateModel(CSceneManager* pSceneMgr, xst_castring& strName, xst_castring& strMeshName, xst_castring& strNodeName, xst_castring& strModelGroup, xst_castring& strMeshGroup)
	{
		xst_assert( pSceneMgr, "(CModelManager::CreateModel)" );
		ModelPtr pModel = CreateModel( strName, strModelGroup );
		MeshPtr pMesh;
		{
			//XST::CSimpleProfiler Prof( "create mesh" );
			pMesh = m_pMeshMgr->LoadMesh( strMeshName, strMeshGroup );
		}
		if( pMesh == xst_null )
		{
			DestroyResource( strName );
			return ModelPtr();
		}

		pModel->SetMesh( pMesh );
		
		CSceneNode* pNode = pSceneMgr->CreateNode( strNodeName );
		pNode->AddObject( pModel );

		return pModel;
	}


	Resources::IResource*	CModelManager::_CreateResource(xst_castring& strName, cul32& ulHandle, GroupPtr pGroup)
	{
		CModel* pModel = xst_new CModel( m_pRenderSystem, m_pRenderSystem->GetInputLayout( ILEs::POSITION ), this, ulHandle, strName, 100, XST::ResourceStates::CREATED, this->m_pMemoryMgr );
		//Set default material
		pModel->SetMaterial( m_pMeshMgr->GetMaterialManager()->GetDefaultMaterial() );
		return pModel;
	}

	i32	CModelManager::PrepareResource(ResourcePtr pRes)
	{
		ModelPtr pModel = pRes;

		return 0;
	}

}//xse