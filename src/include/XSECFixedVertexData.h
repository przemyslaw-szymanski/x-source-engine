#ifndef XSE_CFIXED_VERTEX_DATA_H
#define XSE_CFIXED_VERTEX_DATA_H

#include "XSERenderSystemCommon.h"
#include "XST/XSTCBinaryData.h"
#include "XSEIInputLayout.h"
#include "XSECFixedVertexData.h"

namespace XSE
{

	class XST_API CFixedVertexData
	{
		friend class CVertexData;
		protected:


		public:
									CFixedVertexData();
			virtual					~CFixedVertexData();

					i32				Create(ul32 ulVertexSize, ul32 ulVertexCount);

					i32				Clone(CFixedVertexData* pData);

			xst_fi	const IInputLayout*	GetInputLayout() const
										{ return m_pInputLayout; }

					u8*				GetData()
									{ return m_Data.GetData(); }

					cu8*			GetData() const
									{ return m_Data.GetData(); }

			xst_fi	XST::CBinaryData&	GetBinaryData()
									{ return m_Data; }

			xst_fi	const XST::CBinaryData&	GetBinaryData() const
											{ return m_Data; }

					ul32			GetVertexCount()
									{ return m_ulVertexCount; }

					cul32			GetVertexCount() const
									{ return m_ulVertexCount; }

			xst_fi	ul32			GetSize()
									{ return m_Data.GetSize(); }

					//void			AddAttribute(VERTEX_ATTRIBUTE eAttribute);

					i32				GetAttribute(cul32& ulVertexId, cul32& ulElementOffset, Vec2* pVec);
					i32				GetAttribute(cul32& ulVertexId, cul32& ulElementOffset, Vec3* pVec);
					i32				GetAttribute(cul32& ulVertexId, cul32& ulElementOffset, Vec4* pVec);

			xst_fi	i32				GetPosition(cul32& ulVertexId, cul32& ulElementOffset, Vec3* pVec)
									{ return GetAttribute( ulVertexId, ulElementOffset, pVec ); }

			xst_fi	i32				GetNormal(cul32& ulVertexId, cul32& ulElementOffset, Vec3* pVec)
									{ return GetAttribute( ulVertexId, ulElementOffset, pVec ); }

			xst_fi	i32				GetBinormal(cul32& ulVertexId, cul32& ulElementOffset, Vec3* pVec)
									{ return GetAttribute( ulVertexId, ulElementOffset, pVec ); }

			xst_fi	i32				GetTangent(cul32& ulVertexId, cul32& ulElementOffset, Vec3* pVec)
									{ return GetAttribute( ulVertexId, ulElementOffset, pVec ); }

			xst_fi	i32				GetTexCoord(cul32& ulVertexId, cul32& ulElementOffset, Vec2* pVec)
									{ return GetAttribute( ulVertexId, ulElementOffset, pVec ); }

			xst_fi	i32				GetColor(cul32& ulVertexId, cul32& ulElementOffset, Vec4* pVec)
									{ return GetAttribute( ulVertexId, ulElementOffset, pVec ); }
			
					i32				SetAttribute(cul32& ulVertexId, cul32& ulElementOffset, const Vec2& Vec);
					i32				SetAttribute(cul32& ulVertexId, cul32& ulElementOffset, const Vec3& Vec);
					i32				SetAttribute(cul32& ulVertexId, cul32& ulElementOffset, const Vec4& Vec);

					i32				SetAttribute(cul32& ulVertexId, cul32& ulElementOffset, cf32& fX, cf32& fY);
					i32				SetAttribute(cul32& ulVertexId, cul32& ulElementOffset, cf32& fX, cf32& fY, cf32& fZ);
					i32				SetAttribute(cul32& ulVertexId, cul32& ulElementOffset, cf32& fX, cf32& fY, cf32& fZ, cf32& fW);

			xst_fi	i32				SetPosition(cul32& ulVertexId, cul32& ulElementOffset, const Vec3& Vec)
									{ return SetAttribute( ulVertexId, ulElementOffset, Vec ); }

			xst_fi	i32				SetNormal(cul32& ulVertexId, cul32& ulElementOffset, const Vec3& Vec)
									{ return SetAttribute( ulVertexId, ulElementOffset, Vec ); }

			xst_fi	i32				SetBinormal(cul32& ulVertexId, cul32& ulElementOffset, const Vec3& Vec)
									{ return SetAttribute( ulVertexId, ulElementOffset, Vec ); }

			xst_fi	i32				SetTangent(cul32& ulVertexId, cul32& ulElementOffset, const Vec3& Vec)
									{ return SetAttribute( ulVertexId, ulElementOffset, Vec ); }

			xst_fi	i32				SetTexCoord(cul32& ulVertexId, cul32& ulElementOffset, const Vec2& Vec)
									{ return SetAttribute( ulVertexId, ulElementOffset, Vec ); }

			xst_fi	i32				SetColor(cul32& ulVertexId, cul32& ulElementOffset, const Vec4& Vec)
									{ return SetAttribute( ulVertexId, ulElementOffset, Vec ); }

			xst_fi	i32				SetPosition(cul32& ulVertexId, cul32& ulElementOffset, cf32& fX, cf32& fY, cf32& fZ)
									{ return SetAttribute( ulVertexId, ulElementOffset, fX, fY, fZ ); }

			xst_fi	i32				SetNormal(cul32& ulVertexId, cul32& ulElementOffset, cf32& fX, cf32& fY, cf32& fZ)
									{ return SetAttribute( ulVertexId, ulElementOffset, fX, fY, fZ ); }

			xst_fi	i32				SetBinormal(cul32& ulVertexId, cul32& ulElementOffset, cf32& fX, cf32& fY, cf32& fZ)
									{ return SetAttribute( ulVertexId, ulElementOffset, fX, fY, fZ ); }

			xst_fi	i32				SetTangent(cul32& ulVertexId, cul32& ulElementOffset, cf32& fX, cf32& fY, cf32& fZ)
									{ return SetAttribute( ulVertexId, ulElementOffset, fX, fY, fZ ); }

			xst_fi	i32				SetTexCoord(cul32& ulVertexId, cul32& ulElementOffset, cf32& fX, cf32& fY)
									{ return SetAttribute( ulVertexId, ulElementOffset, fX, fY ); }

			xst_fi	i32				SetColor(cul32& ulVertexId, cul32& ulElementOffset, cf32& fR, cf32& fG, cf32& fB, cf32& fA)
									{ return SetAttribute( ulVertexId, ulElementOffset, fR, fG, fB, fA ); }

		protected:

					ul32			_CalcOffset(cul32& ulVertexId, cul32& ulElementOffset);

		protected:

			//Vertex data buffer
			XST::CBinaryData	m_Data;
			//Data pointer
			u8*					m_pData;
			//Vertex data buffer size
			ul32				m_ulBufferSize;
			//Vertex size in bytes
			ul32				m_ulVertexSize;
			//Input layout for this vertex data - vertex attributes
			IInputLayout*		m_pInputLayout;
			//Number of vertices
			ul32				m_ulVertexCount;
			//Atribute mapping: attribute id -> attribute id in m_Data
			u32					m_auiAttributeMap[ XSE::InputLayoutElements::_ENUM_COUNT ];
			//Check data
	};

}//xse

#endif