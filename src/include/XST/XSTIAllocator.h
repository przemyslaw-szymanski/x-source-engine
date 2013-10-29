#ifndef XST_IALLOCATOR_H
#define XST_IALLOCATOR_H

#include "XSTCommon.h"

namespace XST
{
	class IAllocator
	{
		public:

			virtual			~IAllocator() {}

			virtual	bool	AllocatePool(ul32 ulBytes) { return true; }
			virtual	bool	AllocatePool(ul32 ulChunkSize, ul32 ulChunkCount) { return true; }

			virtual void*	Allocate() { return 0; }
			virtual void*	Allocate(u32 _uiSize) { return 0; }
			virtual void*	Allocate(void* _pData) { return 0; }
			virtual void*	Allocate(void* _pData, u32 _uiDataSize) { return 0; }
			virtual void*	Allocate(u32 _uiElementCount, u32 _uiElementSize) { return 0; }

			virtual void	Deallocate(void* _pPtr) { return; }
			virtual void	Deallocate(void** _pPtr) { return; }
			virtual void	Deallocate(void* _pPtr, u32 _uiSize) { return; }
			virtual void	Deallocate(void* _pPtr, u32 _uiElementCount, u32 _uiElementSize) { return; }
	};

	class CDefaultAllocator : public IAllocator
	{
		public:

			virtual			~CDefaultAllocator() {}

			virtual void*	Allocate() { return 0; }
			virtual void*	Allocate(u32 _uiSize) { return xst_malloc(_uiSize); }
			virtual void*	Allocate(void* _pData) { return xst_malloc( (*(u32*)_pData) ); }
			virtual void*	Allocate(void* _pData, u32 _uiDataSize) { return xst_malloc( (*(u32*)_pData) ); }
			virtual void*	Allocate(u32 _uiElementCount, u32 _uiElementSize) { return xst_calloc(_uiElementCount, _uiElementSize); }

			virtual void	Deallocate(void* _pPtr) { xst_free(_pPtr); }
			virtual void	Deallocate(void* _pPtr, u32 _uiSize) { xst_free(_pPtr); }
			virtual void	Deallocate(void* _pPtr, u32 _uiElementCount, u32 _uiElementSize) { xst_free(_pPtr); }
	};

	
}//XST

#endif//XST_IALLOCATOR_H
