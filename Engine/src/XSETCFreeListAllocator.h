#ifndef XSE_TCFREELIST_ALLOCATOR_H
#define XSE_TCFREELIST_ALLOCATOR_H

#include "XSTTCMemoryManager.h"

namespace XSE
{
	template< class _T_ >
	class TCFreeListAllocator
	{
		public:

			virtual			~TCFreeListAllocator()
			{}

			static int		Create(unsigned uCount);
			static void		Destroy();

			xst_fi static 
			void*			Allocate()
							{ return m_pMemMgr->Allocate(); }
			xst_fi static 
			void*			Allocate(const std::size_t& uiSize)
							{ return m_pMemMgr->Allocate( uiSize ); }
			xst_fi static 
			void*			Allocate(const std::size_t& uiSize, const char* lpszFile, unsigned uiLine)
							{ return m_pMemMgr->Allocate( uiSize, lpszFile, uiLine ); }
			xst_fi static 
			void*			Allocate(const std::size_t& uiSize, u32 uiBlock, const char* lpszFile, unsigned uiLine)
							{ return m_pMemMgr->Allocate( uiSize, uiBlock, lpszFile, uiLine ); }
			xst_fi static 
			void*			Allocate(const std::size_t& uiSize, void* pPtr)
							{ return m_pMemMgr->Allocate( uiSize, pPtr ); }
			xst_fi static 
			void*			Allocate(const std::size_t& uiSize, const std::nothrow_t& nt)
							{ return m_pMemMgr->Allocate( uiSize ); }

			xst_fi static
			void			Deallocate(void* pPtr)
							{ return m_pMemMgr->Deallocate( pPtr ); }

		protected:

			static XST::TCFreeListMemoryManager< _T_ >*	m_pMemMgr;
	};

	template< class _T_ >
	XST::TCFreeListMemoryManager< _T_ >* TCFreeListAllocator< _T_ >::m_pMemMgr = xst_null;

	template< class _T_ >
	int TCFreeListAllocator< _T_ >::Create(unsigned uCount)
	{
		xst_assert( m_pMemMgr == xst_null, "(TCFreeListAllocator::Init)" );
		m_pMemMgr = xst_new XST::TCFreeListMemoryManager< _T_ >( uCount );
		if( !m_pMemMgr )
		{
			return XST_FAIL;
		}

		return XST_OK;
	}

	template< class _T_ >
	void TCFreeListAllocator< _T_ >::Destroy()
	{
		xst_delete( m_pMemMgr );
	}

	template< class _T_ >
	class TCDefaultAllocator
	{
		public:

			virtual			~TCDefaultAllocator()
			{}

			static int		Create( unsigned uCount )
							{ return XST_OK; }

			static void		Destroy()
			{}

			xst_fi static 
			void*			Allocate()
							{ return xst_malloc( sizeof( _T_ ) ); }
			xst_fi static 
			void*			Allocate(const std::size_t& uiSize)
							{ return xst_malloc( uiSize ); }
			xst_fi static 
			void*			Allocate(const std::size_t& uiSize, const char* lpszFile, unsigned uiLine)
							{ return xst_malloc( uiSize ); }
			xst_fi static 
			void*			Allocate(const std::size_t& uiSize, u32 uiBlock, const char* lpszFile, unsigned uiLine)
							{ return xst_malloc( uiSize ); }
			xst_fi static 
			void*			Allocate(const std::size_t& uiSize, void* pPtr)
							{ return xst_malloc( uiSize ); }
			xst_fi static 
			void*			Allocate(const std::size_t& uiSize, const std::nothrow_t& nt)
							{ return xst_malloc( uiSize ); }

			xst_fi static
			void			Deallocate(void* pPtr)
							{ return xst_free( pPtr ); }

	};

} // XSE

#endif // XSE_TCFREELIST_ALLOCATOR_H