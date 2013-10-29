#include "../../include/XST/XSTCLuaCall.h"
#include "../../include/XST/XSTCStringUtil.h"

namespace XST
{
		bool CLuaCall::_GetFunction(xst_castring& _strName)
        {
                //Split
                m_vTokens.clear();
                XST::StringUtil::Split( _strName, &m_vTokens, ".:" );
                if( m_vTokens.size() > 1 )
                {
                        lua_getglobal( m_pLuaState, m_vTokens[0].c_str() );
                        if( !lua_istable( m_pLuaState, -1 ) )
                        {
                                m_strLastError = m_vTokens[0] + " is not a table.";
                                XST_LOG_ERR( m_strLastError );
                                return false;
                        }

                        for(u32 i = 1; i < m_vTokens.size() - 1; ++i)
                        {
                                lua_pushstring( m_pLuaState, m_vTokens[i].data() );
                                lua_gettable( m_pLuaState, -2 );
                                if( !lua_istable( m_pLuaState, -1 ) )
                                {
                                        m_strLastError = m_vTokens[i] + " is not a table.";
                                        XST_LOG_ERR( m_strLastError );
                                        return false;
                                }
                        }

                        lua_pushstring( m_pLuaState, m_vTokens.back().data() );
                        lua_gettable( m_pLuaState, -2 );
                        if( !lua_isfunction( m_pLuaState, -1 ) )
                        {
                                m_strLastError = m_vTokens.back() + " is not a function.";
                                XST_LOG_ERR( m_strLastError );
                                return false;
                        }
                }
                else
                {
                        lua_getglobal(m_pLuaState, _strName.c_str());
                        if(!lua_isfunction(m_pLuaState, -1))
                        {
                                lua_pop(m_pLuaState, 1);
                                m_strLastError = _strName + " is not a function.";
                                XST_LOG_ERR( m_strLastError );
                                return false;
                        }
                }

                return true;

        }

        bool CLuaCall::_GetFunction(lpcastr _strName)
        {
                //Split
                m_vTokens.clear();
                XST::StringUtil::Split( _strName, &m_vTokens, ".:" );
                if( m_vTokens.size() > 1 )
                {
                        lua_getglobal( m_pLuaState, m_vTokens[0].c_str() );
                        if( !lua_istable( m_pLuaState, -1 ) )
                        {
                                m_strLastError = m_vTokens[0] + " is not a table.";
                                XST_LOG_ERR( m_strLastError );
                                return false;
                        }

                        for(u32 i = 1; i < m_vTokens.size() - 1; ++i)
                        {
                                lua_pushstring( m_pLuaState, m_vTokens[i].data() );
                                lua_gettable( m_pLuaState, -2 );
                                if( !lua_istable( m_pLuaState, -1 ) )
                                {
                                        m_strLastError = m_vTokens[i] + " is not a table.";
                                        XST_LOG_ERR( m_strLastError );
                                        return false;
                                }
                        }

                        lua_pushstring( m_pLuaState, m_vTokens.back().data() );
                        lua_gettable( m_pLuaState, -2 );
                        if( !lua_isfunction( m_pLuaState, -1 ) )
                        {
                                m_strLastError = m_vTokens.back() + " is not a function.";
                                XST_LOG_ERR( m_strLastError );
                                return false;
                        }
                }
                else
                {
                        lua_getglobal( m_pLuaState, _strName );
                        if(!lua_isfunction(m_pLuaState, -1))
                        {
                                lua_pop(m_pLuaState, 1);
                                m_strLastError = _strName;
                                m_strLastError += " is not a function.";
                                XST_LOG_ERR( m_strLastError );
                                return false;
                        }
                }

                return true;

        }

        bool CLuaCall::_GetFunction(lpcastr _lpszNamespace, lpcastr _lpszTableName, lpcastr _lpszFuncName)
        {
                lua_getglobal( m_pLuaState, _lpszNamespace );
                if( !lua_istable( m_pLuaState, -1 ) )
                {
                        m_strLastError = _lpszNamespace;
                        m_strLastError += " is not a table.";
                        XST_LOG_ERR( m_strLastError );
                        return false;
                }

                lua_pushstring( m_pLuaState, _lpszTableName );
                lua_gettable( m_pLuaState, -2 );
                if( !lua_istable( m_pLuaState, -1 ) )
                {
                        m_strLastError = _lpszTableName;
                        m_strLastError += " is not a table.";
                        XST_LOG_ERR( m_strLastError );
                        return false;
                }

                lua_pushstring( m_pLuaState, _lpszFuncName );
                lua_gettable( m_pLuaState, -2 );
                if( !lua_isfunction( m_pLuaState, -1 ) )
                {
                        m_strLastError = _lpszFuncName;
                        m_strLastError += " is not a function.";
                        XST_LOG_ERR( m_strLastError );
                        return false;
                }

                return true;
        }

        bool CLuaCall::_GetFunction(lpcastr* _alpszTableNames, cu32& _uiArraySize)
        {
                if( _uiArraySize > 1 )
                {
                        lua_getglobal( m_pLuaState, _alpszTableNames[0] );
                        if( !lua_istable( m_pLuaState, -1 ) )
                        {
                                m_strLastError = _alpszTableNames[0];
                                m_strLastError += " is not a table.";
                                XST_LOG_ERR( m_strLastError );
                                return false;
                        }

                        //The last element in the array is function name
                        for(u32 i = 1; i < _uiArraySize - 1; ++i)
                        {
                                lua_pushstring( m_pLuaState, _alpszTableNames[i] );
                                lua_gettable( m_pLuaState, -2 );
                                if( !lua_istable( m_pLuaState, -1 ) )
                                {
                                        m_strLastError = _alpszTableNames[i];
                                        m_strLastError += " is not a table.";
                                        XST_LOG_ERR( m_strLastError );
                                        return false;
                                }
                        }

                        //Get the function
                        lua_pushstring( m_pLuaState, _alpszTableNames[ _uiArraySize -1 ] );
                        lua_gettable( m_pLuaState, -2 );
                        if( !lua_isfunction( m_pLuaState, -1 ) )
                        {
                                m_strLastError = _alpszTableNames[ _uiArraySize - 1 ];
                                m_strLastError += " is not a function.";
                                XST_LOG_ERR( m_strLastError );
                                return false;
                        }
                }
                else
                {
                        lua_getglobal( m_pLuaState, _alpszTableNames[0] );
                        if( !lua_isfunction( m_pLuaState, -1 ) )
                        {
                                m_strLastError = _alpszTableNames[0];
                                m_strLastError += " is not a function.";
                                XST_LOG_ERR( m_strLastError );
                                return false;
                        }
                }

                return true;
        }

}//xst