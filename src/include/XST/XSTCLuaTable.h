#ifndef XST_CLUA_TABLE_H
#define XST_CLUA_TABLE_H

#include "XSTCLuaCall.h"

namespace XST
{
	class CLuaApi;

	class CLuaTable
    {

        public:

                enum
                {
					MAX_STRING_LENGTH = 256
                };

                enum DATA_TYPE
                {
                    NONE = -1,
                    NIL = 0,
                    NUMBER,
                    BOOL,
                    INT,
                    FLOAT,
                    STRING,
                    FUNCTION,
                    TABLE,
					USER_DATA
                };

                struct SString
                {
                    ch8             strText[MAX_STRING_LENGTH];
                    u16             uiLength;

                    xst_fi lpcastr operator=(ch8* _lpszStr)
                    {
                            ch8* pDst = strText;
                            uiLength = 0;
                            while( ( *pDst++ = *_lpszStr++ ) )
                            {
                                    ++uiLength;
                            }

                            return pDst;
                    }

                    xst_fi lpcastr operator=(lpcastr _lpszStr)
                    {
                            ch8* pDst = strText;
                            uiLength = 0;
                            while( ( *pDst++ = *_lpszStr++ ) )
                            {
                                    ++uiLength;
                            }

                            return pDst;
                    }
                };

                typedef union UTableData
                {
                    bool		bValue;
                    i32			iValue;
                    f32			fValue;
                    SString		strValue;
					void*		pValue;
					CLuaTable*	pTable;

                    UTableData() {}
                    UTableData(const bool& _value) : bValue(_value) {}
                    UTableData(const i32& _value) : iValue(_value) {}
                    UTableData(const f32& _value) : fValue(_value) {}
                    UTableData(const SString& _value) : strValue(_value) {}
                    UTableData(lpcastr _value) { strValue = _value; }
					UTableData(CLuaTable* _value) : pTable(_value) {}
					UTableData(void* _value) : pValue(_value) {}
                    xst_fi void operator=(const bool& _bValue) { bValue = _bValue; }
                    xst_fi void operator=(ci32& _iValue) { iValue = _iValue; }
                    xst_fi void operator=(cf32& _fValue) { fValue = _fValue; }
                    xst_fi void operator=(const SString& _strValue) { strValue = _strValue; }
					xst_fi void operator=(CLuaTable* _pValue) { pTable = _pValue; }
					xst_fi void operator=(void* _pValue) { pValue = _pValue; }

                }TableData;

                struct STableData
                {
                    STableData() : eType(NONE) {}
                    STableData(const DATA_TYPE& _eType, const TableData& _uData) : eType(_eType), uData(_uData) {}
                    STableData(const DATA_TYPE& _eType, const bool& _value) : eType(_eType), uData(_value) { }
                    STableData(const DATA_TYPE& _eType, const i32& _value) : eType(_eType), uData(_value) {}
                    STableData(const DATA_TYPE& _eType, const f32& _value) : eType(_eType), uData(_value) {}
                    STableData(const DATA_TYPE& _eType, const SString& _value) : eType(_eType), uData(_value) {}
					STableData(const DATA_TYPE& _eType, CLuaTable* _value) : eType( _eType ), uData( _value ) {}
					STableData(const DATA_TYPE& _eType, void* _value) : eType( _eType ), uData( _value ) {}
                    DATA_TYPE eType;
                    TableData uData;
                };


                typedef STableData								Property;
                typedef xst_map<xst_astring, Property>          Table;
                typedef xst_pair<xst_astring, Property>         TablePair;
                typedef xst_map<xst_astring, CLuaTable*>        TablesTable;
                typedef xst_pair<xst_astring, CLuaTable*>       TablesTablePair;
                typedef xst_vector<CLuaTable*>					TableVec;

                typedef Table::iterator							ItemItr;
                typedef Table::const_iterator					TableConstItr;
                typedef TablesTable::const_iterator				ChildTableItr;

        public:

												CLuaTable(xst_castring& _strName);
												~CLuaTable();

                                void			Clear();

								i32				Create(CLuaApi* pApi, lpcastr lpszTableName);

                xst_fi  xst_castring&			GetTableName()
												{ return m_strTableName; }

                                void			Inherit(const CLuaTable* _pTable);

                                i32				GetString(xst_castring& _strKey, SString* _pstrOutString);

								i32				GetString(xst_castring& _strKey, xst_astring* pstrStringOut);

                                i32				GetFloat(xst_castring& _strKey, f32* _pfOutFloat);

                                i32				GetInt(xst_castring& _strKey, i32* _piOutInt);

                                i32				GetBool(xst_castring& _strKey, bool* _pbOutBool);


                xst_fi  STableData&             Get(xst_castring& _strKey) 
												{ return m_mTable[_strKey]; }

                                bool			Get(xst_castring& _strKey, const bool& _bDefault);
                                i32				Get(xst_castring& _strKey, ci32& _iDefault);
                                f32				Get(xst_castring& _strKey, cf32& _fDefault);
                                lpcastr			Get(xst_castring& _strKey, lpcastr _lpszDefault);

                xst_fi  SString&                GetString(xst_castring& _strKey)
												{ return m_mTable[_strKey].uData.strValue; }

                xst_fi  f32						GetFloat(xst_castring& _strKey)
												{ return m_mTable[_strKey].uData.fValue; }

                xst_fi  i32						GetInt(xst_castring& _strKey)
												{ return m_mTable[_strKey].uData.iValue; }

                xst_fi  bool                    GetBool(xst_castring& _strKey)
												{ return m_mTable[_strKey].uData.bValue; }

                xst_fi  TableConstItr			GetBeginIterator() const
												{ return m_mTable.begin(); }

                xst_fi  TableConstItr			GetEndIterator() const
												{ return m_mTable.end(); }

                /*xst_fi        void                    SetString(xst_castring& _strKey, const SString& _strValue)
                                                                { m_mTable[ _strKey ] = STableData( STRING, _strValue ); }*/

                                void			SetString(xst_castring& _strKey, lpcastr _lpszString, const u32& _uiStringLength);
                                void			SetString(xst_castring& _strKey, lpcastr _lpszString);

                                void			SetString(xst_castring& _strKey, xst_castring& _strValue);


                xst_fi  void                    Set(lpcastr _strKey, ci32& _value)
												{ m_mTable[ (_strKey == 0)? XST::StringUtil::ToString(m_ulID++) : _strKey ] = STableData( INT, _value ); }

                xst_fi  void                    Set(lpcastr _strKey, cf32& _value)
												{ m_mTable[ (_strKey == 0)? XST::StringUtil::ToString(m_ulID++) : _strKey ] = STableData( FLOAT, _value ); }

                xst_fi  void                    Set(lpcastr _strKey, const bool& _value)
												{ m_mTable[ (_strKey == 0)? XST::StringUtil::ToString(m_ulID++) : _strKey ] = STableData( BOOL, _value ); }

                xst_fi  void                    Set(lpcastr _strKey, const SString& _value)
												{ m_mTable[ (_strKey == 0)? XST::StringUtil::ToString(m_ulID++) : _strKey ] = STableData( STRING, _value ); }

                xst_fi  void                    Set(lpcastr _strKey, lpcastr _value)
												{ m_mTable[ (_strKey == 0)? XST::StringUtil::ToString(m_ulID++) : _strKey ] = STableData( STRING, _value ); }

                xst_fi  void                    SetFunction(lpcastr _strKey, lpcastr _value)
												{ m_mTable[ (_strKey == 0)? XST::StringUtil::ToString(m_ulID++) : _strKey ] = STableData( FUNCTION, _value ); }

                xst_fi  void                    SetTable(lpcastr _strKey, lpcastr _value)
												{ m_mTable[ (_strKey == 0)? XST::StringUtil::ToString(m_ulID++) : _strKey ] = STableData( TABLE, _value ); }

                xst_fi  void                    Set(lpcastr _strKey, CLuaTable* _pTable)
												{ _pTable->m_pParentTable = this; m_mChildTables[ (_strKey == 0)? XST::StringUtil::ToString(m_ulID++) : _strKey ] = _pTable; }

                xst_fi  void                    Set(lpcastr _strKey, const TableData& _value)
												{ m_mTable[ (_strKey == 0)? XST::StringUtil::ToString(m_ulID++) : _strKey ] = STableData( NONE, _value ); }

				xst_fi  void                    Set(lpcastr _strKey, void* _value)
												{ m_mTable[ (_strKey == 0)? XST::StringUtil::ToString(m_ulID++) : _strKey ] = STableData( USER_DATA, (void*)_value ); }

                xst_fi  CLuaTable*              GetParentTable() const
												{ return m_pParentTable; }

                xst_fi  CLuaTable*              GetChildTable(xst_castring& _strTableName) 
												{ return m_mChildTables[ _strTableName ]; }

                xst_fi  const TablesTable&      GetChildTableMap() const
												{ return m_mChildTables; }

                xst_fi  const TableVec&			GetChildTables() const
                                                { return m_vChildTables; }

                xst_fi  const Table&			GetTableData() const
												{ return m_mTable; }

						i32						Create(CLuaApi* pApi, CLuaTable* pParentTable, lpcastr lpszTableName);

		private:

						i32						_GetTableInfo(CLuaApi* pApi, CLuaTable* pParentTable, lpcastr lpszTableName);

        private:
        
                Table           m_mTable;
                TablesTable     m_mChildTables;
                CLuaTable*      m_pParentTable;
                TableVec        m_vChildTables;

                ul32            m_ulID;
                xst_astring     m_strTableName;

    };


}//xst

#endif