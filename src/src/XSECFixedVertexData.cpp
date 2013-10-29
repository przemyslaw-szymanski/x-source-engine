#include "XSECFixedVertexData.h"
#include "XSEIInputLayout.h"
#include "XSERenderSystemCommon.h"

namespace XSE
{
	void GetAvailableAttributes(u32* auiAttributeMap, cu32& uiMapSize, IInputLayout* pIL);

	CFixedVertexData::CFixedVertexData() : m_ulBufferSize( 0 ), m_pInputLayout( xst_null ), m_ulVertexSize( 0 ), m_ulVertexCount( 0 ), m_pData( xst_null )
	{
		
	}

	CFixedVertexData::~CFixedVertexData()
	{
	}

	i32 CFixedVertexData::Create(ul32 ulVertexSize, ul32 ulVertexCount)
	{
		m_ulVertexCount = ulVertexCount;
		m_ulVertexSize = ulVertexSize;
		m_ulBufferSize = ulVertexSize * ulVertexCount;

		if( m_ulBufferSize <= 0 )
		{
			XST_LOG_ERR( "Buffer has no vertex attributes" );
			return XST_FAIL;
		}

		//If buffer is created do not create new if it has enough space
		if( m_Data.GetCapacity() < m_ulBufferSize )
		{
			if( XST_FAILED( m_Data.Create( m_ulBufferSize ) ) )
			{
				XST_LOG_ERR( "Failed to create vertex buffer. Memory error." );
				return XST_FAIL;
			}
		}

		m_pData = m_Data.GetData();

		return XST_OK;
	}

	i32 CFixedVertexData::Clone(CFixedVertexData* pData)
	{

		if( XST_FAILED( pData->Create( this->m_ulVertexSize, this->m_ulVertexCount ) ) )
		{
			return XST_FAIL;
		}
		
		xst_memcpy( pData->m_pData, this->m_Data.GetCapacity(), this->m_pData, this->m_Data.GetSize() );

		return XST_OK;
	}

	ul32 CFixedVertexData::_CalcOffset(cul32& ulVertexId, cul32& ulElementOffset)
	{
		return ( ulVertexId * m_ulVertexSize ) + ulElementOffset;
	}

	i32	CFixedVertexData::GetAttribute(cul32& ulVertexId, cul32& ulElementOffset, Vec2* pVec)
	{

		cul32 ulOffset = _CalcOffset( ulVertexId, ulElementOffset );
		return m_Data.Read< Vec2 >( pVec, ulOffset );
	}

	i32	CFixedVertexData::GetAttribute(cul32& ulVertexId, cul32& ulElementOffset, Vec3* pVec)
	{

		cul32 ulOffset = _CalcOffset( ulVertexId, ulElementOffset );
		return m_Data.Read< Vec3 >( pVec, ulOffset );
	}

	i32	CFixedVertexData::GetAttribute(cul32& ulVertexId, cul32& ulElementOffset, Vec4* pVec)
	{

		cul32 ulOffset = _CalcOffset( ulVertexId, ulElementOffset );
		return m_Data.Read< Vec4 >( pVec, ulOffset );
	}

	i32	CFixedVertexData::SetAttribute(cul32& ulVertexId, cul32& ulElementOffset, const Vec2& Vec)
	{	
		cul32 ulOffset = _CalcOffset( ulVertexId, ulElementOffset );
		return m_Data.Write< Vec2 >( Vec, ulOffset );
	}

	i32	CFixedVertexData::SetAttribute(cul32& ulVertexId, cul32& ulElementOffset, const Vec3& Vec)
	{

		cul32 ulOffset = _CalcOffset( ulVertexId, ulElementOffset );
		return m_Data.Write< Vec3 >( Vec, ulOffset );
	}

	i32	CFixedVertexData::SetAttribute(cul32& ulVertexId, cul32& ulElementOffset, const Vec4& Vec)
	{

		cul32 ulOffset = _CalcOffset( ulVertexId, ulElementOffset );
		return m_Data.Write< Vec4 >( Vec, ulOffset );
	}

	i32	CFixedVertexData::SetAttribute(cul32& ulVertexId, cul32& ulElementOffset, cf32& fX, cf32& fY)
	{
		cul32 ulOffset = _CalcOffset( ulVertexId, ulElementOffset );
		f32 faData[ 2 ] = { fX, fY };
		return m_Data.WriteArray< f32 >( faData, sizeof( f32 ) * 2, ulOffset );
	}

	i32	CFixedVertexData::SetAttribute(cul32& ulVertexId, cul32& ulElementOffset, cf32& fX, cf32& fY, cf32& fZ)
	{
		cul32 ulOffset = _CalcOffset( ulVertexId, ulElementOffset );
		f32 faData[ 3 ] = { fX, fY, fZ };
		return m_Data.WriteArray< f32 >( faData, sizeof( f32 ) * 3, ulOffset );
	}

	i32	CFixedVertexData::SetAttribute(cul32& ulVertexId, cul32& ulElementOffset, cf32& fX, cf32& fY, cf32& fZ, cf32& fW)
	{
		cul32 ulOffset = _CalcOffset( ulVertexId, ulElementOffset );
		f32 faData[ 4 ] = { fX, fY, fZ, fW };
		return m_Data.WriteArray< f32 >( faData, sizeof( f32 ) * 4, ulOffset );
	}

}//xse