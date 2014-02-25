#include "../include/XSECLuaScriptManager.h"
#include "XSECLuaScript.h"

namespace XSE
{
	CLuaScriptManager::CLuaScriptManager()
	{
		m_pDefaultApi = xst_new XST::CLuaApi();
		m_pDefaultApi->OpenLibs();
	}

	CLuaScriptManager::~CLuaScriptManager()
	{
		xst_delete( m_pDefaultApi );
	}

	i32 CLuaScriptManager::_CreateMemoryPool(cul32 &ulObjCount, XST::IAllocator *pAllocator)
	{
		if( pAllocator )
		{
			xst_delete( this->m_pMemoryMgr );
			this->m_pMemoryMgr = pAllocator;
			if( !this->m_pMemoryMgr->AllocatePool( sizeof( Resources::CLuaScript ), ulObjCount ) )
			{
				XST_LOG_ERR( "Create memory pool failed in CLuaScriptManager" );
				return RESULT::FAILED;
			}
		}
		else
		{
			xst_delete( this->m_pMemoryMgr );
			this->m_pMemoryMgr = xst_new XST::TCFreeListMemoryManager< Resources::CLuaScript >( ulObjCount );
		}

		return RESULT::OK;
	}

	i32	CLuaScriptManager::PrepareResource(ResourcePtr pRes)
	{
		//Compile script
		LuaScriptPtr pScript = pRes;
		return pScript->Compile();
	}

	Resources::IResource* CLuaScriptManager::_CreateResource(xst_castring &strName, const IResourceManager::ResourceHandle& ulHandle, XSE::IResourceManager::GroupWeakPtr pGroup)
	{
		Resources::CLuaScript* pScript = xst_new Resources::CLuaScript( m_pDefaultApi, this, ulHandle, strName, XST::ResourceType::LUA_SCRIPT, XST::ResourceStates::CREATED, this->m_pMemoryMgr );
		if( pScript == xst_null )
		{
			XST_LOG_ERR( "Create script: " << strName << " failed. Not enough memory" );
			return xst_null;
		}

		pScript->m_iResourceState = XST::ResourceStates::CREATED;

		return pScript;
	}

	LuaScriptPtr CLuaScriptManager::CreateScript(xst_castring& strName, xst_castring& strGroupName, XST::CLuaApi* pApi)
	{
		ResourcePtr pRes = this->CreateResource( strName, strGroupName );
		if( pRes.IsNull() )
		{
			return LuaScriptPtr();
		}

		LuaScriptPtr pScript( pRes );
		pScript->m_pApi = ( pApi != xst_null )? pApi : m_pDefaultApi;

		return pScript;
	}

	LuaScriptPtr CLuaScriptManager::LoadScript(xst_castring &strName, xst_castring &strGroupName, XST::CLuaApi* pApi)
	{
		ResourcePtr pRes = this->LoadResource( strName, strGroupName );
		if( pRes.IsNull() )
		{
			return LuaScriptPtr();
		}

		LuaScriptPtr pScript( pRes );
		pScript->m_pApi = ( pApi != xst_null )? pApi : m_pDefaultApi;
		if( pScript->Load() != RESULT::OK || pScript->Compile() != RESULT::OK )
		{
			XST_LOG_ERR( pScript->GetApi().GetLastError() );
			this->DestroyResource( pRes );
			pScript = xst_null;
			return LuaScriptPtr();
		}

		pScript->m_iResourceState = XST::ResourceStates::LOADED;

		return pScript;
	}

}//xse