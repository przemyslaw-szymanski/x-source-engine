#ifndef XST_CSMART_POINTER_H
#define XST_CSMART_POINTER_H

#include "XSTCommonInclude.h"
#include "XSTCommonTypes.h"
#include "XSTIObject.h"

namespace XST
{
	template<class _T_>
	class TCWeakPointer
	{
		public:

			explicit    TCWeakPointer(_T_* _pPtr) : m_pPtr( _pPtr )
                        {
                        }

						TCWeakPointer(const TCWeakPointer& _Ptr)
                        {
                            if( this != &_Ptr )
                            {
                                xst_delete( m_pPtr );
                            }
                            m_pPtr = _Ptr->m_pPtr;
                        }

			virtual		~TCWeakPointer() 
						{
							m_pPtr = xst_null;
						}

		protected:

            _T_*    m_pPtr = xst_null;
	};

	template<class  _T_>
	class TCSmartPointer //: public TCWeakPointer< _T_ >
	{
        public:

                        TCSmartPointer() : m_pPtr( 0 ) {}

			explicit    TCSmartPointer(_T_* _pPtr) : m_pPtr( _pPtr )
                        {
                        }

						TCSmartPointer(const TCSmartPointer& _Ptr) : m_pPtr( 0 )
                        {
                            if( m_pPtr != &_Ptr.m_pPtr )
                            {
								delete m_pPtr;
                            }
                            m_pPtr = _Ptr.m_pPtr;
                        }

						TCSmartPointer(TCSmartPointer&& _Ptr)
						{
							if( m_pPtr != &_Ptr.m_pPtr )
                            {
                                delete m_pPtr;
                            }
							m_pPtr = _Ptr.m_pPtr;
							xst_delete( _Ptr.m_pPtr );
						}

            virtual     ~TCSmartPointer()
            {
                xst_delete( m_pPtr );
            }

			TCSmartPointer& operator=(const TCSmartPointer& _Right)
            {
                if( this == &_Right )
                    return *this;

                delete m_pPtr;
                m_pPtr = _Right.m_pPtr;
                return *this;
            }

			TCSmartPointer& operator=(TCSmartPointer&& _Right)
            {
                if( this == &_Right )
                    return *this;

                delete m_pPtr;
                m_pPtr = _Right.m_pPtr;
				xst_delete( _Right.m_pPtr );
                return *this;
            }

			TCSmartPointer& operator=(_T_* pRight)
            {
                if( this == &pRight )
                    return *this;

                delete m_pPtr;
                m_pPtr = pRight.m_pPtr;
                return *this;
            }

            _T_*        GetPointer() const
            {
                return m_pPtr;
            }

			xst_fi operator bool()
			{
				return m_pPtr != xst_null;
			}

            _T_*	operator->()
            {
                return m_pPtr;
            }

			xst_fi bool IsNull()
			{
				return m_pPtr == xst_null;
			}

		protected:

            _T_*    m_pPtr = xst_null;
	};


    template<class _T_>
    class TCObjectSmartPointer 
    {
        public:

						xst_fi TCObjectSmartPointer() 
						{
						}

			explicit	xst_fi TCObjectSmartPointer(_T_* pPtr) : m_pPtr( pPtr )
						{
						}

						xst_fi TCObjectSmartPointer(const TCObjectSmartPointer& pRight)
                        {
							if( m_pPtr != pRight.m_pPtr )
								delete m_pPtr;

                            m_pPtr = pRight.m_pPtr;
							if( m_pPtr )
								m_pPtr->AddRef();
                        }

						xst_fi TCObjectSmartPointer(TCObjectSmartPointer&& pRight)
                        {
							if( m_pPtr != pRight.m_pPtr )
								delete m_pPtr;

                            m_pPtr = pRight.m_pPtr;
							if( m_pPtr )
								m_pPtr->AddRef();
							pRight.Release();
                        }

						template<class _A_>
						xst_fi TCObjectSmartPointer(const TCObjectSmartPointer< _A_ >& pRight) 
						{
							if( m_pPtr != pRight.GetPointer() )
								delete m_pPtr;

							m_pPtr = (_T_*)pRight.GetPointer();
							if( m_pPtr )
								m_pPtr->AddRef();
						}

						template<class _A_>
						xst_fi TCObjectSmartPointer(TCObjectSmartPointer< _A_ >&& pRight) 
						{
							if( m_pPtr != pRight.GetPointer() )
								delete m_pPtr;

							m_pPtr = (_T_*)pRight.GetPointer();
							if( m_pPtr )
								m_pPtr->AddRef();
							pRight.Release();
						}

		virtual	xst_fi  ~TCObjectSmartPointer()
		{
			xst_release( m_pPtr );
		}

		xst_fi	void Release()
		{
			xst_release( m_pPtr );
		}

		xst_fi TCObjectSmartPointer&   operator=(const TCObjectSmartPointer& pRight)
		{
			if( m_pPtr == pRight.m_pPtr )
				return *this;

			xst_release( this->m_pPtr );
			this->m_pPtr = pRight.m_pPtr;
			if( this->m_pPtr )
				this->m_pPtr->AddRef();
			return *this;
		}

		template<class _A_>
		xst_fi TCObjectSmartPointer&   operator=(const TCObjectSmartPointer< _A_ >& pRight)
		{
			if( m_pPtr == pRight.GetPointer() )
				return *this;

			xst_release( this->m_pPtr );
			this->m_pPtr = (_T_*)pRight.GetPointer();
			if( this->m_pPtr )
				this->m_pPtr->AddRef();
			return *this;
		}

		xst_fi TCObjectSmartPointer&   operator=(TCObjectSmartPointer&& pRight)
		{
			if( m_pPtr == pRight.m_pPtr )
				return *this;

			xst_release( this->m_pPtr );
			this->m_pPtr = pRight.m_pPtr;
			if( this->m_pPtr )
				this->m_pPtr->AddRef();
			pRight.Release();
			return *this;
		}

		template<class _A_>
		xst_fi TCObjectSmartPointer&   operator=(TCObjectSmartPointer< _A_ >&& pRight)
		{
			if( m_pPtr == pRight.GetPointer() )
				return *this;

			xst_release( this->m_pPtr );
			this->m_pPtr = (_T_*)pRight.GetPointer();
			if( this->m_pPtr )
				this->m_pPtr->AddRef();
			pRight.Release();
			return *this;
		}

		xst_fi TCObjectSmartPointer&   operator=(_T_* pRight)
		{
			if( m_pPtr == pRight )
				return *this;
			xst_release( this->m_pPtr );
			this->m_pPtr = pRight;
			if( this->m_pPtr )
				this->m_pPtr->AddRef();
			return *this;
		}

		xst_fi const bool	operator==(const TCObjectSmartPointer& pRight) const
		{
			return this->m_pPtr == pRight.m_pPtr;
		}

		xst_fi const bool	operator!=(const TCObjectSmartPointer& pRight) const
		{
			return this->m_pPtr != pRight.m_pPtr;
		}

		xst_fi const bool	operator<=(const TCObjectSmartPointer& pRight) const
		{
			return this->m_pPtr <= pRight.m_pPtr;
		}

		xst_fi const bool	operator>=(const TCObjectSmartPointer& pRight) const
		{
			return this->m_pPtr >= pRight.m_pPtr;
		}

		xst_fi const bool	operator<(const TCObjectSmartPointer& pRight) const
		{
			return this->m_pPtr < pRight.m_pPtr;
		}

		xst_fi const bool	operator>(const TCObjectSmartPointer& pRight) const
		{
			return this->m_pPtr > pRight.m_pPtr;
		}

		xst_fi _T_*        GetPointer() const
        {
            return m_pPtr;
        }

		xst_fi operator bool() const
		{
			return m_pPtr != xst_null;
		}

		xst_fi _T_*	operator->()
        {
            return m_pPtr;
        }

		xst_fi const _T_*	operator->() const
		{
			return m_pPtr;
		}

		xst_fi bool IsNull() const
		{
			return m_pPtr == xst_null;
		}

		xst_fi void operator delete(void* pPtr)
		{
			/*TCObjectSmartPointer< _T_ >* pSPtr = (TCObjectSmartPointer< _T_ >*)pPtr;
			*pSPtr.Release();*/
		}

    protected:

        _T_*    m_pPtr = xst_null;

    };

	#define XST_OBJSMART_PTR( _type ) \
		XST_TEMPLATE_CLASS XST::TCObjectSmartPointer< _type >; \
		XST::TCObjectSmartPointer< _type >

	#define XST_TYPEDEF_OBJSMART_PTR( _type ) \
		XST_TEMPLATE_CLASS XST::TCObjectSmartPointer< _type >; \
		typedef XST::TCObjectSmartPointer< _type >

	template<class _T_>
    class TCSharedSmartPointer 
    {
        public:

						TCSharedSmartPointer() : m_pPtr( 0 ), m_piRefCounter(0) 
						{
						}

			explicit	TCSharedSmartPointer(_T_* pPtr) : m_pPtr( pPtr ), m_piRefCounter( xst_new i32( 1 ) ) 
						{
						}

						TCSharedSmartPointer(const TCSharedSmartPointer& pRight) : m_pPtr( 0 ), m_piRefCounter( 0 )
                        {
							m_pPtr = pRight.m_pPtr;
							m_piRefCounter = pRight.m_piRefCounter;
							if( m_piRefCounter )
							{
								++( *m_piRefCounter );
							}
                        }

		virtual		    ~TCSharedSmartPointer()
		{
			Release();
		}


		TCSharedSmartPointer&   operator=(const TCSharedSmartPointer& pRight)
		{
			if( m_pPtr == pRight.m_pPtr )
			{
				return *this;
			}
	
			Release();

			m_pPtr = pRight.m_pPtr;
			m_piRefCounter = pRight.m_piRefCounter;

			if( m_piRefCounter )
			{
				++( *m_piRefCounter );
			}

			return *this;
		}

		xst_fi i32 Release()
		{
			xst_assert( m_piRefCounter, "Release(): Reference counter is not created" );
			if( --( *m_piRefCounter ) <= 0 )
			{
				delete m_pPtr;
				m_pPtr = 0;
				delete m_piRefCounter;
				m_piRefCounter = 0;
				return RESULT::OK;
			}

			return RESULT::FAILED;
		}

		TCSharedSmartPointer&   operator=(_T_* pRight)
		{
			if( m_pPtr == pRight )
			{
				return *this;
			}

			Release();

			m_pPtr = pRight;
			m_piRefCounter = pRight.m_piRefCounter;

			if( m_piRefCounter )
			{
				(*m_piRefCounter) = 1;
			}

			return *this;
		}

		_T_*        GetPointer() const
        {
            return m_pPtr;
        }

		xst_fi operator bool()
		{
			return m_pPtr != xst_null;
		}

		_T_*	operator->()
        {
            return m_pPtr;
        }

		xst_fi bool IsNull()
		{
			return m_pPtr == xst_null;
		}

    protected:

        _T_*    m_pPtr;
		i32*	m_piRefCounter;

    };

}//XSt

#endif
