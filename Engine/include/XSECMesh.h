#ifndef XSE_CMESH_H
#define XSE_CMESH_H

#include "XSECommon.h"
#include "XSEIRenderableObject.h"
#include "XSECMaterial.h"
#include "XSEIVertexBuffer.h"
#include "XSEIIndexBuffer.h"
#include "XSETCFreeListAllocator.h"

#include <array>

namespace XSE
{
	struct XST_API SMeshLOD
	{
		SMeshLOD()  :	ulBeginVertexId( 0 ), ulEndIndexId( 0 ), ulEndVertexId( 0 ), ulBeginIndexId( 0 ), fDistance( 0.0f ), byID( -1 ),
						pVertexBuffer( xst_null ), pIndexBuffer( xst_null ), pMaterial( xst_null ) {}

		SMeshLOD(const VertexBufferPtr& pVB, const IndexBufferPtr& pIB, const MaterialPtr& pMat,
				ul32 ulBeginVId, ul32 ulEndVId, ul32 ulBeginIId, ul32 ulEndIId, f32 fRange, u8 byID) :
				pVertexBuffer( pVB ), pIndexBuffer( pIB ), pMaterial( pMat ), ulBeginVertexId( ulBeginIId ), ulEndVertexId( ulEndVId ),
				ulBeginIndexId( ulBeginIId ), ulEndIndexId( ulEndIId ), fDistance( fDistance ), byID( byID ) {}

		~SMeshLOD()
		{
			//xst_release( pVertexBuffer );
			//xst_release( pIndexBuffer );
		}

		VertexBufferPtr	pVertexBuffer;
		IndexBufferPtr	pIndexBuffer;
		MaterialPtr		pMaterial;
		ul32			ulBeginVertexId;
		ul32			ulEndVertexId;
		ul32			ulBeginIndexId;
		ul32			ulEndIndexId;
		f32				fDistance;
		ch8				byID;
	};

	namespace Resources
	{
		class XST_API CMesh : public IRenderableObject, public IResource, public XST::TIAllocableObject< TCFreeListAllocator< CMesh > >
		{
			friend class CMeshManager;
			friend class CModel;
			friend class CStaticGeometry;
			friend class CStaticGeometryGroup;
			friend class CDynamicGeometry;
			friend class CDynamicGeometryGroup;

			public:

				typedef XST::TCObjectSmartPointer< CMesh >	MeshPtr;
                typedef XST::TCWeakPointer< CMesh >         MeshWeaKPtr;

				//XST_TYPEDEF_OBJSMART_PTR( CMesh )	MeshPtr;

				//xst_tvectorx( MeshPtr )			MeshVector;
				//typedef MeshVector::iterator	MeshIterator;
				typedef i32 (CMesh::*pfnRender)();

				//typedef xst_vector< SMeshLOD >	LODVec;
				//typedef std::array< SMeshLOD, 20 > LODVec;
				typedef XST::TCConstantArray< SMeshLOD, XSE_MESH_MAX_LOD_LEVELS + 30 > LODVec;

				//typedef XST::TCIterator< MeshVector >		MeshIterator;
				//typedef XST::TCConstIterator< MeshVector >	ConstMeshIterator;

			public:
											CMesh();
											CMesh(IRenderSystem* pRS, IInputLayout* pIL, lpcastr pDbgName);
											CMesh(IRenderSystem* pRS, IInputLayout* pIL, XSE_IRESOURCE_DECL_PARAMS_DEFAULT);
				virtual						~CMesh();

						void				Render(IRenderSystem* pRS) xst_implement;

						void				Update(cf32& fTime = 0.0f) xst_implement;

						void				Disable(cu32& uiReason) xst_implement;

                        void                SetVisible(bool bVisible) xst_implement;

                xst_fi  Handle              GetObjectHandle() const xst_implement
                                            { return this->GetResourceHandle(); }

						VertexBufferPtr		CreateVertexBuffer(bool bUseAsDefault = true);
						
						void				SetVertexBuffer(const VertexBufferWeakPtr& pVB, cu32& uiLODId = 0);

						IndexBufferPtr		CreateIndexBuffer(bool bUseAsDefault = true);
						
						void				SetIndexBuffer(const IndexBufferWeakPtr& pIB, cu32& uiLODId = 0);

				xst_fi	SMeshLOD*			GetLOD(cu32& uiId)
											{ return &m_vLODs[ uiId ]; }

						void				SetLOD(cu8& uiLOD);
						void				SetLOD(cu32& uiLOD, const SMeshLOD& LOD);

				xst_fi
				const	SMeshLOD*			GetLOD(cu32& uiId) const
											{ return &m_vLODs[ uiId ]; }

						void				SetLODCount(cu32& uiLODCount);

						u8					GetCurrentLODID() const
											{ return m_pCurrentLOD->byID; }

				xst_fi	SMeshLOD*			GetCurrentLOD()
											{ return m_pCurrentLOD; }

				xst_fi	
				const	SMeshLOD*			GetCurrentLOD() const
											{ return m_pCurrentLOD; }

						//u32					AddLOD(const SMeshLOD& pMeshLOD);
						SMeshLOD&			AddLOD();
						SMeshLOD&			AddLOD(cu32& uiLODID);

						//SMeshLOD*			CreateLOD();

				xst_fi	VertexBufferPtr		GetVertexBuffer()
											{ xst_assert( m_pCurrentLOD, "(CMesh::GetVertexBuffer) Current lod is not set" ); return m_pCurrentLOD->pVertexBuffer; }

				xst_fi	
				const	VertexBufferPtr		GetVertexBuffer() const
											{ xst_assert( m_pCurrentLOD, "(CMesh::GetVertexBuffer) Current lod is not set" ); return m_pCurrentLOD->pVertexBuffer; }

				xst_fi	IndexBufferPtr		GetIndexBuffer(cu32& uiLODId = 0)
											{ xst_assert( m_pCurrentLOD, "(CMesh::GetIndexBuffer) Current lod is not set" ); return m_pCurrentLOD->pIndexBuffer; }

				xst_fi	
				const	IndexBufferPtr		GetIndexBuffer(cu32& uiLODId = 0) const
											{ xst_assert( m_pCurrentLOD, "(CMesh::GetIndexBuffer) Current lod is not set" ); return m_pCurrentLOD->pIndexBuffer; }

						void				DestroyBuffers();

						void				SetMaterial(MaterialWeakPtr pMat, cu32& uiLODId = 0);

						MaterialWeakPtr		GetMaterial() xst_implement;
				const	MaterialWeakPtr		GetMaterial() const xst_implement;

				xst_fi	bool				IsGeometryIndexed() const
											{ return m_bIndexedGeometry; }

				//xst_fi	xst_castring&		GetObjectName() const
				//							{ return this->_GetDbgName(); /*this->m_strResourceName;*/ }

						void				SetInputLayout(cul32& ulInputLayout);
						void				SetInputLayout(const IInputLayout* pIL);

			protected:

						void				_Init(XSE::IResourceManager* pCreator, const Handle& Handle, const Name& strName, XST::Types::ci32& iType, XST::Types::ci32& iState);
					
						void				_SetSceneNode(CSceneNode* pNode);

						i32					_Render();

						i32					_RenderIndexed();

						i32					_RenderEmpty() 
											{ return XST_OK; }

			protected:

				LODVec			m_vLODs;
				SMeshLOD*		m_pCurrentLOD;
				IRenderSystem*	m_pRS;
				//MeshVector		m_vSubMeshes;
				pfnRender		m_RenderMethod;

				ul32			m_ulCloneId;
				bool			m_bIndexedGeometry;
				bool			m_bIsCloned;
		};

		XST_TEMPLATE_CLASS XST::TCObjectSmartPointer< CMesh >;
        XST_TEMPLATE_CLASS XST::TCWeakPointer< CMesh >;

	}//resources

	typedef Resources::CMesh::MeshPtr MeshPtr;
    typedef Resources::CMesh::MeshWeaKPtr MeshWeakPtr;

}//xse

#endif