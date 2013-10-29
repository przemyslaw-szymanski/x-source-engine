#ifndef XSE_CSTATIC_GEOMETRY_GROUP_H
#define XSE_CSTATIC_GEOMETRY_GROUP_H

#include "XSECMesh.h"
#include "XSECMaterial.h"
#include "XSECStaticGeometryObject.h"

namespace XSE
{
	class XST_API CStaticGeometryGroup : public IRenderableObject
	{
		friend class CStaticGeometry;

		public:

			typedef CStaticGeometryBatch								Batch;
			typedef CStaticGeometrySubGroup							SubGroup;
			typedef fastdelegate::FastDelegate2< xst_unknown, const CStaticGeometryObject* >	BuildDelegate;

		protected:

			typedef xst_vector< SubGroup::Object >						ObjectVector;
			//typedef XST::TCSortedDynamicArray< Object >					ObjectArray;
			typedef xst_vector< Batch >									BatchVector;
			typedef xst_vector< SubGroup >								SubGroupVector;
			typedef xst_map< const Resources::CMaterial*, BatchVector >	BatchMap;
			typedef xst_map< const IInputLayout*, BatchMap >			ObjectMap;
			typedef xst_vector< Vec2 >									RangeVector;

		public:

			typedef XST::TCIterator< BatchVector >		BatchIterator;
			typedef XST::TCConstIterator< BatchVector >	ConstBatchIterator;
			typedef XST::TCIterator< SubGroupVector >	SubGroupIterator;

		public:

									CStaticGeometryGroup(CStaticGeometry* pDynamicGeometry, ul32 ulID);
			virtual					~CStaticGeometryGroup();

					void			Render(IRenderSystem* pRS);

					void			SetVisible(bool bVisible);
					
					void			DisableObject(cu32& uiReason);


					void			AddMesh(const MeshPtr& pMesh, const MaterialPtr& pMat, const Vec3& vecPos = Vec3::ZERO, xst_unknown pUserData = xst_null);

					i32				Build(CMeshManager* pMeshMgr);

					void			Update();

					ul32			GetObjectHandle() const
									{ return 0; }

					xst_castring&	GetObjectName() const
									{ return XST::StringUtil::EmptyAString; }

			const	VertexBufferPtr	GetVertexBuffer() const
									{ return m_pVB; }

			const	IndexBufferPtr	GetIndexBuffer() const
									{ return m_pIB; }

					BatchIterator		GetBatchIterator();
					ConstBatchIterator	GetBatchIterator() const;

					SubGroupIterator	GetSubGroupIterator()
										{ return SubGroupIterator( &m_vSubGroups ); }

		protected:

					void			_SetSceneNode(CSceneNode* pNode);

					void			_OnObjectBuildDefault(xst_unknown pUserData, const CStaticGeometryObject* pObj) {}

		protected:

			VertexBufferPtr		m_pVB;
			IndexBufferPtr		m_pIB;
			//ObjectArray		m_aObjects;
			ObjectVector		m_vObjects;
			ul32				m_ulVBufferSize;
			ul32				m_ulIndexCount;
			BatchVector			m_vBatches;
			//Range of the input layouts sorted in object array
			Vec2				m_vecCurrRange;
			const IInputLayout* m_pCurrIL;
			RangeVector			m_vRanges;
			SubGroupVector		m_vSubGroups;
			CStaticGeometry*	m_pDynamicGeometry;
			BuildDelegate		m_OnObjectBuild;
			ul32				m_ulID;
	};

}//xse

#endif