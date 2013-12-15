#include "XSTCAsmScript.h"
#include "../../include/XST/XSTCStringUtil.h"
#include "../../include/XST/XSTCLogger.h"

#define XST_SCRIPT_ADD_KEYWORD(_name, _type) \
	m_mKeywords.insert( KeywordMap::value_type( _name, _type ) )
#define XST_SCRIPT_ADD_OPERATOR(_name, _type) \
	m_mOperators.insert( OperatorMap::value_type( _name, _type ) )
#define XST_SCRIPT_ADD_SIGN(_name, _type) \
	m_mSigns.insert( SignMap::value_type( _name, _type ) )
#define XST_SCRIPT_ADD_VARIABLE_TYPE(_name, _type) \
	m_mVarTypes.insert( VariableTypeMap::value_type( _name, _type ) )

namespace XST
{
	/*struct Keywords
	{
		enum KEYWORD
		{
			INT = XST_SCRIPT_CODE( "int" ),
			FLOAT = XST_SCRIPT_CODE( "float" ),
			STRING = XST_SCRIPT_CODE( "string" ),
			BOOL = XST_SCRIPT_CODE( "bool" ),
			FUNCTION = XST_SCRIPT_CODE( "fn" ),
			JMP = XST_SCRIPT_CODE( "jpm" ),
			TEST = XST_SCRIPT_CODE( "test" ),
			IFE = XST_SCRIPT_CODE( "ife" )
		};
	};*/

	struct KeywordTypes
	{
		enum KEYWORD_TYPE
		{
			OPERATOR,
			DECLARATION,
			STATEMENT
		};
	};

	

	struct Operators
	{
		enum OPERATOR
		{
			EQUALS = 1,
			LESS,
			GREATER,
			LESS_EQUALS,
			GREATER_EQUALS,
			NOT_EQUALS,
			NOT,
			ROUND_BRACKET_OPEN ,
			ROUND_BRACKET_CLOSE ,
			SQUARE_BRACKET_OPEN ,
			SQUARE_BRACKET_CLOSE ,
			AND ,
			OR,
			ADD,
			SUB,
			MUL,
			DIV,
			MOV
		};
	};

	struct Statements
	{
		enum STATEMENT
		{
			OPERATOR = 1,
			CONDITION,
			LOOP,
			DECLARATION
		};
	};

	struct SignTypes
	{
		enum SIGN
		{
			COMMA,
			SQUARE_BRACKET_OPEN,
			SQUARE_BRACKET_CLOSE,
			QUOTE
		};
	};

	void CAsmScriptFunction::ClearStack()
	{
		m_mLocalVariables.clear();
		m_sParams.clear();
	}

	CAsmScript::CAsmScript()
	{
		XST_SCRIPT_ADD_KEYWORD( "int", KeywordTypes::DECLARATION );
		XST_SCRIPT_ADD_KEYWORD( "float", KeywordTypes::DECLARATION );
		XST_SCRIPT_ADD_KEYWORD( "string", KeywordTypes::DECLARATION );
		XST_SCRIPT_ADD_KEYWORD( "bool", KeywordTypes::DECLARATION );
		XST_SCRIPT_ADD_KEYWORD( "fn", KeywordTypes::DECLARATION );
		XST_SCRIPT_ADD_KEYWORD( "jmp", KeywordTypes::STATEMENT );
		XST_SCRIPT_ADD_KEYWORD( "test", KeywordTypes::STATEMENT );
		XST_SCRIPT_ADD_KEYWORD( "ife", KeywordTypes::STATEMENT );
		XST_SCRIPT_ADD_KEYWORD( "ifne", KeywordTypes::STATEMENT );
		XST_SCRIPT_ADD_KEYWORD( "ifg", KeywordTypes::STATEMENT );
		XST_SCRIPT_ADD_KEYWORD( "ifl", KeywordTypes::STATEMENT );
		XST_SCRIPT_ADD_KEYWORD( "ifge", KeywordTypes::STATEMENT );
		XST_SCRIPT_ADD_KEYWORD( "ifle", KeywordTypes::STATEMENT );
		XST_SCRIPT_ADD_KEYWORD( "ifn", KeywordTypes::STATEMENT );
		XST_SCRIPT_ADD_KEYWORD( "call", KeywordTypes::STATEMENT );
		XST_SCRIPT_ADD_KEYWORD( "push", KeywordTypes::STATEMENT );
		XST_SCRIPT_ADD_KEYWORD( "pop", KeywordTypes::STATEMENT );
		XST_SCRIPT_ADD_KEYWORD( "ret", KeywordTypes::STATEMENT );

		XST_SCRIPT_ADD_KEYWORD( "eq", KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( "ls", KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( "gt", KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( "ge", KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( "le", KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( "ne", KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( "not", KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( "and", KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( "or", KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( "add", KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( "sub", KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( "mul", KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( "div", KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( "mov", KeywordTypes::OPERATOR );


		XST_SCRIPT_ADD_OPERATOR( "eq", SOpInfo( 2, Operators::EQUALS ) );
		XST_SCRIPT_ADD_OPERATOR( "ls", SOpInfo( 2, Operators::LESS ) );
		XST_SCRIPT_ADD_OPERATOR( "gt", SOpInfo( 2, Operators::GREATER ) );
		XST_SCRIPT_ADD_OPERATOR( "ge", SOpInfo( 2, Operators::GREATER_EQUALS ) );
		XST_SCRIPT_ADD_OPERATOR( "le", SOpInfo( 2, Operators::LESS_EQUALS ) );
		XST_SCRIPT_ADD_OPERATOR( "ne", SOpInfo( 2, Operators::NOT_EQUALS ) );
		XST_SCRIPT_ADD_OPERATOR( "not", SOpInfo( 1, Operators::NOT ) );
		XST_SCRIPT_ADD_OPERATOR( "and", SOpInfo( 2, Operators::AND ) );
		XST_SCRIPT_ADD_OPERATOR( "or", SOpInfo( 2 , Operators::OR ) );
		XST_SCRIPT_ADD_OPERATOR( "add", SOpInfo( 3, Operators::ADD ) );
		XST_SCRIPT_ADD_OPERATOR( "sub", SOpInfo( 3, Operators::SUB ) );
		XST_SCRIPT_ADD_OPERATOR( "mul", SOpInfo( 3, Operators::MUL ) );
		XST_SCRIPT_ADD_OPERATOR( "div", SOpInfo( 3, Operators::DIV ) );
		XST_SCRIPT_ADD_OPERATOR( "mov", SOpInfo( 2, Operators::MOV ) );

		XST_SCRIPT_ADD_SIGN( ",", SignTypes::COMMA );
		XST_SCRIPT_ADD_SIGN( "[", SignTypes::SQUARE_BRACKET_OPEN );
		XST_SCRIPT_ADD_SIGN( "]", SignTypes::SQUARE_BRACKET_CLOSE );
		XST_SCRIPT_ADD_SIGN( "\"", SignTypes::QUOTE );

		XST_SCRIPT_ADD_VARIABLE_TYPE( "int", VariableTypes::INT );
		XST_SCRIPT_ADD_VARIABLE_TYPE( "float", VariableTypes::FLOAT );
		XST_SCRIPT_ADD_VARIABLE_TYPE( "string", VariableTypes::STRING );
		XST_SCRIPT_ADD_VARIABLE_TYPE( "bool", VariableTypes::BOOL );
		
	}

	CAsmScript::~CAsmScript()
	{
	}

	i32 CAsmScript::RunFunction(CAsmScriptFunction* pFunc, xst_castring& strName)
	{
		Function& Func = pFunc->GetFunction( strName );
		
		for(u32 i = 0; i < Func.m_vStatements.size(); ++i)
		{
			switch( Func.m_vStatements[ i ].byType )
			{
				case Statements::DECLARATION:
				{
					Func.AddLocalVariable( Func.m_vStatements[ i ].aArgs[ 0 ].strName, Func.m_vStatements[ i ].aArgs[ 0 ].byType );			
				}
				break;

				case Statements::OPERATOR:
				{
					switch( Func.m_vStatements[ i ].byOpType )
					{
						case Operators::MOV:
						{
							SAsmScriptVariable* pSrcVar = xst_null, *pDstVar = xst_null;

							if( Func.m_vStatements[ i ].aArgs[ 0 ].byScope == SAsmScriptStatement::SArg::GLOBAL )
							{
								pDstVar = m_MainFunc.GetVariablePtr( Func.m_vStatements[ i ].aArgs[ 0 ].strName );
							}
							else if( Func.m_vStatements[ i ].aArgs[ 0 ].byScope == SAsmScriptStatement::SArg::LOCAL )
							{
								pDstVar = Func.GetVariablePtr( Func.m_vStatements[ i ].aArgs[ 0 ].strName );
							}

							if( Func.m_vStatements[ i ].aArgs[ 1 ].byScope == SAsmScriptStatement::SArg::GLOBAL )
							{
								pSrcVar = m_MainFunc.GetVariablePtr( Func.m_vStatements[ i ].aArgs[ 1 ].strName );
							}
							else if( Func.m_vStatements[ i ].aArgs[ 1 ].byScope == SAsmScriptStatement::SArg::LOCAL )
							{
								pSrcVar = Func.GetVariablePtr( Func.m_vStatements[ i ].aArgs[ 1 ].strName );
							}
							else if( Func.m_vStatements[ i ].aArgs[ 1 ].byScope == SAsmScriptStatement::SArg::CONSTANT )
							{
								if( pDstVar ) *pDstVar = (f32)atof( Func.m_vStatements[ i ].aArgs[ 1 ].strName.data() );
							}

							if( pSrcVar && pDstVar )
							{
								*pDstVar = *pSrcVar;
							}
						
						}
						break;

						case Operators::ADD:
						{
							SAsmScriptVariable* pVar1 = xst_null, *pVar2 = xst_null, *pDstVar = xst_null, ConstVar1, ConstVar2;

							if( Func.m_vStatements[ i ].aArgs[ 0 ].byScope == SAsmScriptStatement::SArg::GLOBAL )
							{
								pDstVar = m_MainFunc.GetVariablePtr( Func.m_vStatements[ i ].aArgs[ 0 ].strName );
							}
							else if( Func.m_vStatements[ i ].aArgs[ 0 ].byScope == SAsmScriptStatement::SArg::LOCAL )
							{
								pDstVar = Func.GetVariablePtr( Func.m_vStatements[ i ].aArgs[ 0 ].strName );
							}

							if( Func.m_vStatements[ i ].aArgs[ 1 ].byScope == SAsmScriptStatement::SArg::GLOBAL )
							{
								pVar1 = m_MainFunc.GetVariablePtr( Func.m_vStatements[ i ].aArgs[ 1 ].strName );
							}
							else if( Func.m_vStatements[ i ].aArgs[ 1 ].byScope == SAsmScriptStatement::SArg::LOCAL )
							{
								pVar1 = Func.GetVariablePtr( Func.m_vStatements[ i ].aArgs[ 1 ].strName );
							}
							else if( Func.m_vStatements[ i ].aArgs[ 1 ].byScope == SAsmScriptStatement::SArg::CONSTANT )
							{
								if( StringUtil::IsNumber( Func.m_vStatements[ i ].aArgs[ 1 ].strName ) )
								{
									ConstVar1 = SAsmScriptVariable( (f32)atof( Func.m_vStatements[ i ].aArgs[ 1 ].strName.data() ) );
								}
								else
								{
									ConstVar1 = SAsmScriptVariable( Func.m_vStatements[ i ].aArgs[ 1 ].strName.data() );
								}

								pVar1 = &ConstVar1;
							}

							if( Func.m_vStatements[ i ].aArgs[ 2 ].byScope == SAsmScriptStatement::SArg::GLOBAL )
							{
								pVar2 = m_MainFunc.GetVariablePtr( Func.m_vStatements[ i ].aArgs[ 2 ].strName );
							}
							else if( Func.m_vStatements[ i ].aArgs[ 2 ].byScope == SAsmScriptStatement::SArg::LOCAL )
							{
								pVar2 = Func.GetVariablePtr( Func.m_vStatements[ i ].aArgs[ 2 ].strName );
							}
							else if( Func.m_vStatements[ i ].aArgs[ 2 ].byScope == SAsmScriptStatement::SArg::CONSTANT )
							{
								if( StringUtil::IsNumber( Func.m_vStatements[ i ].aArgs[ 2 ].strName ) )
								{
									ConstVar2 = SAsmScriptVariable( (f32)atof( Func.m_vStatements[ i ].aArgs[ 2 ].strName.data() ) );
								}
								else
								{
									ConstVar2 = SAsmScriptVariable( Func.m_vStatements[ i ].aArgs[ 2 ].strName.data() );
								}

								pVar2 = &ConstVar2;
							}

							if( pDstVar && pVar1 && pVar2 )
							{
								pDstVar->Add( *pVar1, *pVar2 );
								XST::CDebug::PrintDebugLN( StringUtil::ToString( pDstVar->fValue ) );
							}
						}
						break;

					}//switch
				}
				break;

				case Statements::CONDITION:
				{
				}
				break;

				case Statements::LOOP:
				{
				}
				break;
			}//switch
		}//for

		Func.ClearStack();

		return RESULT::OK;
	}

	i32 CAsmScript::Parse(lpcastr pData, ul32 ulDataSize)
	{
		TokenLineVector vTokenLines;

		//Tokenize lines
		XST::StringUtil::TokensA vLines = XST::StringUtil::Split( pData, "\r\n" );
		for(u32 i = 0; i < vLines.size(); ++i)
		{
			vTokenLines.push_back( XST::StringUtil::Split( vLines[ i ], "\t, " ) );
		}

		i32 iResult = LexicalAnalyzer( vTokenLines );
		if( iResult != RESULT::OK )
		{
			return RESULT::FAILED;
		}

		iResult = SyntaxAnalyzer( vTokenLines );
		if( iResult != RESULT::OK )
		{
			return RESULT::FAILED;
		}

		return RESULT::OK;
	}

	i32 CAsmScript::LexicalAnalyzer(const CAsmScript::TokenLineVector& vTokenLines)
	{
		KeywordMap::iterator KeywordItr;

		for(u32 l = 0; l < vTokenLines.size(); ++l)
		{
			const TokenVector& vTokens = vTokenLines[ l ];
			if( vTokens.size() == 0 ) continue;

			//Find this keyword
			KeywordItr = m_mKeywords.find( vTokens[ 0 ] );
			//If no keyword found, return error
			if( KeywordItr == m_mKeywords.end() )
			{
				XST_LOG_ERR( "Unknown keyword: " << vTokens[ 0 ] << " at line: " << l );
				return RESULT::FAILED;
			}

			for(u32 t = 0; t < vTokenLines[ l ].size(); ++t)
			{
				
			}
		}

		return RESULT::OK;
	}

	xst_fi bool IsFunctionDecl(xst_castring& strToken)
	{
		return strToken == "fn";
	}

	i32 CAsmScript::SyntaxAnalyzer(const CAsmScript::TokenLineVector& vTokenLines)
	{
		KeywordMap::iterator KeywordItr;

		Function* pCurrFunc = &m_MainFunc;

		for(u32 l = 0; l < vTokenLines.size(); ++l)
		{
			const TokenVector& vTokens = vTokenLines[ l ];
			if( vTokens.size() == 0 ) continue;

			//Get keyword
			i32 iKeywordType = m_mKeywords[ vTokens[ 0 ] ];
			
			switch( iKeywordType )
			{
				case KeywordTypes::DECLARATION:
				{
					//If function declaration
					if( IsFunctionDecl( vTokens[ 0 ] ) )
					{
						if( vTokens.size() > 2 )
						{
							XST_LOG_ERR( "Too many parameters for function declaration at line: " << l );
							return RESULT::FAILED;
						}

						if( pCurrFunc->AddFunction( vTokens[ 1 ] ) != RESULT::OK )
						{
							XST_LOG_ERR( "[" << l << "]: Function: " << vTokens[ 1 ] << " already defined in function: " << pCurrFunc->GetName() );
							return RESULT::FAILED;
						}

						CAsmScriptFunction& Func = pCurrFunc->GetFunction( vTokens[ 1 ] );
						ul32 ulEndPos = l;
						if( ParseFunction( &Func, vTokenLines, l, &ulEndPos ) != RESULT::OK )
						{
							return RESULT::FAILED;
						}

						l = ulEndPos;
					}
					else
					{
						if( vTokens.size() < 2 ) 
						{
							XST_LOG_ERR( "Too few variables declared at line: " << l );
							return RESULT::FAILED;
						}
						

						//Get all declared variables
						for(u32 i = 1; i < vTokens.size(); ++i)
						{
							pCurrFunc->AddLocalVariable( vTokens[ i ], m_mVarTypes[ vTokens[ 0 ] ] );
						}
					}
				}
				break;


				case KeywordTypes::STATEMENT:
				{
					SAsmScriptStatement Stat;
					
					xst_castring& strStatement = vTokens[ 0 ];
					
					if( strStatement == "jmp" )
					{
					}
					else if( strStatement == "test" )
					{
					}
					else if( strStatement == "ife" )
					{
					}
					else if( strStatement == "ifne" )
					{
					}
					else if( strStatement == "ifg" )
					{
					}
					else if( strStatement == "ifl" )
					{
					}
					else if( strStatement == "ifle" )
					{
					}
					else if( strStatement == "ifn" )
					{
					}
					else if( strStatement == "ifge" )
					{
					}
					else if( strStatement == "call" )
					{
						if( vTokens.size() < 2 )
						{
							XST_LOG_ERR( "[" << l << "] Missing function name" );
							return RESULT::FAILED;
						}

						if( RunFunction( pCurrFunc, vTokens[ 1 ] ) != RESULT::OK )
						{
							return RESULT::FAILED;
						}
					}
					else if( strStatement == "push" )
					{
					}
					else if( strStatement == "pop" )
					{
					}
					else if( strStatement == "ret" )
					{
					}
					else
					{
						XST_LOG_ERR( "[" << l << "] Unknown statement: " << strStatement );
						return RESULT::FAILED;
					}
				}
				break;

			}
			
		}

		return RESULT::OK;
	}

	i32 CAsmScript::ParseFunction(CAsmScriptFunction* pFunc, const CAsmScript::TokenLineVector& vTokenLines, ul32 uCurrLine, ul32* pulEndBodyPosOut)
	{
		if( vTokenLines.size() <= uCurrLine )
		{
			XST_LOG_ERR( "[" << uCurrLine << "] No function body close in function: " << pFunc->GetName() );
			return RESULT::FAILED;
		}

		*pulEndBodyPosOut = vTokenLines.size();

		ul32 ulRetPos = 0;
		//Find function and ret
		for(ul32 l = uCurrLine + 1; l < vTokenLines.size(); ++l)
		{
			//Find function
			xst_castring& strToken = vTokenLines[ l ][ 0 ];
			if( strToken == "fn" )
			{
				if( vTokenLines[ l ].size() < 2 )
				{
					XST_LOG_ERR( "[" << l << "] No function name specified" );
					return RESULT::FAILED;
				}

				if( pFunc->AddFunction( vTokenLines[ l ][ 1 ] ) != RESULT::OK )
				{
					XST_LOG_ERR( "[" << l << "] Function: " << vTokenLines[ l ][ 1 ] << " already defined in function: " << pFunc->GetName() );
					return RESULT::FAILED;
				}

				XST::CAsmScriptFunction& Fun = pFunc->GetFunction( vTokenLines[ l ][ 1 ] );
				ul32 ulEndPos = l;
				if( ParseFunction( &Fun, vTokenLines, l, &ulEndPos ) != RESULT::OK )
				{
					return RESULT::FAILED;
				}

				l = ulEndPos;
			}
			else if( strToken == "ret" )
			{
				ulRetPos = l;
				*pulEndBodyPosOut = ulRetPos;
				break;
			}
		}

		xst_map< xst_astring, bool > LocalVarMap;

		for(ul32 l = uCurrLine + 1; l < ulRetPos; ++l )
		{
			SAsmScriptStatement Stat;
			
			const TokenVector& vTokens = vTokenLines[ l ];
			if( vTokens.size() == 0 ) continue;

			//Get keyword
			i32 iKeywordType = m_mKeywords[ vTokens[ 0 ] ];

			switch( iKeywordType )
			{
				case KeywordTypes::DECLARATION:
				{
					if( vTokens.size() < 2 )
					{
						XST_LOG_ERR( "[" << l << "] Too few arguments for variable declaration" );
						return RESULT::FAILED;
					}

					if( LocalVarMap.find( vTokens[ 1 ] ) != LocalVarMap.end() )
					{
						XST_LOG_ERR( "[" << l << "] Variable: " << vTokens[ 1 ] << " already declared in function: " << pFunc->GetName() );
						return RESULT::FAILED;
					}

					LocalVarMap.insert( xst_pair< xst_astring, bool >( vTokens[ 1 ], true ) );

					SAsmScriptStatement Stat;
					Stat.byType = Statements::DECLARATION;
					Stat.byArgCount = 1;
					Stat.aArgs[ 0 ].byScope = SAsmScriptStatement::SArg::LOCAL;
					Stat.aArgs[ 0 ].strName = vTokens[ 1 ];

					if( vTokens[ 0 ] == "int" )
					{
						Stat.aArgs[ 0 ].byType = VariableTypes::INT;
					}
					else if( vTokens[ 0 ] == "float" )
					{
						Stat.aArgs[ 0 ].byType = VariableTypes::FLOAT;
					}
					else if( vTokens[ 0 ] == "string" )
					{
						Stat.aArgs[ 0 ].byType = VariableTypes::STRING;
					}
					else if( vTokens[ 0 ] == "bool" )
					{
						Stat.aArgs[ 0 ].byType = VariableTypes::BOOL;
					}

					pFunc->AddStatement( Stat );
				}
				break;

				case KeywordTypes::OPERATOR:
				{
					const SOpInfo& OpInfo = m_mOperators[ vTokens[ 0 ] ];
					SAsmScriptStatement Stat;
					Stat.byArgCount = OpInfo.byArgCount;
					Stat.byType = Statements::OPERATOR;
					Stat.byOpType = OpInfo.byType;

					if( OpInfo.byArgCount > 3 ) 
					{
						XST_LOG_ERR( "[" << l << "] Too many arguments for operator: " << vTokens[ 0 ] );
						return RESULT::FAILED;
					}
					xst_map< xst_astring, bool >::iterator Itr;

					for(u32 i = 1; i < vTokens.size() && i <= OpInfo.byArgCount; ++i)
					{
						//If not local variable in function
						Itr = LocalVarMap.find( vTokens[ i ] );
						if( Itr == LocalVarMap.end() )
						{
							//If not global variable
							if( !m_MainFunc.IsVariableExists( vTokens[ i ] ) )
							{
								//If not constant variable
								if( !XST::StringUtil::IsNumber( vTokens[ i ] ) )
								{
									XST_LOG_ERR( "[" << l << "] Variable: " << vTokens[ i ] << " not declared in function: " << pFunc->GetName() << " or global scope" );
									return RESULT::FAILED;
								}
								else
								{
									Stat.aArgs[ i - 1 ].byScope = SAsmScriptStatement::SArg::CONSTANT;
									Stat.aArgs[ i - 1 ].strName = vTokens[ i ];
								}
							}
							else
							{
								Stat.aArgs[ i - 1 ].byScope = SAsmScriptStatement::SArg::GLOBAL;
								Stat.aArgs[ i - 1 ].strName = vTokens[ i ];
							}
						}
						else
						{
							Stat.aArgs[ i - 1 ].byScope = SAsmScriptStatement::SArg::LOCAL;
							Stat.aArgs[ i - 1 ].strName = vTokens[ i ];
						}
					}

					pFunc->AddStatement( Stat );

				}
				break;

				case KeywordTypes::STATEMENT:
				{
					SAsmScriptStatement Stat;
					
					xst_castring& strStatement = vTokens[ 0 ];
					
					if( strStatement == "jmp" )
					{
					}
					else if( strStatement == "test" )
					{
					}
					else if( strStatement == "ife" )
					{
					}
					else if( strStatement == "ifne" )
					{
					}
					else if( strStatement == "ifg" )
					{
					}
					else if( strStatement == "ifl" )
					{
					}
					else if( strStatement == "ifle" )
					{
					}
					else if( strStatement == "ifn" )
					{
					}
					else if( strStatement == "ifge" )
					{
					}
					else if( strStatement == "call" )
					{
						if( vTokens.size() < 2 )
						{
							XST_LOG_ERR( "[" << l << "] Missing function name" );
							return RESULT::FAILED;
						}

						if( RunFunction( pFunc, vTokens[ 1 ] ) != RESULT::OK )
						{
							return RESULT::FAILED;
						}
					}
					else if( strStatement == "push" )
					{
					}
					else if( strStatement == "pop" )
					{
					}
					else if( strStatement == "ret" )
					{
					}
					else
					{
						XST_LOG_ERR( "[" << l << "] Unknown statement: " << strStatement );
						return RESULT::FAILED;
					}

					pFunc->AddStatement( Stat );
				}
				break;
			
			}

		}

		return RESULT::OK;
	}

}//xst