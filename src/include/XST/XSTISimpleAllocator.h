#ifndef XST_ISIMPLE_ALLOCATOR_H
#define XST_ISIMPLE_ALLOCATOR_H

#include "XSTCommon.h"

namespace XST
{
	class ISimpleAllocator
	{
		public:

			XST_DECLPV(void*, Allocate(u32 _uiSize));
			XST_DECLPV(void, Deallocate(void* _pPtr));
	};
}

#endif