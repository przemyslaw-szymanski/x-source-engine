#include "XSECSceneDebug.h"
#include "XSECSceneManager.h"
#include "XSECMeshManager.h"
#include "XSECMaterialManager.h"
#include "XSECModelManager.h"
#include "XSECSceneNode.h"
#include "XSEIRenderSystem.h"

namespace XSE
{
	CMeshManager* g_pMeshMgr;

	CSceneDebug::CSceneDebug(CSceneManager* pMgr) :
		m_pMgr( pMgr )
	{
		g_pMeshMgr = CMeshManager::GetSingletonPtr();
		SLineBoxOptions Options;
		Options.colColor = CColor::WHITE;
		Options.vecPos = Vec3::ZERO;
		Options.vecSize = 1.0f;

		IInputLayout* pIL = pMgr->GetRenderSystem()->GetInputLayout( ILEs::POSITION | ILEs::COLOR );
		//m_pAABBMesh = pMgr->GetMeshManager()->CreateMesh( "xse_dbg_aabb", ILEs::POSITION | ILEs::COLOR, BasicShapes::LINE_BOX, &Options, "Debug" );
		m_pAABBMesh = pMgr->GetMeshManager()->CreateMesh( "xse_dbg_aabb", pIL, BasicShapes::LINE_BOX, &Options, "Debug" );
		MaterialPtr pMat = pMgr->GetMaterialManager()->GetMaterial( CMaterialManager::DEFAULT_MAT_COLOR, DEFAULT_GROUP );
		m_pAABBMesh->SetMaterial( pMat );
		m_pAABBMesh->m_bDbgObject = true;
		m_pAABBMesh->SetVisible(false);
	}

	CSceneDebug::~CSceneDebug()
	{
		m_pMgr = xst_null;
	}

	void CSceneDebug::ShowPartitionSystem(bool bShow)
	{

	}

	void CSceneDebug::ShowObjectsAABB(bool bShow)
	{

	}
	extern xst_astring g_strName;
	ch8 g_strBuff[ 64 ];
	

	i32 CSceneDebug::_CreateObjectAABBMesh(CObject* pObj)
	{
#if defined( XSE_SCENE_DEBUG )
		return XST_FAIL;
	XSTSimpleProfiler();
		if( pObj->IsDbgObject() )
			return XST_FAIL;

		/*const XST::CAABB& AABB = pObj->GetBoundingVolume().GetAABB();

		MeshPtr pClonedMesh;
		{ XSTSimpleProfiler2( "CloneMesh" );
		pClonedMesh = MeshPtr( m_pAABBMesh->Clone( XST::StringUtil::EmptyAString, false ) );
		}
		{ XSTSimpleProfiler2( "CreateNode" );
		m_pMgr->CreateNode( pClonedMesh->GetResourceName() )->AddObject( pClonedMesh );
		}
		pObj->m_pAABBMesh = pClonedMesh.GetPtr();
		pObj->m_pAABBMesh->SetObjectScale( AABB.CalcSize() );
		Vec3 vecPos = AABB.CalcCenter();
		pObj->m_pAABBMesh->SetPosition( AABB.CalcCenter() );
		pObj->m_pAABBMesh->Update();
		pClonedMesh->m_bDbgObject = true;*/
		
		
		//xst_sprintf( g_strBuff, 64, "xse_clone_mesh_%d", pObj->GetObjectHandle() );
		//g_strName.assign( g_strBuff );
		////MeshPtr pM = g_pMeshMgr->CreateMesh( g_strName );
		//Resources::CMesh* pMesh = xst_new Resources::CMesh( 0, 0, 0 );

#endif
		return XST_OK;
	}

	void CSceneDebug::_UpdateObjectAABBMesh(CObject* pObj)
	{
#if defined( XSE_SCENE_DEBUG )
		if( pObj->m_pAABBMesh == xst_null || !pObj->m_pAABBMesh->IsVisible() || pObj->m_pAABBMesh->IsDisabled() )
			return;
#endif
	}
}