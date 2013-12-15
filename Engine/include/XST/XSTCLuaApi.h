#ifndef XST_CLUA_API_H
#define XST_CLUA_API_H

#include "XSTCLuaScriptManager.h"
#include "XSTCLuaTableManager.h"
#include "XSTCLuaCall.h"

#if defined (XST_WINDOWS)
#pragma push_macro( "LoadString" )
#undef LoadString
#endif //LoadString

namespace XST
{

	class CLuaTable;

	#define XST_LF0(_retType, _methodName, _luaFunc) \
		xst_fi _retType _methodName() \
		{ xst_assert( m_pState, "Lua state not created" ); return (_retType)_luaFunc( m_pState ); }

	#define XST_LF1(_retType, _methodName, _luaFunc, _argType1) \
		xst_fi _retType _methodName(_argType1 arg1) \
		{ xst_assert( m_pState, "Lua state not created" ); return (_retType)_luaFunc( m_pState, arg1 ); }

	#define XST_LF2(_retType, _methodName, _luaFunc, _argType1, _argType2) \
		xst_fi _retType _methodName(_argType1 arg1, _argType2 arg2) \
		{ xst_assert( m_pState, "Lua state not created" ); return (_retType)_luaFunc( m_pState, arg1, arg2 ); }

	#define XST_LF3(_retType, _methodName, _luaFunc, _argType1, _argType2, _argType3) \
		xst_fi _retType _methodName(_argType1 arg1, _argType2 arg2, _argType3 arg3) \
		{ xst_assert( m_pState, "Lua state not created" ); return (_retType)_luaFunc( m_pState, arg1, arg2, arg3 ); }

	class XST_API CLuaApi : public CLuaCall
	{
		public:

										CLuaApi();
			virtual						~CLuaApi();

					i32					Create();

					void				Destroy();

					i32					LoadScript(cu8* pScriptData, cul32& ulScriptSize);

										XST_LF1( int, IsBoolean, lua_isboolean, int );
										XST_LF1( int, IsCFunction, lua_iscfunction, int );
										XST_LF1( int, IsFunction, lua_isfunction, int );
										XST_LF1( int, IsLightUserData, lua_islightuserdata, int );
										XST_LF1( int, IsNil, lua_isnil, int );
										XST_LF1( int, IsNone, lua_isnone, int );
										XST_LF1( int, IsNoneOrNil, lua_isnoneornil, int );
										XST_LF1( int, IsNumber, lua_isnumber, int )
										XST_LF1( int, IsString, lua_isstring, int )
										XST_LF1( int, IsTable, lua_istable, int );
										XST_LF1( int, IsThread, lua_isthread, int );
										XST_LF1( int, IsUserData, lua_isuserdata, int )
										XST_LF2( int, LessThan, lua_lessthan, int, int )
										XST_LF3( int, Load, lua_load, lua_Reader, void*, const char* )
			xst_fi	lua_State*			NewState(lua_Alloc Alloc, void* p)
										{ xst_assert( m_pState, "Lua state not created" ); return lua_newstate( Alloc, p ); }
										XST_LF0( void, NewTable, lua_newtable );
										XST_LF2( void, CreateTable, lua_createtable, int, int )
										XST_LF0( lua_State*, NewThread, lua_newthread );
										XST_LF1( void*, NewUserData, lua_newuserdata, unsigned int );
										XST_LF1( int, Next, lua_next, int );;
										XST_LF1( unsigned int, ObjLen, lua_objlen, int );
										XST_LF3( int, PCall, lua_pcall, int, int, int );
										XST_LF1( void, Pop, lua_pop, int );
										XST_LF1( void, PushBoolean, lua_pushboolean, int );
										XST_LF2( void, PushCClosure, lua_pushcclosure, lua_CFunction, int );
										XST_LF1( void, PushCFunction, lua_pushcfunction, lua_CFunction );;
										XST_LF1( void, PushLightUserData, lua_pushlightuserdata, void* );
										XST_LF2( void, PushLString, lua_pushlstring, const char*, unsigned int );
										XST_LF0( void, PushNil, lua_pushnil );
										XST_LF1( void, PushNumber, lua_pushnumber, lua_Number);
										XST_LF1( void, PushString, lua_pushstring, const char* );
										XST_LF0( int, PushThread, lua_pushthread );
										XST_LF1( void, PushValue, lua_pushvalue, int );
										XST_LF2( void, PushVFString, lua_pushvfstring, const char*, va_list );
										XST_LF2( int, RawEqual, lua_rawequal, int, int );
										XST_LF1( void, RawGet, lua_rawget, int );
										XST_LF2( void, RawGetI, lua_rawgeti, int, int );
										XST_LF1( void, RawSet, lua_rawset, int );
										XST_LF2( void, RawSetI, lua_rawseti, int, int );
										XST_LF2( void, Register, lua_register, const char*, lua_CFunction );
										XST_LF1( void, Remove, lua_remove, int );
										XST_LF1( void, Replace, lua_replace, int );
										XST_LF1( int, Resume, lua_resume, int );
										XST_LF2( void, SetAllocF, lua_setallocf, lua_Alloc, void* );
										XST_LF1( int, SetFEnv, lua_setfenv, int );
										XST_LF2( void, SetField, lua_setfield, int, const char* );
										XST_LF1( void, SetGlobal, lua_setglobal, const char* );
										XST_LF1( void, GetGlobal, lua_getglobal, const char* );
										XST_LF1( int, SetMetaTable, lua_setmetatable, int );
										XST_LF1( void, SetTable, lua_settable, int );
										XST_LF1( void, SetTop, lua_settop, int );
										XST_LF0( int, Status, lua_status );
										XST_LF1( int, ToBoolean, lua_toboolean, int );
										XST_LF1( lua_CFunction, ToCFunction, lua_tocfunction, int );		
										XST_LF1( lua_Integer, ToInteger, lua_tointeger, int );
										XST_LF2( const char*, ToLString, lua_tolstring, int, size_t* );
										XST_LF1( double, ToNumber, lua_tonumber, int );
										XST_LF1( const void*, ToPointer, lua_topointer, int );
										XST_LF1( const char*, ToString, lua_tostring, int );
										XST_LF1( lua_State*, ToThread, lua_tothread, int );
										XST_LF1( void*, ToUserData, lua_touserdata, int );
										XST_LF1( int, Type, lua_type, int );
										XST_LF1( const char*, TypeName, lua_typename, int );
			xst_fi void					ArgCheck(int cond, int narg, const char* extramsg) const
										{ xst_assert( m_pState, "Lua state not created" ); luaL_argcheck( m_pState, cond, narg, extramsg ); }
										XST_LF2( int, ArgError, luaL_argerror, int, const char* );
										XST_LF2( int, CallMeta, luaL_callmeta, int, const char* );
										XST_LF1( void, CheckAny, luaL_checkany, int );
										XST_LF1( int, CheckInt, luaL_checkint, int );
										XST_LF1( lua_Integer, CheckInteger, luaL_checkinteger, int );
										XST_LF1( long, CheckLong, luaL_checklong, int );
										XST_LF1( const char*, CheckString, luaL_checkstring, int );
										XST_LF2( void, GetField, lua_getfield, int, const char* );
										XST_LF1( lua_Number, CheckNumber, luaL_checknumber, int );
			xst_fi	int					CheckOption(int arg1, const char* arg2, const char* const arg3[])
										{ xst_assert( m_pState, "Lua state not created" ); return luaL_checkoption( m_pState, arg1, arg2, arg3 ); }
										XST_LF2( void, CheckStack, luaL_checkstack, int, const char* );
										XST_LF1( const char*, CheckLString, luaL_checkstring, int );
										XST_LF2( void, CheckType, luaL_checktype, int, int );
										XST_LF2( void*, CheckUData, luaL_checkudata, int, const char* );
										XST_LF1( int, DoFile, luaL_dofile, const char* );
										XST_LF1( int, DoString, luaL_dostring, const char* );
										XST_LF2( int, GetMetaField, luaL_getmetafield, int, const char* );
										XST_LF1( void, GetMetaTable, luaL_getmetatable, const char* );
										XST_LF3( const char*, GSub, luaL_gsub, const char*, const char*, const char* );
										XST_LF3( int, LoadBuffer, luaL_loadbuffer, const char*, size_t, const char* );
										XST_LF1( int, LoadFile, luaL_loadfile, const char* );
										XST_LF1( int, LoadLuaString, luaL_loadstring, const char* );
										XST_LF1( int, NewMetaTable, luaL_newmetatable, const char* );
			xst_fi lua_State*			LNewState()
										{ xst_assert( m_pState, "Lua state not created" ); return luaL_newstate(); }
										XST_LF0( void, OpenLibs, luaL_openlibs );
										XST_LF0( int, OpenBase, luaopen_base );
										XST_LF0( int, OpenTable, luaopen_table );
										XST_LF0( int, OpenIO, luaopen_io );
										XST_LF0( int, OpenOS, luaopen_os );
										XST_LF0( int, OpenMath, luaopen_math );
										XST_LF0( int, OpenString, luaopen_string );
										XST_LF0( int, OpenDebug, luaopen_debug );
										XST_LF0( int, OpenPackage, luaopen_package );
										XST_LF3( void, OpenLib, luaL_openlib, const char*, luaL_Reg*, int );
										XST_LF2( int, OptInt, luaL_optint, int, lua_Integer );
										XST_LF2( lua_Integer, OptInteger, luaL_optinteger, int, lua_Integer );
										XST_LF2( long, OptLong, luaL_optlong, int, lua_Integer );
										XST_LF3( const char*, OptLString, luaL_optlstring, int, const char*, size_t* );
										XST_LF2( lua_Number, OptNumber, luaL_optnumber, int, lua_Number );
										XST_LF2( const char*, OptString, luaL_optstring, int, const char* );
										XST_LF1( int, Ref, luaL_ref, int );
										XST_LF2( void, LRegister, luaL_register, const char*, const luaL_Reg* );
										XST_LF1( const char*, LTypeName, luaL_typename, int );
										XST_LF2( int, TypeError, luaL_typerror, int, const char* );
										XST_LF2( void, Unref, luaL_unref, int, int );
										XST_LF1( void, Where, luaL_where, int );

					bool				GetTable(lpcastr lpszTableName);

					bool				GetFunction(lpcastr lpszFuncName);

					//CLuaTable*			ParseTable(lpcastr lpszTableName);

			xst_fi	lua_State*			GetState() const
										{ return m_pState; }

			xst_fi	lpcastr				GetLastError()
										{ return ToString( -1 ); }

			xst_fi	bool				IsCreated() const
										{ return m_bCreated; }

			xst_fi	bool				IsLoaded() const
										{ return m_bLoaded; }

			xst_fi	bool				IsReady() const
										{ return m_bCreated && m_bLoaded; }

		protected:

			xst_unknown		m_hLuaLib;
			lua_State*		m_pState;
			bool			m_bCreated;
			bool			m_bLoaded;

	};

}//xst

#if defined (XST_WINDOWS)
#pragma pop_macro( "LoadString" )
#endif //XST_LOADSTRING

#endif