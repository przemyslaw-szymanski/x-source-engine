#include "../../include/XST/XSTCLuaApi.h"
#include "../../include/XST/XSTPlatform.h"
#include "../../include/XST/XSTCLogger.h"

namespace XST
{
	#define XST_LFLOAD(_pfnPtrName, _pfnName, _luaFName) \
		this->_pfnName = (_pfnPtrName)CPlatform::GetProcAddress( m_hLuaLib, XST_TOSTRING( _luaFName ) ); \
		if( !this->_pfnName ) m_bLoaded = false;

	CLuaApi::CLuaApi() : m_bLoaded( false ), m_bCreated( false ), m_pState( xst_null )
	{
		Create();
		this->m_pLuaState = m_pState;
	}

	CLuaApi::~CLuaApi()
	{
		lua_close( m_pState );
		m_pState = xst_null;
	}

	void CLuaApi::Destroy()
	{
		if( m_pState != xst_null )
		{
			lua_close( m_pState );
			m_pState = xst_null;
		}

		m_bLoaded = m_bCreated = false;
	}

	i32 CLuaApi::Create()
	{
		if( !m_bCreated || m_pState == xst_null )
		{
			m_bLoaded = true;
			Destroy();
			m_pState = lua_open();
			if( m_pState )
			{
				m_bCreated = true;
			}
			else
			{
				XST_LOG_ERR( "Create lua state failed" );
				return RESULT::FAILED;
			}
		}

		return RESULT::OK;
	}

	i32 CLuaApi::LoadScript(XST::Types::cu8 *pScriptData, XST::Types::cul32 &ulScriptSize)
	{
		return RESULT::OK;
	}

	bool CLuaApi::GetTable(XST::Types::lpcastr lpszTableName)
	{
		GetGlobal( lpszTableName );
		return IsTable( -1 ) == 1;
	}

	bool CLuaApi::GetFunction(lpcastr lpszFuncName)
	{
		GetGlobal( lpszFuncName );
		return IsFunction( - 1 ) == 1;
	}

}//xst