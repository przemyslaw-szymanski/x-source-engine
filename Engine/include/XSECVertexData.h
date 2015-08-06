#ifndef XSE_CVERTEX_DATA_H
#define XSE_CVERTEX_DATA_H

#include "XSERenderSystemCommon.h"
#include "XST/XSTCBinaryData.h"
#include "XSEIInputLayout.h"
#include "XSECFixedVertexData.h"

namespace XSE
{
    //Vertex data is a buffer of bytes contains all vertex informations like position, normal, binormal, texcoords etc
    //Those informations are set in a raw byte buffer which is a graphics api friendly format
    //Engine does not use any vertex structure because each 3d model can has different vertex attributes (only what attributes are necessary to save memory space)
    class XST_API CVertexData
    {
        friend class CFixedVertexData;
        protected:

            typedef ul32 (CVertexData::*pfnCheck)(cul32& ulVertexId, IInputLayout* pIL, cul32& ulAttributeId);

        public:

            struct BufferTypes
            {
                enum TYPE
                {
                    FRONT = 1,
                    BACK = 2
                };
            };

        public:
                                    CVertexData();
            virtual					~CVertexData();

                    i32				Create(ul32 ulVertexCount, const IInputLayout* pIL, bool bDoubleBuffer);

                    i32				Clone(CVertexData* pData);
                    i32				Clone(CFixedVertexData* pData);

            xst_fi	
            const IInputLayout*		GetInputLayout() const
                                    { return m_pInputLayout; }

                    u8*				GetData()
                                    { return m_Data.GetData(); }

                    cu8*			GetData() const
                                    { return m_Data.GetData(); }

                    i32				SetData(const CVertexData& SrcData);

                    i32				SetData(cu8* const pData, ul32 ulDataSize);

            xst_fi	
            XST::CBinaryData&		GetBinaryData()
                                    { return m_Data; }

            xst_fi	
            const XST::CBinaryData&	GetBinaryData() const
                                    { return m_Data; }

                    ul32			GetVertexCount()
                                    { return m_ulVertexCount; }

                    cul32			GetVertexCount() const
                                    { return m_ulVertexCount; }

            xst_fi	ul32			GetSize()
                                    { return m_ulBufferSize; }

            xst_fi	cul32			GetSize() const
                                    { return m_ulBufferSize; }

                    i32				GetAttribute(cul32& ulVertexId, cul32& ulAttributeId, Vec2* pVec);
                    i32				GetAttribute(cul32& ulVertexId, cul32& ulAttributeId, Vec3* pVec);
                    i32				GetAttribute(cul32& ulVertexId, cul32& ulAttributeId, Vec4* pVec);

            xst_fi	i32				GetPosition(cul32& ulVertexId, Vec3* pVec)
                                    { return GetAttribute( ulVertexId, ILEIds::POSITION, pVec ); }

            xst_fi	i32				GetNormal(cul32& ulVertexId, Vec3* pVec)
                                    { return GetAttribute( ulVertexId, ILEIds::NORMAL, pVec ); }

            xst_fi	i32				GetBinormal(cul32& ulVertexId, Vec3* pVec)
                                    { return GetAttribute( ulVertexId, ILEIds::BINORMAL, pVec ); }

            xst_fi	i32				GetTexCoord0(cul32& ulVertexId, Vec2* pVec)
                                    { return GetAttribute( ulVertexId, ILEIds::TEXCOORD0, pVec ); }

            xst_fi	i32				GetTangent(cul32& ulVertexId, Vec3* pVec)
                                    { return GetAttribute( ulVertexId, ILEIds::TANGENT, pVec ); }

            xst_fi	i32				GetTexCoord(cul32& ulVertexId, Vec2* pVec, cu32& uiTexCoordId)
                                    { return GetAttribute( ulVertexId, uiTexCoordId, pVec ); }

            xst_fi	i32				GetColor(cul32& ulVertexId, Vec4* pVec)
                                    { return GetAttribute( ulVertexId, ILEIds::COLOR, pVec ); }
            
                    i32				SetAttribute(cul32& ulVertexId, cul32& ulAttributeId, const Vec2& Vec);
                    i32				SetAttribute(cul32& ulVertexId, cul32& ulAttributeId, const Vec3& Vec);
                    i32				SetAttribute(cul32& ulVertexId, cul32& ulAttributeId, const Vec4& Vec);

                    i32				SetAttribute(cul32& ulVertexId, cul32& ulAttributeId, cf32& fX, cf32& fY);
                    i32				SetAttribute(cul32& ulVertexId, cul32& ulAttributeId, cf32& fX, cf32& fY, cf32& fZ);
                    i32				SetAttribute(cul32& ulVertexId, cul32& ulAttributeId, cf32& fX, cf32& fY, cf32& fZ, cf32& fW);

            xst_fi	i32				SetPosition(cul32& ulVertexId, const Vec3& Vec)
                                    { return SetAttribute( ulVertexId, ILEIds::POSITION, Vec ); }

            xst_fi	i32				SetNormal(cul32& ulVertexId, const Vec3& Vec)
                                    { return SetAttribute( ulVertexId, ILEIds::NORMAL, Vec ); }

            xst_fi	i32				SetBinormal(cul32& ulVertexId, const Vec3& Vec)
                                    { return SetAttribute( ulVertexId, ILEIds::BINORMAL, Vec ); }

            xst_fi	i32				SetTangent(cul32& ulVertexId, const Vec3& Vec)
                                    { return SetAttribute( ulVertexId, ILEIds::TANGENT, Vec ); }

            xst_fi	i32				SetTexCoord(cul32& ulVertexId, cu32& uiTexCoordId, const Vec2& Vec)
                                    { return SetAttribute( ulVertexId, uiTexCoordId, Vec ); }

            xst_fi	i32				SetTexCoord0(cul32& ulVertexId, const Vec2& Vec)
                                    { return SetAttribute( ulVertexId, ILEIds::TEXCOORD0, Vec ); }

            xst_fi	i32				SetColor(cul32& ulVertexId, const Vec4& Vec)
                                    { return SetAttribute( ulVertexId, ILEIds::COLOR, Vec ); }

            xst_fi	i32				SetPosition(cul32& ulVertexId, cf32& fX, cf32& fY, cf32& fZ)
                                    { return SetAttribute( ulVertexId, ILEIds::POSITION, fX, fY, fZ ); }

            xst_fi	i32				SetNormal(cul32& ulVertexId, cf32& fX, cf32& fY, cf32& fZ)
                                    { return SetAttribute( ulVertexId, ILEIds::NORMAL, fX, fY, fZ ); }

            xst_fi	i32				SetBinormal(cul32& ulVertexId, cf32& fX, cf32& fY, cf32& fZ)
                                    { return SetAttribute( ulVertexId, ILEIds::BINORMAL, fX, fY, fZ ); }

            xst_fi	i32				SetTangent(cul32& ulVertexId, cf32& fX, cf32& fY, cf32& fZ)
                                    { return SetAttribute( ulVertexId, ILEIds::TANGENT, fX, fY, fZ ); }

            xst_fi	i32				SetTexCoord(cul32& ulVertexId, cu32& uiTexCoordId, cf32& fX, cf32& fY)
                                    { return SetAttribute( ulVertexId, uiTexCoordId, fX, fY ); }

            xst_fi	i32				SetColor(cul32& ulVertexId, cf32& fR, cf32& fG, cf32& fB, cf32& fA)
                                    { return SetAttribute( ulVertexId, ILEIds::COLOR, fR, fG, fB, fA ); }

            // Double buffering

                    i32				GetAttribute(BufferTypes::TYPE eBuffer, cul32& ulVertexId, cul32& ulAttributeId, Vec2* pVec);
                    i32				GetAttribute(BufferTypes::TYPE eBuffer, cul32& ulVertexId, cul32& ulAttributeId, Vec3* pVec);
                    i32				GetAttribute(BufferTypes::TYPE eBuffer, cul32& ulVertexId, cul32& ulAttributeId, Vec4* pVec);

            xst_fi	i32				GetPosition(BufferTypes::TYPE eBuffer, cul32& ulVertexId, Vec3* pVec)
                                    { return GetAttribute( ulVertexId, ILEIds::POSITION, pVec ); }

            xst_fi	i32				GetNormal(BufferTypes::TYPE eBuffer, cul32& ulVertexId, Vec3* pVec)
                                    { return GetAttribute( ulVertexId, ILEIds::NORMAL, pVec ); }

            xst_fi	i32				GetBinormal(BufferTypes::TYPE eBuffer, cul32& ulVertexId, Vec3* pVec)
                                    { return GetAttribute( ulVertexId, ILEIds::BINORMAL, pVec ); }

            xst_fi	i32				GetTexCoord0(BufferTypes::TYPE eBuffer, cul32& ulVertexId, Vec2* pVec)
                                    { return GetAttribute( ulVertexId, ILEIds::TEXCOORD0, pVec ); }

            xst_fi	i32				GetTangent(BufferTypes::TYPE eBuffer, cul32& ulVertexId, Vec3* pVec)
                                    { return GetAttribute( ulVertexId, ILEIds::TANGENT, pVec ); }

            xst_fi	i32				GetTexCoord(BufferTypes::TYPE eBuffer, cul32& ulVertexId, Vec2* pVec, cu32& uiTexCoordId)
                                    { return GetAttribute( ulVertexId, uiTexCoordId, pVec ); }

            xst_fi	i32				GetColor(BufferTypes::TYPE eBuffer, cul32& ulVertexId, Vec4* pVec)
                                    { return GetAttribute( ulVertexId, ILEIds::COLOR, pVec ); }
            
                    i32				SetAttribute(BufferTypes::TYPE eBuffer, cul32& ulVertexId, cul32& ulAttributeId, const Vec2& Vec);
                    i32				SetAttribute(BufferTypes::TYPE eBuffer, cul32& ulVertexId, cul32& ulAttributeId, const Vec3& Vec);
                    i32				SetAttribute(BufferTypes::TYPE eBuffer, cul32& ulVertexId, cul32& ulAttributeId, const Vec4& Vec);

                    i32				SetAttribute(BufferTypes::TYPE eBuffer, cul32& ulVertexId, cul32& ulAttributeId, cf32& fX, cf32& fY);
                    i32				SetAttribute(BufferTypes::TYPE eBuffer, cul32& ulVertexId, cul32& ulAttributeId, cf32& fX, cf32& fY, cf32& fZ);
                    i32				SetAttribute(BufferTypes::TYPE eBuffer, cul32& ulVertexId, cul32& ulAttributeId, cf32& fX, cf32& fY, cf32& fZ, cf32& fW);

            xst_fi	i32				SetPosition(BufferTypes::TYPE eBuffer, cul32& ulVertexId, const Vec3& Vec)
                                    { return SetAttribute( ulVertexId, ILEIds::POSITION, Vec ); }

            xst_fi	i32				SetNormal(BufferTypes::TYPE eBuffer, cul32& ulVertexId, const Vec3& Vec)
                                    { return SetAttribute( ulVertexId, ILEIds::NORMAL, Vec ); }

            xst_fi	i32				SetBinormal(BufferTypes::TYPE eBuffer, cul32& ulVertexId, const Vec3& Vec)
                                    { return SetAttribute( ulVertexId, ILEIds::BINORMAL, Vec ); }

            xst_fi	i32				SetTangent(BufferTypes::TYPE eBuffer, cul32& ulVertexId, const Vec3& Vec)
                                    { return SetAttribute( ulVertexId, ILEIds::TANGENT, Vec ); }

            xst_fi	i32				SetTexCoord(BufferTypes::TYPE eBuffer, cul32& ulVertexId, cu32& uiTexCoordId, const Vec2& Vec)
                                    { return SetAttribute( ulVertexId, uiTexCoordId, Vec ); }

            xst_fi	i32				SetTexCoord0(BufferTypes::TYPE eBuffer, cul32& ulVertexId, const Vec2& Vec)
                                    { return SetAttribute( ulVertexId, ILEIds::TEXCOORD0, Vec ); }

            xst_fi	i32				SetColor(BufferTypes::TYPE eBuffer, cul32& ulVertexId, const Vec4& Vec)
                                    { return SetAttribute( ulVertexId, ILEIds::COLOR, Vec ); }

            xst_fi	i32				SetPosition(BufferTypes::TYPE eBuffer, cul32& ulVertexId, cf32& fX, cf32& fY, cf32& fZ)
                                    { return SetAttribute( ulVertexId, ILEIds::POSITION, fX, fY, fZ ); }

            xst_fi	i32				SetNormal(BufferTypes::TYPE eBuffer, cul32& ulVertexId, cf32& fX, cf32& fY, cf32& fZ)
                                    { return SetAttribute( ulVertexId, ILEIds::NORMAL, fX, fY, fZ ); }

            xst_fi	i32				SetBinormal(BufferTypes::TYPE eBuffer, cul32& ulVertexId, cf32& fX, cf32& fY, cf32& fZ)
                                    { return SetAttribute( ulVertexId, ILEIds::BINORMAL, fX, fY, fZ ); }

            xst_fi	i32				SetTangent(BufferTypes::TYPE eBuffer, cul32& ulVertexId, cf32& fX, cf32& fY, cf32& fZ)
                                    { return SetAttribute( ulVertexId, ILEIds::TANGENT, fX, fY, fZ ); }

            xst_fi	i32				SetTexCoord(BufferTypes::TYPE eBuffer, cul32& ulVertexId, cu32& uiTexCoordId, cf32& fX, cf32& fY)
                                    { return SetAttribute( ulVertexId, uiTexCoordId, fX, fY ); }

            xst_fi	i32				SetColor(BufferTypes::TYPE eBuffer, cul32& ulVertexId, cf32& fR, cf32& fG, cf32& fB, cf32& fA)
                                    { return SetAttribute( ulVertexId, ILEIds::COLOR, fR, fG, fB, fA ); }

        protected:

                    //Calculates byte offset for specific vertex index, input layout and attribute like: POSITION, NORMAL, etc
                    ul32			_CalcOffset(BufferTypes::TYPE eBuffer, cul32& ulVertexId, const IInputLayout* pIL, cul32& ulAttributeId) const;

        protected:

            //Atribute mapping: attribute id -> attribute id in m_Data
            u32					m_auiAttributeMap[ XSE::InputLayoutElements::_ENUM_COUNT ];
            //Vertex data buffer
            XST::CBinaryData	m_Data;
            //Data pointer
            u8*					m_pData;
            //Input layout for this vertex data - vertex attributes
            const IInputLayout*		m_pInputLayout;
            //Vertex data buffer size
            ul32				m_ulBufferSize;
            //Number of vertices
            ul32				m_ulVertexCount;
            //Vertex size in bytes
            u32				    m_ulVertexSize : 31;
            // Is double buffer
            u32                 m_bIsDoubleBuffer : 1;
    };

    void DebugPrintVertexData(CVertexData& Data);

}//xse

#endif