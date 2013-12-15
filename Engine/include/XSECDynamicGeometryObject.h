#ifndef XSE_CDYNAMIC_GEOMETRY_OBJECT_H
#define XSE_CDYNAMIC_GEOMETRY_OBJECT_H

#include "XSECommon.h"

namespace XSE
{
	class CDynamicGeometrySubGroup;

	class XST_API CDynamicGeometryObject
	{
		friend class CDynamicGeometryGroup;
		friend class CDynamicGeometry;

		public:

			CDynamicGeometryObject() :	m_ulBeginVertex( 0 ), m_ulVertexCount( 0 ), m_usBeginIndex( 0 ), m_ulIndexCount( 0 ),
										m_pMaterial( xst_null ), m_pMesh( xst_null ), m_vecPosition( Vec3::ZERO ),
										m_ulGroupId( 0 ), m_ulSubGroupId( 0 ) {}
			virtual				~CDynamicGeometryObject() {}

			xst_fi bool operator<(const CDynamicGeometryObject& Right)
			{
				return this->m_pMesh->GetInputLayout() < Right.m_pMesh->GetInputLayout();
			}

			xst_fi bool operator==(const CDynamicGeometryObject& Right)
			{
				return	m_ulBeginVertex == Right.m_ulBeginVertex &&
						m_ulVertexCount == Right.m_ulVertexCount &&
						m_usBeginIndex == Right.m_usBeginIndex &&
						m_ulIndexCount == Right.m_ulIndexCount &&
						m_pMaterial == Right.m_pMaterial &&
						m_pMesh == Right.m_pMesh &&
						m_vecPosition == Right.m_vecPosition;
			}

			xst_fi i32 CompareMaterials(const CDynamicGeometryObject& Left) const
			{
				/*i32 i = m_pMaterial->Compare( Left.m_pMaterial );
				return i;*/
				if( m_pMaterial < Left.m_pMaterial ) return -1;
				else if( m_pMaterial > Left.m_pMaterial ) return 1;
				else return 0;
			}

			const Resources::CMaterial*	m_pMaterial;
			ul32						m_ulBeginVertex;
			ul32						m_ulVertexCount;
			u16							m_usBeginIndex;
			ul32						m_ulIndexCount;
			Resources::CMesh*			m_pMesh;
			Vec3						m_vecPosition;
			ul32						m_ulGroupId;
			ul32						m_ulSubGroupId;
	};

	class XST_API CDynamicGeometryBatch
	{
		friend class CDynamicGeometry;
		friend class CDynamicGeometryGroup;
		friend class CDynamicGeometryObject;

		public:

										CDynamicGeometryBatch() :	m_ulBeginVertex( 0 ), m_ulVertexCount( 0 ), m_usBeginIndex( 0 ), 
																	m_ulIndexCount( 0 ){}
			virtual						~CDynamicGeometryBatch() {}

			xst_fi	const Resources::CMaterial*	GetMaterial() const
												{ return m_pMaterial; }

			xst_fi	ul32				GetBeginVertex() const
										{ return m_ulBeginVertex; }

			xst_fi	ul32				GetVertexCount() const
										{ return m_ulVertexCount; }

			xst_fi	u16					GetBeginIndex() const
										{ return m_usBeginIndex; }

			xst_fi	ul32				GetIndexCount() const
										{ return m_ulIndexCount; }

		protected:

			const Resources::CMaterial*	m_pMaterial;
			ul32						m_ulBeginVertex;
			ul32						m_ulVertexCount;
			u16							m_usBeginIndex;
			ul32						m_ulIndexCount;
	};

	class XST_API CDynamicGeometrySubGroup
	{
		friend class CDynamicGeometry;
		friend class CDynamicGeometryGroup;
		friend class CDynamicGeometryBatch;

		public:

		typedef CDynamicGeometryObject		Object;
		typedef CDynamicGeometryBatch		Batch;
		typedef xst_vector< Object >		ObjectVector;
		typedef xst_vector< Batch >			BatchVector;
		typedef xst_vector< xst_unknown >	UserDataVector;

		public:

			CDynamicGeometrySubGroup() :m_pInputLayout( xst_null ), m_pVertexBuffer( xst_null ),
										m_pIndexBuffer( xst_null ), m_ulVertexCount( 0 ), m_ulIndexCount( 0 ){}

			xst_fi	const IVertexBuffer*	GetVertexBuffer() const
											{ return m_pVertexBuffer; }

			xst_fi	const IIndexBuffer*		GetIndexBuffer() const
											{ return m_pIndexBuffer; }

			xst_fi	const IInputLayout*		GetInputLayout() const
											{ return m_pInputLayout; }

			xst_fi	const BatchVector&		GetBatches() const
											{ return m_vBatches; }


		protected:

			const IInputLayout*		m_pInputLayout;
			IVertexBuffer*			m_pVertexBuffer;
			IIndexBuffer*			m_pIndexBuffer;
			ObjectVector			m_vObjects;
			ul32					m_ulVertexCount;
			ul32					m_ulIndexCount;
			BatchVector				m_vBatches;
			UserDataVector			m_vUserDatas;
	};

}//xse

#endif