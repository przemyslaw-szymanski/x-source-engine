#include "XSECModel.h"
#include "XSECSceneNode.h"

namespace XSE
{
	namespace Resources
	{

		CModel::CModel(IRenderSystem* pRS, IInputLayout* pIL, XSE_IRESOURCE_DECL_PARAMS) : 
			XSE_IRESOURCE_CTOR, 
			IRenderableObject( OT::MODEL, pIL, strName.c_str() ),
			m_pRS( pRS ),
			m_byMinLod( 0 ),
			m_byMaxLod( 4 )
		{
			m_RenderMethod = &CModel::_Render;
			//m_vMeshes.push_back( MeshPtr() );
		}

		CModel::~CModel()
		{
			//m_pMesh = xst_null;
			m_pRS = xst_null;
		}

		void CModel::Render(IRenderSystem* pRS)
		{
			//m_pMesh->Render( pRS );
			//m_pCurrLOD->pMesh->Render( pRS );
			xst_stl_foreach( m_MeshItr, m_vMeshes )
			{
				(*m_MeshItr)->Render( pRS );
			}
		}

		void CModel::SetMesh(MeshPtr pMesh, cu32& uiId)
		{
			//xst_assert( m_pMesh != xst_null, "Mesh is already set" );

			//m_pMesh = pMesh;
			////Mesh material has higher priority so set model material to mesh material
			//if( m_pMesh->GetMaterial() )
			//{
			//	m_pMaterial = m_pMesh->GetMaterial();
			//}
			//else
			//if( m_pMaterial )
			//{
			//	m_pMesh->SetMaterial( m_pMaterial );
			//}
			m_vMeshes[ uiId ] = pMesh;
			if( m_vMeshes[ uiId ]->m_pMaterial == xst_null )
			{
				m_vMeshes[ uiId ]->m_pMaterial = this->m_pMaterial;
			}
		}

		void CModel::SetMaterial(MaterialPtr pMat, cu32& uiId)
		{
			this->m_pMaterial = pMat;
			if( m_vMeshes.size() > 0 && m_vMeshes[ uiId ] != xst_null )
			{
				m_vMeshes[ uiId ]->SetMaterial( pMat );
			}
		}

		void CModel::_SetSceneNode(CSceneNode* pNode)
		{
			this->m_pSceneNode = pNode;
			xst_stl_foreach( m_MeshItr, m_vMeshes )
			{
				(*m_MeshItr)->_SetSceneNode( pNode );
			}
		}

		void CModel::Disable(cu32& uiReason)
		{
			this->m_uiObjDisableReason = uiReason;

			if( !this->IsDisabled() )
			{
				this->SetVisible( this->IsVisible() );
			}
			else
			{
				m_RenderMethod = &CModel::_RenderEmpty;
			}

			xst_stl_foreach( m_MeshItr, m_vMeshes )
			{
				(*m_MeshItr)->Disable( uiReason );
			}
		}

		void CModel::SetVisible(bool bVisible)
		{
			this->m_bVisible = bVisible;

			if( bVisible && !this->IsDisabled() )
			{
				m_RenderMethod = &CModel::_Render;
			}
			else
			{
				m_RenderMethod = &CModel::_RenderEmpty;
			}

			xst_stl_foreach( m_MeshItr, m_vMeshes )
			{
				(*m_MeshItr)->SetVisible( bVisible );
			}
		}

		void CModel::SetLOD(u8 byLOD)
		{
			byLOD = XST::Math::Clamp( byLOD, m_byMinLod, m_byMaxLod );
			xst_stl_foreach( m_MeshItr, m_vMeshes )
			{
				(*m_MeshItr)->SetLOD( byLOD );
			}

			m_byCurrLod = byLOD;
		}

		u32 CModel::AddMesh(MeshPtr pMesh, bool bSetModelMaterial)
		{ 
			this->m_pSceneNode->AddObject( pMesh );
			m_vMeshes.push_back( pMesh ); 
			this->CalcObjectBoundingVolume();
			CObject::IsDirty( true );
			this->m_pSceneNode->ReorganizeObject( this );
			if( bSetModelMaterial )
				pMesh->SetMaterial( this->m_pMaterial );
			if( pMesh->IsVisible() )
				pMesh->SetVisible( this->IsVisible() );
			return m_vMeshes.size() - 1; 
		}

		const CBoundingVolume&	CModel::CalcObjectBoundingVolume()
		{
			MeshVec::iterator Itr;
			Vec3 vecMin( Vec3::ZERO ), vecMax( Vec3::ZERO );
			
			xst_stl_foreach( Itr, m_vMeshes )
			{
				const XST::CAABB& AABB = (*Itr)->GetBoundingVolume().GetAABB();
				if( vecMin > AABB.vecMin )
					vecMin = AABB.vecMin;
				if( vecMax < AABB.vecMax )
					vecMax = AABB.vecMax;
			}

			this->m_ObjBoundingVolume.BuildFromMinMax( vecMin, vecMax );
			return this->m_ObjBoundingVolume;
		}

		i32 CModel::_Render()
		{
			//m_pMesh->Render();
			return 0;
		}

		void CModel::SetPosition(cf32& fX, cf32& fY, cf32& fZ)
		{
			IRenderableObject::SetPosition( fX, fY, fZ );
			for( MeshPtr& pMesh : m_vMeshes )
			{
				// Meshes need to be updated
				pMesh->IsDirty( true );
			}
		}

	}//resources

}//xse