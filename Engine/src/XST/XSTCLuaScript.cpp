#include "../../include/XST/XSTCLuaScript.h"
#include "../../include/XST/XSTCLuaScriptManager.h"

namespace XST
{

	void*	CLuaScriptAllocator::Allocate(size_t uiSize)
	{
		if( CLuaScriptManager::IsSingletonCreated() )
		{
			return CLuaScriptManager::GetSingletonPtr()->_GetAllocator()->Allocate( uiSize );
		}

		return xst_malloc( uiSize );
	}

	void*	CLuaScriptAllocator::Allocate(size_t uiSize, const char* lpszFile, u32 uiLine)
	{
		if( CLuaScriptManager::IsSingletonCreated() )
		{
			return CLuaScriptManager::GetSingletonPtr()->_GetAllocator()->Allocate( uiSize );
		}

		return xst_malloc( uiSize );
	}

	void*	CLuaScriptAllocator::Allocate(size_t uiSize, u32 uiBlock, const char* lpszFile, u32 uiLine)
	{
		if( CLuaScriptManager::IsSingletonCreated() )
		{
			return CLuaScriptManager::GetSingletonPtr()->_GetAllocator()->Allocate( uiSize );
		}

		return xst_malloc( uiSize );
	}

	void*	CLuaScriptAllocator::Allocate(size_t uiSize, void* pPtr)
	{
		if( CLuaScriptManager::IsSingletonCreated() )
		{
			return CLuaScriptManager::GetSingletonPtr()->_GetAllocator()->Allocate( uiSize );
		}

		return xst_malloc( uiSize );
	}

	void*	CLuaScriptAllocator::Allocate(size_t uiSize, const std::nothrow_t &nt)
	{
		if( CLuaScriptManager::IsSingletonCreated() )
		{
			return CLuaScriptManager::GetSingletonPtr()->_GetAllocator()->Allocate( uiSize );
		}

		return xst_malloc( uiSize );
	}

	void	CLuaScriptAllocator::Deallocate(void* pPtr)
	{
		if( CLuaScriptManager::IsSingletonCreated() )
		{
			return CLuaScriptManager::GetSingletonPtr()->_GetAllocator()->Deallocate( pPtr );
		}

		return xst_free( pPtr );
	}

	namespace Resources
	{

		CLuaScript::CLuaScript(CLuaApi* pApi, XST_IRESOURCE_DECL_PARAMS) : m_pApi( pApi ), XST_IRESOURCE_CTOR
		{
		}

		CLuaScript::~CLuaScript()
		{
			Unload();
		}

		i32 CLuaScript::Load(xst_castring& strFileName)
		{
			return RESULT::OK;
		}

		i32 CLuaScript::Load(xst_castring& strFileName, xst_castring& strGroupName)
		{
			return RESULT::OK;
		}

		void CLuaScript::Unload()
		{
		}

	}//resources
}//xst