#include "XSECDynamicGeometryGroup.h"
#include "XSEIRenderSystem.h"
#include "XSECMeshManager.h"
#include "XSEIVertexBuffer.h"
#include "XSEIIndexBuffer.h"
#include "XSECDynamicGeometry.h"

#include "XST/XSTAlgorithm.h"

#include "GUI/XSECComponent.h"

namespace XSE
{

	i32 CompareMaterials(const void* pLeft, const void* pRight)
	{
		const CDynamicGeometrySubGroup::Object* pLeftObj = (const CDynamicGeometrySubGroup::Object*)pLeft;
		const CDynamicGeometrySubGroup::Object* pRightObj = (const CDynamicGeometrySubGroup::Object*)pRight;

		return pLeftObj->CompareMaterials( *pRightObj );
	}

	i32 CmpObjMat(const CDynamicGeometrySubGroup::Object& Left, const CDynamicGeometrySubGroup::Object& Right)
	{
		//Left < Right == -1
		//Left == Right == 0
		//Left > Right == 1
		return Left.CompareMaterials( Right );
	}

	i32 CmpInt(i32 iLeft, i32 iRight)
	{
		if( iLeft == iRight ) return 0;
		if( iLeft < iRight ) return -1;
		if( iLeft > iRight) return 1;
	}

	//void SortByMaterial(CDynamicGeometryGroup::ObjectVector& vObjs);

	

	CDynamicGeometryGroup::CDynamicGeometryGroup(CDynamicGeometry* pDynamicGeometry, ul32 ulID) : 
		IRenderableObject( ObjectTypes::DYNAMIC_GEOMETRY_GROUP, xst_null, false ),
		m_pDynamicGeometry( pDynamicGeometry ),
		m_ulVBufferSize( 0 ),
		m_ulIndexCount( 0 ),
		m_pCurrIL( xst_null ),
		m_vecCurrRange( -1, 0 ),
		m_ulID( ulID )
	{
		m_OnObjectBuild.bind( this, &CDynamicGeometryGroup::_OnObjectBuildDefault );
	}

	CDynamicGeometryGroup::~CDynamicGeometryGroup()
	{
	}

	void CDynamicGeometryGroup::Render(IRenderSystem* pRS)
	{
	}

	void CDynamicGeometryGroup::SetVisible(bool bVisible)
	{
		this->m_bVisible = bVisible;
	}

	void CDynamicGeometryGroup::Disable(cu32& uiReason)
	{
		this->m_uiObjDisableReason = uiReason;
	}

	void CDynamicGeometryGroup::AddMesh(const MeshPtr& pMesh, const MaterialPtr& pMat, const Vec3& vecPos, xst_unknown pUserData)
	{
		SubGroup::Object Obj;
		Obj.m_pMaterial = pMat.GetPtr();
		Obj.m_vecPosition = vecPos;
		Obj.m_pMesh = pMesh.GetPtr();
	
		//m_vObjects.push_back( Obj );
		m_vObjects.push_back( Obj );

		//Find input layout in sub groups
		bool bFound = false;
		SubGroupVector::iterator Itr;
		xst_stl_foreach( Itr, m_vSubGroups )
		{
			if( (*Itr).GetInputLayout() == Obj.m_pMesh->GetInputLayout() )
			{
				bFound = true;
				//Add object to the existing sub group
				(*Itr).m_vObjects.push_back( Obj );
				(*Itr).m_vUserDatas.push_back( pUserData );
				(*Itr).m_ulVertexCount += pMesh.GetPtr()->GetVertexBuffer()->GetVertexCount();
				(*Itr).m_ulIndexCount += pMesh.GetPtr()->GetIndexBuffer()->GetIndexCount();
				break;
			}
		}

		if( !bFound )
		{
			SubGroup SubGr;
			SubGr.m_pInputLayout = Obj.m_pMesh->GetInputLayout();
			SubGr.m_vObjects.push_back( Obj );
			SubGr.m_vUserDatas.push_back( pUserData );
			SubGr.m_ulVertexCount += pMesh.GetPtr()->GetVertexBuffer()->GetVertexCount();
			SubGr.m_ulIndexCount += pMesh.GetPtr()->GetIndexBuffer()->GetIndexCount();
			m_vSubGroups.push_back( SubGr );
		}

		m_ulVBufferSize += Obj.m_pMesh->GetVertexBuffer()->GetVertexData().GetSize();
		m_ulIndexCount += Obj.m_pMesh->GetIndexBuffer()->GetIndexData().GetIndexCount();
	}

	i32	CDynamicGeometryGroup::Build(CMeshManager* pMeshMgr)
	{
		//Create batch mesh
		if( m_vObjects.size() < 1 )
		{
			return XST_OK;
		}


		SubGroupVector::iterator Itr;
		{
			XST::CSimpleProfiler P( "s" );
		//For each sub group, sort by material
		xst_stl_foreach( Itr, m_vSubGroups )
		{
			ObjectVector& vObjs = (*Itr).m_vObjects;
			XST::Algorithm::QuickSort1( &vObjs[ 0 ], 0, vObjs.size() - 1, CmpObjMat );
		}
		}

		Vec3 vecTmp3;
		Vec2 vecTmp2;
		Vec4 vecTmp4;
		const Resources::CMaterial* pCurrMat = xst_null;
		CDynamicGeometryBatch* pCurrBatch = xst_null;
		ul32 ulCurrSubGroupId = 0;

		//For each material grouped in each input layout create batch vertex buffer
		xst_stl_foreach( Itr, m_vSubGroups )
		{
			IVertexBuffer* pVB;
			IIndexBuffer* pIB;

			(*Itr).m_pVertexBuffer = pMeshMgr->GetRenderSystem()->CreateVertexBuffer();
			(*Itr).m_pIndexBuffer = pMeshMgr->GetRenderSystem()->CreateIndexBuffer();

			pVB = (*Itr).m_pVertexBuffer;
			pVB->SetInputLayout( (*Itr).m_pInputLayout );
			pVB->SetTopologyType( TopologyTypes::TRIANGLE_LIST );
			pVB->SetUsage( BufferUsages::DEFAULT );
			pVB->SetVertexCount( (*Itr).m_ulVertexCount );

			pIB = (*Itr).m_pIndexBuffer;
			pIB->SetUsage( BufferUsages::DEFAULT );
			pIB->SetIndexCount( (*Itr).m_ulIndexCount );

			pVB->Lock();
			pIB->Lock();
			
			CVertexData& VData = pVB->GetVertexData();
			CIndexData& IData = pIB->GetIndexData();

			const IInputLayout* pIL = (*Itr).m_pInputLayout;
			
			ul32 ulCurrVertex = 0;
			ul32 ulCurrIndexCount = 0;
			ul32 ulCurrVertexCount = 0;
			ul32 ulCurrIndex = 0;
			ul32 ulStartVertex = 0;
			u16	 usStartIndex = 0;

			for(u32 i = 0; i < (*Itr).m_vObjects.size(); ++i)
			{
				CVertexData& TmpVData = (*Itr).m_vObjects[ i ].m_pMesh->GetVertexBuffer()->GetVertexData();
				CIndexData& TmpIData = (*Itr).m_vObjects[ i ].m_pMesh->GetIndexBuffer()->GetIndexData();

				ulStartVertex = ulCurrVertex;

				//Set vertices
				for(u32 v = 0; v < TmpVData.GetVertexCount(); ++v)
				{
					if( pIL->IsPosition() )
					{
						TmpVData.GetPosition( v, &vecTmp3 );
						VData.SetPosition( ulCurrVertex, vecTmp3 + (*Itr).m_vObjects[ i ].m_vecPosition );
					}

					if( pIL->IsColor() )
					{
						TmpVData.GetColor( v, &vecTmp4 );
						VData.SetColor( ulCurrVertex, vecTmp4 );
					}

					//Increment current vertex
					++ulCurrVertex;
				}

				if( pCurrMat != (*Itr).m_vObjects[ i ].m_pMaterial )
				{
					pCurrMat = (*Itr).m_vObjects[ i ].m_pMaterial;
					//Create batch
					CDynamicGeometryBatch Batch;
					Batch.m_usBeginIndex = (u16)ulCurrIndex;
					Batch.m_ulIndexCount = TmpIData.GetIndexCount();
					Batch.m_pMaterial = pCurrMat;
					(*Itr).m_vBatches.push_back( Batch );
					pCurrBatch = &(*Itr).m_vBatches.back();
				}

				usStartIndex = (u16)ulCurrIndex;
				//Set indices
				for(u32 id = 0; id < TmpIData.GetIndexCount(); ++id)
				{
					u16 usIndex = TmpIData.GetIndex( id ) + ulCurrVertexCount;
					IData.SetIndex( ulCurrIndex++, TmpIData.GetIndex( id ) + ulCurrVertexCount );
				}

				pCurrBatch->m_ulIndexCount = ulCurrIndex;

				ulCurrVertexCount = ulCurrVertex;

				(*Itr).m_vObjects[ i ].m_ulBeginVertex = ulStartVertex;
				(*Itr).m_vObjects[ i ].m_ulVertexCount = TmpVData.GetVertexCount();
				(*Itr).m_vObjects[ i ].m_usBeginIndex = usStartIndex;
				(*Itr).m_vObjects[ i ].m_ulIndexCount = TmpIData.GetIndexCount();
				(*Itr).m_vObjects[ i ].m_ulGroupId = this->m_ulID;
				(*Itr).m_vObjects[ i ].m_ulSubGroupId = ulCurrSubGroupId;

				xst_assert2( m_OnObjectBuild != xst_null );
				CDynamicGeometryObject* pObj = &(*Itr).m_vObjects[ i ];
				m_OnObjectBuild( (*Itr).m_vUserDatas[ i ], pObj );
				//GUI::CComponent* pCmp = (GUI::CComponent*)(*Itr).m_vUserDatas[ i ];
			}

			pVB->Unlock();
			pVB->Create();

			pIB->Unlock();
			pIB->Create();

			++ulCurrSubGroupId;

			//Clear object buffer
			(*Itr).m_vObjects.clear();
			
		}

		return XST_OK;
	}

	void CDynamicGeometryGroup::Update()
	{
		SubGroupVector::iterator Itr;
		xst_stl_foreach( Itr, m_vSubGroups )
		{
			m_pDynamicGeometry->m_pRenderSystem->UpdateVertexBuffer( (*Itr).m_pVertexBuffer, 0, (*Itr).m_ulVertexCount, (*Itr).m_pVertexBuffer->GetVertexData() );
		}
		
	}

	void CDynamicGeometryGroup::_SetSceneNode(XSE::CSceneNode *pNode)
	{
		this->m_pSceneNode = pNode;
	}

	CDynamicGeometryGroup::BatchIterator CDynamicGeometryGroup::GetBatchIterator()
	{
		return BatchIterator( &m_vBatches );
	}

	CDynamicGeometryGroup::ConstBatchIterator CDynamicGeometryGroup::GetBatchIterator() const
	{
		return ConstBatchIterator( &m_vBatches );
	}

}//xse