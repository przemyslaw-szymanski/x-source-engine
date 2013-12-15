#include "../../include/XST/XSTCScript.h"
#include "../../include/XST/XSTCTextFileReader.h"
#include "XSTCAsmScript.h"
#include "../../include/XSt/XSTCLogger.h"

#define XST_SCRIPT_ADD_KEYWORD(_code, _type) \
	m_mKeywords.insert( KeywordMap::value_type( _code, _type ) )
#define XST_SCRIPT_ADD_OPERATOR(_code, _type) \
	m_mOperators.insert( OperatorMap::value_type( _code, _type ) )

namespace XST
{
	struct Keywords
	{
		enum KEYWORD
		{
			/*INT = XST_SCRIPT_CODE( "int" ),
			FLOAT = XST_SCRIPT_CODE( "float" ),
			STRING = XST_SCRIPT_CODE( "string" ),
			BOOL = XST_SCRIPT_CODE( "bool" ),
			FUNCTION = XST_SCRIPT_CODE( "function" ),
			FOR = XST_SCRIPT_CODE( "for" ),
			WHILE = XST_SCRIPT_CODE( "while" ),
			DO = XST_SCRIPT_CODE( "do" ),
			CLASS = XST_SCRIPT_CODE( "class" ),
			IF = XST_SCRIPT_CODE( "if" ),
			ELSE = XST_SCRIPT_CODE( "else" )*/
		};
	};

	struct KeywordTypes
	{
		enum KEYWORD_TYPE
		{
			OPERATOR,
			DECLARATION,
			STATEMENT
		};
	};

	/*struct Operators
	{
		enum OPERATOR
		{
			EQUALS = XST_SCRIPT_CODE( "==" ),
			LESS = XST_SCRIPT_CODE( "<" ),
			GREATER = XST_SCRIPT_CODE( ">" ),
			LESS_EQUALS = XST_SCRIPT_CODE( "<=" ),
			GREATER_EQUALS = XST_SCRIPT_CODE( ">=" ),
			NOT_EQUALS = XST_SCRIPT_CODE( "!=" ),
			NOT = XST_SCRIPT_CODE( "!" ),
			BEGIN = XST_SCRIPT_CODE( "{" ),
			END = XST_SCRIPT_CODE( "}" ),
			ROUND_BRACKET_OPEN = XST_SCRIPT_CODE( "(" ),
			ROUND_BRACKET_CLOSE = XST_SCRIPT_CODE( ")" ),
			SQUARE_BRACKET_OPEN = XST_SCRIPT_CODE( "[" ),
			SQUARE_BRACKET_CLOSE = XST_SCRIPT_CODE( "]" ),
			AND = XST_SCRIPT_CODE( "&&" ),
			OR = XST_SCRIPT_CODE( "||" ),
			PLUS = XST_SCRIPT_CODE( "+" ),
			MINUS = XST_SCRIPT_CODE( "-" ),
			MUL = XST_SCRIPT_CODE( "*" ),
			DIV = XST_SCRIPT_CODE( "/" ),
			ASSIGNMENT = XST_SCRIPT_CODE( "=" )
		};
	};*/

	CScript::CScript()
	{
		/*XST_SCRIPT_ADD_KEYWORD( Keywords::INT, KeywordTypes::DECLARATION  );
		XST_SCRIPT_ADD_KEYWORD( Keywords::FLOAT, KeywordTypes::DECLARATION );
		XST_SCRIPT_ADD_KEYWORD( Keywords::STRING, KeywordTypes::DECLARATION );
		XST_SCRIPT_ADD_KEYWORD( Keywords::BOOL, KeywordTypes::DECLARATION );
		XST_SCRIPT_ADD_KEYWORD( Keywords::FUNCTION, KeywordTypes::DECLARATION );
		XST_SCRIPT_ADD_KEYWORD( Keywords::FOR, KeywordTypes::STATEMENT );
		XST_SCRIPT_ADD_KEYWORD( Keywords::WHILE, KeywordTypes::STATEMENT );
		XST_SCRIPT_ADD_KEYWORD( Keywords::DO, KeywordTypes::STATEMENT );
		XST_SCRIPT_ADD_KEYWORD( Keywords::CLASS, KeywordTypes::DECLARATION );
		XST_SCRIPT_ADD_KEYWORD( Keywords::IF, KeywordTypes::STATEMENT );
		XST_SCRIPT_ADD_KEYWORD( Keywords::ELSE, KeywordTypes::STATEMENT );
		
		XST_SCRIPT_ADD_KEYWORD( Operators::AND, KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( Operators::ASSIGNMENT, KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( Operators::BEGIN, KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( Operators::DIV, KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( Operators::END, KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( Operators::EQUALS, KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( Operators::GREATER, KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( Operators::GREATER_EQUALS, KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( Operators::LESS, KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( Operators::LESS_EQUALS, KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( Operators::MINUS, KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( Operators::MUL, KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( Operators::NOT, KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( Operators::NOT_EQUALS, KeywordTypes::OPERATOR);
		XST_SCRIPT_ADD_KEYWORD( Operators::OR, KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( Operators::PLUS, KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( Operators::ROUND_BRACKET_CLOSE, KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( Operators::ROUND_BRACKET_OPEN, KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( Operators::SQUARE_BRACKET_CLOSE, KeywordTypes::OPERATOR );
		XST_SCRIPT_ADD_KEYWORD( Operators::SQUARE_BRACKET_OPEN, KeywordTypes::OPERATOR );

		XST_SCRIPT_ADD_OPERATOR( Operators::AND, SOpInfo( 2 ) );
		XST_SCRIPT_ADD_OPERATOR( Operators::ASSIGNMENT, SOpInfo( 2 ) );
		XST_SCRIPT_ADD_OPERATOR( Operators::BEGIN, SOpInfo( 0 ) );
		XST_SCRIPT_ADD_OPERATOR( Operators::DIV, SOpInfo( 2 ) );
		XST_SCRIPT_ADD_OPERATOR( Operators::END, SOpInfo( 0 ) );
		XST_SCRIPT_ADD_OPERATOR( Operators::EQUALS, SOpInfo( 2 ) );
		XST_SCRIPT_ADD_OPERATOR( Operators::GREATER, SOpInfo( 2 ) );
		XST_SCRIPT_ADD_OPERATOR( Operators::GREATER_EQUALS, SOpInfo( 2 ) );
		XST_SCRIPT_ADD_OPERATOR( Operators::LESS, SOpInfo( 2 ) );
		XST_SCRIPT_ADD_OPERATOR( Operators::LESS_EQUALS, SOpInfo( 2 ) );
		XST_SCRIPT_ADD_OPERATOR( Operators::MINUS, SOpInfo( 2 ) );
		XST_SCRIPT_ADD_OPERATOR( Operators::MUL, SOpInfo( 2 ) );
		XST_SCRIPT_ADD_OPERATOR( Operators::NOT, SOpInfo( 1 ) );
		XST_SCRIPT_ADD_OPERATOR( Operators::NOT_EQUALS, SOpInfo( 2 ) );
		XST_SCRIPT_ADD_OPERATOR( Operators::OR, SOpInfo( 2 ) );
		XST_SCRIPT_ADD_OPERATOR( Operators::PLUS, SOpInfo( 2 ) );
		XST_SCRIPT_ADD_OPERATOR( Operators::ROUND_BRACKET_CLOSE, SOpInfo( 0 ) );
		XST_SCRIPT_ADD_OPERATOR( Operators::ROUND_BRACKET_OPEN, SOpInfo( 0 ) );
		XST_SCRIPT_ADD_OPERATOR( Operators::SQUARE_BRACKET_CLOSE, SOpInfo( 0 ) );
		XST_SCRIPT_ADD_OPERATOR( Operators::SQUARE_BRACKET_OPEN, SOpInfo( 0 ) );*/
		
	}

	CScript::~CScript()
	{
	}

	i32 CScript::Load(xst_castring &strFileName)
	{
		xst_astring strExt = XST::IFile::GetFileExtension( strFileName );
		XST::CTextFileReader Reader;
		if( Reader.Open( strFileName ) != RESULT::OK )
		{
			XST_LOG_ERR( "Failed to open file: " << strFileName );
			return RESULT::FAILED;
		}

		if( Reader.ReadAll() != RESULT::OK )
		{
			XST_LOG_ERR( "Failed to read file: " << strFileName );
			return RESULT::FAILED;
		}

		if( Reader.GetEncoding() != XST::ITextFile::ANSI )
		{
			XST_LOG_ERR( "Only ascii encoding is supported for xst scripts" );
			return RESULT::FAILED;
		}

		if( strExt == "xsp" )
		{
			//Parse text to bytecode
		}
		else if( strExt == "xsa" )
		{
			//Parse bytecode to binary
			CAsmScript ASpt;
			ASpt.Parse( Reader.GetAsciiData(), Reader.GetSize() );
		}
		else if( strExt == "xsb" )
		{
			//Parse binary
		}
		return RESULT::OK;
	}
}//xst