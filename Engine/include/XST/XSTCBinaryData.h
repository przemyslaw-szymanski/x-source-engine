#ifndef XST_CBINARY_DATA_H
#define XST_CBINARY_DATA_H

#include "XSTCommonInclude.h"
#include "XSTCommonTypes.h"

namespace XST
{
	using namespace XST::Types;

	class XST_API CBinaryData
	{

		public:

									CBinaryData();
			virtual					~CBinaryData();

					i32				Create(ul32 ulDataSize);

					void			Delete();

					i32				Reserve(ul32 ulSize);

			xst_fi	void			Zero()
									{ SetData( 0 ); }

					i32				SetData(const CBinaryData& SrcData)
									{ return SetData( SrcData.GetData(), SrcData.GetSize() ); }

					i32				SetData(cu8* pData, cul32& ulDataSize);

					void			SetData(cu8& byValue);

			xst_fi	u8*				GetData()
									{ return m_pData; }

			xst_fi	cu8*			GetData() const
									{ return m_pData; }

			xst_fi	ul32			GetSize() 
									{ return m_ulCurrentSize; }

			xst_fi	cul32			GetSize() const
									{ return m_ulCurrentSize; }

			xst_fi	ul32			GetCapacity()
									{ return m_ulCapacity; }

			xst_fi	ul32			GetCapacity() const
									{ return m_ulCapacity; }

					template< class _T_ >
					i32				Write(const _T_& tData);

					template< class _T_ >
					i32				Write(const _T_& tData, ul32 ulOffset);

					template< class _T_ >
					i32				Write(const _T_& tData, cu16& usDataSize);

					template< class _T_ >
					i32				Write(const _T_& tData, ul32 ulOffset, cu16& usDataSize);

					template< class _T_ >
					i32				WriteArray(const _T_* pData, cul32& ulArraySizeInBytes);

					template< class _T_ >
					i32				WriteArray(const _T_* pData, cul32& ulArraySizeInBytes, cul32& ulOffset);


					template< class _T_ >
					i32				Read(_T_* pOut);

					template< class _T_ >
					i32				Read(_T_* pOut, ul32 ulOffset);

					template< class _T_ >
					i32				Read(_T_* pOut, cu16& usDataSize);

					template< class _T_ >
					i32				Read(_T_* pOut, ul32 ulOffset, cu16& usDataSize);

					bool			IsEndOfBuffer() const;
					bool			IsEndOfBuffer(ul32 ulOffset) const;
					bool			IsEndOfBuffer(ul32 ulPosition, ul32 ulDataSize) const;

		protected:

			u8*		m_pData;
			u8*		m_pCurrentPosition;
			ul32	m_ulCurrentSize;
			ul32	m_ulCapacity;
	};

	template< class _T_ >
	i32 CBinaryData::Write(const _T_ &tData)
	{
		xst_assert( !IsEndOfBuffer( sizeof( _T_ ) ), "(CBinaryData::Write)" );

		xst_memcpy( m_pCurrentPosition, m_ulCapacity, &tData, sizeof( _T_ ) );
		m_pCurrentPosition += sizeof( _T_ );
		m_ulCurrentSize += sizeof( _T_ );

		return XST_OK;
	}

	template< class _T_ >
	i32 CBinaryData::Write(const _T_ &tData, ul32 ulOffset)
	{
		xst_assert( !IsEndOfBuffer( ulOffset, sizeof( _T_ ) ), "(CBinaryData::Write)" );

		xst_memcpy( m_pData + ulOffset, m_ulCapacity, &tData, sizeof( _T_ ) );
		m_pCurrentPosition = m_pData;
		m_pCurrentPosition += ulOffset;
		m_pCurrentPosition += sizeof( _T_ );
		m_ulCurrentSize += sizeof( _T_ );

		return XST_OK;
	}

	template< class _T_ >
	i32 CBinaryData::Write(const _T_ &tData, cu16& usDataSize)
	{
		xst_assert( !IsEndOfBuffer( usDataSize ), "(CBinaryData::Write)" );

		xst_memcpy( m_pCurrentPosition, m_ulCapacit - sizeof( _T_ ), &tData, usDataSize );
		m_pCurrentPosition += usDataSize;
		m_ulCurrentSize += usDataSize;

		return XST_OK;
	}

	template< class _T_ >
	i32 CBinaryData::Write(const _T_ &tData, ul32 ulOffset, cu16& usDataSize)
	{
		xst_assert( !IsEndOfBuffer( ulOffset, usDataSize ), "(CBinaryData::Write)" );

		xst_memcpy( m_pData + ulOffset, m_ulCapacity, &tData, usDataSize );
		m_pCurrentPosition = m_pData;
		m_pCurrentPosition += ulOffset;
		m_pCurrentPosition += usDataSize;
		m_ulCurrentSize += usDataSize;

		return XST_OK;
	}

	template< class _T_ >
	i32	CBinaryData::WriteArray(const _T_* pData, cul32& ulArraySizeInBytes)
	{
		xst_assert( !IsEndOfBuffer( ulArraySizeInBytes ), "(CBinaryData::Write)" );

		xst_memcpy( m_pCurrentPosition, ulArraySizeInBytes, pData, ulArraySizeInBytes );
		m_pCurrentPosition += ulArraySizeInBytes;
		m_ulCurrentSize += ulArraySizeInBytes;

		return XST_OK;
	}

	template< class _T_ >
	i32	CBinaryData::WriteArray(const _T_* pData, cul32& ulArraySizeInBytes, cul32& ulOffset)
	{
		xst_assert( !IsEndOfBuffer( ulOffset, ulArraySizeInBytes ), "(CBinaryData::Write)" );

		xst_memcpy( m_pData + ulOffset, ulArraySizeInBytes, pData, ulArraySizeInBytes );
		m_pCurrentPosition = m_pData;
		m_pCurrentPosition += ulOffset;
		m_pCurrentPosition += ulArraySizeInBytes;
		m_ulCurrentSize += ulArraySizeInBytes;

		return XST_OK;
	}

	template< class _T_ >
	i32 CBinaryData::Read(_T_* pOut)
	{
		xst_assert( !IsEndOfBuffer(), "(CBinaryData::Write)" );

		*pOut = *(_T_*)m_pCurrentPosition;
		m_pCurrentPosition += sizeof( _T_ );

		return XST_OK;
	}

	template< class _T_ >
	i32 CBinaryData::Read(_T_* pOut, ul32 ulOffset)
	{
		xst_assert( !IsEndOfBuffer( ulOffset, sizeof( _T_ ) ), "(CBinaryData::Write)" );

		m_pCurrentPosition = m_pData + ulOffset;
		*pOut = *(_T_*)m_pCurrentPosition;
		m_pCurrentPosition += sizeof( _T_ );

		return XST_OK;
	}

	template< class _T_ >
	i32 CBinaryData::Read(_T_* pOut, cu16& usDataSize)
	{
		xst_assert( !IsEndOfBuffer(), "(CBinaryData::Write)" );

		*pOut = *(_T_*)m_pCurrentPosition;
		m_pCurrentPosition += usDataSize;

		return XST_OK;
	}

	template< class _T_ >
	i32 CBinaryData::Read(_T_* pOut, ul32 ulOffset, cu16& usDataSize)
	{
		xst_assert( !IsEndOfBuffer( ulOffset, usDataSize ), "(CBinaryData::Write)" );

		m_pCurrentPosition = m_pData + ulOffset;
		*pOut = ( *(_T_*)m_pCurrentPosition );
		m_pCurrentPosition += usDataSize;

		return XST_OK;
	}


}//xst

#endif