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
        return Size * uiRowVertexCount; // width * vertex count, height * vertex count
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
            cul32 ulSize = m_vPositions.size();
            if( Info.pIL->IsNormal() && m_vNormals.size() < ulSize )
            {
                m_vNormals.resize( ulSize );
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
            if( Info.pIL->IsTexCoord0() && m_vTexCoords[ 0 ].size() < ulSize )
            {
                m_vTexCoords[ 0 ].resize( ulSize );
            }
            if( Info.pIL->IsTexCoord1() && m_vTexCoords[ 1 ].size() < ulSize )
            {
                m_vTexCoords[ 1 ].resize( ulSize );
            }
            if( Info.pIL->IsTexCoord2() && m_vTexCoords[ 2 ].size() < ulSize )
            {
                m_vTexCoords[ 2 ].resize( ulSize );
            }
            if( Info.pIL->IsTexCoord3() && m_vTexCoords[ 3 ].size() < ulSize )
            {
                m_vTexCoords[ 3 ].resize( ulSize );
            }
            if( Info.pIL->IsTexCoord4() && m_vTexCoords[ 4 ].size() < ulSize )
            {
                m_vTexCoords[ 4 ].resize( ulSize );
            }
            if( Info.pIL->IsTexCoord5() && m_vTexCoords[ 5 ].size() < ulSize )
            {
                m_vTexCoords[ 5 ].resize( ulSize );
            }
            if( Info.pIL->IsTexCoord6() && m_vTexCoords[ 6 ].size() < ulSize )
            {
                m_vTexCoords[ 6 ].resize( ulSize );
            }
            if( Info.pIL->IsTexCoord7() && m_vTexCoords[ 7 ].size() < ulSize )
            {
                m_vTexCoords[ 7 ].resize( ulSize );
            }
        }

        return XST_OK;
    }

    i32 CTileBuffer::CalcPositions(const Vec3& vecDirection)
    {
        cul32 ulVertexCount = m_Info.TmpVertexCount.ElementMul();
        cul32 ulSize = m_vPositions.size();
        if( ulSize < ulVertexCount )
        {
            XST_LOG_ERR("Temporary position buffer is too small");
            xst_assert2(m_vPositions.size() >= ulVertexCount);
            return XST_FAIL;
        }
        else if( ulSize == ulVertexCount )
        {
            return XST_OK; // ok as this buffer is set already
        }

        const CPoint& VertexCount = m_Info.TmpVertexCount;
        const Vec2 vecDistance( m_Info.vecSize.x / VertexCount.x, m_Info.vecSize.y / VertexCount.y );
        Vec3 vecCurrPos = Vec3::ZERO;

        for( ul32 y = 0; y < VertexCount.y; ++y )
        {
            vecCurrPos.y = y * vecDistance.y;
            for( ul32 x = 0; x < VertexCount.x; ++x )
            {
                vecCurrPos = x * vecDistance.x;
                // Cache miss here, it is not possible to prefetch elements of this array
                // because it is 'random' access
                m_vPositions[ XST_ARRAY_2D_TO_1D( x, y, VertexCount.x ) ] = vecCurrPos;
            }
        }
        return XST_OK;
    }

    i32 CTileBuffer::CreateTiles()
    {
        cul32 ulSize = m_vTiles.size();
        cul32 uTileCount = m_Info.TileCount.ElementMul();
        if( ulSize == uTileCount )
        {
            return XST_OK;
        }

        m_vTiles.resize( uTileCount );

        return XST_OK;
    }

    i32 CTileBuffer::CreateVertexBuffers()
    {
        xst_assert2( m_pRS );
        XST_RET_FAIL(CalcPositions(Vec3::Z));
        const auto& TileCount = m_Info.TileCount;
        const auto& TmpVertexCount = m_Info.TmpVertexCount;
        cu32 uTileRowVertexCount = m_Info.uiTileRowVertexCount;
        
        // Create vertex buffers
        u16 bDoubleBuffered = m_Info.eUsage == BufferUsages::DYNAMIC_DOUBLEBUFFER ? 1 : 0;
        cul32 uVertexCount = m_Info.VertexCount.ElementMul();
        cul32 ulVertexBufferCount = std::ceilf( (f32)uVertexCount / m_pRS->GetLimits().ulMaxVertexBufferSize );
        if( ulVertexBufferCount > 2 )
        {
            XST_LOG_ERR("Vertex buffer too large");
            return XST_FAIL;
        }
        
        cu16 uFrontBufferId = 0;
        cu16 uBackBufferId = ulVertexBufferCount > 1 ? 1 : 0;
        cu64 uFrontBufferOffset = 0;
        cu64 uBackBufferOffset = uVertexCount * !uBackBufferId; // uVertexCount if one buffer or 0 if a new one

        m_vpVBs.resize( ulVertexBufferCount );

        for( auto& pVB : m_vpVBs )
        {
            pVB = m_pRS->CreateVertexBuffer();
            if( pVB.IsValid )
            {
                pVB->SetInputLayout( m_Info.pIL );
                pVB->SetTopologyType( XSE::TopologyTypes::TRIANGLE_LIST );
                pVB->SetUsage( m_Info.eUsage );
                pVB->SetVertexCount( uVertexCount );
                XST_RET_FAIL( pVB->Lock() );
            }
            else
            {
                return XST_FAIL;
            }
        }

        IVertexBuffer* pFrontVB = m_vpVBs[ uFrontBufferId ].GetPtr();
        IVertexBuffer* pBackVB = m_vpVBs[ uBackBufferId ].GetPtr();
        auto& FrontVData = pFrontVB->GetVertexData();
        auto& BackVData = pBackVB->GetVertexData();

        CPoint VertexPosInBuffer = CPoint::ZERO;
        ul32 uCurrVertexPosInBuffer;

        for( i32 iTileY = 0; iTileY < TileCount.y; ++iTileY )
        {
            for( i32 iTileX = 0; iTileX < TileCount.x; ++iTileX )
            {
                for( u32 uVertexY = 0; uVertexY < uTileRowVertexCount; ++uVertexY )
                {
                    VertexPosInBuffer.y = iTileY * (uTileRowVertexCount - 1) + uVertexY;
                    for( u32 uVertexX = 0; uVertexX < uTileRowVertexCount; ++uVertexX )
                    {
                        VertexPosInBuffer.x = iTileX * (uTileRowVertexCount - 1) + uVertexX; // -1 because last vertex is shared between tiles
                        uCurrVertexPosInBuffer = XST_ARRAY_2D_TO_1D( VertexPosInBuffer.x, VertexPosInBuffer.y, TmpVertexCount.width );
                        const auto& vecPos = m_vPositions[ uCurrVertexPosInBuffer ];
                        // Set both to front and back buffer

                        /*char buff[ 20 ]; xst_sprintf( buff, 20, "%d, %d", VertexPosInBuffer.x, VertexPosInBuffer.y );
                        XST::CDebug::PrintDebugLN(buff);*/
                    }
                }
            }
        }

        for( auto& pVB : m_vpVBs )
        {
            XST_RET_FAIL( pVB->Unlock() );
        }

        return XST_OK;
    }

    i32 CTileBuffer::CreateIndexBuffers()
    {
        return XST_OK;
    }

    i32 CTileBuffer::Create()
    {
        XST_RET_FAIL( CreateVertexBuffers() );
        XST_RET_FAIL( CreateIndexBuffers() );

        return XST_FAIL;
    }
} // xse