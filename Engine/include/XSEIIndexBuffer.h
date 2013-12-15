#ifndef XSE_IINDEX_BUFFER_H
#define XSE_IINDEX_BUFFER_H

#include "XSECIndexData.h"
#include "XSERenderSystemCommon.h"

namespace XSE
{
	class IIndexBuffer : public XST::IObject
	{
		public:

			virtual						~IIndexBuffer() {}

			virtual	i32					Lock() = 0;
			virtual i32					Unlock(bool bCreate = true) = 0;
			virtual void				SetUsage(BUFFER_USAGE eUsage) = 0;
			virtual BUFFER_USAGE		GetUsage() = 0;
			virtual i32					Create() = 0;
			virtual void				SetIndexSize(cu32& uiSizeInBytes) = 0;
			virtual i32					SetIndexCount(ul32 ulIndexCount) = 0;
			virtual ul32				GetIndexCount() const = 0;
			virtual void				SetLocationType(BUFFER_LOCATION_TYPE eLocType) = 0;
			virtual BUFFER_LOCATION_TYPE	GetLocationType() = 0;	
			virtual void				Destroy() = 0;
			virtual bool				IsTriangleStripDegenerationSupported() const = 0;
			virtual void				DegenerateTriangleStrip(cul32& ulIndex) = 0;
			virtual void				DegenerateTriangleStrip(cu16& usIndex) = 0;
			virtual ul32				GetSize() const = 0;

			virtual	CIndexData&			GetIndexData()
										{ return m_IndexData; }

			virtual const CIndexData&	GetIndexData() const
										{ return m_IndexData; }

		protected:

			CIndexData	m_IndexData;
	};

	typedef XST::TCObjectSmartPointer< IIndexBuffer >	IndexBufferPtr;
	XST_TEMPLATE_CLASS XST::TCObjectSmartPointer< IIndexBuffer >;

	static void DebugPrintIndexData(CIndexData& Data)
	{
		Vec3 vecPos;
		ul32 ulVertexSize = Data.GetSize();
		XST::CDebug::PrintDebugLN( XST::StringUtil::ToString( ulVertexSize ) );

		for(u32 i = 0; i < Data.GetIndexCount(); i += 3)
		{
			Vec3 v( (f32)Data.GetIndex( i ), (f32)Data.GetIndex( i + 1 ), (f32)Data.GetIndex( i + 2 ) );
			XST::CDebug::PrintDebugLN( XST::StringUtil::ToString< Vec3 >( v ) );
		}
	}

}//xse

#endif