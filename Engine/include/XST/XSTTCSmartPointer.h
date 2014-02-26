#ifndef XST_CSMART_POINTER_H
#define XST_CSMART_POINTER_H

#include "XSTCommonInclude.h"
#include "XSTCommonTypes.h"
#include "XSTIObject.h"

namespace XST
{
    template< class _T_ > class TCObjectSmartPointer;

	template<class _T_>
	class TCWeakPointer
	{
        friend class TCObjectSmartPointer< _T_ >;

		public:

            xst_fi      TCWeakPointer()
                        {}

			explicit
            xst_fi      TCWeakPointer(_T_* pPtr) : m_pPtr( pPtr )
                        {}

			xst_fi      TCWeakPointer(const TCWeakPointer& Ptr) : TCWeakPointer( Ptr.m_pPtr )
                        {}

            template< class _U_ >
            xst_fi      TCWeakPointer(const TCWeakPointer< _U_ >& Ptr) : TCWeakPointer( static_cast< _T_* >( Ptr.GetPtr() ) )
                        {}

            template< class _U_ >
            xst_fi      TCWeakPointer(const TCObjectSmartPointer< _U_ >& Ptr) : TCWeakPointer( static_cast< _T_* >( Ptr.GetPtr() ) )
                        {}

			virtual
            xst_fi      ~TCWeakPointer() 
						{
							m_pPtr = xst_null;
						}

            xst_fi TCWeakPointer&   operator=(const TCWeakPointer& Right)
		    {
                m_pPtr = Right.m_pPtr;
			    return *this;
		    }

		    template<class _U_>
		    xst_fi TCWeakPointer&   operator=(const TCWeakPointer< _U_ >& Right)
		    {
                m_pPtr = static_cast< _T_* >( Right.GetPtr() );
			    return *this;
		    }

		    template<class _U_>
		    xst_fi TCWeakPointer&   operator=(const TCObjectSmartPointer< _U_ >& Right)
		    {
                m_pPtr = static_cast< _T_* >( Right.GetPtr() );
			    return *this;
		    }

		    xst_fi TCWeakPointer&   operator=(_T_* pRight)
		    {
                m_pPtr = pRight;
			    return *this;
		    }

		    xst_fi bool	operator==(const TCWeakPointer& pRight) const
		    {
			    return this->m_pPtr == pRight.m_pPtr;
		    }

		    xst_fi bool	operator!=(const TCWeakPointer& pRight) const
		    {
			    return this->m_pPtr != pRight.m_pPtr;
		    }

            xst_fi bool	operator==(const _T_* pRight) const
		    {
			    return this->m_pPtr == pRight;
		    }

		    xst_fi bool	operator!=(const _T_* pRight) const
		    {
			    return this->m_pPtr != pRight;
		    }

		    xst_fi _T_* GetPtr() const
            {
                return m_pPtr;
            }

		    /*xst_fi operator bool() const
		    {
			    return m_pPtr != xst_null;
		    }*/

            xst_fi bool operator!() const
            { return !m_pPtr; }

            xst_fi bool operator<(const TCWeakPointer& Right) const
            { return m_pPtr < Right.m_pPtr; }

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

			xst_fi bool IsValid() const
			{ return m_pPtr != xst_null; }

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
        friend class TCWeakPointer< _T_ >;

        public:

			xst_fi TCObjectSmartPointer() 
			{}

		    explicit xst_fi TCObjectSmartPointer(_T_* pPtr) : m_pPtr( pPtr )
			{}

			xst_fi TCObjectSmartPointer(const TCObjectSmartPointer& Right)
            {
                _BaseSetPtr( Right.m_pPtr );
            }

		    template<class _U_>
		    xst_fi TCObjectSmartPointer(const TCObjectSmartPointer< _U_ >& Right) 
		    {
                _BaseSetPtr( static_cast< _T_* >( Right.GetPtr() ) );
            }

            template<class _U_>
		    xst_fi TCObjectSmartPointer(const TCWeakPointer< _U_ >& Right) 
		    {
                _BaseSetPtr( static_cast< _T_* >( Right.GetPtr() ) );
            }

		    virtual	xst_fi  ~TCObjectSmartPointer()
		    {
                Release();
		    }

		    xst_fi	void Release()
		    {
			    xst_release( m_pPtr );
		    }

		    xst_fi TCObjectSmartPointer&   operator=(const TCObjectSmartPointer& Right)
		    {
                _SetPtr( Right.m_pPtr );
			    return *this;
		    }

		    template<class _U_>
		    xst_fi TCObjectSmartPointer&   operator=(const TCObjectSmartPointer< _U_ >& Right)
		    {
                _SetPtr( static_cast< _T_* >( Right.GetPtr() ) );
			    return *this;
		    }

		    template<class _U_>
		    xst_fi TCObjectSmartPointer&   operator=(const TCWeakPointer< _U_ >& Right)
		    {
                _SetPtr( static_cast< _T_* >( Right.GetPtr() ) );
			    return *this;
		    }

		    xst_fi TCObjectSmartPointer&   operator=(_T_* pRight)
		    {
                _SetPtr( pRight );
			    return *this;
		    }

			template< class _U_ >
		    xst_fi bool	operator==(const TCObjectSmartPointer< _U_ >& Right) const
		    {
			    return this->m_pPtr == Right.GetPtr();
		    }

			template< class _U_ >
		    xst_fi bool	operator!=(const TCObjectSmartPointer< _U_ >& Right) const
		    {
			    return this->m_pPtr != pRight.GetPtr();
		    }

			template< class _U_ >
            xst_fi bool	operator==(_U_* pRight) const
		    {
			    return this->m_pPtr == pRight;
		    }

			template< class _U_ >
		    xst_fi bool	operator!=(_U_* pRight) const
		    {
			    return this->m_pPtr != pRight;
		    }

			template< class _U_ >
            xst_fi bool	operator==(const _U_& pRight) const
		    {
			    return this->m_pPtr == pRight;
		    }

			template< class _U_ >
		    xst_fi bool	operator!=(const _U_& pRight) const
		    {
			    return this->m_pPtr != pRight;
		    }

		    /*xst_fi operator bool() const
		    {
			    return m_pPtr != xst_null;
		    }*/

            xst_fi bool operator!() const
            { return !m_pPtr; }

			template< class _U_ >
            xst_fi bool operator<(const TCObjectSmartPointer< _U_ >& Right) const
            { return m_pPtr < Right.GetPtr(); }

		    xst_fi _T_*	operator->()
            { return m_pPtr; }

		    xst_fi const _T_* operator->() const
		    { return m_pPtr; }

			xst_fi _T_* GetPtr() const
            { return m_pPtr; }

		    xst_fi bool IsNull() const
		    { return m_pPtr == xst_null; }

			xst_fi bool IsValid() const
			{ return m_pPtr != xst_null; }

    protected:

            xst_fi void _BaseSetPtr(_T_* pPtr)
            {
                m_pPtr = pPtr;
                if( m_pPtr )
                    m_pPtr->AddRef();
            }

            xst_fi void _SetPtr(_T_* pPtr)
            {
                if( m_pPtr != pPtr )
                {
                    if( m_pPtr )
                        m_pPtr->Release();
                    m_pPtr = pPtr;
                    if( m_pPtr )
                        m_pPtr->AddRef();
                }
            }

    protected:

        _T_*    m_pPtr = xst_null;

    };

	/*template< class _T_ >
    xst_fi operator bool(const TCObjectSmartPointer< _T_ >& Ptr)
    { return Ptr.GetPtr() != xst_null; }

    template< class _T_ >
    xst_fi operator size_t(const TCObjectSmartPointer< _T_ >& Ptr)
    { return Ptr.GetPtr(); }*/

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
