#ifndef XST_CASM_SCRIPT_H
#define XST_CASM_SCRIPT_H

//#include "../../include/XSTXSTCScript.h"
#include "../../include/XST/XSTCommonTypes.h"
#include "../../include/XST/XSTCommonInclude.h"
#include "../../include/XST/XSTTCStack.h"

namespace XST
{
	using namespace XST::Types;

	struct VariableTypes
	{
		enum VARIABLE_TYPE
		{
			INT = 1,
			FLOAT,
			STRING,
			BOOL
		};
	};

	struct SAsmScriptVariable
	{
		typedef lpcastr string;
		union
		{
			i32		iValue;
			f32		fValue;
			bool	bValue;
			string	strValue;
		};

		VariableTypes::VARIABLE_TYPE eType;

		SAsmScriptVariable() {}
		SAsmScriptVariable(const i32& iVal) : iValue( iVal ), eType( VariableTypes::INT ) {}
		SAsmScriptVariable(const f32& fVal) : fValue( fVal ), eType( VariableTypes::FLOAT ) {}
		SAsmScriptVariable(const bool& bVal) : bValue( bVal ), eType( VariableTypes::BOOL ) {}
		SAsmScriptVariable(const string& strVal) : strValue( strVal ), eType( VariableTypes::STRING ) {}
		SAsmScriptVariable(VariableTypes::VARIABLE_TYPE eVarType) : eType( eVarType ) {}
		xst_fi void operator=(const i32& iVal) { iValue = iVal; }
		xst_fi void operator=(const f32& fVal) { fValue = fVal; }
		xst_fi void operator=(const bool& bVal) { bValue = bVal; }
		xst_fi void operator=(const string& strVal) { strValue = strVal; }
		xst_fi void operator=(const SAsmScriptVariable& Var) { if( Var.eType == VariableTypes::STRING ) strValue = Var.strValue; else fValue = Var.fValue; }
		xst_fi i32 operator+(const i32& iVal) { return iValue + iVal; }
		xst_fi f32 operator+(const f32& fVal) { return fValue + fVal; }
		xst_fi bool operator+(const bool& bVal) { return bValue + bVal; }
		//xst_fi void operator+(const string& strVal) { strValue = strVal; }
		xst_fi void Add(const SAsmScriptVariable& VarLeft, const SAsmScriptVariable& VarRight)
		{ fValue = VarLeft.fValue + VarRight.fValue; }

	};

	struct SAsmScriptStatement
	{
		struct SArg
		{
			enum
			{
				LOCAL = 1,
				GLOBAL,
				CONSTANT
			};

			xst_astring strName;
			u8			byScope;
			u8			byType;
		};

		u8		byArgCount;
		u8		byType;
		u8		byOpType;
		SArg	aArgs[ 3 ];
	};

	class CAsmScriptFunction
	{
		friend class CAsmScript;
		public:

			typedef SAsmScriptVariable							Variable;
			typedef CAsmScriptFunction							Function;
			typedef SAsmScriptStatement							Statement;
			typedef xst_map< xst_astring, SAsmScriptVariable >	VariableMap;
			typedef xst_map< xst_astring, i32 >					ParamMap;
			typedef xst_map< xst_astring, Function >			FunctionMap;
			typedef xst_vector< Statement >						StatementVector;

			xst_fi	void	DeclParam(xst_castring& strName, i32 iType)
							{ m_mParams.insert( ParamMap::value_type( strName, iType ) ); }

			xst_fi	i32		AddLocalVariable(xst_castring& strName, i32 iType)
			{
				VariableMap::iterator Itr = m_mLocalVariables.find( strName );
				if( Itr != m_mLocalVariables.end() ) return RESULT::FAILED;
				m_mLocalVariables.insert( VariableMap::value_type( strName, (VariableTypes::VARIABLE_TYPE)iType ) );
				return RESULT::OK;
			}

			xst_fi	i32	AddFunction(xst_castring& strName)
			{
				FunctionMap::iterator Itr = m_mFunctions.find( strName );
				if( Itr != m_mFunctions.end() )
				{
					return RESULT::FAILED;
				}
				m_mFunctions.insert( FunctionMap::value_type( strName, Function() ) );
				return RESULT::OK;
			}

			xst_fi	Function&	GetFunction(xst_castring& strName)
			{
				return m_mFunctions[ strName ];
			}

			xst_fi	bool	IsFunctionExists(xst_castring& strName)
			{
				return m_mFunctions.find( strName ) != m_mFunctions.end();
			}

			xst_fi	xst_castring&	GetName()
									{ return m_strName; }

			xst_fi	void			SetName(xst_castring& strName)
									{ m_strName = strName; }

			xst_fi	void			AddStatement(const Statement& Stat)
									{ m_vStatements.push_back( Stat ); }

			xst_fi	bool			IsVariableExists(xst_castring& strName)
									{ return m_mLocalVariables.find( strName ) != m_mLocalVariables.end(); }

			xst_fi	Variable&		GetVariable(xst_castring& strName)
									{ return m_mLocalVariables.find( strName )->second; }

			xst_fi	Variable*		GetVariablePtr(xst_castring& strName)
									{ return &m_mLocalVariables.find( strName )->second; }

					void			ClearStack();


		protected:

			VariableMap		m_mLocalVariables;
			FunctionMap		m_mFunctions;
			ParamMap		m_mParams;
			xst_astring		m_strName;
			StatementVector	m_vStatements;
			TCStack< Variable> m_sParams;
			u8				m_byParamCount;
	};

	class CAsmScript
	{
		public:

			struct SOpInfo
			{
				SOpInfo() : byArgCount( 0 ), byType( 0 ) {}
				SOpInfo(cu8& byAC, cu8& byT) : byArgCount( byAC ), byType( byT ) {}

				u8	byArgCount;
				u8	byType;	
			};

			typedef i32										KeywordType;
			typedef xst_map< xst_astring, KeywordType >		KeywordMap;
			typedef SOpInfo									OperatorInfo;
			typedef xst_map< xst_astring, OperatorInfo >	OperatorMap;
			typedef i32										SignType;
			typedef xst_map< xst_astring, SignType >		SignMap;

			typedef CAsmScriptFunction						Function;
			typedef xst_map< xst_astring, Function >		FunctionMap;

			typedef xst_vector< xst_astring >				TokenVector;
			typedef xst_vector< TokenVector >				TokenLineVector;

			typedef xst_map< xst_astring, i32 >				VariableTypeMap;

		public:

								CAsmScript();
			virtual				~CAsmScript();

					i32			Parse(lpcastr pData, ul32 ulDataSize);

		protected:

					i32			LexicalAnalyzer(const TokenLineVector& vTokenLines);

					i32			SyntaxAnalyzer(const TokenLineVector& vTokenLines);

					i32			ParseFunction(CAsmScriptFunction* pFunc, const CAsmScript::TokenLineVector& vTokenLines, ul32 uCurrLine, ul32* pulEndBodyPosOut);

					i32			RunFunction(Function* pFunc, xst_castring& strName);

		protected:

			KeywordMap		m_mKeywords;
			OperatorMap		m_mOperators;
			SignMap			m_mSigns;
			
			FunctionMap		m_mFunctions;
			Function		m_MainFunc;

			VariableTypeMap	m_mVarTypes;
	};

}//xst

#endif