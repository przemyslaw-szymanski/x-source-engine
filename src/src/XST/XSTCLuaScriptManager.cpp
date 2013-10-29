#include "../../include/XST/XSTCLuaScriptManager.h"
#include "../../include/XST/XSTCFileManager.h"
#include "../../include/XST/XSTCLuaApi.h"

namespace XST
{

	CLuaScriptManager::CLuaScriptManager() : m_pDefaultLuaApi( xst_null )
	{
		//Create allocator
		m_pScriptAllocator = xst_new TCFreeListMemoryManager< Resources::CLuaScript >( 100 );
	}

	CLuaScriptManager::~CLuaScriptManager()
	{
		xst_delete( m_pScriptAllocator );

		_GroupIterator Itr;
		for(Itr = this->m_mResources.begin(); Itr != this->m_mResources.end(); ++Itr)
		{

		}

		xst_delete( m_pDefaultLuaApi );
	}

	CLuaScriptManager::GroupPtr	CLuaScriptManager::CreateGroup(xst_castring& strGroupName, CLuaApi* pApi)
	{
		GroupPtr pGr = this->GetOrCreateGroup( strGroupName );
		pGr->SetUserData( pApi );
		return pGr;
	}

	LuaScriptPtr CLuaScriptManager::CreateScript(xst_castring& strScriptName, xst_castring& strGroupName)
	{
		Resources::CLuaScript* pBaseScript = xst_null;
		GroupPtr pGr = this->GetOrCreateGroup( strGroupName );
		CLuaApi* pApi = (CLuaApi*)pGr->GetUserData();
		if( !pApi ) pApi = m_pDefaultLuaApi;

        CLuaScriptManager* p = this;
		IBaseResourceManager* pCreator = (IBaseResourceManager*)((BaseLuaScriptManager*)p);
		pBaseScript = xst_new Resources::CLuaScript( pApi, strScriptName, ResourceType::LUA_SCRIPT, ResourceStates::CREATED, this->m_pScriptAllocator );
		if( !pBaseScript )
		{
			return LuaScriptPtr( xst_null );
		}

		LuaScriptPtr pScript( pBaseScript );
		if( !pGr->AddResource( strScriptName, pScript ) )
		{
			return LuaScriptPtr( xst_null );
		}

		/*pBaseScript->m_strResourceName = strScriptName;
		pBaseScript->m_iResourceState = ResourceStates::CREATED;
		pBaseScript->m_iResourceType = ResourceType::LUA_SCRIPT;
		CLuaScriptManager* p = this;
		pBaseScript->m_pResourceCreator = (IBaseResourceManager*)(TCResourceManager< LuaScriptPtr >*)p;
		pBaseScript->m_pResourceAllocator = this->m_pScriptAllocator;*/

		return pScript;
	}

	LuaScriptPtr CLuaScriptManager::LoadScript(xst_castring& strScriptName, xst_castring& strGroupName)
	{
		LuaScriptPtr pScript = CreateScript( strScriptName, strGroupName );
		if( pScript.IsNull() )
		{
			return pScript;
		}

		FilePtr pFile = CFileManager::GetSingletonPtr()->LoadFile( strScriptName, strGroupName );
		if( pFile.IsNull() )
		{
			//Destroy script
			this->RemoveResource( strScriptName );
			return LuaScriptPtr( xst_null );
		}

		//pScript->m_pResourceFile = pFile;
		pScript->_SetFile( pFile );
		if( pScript->GetApi()->LoadScript( pFile->GetData().GetData(), pFile->GetData().GetSize() ) != RESULT::OK )
		{
			//Destroy script
			this->RemoveResource( strScriptName );
			return LuaScriptPtr( xst_null );
		}

		return pScript;
	}

	void CLuaScriptManager::UnloadScript(xst_castring& strScriptName)
	{
	}

	void CLuaScriptManager::UnloadScript(cu32& uiScriptHandle)
	{
	}

	void CLuaScriptManager::UnloadScript(LuaScriptPtr pScript)
	{
	}

}//xst
