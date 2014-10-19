#include "XSECIndexData.h"

namespace XSE
{
	xst_fi ul32 CalcOffset(cul32& ulIndexId, cul32& ulIndexSize)
	{
		return ulIndexId * ulIndexSize;
	}

	CIndexData::CIndexData() : 
		m_ulTriangleCount( 0 ), 
		m_ulCurrentTriangleId( 0 ), 
		m_usIndexSize( sizeof( u16 ) ),
		m_ulIndexCount( 0 ),
		m_ulBufferSize( 0 ),
		_GetIndex( (pfnGetIndex)&CIndexData::_GetU16Index )
	{
	}

	CIndexData::~CIndexData()
	{
	}

	void CIndexData::Delete()
	{
		m_Data.Delete();
		m_ulTriangleCount = m_ulCurrentTriangleId = m_ulBufferSize = 0;
	}

	i32 CIndexData::Create(cul32& ulIndexCount)
	{
		////Calculate the index size
		//u16 usMax = 0; --usMax;
		//if( ulIndexCount >= usMax )
		//{
		//	m_usIndexSize = sizeof( ul32 );
		//	_GetIndex = &CIndexData::_GetU32Index;
		//}

		//cul32 ulDataSize = ulIndexCount * m_usIndexSize;
		//m_ulBufferSize = ulDataSize;
		//if( XST_FAILED( m_Data.Create( ulDataSize ) ) )
		//{
		//	return XST_FAIL;
		//}

		//m_Data.Reserve( ulDataSize );

		////Calc triangle count 
		//m_ulTriangleCount = ulIndexCount / 3;

		//m_ulIndexCount = ulIndexCount;

		//return XST_OK;
		//return Create( ulIndexCount, ulIndexCount );

		u16 usMax = 0; --usMax;
		if( ulIndexCount >= usMax )
		{
			return Create( ulIndexCount, sizeof( u32 ) );
		}

		return Create( ulIndexCount, sizeof( u16 ) );
	}

	i32	CIndexData::Create(cul32& ulIndexCount, cu32& uiIndexSize)
	{
		//Calculate the index size
		/*u16 usMax = 0; --usMax;
		if( uiIndexSize >= usMax )
		{
			m_usIndexSize = sizeof( ul32 );
			_GetIndex = &CIndexData::_GetU32Index;
		}*/
		if( uiIndexSize == sizeof( u32 ) )
		{
			m_usIndexSize = sizeof( u32 );
			_GetIndex = (pfnGetIndex)&CIndexData::_GetU32Index;
		}
		else
		{
			m_usIndexSize = sizeof( u16 );
			_GetIndex = (pfnGetIndex)&CIndexData::_GetU16Index;
		}

		cul32 ulDataSize = ulIndexCount * m_usIndexSize;
		m_ulBufferSize = ulDataSize;
		if( XST_FAILED( m_Data.Create( ulDataSize ) ) )
		{
			return XST_FAIL;
		}

		m_Data.Reserve( ulDataSize );

		//Calc triangle count 
		m_ulTriangleCount = ulIndexCount / 3;

		m_ulIndexCount = ulIndexCount;

		return XST_OK;
	}

	i32 CIndexData::CopyFrom(const CIndexData& Other)
	{
		xst_assert( m_ulBufferSize == Other.m_ulBufferSize, "(CIndexData::CopyFrom) Invalid buffer size" );

		if( XST_FAILED( m_Data.SetData( Other.m_Data ) ) )
		{
			return XST_FAIL;
		}
		m_ulIndexCount = Other.m_ulIndexCount;
		m_ulTriangleCount = Other.m_ulTriangleCount;
		return XST_OK;
	}

	void CIndexData::SetTriangle(cul32& ulTriangleId, cu16& usIndex0, cu16& usIndex1, cu16& usIndex2)
	{
		xst_assert( ulTriangleId < m_ulTriangleCount, "(CIndexData::SetTriangle) TriangleId out of bounds" );
		m_ulCurrentTriangleId = ulTriangleId;
		ul32 ulStartIndex = ( m_ulCurrentTriangleId ) * 3;
		
		m_Data.Write( usIndex0, CalcOffset( ulStartIndex + 0, m_usIndexSize ) );
		m_Data.Write( usIndex1, CalcOffset( ulStartIndex + 1, m_usIndexSize ) );
		m_Data.Write( usIndex2, CalcOffset( ulStartIndex + 2, m_usIndexSize ) );
	}

	void CIndexData::AddTriangle(cu16& usIndex0, cu16& usIndex1, cu16& usIndex2)
	{
		xst_assert( m_ulCurrentTriangleId < m_ulTriangleCount, "(CIndexData::AddTriangle) Too many triangles. Buffer is too small" );
		SetTriangle( m_ulCurrentTriangleId, usIndex0, usIndex1, usIndex2 );
		++m_ulCurrentTriangleId;
	}

	ul32 CIndexData::_CalcOffset(cul32 &ulIndexId) const
	{
		return ulIndexId * m_usIndexSize;
	}

	void CIndexData::GetTriangle(cu32& uTriangleId, u32 (*ppTriangleIndices)[3]) const
	{
		xst_assert( uTriangleId < m_ulTriangleCount, "(CIndexData::GetTriangle) TriangleID out of bounds" );
		cu8* pData = GetData();
		ul32 ulTriIndex = uTriangleId * 3;
		(*ppTriangleIndices)[ 0 ] = pData[ _CalcOffset( ulTriIndex + 0 ) ];
		(*ppTriangleIndices)[ 1 ] = pData[ _CalcOffset( ulTriIndex + 1 ) ];
		(*ppTriangleIndices)[ 2 ] = pData[ _CalcOffset( ulTriIndex + 2 ) ];
	}

}//xse