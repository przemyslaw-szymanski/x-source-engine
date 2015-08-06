#include "XSECD3D11VertexBuffer.h"
#include "XSECD3D11RenderSystem.h"
#include "XSECD3D11InputLayout.h"

#if defined (XST_WINDOWS)
namespace XSE
{
    namespace D3D11
    {
        //void WriteVerticesToByteBuffer(u8* pVertexData, ul32 ulDataSize, const Vertex* aVertices, ul32 ulVertexCount, IInputLayout* pIL);
        ul32 CalcVertexSize(IInputLayout* pIL);

        CVertexBuffer::CVertexBuffer(CRenderSystem* pRS, XST::IAllocator* pAlloc) :	
            m_pRS( pRS ), 
            m_eD3DTopologyType( D3D_PRIMITIVE_TOPOLOGY_UNDEFINED ), 
            m_eD3DBufferUsage( D3D11_USAGE_DEFAULT ),
            m_eTopologyType( (XSE::TOPOLOGY_TYPE)0 ),
            m_eUsage( (XSE::BUFFER_USAGE)0 ),
            m_pD3DBuffer( xst_null ), 
            m_pInputLayout( xst_null ), 
            m_ulVertexCount( 0 )
        {
        }

        CVertexBuffer::~CVertexBuffer()
        {
            m_pRS->_DestroyVertexBuffer( this );
            //xst_release( m_pD3DBuffer );
            m_pRS = xst_null;
            m_pInputLayout = xst_null;
        }

        i32 CVertexBuffer::Lock()
        {
            cul32 ulVertexCount = m_ulVertexCount * m_eUsage == BufferUsages::DYNAMIC_DOUBLEBUFFER ? 2 : 1;
            if( XST_FAILED( m_VertexData.Create( ulVertexCount, m_pInputLayout ) ) )
            {
                XST_LOG_ERR( "Failed to create vertex buffer. Memory error." );
                return XST_FAIL;
            }

            return XST_OK;
        }

        i32 CVertexBuffer::Unlock(bool bCreate)
        {
            if( bCreate )
            {
                return Create();
            }
            return XST_OK;
        }

        void CVertexBuffer::Update(cu32& uiStartVertex, cu32& uiVertexCount)
        {
            m_pRS->UpdateVertexBuffer( this, uiStartVertex, uiVertexCount, this->m_VertexData );
        }

        i32 CVertexBuffer::SetVertexCount(ul32 ulCount)
        {
            /*m_aVertices.Delete();
            if( !m_aVertices.Create( ulCount ) )
            {
                XST_LOG_ERR( "Failed to create vertex buffer. Memory error." );
                return XST_FAIL;
            }*/
            
            m_ulVertexCount = ulCount;
            return XST_OK;
        }

        void CVertexBuffer::SetTopologyType(XSE::TOPOLOGY_TYPE eType)
        {
            switch( eType )
            {
                case TopologyTypes::UNDEFINED: m_eD3DTopologyType = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED; break;
                case TopologyTypes::LINE_LIST: m_eD3DTopologyType = D3D11_PRIMITIVE_TOPOLOGY_LINELIST; break;
                case TopologyTypes::LINE_STRIP: m_eD3DTopologyType = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP; break;
                case TopologyTypes::POINT_LIST: m_eD3DTopologyType = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST; break;
                case TopologyTypes::TRIANGLE_LIST: m_eD3DTopologyType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST; break;
                case TopologyTypes::TRIANGLE_STRIP: m_eD3DTopologyType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP; break;
                default: m_eD3DTopologyType = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED; break;
            }

            m_eTopologyType = eType;
        }

        XSE::TOPOLOGY_TYPE CVertexBuffer::GetTopologyType()
        {
            return m_eTopologyType;
        }

        void CVertexBuffer::SetUsage(XSE::BUFFER_USAGE eUsage)
        {
            switch( eUsage )
            {
                case BufferUsages::DEFAULT: m_eD3DBufferUsage = D3D11_USAGE_DEFAULT; break;
                case BufferUsages::DYNAMIC: m_eD3DBufferUsage = D3D11_USAGE_DYNAMIC; break;
                case BufferUsages::STATIC: m_eD3DBufferUsage = D3D11_USAGE_IMMUTABLE; break;
                case BufferUsages::DYNAMIC_DOUBLEBUFFER: m_eD3DBufferUsage = D3D11_USAGE_DYNAMIC; break;
                default: m_eD3DBufferUsage = D3D11_USAGE_DEFAULT; break;
            }

            m_eUsage = eUsage;
        }

        XSE::BUFFER_USAGE CVertexBuffer::GetUsage()
        {
            return m_eUsage;
        }

        i32	CVertexBuffer::Create()
        {
            if( m_pD3DBuffer != xst_null )
            {
                return XST_OK;
            }

            ul32 ulBuffSize = m_VertexData.GetSize();
            u8* pBuff = m_VertexData.GetData();

            xst_zero( &m_D3DBufferDesc, sizeof( m_D3DBufferDesc ) );
            m_D3DBufferDesc.Usage = m_eD3DBufferUsage;
            m_D3DBufferDesc.ByteWidth = ulBuffSize;
            m_D3DBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            m_D3DBufferDesc.CPUAccessFlags = 0;
            if( m_eUsage == BufferUsages::DYNAMIC )
            {
                m_D3DBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            }

            
            xst_zero( &m_D3DInitData, sizeof( m_D3DInitData ) );
            m_D3DInitData.pSysMem = pBuff;//m_VertexData.GetData();

            return m_pRS->_CreateVertexBuffer( this );
        }
        
        void CVertexBuffer::SetInputLayout(ul32 uElements)
        {
            m_pInputLayout = (CInputLayout*)m_pRS->GetInputLayout( uElements );
        }

        void CVertexBuffer::SetInputLayout(const IInputLayout* pInputLayout)
        {
            m_pInputLayout = (CInputLayout*)pInputLayout;
        }


        ul32 CalcVertexSize(IInputLayout* pIL)
        {
            xst_assert( pIL, "(CalcBufferSize) InputLayout is not set" );
            ul32 ulSize = sizeof( Vec3 ); //position

            if( pIL->IsNormal() )
            {
                ulSize += pIL->GetNormalSize();
            }

            if( pIL->IsTexCoord0() )
            {
                ulSize += pIL->GetTexCoordSize();
            }

            if( pIL->IsTexCoord1() )
            {
                ulSize += pIL->GetTexCoordSize();
            }

            if( pIL->IsTexCoord2() )
            {
                ulSize += pIL->GetTexCoordSize();
            }

            if( pIL->IsTexCoord3() )
            {
                ulSize += pIL->GetTexCoordSize();
            }

            if( pIL->IsTexCoord4() )
            {
                ulSize += pIL->GetTexCoordSize();
            }

            if( pIL->IsTexCoord5() )
            {
                ulSize += pIL->GetTexCoordSize();
            }

            if( pIL->IsTexCoord6() )
            {
                ulSize += pIL->GetTexCoordSize();
            }

            if( pIL->IsTexCoord7() )
            {
                ulSize += pIL->GetTexCoordSize();
            }

            if( pIL->IsBinormal() )
            {
                ulSize += pIL->GetBinormalSize();
            }

            if( pIL->IsTangent() )
            {
                ulSize += pIL->GetTangentSize();
            }

            if( pIL->IsColor() )
            {
                ulSize += pIL->GetColorSize();
            }

            if( pIL->IsSpecular() )
            {
                ulSize += pIL->GetSpecularSize();
            }

            if( pIL->IsDiffuse() )
            {
                ulSize += pIL->GetDiffuseSize();
            }

            return ulSize;
        }

        //void WriteVerticesToByteBuffer(u8* pVertexData, ul32 ulDataSize, const Vertex* aVertices, ul32 ulVertexCount, IInputLayout* pIL)
        //{
        //	u8* pCurrVertex = pVertexData;
        //	ul32 ulCurrSize = 0;

        //	for(ul32 i = 0; i < ulVertexCount; ++i)
        //	{
        //		//Write position
        //		xst_memcpy( pCurrVertex, sizeof( Vec3 ), &aVertices[ i ].vecPosition, sizeof( Vec3 ) );
        //		pCurrVertex += sizeof( Vec3 );
        //	}
        //}


}//d3d11
}//xse
#endif //XST_WINDOWS