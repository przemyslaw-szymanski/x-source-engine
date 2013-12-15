#include "../../include/XST/XSTCLuaTable.h"
#include "../../include/XST/XSTCLogger.h"
#include "../../include/XST/XSTCLuaApi.h"

namespace XST
{
	CLuaTable::CLuaTable(xst_castring& _strTableName) : m_strTableName( _strTableName ), m_pParentTable(0)
    {
    }

    CLuaTable::~CLuaTable()
    {
		Clear();
    }

    void CLuaTable::Clear()
    {
		for(TablesTable::iterator Itr = m_mChildTables.begin(); Itr != m_mChildTables.end(); ++Itr)
		{
			xst_delete( Itr->second );
		}

        m_mTable.clear();
        m_mChildTables.clear();
        m_pParentTable = 0;
        XSTVectorClear( m_vChildTables, CLuaTable* );
    }

    void CLuaTable::Inherit(const CLuaTable* _pTable)
    {
        xst_assert(_pTable != 0, "(CLuaTable::Inherit) Destination table is null");
        m_mTable = _pTable->GetTableData();
    }

	i32 CLuaTable::Create(CLuaApi* pApi, lpcastr lpszTableName)
	{
		xst_assert( pApi, "(CLuaTable::Create) Lua api is null" );
		return Create( pApi, xst_null, lpszTableName );
	}

	i32 CLuaTable::Create(CLuaApi* pApi, CLuaTable* pParentTable, lpcastr lpszTableName)
	{
		if( !pApi->GetTable( lpszTableName ) )
		{
			XST_LOG_ERR( "Table: " << lpszTableName << " doesn't exists in lua script" );
			return RESULT::FAILED;
		}

		this->m_pParentTable = pParentTable;
		return _GetTableInfo( pApi, this, lpszTableName );
	}

	i32 CLuaTable::_GetTableInfo(CLuaApi* pApi, CLuaTable* pParentTable, lpcastr lpszTableName)
	{
		CLuaTable* pTable = pParentTable;

		xst_castring strTableName = pTable->GetTableName() + ".";
        lpcastr lpszKey = 0;
        const i32 iVar = -2;
        const i32 iValue = -1;

        xst_astring strValue;
        lpcastr lpszTempValue = 0;
        u32 uiTempValLen = 0;
        bool bIsFloat = false;

        for( pApi->PushNil(); pApi->Next( -2 ); pApi->Pop( 1 ) )
        {
            lpszKey = 0;
            if( pApi->IsString( iVar ) )
            {
				lpszKey = pApi->ToString( iVar );
            }

            switch( pApi->Type( iValue ) )
            {
                case LUA_TNUMBER:
                {
                    lpszTempValue = pApi->ToString( iValue );

                    //Check if it is float
                    if( XST::StringUtil::IsFloat( lpszTempValue ) )
                    {
						pTable->Set( lpszKey, (f32)pApi->ToNumber( iValue ) );
                    }
                    else
                    {
						pTable->Set( lpszKey, (i32)pApi->ToNumber( iValue ) );
                    }
                }
                break;

                case LUA_TBOOLEAN:
                {
					pTable->Set( lpszKey, (bool)pApi->ToBoolean( iValue ) );
                }
                break;

                case LUA_TFUNCTION:
                {
					pTable->SetFunction( lpszKey, lpszKey );
                }
                break;

                case LUA_TSTRING:
                {
					pTable->SetString( lpszKey, pApi->ToString( iValue ) );
                }
                break;

				case LUA_TUSERDATA:
				{
					pTable->Set( lpszKey, pApi->ToUserData( iValue ) );
				}
				break;

                case LUA_TTABLE:
                {
                    //CLuaTable* pTempTable = m_pTableMgr->CreateTable( strTableName + ((lpszKey)? lpszKey : ""), _strTableGroupName );
                    CLuaTable* pChildTable = xst_new CLuaTable( ( ( lpszKey )? lpszKey : "" ) );
					if( _GetTableInfo( pApi, pChildTable, lpszKey ) != RESULT::OK )
					{
						xst_delete( pChildTable );
						return RESULT::FAILED;
					}

					pTable->SetTable( lpszKey, pChildTable->GetTableName().data() );
                    pTable->Set( lpszKey, pChildTable );
                }
                break;
            }

        }

		return RESULT::OK;

	}

    void CLuaTable::SetString(xst_castring& _strKey, xst_castring& _strValue)
    {
        //TableData Data;
        //const u32 uiSize = (_strValue.length() >= CLuaTable::MAX_STRING_LENGTH)? CLuaTable::MAX_STRING_LENGTH : _strValue.length();
        //memcpy(Data.strValue.strText, _strValue.data(), uiSize);
        //Data.strValue.strText[uiSize] = 0;
        //Data.strValue.uiLength = uiSize;
        ////m_mTable.insert( TablePair(_strKey, Data) );
        if( _strValue.length() == 0 )
                return;

        const u32 uiLen = _strValue.length();
        SString Str;
        Str.uiLength = (uiLen > CLuaTable::MAX_STRING_LENGTH - 1)? CLuaTable::MAX_STRING_LENGTH - 1 : uiLen;
        xst_memcpy( Str.strText, CLuaTable::MAX_STRING_LENGTH, _strValue.data(), Str.uiLength );
        Str.strText[ Str.uiLength ] = 0;

        m_mTable[ _strKey ] = STableData( STRING, Str );
    }

    void CLuaTable::SetString(xst_castring& _strKey, lpcastr _lpszString)
    {
        /*TableData Data;
        const u32 ulStringLen = strlen(_lpszString);
        const u32 uiSize = ( ulStringLen >= CLuaTable::MAX_STRING_LENGTH )? CLuaTable::MAX_STRING_LENGTH : ulStringLen;
        memcpy(Data.strValue.strText, _lpszString, uiSize);
        Data.strValue.strText[uiSize] = 0;
        Data.strValue.uiLength = uiSize;
        m_mTable.insert( TablePair(_strKey, Data) );*/

        if( _lpszString == 0 )
                return;

        const u32 uiLen = strlen( _lpszString );
        SString Str;
        Str.uiLength = (uiLen > CLuaTable::MAX_STRING_LENGTH - 1)? CLuaTable::MAX_STRING_LENGTH - 1 : uiLen;
        xst_memcpy( Str.strText, CLuaTable::MAX_STRING_LENGTH, _lpszString, Str.uiLength );
        Str.strText[ Str.uiLength ] = 0;

        m_mTable[ _strKey ] = STableData( STRING, Str );
    }

    void CLuaTable::SetString(xst_castring& _strKey, lpcastr _lpszString, const u32& _uiStringLength)
    {
        /*TableData Data;
        const u32 uiSize = ( _uiStringLength >= CLuaTable::MAX_STRING_LENGTH )? CLuaTable::MAX_STRING_LENGTH : _uiStringLength;
        memcpy(Data.strValue.strText, _lpszString, uiSize);
        Data.strValue.strText[uiSize] = 0;
        Data.strValue.uiLength = uiSize;
        m_mTable.insert( TablePair(_strKey, Data) );*/

        if( _lpszString == 0 )
                return;

        const u32 uiLen = _uiStringLength;
        SString Str;
        Str.uiLength = (uiLen > CLuaTable::MAX_STRING_LENGTH - 1)? CLuaTable::MAX_STRING_LENGTH - 1 : uiLen;
        xst_memcpy( Str.strText, CLuaTable::MAX_STRING_LENGTH, _lpszString, Str.uiLength );
        Str.strText[ Str.uiLength ] = 0;

        m_mTable[ _strKey ] = STableData( STRING, Str );
    }

    i32     CLuaTable::GetString(xst_castring& _strKey, SString* _pstrOutString)
    {
        ItemItr Itr = m_mTable.lower_bound(_strKey);
        //If resource exists
        if( Itr != m_mTable.end() && !(m_mTable.key_comp()(_strKey, Itr->first)) )
        {
                (*_pstrOutString) = Itr->second.uData.strValue;
                return RESULT::OK;
        }

        return RESULT::FAILED;
    }

	i32     CLuaTable::GetString(xst_castring& _strKey, xst_astring* _pstrOutString)
    {
        ItemItr Itr = m_mTable.lower_bound(_strKey);
        //If resource exists
        if( Itr != m_mTable.end() && !(m_mTable.key_comp()(_strKey, Itr->first)) )
        {
				(*_pstrOutString) = Itr->second.uData.strValue.strText;
                return RESULT::OK;
        }

        return RESULT::FAILED;
    }

    i32     CLuaTable::GetFloat(xst_castring& _strKey, f32* _pfOutFloat)
    {
        ItemItr Itr = m_mTable.lower_bound(_strKey);
        //If resource exists
        if( Itr != m_mTable.end() && !(m_mTable.key_comp()(_strKey, Itr->first)) )
        {
                (*_pfOutFloat) = Itr->second.uData.fValue;
                return RESULT::OK;
        }

        return RESULT::FAILED;
    }

    i32     CLuaTable::GetInt(xst_castring& _strKey, i32* _piOutInt)
    {
        ItemItr Itr = m_mTable.lower_bound(_strKey);
        //If resource exists
        if( Itr != m_mTable.end() && !(m_mTable.key_comp()(_strKey, Itr->first)) )
        {
                (*_piOutInt) = Itr->second.uData.iValue;
                return RESULT::OK;
        }

        return RESULT::FAILED;
    }

    i32     CLuaTable::GetBool(xst_castring& _strKey, bool* _pbOutBool)
    {
        ItemItr Itr = m_mTable.lower_bound(_strKey);
        //If resource exists
        if( Itr != m_mTable.end() && !(m_mTable.key_comp()(_strKey, Itr->first)) )
        {
                (*_pbOutBool) = Itr->second.uData.bValue;
                return RESULT::OK;
        }

        return RESULT::FAILED;
    }

    bool CLuaTable::Get(xst_castring& _strKey, const bool& _bDefault)
    {
        STableData& Data = Get( _strKey );
        if( Data.eType == BOOL )
                return Data.uData.bValue;
        return _bDefault;
    }

    i32 CLuaTable::Get(xst_castring& _strKey, ci32& _iDefault)
    {
        STableData& Data = Get( _strKey );
        if( Data.eType == INT )
                return Data.uData.iValue;
        return _iDefault;
    }

    f32 CLuaTable::Get(xst_castring& _strKey, cf32& _fDefault)
    {
        STableData& Data = Get( _strKey );
        if( Data.eType == FLOAT )
                return Data.uData.fValue;
        return _fDefault;
    }

    lpcastr CLuaTable::Get(xst_castring& _strKey, lpcastr _lpszDefault)
    {
        STableData& Data = Get( _strKey );
        if( Data.eType == STRING )
                return Data.uData.strValue.strText;
        return _lpszDefault;
    }

}//xst
