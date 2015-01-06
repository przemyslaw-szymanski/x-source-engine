#include "XST/XSTCBinaryData.h"

namespace XST
{
	CBinaryData::CBinaryData() : m_pData( xst_null ), m_ulCurrentSize( 0 ), m_ulCapacity( 0 ), m_pCurrentPosition( xst_null )
	{
	}

	CBinaryData::~CBinaryData()
	{
		Delete();
	}

	void CBinaryData::Delete()
	{
		xst_deletea( m_pData );
		m_ulCurrentSize = m_ulCapacity = 0;
		m_pCurrentPosition = xst_null;
	}

	i32 CBinaryData::Create(ul32 ulSize)
	{
		Delete();
		m_ulCapacity = ulSize + 1;
		
		m_pData = xst_new u8[ m_ulCapacity ];
		if( m_pData == xst_null )
		{
			return XST_FAIL;
		}
		m_pData[ ulSize ] = 0;
		m_pCurrentPosition = m_pData;

		return XST_OK;
	}

	i32 CBinaryData::Reserve(ul32 ulSize)
	{
		if( ulSize > m_ulCapacity )
		{
			return XST_FAIL;
		}

		//m_ulCurrentSize = ulSize;
		return XST_OK;
	}

	void CBinaryData::SetData(cu8& byValue)
	{
		xst_assert( m_ulCapacity > 0, "(CBinaryData::SetData) Data not created" );

		memset( m_pData, byValue, m_ulCapacity );
	}

	i32 CBinaryData::SetData(cu8* pData, cul32& ulDataSize)
	{
		xst_assert( m_ulCapacity > ulDataSize, "(CBinaryData::SetData) Source data is too large for this buffer" );
		
		m_ulCurrentSize = 0;

		xst_memcpy( m_pData, ulDataSize, pData, ulDataSize );
		m_pCurrentPosition = m_pData;
		m_ulCurrentSize = ulDataSize;
		return XST_OK;
	}

	i32 CBinaryData::MoveData(u8** ppData, cul32& uDataSize)
	{
		xst_assert( (*ppData) != xst_null, "(CBinaryData::GrabData) Data source must not be null" );
		Delete();
		m_pData = (*ppData);
		m_ulCurrentSize = 0;
		m_pCurrentPosition = m_pData;
		m_ulCapacity = uDataSize;
		(*ppData) = xst_null;
		return XST_OK;
	}

	bool CBinaryData::IsEndOfBuffer() const
	{
		//return m_pCurrentPosition >= m_pData + m_ulCapacity;
		return m_ulCurrentSize > m_ulCapacity;
	}

	bool CBinaryData::IsEndOfBuffer(ul32 ulOffset) const
	{
		//return m_pCurrentPosition + ulOffset >= m_pData + m_ulCapacity;
		return m_ulCurrentSize + ulOffset > m_ulCapacity;
	}

	bool CBinaryData::IsEndOfBuffer(ul32 ulPosition, ul32 ulDataSize) const
	{
		//return m_pData + ulPosition + ulDataSize >= m_pData + m_ulCapacity;
		return ulPosition + ulDataSize > m_ulCapacity;
	}

}//xst