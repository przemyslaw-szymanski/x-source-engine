#include "XSECD3D11IndexBuffer.h"
#include "XSECD3D11RenderSystem.h"

#if defined (XST_WINDOWS)
namespace XSE
{
	namespace D3D11
	{
		CIndexBuffer::CIndexBuffer(CRenderSystem* pRS) :
			m_eUsage( BufferUsages::DEFAULT ), 
			m_pD3DIndexBuffer( xst_null ), 
			m_eD3DBufferUsage( D3D11_USAGE_DEFAULT ), 
			m_ulIndexCount( 0 ), 
			m_pRS( pRS ), 
			m_eBufferLocationType( BufferLocationTypes::HARDWARE ),
			m_eD3DDataFormat( DXGI_FORMAT_R16_UINT ),
			m_uiIndexSize( sizeof( u16 ) )
		{

		}

		CIndexBuffer::~CIndexBuffer()
		{
			Destroy();
		}

		void CIndexBuffer::Destroy()
		{
			m_IndexData.Delete();
			xst_release( m_pD3DIndexBuffer );
			m_pRS = xst_null;
		}

		void CIndexBuffer::SetUsage(BUFFER_USAGE eUsage)
		{
			m_eUsage = eUsage;
			switch( eUsage )
			{
				case BufferUsages::DEFAULT: m_eD3DBufferUsage = D3D11_USAGE_DEFAULT; break;
				case BufferUsages::DYNAMIC: m_eD3DBufferUsage = D3D11_USAGE_DYNAMIC; break;
				case BufferUsages::STATIC: m_eD3DBufferUsage = D3D11_USAGE_IMMUTABLE; break;
				default: m_eD3DBufferUsage = D3D11_USAGE_DEFAULT; m_eUsage = BufferUsages::DEFAULT; break;
			}
		}

		i32 CIndexBuffer::Lock()
		{
			return this->m_IndexData.Create( m_ulIndexCount, m_uiIndexSize );
		}

		i32 CIndexBuffer::Unlock(bool bCreate)
		{
			m_ulIndexCount = m_IndexData.GetIndexCount();
			
			if( bCreate )
			{
				return Create();
			}

			return XST_OK;
		}

		void CIndexBuffer::SetIndexSize(cu32& uiSize)
		{
			if( uiSize == sizeof( u32 ) || uiSize == sizeof( u16 ) )
				m_uiIndexSize = uiSize;
			else
				m_uiIndexSize = sizeof( u16 );
		}

		i32 CIndexBuffer::Create()
		{
			if( m_pD3DIndexBuffer != xst_null )
			{
				return XST_OK;
			}

			ul32 ulDataSize = m_IndexData.GetSize();
			//TODO: check whether ulDataSize == 0 and log it with warning. There is an D3D error is no indices are set
			const void* pData = m_IndexData.GetData();

			/*for(u32 i = 0; i < m_IndexData.GetIndexCount(); ++i)
			{
				XST::CDebug::PrintDebugLN( XST::StringUtil::ToString( m_IndexData.GetIndex( i ) ) );
			}*/

			//Set the data format based on data size from the buffer
			m_eD3DDataFormat = DXGI_FORMAT_R16_UINT;
			if( m_IndexData.GetIndexSize() == sizeof( ul32 ) )
			{
				m_eD3DDataFormat = DXGI_FORMAT_R32_UINT;
			}

			xst_zero( &m_D3DBufferDesc, sizeof( m_D3DBufferDesc ) );
			m_D3DBufferDesc.Usage = m_eD3DBufferUsage;
			m_D3DBufferDesc.ByteWidth = ulDataSize;        
			m_D3DBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			m_D3DBufferDesc.CPUAccessFlags = 0;
			if( m_eUsage == BufferUsages::DYNAMIC )
			{
				m_D3DBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			}

			xst_zero( &m_D3DInitData, sizeof( m_D3DInitData ) );
			m_D3DInitData.pSysMem = pData;
			
			if( XST_FAILED( m_pRS->_CreateIndexBuffer( this ) ) )
			{
				return XST_FAIL;
			}

			//If only hardware delete buffer to save memory
			if( m_eBufferLocationType == BufferLocationTypes::HARDWARE )
			{
				//m_aIndexBuffer.Delete();
			}

			return XST_OK;
		}

		/*u16* CIndexBuffer::AddIndices(cul32 &ulIndexCount)
		{
			return m_aIndexBuffer.AddElements( ulIndexCount );
		}

		u16* CIndexBuffer::AddIndex()
		{
			return m_aIndexBuffer.AddElement();
		}*/

		i32 CIndexBuffer::SetIndexCount(ul32 ulIndexCount)
		{
			m_ulIndexCount = ulIndexCount;
		
			return XST_OK;
		}

	}//d3d11
}//xse
#endif