#ifndef XST_IALLOCABLE_OBJECT_H
#define XST_IALLOCABLE_OBJECT_H

namespace XST
{
	template<class _ALLOC_>
	class XST_API TIAllocableObject
	{
		public:

			virtual		~TIAllocableObject() {}

			//Standard new
			void*	operator new(size_t uiSize)
			{
				return _ALLOC_::Allocate( uiSize );
			}

			//Debug new
			void*	operator new(size_t uiSize, const char* lpszFile, u32 uiLine)
			{
				return _ALLOC_::Allocate( uiSize, lpszFile, uiLine );
			}

			//Visual Studio debug new
			void*	operator new(size_t uiSize, u32 uiBlock, const char* lpszFile, u32 uiLine)
			{
				return _ALLOC_::Allocate( uiSize, uiBlock, lpszFile, uiLine );
			}

			//Placement new
			void*	operator new(size_t uiSize, void* pPtr)
			{
				return _ALLOC_::Allocate( uiSize, pPtr );
			}

			//Nothrow new
			void*	operator new(size_t uiSize, const std::nothrow_t& nt)
			{
				return _ALLOC_::Allocate( uiSize, nt );
			}

			void	operator delete(void* pPtr)
			{
				return _ALLOC_::Deallocate( pPtr );
			}
	};

}//xst

#endif