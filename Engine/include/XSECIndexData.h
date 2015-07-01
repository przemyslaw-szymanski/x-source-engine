#ifndef XSE_CINDEX_DATA_H
#define XSE_CINDEX_DATA_H

#include "XSECommon.h"
#include "XSETCData.h"
#include "XST/XSTCBinaryData.h"

namespace XSE
{
	class XST_API CIndexData
	{
		typedef ul32 (CIndexData::*pfnGetIndex)(cul32&) const;

		public:

								CIndexData();
			virtual				~CIndexData();

					i32			Create(cul32& ulIndexCount);

					i32			Create(cul32& ulIndexCount, cu32& uiIndexSize);

					void		Delete();

					i32			CopyFrom(const CIndexData& Other);

			xst_fi	cu8*		GetData() const
								{ return m_Data.GetData(); }

			xst_fi	ul32		GetSize()
								{ return m_Data.GetSize(); }

			xst_fi	cul32		GetSize() const
								{ return m_Data.GetSize(); }

			xst_fi	ul32		GetIndexCount()
								{ return m_ulIndexCount; }

			xst_fi	cul32		GetIndexCount() const
								{ return m_ulIndexCount; }

			xst_fi	void		SetIndex(cul32& ulIndexId, cul32& ulIndex)
								{ m_Data.Write( ulIndex, _CalcOffset( ulIndexId ), m_usIndexSize ); }

					i32			SetData(cu8* pData, ul32 ulDataSize);

			xst_fi	ul32		GetIndex(cul32& ulIndexId)
								{ return XST_CALL_MEMBER_FN_PTR( this, _GetIndex )( ulIndexId ); }

			xst_fi	cul32		GetIndex(cul32& ulIndexId) const
								{ return XST_CALL_MEMBER_FN_PTR( this, _GetIndex )( ulIndexId ); }

					void		SetTriangle(cul32& ulTriangleId, cu16& usIndex0, cu16& usIndex1, cu16& usIndex2);

					void		AddTriangle(cu16& usIndex0, cu16& usIndex1, cu16& usIndex2);

			xst_fi	void		ResetTriangleId()
								{ m_ulCurrentTriangleId = 0; }

			xst_fi	u16			GetIndexSize() const
								{ return m_usIndexSize; }

			xst_fi	ul32		GetTriangleCount() const
								{ return m_ulTriangleCount; }

					void		GetTriangle(cu32& uTriangleId, u32 (*ppTriangleIndices)[3]) const;

		protected:

					ul32		_CalcOffset(cul32& ulIndexId) const;
			xst_fi	ul32		_GetU16Index(cul32& ulIndexId)
								{ u16 usIndex = 0; m_Data.Read( &usIndex, _CalcOffset( ulIndexId ), m_usIndexSize ); return usIndex; }

			xst_fi	ul32		_GetU32Index(cul32& ulIndexId)
								{ ul32 ulIndex = 0; m_Data.Read( &ulIndex, _CalcOffset( ulIndexId ), m_usIndexSize ); return ulIndex; }

					pfnGetIndex	_GetIndex;

		protected:

			//TCData< u32 >		m_Data;
			XST::CBinaryData	m_Data;
			ul32				m_ulTriangleCount;
			ul32				m_ulCurrentTriangleId;
			u16					m_usIndexSize; //size of the one index in bytes
			ul32				m_ulIndexCount; u8 m_padding[ 2 ];
			ul32				m_ulBufferSize;
	};

	//class CIndexData
	//{
	//	public:

	//							CIndexData();
	//		virtual				~CIndexData();

	//				i32			Create(cul32& ulIndexCount);

	//				void		Delete();

	//		xst_fi	cu32*		GetData() const
	//							{ return m_Data.GetData(); }

	//		xst_fi	ul32		GetSize()
	//							{ return m_Data.GetTotalByteCount(); }

	//		xst_fi	cul32		GetSize() const
	//							{ return m_Data.GetTotalByteCount(); }

	//		xst_fi	ul32		GetIndexCount()
	//							{ return m_ulIndexCount; }

	//		xst_fi	cul32		GetIndexCount() const
	//							{ return m_ulIndexCount; }

	//		xst_fi	void		SetIndex(cul32& ulIndexId, cul32& ulIndex)
	//							{ m_Data[ ulIndexId ] = ulIndex; }
	//							//{ m_Data.Write( ulIndex, _CalcOffset( ulIndexId ) ); }

	//		xst_fi	ul32		GetIndex(cul32& ulIndexId)
	//							{ return m_Data[ ulIndexId ]; }
	//							//{ ul32 ulIndex = 0; m_Data.Read( &ulIndex, _CalcOffset( ulIndexId ) ); return ulIndex; }

	//				void		SetTriangle(cul32& ulTriangleId, cu16& usIndex0, cu16& usIndex1, cu16& usIndex2);

	//				void		AddTriangle(cu16& usIndex0, cu16& usIndex1, cu16& usIndex2);

	//		xst_fi	void		ResetTriangleId()
	//							{ m_ulCurrentTriangleId = 0; }

	//		xst_fi	u16			GetIndexSize() const
	//							{ return m_usIndexSize; }

	//	protected:

	//				ul32		_CalcOffset(cul32& ulIndexId);

	//	protected:

	//		TCData< u32 >		m_Data;
	//		//XST::CBinaryData	m_Data;
	//		ul32				m_ulTriangleCount;
	//		ul32				m_ulCurrentTriangleId;
	//		u16					m_usIndexSize; //size of the one index in bytes
	//		ul32				m_ulIndexCount;
	//};

}//xse

#endif