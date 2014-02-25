#include "XSECDynamicGeometry.h"
#include "XSECDynamicGeometryGroup.h"
#include "XSEIRenderSystem.h"
#include "XSEIPass.h"
#include "XSEITechnique.h"

namespace XSE
{
	CDynamicGeometry::CDynamicGeometry(	xst_castring &strName, IRenderSystem *pRS, CSceneManager *pSceneMgr, 
										CMeshManager *pMeshMgr, xst_castring &strGroup) :
		IRenderableObject( ObjectTypes::DYNAMIC_GEOMETRY, xst_null, strName.c_str(), true ),
		m_strName( strName ),
		m_pRenderSystem( pRS ),
		m_pSceneMgr( pSceneMgr ),
		m_pMeshMgr( pMeshMgr ),
		m_cstrGroupName( strGroup ),
		m_bIsBuilt( false )
	{
		m_OnBuild.bind( this, &CDynamicGeometry::_OnObjectBuild );
	}

	CDynamicGeometry::~CDynamicGeometry()
	{
		DestroyGroups();
	}

	void CDynamicGeometry::Render(IRenderSystem* pRS)
	{

		//CDynamicGeometryGroup::SubGroupIterator Itr = pGroup->GetSubGroupIterator();
		//Set index buffers
		//m_pRS->SetIndexBuffer( pGroup->GetIndexBuffer().GetPointer() );
		pRS->UpdateObjectInputs();
		pRS->SetTopology( TopologyTypes::TRIANGLE_LIST );

		//while( Itr.HasMoreElements() )
		GroupVector::iterator GrItr;
		CDynamicGeometryGroup::SubGroupVector::iterator SubGrItr;

		xst_stl_foreach( GrItr, m_vGroups )
		{
			xst_stl_foreach( SubGrItr, (*GrItr)->m_vSubGroups )
			{
				//Set input layout
				pRS->SetInputLayoutWithCheck( (*SubGrItr).GetInputLayout() );
				//Set vertex buffer for input layout
				pRS->SetVertexBuffer( (*SubGrItr).GetVertexBuffer() );
				pRS->SetIndexBuffer( (*SubGrItr).GetIndexBuffer() );

				//Set material
				//Loop for each batch
				CDynamicGeometrySubGroup::BatchVector::const_iterator BatchItr;
				for(BatchItr = (*SubGrItr).GetBatches().begin(); BatchItr != (*SubGrItr).GetBatches().end(); ++BatchItr)
				{
					const Resources::CMaterial* pMat = (*BatchItr).GetMaterial();
					const ITechnique* pTech = pMat->GetCurrentTechnique();//Itr.GetValue().GetMaterial()->GetCurrentTechnique();
					for(u32 i = 0; i < pTech->GetPassCount(); ++i)
					{
						pRS->SetVertexShader( pTech->GetPass( i )->GetVertexShader().GetPointer() );
						pRS->SetPixelShader( pTech->GetPass( i )->GetPixelShader().GetPointer() );
						
						ul32 ulIndexCount = (*BatchItr).GetIndexCount();
						ul32 ulBeginIndex = (*BatchItr).GetBeginIndex();
						pRS->DrawIndexed( ulIndexCount - ulBeginIndex, ulBeginIndex, 0 );
						//m_pRS->DrawVertexBuffer( Itr.GetValue().GetVertexBuffer(), Itr.GetValue().GetIndexBuffer() );
						
					}
				}
			}
		}
	}

	CVertexData* CDynamicGeometry::GetVertexData(ul32 ulGroupId, ul32 ulSubGroupId)
	{
		xst_assert2( m_vGroups[ ulGroupId ]->m_vSubGroups[ ulSubGroupId ].m_pVertexBuffer != xst_null );
		return &m_vGroups[ ulGroupId ]->m_vSubGroups[ ulSubGroupId ].m_pVertexBuffer->GetVertexData();
	}

	void CDynamicGeometry::DestroyGroups()
	{
		GroupVector::iterator Itr;
		xst_stl_foreach( Itr, m_vGroups )
		{
			xst_delete( (*Itr) );
		}

		m_vGroups.clear();
	}

	CDynamicGeometry::Group*	CDynamicGeometry::CreateGroup(const Vec3& vecPos)
	{
		Group* pGroup = xst_new CDynamicGeometryGroup( this, m_vGroups.size() );
		if( pGroup == xst_null )
		{
			return xst_null;
		}
		pGroup->m_OnObjectBuild = this->m_OnBuild;
		m_vGroups.push_back( pGroup );
		return pGroup;
	}

	void CDynamicGeometry::SetVisible(bool bVisible)
	{
		this->m_bVisible = bVisible;
	}

	void CDynamicGeometry::Disable(cu32& uiReason)
	{
		this->m_uiObjDisableReason = uiReason;
	}

	i32	CDynamicGeometry::Build()
	{
		if( m_bIsBuilt )
		{
			return XST_OK;
		}

		GroupVector::iterator Itr;
		i32 iResult = XST_OK;
		xst_stl_foreach( Itr, m_vGroups )
		{
			iResult += (*Itr)->Build( m_pMeshMgr );
		}

		m_bIsBuilt = true;
		return iResult;
	}

	void CDynamicGeometry::Update()
	{
		GroupVector::iterator Itr;
		xst_stl_foreach( Itr, m_vGroups )
		{
			(*Itr)->Update();
		}
	}

	void CDynamicGeometry::AddMesh(Group* pGroup, const MeshPtr& pMesh, const MaterialPtr& pMat, const Vec3& vecPos)
	{
	}

	void CDynamicGeometry::AddMesh(ul32 ulGroupID, const MeshPtr& pMesh, const MaterialPtr& pMat, const Vec3& vecPos)
	{
	}

	void CDynamicGeometry::AddMesh(const GroupIterator& GrItr, const MeshPtr& pMesh, const MaterialPtr& pMat, const Vec3& vecPos)
	{
	}


}//xse