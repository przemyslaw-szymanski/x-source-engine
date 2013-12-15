#ifndef XST_CONSTANT_ARRAY_H
#define XST_CONSTANT_ARRAY_H

#include "XSTCommonTypes.h"
#include "XSTCommonInclude.h"

namespace XST
{
	template<class _T_, cu32 _MAX_SIZE_>
	class TIConstantContainer 
	{
		public:

			enum
			{
				MAX_SIZE = _MAX_SIZE_,
				ELEMENT_SIZE = sizeof( _T_ ),
				CAPACITY = MAX_SIZE * ELEMENT_SIZE
			};

		public:

								TIConstantContainer() : m_uiSize( 0 )
								{
								}

				virtual			~TIConstantContainer()
								{
								}

				xst_fi	_T_&	operator[](cu32& _uiID)
				{
					xst_assert( _uiID < m_uiSize, "(IConstantContainer::operator[]) This array has not that number of elements." );
					return m_pData[ _uiID ];
				}

				xst_fi	const _T_&	operator[](cu32& _uiID) const
				{
					xst_assert( _uiID < m_uiSize, "(IConstantContainer::operator[]) This array has not that number of elements." );
					return m_pData[ _uiID ];
				}

				xst_fi	bool	operator<(const TIConstantContainer& _Left)
				{
					return m_uiSize < _Left.m_uiSize;
				}

				xst_fi	bool	operator>(const TIConstantContainer& _Left)
				{
					return m_uiSize > _Left.m_uiSize;
				}

				xst_fi	bool	operator<=(const TIConstantContainer& _Left)
				{
					return m_uiSize <= _Left.m_uiSize;
				}

				xst_fi	bool	operator>=(const TIConstantContainer& _Left)
				{
					return m_uiSize >= _Left.m_uiSize;
				}

				xst_fi	bool	operator==(const TIConstantContainer& _Left)
				{
					return m_uiSize == _Left.m_uiSize;
				}

				xst_fi	u32		size()
								{ return m_uiSize; }

				xst_fi	cu32	size() const
								{ return m_uiSize; }

				xst_fi	u32		capacity()
								{ return CAPACITY; }

				xst_fi	cu32	capacity() const
								{ return CAPACITY; }

				xst_fi	void	clear()
								{ m_uiSize = 0; }

				xst_fi	
				const	_T_*	data() const
								{ return m_pData; }

				xst_i	i32		find(const _T_& tElement)
								{ for(u32 i = m_uiSize; i -->0; ) if( m_pData[ i ] == tElement ) return i; return -1; }

		protected:

			_T_		m_pData[ MAX_SIZE ];
			u32		m_uiSize;
	};

	template<class _T_, cu32 _MAX_SIZE_>
	class TCConstantArray : public virtual TIConstantContainer< _T_, _MAX_SIZE_ >
	{
		public:

			enum
			{
				MAX_SIZE = _MAX_SIZE_,
				ELEMENT_SIZE = sizeof( _T_ ),
				CAPACITY = MAX_SIZE * ELEMENT_SIZE
			};

		public:



				xst_fi	_T_&	operator[](cu32& _uiID)
				{
					xst_assert( _uiID < m_uiSize, "(TCConstantArray::operator[]) This array has not that number of elements." );
					return m_pData[ _uiID ];
				}

				xst_fi	const _T_&	operator[](cu32& _uiID) const
				{
					xst_assert( _uiID < m_uiSize, "(TCConstantArray::operator[]) This array has not that number of elements." );
					return m_pData[ _uiID ];
				}

				xst_fi	bool	operator<(const TCConstantArray& _Left)
				{
					return m_uiSize < _Left.m_uiSize;
				}

				xst_fi	bool	operator>(const TCConstantArray& _Left)
				{
					return m_uiSize > _Left.m_uiSize;
				}

				xst_fi	bool	operator<=(const TCConstantArray& _Left)
				{
					return m_uiSize <= _Left.m_uiSize;
				}

				xst_fi	bool	operator>=(const TCConstantArray& _Left)
				{
					return m_uiSize >= _Left.m_uiSize;
				}

				xst_fi	bool	operator==(const TCConstantArray& _Left)
				{
					return m_uiSize == _Left.m_uiSize;
				}

		public:

								TCConstantArray() : m_uiSize( 0 )
								{
								}

				virtual			~TCConstantArray()
								{
								}

				xst_i	void	push_back(const _T_& _tData)
				{
					xst_assert( m_uiSize < MAX_SIZE, "(TCConstantArray::push_back) Array has not free space." );
					this->m_pData[ this->m_uiSize++ ] = _tData;
				}

				void	push_front(const _T_& _tData)
				{
					xst_assert( this->m_uiSize < MAX_SIZE, "(TCConstantArray::push_back) Array has not free space." );
					_T_* pTmp = this->m_pData;
					#if defined (XST_VISUAL_STUDIO)
                        memcpy_s( pTmp + 1, CAPACITY, this->m_pData, this->m_uiSize * TIConstantContainer< _T_, _MAX_SIZE_ >::ELEMENT_SIZE );
                    #else
                        memcpy( pTmp + 1, this->m_pData, this->m_uiSize * TIConstantContainer< _T_, _MAX_SIZE_ >::ELEMENT_SIZE );
                    #endif
					//memcpy( pTmp + 1, this->m_pData, this->m_uiSize * TIConstantContainer< _T_, _MAX_SIZE_ >::ELEMENT_SIZE );
					this->m_pData[ 0 ] = _tData;
					++this->m_uiSize;
				}

				_T_		pop_front()
				{
					xst_assert( this->m_uiSize > 0, "(TCConstantArray::pop_front) Array has no elements" );
					_T_ tEl = this->m_pData[ 0 ];
					--this->m_uiSize;
					_T_* pTmp = this->m_pData;
					//xst_memcpy( pTmp, CAPACITY, pTmp + 1, this->m_uiSize * TIConstantContainer< _T_, _MAX_SIZE_ >::ELEMENT_SIZE );
					#if defined (XST_VISUAL_STUDIO)
                        memcpy_s( pTmp + 1, CAPACITY, this->m_pData, this->m_uiSize * TIConstantContainer< _T_, _MAX_SIZE_ >::ELEMENT_SIZE );
                    #else
                        memcpy( pTmp + 1, this->m_pData, this->m_uiSize * TIConstantContainer< _T_, _MAX_SIZE_ >::ELEMENT_SIZE );
                    #endif
					return tEl;
				}

				xst_fi _T_	pop_back()
				{
					xst_assert( this->m_uiSize > 0, "(TCConstantArray::pop_front) Array has no elements" );
					return this->m_pData[ --this->m_uiSize ];
				}

				xst_fi	u32		size()
								{ return m_uiSize; }

				xst_fi	cu32	size() const
								{ return m_uiSize; }

				xst_fi	u32		capacity()
								{ return CAPACITY; }

				xst_fi	cu32	capacity() const
								{ return CAPACITY; }

				xst_fi	void	clear()
								{ m_uiSize = 0; }

		private:

			_T_		m_pData[ MAX_SIZE ];
			u32		m_uiSize;

	};

	template<class _T_, cu32 _MAX_SIZE_>
	class TCSortedConstantArray : public TIConstantContainer< _T_, _MAX_SIZE_ >
	{
		public:

			enum
			{
				MAX_SIZE = _MAX_SIZE_,
				ELEMENT_SIZE = sizeof( _T_ ),
				CAPACITY = MAX_SIZE * ELEMENT_SIZE
			};

		public:



				xst_fi	_T_&	operator[](cu32& _uiID)
				{
					xst_assert( _uiID < m_uiSize, "(TCConstantArray::push_back) This array has not that number of elements." );
					return m_pData[ _uiID ];
				}

				xst_fi	bool	operator<(const TCSortedConstantArray& _Left)
				{
					return m_uiSize < _Left.m_uiSize;
				}

				xst_fi	bool	operator>(const TCSortedConstantArray& _Left)
				{
					return m_uiSize > _Left.m_uiSize;
				}

				xst_fi	bool	operator<=(const TCSortedConstantArray& _Left)
				{
					return m_uiSize <= _Left.m_uiSize;
				}

				xst_fi	bool	operator>=(const TCSortedConstantArray& _Left)
				{
					return m_uiSize >= _Left.m_uiSize;
				}

				xst_fi	bool	operator==(const TCSortedConstantArray& _Left)
				{
					return m_uiSize == _Left.m_uiSize;
				}

		public:

                                TCSortedConstantArray()
								{
								}

				virtual			~TCSortedConstantArray()
								{
								}

				void	push_back(const _T_& _tData)
				{
					xst_assert( m_uiSize < MAX_SIZE, "(TCConstantArray::push_back) Array has not free space." );

					if( this->m_uiSize == 0 )
					{
						this->m_pData[ this->m_uiSize++ ] = _tData;
						return;
					}

					_push_back_sorted( _tData, 0, this->m_uiSize );
				}

				_T_		pop_front()
				{
					xst_assert( this->m_uiSize > 0, "(TCConstantArray::pop_front) Array has no elements" );
					_T_ tEl = this->m_pData[ 0 ];
					--this->m_uiSize;
					_T_* pTmp = this->m_pData;
					//xst_memcpy( pTmp, CAPACITY, pTmp + 1, this->m_uiSize * TIConstantContainer< _T_, _MAX_SIZE_ >::ELEMENT_SIZE );
					#if defined (XST_VISUAL_STUDIO)
                        memcpy_s( pTmp + 1, CAPACITY, this->m_pData, this->m_uiSize * TIConstantContainer< _T_, _MAX_SIZE_ >::ELEMENT_SIZE );
                    #else
                        memcpy( pTmp + 1, this->m_pData, this->m_uiSize * TIConstantContainer< _T_, _MAX_SIZE_ >::ELEMENT_SIZE );
                    #endif
					return tEl;
				}

				xst_fi _T_	pop_back()
				{
					xst_assert( this->m_uiSize > 0, "(TCConstantArray::pop_front) Array has no elements" );
					return this->m_pData[ --this->m_uiSize ];
				}

				xst_fi	u32		size()
								{ return m_uiSize; }

				xst_fi	cu32	size() const
								{ return m_uiSize; }

				xst_fi	u32		capacity()
								{ return CAPACITY; }

				xst_fi	cu32	capacity() const
								{ return CAPACITY; }

		private:

				void			_push_back_sorted(const _T_& _tData, cu32& _uiBeginPos, cu32& _uiEndPos)
				{
					cu32 uiHalfSize = ( _uiBeginPos + _uiEndPos ) * 0.5f;
					_T_& tHalfEl = this->m_pData[ uiHalfSize ];

					if( _uiBeginPos == _uiEndPos )
					{
						_T_* pTmp = this->m_pData;
						xst_memcpy( pTmp + _uiBeginPos + 1, TIConstantContainer< _T_, _MAX_SIZE_ >::CAPACITY - (_uiBeginPos + 1) * TIConstantContainer< _T_, _MAX_SIZE_ >::ELEMENT_SIZE, pTmp + _uiBeginPos, TIConstantContainer< _T_, _MAX_SIZE_ >::CAPACITY - (_uiBeginPos + 1) * TIConstantContainer< _T_, _MAX_SIZE_ >::ELEMENT_SIZE );
						/*#if defined (XST_VISUAL_STUDIO)
                            memcpy_s( pTmp + 1, CAPACITY, this->m_pData, this->m_uiSize * TIConstantContainer< _T_, _MAX_SIZE_ >::ELEMENT_SIZE );
                        #else
                            memcpy( pTmp + 1, this->m_pData, this->m_uiSize * TIConstantContainer< _T_, _MAX_SIZE_ >::ELEMENT_SIZE );
                        #endif*/
						this->m_pData[ _uiBeginPos ] = _tData;
						++this->m_uiSize;
						return;
					}

					if( _tData < tHalfEl )
					{
						_push_back_sorted( _tData, _uiBeginPos, uiHalfSize );
					}
					else if( _tData > tHalfEl )
					{
						_push_back_sorted( _tData, uiHalfSize + 1, _uiEndPos );
					}
				}

		private:

			_T_		m_pData[ MAX_SIZE ];
			u32		m_uiSize;

	};

}//xst

#endif
