#ifndef XST_TCDYNAMIC_ARRAY_H
#define XST_TCDYNAMIC_ARRAY_H

#include "XSTCommonTypes.h"
#include "XSTCommonInclude.h"

namespace XST
{
	template<class _T_>
	class XST_API TIDynamicContainer
	{
		public:

			enum
			{
				MAX_SIZE = UINT_MAX,
				ELEMENT_SIZE = sizeof( _T_ ),
				DEFAULT_SIZE = 10
			};

		public:

								TIDynamicContainer() : m_uiSize( 0 ), m_uiCapacity( 0 ), m_pData( xst_null )
								{
									reserve( DEFAULT_SIZE );
								}

								TIDynamicContainer(u32 uiSize) : m_pData( xst_null ), m_uiCapacity( 0 ), m_uiSize( 0 )
								{
									reserve( uiSize );
								}

				virtual			~TIDynamicContainer()
								{
									xst_deletea( m_pData );
									m_uiSize = 0;
									m_uiCapacity = 0;
								}

				virtual	i32		reserve(u32 uiSize)
						{
							xst_deletea( m_pData );

							m_uiCapacity = uiSize * 2;
							m_pData = xst_new _T_[ m_uiCapacity ];

							if( m_pData == xst_null )
							{
								return RESULT::FAILED;
							}

							m_uiSize = 0;

							return RESULT::OK;
						}

				virtual	i32		resize(u32 uiNewSize)
						{
							u32 uiSize = m_uiSize;
							_T_* pTmp = xst_new _T_[ m_uiSize ];
							xst_memcpy( pTmp, m_uiSize * ELEMENT_SIZE, m_pData, m_uiSize * ELEMENT_SIZE );
							if( reserve( uiNewSize ) != RESULT::OK )
							{
								return RESULT::FAILED;
							}

							xst_memcpy( m_pData, m_uiSize * ELEMENT_SIZE, pTmp, uiSize * ELEMENT_SIZE );

							xst_deletea( pTmp );

							return RESULT::OK;
						}

				virtual xst_fi	_T_&	operator[](cu32& _uiID)
				{
					xst_assert( _uiID < m_uiSize, "(IConstantContainer::operator[]) This array has not that number of elements." );
					return m_pData[ _uiID ];
				}

				virtual xst_fi	const _T_&	operator[](cu32& _uiID) const
				{
					xst_assert( _uiID < m_uiSize, "(IConstantContainer::operator[]) This array has not that number of elements." );
					return m_pData[ _uiID ];
				}

				virtual xst_fi	bool	operator<(const TIDynamicContainer& _Left)
				{
					return m_uiSize < _Left.m_uiSize;
				}

				virtual xst_fi	bool	operator>(const TIDynamicContainer& _Left)
				{
					return m_uiSize > _Left.m_uiSize;
				}

				virtual xst_fi	bool	operator<=(const TIDynamicContainer& _Left)
				{
					return m_uiSize <= _Left.m_uiSize;
				}

				virtual xst_fi	bool	operator>=(const TIDynamicContainer& _Left)
				{
					return m_uiSize >= _Left.m_uiSize;
				}

				virtual xst_fi	bool	operator==(const TIDynamicContainer& _Left)
				{
					return m_uiSize == _Left.m_uiSize;
				}

				xst_fi	u32		size()
								{ return m_uiSize; }

				xst_fi	cu32	size() const
								{ return m_uiSize; }

				xst_fi	u32		capacity()
								{ return m_uiCapacity; }

				xst_fi	cu32	capacity() const
								{ return m_uiCapacity; }

				xst_fi	void	clear()
								{ m_uiSize = 0; }

				xst_fi	u32		totalSize()
								{ return m_uiCapacity * ELEMENT_SIZE; }

		protected:

			_T_*	m_pData;
			u32		m_uiSize;
			u32		m_uiCapacity;
	};


	template<class _T_>
	class XST_API TCDynamicArray : public virtual TIDynamicContainer< _T_ >
	{
		public:

			enum
			{
				MAX_SIZE = TIDynamicContainer< _T_ >::MAX_SIZE,
				ELEMENT_SIZE = TIDynamicContainer< _T_ >::ELEMENT_SIZE,
			};

		public:



				xst_fi	_T_&	operator[](cu32& _uiID)
				{
					xst_assert( _uiID < this->m_uiSize, "(TCConstantArray::push_back) This array has not that number of elements." );
					return this->m_pData[ _uiID ];
				}

				xst_fi	const _T_&	operator[](cu32& _uiID) const
				{
					xst_assert( _uiID < this->m_uiSize, "(TCConstantArray::push_back) This array has not that number of elements." );
					return this->m_pData[ _uiID ];
				}

				xst_fi	bool	operator<(const TCDynamicArray& _Left)
				{
					return this->m_uiSize < _Left.m_uiSize;
				}

				xst_fi	bool	operator>(const TCDynamicArray& _Left)
				{
					return this->m_uiSize > _Left.m_uiSize;
				}

				xst_fi	bool	operator<=(const TCDynamicArray& _Left)
				{
					return this->m_uiSize <= _Left.m_uiSize;
				}

				xst_fi	bool	operator>=(const TCDynamicArray& _Left)
				{
					return this->m_uiSize >= _Left.m_uiSize;
				}

				xst_fi	bool	operator==(const TCDynamicArray& _Left)
				{
					return this->m_uiSize == _Left.m_uiSize;
				}

		public:

								TCDynamicArray()
								{
								}

				virtual			~TCDynamicArray()
								{
								}

				virtual xst_i	i32		push_back(const _T_& _tData)
				{
					//xst_assert( m_uiSize < m_uiCapacity, "(TCConstantArray::push_back) Array has not free space." );
					//this->m_pData[ this->m_uiSize++ ] = _tData;
					if( this->m_uiSize + 1 >= this->m_uiCapacity )
					{
						if( this->resize( this->m_uiCapacity * 2 ) != RESULT::OK )
						{
							return RESULT::FAILED;
						}
					}

					this->m_pData[ this->m_uiSize++ ] = _tData;
					return RESULT::OK;
				}

				virtual xst_i	i32 push_back_range(const _T_* pData, u32 uCount)
				{
					if( this->m_uiSize + uCount >= this->m_uiCapacity )
					{
						if( this->resize( this->m_uiCapacity * 2 ) != RESULT::OK )
						{
							return RESULT::FAILED;
						}
					}

					_T_* pTmp = this->m_pData + this->m_uiSize;
					xst_memcpy( pTmp, this->m_uiCapacity, pData, sizeof(_T_) * uCount );
					this->m_uiSize += uCount;
					return RESULT::OK;
				}

				virtual _T_		pop_front()
				{
					xst_assert( this->m_uiSize > 0, "(TCConstantArray::pop_front) Array has no elements" );
					_T_ tEl = this->m_pData[ 0 ];
					--this->m_uiSize;
					_T_* pTmp = this->m_pData;
					//xst_memcpy( pTmp, CAPACITY, pTmp + 1, this->m_uiSize * TIConstantContainer< _T_, _MAX_SIZE_ >::ELEMENT_SIZE );
					#if defined (XST_VISUAL_STUDIO)
						memcpy_s( pTmp + 1, this->m_uiCapacity, this->m_pData, this->m_uiSize * TIDynamicContainer< _T_ >::ELEMENT_SIZE );
                    #else
                        memcpy( pTmp + 1, this->m_pData, this->m_uiSize * TIDynamicContainer< _T_ >::ELEMENT_SIZE );
                    #endif
					return tEl;
				}

				virtual xst_fi _T_	pop_back()
				{
					xst_assert( this->m_uiSize > 0, "(TCConstantArray::pop_front) Array has no elements" );
					return this->m_pData[ --this->m_uiSize ];
				}

				xst_fi	u32		size()
								{ return this->m_uiSize; }

				xst_fi	cu32	size() const
								{ return this->m_uiSize; }

				xst_fi	u32		capacity()
								{ return TIDynamicContainer< _T_ >::CAPACITY; }

				xst_fi	cu32	capacity() const
								{ return TIDynamicContainer< _T_ >::CAPACITY; }

				xst_fi	void	clear()
								{ this->m_uiSize = 0; }

	};

	template<class _T_>
	class XST_API TCSortedDynamicArray : public virtual TIDynamicContainer< _T_ >
	{
		public:

			enum
			{
				MAX_SIZE = TIDynamicContainer< _T_ >::MAX_SIZE,
				ELEMENT_SIZE = sizeof( _T_ )
			};

			typedef fastdelegate::FastDelegate2< const _T_&, const _T_&, bool >	CompareDelegate;
            class XST_API CompareDelegate;
#if( XST_VS_VER < 10 )
			XST_TEMPLATE_CLASS fastdelegate::FastDelegate2< const _T_&, const _T_&, bool >;
#endif
			typedef bool (*pfnCompareLess)(const _T_&, const _T_&);

		public:



				xst_fi	_T_&	operator[](cu32& _uiID)
				{
					xst_assert( _uiID < this->m_uiSize, "(TCConstantArray::push_back) This array has not that number of elements." );
					return this->m_pData[ _uiID ];
				}

				xst_fi	bool	operator<(const TCSortedDynamicArray& _Left)
				{
					return this->m_uiSize < _Left.m_uiSize;
				}

				xst_fi	bool	operator>(const TCSortedDynamicArray& _Left)
				{
					return this->m_uiSize > _Left.m_uiSize;
				}

				xst_fi	bool	operator<=(const TCSortedDynamicArray& _Left)
				{
					return this->m_uiSize <= _Left.m_uiSize;
				}

				xst_fi	bool	operator>=(const TCSortedDynamicArray& _Left)
				{
					return this->m_uiSize >= _Left.m_uiSize;
				}

				xst_fi	bool	operator==(const TCSortedDynamicArray& _Left)
				{
					return this->m_uiSize == _Left.m_uiSize;
				}

		public:

                                TCSortedDynamicArray()
								{
									CompareFunction().bind( &TCSortedDynamicArray< _T_ >::_DefaultCompare );
									m_CompareLess = &TCSortedDynamicArray< _T_ >::_DefaultCompare;
								}

				virtual			~TCSortedDynamicArray()
								{
								}

				void	push_back(const _T_& _tData)
				{
					xst_assert( this->m_uiSize < MAX_SIZE, "(TCConstantArray::push_back) Array has not free space." );

					if( this->m_uiSize + 1 >= this->m_uiCapacity )
					{
						if( this->resize( this->m_uiCapacity * 2 ) != RESULT::OK )
						{
							return;
						}
					}

					if( this->m_uiSize == 0 )
					{
						this->m_pData[ this->m_uiSize++ ] = _tData;
						return;
					}
					else
					if( this->m_uiSize == 1 )
					{
						//if( !m_CompareDelegate( this->m_pData[ 0 ] , _tData ) )
						if( !m_CompareLess( this->m_pData[ 0 ], _tData ) )
						{
							this->m_pData[ this->m_uiSize++ ] = this->m_pData[ 0 ];
							this->m_pData[ 0 ] = _tData;
						}
						else
						{
							this->m_pData[ this->m_uiSize++ ] = _tData;
						}

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
                        memcpy_s( pTmp + 1, this->m_uiCapacity, this->m_pData, this->m_uiSize * TIDynamicContainer< _T_ >::ELEMENT_SIZE );
                    #else
                        memcpy( pTmp + 1, this->m_pData, this->m_uiSize * TIDynamicContainer< _T_ >::ELEMENT_SIZE );
                    #endif
					return tEl;
				}

				xst_fi _T_	pop_back()
				{
					xst_assert( this->m_uiSize > 0, "(TCConstantArray::pop_front) Array has no elements" );
					return this->m_pData[ --this->m_uiSize ];
				}

				xst_fi	u32		size()
								{ return this->m_uiSize; }

				xst_fi	cu32	size() const
								{ return this->m_uiSize; }

				xst_fi	u32		capacity()
								{ return TIDynamicContainer< _T_ >::CAPACITY; }

				xst_fi	cu32	capacity() const
								{ return TIDynamicContainer< _T_ >::CAPACITY; }

				xst_fi	CompareDelegate&	CompareFunction()
											{ return m_CompareDelegate; }

				xst_fi	void	SetCompareFunction(pfnCompareLess Func)
								{ m_CompareLess = Func; }

		private:

				void			_push_back_sorted(const _T_& _tData, cu32& _uiBeginPos, cu32& _uiEndPos)
				{
					u32 uiHalfSize = (u32)(( _uiBeginPos + _uiEndPos ) * 0.5f);
					//if( uiHalfSize >= this->m_uiSize ) uiHalfSize = XST::Math::Max< u32 >( 0, this->m_uiSize - 1 );
					_T_& tHalfEl = this->m_pData[ uiHalfSize ];

					if( _uiBeginPos == _uiEndPos )
					{
						_T_* pTmp = this->m_pData;
						_T_* pSrc = pTmp + _uiBeginPos;
						_T_* pDst = pTmp + _uiBeginPos + 1;
						ul32 ulDstSize = this->m_uiCapacity - ( _uiBeginPos + 1 ) * TIDynamicContainer< _T_ >::ELEMENT_SIZE;
						ul32 ulDataSizeToCopy = this->m_uiCapacity - (_uiBeginPos + 1) * TIDynamicContainer< _T_ >::ELEMENT_SIZE; 
						
						xst_memcpy( pDst, ulDstSize, pSrc, ulDstSize - TIDynamicContainer< _T_ >::ELEMENT_SIZE );
						
						this->m_pData[ _uiBeginPos ] = _tData;
						++this->m_uiSize;
						return;
					}

					//if( _tData < tHalfEl )
					//if( m_CompareDelegate( _tData, tHalfEl ) )
					if( m_CompareLess( _tData, tHalfEl ) )
					{
						_push_back_sorted( _tData, _uiBeginPos, uiHalfSize );
					}
					else //if( _tData > tHalfEl )
					{
						_push_back_sorted( _tData, uiHalfSize + 1, _uiEndPos );
					}
				}

				static bool _DefaultCompare(const _T_& Left, const _T_& Right)
				{
					return Left < Right;
				}

		private:

			//Returns true if param1 is less than param2
			CompareDelegate	m_CompareDelegate;
			pfnCompareLess	m_CompareLess;

	};

}//xst

#endif
