//#include "XSECStaticGeometry.h"
//#include "XSECMesh.h"
//#include "XSEIVertexBuffer.h"
//#include "XSEIIndexBuffer.h"
//#include "XSECSceneManager.h"
//#include "XSECMeshManager.h"
//#include "XSECSceneNode.h"
//
//namespace XSE
//{
//	CStaticGeometryGroup::CStaticGeometryGroup(XSE::CStaticGeometry *pSG, IInputLayout* pIL) :
//		IRenderableObject( OT::STATIC_GEOMETRY_GROUP, pIL ),
//		m_pStaticGeometry( pSG ),
//		m_ulVBufferSize( 0 ),
//		m_ulIBufferSize( 0 ),
//		m_ulIndexCount( 0 ),
//		m_ulVertCount( 0 ),
//		m_pInputLayout( pIL ),
//		m_bBuilt( false )
//	{
//	}
//
//	CStaticGeometryGroup::~CStaticGeometryGroup()
//	{
//	}
//
//	void CStaticGeometryGroup::Render(IRenderSystem* pRS)
//	{
//		xst_assert( m_pMesh != xst_null, "(CStaticGeometryGroup::Render)" );
//		m_pMesh->Render( pRS );
//	}
//
//	i32 CStaticGeometryGroup::AddMesh(const XSE::MeshPtr &pMesh, const MaterialPtr& pMat, const XSE::Vec3 &vecPosition)
//	{
//		Resources::CMesh* pSrcMesh = pMesh.GetPointer();
//		m_ulVBufferSize += pSrcMesh->GetVertexBuffer()->GetVertexData().GetSize();
//		m_ulVertCount += pSrcMesh->GetVertexBuffer()->GetVertexData().GetVertexCount();
//		m_ulIBufferSize += pSrcMesh->GetIndexBuffer()->GetIndexData().GetSize();
//		m_ulIndexCount += pSrcMesh->GetIndexBuffer()->GetIndexData().GetIndexCount();
//		SGeometry Geom = { pSrcMesh, pMat, vecPosition };
//		m_vMeshes.push_back( Geom );
//		return XST_OK;
//	}
//
//	void CStaticGeometryGroup::_SetSceneNode(CSceneNode* pNode)
//	{
//		this->m_pSceneNode = pNode;
//		if( m_pMesh ) m_pMesh.GetPointer()->_SetSceneNode( pNode );
//	}
//
//	i32 CStaticGeometryGroup::Build()
//	{
//		if( m_bBuilt )
//		{
//			XST_LOG_ERR( "Static geometry group in static geometry: " << m_pStaticGeometry->GetName() << " alread built" );
//			return XST_FAIL;
//		}
//
//		xst_castring strName( m_pStaticGeometry->GetName() + XST::StringUtil::ToString( m_pStaticGeometry->m_ulBuildId ) );
//
//		m_pMesh = m_pStaticGeometry->m_pMeshMgr->CreateMesh( strName, this->m_pInputLayout, m_pStaticGeometry->m_strGroupName );
//		if( m_pMesh == xst_null )
//		{
//			return XST_FAIL;
//		}
//
//		CMesh* pTmpMesh;
//		ul32 ulCurrVId = 0;;
//		ul32 ulCurrIId = 0;
//		Vec3 vecTmp3, vecPos;
//		Vec4 vecTmp4;
//		Vec2 vecTmp2;
//
//		IVertexBuffer* pVB = m_pMesh->CreateVertexBuffer();
//		IIndexBuffer* pIB = m_pMesh->CreateIndexBuffer();
//
//		pVB->SetInputLayout( m_pInputLayout );
//		pVB->SetTopologyType( TopologyTypes::TRIANGLE_LIST );
//		pVB->SetUsage( BufferUsages::STATIC );
//		pVB->SetVertexCount( m_ulVertCount );
//
//		pVB->Lock();
//		CVertexData& VData = pVB->GetVertexData();
//
//		//Create vertex buffer
//		for(u32 m = 0; m < m_vMeshes.size(); ++m)
//		{
//			pTmpMesh = m_vMeshes[ m ].pMesh;
//
//			IVertexBuffer* pTmpVB = pTmpMesh->GetVertexBuffer();
//			
//			CVertexData& TmpVData = pTmpVB->GetVertexData();
//			
//			m_pMesh->SetMaterial( m_vMeshes[ m ].pMat );
//
//			for(ul32 v = 0; v < pTmpVB->GetVertexCount(); ++v)
//			{
//				if( m_pInputLayout->IsPosition() )
//				{
//					TmpVData.GetPosition( v, &vecTmp3 );
//					VData.SetPosition( ulCurrVId, vecTmp3 + m_vMeshes[ m ].vecPosition );
//				}
//
//				if( m_pInputLayout->IsNormal() )
//				{
//					TmpVData.GetNormal( v, &vecTmp3 );
//					VData.SetNormal( ulCurrVId, vecTmp3 );
//				}
//
//				if( m_pInputLayout->IsBinormal() )
//				{
//					TmpVData.GetBinormal( v, &vecTmp3 );
//					VData.SetBinormal( ulCurrVId, vecTmp3 );
//				}
//
//				if( m_pInputLayout->IsTangent() )
//				{
//					TmpVData.GetTangent( v, &vecTmp3 );
//					VData.SetTangent( ulCurrVId, vecTmp3 );
//				}
//
//				if( m_pInputLayout->IsColor() )
//				{
//					TmpVData.GetColor( v, &vecTmp4 );
//					VData.SetColor( ulCurrVId, vecTmp4 );
//				}
//
//				if( m_pInputLayout->IsTexCoord0() )
//				{
//					TmpVData.GetTexCoord( v, &vecTmp2, 0 );
//					VData.SetTexCoord( ulCurrVId, 0, vecTmp2 );
//				}
//
//				++ulCurrVId;
//			}
//		}
//
//		pVB->Unlock();
//		pVB->Create();
//
//		pIB->SetIndexCount( m_ulIndexCount );
//		pIB->SetUsage( BufferUsages::STATIC );
//
//		pIB->Lock();
//		ul32 ulCurrVCount = 0;
//
//		CIndexData& IData = pIB->GetIndexData();
//
//		for(u32 m = 0; m < m_vMeshes.size(); ++m)
//		{
//			pTmpMesh = m_vMeshes[ m ].pMesh;
//			IIndexBuffer* pTmpIB = pTmpMesh->GetIndexBuffer();
//			CIndexData& TmpIData = pTmpIB->GetIndexData();
//
//			for(ul32 i = 0; i < TmpIData.GetIndexCount(); ++i)
//			{
//				IData.SetIndex( ulCurrIId++, ulCurrVCount + TmpIData.GetIndex( i ) );
//			}
//
//			ulCurrVCount += pTmpMesh->GetVertexBuffer()->GetVertexCount();
//		}
//
//		pIB->Unlock();
//		pIB->Create();
//
//		m_vMeshes.clear();
//
//		m_bBuilt = true;
//		return XST_OK;
//	}
//
//
//	//STATIC GEOMETRY
//
//	CStaticGeometry::CStaticGeometry(xst_castring& strName, IRenderSystem* pRS, CSceneManager* pSM, 
//									CMeshManager* pMeshMgr, xst_castring& strGroupName) :
//		IRenderableObject( OT::STATIC_GEOMETRY, xst_null ),
//		m_strName( strName ),
//		m_strGroupName( strGroupName ),
//		m_pRS( pRS ),
//		m_pSceneMgr( pSM ),
//		m_pMeshMgr( pMeshMgr ),
//		m_ulBuildId( 0 )
//	{
//	}
//	
//	CStaticGeometry::~CStaticGeometry()
//	{
//		DestroyGroups();
//	}
//
//	CStaticGeometry::Group*	CStaticGeometry::CreateGroup(IInputLayout* pIL)
//	{
//		Group* pGr = xst_new CStaticGeometryGroup( this, pIL );
//		if( pGr == xst_null )
//		{
//			return xst_null;
//		}
//
//		m_vGroups.push_back( pGr );
//
//		return pGr;
//	}
//
//	void CStaticGeometry::DestroyGroups()
//	{
//		for(GroupVector::iterator Itr = m_vGroups.begin(); Itr != m_vGroups.end(); ++Itr)
//		{
//			xst_release( (*Itr) );
//		}
//
//		m_vGroups.clear();
//	}
//
//	void CStaticGeometry::DestroyGroup(ul32 ulId)
//	{
//		xst_delete( m_vGroups[ ulId ] );
//		GroupVector::iterator Itr;
//		std::advance( Itr, ulId );
//		m_vGroups.erase( Itr );
//	}
//
//	i32	CStaticGeometry::Build()
//	{
//		i32 iResult = XST_OK;
//		for(GroupVector::iterator Itr = m_vGroups.begin(); Itr != m_vGroups.end(); ++Itr)
//		{
//			if( !(*Itr)->IsBuilt() )
//			{
//				iResult += (*Itr)->Build();
//				++m_ulBuildId;
//			}
//		}
//
//		return iResult;
//	}
//
//	void CStaticGeometry::Render(IRenderSystem* pRS)
//	{
//		for(GroupVector::iterator Itr = m_vGroups.begin(); Itr != m_vGroups.end(); ++Itr)
//		{
//			(*Itr)->Render( pRS );
//		}
//	}
//
//	CStaticGeometry::GroupIterator CStaticGeometry::GetGroupIterator() const
//	{
//		return GroupIterator( &m_vGroups );
//	}
//
//	void CStaticGeometry::SetVisible(bool bVisible)
//	{
//		m_bVisible = bIs;
//	
//		for(GroupVector::iterator Itr = m_vGroups.begin(); Itr != m_vGroups.end(); ++Itr)
//		{
//			(*Itr)->IsVisible( bIs );
//		}
//	}	
//
//	void CStaticGeometry::_SetSceneNode(CSceneNode* pNode)
//	{
//		this->m_pSceneNode = pNode;
//		//Create scene node for each group
//		GroupVector::iterator Itr;
//		xst_stl_foreach( Itr, m_vGroups )
//		{
//			(*Itr)->_SetSceneNode( pNode->CreateChildNode() );
//		}
//	}
//
//}//xse

#include "XSECStaticGeometry.h"
#include "XSECStaticGeometryGroup.h"
#include "XSEIRenderSystem.h"
#include "XSEIPass.h"
#include "XSEITechnique.h"
#include "XSECSceneNode.h"

namespace XSE
{
	CStaticGeometry::CStaticGeometry(	xst_castring &strName, IRenderSystem *pRS, CSceneManager *pSceneMgr, 
										CMeshManager *pMeshMgr, xst_castring &strGroup) :
		IRenderableObject( ObjectTypes::DYNAMIC_GEOMETRY, xst_null, strName.c_str(), true ),
		m_strName( strName ),
		m_pRenderSystem( pRS ),
		m_pSceneMgr( pSceneMgr ),
		m_pMeshMgr( pMeshMgr ),
		m_cstrGroupName( strGroup ),
		m_bIsBuilt( false )
	{
		m_OnBuild.bind( this, &CStaticGeometry::_OnObjectBuild );
	}

	CStaticGeometry::~CStaticGeometry()
	{
		DestroyGroups();
	}

	void CStaticGeometry::Render(IRenderSystem* pRS)
	{

		//CStaticGeometryGroup::SubGroupIterator Itr = pGroup->GetSubGroupIterator();
		//Set index buffers
		pRS->SetTopology( TopologyTypes::TRIANGLE_LIST );

		//while( Itr.HasMoreElements() )
		GroupVector::iterator GrItr;
		CStaticGeometryGroup::SubGroupVector::iterator SubGrItr;

		xst_stl_foreach( GrItr, m_vGroups )
		{
			//Update position
			pRS->SetTranslation( (*GrItr)->GetPosition() );
			pRS->UpdateObjectInputs();

			xst_stl_foreach( SubGrItr, (*GrItr)->m_vSubGroups )
			{
				//Set input layout
				pRS->SetInputLayoutWithCheck( (*SubGrItr).GetInputLayout() );
				//Set vertex buffer for input layout
				pRS->SetVertexBuffer( (*SubGrItr).GetVertexBuffer() );
				pRS->SetIndexBuffer( (*SubGrItr).GetIndexBuffer() );

				//Set material
				//Loop for each batch
				CStaticGeometrySubGroup::BatchVector::const_iterator BatchItr;
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

	CVertexData* CStaticGeometry::GetVertexData(ul32 ulGroupId, ul32 ulSubGroupId)
	{
		xst_assert2( m_vGroups[ ulGroupId ]->m_vSubGroups[ ulSubGroupId ].m_pVertexBuffer != xst_null );
		return &m_vGroups[ ulGroupId ]->m_vSubGroups[ ulSubGroupId ].m_pVertexBuffer->GetVertexData();
	}

	void CStaticGeometry::DestroyGroups()
	{
		GroupVector::iterator Itr;
		xst_stl_foreach( Itr, m_vGroups )
		{
			xst_delete( (*Itr) );
		}

		m_vGroups.clear();
	}

	CStaticGeometry::Group*	CStaticGeometry::CreateGroup(const Vec3& vecPos)
	{
		Group* pGroup = xst_new CStaticGeometryGroup( this, m_vGroups.size() );
		if( pGroup == xst_null )
		{
			return xst_null;
		}
		pGroup->m_OnObjectBuild = this->m_OnBuild;
		m_vGroups.push_back( pGroup );
		return pGroup;
	}

	void CStaticGeometry::SetVisible(bool bVisible)
	{
		this->m_bVisible = bVisible;
	}

	void CStaticGeometry::Disable(cu32& uiReason)
	{
		this->m_uiObjDisableReason = uiReason;
	}

	i32	CStaticGeometry::Build()
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

	void CStaticGeometry::Update()
	{
		GroupVector::iterator Itr;
		xst_stl_foreach( Itr, m_vGroups )
		{
			(*Itr)->Update();
		}
	}

	void CStaticGeometry::AddMesh(Group* pGroup, const MeshPtr& pMesh, const MaterialPtr& pMat, const Vec3& vecPos)
	{
	}

	void CStaticGeometry::AddMesh(ul32 ulGroupID, const MeshPtr& pMesh, const MaterialPtr& pMat, const Vec3& vecPos)
	{
	}

	void CStaticGeometry::AddMesh(const GroupIterator& GrItr, const MeshPtr& pMesh, const MaterialPtr& pMat, const Vec3& vecPos)
	{
	}

	void CStaticGeometry::_SetSceneNode(CSceneNode* pNode)
	{
		IRenderableObject::_SetSceneNode( pNode );
		GroupVector::iterator Itr;
		xst_stl_foreach( Itr, m_vGroups )
		{
			pNode->CreateChildNode()->AddObject( (*Itr) );
		}
	}

}//xse