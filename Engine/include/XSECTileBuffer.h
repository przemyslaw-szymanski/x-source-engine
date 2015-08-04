#ifndef XSE_CTILEBUFFER_H
#define XSE_CTILEBUFFER_H

#include "XSECommon.h"
#include "XSECBoundingVolume.h"
#include "XSEIVertexBuffer.h"
#include "XSEIIndexBuffer.h"

namespace XSE  
{
    class XST_API CTileBuffer
    {
        public:

            struct STile
            {
                CBoundingVolume Volume;
                u32 uiStartOffset;
                u32 uiEndOffset;
                u16 usId;
                u16 usFrontBufferId : 8;
                u16 usBackBufferId : 8;
            };

            struct STileInfo
            {
                CPoint  VertexCount;
                Vec2    vecSize;
                Vec2    vecVertexDistance;
            };

            enum class BufferUsage
            {
                STATIC,
                DYNAMIC,
                DYNAMIC_DOUBLEBUFFER
            };

            struct SBufferInfo
            {
                Vec3    vecPosition             = Vec3::ZERO;
                Vec3    vecSize                 = Vec3::ZERO;
                CPoint  TileCount               = CPoint::ZERO;
                CPoint  VertexCount             = CPoint::ZERO;
                CPoint  TmpVertexCount          = CPoint::ZERO;
                const IInputLayout* pIL         = xst_null;
                u32     uiTileRowVertexCount    = 0;
                BufferUsage eUsage              = BufferUsage::STATIC;
                u8      uLODCount               = 0;
            };

            using TileVec = xst_vector< STile >;
            using BoundingVolumeVec = xst_vector< CBoundingVolume >;
            using VertexBufferVec = xst_vector< VertexBufferPtr >;
            using IndexBufferVec = xst_vector< IndexBufferPtr >;
            using TexCoordVec = xst_vector< Vec2Vec >;

        public:

                    CTileBuffer();
            virtual ~CTileBuffer();

            i32 Init(const SBufferInfo& Info, IRenderSystem* pRS);

            i32 Create();

            i32 CalcPositions(const Vec3& vecDirection);
            i32 CalcNormals();
            i32 CalcTexCoords(u32 uiId);
            i32 CreateTiles();
            i32 CreateVertexBuffers();
            i32 CreateIndexBuffers();

            // Calc vertex count for a vertex buffer
            CPoint CalcBufferVertexCount(u32 uiRowVertexCount, const CPoint& Size) const;
            // Calc vertex count for a temporary buffers
            CPoint CalcVertexCount(u32 uiRowVertexCount, const CPoint& Size) const;

            xst_fi Vec3Vec& GetPositionBuffer()
            {
                return m_vPositions;
            }

            xst_fi Vec3Vec& GetNormalBuffer()
            {
                return m_vNormals;
            }

            xst_fi Vec2Vec& GetTexCoordBuffer(u32 uiId)
            {
                return m_vTexCoords[ uiId ];
            }

        protected:

            SBufferInfo         m_Info;
            STileInfo           m_TileInfo;
            TileVec             m_vTiles;
            BoundingVolumeVec   m_vVolumes;
            VertexBufferVec     m_vpVBs;
            IndexBufferVec      m_vpIBs;
            Vec3Vec             m_vPositions;
            Vec3Vec             m_vNormals;
            TexCoordVec         m_vTexCoords;
            IRenderSystem*      m_pRS;
    };
} // xse

#endif // XSE_CTILEBUFFER_H