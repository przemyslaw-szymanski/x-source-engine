#ifndef XSE_CD3D11INDEX_BUFFER_H
#define XSE_CD3D11INDEX_BUFFER_H

#include "XSEIIndexBuffer.h"
#include "XSED3D11Common.h"

#if defined (XST_WINDOWS)
namespace XSE
{
	namespace D3D11
	{
		class CIndexBuffer : public IIndexBuffer
		{
			friend class CRenderSystem;
			
			public:

				static const DXGI_FORMAT	D3D_INDEX_FORMAT = DXGI_FORMAT_R16_UINT;

			public:

										CIndexBuffer(CRenderSystem* pRS);
				virtual					~CIndexBuffer();

						i32				Lock();
						
						i32				Unlock(bool bCreate);

						void			SetIndexSize(INDEX_ELEMENT_SIZE eSize) xst_implement;
						
						void			SetUsage(BUFFER_USAGE eUsage);
						
						BUFFER_USAGE	GetUsage()
										{ return m_eUsage; }
						
						i32				Create();
						
						/*u16*			AddIndices(cul32& ulIndexCount);
						
						u16*			AddIndex();*/

						ul32			GetSize() const
										{ return m_IndexData.GetSize(); }
						
						i32				SetIndexCount(ul32 ulIndexCount);
						
						ul32			GetIndexCount() const
										{ return m_IndexData.GetIndexCount(); }

						void			SetLocationType(BUFFER_LOCATION_TYPE eLocType)
										{ m_eBufferLocationType = eLocType; }

						void			Destroy();

						BUFFER_LOCATION_TYPE	GetLocationType()
												{ return m_eBufferLocationType; }

						bool				IsTriangleStripDegenerationSupported() const
											{ return true; }
			
						void				DegenerateTriangleStrip(cul32& ulIndex)
											{ m_IndexData.SetIndex( ulIndex, 0xffffffff ); }
			
						void				DegenerateTriangleStrip(cu16& usIndex)
											{ m_IndexData.SetIndex( usIndex, 0xffff ); }

			protected:

				//TCData< u16 >			m_aIndexBuffer;
				ID3D11Buffer*			m_pD3DBuffer;
				D3D11_SUBRESOURCE_DATA	m_D3DInitData;
				D3D11_USAGE				m_eD3DBufferUsage;
				DXGI_FORMAT				m_eD3DDataFormat;
				BUFFER_USAGE			m_eUsage;
				ul32					m_ulIndexCount;
				CRenderSystem*			m_pRS;
				BUFFER_LOCATION_TYPE	m_eBufferLocationType;
				D3D11_BUFFER_DESC		m_D3DBufferDesc;
				u32						m_uiIndexSize;
		};

	}//d3d11
}//xse
#endif

#endif //INDEX_BUFFER