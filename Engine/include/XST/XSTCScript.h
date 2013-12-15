#ifndef XST_CSCRIPT_H
#define XST_CSCRIPT_H

#include "XSTCommonInclude.h"
#include "XSTCommonTypes.h"

#define XST_SCRIPT_CODE(_name) XST::CHash::GetCRC( (_name) )

namespace XST
{
	using namespace XST::Types;
	class CScript
	{
		protected:

			struct SOpInfo
			{
				SOpInfo(cu8& byAC) : byArgCount( byAC ) {}

				u8 byArgCount;
			};

			typedef i32								KeywordType;
			typedef xst_map< ul32, KeywordType >	KeywordMap;
			typedef i32								OperatorInfo;
			typedef xst_map< ul32, OperatorInfo >	OperatorMap;

		public:

									CScript();
			virtual					~CScript();											

					i32				Load(xst_castring& strFileName);

		protected:

					i32				Parse(lpcastr pData, ul32 ulDataSize);
					i32				SyntaxAnalyzer();
					i32				LexicalAnalyzer();
					i32				Linker();

		protected:
			
			static KeywordMap		m_mKeywords;
			static OperatorMap		m_mOperators;

	};

}//xst

#endif