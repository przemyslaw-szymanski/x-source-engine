#ifndef XSE_MEMORY_ALLOCATORS_H
#define XSE_MEMORY_ALLOCATORS_H

#include "XSECommon.h"

namespace XSE
{
	class LuaScriptAllocator
	{
		public:

			static	void*				Allocate(size_t uiSize);

			static	void*				Allocate(size_t uiSize, const char* lpszFile, u32 uiLine);

			static	void*				Allocate(size_t uiSize, u32 uiBlock, const char* lpszFile, u32 uiLine);

			static	void*				Allocate(size_t uiSize, void* pPtr);

			static	void*				Allocate(size_t uiSize, const std::nothrow_t& nt);

			static	void				Deallocate(void* pPtr);
	};

}//xse

#endif