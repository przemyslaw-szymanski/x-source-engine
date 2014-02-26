#include "GUI/XSEIGUIRenderer.h"
#include "XSECStaticGeometry.h"
#include "XSECMeshManager.h"
#include "XSEIInputLayout.h"
#include "XSEIIndexBuffer.h"
#include "XSEIVertexBuffer.h"
#include "GUI/XSECComponent.h"
#include "XSEIRenderSystem.h"
#include "XSECMaterial.h"
#include "XSEIPass.h"
#include "XSECMaterialManager.h"
#include "XSECDynamicGeometryGroup.h"
#include "XSECShaderManager.h"
#include "XSECSceneManager.h"
#include "XSECDynamicGeometry.h"

namespace XSE
{
	namespace GUI
	{
		void CreateQuadVertices(CVertexData* pDataOut, IInputLayout* pIL);
		void CreateQuadIndices(CIndexData* pDataOut);

		IGUIRenderer::IGUIRenderer(IRenderSystem* pRS, CSceneManager* pSceneMgr, CMeshManager* pMeshMgr) :
			m_pRS( pRS ),
			m_pMeshMgr( pMeshMgr ),
			m_pSceneMgr( pSceneMgr ),
			m_pDynamicGeometry( xst_null )
		{
		}

		IGUIRenderer::~IGUIRenderer()
		{
			//m_pSceneMgr->DestroyDynamicGeometry( m_pDynamicGeometry );
		}

		i32 IGUIRenderer::Init()
		{
			m_pDefaultMaterial = m_pMeshMgr->GetMaterialManager()->CreateMaterial( "xse_default_gui_material", "GUI" );
			if( m_pDefaultMaterial == xst_null )
			{
				return XST_FAIL;
			}

			IPass* pPass = m_pDefaultMaterial->GetCurrentTechnique()->GetPass( 0 );
			//Create vertex shader

			return XST_OK;
		}

		i32 IGUIRenderer::AddComponent(CComponent* pComp)
		{
			xst_assert( pComp, "(IGUIRenderer::AddComponnet" );
			/*CompMap::iterator Itr = m_mComps.find( pComp->GetMaterial() );
			if( Itr == m_mComps.end() )
			{
				CompVector vComps;
				vComps.push_back( pComp );
				m_mComps.insert( CompMap::value_type( pComp->GetMaterial(), vComps ) );
			}
			else
			{
				Itr->second.push_back( pComp );
			}*/
			m_vComps.push_back( pComp );
			return XST_OK;
		}

		CDynamicGeometryGroup* pGroup;
		MaterialPtr pTmpMat;

		i32 IGUIRenderer::BuildGUI(IInputLayout* pIL, xst_castring& strGroup)
		{
			//Create quad mesh
			SRect2DOptions Opts;
			Opts.eUsage = BufferUsages::DYNAMIC;
			Opts.vecPosition = Vec2::ZERO;
			Opts.vecSize = ( 150.0f, 100.0f );
			MeshPtr pMesh = m_pMeshMgr->CreateMesh( "xst_tmp_gui_quad_mesh", pIL, BasicShapes::RECT_2D, &Opts, strGroup );
			if( pMesh == xst_null )
			{
				return XST_FAIL;
			}
			//Create one batch of dynamic geometry
			//m_pStaticGeometry = xst_new CStaticGeometry( "xse_gui_buffer", m_pRS, m_pSceneMgr, m_pMeshMgr, strGroup );
			m_pDynamicGeometry = m_pSceneMgr->CreateDynamicGeometry( "xse_gui_buffer" );
			m_pDynamicGeometry->GetOnBuildDelegate().bind( this, &IGUIRenderer::_OnComponentBuild );
			
			CDynamicGeometryGroup* pGr = m_pDynamicGeometry->CreateGroup();
			//CompMap::iterator Itr;
			CompVector::iterator Itr;
			xst_stl_foreach( Itr, m_vComps )
			{	
				/*CompVector::iterator CompItr;
				xst_stl_foreach( CompItr, Itr->second )
				{
					pGr->AddMesh( pMesh, Itr->first, (*CompItr)->GetPosition() );
					pTmpMat = Itr->first;
				}*/
				pGr->AddMesh( pMesh, (*Itr)->GetMaterial(), (*Itr)->GetPosition(), (*Itr) );
			}

			m_pDynamicGeometry->Build();

			//m_pSceneMgr->AddToRenderQueue( RenderQueueTypes::GUI, m_pDynamicGeometry );
			m_pMeshMgr->DestroyResource( pMesh );
			return XST_OK;
		}

		void IGUIRenderer::SetComponentPosition(const CComponent* pCmp)
		{
			Vec3 vecPos;
			if( m_pDynamicGeometry.IsValid() )
			{
				CVertexData* pData = m_pDynamicGeometry->GetVertexData( pCmp->m_ulGroupId, pCmp->m_ulSubGroupId );
				pData->SetPosition( pCmp->m_usBeginVertex + 0, pCmp->m_vecPosition );
				pData->SetPosition( pCmp->m_usBeginVertex + 1, Vec3( pCmp->m_vecPosition.x + pCmp->m_vecSize.x, pCmp->m_vecPosition.y, pCmp->m_vecPosition.z ) );
				pData->SetPosition( pCmp->m_usBeginVertex + 2, Vec3( pCmp->m_vecPosition.x + pCmp->m_vecSize.x, pCmp->m_vecPosition.y - pCmp->m_vecSize.y, pCmp->m_vecPosition.z ) );
				pData->SetPosition( pCmp->m_usBeginVertex + 3, Vec3( pCmp->m_vecPosition.x , pCmp->m_vecPosition.y - pCmp->m_vecSize.y, pCmp->m_vecPosition.z ) );
			}
		}

		void IGUIRenderer::RenderGUI()
		{
			m_pDynamicGeometry->Update();
		}

		void IGUIRenderer::SetSceneManager(XSE::CSceneManager *pSM)
		{
			m_pSceneMgr = pSM;
		}

		void IGUIRenderer::_OnComponentBuild(xst_unknown pCmp, const CDynamicGeometryObject* pObj)
		{
			if( !pCmp || !pObj )
				return;

			CComponent* pComponent = (CComponent*)pCmp;
			pComponent->m_usBeginIndex = pObj->m_usBeginIndex;
			pComponent->m_usIndexCount = (u16)pObj->m_ulIndexCount;
			pComponent->m_ulGroupId = pObj->m_ulGroupId;
			pComponent->m_ulSubGroupId = pObj->m_ulSubGroupId;
			pComponent->m_usBeginVertex = (u16)pObj->m_ulBeginVertex;
		}

	}//gui
}//xse