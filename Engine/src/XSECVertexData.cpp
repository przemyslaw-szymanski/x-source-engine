#include "XSECVertexData.h"
#include "XSEIInputLayout.h"
#include "XSERenderSystemCommon.h"

namespace XSE
{

	void DebugPrintVertexData(CVertexData& Data)
	{
		xst_stringstream ss;
		const IInputLayout* pIL = Data.GetInputLayout();
		Vec3 vecTmp3;
		Vec2 vecTmp2;
		Vec4 vecTmp4;

		for(ul32 i = 0; i < Data.GetVertexCount(); ++i)
		{
			if( pIL->IsPosition() )
			{
				Data.GetPosition( i, &vecTmp3 );
				ss << ";Pos: " << vecTmp3;
			}

			if( pIL->IsColor() )
			{
				Data.GetColor( i, &vecTmp4 );
				ss << ";Col: " << vecTmp4;
			}

			ss << xst_endl;
		}

		XST::CDebug::PrintDebugLN( ss.str() );
	}

	void GetAvailableAttributes(u32* auiAttributeMap, cu32& uiMapSize, const IInputLayout* pIL);

	CVertexData::CVertexData() : m_ulBufferSize( 0 ), m_pInputLayout( xst_null ), m_ulVertexSize( 0 ), m_ulVertexCount( 0 ), m_pData( xst_null )
	{
		
	}

	CVertexData::~CVertexData()
	{
	}

	i32 CVertexData::Create(ul32 ulVertexCount, const IInputLayout* pIL)
	{
		xst_assert( pIL, "(CVertexData::Create)" );

		m_pInputLayout = pIL;
		m_ulVertexSize = pIL->GetVertexSize();
		m_ulVertexCount = ulVertexCount;
		m_ulBufferSize = m_ulVertexSize * ulVertexCount;
		//Set default value for each element. If 0xffffffff is set that attribute is not used
		memset( m_auiAttributeMap, 0xffffffff, sizeof( u32 ) * (u32)InputLayoutElements::_ENUM_COUNT );

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

		//Get available attributes
		GetAvailableAttributes( m_auiAttributeMap, InputLayoutElements::_ENUM_COUNT, m_pInputLayout );

		m_Data.Zero();

		m_pData = m_Data.GetData();

		return XST_OK;
	}

	i32 CVertexData::Clone(CVertexData* pData)
	{

		if( XST_FAILED( pData->Create( this->m_ulVertexCount, this->m_pInputLayout ) ) )
		{
			return XST_FAIL;
		}
		
		xst_memcpy( pData->m_pData, this->m_Data.GetCapacity(), this->m_pData, this->m_Data.GetSize() );

		return XST_OK;
	}

	static void XSTAssertMsg(lpcastr pszExMsg, lpcastr pszFile, lpcastr pszLine)
	{
	
	}

	i32 CVertexData::SetData(const CVertexData& Data)
	{
		xst_assert( m_pInputLayout == Data.GetInputLayout(), "(CVertexData::SetData)" );
		xst_assert( m_Data.GetCapacity() >= Data.m_Data.GetCapacity(), "(CVertexData::SetData)" );
		xst_assert( m_ulBufferSize >= Data.m_ulBufferSize, "(CVertexData::SetData)" );

		//Set new size
		m_ulBufferSize = Data.m_ulBufferSize;

		//Copy memory
		if( XST_FAILED( m_Data.SetData( Data.m_Data ) ) )
		{
			return XST_FAIL;
		}

		return XST_OK;
	}

	i32 CVertexData::Clone(CFixedVertexData* pData)
	{

		if( XST_FAILED( pData->Create( this->GetInputLayout()->GetVertexSize(), this->m_ulVertexCount ) ) )
		{
			return XST_FAIL;
		}
		
		xst_memcpy( pData->m_pData, this->m_Data.GetCapacity(), this->m_pData, this->m_Data.GetSize() );

		return XST_OK;
	}

	ul32 CVertexData::_CalcOffset(cul32& ulVertexId, const IInputLayout* pIL, cul32& ulAttributeId) const 
	{
		xst_assert( m_auiAttributeMap[ ulAttributeId ] != 0xffffffff, "(CVertexData::_CalcOffset) This attribute is not available for this vertex data. Check the input layout" );
		xst_assert( m_ulBufferSize != 0, "(CVertexData::_CalcOffset) Vertex size is not set. Create method not called" );

		return ( ulVertexId * m_ulVertexSize ) + pIL->GetElementOffset( m_auiAttributeMap[ ulAttributeId ] );
	}

	i32	CVertexData::GetAttribute(cul32& ulVertexId, cul32& ulAttributeId, Vec2* pVec)
	{

		cul32 ulOffset = _CalcOffset( ulVertexId, m_pInputLayout, ulAttributeId );
		return m_Data.Read< Vec2 >( pVec, ulOffset );
	}

	i32	CVertexData::GetAttribute(cul32& ulVertexId, cul32& ulAttributeId, Vec3* pVec)
	{

		cul32 ulOffset = _CalcOffset( ulVertexId, m_pInputLayout, ulAttributeId );
		return m_Data.Read< Vec3 >( pVec, ulOffset );
	}

	i32	CVertexData::GetAttribute(cul32& ulVertexId, cul32& ulAttributeId, Vec4* pVec)
	{

		cul32 ulOffset = _CalcOffset( ulVertexId, m_pInputLayout, ulAttributeId );
		return m_Data.Read< Vec4 >( pVec, ulOffset );
	}

	i32	CVertexData::SetAttribute(cul32& ulVertexId, cul32& ulAttributeId, const Vec2& Vec)
	{	
		cul32 ulOffset = _CalcOffset( ulVertexId, m_pInputLayout, ulAttributeId );
		return m_Data.Write< Vec2 >( Vec, ulOffset );
	}

	i32	CVertexData::SetAttribute(cul32& ulVertexId, cul32& ulAttributeId, const Vec3& Vec)
	{

		cul32 ulOffset = _CalcOffset( ulVertexId, m_pInputLayout, ulAttributeId );
		return m_Data.Write< Vec3 >( Vec, ulOffset );
	}

	i32	CVertexData::SetAttribute(cul32& ulVertexId, cul32& ulAttributeId, const Vec4& Vec)
	{

		cul32 ulOffset = _CalcOffset( ulVertexId, m_pInputLayout, ulAttributeId );
		return m_Data.Write< Vec4 >( Vec, ulOffset );
	}

	i32	CVertexData::SetAttribute(cul32& ulVertexId, cul32& ulAttributeId, cf32& fX, cf32& fY)
	{
		cul32 ulOffset = _CalcOffset( ulVertexId, m_pInputLayout, ulAttributeId );
		f32 faData[ 2 ] = { fX, fY };
		return m_Data.WriteArray< f32 >( faData, sizeof( f32 ) * 2, ulOffset );
	}

	i32	CVertexData::SetAttribute(cul32& ulVertexId, cul32& ulAttributeId, cf32& fX, cf32& fY, cf32& fZ)
	{
		cul32 ulOffset = _CalcOffset( ulVertexId, m_pInputLayout, ulAttributeId );
		f32 faData[ 3 ] = { fX, fY, fZ };
		return m_Data.WriteArray< f32 >( faData, sizeof( f32 ) * 3, ulOffset );
	}

	i32	CVertexData::SetAttribute(cul32& ulVertexId, cul32& ulAttributeId, cf32& fX, cf32& fY, cf32& fZ, cf32& fW)
	{
		cul32 ulOffset = _CalcOffset( ulVertexId, m_pInputLayout, ulAttributeId );
		f32 faData[ 4 ] = { fX, fY, fZ, fW };
		return m_Data.WriteArray< f32 >( faData, sizeof( f32 ) * 4, ulOffset );
	}

	void GetAvailableAttributes(u32* auiAttributeMap, cu32& uiMapSize, const IInputLayout* pIL)
	{
		const SInputLayoutElement* aElements = pIL->GetElements();
		for(u32 i = 0; i < pIL->GetElementCount(); ++i)
		{
			const SInputLayoutElement& Element = aElements[ i ];
			
			auiAttributeMap[ Element.uiId ] = i;
		}
	}

	
}//xse