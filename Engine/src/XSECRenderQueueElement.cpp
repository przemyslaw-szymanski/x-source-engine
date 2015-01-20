#include "XSECSceneManager.h"
#include "XSECMaterial.h"
#include "XSEITechnique.h"
#include "XSEIPass.h"
#include "XSEIRenderSystem.h"
#include "XSECModel.h"

namespace XSE
{
	using namespace Resources;

	u32 RenderQueueTypes::TERRAIN		= 0;
	u32 RenderQueueTypes::STATIC		= 1;
	u32 RenderQueueTypes::ENVIRONMENT	= 2;
	u32 RenderQueueTypes::DYNAMIC		= 3;
	u32 RenderQueueTypes::SCREEN_SPACE	= 4;
	u32 RenderQueueTypes::GUI			= 5;

	CRenderQueueElement::CRenderQueueElement(RENDER_QUEUE_TYPE uiElementType, bool bManualDestroy) : 
		m_uiElementType( uiElementType ), 
		m_bManualDestroy( bManualDestroy )
	{
	}

	CRenderQueueElement::~CRenderQueueElement()
	{
		m_vObjects.clear();
		m_vManualRenderObjects.clear();
	}

	void CRenderQueueElement::Render(CSceneManager* pSceneMgr, IRenderSystem* pRenderSystem)
	{
		xst_assert2( pRenderSystem );
		xst_assert2( pSceneMgr );

		ITechnique* pTech;
		IPass* pPass;
		IVertexShader* pVS;
		IPixelShader* pPS;
		CMaterial* pMat;
		IRenderableObject* pObj;
		Mtx4 mtxTmp;

		//const CRenderTree::ObjVec& vObjs = m_RenderTree.GetObjects();

		//Render automatic rendered objects
		ObjectVector::iterator Itr;
		xst_stl_foreach( Itr, m_vObjects )
		{
			pObj = (*Itr);
			lpcastr dbg = pObj->_GetDbgName();
			if( pObj->IsDisabled() )
				continue; // TODO: Cache miss here

			pMat = pObj->GetMaterial().GetPtr();
			pObj->Update();
			pTech = pMat->GetCurrentTechnique();
			for(u32 i = 0; i < pTech->GetPassCount(); ++i)
			{
				pPass = pTech->GetPass( i );
				pVS = pPass->GetVertexShader().GetPtr();
				pPS = pPass->GetPixelShader().GetPtr();

				//Set shaders
				pRenderSystem->SetVertexShaderWithCheck( pVS );
				pRenderSystem->SetPixelShaderWithCheck( pPS );
				pRenderSystem->SetInputLayoutWithCheck( pObj->GetInputLayout() );
				//Do transformations
				//const Vec3& vecPos = pObj->GetPosition();
				//pRenderSystem->SetTranslation( vecPos );
				//const Mtx4& mtxTransform = pObj->GetTransformMatrix();
				pObj->GetWorldTransformMatrix( &mtxTmp );
				pRenderSystem->SetMatrix( MatrixTypes::WORLD, mtxTmp );
				//Update shaders input
				pRenderSystem->UpdateDrawCallInputs();
				//Draw object
				if( pObj->GetDisableReason() == ObjectDisableReasons::NOT_DISABLED )
				{
					pObj->Render( pRenderSystem );
				}
			}
		}

		//Render manual rendered objects
		xst_stl_foreach( Itr, m_vManualRenderObjects )
		{
			(*Itr)->Update();
			(*Itr)->Render( pRenderSystem );
		}
	}

	void CRenderQueueElement::SortObjects()
	{
	}

	void CRenderQueueElement::AddObject(IRenderableObject* pObject)
	{
		xst_assert( pObject != xst_null, "Null object" );

		if( pObject->IsManualRendering() )
		{
			m_vManualRenderObjects.push_back( pObject );
			pObject->_OnAddToRenderQueue( this );
			return;
		}

		switch( pObject->GetObjectType() )
		{
			case ObjectTypes::MODEL:
			{
				CModel* pModel = (CModel*)pObject;
				CModel::MeshVecItr Itr;
				xst_stl_foreach( Itr, pModel->GetMeshes() )
				{
					AddObject( (*Itr) );
				}
			}
			break;

			case ObjectTypes::MESH:
			{
				m_vObjects.push_back( pObject );
				pObject->_OnAddToRenderQueue( this );
			}
			break;

			case ObjectTypes::TERRAIN:
			{
				m_vObjects.push_back( pObject );
				pObject->_OnAddToRenderQueue( this );
			}
			break;
		}
	}


	void CRenderQueueElement::RemoveObject(IRenderableObject* pObject)
	{
	}

}//xse