#include "XSECTileBuffer.h"
#include "XSEIRenderSystem.h"

namespace XSE
{
    CTileBuffer::CTileBuffer()
    {

    }

    CTileBuffer::~CTileBuffer()
    {

    }

    CPoint CTileBuffer::CalcVertexCount(u32 uiRowVertexCount, const CPoint& Size) const
    {
        CPoint Count;
        Count.x = Size.x * uiRowVertexCount - Size.x + 1;
        Count.y = Size.y * uiRowVertexCount - Size.y + 1;
        return Count;
    }

    CPoint CTileBuffer::CalcBufferVertexCount(u32 uiRowVertexCount, const CPoint& Size) const
    {
        ul32 uiTileVertexCount = uiRowVertexCount * uiRowVertexCount;
        return Size * uiTileVertexCount;
    }

    i32 CTileBuffer::Init(const SBufferInfo& Info, IRenderSystem* pRS)
    {
        xst_assert2( pRS );
        m_Info = Info;
        m_pRS = pRS;

        if( m_Info.TileCount <= 0 )
        {
            return XST_FAIL;
        }

        if( m_Info.uiTileRowVertexCount == 0 )
        {
            return XST_FAIL;
        }
        
        m_Info.VertexCount = CalcBufferVertexCount( m_Info.uiTileRowVertexCount, m_Info.TileCount );
        m_Info.TmpVertexCount = CalcVertexCount( m_Info.uiTileRowVertexCount, m_Info.TileCount );
        ul32 uVertexCount = m_Info.VertexCount.ElementMul();
        if( m_vPositions.size() < m_Info.TmpVertexCount.ElementMul() )
        {
            m_vPositions.resize( m_Info.TmpVertexCount.ElementMul(), Vec3::ZERO );
            if( Info.pIL->IsNormal() )
            {
                m_vNormals.resize( m_vPositions.size() );
            }
            
            u32 uTexCoordCount = 0;
            if( Info.pIL->IsTexCoord0() )
            {
                uTexCoordCount++;
            }
            if( Info.pIL->IsTexCoord1() )
            {
                uTexCoordCount++;
            }
            if( Info.pIL->IsTexCoord2() )
            {
                uTexCoordCount++;
            }
            if( Info.pIL->IsTexCoord3() )
            {
                uTexCoordCount++;
            }
            if( Info.pIL->IsTexCoord4() )
            {
                uTexCoordCount++;
            }
            if( Info.pIL->IsTexCoord5() )
            {
                uTexCoordCount++;
            }
            if( Info.pIL->IsTexCoord6() )
            {
                uTexCoordCount++;
            }
            if( Info.pIL->IsTexCoord7() )
            {
                uTexCoordCount++;
            }

            m_vTexCoords.resize( uTexCoordCount );
            if( Info.pIL->IsTexCoord0() )
            {
                m_vTexCoords[ 0 ].resize( m_vPositions.size() );
            }
            if( Info.pIL->IsTexCoord1() )
            {
                m_vTexCoords[ 1 ].resize( m_vPositions.size() );
            }
            if( Info.pIL->IsTexCoord2() )
            {
                m_vTexCoords[ 2 ].resize( m_vPositions.size() );
            }
            if( Info.pIL->IsTexCoord3() )
            {
                m_vTexCoords[ 3 ].resize( m_vPositions.size() );
            }
            if( Info.pIL->IsTexCoord4() )
            {
                m_vTexCoords[ 4 ].resize( m_vPositions.size() );
            }
            if( Info.pIL->IsTexCoord5() )
            {
                m_vTexCoords[ 5 ].resize( m_vPositions.size() );
            }
            if( Info.pIL->IsTexCoord6() )
            {
                m_vTexCoords[ 6 ].resize( m_vPositions.size() );
            }
            if( Info.pIL->IsTexCoord7() )
            {
                m_vTexCoords[ 7 ].resize( m_vPositions.size() );
            }
        }

        return XST_OK;
    }

    i32 CTileBuffer::Create()
    {

        return XST_FAIL;
    }
} // xse