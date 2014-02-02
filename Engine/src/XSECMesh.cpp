#include "XSECMesh.h"
#include "XSEIVertexBuffer.h"
#include "XSEIIndexBuffer.h"
#include "XSEIRenderSystem.h"

namespace XSE
{
	namespace Resources
	{

		xst_fi bool MeshLODCompare(const SMeshLOD& Left, const SMeshLOD& Right)
		{
			return Left.fDistance < Right.fDistance;
		}

		CMesh::CMesh(IRenderSystem* pRS, IInputLayout* pIL, XSE_IRESOURCE_DECL_PARAMS) :	
			XSE_IRESOURCE_CTOR, 
			IRenderableObject( OT::MESH, pIL, strName.c_str() ), 
			m_pRS( pRS ), 											
			m_bIndexedGeometry( false ),
			m_bIsCloned( false ),
			m_ulCloneId( 0 ),
			m_RenderMethod( &CMesh::_Render )
		{
			//m_RenderMethod = &CMesh::_Render;
			//m_vLODs.push_back( xst_new SMeshLOD() ); //add first lod, as default
			//m_pCurrentLOD = m_vLODs[ 0 ];
			//m_pVertexBuffer = m_pCurrentLOD->pVertexBuffer;
			//m_pIndexBuffer = m_pCurrentLOD->pIndexBuffer;
		}

		CMesh::~CMesh()
		{
			m_pCurrentLOD = xst_null;
			//DestroyBuffers();
			m_pRS = xst_null;
		}

		void CMesh::DestroyBuffers()
		{
			for(u32 i = 0; i < m_vLODs.size(); ++i)
			{
				//xst_delete( m_vLODs[ i ] );
			}

			m_vLODs.clear();
		}

		void CMesh::Update()
		{
			//if( m_pVertexBuffer ) m_pVertexBuffer->SetInputLayout( this->m_pInputLayout );
			/*for(u32 i = 0; i < m_vLODs.size(); ++i)
			{
				if( m_vLODs[ i ]->pVertexBuffer )
				{
					m_vLODs[ i ]->pVertexBuffer->SetInputLayout( this->m_pInputLayout );
				}
			}*/
			IRenderableObject::Update();
		}

		VertexBufferPtr CMesh::CreateVertexBuffer()
		{
			VertexBufferPtr pVB( m_pRS->CreateVertexBuffer() ); //~0.002sec in debug
			if( m_pInputLayout != xst_null )
			{
				pVB->SetInputLayout( m_pInputLayout );
			}
		
			if( m_vLODs.empty() )
			{
				//m_vLODs.push_back( xst_new SMeshLOD() );
				/*m_vLODs.push_back( SMeshLOD() );
				m_pCurrentLOD = &m_vLODs[ 0 ];
				m_pCurrentLOD->byID = 0;*/
				m_pCurrentLOD = &AddLOD();
			}

			if( m_vLODs[ 0 ].pVertexBuffer == xst_null )
			{
				m_vLODs[ 0 ].pVertexBuffer = pVB;
			}

			return pVB;

		}

		void CMesh::SetVertexBuffer(VertexBufferPtr pVB, cu32& uiLODId)
		{
			xst_assert( pVB != xst_null, "Vertex buffer is null" );
			m_vLODs[ uiLODId ].pVertexBuffer = pVB;
			if( m_vLODs[ uiLODId ].pIndexBuffer == xst_null )
			{
				m_vLODs[ uiLODId ].pIndexBuffer = m_vLODs[ 0 ].pIndexBuffer;
			}
		}

		IndexBufferPtr CMesh::CreateIndexBuffer()
		{
			IndexBufferPtr pIB( m_pRS->CreateIndexBuffer() );

			if( m_vLODs.empty() )
			{
				//Ptr lod
				/*m_vLODs.push_back( xst_new SMeshLOD() );
				m_pCurrentLOD = &m_vLODs[ 0 ];*/
				//Obj lod
				m_pCurrentLOD = &AddLOD();
			}
		
			if( m_vLODs[ 0 ].pIndexBuffer == xst_null )
			{
				m_vLODs[ 0 ].pIndexBuffer = pIB;
				m_bIndexedGeometry = true;
				//Change render method to render as indexed geometry
				m_RenderMethod = &CMesh::_RenderIndexed;
			}

			return pIB;
		}

		void CMesh::SetIndexBuffer(IndexBufferPtr pIB, cu32& uiLODId)
		{
			xst_assert( pIB != xst_null, "Vertex buffer is null" );
			m_vLODs[ uiLODId ].pIndexBuffer = pIB;
			if( m_vLODs[ uiLODId ].pVertexBuffer == xst_null )
			{
				m_vLODs[ uiLODId ].pVertexBuffer = m_vLODs[ 0 ].pVertexBuffer;
			}
			//DebugPrintIndexData( m_vLODs[ uiLODId ].pIndexBuffer->GetIndexData() );
			m_bIndexedGeometry = true;
			m_RenderMethod = &CMesh::_RenderIndexed;
		}

		void CMesh::SetMaterial(XSE::MaterialPtr pMat, cu32& uiLODId)
		{
			xst_assert( pMat != xst_null, "(CMesh::SetMaterial)" );
			//TODO Override GetMaterial method for mesh. Should returns current lod material
			m_pMaterial = pMat;
			if( uiLODId < m_vLODs.size() )
			{
				m_vLODs[ uiLODId ].pMaterial = pMat;
			}
		}

		void CMesh::_SetSceneNode(CSceneNode* pNode)
		{
			this->m_pSceneNode = pNode;
		}

		void CMesh::SetInputLayout(IInputLayout* pIL)
		{
			xst_assert( pIL != xst_null, "(CMesh::SetInputLayout)" );
			this->m_pInputLayout = pIL;
			for(u32 i = 0; i < m_vLODs.size(); ++i)
			{
				if( m_vLODs[ i ].pVertexBuffer )
				{
					m_vLODs[ i ].pVertexBuffer->SetInputLayout( pIL );
				}
			}
		}

		/*i32 CMesh::PrepareForRender()
		{
			xst_assert( m_pVertexBuffer, "(CMesh::PrepareForRender) VertexBuffer not created" );
			return m_pRS->SetVertexBuffer( m_pVertexBuffer );
		}*/

		void CMesh::Render(IRenderSystem* pRS)
		{
			XST_CALL_MEMBER_FN_PTR( this, m_RenderMethod )();
		}

		void CMesh::DisableObject(cu32& uiReason)
		{
			this->m_uiObjDisableReason = uiReason;

			if( !this->IsObjectDisabled() )
			{
				this->SetVisible( IRenderableObject::IsVisible() );
			}
			else
			{
				m_RenderMethod = &CMesh::_RenderEmpty;
			}
		}

		void CMesh::SetVisible(bool bVisible)
		{
			this->m_bVisible = bVisible;

			if( bVisible && !this->IsObjectDisabled() )
			{
				if( m_bIndexedGeometry )
				{
					m_RenderMethod = &CMesh::_RenderIndexed;
				}
				else
				{
					m_RenderMethod = &CMesh::_Render;
				}
			}
			else
			{
				m_RenderMethod = &CMesh::_RenderEmpty;
			}
		}

		//SMeshLOD* CMesh::CreateLOD()
		//{
		//	SMeshLOD* pLOD = xst_new SMeshLOD();
		//	//m_vLODs.push_back( pLOD );
		//	return pLOD;
		//}


		//u32 CMesh::AddLOD(SMeshLOD* pLOD)
		//{
		//	u32 uiID = m_vLODs.size();
		//	pLOD->byID = uiID;

		//	if( m_vLODs.empty() )
		//	{
		//		m_vLODs.push_back( pLOD );
		//		return uiID;
		//	}

		//	//If there is no first lod set it
		//	if( m_vLODs[ 0 ].pIndexBuffer == xst_null && m_vLODs[ 0 ].pVertexBuffer == xst_null )
		//	{
		//		m_vLODs[ 0 ] = pLOD;
		//		return uiID;
		//	}

		//	if( pLOD->pIndexBuffer == xst_null ) 
		//		pLOD->pIndexBuffer = m_vLODs[ 0 ].pIndexBuffer;
		//	if( pLOD->pVertexBuffer == xst_null ) 
		//		pLOD->pVertexBuffer = m_vLODs[ 0 ].pVertexBuffer;
		//	if( pLOD->pMaterial == xst_null ) 
		//		pLOD->pMaterial = m_vLODs[ 0 ].pMaterial;

		//	m_vLODs.push_back( pLOD );
		//	//DebugPrintIndexData( m_vLODs[ 0 ].pIndexBuffer->GetIndexData() );
		//	return uiID;
		//}

		SMeshLOD& CMesh::AddLOD()
		{
			SMeshLOD LOD;
			LOD.byID = m_vLODs.size();
			m_vLODs.push_back( LOD );

			if( m_vLODs.size() == 1 ) //if it is a first lod set it as current to use in rendering
			{
				m_pCurrentLOD = &m_vLODs.back();
				if( m_pCurrentLOD->pIndexBuffer == xst_null ) 
					m_pCurrentLOD->pIndexBuffer = m_vLODs[ 0 ].pIndexBuffer;
				if( m_pCurrentLOD->pVertexBuffer == xst_null ) 
					m_pCurrentLOD->pVertexBuffer = m_vLODs[ 0 ].pVertexBuffer;
				if( m_pCurrentLOD->pMaterial == xst_null ) 
					m_pCurrentLOD->pMaterial = m_vLODs[ 0 ].pMaterial;
			}
			return m_vLODs.back();
		}

		SMeshLOD& CMesh::AddLOD(cu32& uiLODID)
		{
			SMeshLOD* pLOD = &m_vLODs[ uiLODID ];
			pLOD->byID = (u8)uiLODID;

			if( m_pCurrentLOD == xst_null || m_pCurrentLOD->byID == -1 )
			{
				m_pCurrentLOD = pLOD;
			}

			return m_vLODs[ uiLODID ];
		}

		void CMesh::SetLODCount(cu32& uiLODCount)
		{
			//m_vLODs.reserve( uiLODCount );
			SMeshLOD LOD;
			m_vLODs.resize( uiLODCount, LOD );
			for( u32 i = 0; i < m_vLODs.size(); ++i )
			{
				m_vLODs[ i ] = m_vLODs[ 0 ];
				m_vLODs[ i ].byID = i;
			}
		}

		void CMesh::SetLOD(cu32& uiLOD, const SMeshLOD& LOD)
		{
			m_vLODs[ uiLOD ] = LOD;
		}

		void CMesh::SetLOD(cu8& uiLOD)
		{
			m_pCurrentLOD = &m_vLODs[ Math::Max< u8 >( 0, uiLOD ) ];
			xst_assert( m_pCurrentLOD, "(CMesh::SetLOD)" );
			xst_assert( m_pCurrentLOD->pVertexBuffer, "(CMesh::SetLOD)" );
			xst_assert( m_pCurrentLOD->pIndexBuffer, "(CMesh::SetLOD)" );
			//DebugPrintIndexData( m_pCurrentLOD->pIndexBuffer->GetIndexData() );
			//Todo
			//If previous material is different to new lod force update render queue
		}

		i32 CMesh::_Render()
		{
			xst_assert( this->m_pCurrentLOD != xst_null, "(CMesh::_Render) No LOD set for mesh" );
			const IVertexBuffer* pVB = this->m_pCurrentLOD->pVertexBuffer.GetPointer(); //this->m_pVertexBuffer.GetPointer();
			m_pRS->SetVertexBufferWithCheck( pVB );
			return m_pRS->DrawVertexBuffer( pVB );
		}

		i32 CMesh::_RenderIndexed()
		{
			xst_assert( this->m_pCurrentLOD != xst_null, "(CMesh::_RenderIndexed) No LOD set for mesh" );
			IVertexBuffer* pVB = this->m_pCurrentLOD->pVertexBuffer.GetPointer();
			IIndexBuffer* pIB = this->m_pCurrentLOD->pIndexBuffer.GetPointer();
			//DebugPrintIndexData( pIB->GetIndexData() );
			m_pRS->SetVertexBufferWithCheck( pVB );
			m_pRS->SetIndexBufferWithCheck( pIB );
			return m_pRS->DrawVertexBuffer( pVB, pIB );
		}

	}//resources
}//xse