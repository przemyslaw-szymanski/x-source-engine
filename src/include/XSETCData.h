#ifndef XSE_TCDATA_H
#define XSE_TCDATA_H

#include "XSECommon.h"

namespace XSE
{
	template< class _T_ >
	class TCData
	{
		public:

							TCData() : m_aData( xst_null ), m_pCurrPosition( xst_null ), m_ulSize( 0 ), m_ulCapacity( 0 ) {}
			virtual			~TCData() 
			{
				Delete();
			}

			i32	Create(ul32 ulSize)
			{
				Delete();
				m_aData = xst_new _T_[ ulSize ];
				m_pCurrPosition = m_aData;
				m_ulSize = 0;
				m_ulCapacity = ulSize;
				if( !m_aData )
				{
					return XST_FAIL;
				}

				return XST_OK;
			}

			i32 Reserve(ul32 ulSize)
			{
				if( ulSize > m_ulCapacity )
				{
					return XST_FAIL;
				}

				m_ulSize = ulSize;

				return XST_OK;
			}

			void Delete()
			{
				xst_deletea( m_aData );
				m_ulSize = m_ulCapacity = 0;
				m_pCurrPosition = xst_null;
			}

			xst_i _T_*	AddElements(ul32 ulCount)
			{
				if( !IsNewDataAvailable( ulCount ) )
				{
					return xst_null;
				}

				_T_* aVertices = (_T_*)m_pCurrPosition;
				m_pCurrPosition += ulCount;
				m_ulSize += ulCount;

				return aVertices;
			}

			xst_fi _T_* AddElement()
			{
				return AddElements( 1 );
			}

			bool IsNewDataAvailable(ul32 ulCount) const
			{
				return m_ulSize + ulCount <= m_ulCapacity;
			}

			xst_fi ul32 GetSize() const
			{
				return m_ulSize;
			}

			xst_fi ul32 GetCapacity() const
			{
				return m_ulCapacity;
			}

			xst_fi ul32 GetElementSize() const
			{
				return sizeof( _T_ );
			}

			xst_fi ul32 GetTotalByteCount() const
			{
				return GetSize() * GetElementSize();
			}

			xst_fi const _T_* GetData() const
			{
				return m_aData;
			}

			xst_fi _T_* GetData()
			{
				return m_aData;
			}

			xst_fi void SetElement(ul32 ulElementId, const _T_& tElement)
			{
				xst_assert( ulElementId < m_ulSize, "(TCData::SetElement) Element Id out of bounds" );
				m_aData[ ulElementId ] = tElement;
			}

			xst_fi _T_& operator[](cul32& ulElementID)
			{
				//std::stringstream ss;
				//ss << ulElementID;
				xst_assert( ulElementID < m_ulSize, "(TCData::operator[]) Element Id out of bounds" );
				/*if( ulElementID >= m_ulSize )
				{
					MessageBoxA( 0, ss.str().data(), 0, 0 );
				}*/
				return m_aData[ ulElementID ];
			}

			xst_fi const _T_& operator[](cul32& ulElementID) const
			{
				xst_assert( ulElementID < m_ulSize, "(TCData::operator[]) Element Id out of bounds" );
				return m_aData[ ulElementID ];
			}

		protected:

			_T_*	m_aData;
			_T_*	m_pCurrPosition;
			ul32	m_ulSize;
			ul32	m_ulCapacity;

	};
}//xse

#endif