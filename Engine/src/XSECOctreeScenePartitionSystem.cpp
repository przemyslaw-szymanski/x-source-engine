#include "XSECOctreeScenePartitionSystem.h"
#include "XSECOctree.h"
#include "XSECSceneManager.h"
#include "XSEIRenderSystem.h"
#include "XSECModelManager.h"
#include "XSECMeshManager.h"
#include "XSECSceneNode.h"
#include "XSECCamera.h"
#include "XSECMaterialManager.h"
#include "XST/XSTCToString.h"

namespace XSE
{
	void NoCullTest(const CCamera*, const CBoundingVolume&, OBJECT_DISABLE_REASON*);
	void SphereCullTest(const CCamera*, const CBoundingVolume&, OBJECT_DISABLE_REASON*);
	void AABBCullTest(const CCamera*, const CBoundingVolume&, OBJECT_DISABLE_REASON*);
	void SphereAABBCullTest(const CCamera*, const CBoundingVolume&, OBJECT_DISABLE_REASON*);

	class COctreeListener : public IOctreeListener
	{
		friend class COctreeScenePartitionSystem;

		public:

								COctreeListener(COctreeScenePartitionSystem* pSystem) : m_pSystem( pSystem ) {}
			virtual				~COctreeListener() {}

					void		OnAddNode(const COctree* pNode)
					{
						#if defined( XSE_RENDERER_DEBUG )
							//For debug purpose
							XST::CAABB AABB = pNode->CalcAABB();
						
							SLineBoxOptions Options;
							Options.vecSize = AABB.CalcSize();
							Options.vecPos = AABB.CalcCenter();
							Options.colColor = XSE::CColor( 0, 255, 0 );
							xst_stringstream ssName;
							ssName << m_pSystem->m_strSceneNodeName << "/" << AABB.CalcCenter();
							IInputLayout* pIL = m_pSystem->m_pInputLayout;

							//MeshPtr pMesh = m_pSystem->m_pSceneMgr->GetModelManager()->GetMeshManager()->CreateMesh( ssName.str(), pIL, BasicShapes::LINE_BOX, &Options );
							//pMesh->SetVisible( true );
							//m_pSystem->m_pOctModel->AddMesh( pMesh, true );
						#endif //XSE_RENDER_DEBUG
					}

		protected:

			COctreeScenePartitionSystem*	m_pSystem;
	};

	COctreeScenePartitionSystem::COctreeScenePartitionSystem(CSceneManager* pMgr) : 
		IScenePartitionSystem( pMgr ),
		m_byMaxOctreeDepth( 5 ),
		m_pOctree( xst_null )
	{
		SetViewFrustumCullTestType( ViewFrustumCullTypes::SPHERE );
		m_pOctListener = xst_new COctreeListener( this );
		m_pInputLayout = m_pSceneMgr->GetRenderSystem()->GetInputLayout( ILEs::POSITION | ILEs::COLOR );
		m_strSceneNodeName = m_pSceneMgr->GetName() + "/xse_octree_model";
		m_pOctModel = m_pSceneMgr->GetModelManager()->CreateModel( m_strSceneNodeName, m_pSceneMgr->GetName() );
		m_pOctModel->SetMaterial( m_pSceneMgr->GetModelManager()->GetMeshManager()->GetMaterialManager()->GetMaterial( CMaterialManager::DEFAULT_MAT_COLOR, DEFAULT_GROUP ) );
		m_pOctModel->SetVisible( false );
	}

	COctreeScenePartitionSystem::~COctreeScenePartitionSystem()
	{
		xst_delete( m_pOctree );
		xst_delete( m_pOctListener );
	}

	i32 COctreeScenePartitionSystem::Init()
	{
		m_vOctNodes.reserve( 2000 );

		m_pOctree = xst_new COctree( m_pSceneMgr->GetSize(), Vec3::ZERO, m_pOctListener );
		m_pSceneMgr->CreateNode( m_strSceneNodeName )->AddObject( m_pOctModel );
		m_pOctListener->OnAddNode( m_pOctree );
		return XST_OK;
	}

	void COctreeScenePartitionSystem::StartProcessing()
	{
		m_bProcessingStopped = false;
	}

	void COctreeScenePartitionSystem::StopProcessing()
	{
		m_bProcessingStopped = true;
	}

	void COctreeScenePartitionSystem::SkipProcess()
	{
		m_bProcessSkipped = true;
	}

#if defined( XSE_RENDERER_DEBUG )
	u32 g_uiObjChecked = 0;
	u32 g_uiObjDisabled = 0;
	u32 g_uiNodesChecked = 0;
	u32 g_uiAllNodes = 0;
	u32 g_uiNodesDisabled = 0;
	u32 g_uiRangeCullNodeDisabled = 0;
	u32 g_uiSphereCullNodeDisabled = 0;
	u32 g_uiAABBCullNodeDisabled = 0;
	u32 g_uiSphereCullObjDisabled;
	u32 g_uiAABBCullObjDisabled;
#endif
	void	COctreeScenePartitionSystem::Update()
	{
		if( m_bProcessSkipped || m_bProcessingStopped )
		{
			m_bProcessSkipped = false;
			return; //ok do nothing in this iteration
		}

		m_vOctNodes.clear();
#if defined( XSE_RENDERER_DEBUG )
		g_uiObjChecked = 0;
		g_uiObjDisabled = 0;
		g_uiNodesChecked = 0;
		g_uiAllNodes = 0;
		g_uiNodesDisabled = 0;
		g_uiAABBCullNodeDisabled = 0;
		g_uiSphereCullNodeDisabled = 0;
		g_uiRangeCullNodeDisabled = 0;
		g_uiSphereCullObjDisabled = 0;
		g_uiAABBCullObjDisabled = 0;
#endif
		CCamera* pCam = m_pSceneMgr->GetComputeCamera();

		CBoundingSphere CamSphere( pCam->GetViewDistance(), pCam->GetPosition() );
		
		//_OctRangeCull( CamSphere );
		//_OctFrustumCull( m_pOctree, pCam );

		_OctRangeCull( m_pOctree, CamSphere );
		_OctFrustumCull( pCam );
	}
			
	void COctreeScenePartitionSystem::AddObject(CObject* pObj)
	{
		this->m_vObjects.push_back( pObj );
		m_pOctree->AddObject( pObj, m_byMaxOctreeDepth );
		//XSE::CDebug::PrintDebugLN( pObj->_GetDbgName() );
	}

	void COctreeScenePartitionSystem::RemoveObject(CObject* pObj)
	{
		this->m_vObjects.erase( xst_find( this->m_vObjects.begin(), this->m_vObjects.end(), pObj ) );
		m_pOctree->RemoveObject( pObj );
	}


	void DisableOctNode(COctree* pNode, u32 uiReason)
	{
		if( pNode == xst_null )
			return;
#if defined( XSE_RENDERER_DEBUG )
g_uiNodesDisabled++;
#endif
		COctree::ObjectVector& vObjs = pNode->GetObjects();
		CObject* pObj;

		for(u32 i = vObjs.size(); i --> 0;)
		{
			pObj = vObjs[ i ];

			//If this object is disabled skip it, it is disabled by other culling test
			if( pObj->GetObjectDisableReason() != ODR::NOT_DISABLED )
				continue;
#if defined( XSE_RENDERER_DEBUG )
g_uiObjDisabled++;
#endif
			pObj->DisableObject( uiReason );
		}

		pNode->IsVisible( false );

		//Do the same for each child
		COctree** apChildren = pNode->GetChildren();
		DisableOctNode( apChildren[ 0 ], uiReason );
		DisableOctNode( apChildren[ 1 ], uiReason );
		DisableOctNode( apChildren[ 2 ], uiReason );
		DisableOctNode( apChildren[ 3 ], uiReason );
		DisableOctNode( apChildren[ 4 ], uiReason );
		DisableOctNode( apChildren[ 5 ], uiReason );
		DisableOctNode( apChildren[ 6 ], uiReason );
		DisableOctNode( apChildren[ 7 ], uiReason );
	}

	void COctreeScenePartitionSystem::_NodeObjectsFrustumCull(COctree* pNode, const CCamera* pCamera)
	{
		CObject* pObj;
		COctree::ObjectVector& vObjs = pNode->GetObjects();
		OBJECT_DISABLE_REASON eDisableReason;

		for(u32 o = vObjs.size(); o --> 0;)
		{
			pObj = vObjs[ o ];
			//If this object is disabled for other reason
			// TODO: Cache miss here
			if( pObj->GetObjectDisableReason() != ODR::NOT_DISABLED && pObj->GetObjectDisableReason() != ODR::FRUSTUM_CULLING )
				continue; //skip it

			//const CBoundingSphere& ObjSphere = pObj->GetObjectBoundingVolume().GetSphere();
			const CBoundingVolume& Volume = pObj->GetObjectBoundingVolume();
#if defined( XSE_RENDERER_DEBUG )
g_uiObjChecked++;
#endif
			eDisableReason = ODR::FRUSTUM_CULLING;
			/*if( pCamera->IsSphereInFrustum( Volume.GetSphere() ) )
			{
				eDisableReason = ODR::NOT_DISABLED;
				if( !pCamera->IsAABBInFrustum( Volume.GetAABB() ) )
				{
					eDisableReason = ODR::FRUSTUM_CULLING;
				}
			}*/
			// Function pointer should not be so slow in this case
			m_CullTest( pCamera, Volume, &eDisableReason );

			pObj->DisableObject( eDisableReason );
		}
	}

	void COctreeScenePartitionSystem::_NodeObjectsRangeCull(COctree* pNode, const CBoundingSphere& CamSphere)
	{
		CObject* pObj;
		COctree::ObjectVector& vObjs = pNode->GetObjects();
		f32 fDist;

		for(u32 o = vObjs.size(); o --> 0;)
		{
			pObj = vObjs[ o ];

			const CBoundingSphere& ObjSphere = pObj->GetObjectBoundingVolume().GetSphere();

			//If this object is disabled by other test do not test it
			if( pObj->GetObjectDisableReason() != ODR::NOT_DISABLED && pObj->GetObjectDisableReason() != ODR::RANGE_CULLING )
				continue;

			fDist = CamSphere.CalcDistance( ObjSphere );
			pObj->SetObjectDistanceToCamera( fDist );

			if( fDist >= ObjSphere.fRadius + CamSphere.fRadius )
			{
				pObj->DisableObject( ObjectDisableReasons::RANGE_CULLING );
			}
			else
			{
				pObj->DisableObject( ObjectDisableReasons::NOT_DISABLED );
			}
		}
	}

	//TODO
	void COctreeScenePartitionSystem::_OctFrustumCull(COctree* pNode, const CCamera* pCamera)
	{
		if( pNode == xst_null )
			return;

		// Check the bounding sphere
		// TODO: cache miss here. Calculate bounding volume based only on the node level and its position
		CBoundingVolume Volume = pNode->CalcBoundingVolume();
		if( !pCamera->IsSphereInFrustum( Volume.GetSphere() ) /*&& pNode->IsVisible()*/ )
		{
			DisableOctNode( pNode, ODR::FRUSTUM_CULLING );
			return;
		}

		if( !pCamera->IsAABBInFrustum( Volume.GetAABB() ) /*&& pNode->IsVisible()*/ )
		{
			DisableOctNode( pNode, ODR::FRUSTUM_CULLING );
			return;
		}

		pNode->IsVisible( true );
		m_vOctNodes.push_back( pNode );

		_NodeObjectsFrustumCull( pNode, pCamera );

		COctree** apChildren = pNode->GetChildren();
		_OctFrustumCull( apChildren[ 0 ], pCamera );
		_OctFrustumCull( apChildren[ 1 ], pCamera );
		_OctFrustumCull( apChildren[ 2 ], pCamera );
		_OctFrustumCull( apChildren[ 3 ], pCamera );
		_OctFrustumCull( apChildren[ 4 ], pCamera );
		_OctFrustumCull( apChildren[ 5 ], pCamera );
		_OctFrustumCull( apChildren[ 6 ], pCamera );
		_OctFrustumCull( apChildren[ 7 ], pCamera );
	}

	void COctreeScenePartitionSystem::_OctFrustumCull(const CCamera* pCamera)
	{
		for( u32 i = m_vOctNodes.size(); i-- > 0; )
		{
			COctree* pNode = m_vOctNodes[ i ];
			// Check the bounding sphere
			// TODO: cache miss here. Calculate bounding volume based only on the node level and its position
			CBoundingVolume Volume = pNode->CalcBoundingVolume();
			if( !pCamera->IsSphereInFrustum( Volume.GetSphere() ) /*&& pNode->IsVisible()*/ )
			{
#if defined( XSE_RENDERER_DEBUG )
g_uiSphereCullNodeDisabled++;
#endif
				DisableOctNode( pNode, ODR::FRUSTUM_CULLING );
				continue;
			}

			if( !pCamera->IsAABBInFrustum( Volume.GetAABB() ) /*&& pNode->IsVisible()*/ )
			{
#if defined( XSE_RENDERER_DEBUG )
g_uiAABBCullNodeDisabled++;
#endif
				DisableOctNode( pNode, ODR::FRUSTUM_CULLING );
				continue;
			}

			pNode->IsVisible( true );

			//Check node children
			_NodeObjectsFrustumCull( pNode, pCamera );
		}

	}

#define xst_vector_fast_remove_by_id(_vVector, _iId) 

	template<class _T_> 
	void XSTVectorFastRemove(xst_vector< _T_ >* pVecOut, u32 uiId)
	{
		_T_ tTmp = pVecOut->back();
		pVecOut->insert( pVecOut->begin() + pVecOut->size() - 1, xst_null );
		pVecOut->insert( pVecOut->begin() + uiId, tTmp );
		//pVecOut->pop_back();
	}

	//TODO
	void COctreeScenePartitionSystem::_OctRangeCull(const CBoundingSphere& CamSphere)
	{
		CObject* pObj;
		COctree* pNode;
		f32 fDist;

		for(u32 i = 0; i < m_vOctNodes.size() && m_vOctNodes[ i ] != xst_null; ++i)
		{
			pNode = m_vOctNodes[ i ];

			const CBoundingSphere NodeSphere = pNode->CalcBoundingSphere();
			//If the camera is too far from this node disable all objects
			if( !NodeSphere.Intersects( CamSphere ) )
			{
				DisableOctNode( pNode, ODR::RANGE_CULLING );
				m_vOctNodes[ i ] = m_vOctNodes.back();
				m_vOctNodes[ m_vOctNodes.size() - 1 ] = xst_null; 		
				continue;
			}

			_NodeObjectsRangeCull( pNode, CamSphere );
		}
		
	}

	void COctreeScenePartitionSystem::_OctRangeCull(COctree* pNode, const CBoundingSphere& CamSphere)
	{
		if( pNode == xst_null )
			return;

		const CBoundingSphere NodeSphere = pNode->CalcBoundingSphere();
		//If the camera is too far from this node disable all objects
		if( !NodeSphere.Intersects( CamSphere ) )
		{
#if defined( XSE_RENDERER_DEBUG )
g_uiRangeCullNodeDisabled++;
#endif
			DisableOctNode( pNode, ODR::RANGE_CULLING );
			return;
		}

		m_vOctNodes.push_back( pNode );

		_NodeObjectsRangeCull( pNode, CamSphere );

		COctree** apChildren = pNode->GetChildren();
		_OctRangeCull( apChildren[ 0 ], CamSphere );
		_OctRangeCull( apChildren[ 1 ], CamSphere );
		_OctRangeCull( apChildren[ 2 ], CamSphere );
		_OctRangeCull( apChildren[ 3 ], CamSphere );
		_OctRangeCull( apChildren[ 4 ], CamSphere );
		_OctRangeCull( apChildren[ 5 ], CamSphere );
		_OctRangeCull( apChildren[ 6 ], CamSphere );
		_OctRangeCull( apChildren[ 7 ], CamSphere );
	}

	void COctreeScenePartitionSystem::SetViewFrustumCullTestType(const VIEW_FRUSTUM_CULL_TEST_TYPE& eType)
	{
		IScenePartitionSystem::m_eViewFrustumCullType = eType;
		switch( eType )
		{
			case ViewFrustumCullTypes::NONE:
				m_CullTest = &NoCullTest;
			break;
			case ViewFrustumCullTypes::AABB:
				m_CullTest = &AABBCullTest;
			break;
			case ViewFrustumCullTypes::SPHERE:
				m_CullTest = &SphereCullTest;
			break;
			case ViewFrustumCullTypes::SPHERE_AABB:
				m_CullTest = &SphereAABBCullTest;
			break;
		}
	}

	// Locals
	void NoCullTest(const CCamera*, const CBoundingVolume&, OBJECT_DISABLE_REASON* peOut)
	{
		*peOut = ObjectDisableReasons::NOT_DISABLED;
	}

	void SphereCullTest(const CCamera* pCam, const CBoundingVolume& Vol, OBJECT_DISABLE_REASON* peReasonOut)
	{
		*peReasonOut = ( pCam->IsSphereInFrustum( Vol.GetSphere() ) )? ObjectDisableReasons::NOT_DISABLED : ObjectDisableReasons::FRUSTUM_CULLING;
	}

	void AABBCullTest(const CCamera* pCam, const CBoundingVolume& Vol, OBJECT_DISABLE_REASON* peReasonOut)
	{
		*peReasonOut = ( pCam->IsAABBInFrustum( Vol.GetAABB() ) )? ObjectDisableReasons::NOT_DISABLED : ObjectDisableReasons::FRUSTUM_CULLING;
	}

	void SphereAABBCullTest(const CCamera* pCam, const CBoundingVolume& Vol, OBJECT_DISABLE_REASON* peReasonOut)
	{
#if defined( XSE_RENDERER_DEBUG )
g_uiSphereCullObjDisabled++;	
#endif
		if( pCam->IsSphereInFrustum( Vol.GetSphere() ) )
		{
			*peReasonOut = ObjectDisableReasons::NOT_DISABLED;
			if( !pCam->IsAABBInFrustum( Vol.GetAABB() ) )
			{
				*peReasonOut = ObjectDisableReasons::FRUSTUM_CULLING;
#if defined( XSE_RENDERER_DEBUG )
g_uiSphereCullObjDisabled--;
g_uiAABBCullObjDisabled++;
#endif
			}
		}
	}
}