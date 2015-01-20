#include "XSECRenderQueue.h"
#include "XSECMesh.h"
#include "XSECModel.h"
#include "XSEIRenderSystem.h"
#include "XSEITechnique.h"
#include "XSEIPass.h"
#include "XSECStaticGeometry.h"
#include "XSECDynamicGeometry.h"

namespace XSE
{
	

	CRenderQueue::CRenderQueue() 
	{
		//Add elements
		
		for(u32 i = RenderQueueTypes::TERRAIN; i <= RenderQueueTypes::GUI; ++i)
		{
			m_vElements.push_back( xst_new CRenderQueueElement( i ) );
		}
	}

	CRenderQueue::~CRenderQueue()
	{
		ElementVector::iterator Itr;
		xst_stl_foreach( Itr, m_vElements )
		{
			if( !(*Itr)->IsManualDestroy() )
			{
				xst_delete( (*Itr) );
			}
		}

		m_vElements.clear();
	}

	void CRenderQueue::Render(CSceneManager* pSceneMgr)
	{
		xst_assert2( m_pRS );
		xst_assert2( pSceneMgr );

		//ITechnique* pTech;
		//IPass* pPass;
		//IVertexShader* pVS;
		//IPixelShader* pPS;
		//CMaterial* pMat;
		//IRenderableObject* pObj;

		////const CRenderTree::ObjVec& vObjs = m_RenderTree.GetObjects();

		////Render automatic rendered objects
		//ObjectList::iterator Itr;
		//xst_stl_foreach( Itr, m_lObjects )
		//{
		//	pObj = (*Itr);
		//	pMat = pObj->GetMaterial().GetPtr();

		//	pTech = pMat->GetCurrentTechnique();
		//	for(u32 i = 0; i < pTech->GetPassCount(); ++i)
		//	{
		//		pPass = pTech->GetPass( i );
		//		pVS = pPass->GetVertexShader().GetPtr();
		//		pPS = pPass->GetPixelShader().GetPtr();

		//		//Set shaders
		//		m_pRS->SetVertexShaderWithCheck( pVS );
		//		m_pRS->SetPixelShaderWithCheck( pPS );
		//		m_pRS->SetInputLayoutWithCheck( pObj->GetInputLayout() );
		//		//Do transformations
		//		const Vec3& vecPos = pObj->GetPosition();
		//		m_pRS->SetTranslation( vecPos );
		//		//Update shaders input
		//		m_pRS->UpdateDrawCallInputs();
		//		//Draw object
		//		pObj->Render( m_pRS );
		//	}
		//}

		////Render manual rendered objects
		//xst_stl_foreach( Itr, m_lManualRenderObjects )
		//{
		//	(*Itr)->Render( m_pRS );
		//}

		ElementVector::iterator Itr;
		xst_stl_foreach( Itr, m_vElements )
		{
			(*Itr)->Render( pSceneMgr, m_pRS );
		}

	}

	void CRenderQueue::AddObject(RENDER_QUEUE_TYPE eType, IRenderableObject* pObj)
	{
		return m_vElements[ eType ]->AddObject( pObj );
	}

	void CRenderQueue::AddObject(IRenderableObject* pObj)
	{
		switch( pObj->GetObjectType() )
		{
			case OT::MESH:
			{
				m_vElements[ RenderQueueTypes::DYNAMIC ]->AddObject( pObj );
			}
			break;

			case OT::MODEL:
			{
				m_vElements[ RenderQueueTypes::DYNAMIC ]->AddObject( pObj );
			}
			break;

			case OT::DYNAMIC_GEOMETRY:
			{
				m_vElements[ RenderQueueTypes::DYNAMIC ]->AddObject( pObj );
			}
			break;

			case OT::DYNAMIC_GEOMETRY_GROUP:
			{
				m_vElements[ RenderQueueTypes::DYNAMIC ]->AddObject( pObj );
			}
			break;

			case OT::STATIC_GEOMETRY:
			{
				m_vElements[ RenderQueueTypes::STATIC ]->AddObject( pObj );
			}
			break;

			case OT::STATIC_GEOMETRY_GROUP:
			{
				m_vElements[ RenderQueueTypes::STATIC ]->AddObject( pObj );
			}
			break;

			case OT::TERRAIN:
			{
				m_vElements[ RenderQueueTypes::TERRAIN ]->AddObject( pObj );
			}
			break;
		}

	}

	void CRenderQueue::Remove(RENDER_QUEUE_TYPE eType, IRenderableObject* pObject)
	{
		ElementVector::iterator Itr;
		xst_stl_foreach( Itr, m_vElements )
		{
			(*Itr)->RemoveObject( pObject );
		}
	}

}//xse