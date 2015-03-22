#ifndef XSE_CLUASCRIPTMANAGER_H
#define XSE_CLUASCRIPTMANAGER_H

#include "XSECLuaScript.h"
#include "XSEIResourceManager.h"

namespace XSE
{
	class XST_API CLuaScriptManager : public IResourceManager, public XST::TCSingleton< CLuaScriptManager >
	{
		public:

											CLuaScriptManager();
			virtual							~CLuaScriptManager();

					LuaScriptPtr			CreateScript(xst_castring& strName, xst_castring& strGroupName, XST::CLuaApi* pApi = xst_null);

					LuaScriptPtr			LoadScript(xst_castring& strName, xst_castring& strGroupName, XST::CLuaApi* pApi = xst_null);

					i32						PrepareResource(ResourceWeakPtr *const ppRes) xst_implement;

		protected:

			virtual	Resources::IResource*	_CreateResource(xst_castring& strName, const ResourceHandle& ulHandle, GroupWeakPtr pGroup) xst_implement;
			//virtual	i32						_CreateMemoryPool(cul32& ulObjCount, XST::IAllocator* pAllocator = xst_null);

		protected:

			XST::CLuaApi*		m_pDefaultApi;
	};

}//xse

#endif