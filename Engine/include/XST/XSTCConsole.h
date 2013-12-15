#ifndef XST_CCONSOLE_H
#define XST_CCONSOLE_H


#include "XSTCommonInclude.h"
#include "XSTCommonTypes.h"
#include "XSTCStringUtil.h"

namespace XST
{

	class CConsole
	{
		public:

#if defined(XST_VISUAL_STUDIO)

			static inline void Write(lpcastr _lpszText)
			{
				OutputDebugStringA(_lpszText);
			}

			static inline void Write(const xst_astring& _strText)
			{
				OutputDebugStringA(_strText.data());
			}

			template<typename _T_>
			static inline void Write(_T_ _tData)
			{
				OutputDebugStringA(CStringUtil::ToString(_tData).c_str());
			}


			static inline void WriteLN(lpcastr _lpszText)
			{
				OutputDebugStringA(_lpszText);
				OutputDebugStringA("\n");
			}

			template<typename _T_>
			static inline void WriteLN(_T_ _tData)
			{
				OutputDebugStringA(StringUtil::ToString(_tData).c_str());
				OutputDebugStringA("\n");
			}

			static inline void WriteLN(xst_castring& _strText)
			{
				OutputDebugStringA(_strText.data());
				OutputDebugStringA("\n");
			}

			static inline void	Write(lpcwstr _lpszText)
			{
				OutputDebugStringW(_lpszText);
			}

			static inline void Write(const xst_wstring& _strText)
			{
				OutputDebugStringW(_strText.data());
			}

			static inline void WriteLN(lpcwstr _lpszText)
			{
				OutputDebugStringW(_lpszText);
				OutputDebugStringW(L"\n");
			}

			static inline void WriteLN(const xst_wstring& _strText)
			{
				OutputDebugStringW(_strText.data());
				OutputDebugStringW(L"\n");
			}
#else

			static inline void	Write(lpcastr _lpszText)
			{

			}

			static inline void WriteLN(lpcastr _lpszText)
			{

			}

			static inline void	Write(lpcwstr _lpszText)
			{

			}

			static inline void WriteLN(lpcwstr _lpszText)
			{

			}

#endif

	};

}//XST



#endif//XST_CCONSOLE_H
