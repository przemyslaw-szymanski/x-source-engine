#ifndef XST_CLUA_SCRIPT_MANAGER_H
#define XST_CLUA_SCRIPT_MANAGER_H

#include "XSTCLuaScript.h"
#include "XSTTCSingleton.h"
#include "XSTTCResourceManager.h"
#include "XSTTCMemoryManager.h"

namespace XST
{
	class CLuaScriptAllocator;
	class CLuaApi;

	typedef TCResourceManager< LuaScriptPtr > BaseLuaScriptManager;

	class XST_API CLuaScriptManager : public TCSingleton< CLuaScriptManager >, public BaseLuaScriptManager
	{
		friend class CLuaScriptAllocator;

		public:

			typedef TCResourceManager< LuaScriptPtr >	BaseManager;
			//typedef BaseManager::GroupIterator			GroupIterator;
			typedef BaseManager::ResourceIterator		ResourceIterator;
			typedef BaseManager::GroupPtr				GroupPtr;

		public:

									CLuaScriptManager();
			virtual					~CLuaScriptManager();

					GroupPtr		CreateGroup(xst_castring& strGroupName, CLuaApi* pApi);

					LuaScriptPtr	CreateScript(xst_castring& strScriptName, xst_castring& strGroupName);

					LuaScriptPtr	LoadScript(xst_castring& strScriptName, xst_castring& strGroupName);

					void			UnloadScript(xst_castring& strScriptName);

					void			UnloadScript(cu32& uiScriptHandle);

					void			UnloadScript(LuaScriptPtr pScript);

			const	IAllocator*		GetAllocator() const
									{ return m_pScriptAllocator; }

		protected:

					IAllocator*		_GetAllocator()
									{ return m_pScriptAllocator; }

		protected:

					IAllocator*		m_pScriptAllocator;
					CLuaApi*		m_pDefaultLuaApi;
	};

	

}//xst

#endif