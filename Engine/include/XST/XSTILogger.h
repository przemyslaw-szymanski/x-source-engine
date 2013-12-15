#ifndef XST_ILOGGER_H
#define XST_ILOGGER_H

#include "XSTCommon.h"

namespace XST
{

	class ILogger
	{

		public:

			virtual					~ILogger() {}

			XST_DECLPV(i32,			Init(xst_castring& strFileName));
			XST_DECLPV(i32,			Write(xst_castring& strMessage));
			XST_DECLPV(i32,			WriteError(xst_castring& strMessage));
			XST_DECLPV(i32,			WriteWarning(xst_castring& strMessage));
			//XST_DECLPV(i32,			Write(lpcastr _lpszFormat, ...));
			//XST_DECLPV(i32,			Write(lpcastr _lpszFunction, const i32& _iLine, lpcastr _lpszFormat, ...));

	};

}//xst

/*
va_list vlArgs;
				va_start( vlArgs, _lpszFormat );
				ch8 achBuff[1024];
				vsprintf_s(achBuff, 1024, _lpszFormat, vlArgs);
				va_end( vlArgs );

				m_strError = achBuff;
*/

#endif
