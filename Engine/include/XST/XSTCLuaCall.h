#ifndef XST_CLUA_CALL_H
#define XST_CLUA_CALL_H

#include "ThirdParty/lua51/lua.hpp"
#include "ThirdParty/lua51/tolua++.h"
#include "XSTCLogger.h"

//Disable warning that stl container is not exported to dll
//#pragma warning( disable: 4251 )

namespace XST
{
	struct VoidT
    {
    };

    struct UserT
    {
            UserT() {}
            UserT(xst_unknown _pData, lpcastr _lpszName)
            {
                    xst_memcpy( strName, 32, _lpszName, 32 );
                    pData = _pData;
            }

            ch8                     strName[32];
            xst_unknown     pData;
    };

    class XST_API CLuaCall
    {
		friend class CLuaApi;

		public:
							CLuaCall() : m_pLuaState(0) {}
				explicit	CLuaCall(lua_State* _pLuaState) : m_pLuaState(_pLuaState) {}

				virtual		~CLuaCall() {}

                //i32                                   Init(XST::ILogger* pLogger);

							template<typename _RETURN_>
							i32 Call(const xst_astring& _strName, _RETURN_* _pReturnOut = 0)
							{
								if(!_GetFunction(_strName))
								{
										return RESULT::FAILED;
								}

								return _Call<_RETURN_>( 0, _pReturnOut );
							}

							template<typename _RETURN_>
							i32 Call(lpcastr _lpszName, _RETURN_* _pReturnOut = 0)
							{
									if(!_GetFunction( _lpszName ))
									{
											return RESULT::FAILED;
									}

									return _Call<_RETURN_>( 0, _pReturnOut );
							}

							template<typename _RETURN_>
							i32 Call(lpcastr _lpszNamespace, lpcastr _lpszTable, lpcastr _lpszFunction, _RETURN_* _pReturnOut = 0)
							{
									if(!_GetFunction( _lpszNamespace, _lpszTable, _lpszFunction ))
									{
											return RESULT::FAILED;
									}

									return _Call<_RETURN_>( 0, _pReturnOut );
							}

							template<typename _RETURN_>
							i32 Call(lpcastr* _alpszNames, const u32& _uiArraySize, _RETURN_* _pReturnOut = 0)
							{
									if(!_GetFunction( _alpszNames, _uiArraySize ))
									{
											return RESULT::FAILED;
									}

									return _Call<_RETURN_>( 0, _pReturnOut );
							}

							template<typename _RETURN_, typename _T1_>
							i32 Call(const xst_astring& _strName, _RETURN_* _pReturnOut, const _T1_& _tArg1)
							{
									if(!_GetFunction(_strName))
									{
											return RESULT::FAILED;
									}

									_Set(_tArg1);

									return _Call<_RETURN_>(1, _pReturnOut);
							}

							template<typename _RETURN_, typename _T1_>
							i32 Call(lpcastr _lpszName, _RETURN_* _pReturnOut, const _T1_& _tArg1)
							{
									if(!_GetFunction( _lpszName ))
									{
											return RESULT::FAILED;
									}

									_Set(_tArg1);

									return _Call<_RETURN_>( 1, _pReturnOut );
							}

							template<typename _RETURN_, typename _T1_>
							i32 Call(lpcastr _lpszNamespace, lpcastr _lpszTable, lpcastr _lpszFunction, _RETURN_* _pReturnOut, const _T1_& _tArg1)
							{
									if(!_GetFunction( _lpszNamespace, _lpszTable, _lpszFunction ))
									{
											return RESULT::FAILED;
									}

									_Set(_tArg1);

									return _Call<_RETURN_>( 1, _pReturnOut );
							}

							template<typename _RETURN_, typename _T1_>
							i32 Call(lpcastr* _alpszNames, const u32& _uiArraySize, _RETURN_* _pReturnOut, const _T1_& _tArg1)
							{
									if(!_GetFunction( _alpszNames, _uiArraySize ))
									{
											return RESULT::FAILED;
									}

									_Set(_tArg1);

									return _Call<_RETURN_>( 1, _pReturnOut );
							}

							template<typename _RETURN_, typename _T1_, typename _T2_>
							i32 Call(const xst_astring& _strName, _RETURN_* _pReturnOut, const _T1_& _tArg1, const _T2_& _tArg2)
							{
									if(!_GetFunction(_strName))
									{
											return RESULT::FAILED;
									}

									_Set(_tArg1);
									_Set(_tArg2);

									return _Call<_RETURN_>(2, _pReturnOut);
							}

							template<typename _RETURN_, typename _T1_, typename _T2_>
							i32 Call(lpcastr _lpszName, _RETURN_* _pReturnOut, const _T1_& _tArg1, const _T2_& _tArg2)
							{
									if(!_GetFunction( _lpszName ))
									{
											return RESULT::FAILED;
									}

									_Set(_tArg1);
									_Set(_tArg2);

									return _Call<_RETURN_>( 2, _pReturnOut );
							}

							template<typename _RETURN_, typename _T1_, typename _T2_>
							i32 Call(lpcastr _lpszNamespace, lpcastr _lpszTable, lpcastr _lpszFunction, _RETURN_* _pReturnOut, const _T1_& _tArg1, const _T2_& _tArg2)
							{
									if(!_GetFunction( _lpszNamespace, _lpszTable, _lpszFunction ))
									{
											return RESULT::FAILED;
									}

									_Set(_tArg1);
									_Set(_tArg2);

									return _Call<_RETURN_>( 2, _pReturnOut );
							}

							template<typename _RETURN_, typename _T1_, typename _T2_>
							i32	Call(lpcastr* _alpszNames, const u32& _uiArraySize, _RETURN_* _pReturnOut, const _T1_& _tArg1, const _T2_& _tArg2)
							{
									if(!_GetFunction( _alpszNames, _uiArraySize ))
									{
											return RESULT::FAILED;
									}

									_Set(_tArg1);
									_Set(_tArg2);

									return _Call<_RETURN_>( 2, _pReturnOut );
							}

							template<typename _RETURN_, typename _T1_, typename _T2_, typename _T3_>
							i32	Call(const xst_astring& _strName, _RETURN_* _pReturnOut, const _T1_& _tArg1, const _T2_& _tArg2, const _T3_& _tArg3)
							{
									if(!_GetFunction(_strName))
									{
											return RESULT::FAILED;
									}

									_Set(_tArg1);
									_Set(_tArg2);
									_Set(_tArg3);

									return _Call<_RETURN_>(3, _pReturnOut);
							}

							template<typename _RETURN_, typename _T1_, typename _T2_, typename _T3_>
							i32	Call(lpcastr _lpszName, _RETURN_* _pReturnOut, const _T1_& _tArg1, const _T2_& _tArg2, const _T3_& _tArg3)
							{
									if(!_GetFunction( _lpszName ))
									{
											return RESULT::FAILED;
									}

									_Set(_tArg1);
									_Set(_tArg2);
									_Set(_tArg3);

									return _Call<_RETURN_>( 3, _pReturnOut );
							}

							template<typename _RETURN_, typename _T1_, typename _T2_, typename _T3_>
							i32	Call(lpcastr _lpszNamespace, lpcastr _lpszTable, lpcastr _lpszFunction, _RETURN_* _pReturnOut, const _T1_& _tArg1, const _T2_& _tArg2, const _T3_& _tArg3)
							{
									if(!_GetFunction( _lpszNamespace, _lpszTable, _lpszFunction ))
									{
											return RESULT::FAILED;
									}

									_Set(_tArg1);
									_Set(_tArg2);
									_Set(_tArg3);

									return _Call<_RETURN_>( 3, _pReturnOut );
							}

							template<typename _RETURN_, typename _T1_, typename _T2_, typename _T3_>
							i32	Call(lpcastr* _alpszNames, const u32& _uiArraySize, _RETURN_* _pReturnOut, const _T1_& _tArg1, const _T2_& _tArg2, const _T3_& _tArg3)
							{
									if(!_GetFunction( _alpszNames, _uiArraySize ))
									{
											return RESULT::FAILED;
									}

									_Set(_tArg1);
									_Set(_tArg2);
									_Set(_tArg3);

									return _Call<_RETURN_>( 3, _pReturnOut );
							}

							template<typename _RETURN_, typename _T1_, typename _T2_, typename _T3_, typename _T4_>
							i32	Call(const xst_astring& _strName, _RETURN_* _pReturnOut, const _T1_& _tArg1, const _T2_& _tArg2, const _T3_& _tArg3, const _T4_& _tArg4)
							{
									if(!_GetFunction(_strName))
									{
											return RESULT::FAILED;
									}

									_Set(_tArg1);
									_Set(_tArg2);
									_Set(_tArg3);
									_Set(_tArg4);

									return _Call<_RETURN_>(4, _pReturnOut);
							}

							template<typename _RETURN_, typename _T1_, typename _T2_, typename _T3_, typename _T4_>
							i32	Call(lpcastr _lpszName, _RETURN_* _pReturnOut, const _T1_& _tArg1, const _T2_& _tArg2, const _T3_& _tArg3, const _T4_& _tArg4)
							{
									if(!_GetFunction( _lpszName ))
									{
											return RESULT::FAILED;
									}

									_Set(_tArg1);
									_Set(_tArg2);
									_Set(_tArg3);

									return _Call<_RETURN_>( 4, _pReturnOut );
							}

							template<typename _RETURN_, typename _T1_, typename _T2_, typename _T3_, typename _T4_>
							i32	Call(lpcastr _lpszNamespace, lpcastr _lpszTable, lpcastr _lpszFunction, _RETURN_* _pReturnOut, const _T1_& _tArg1, const _T2_& _tArg2, const _T3_& _tArg3, const _T4_& _tArg4)
							{
								if(!_GetFunction( _lpszNamespace, _lpszTable, _lpszFunction ))
								{
										return RESULT::FAILED;
								}

								_Set(_tArg1);
								_Set(_tArg2);
								_Set(_tArg3);

								return _Call<_RETURN_>( 4, _pReturnOut );
							}

							template<typename _RETURN_, typename _T1_, typename _T2_, typename _T3_, typename _T4_>
							i32	Call(lpcastr* _alpszNames, const u32& _uiArraySize, _RETURN_* _pReturnOut, const _T1_& _tArg1, const _T2_& _tArg2, const _T3_& _tArg3, const _T4_& _tArg4)
							{
								if(!_GetFunction( _alpszNames, _uiArraySize ))
								{
										return RESULT::FAILED;
								}

								_Set(_tArg1);
								_Set(_tArg2);
								_Set(_tArg3);

								return _Call<_RETURN_>( 4, _pReturnOut );
							}

							xst_fi const xst_astring&	GetLastError()
														{ return m_strLastError; }

        protected:

							template<typename _RETURN_>
							i32	_Call(u32 _uiArgCount, _RETURN_* _pReturnOut)
							{
								//If not procedure
								i32 iReturns = (_pReturnOut != 0);
								if(lua_pcall(m_pLuaState, _uiArgCount, iReturns, 0) != 0)
								{
										m_strLastError = lua_tostring(m_pLuaState, -1);
										//Core::CLastError::Set( m_strLastError );
										//if( m_pLogger ) m_pLogger->Write( m_strLastError );
										XST_LOG_ERR( m_strLastError );
										return RESULT::FAILED;
								}

								if( _pReturnOut )
										_Get(_pReturnOut);

								return RESULT::OK;
							}

							bool	_GetFunction(const xst_astring& _strName);
							bool	_GetFunction(lpcastr _lpszName);
							bool	_GetFunction(lpcastr _lpszNamespace, lpcastr _lpszTableName, lpcastr _lpszFuncName);
							//bool                          _GetFunction(const xst_astring* _aTableNames, const u32& _uiArraySize);
							bool	_GetFunction(lpcastr* _alpszTableNames, cu32& _uiArraySize);


                xst_fi  void	_Set(ci32& _value) { lua_pushinteger(m_pLuaState, _value); }
                xst_fi  void    _Get(i32* _pValueOut) const { (*_pValueOut) = (i32)lua_tointeger(m_pLuaState, -1); }

                xst_fi  void	_Set(ci16& _value) { lua_pushinteger(m_pLuaState, _value); }
                xst_fi  void	_Get(i16* _pValueOut) const { (*_pValueOut) = (i16)lua_tointeger(m_pLuaState, -1); }

                xst_fi  void	_Set(const i8& _value) { lua_pushinteger(m_pLuaState, _value); }
                xst_fi  void	_Get(i8* _pValueOut) const { (*_pValueOut) = (i8)lua_tointeger(m_pLuaState, -1); }

                xst_fi  void	_Set(cu32& _value) { lua_pushinteger(m_pLuaState, _value); }
                xst_fi  void	_Get(u32* _pValueOut) const { (*_pValueOut) = (u32)lua_tointeger(m_pLuaState, -1); }

                xst_fi  void	_Set(cu16& _value) { lua_pushinteger(m_pLuaState, _value); }
                xst_fi  void	_Get(u16* _pValueOut) const { (*_pValueOut) = (u16)lua_tointeger(m_pLuaState, -1); }

                xst_fi  void	_Set(cu8& _value) { lua_pushinteger(m_pLuaState, _value); }
                xst_fi  void	_Get(u8* _pValueOut) const { (*_pValueOut) = (u8)lua_tointeger(m_pLuaState, -1); }

                xst_fi  void	_Set(cf32& _value) { lua_pushnumber(m_pLuaState, _value); }
                xst_fi  void	_Get(f32* _pValueOut) const { (*_pValueOut) = (f32)lua_tonumber(m_pLuaState, -1); }

                xst_fi  void	_Set(const xst_astring& _value) { lua_pushstring(m_pLuaState, _value.c_str()); }
                xst_fi  void	_Get(xst_astring* _pValueOut) const { (*_pValueOut) = (ch8*)lua_tostring(m_pLuaState, -1); }

                xst_fi  void	_Set(const bool& _value) { lua_pushboolean(m_pLuaState, _value); }
                xst_fi  void	_Get(bool* _pValueOut) const { (*_pValueOut) = (bool)lua_toboolean(m_pLuaState, -1); }

                xst_fi  void	_Set(void* _value) { lua_pushlightuserdata(m_pLuaState, _value); }
                xst_fi  void	_Get(void* _pValueOut) const { if(_pValueOut) (_pValueOut) = (void*)lua_touserdata(m_pLuaState, -1); }

                xst_fi  void	_Set(const UserT& _UserType) { tolua_pushusertype(m_pLuaState, _UserType.pData, _UserType.strName); }

                xst_fi  void	_Get(VoidT* _pValueOut) const {}

		protected:

                lua_State*          m_pLuaState;
                xst_astring         m_strLastError;
				StringVector		m_vTokens; //Used in _Call method
    };

};

#endif
