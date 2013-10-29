#include "../include/XSEMemoryAllocators.h"
#include "../include/XSECLuaScript.h"

namespace XSE
{
	typedef XST::TCFreeListMemoryManager< Resources::CLuaScript > LuaScriptManager;

	void* LuaScriptAllocator::Allocate(size_t uiSize)
	{
		if( LuaScriptManager::IsSingletonCreated() )
		{
			return LuaScriptManager::GetSingletonPtr()->Allocate( uiSize );
		}

		return xst_malloc( uiSize );
	}

	void*	LuaScriptAllocator::Allocate(size_t uiSize, const char* lpszFile, u32 uiLine)
	{
		if( LuaScriptManager::IsSingletonCreated() )
		{
			return LuaScriptManager::GetSingletonPtr()->Allocate( uiSize );
		}

		return xst_malloc( uiSize );
	}

	void*	LuaScriptAllocator::Allocate(size_t uiSize, u32 uiBlock, const char* lpszFile, u32 uiLine)
	{
		if( LuaScriptManager::IsSingletonCreated() )
		{
			return LuaScriptManager::GetSingletonPtr()->Allocate( uiSize );
		}

		return xst_malloc( uiSize );
	}

	void*	LuaScriptAllocator::Allocate(size_t uiSize, void* pPtr)
	{
		if( LuaScriptManager::IsSingletonCreated() )
		{
			return LuaScriptManager::GetSingletonPtr()->Allocate( uiSize );
		}

		return xst_malloc( uiSize );
	}

	void*	LuaScriptAllocator::Allocate(size_t uiSize, const std::nothrow_t& nt)
	{
		if( LuaScriptManager::IsSingletonCreated() )
		{
			return LuaScriptManager::GetSingletonPtr()->Allocate( uiSize );
		}

		return xst_malloc( uiSize );
	}

	void	LuaScriptAllocator::Deallocate(void* pPtr)
	{
		if( LuaScriptManager::IsSingletonCreated() )
		{
			return LuaScriptManager::GetSingletonPtr()->Deallocate( pPtr );
		}

		return xst_free( pPtr );
	}

}//xse