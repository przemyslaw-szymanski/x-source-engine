#ifndef XST_EMPTY_LOGGER_H
#define XST_EMPTY_LOGGER_H

#include "XSTILogger.h"

namespace XST
{

	class CEmptyLogger : public ILogger
	{
		public:

			XST_DECLV(i32,			Init(xst_castring& _strFileName) { return 0; } );
			XST_DECLV(i32,			Write(xst_castring& _strMessage) { return 0; } );
			XST_DECLV(i32,			Write(lpcastr _lpszFormat, ...) { return 0; } );
			XST_DECLV(i32,			Write(lpcastr _lpszFormat, lpcastr _lpszTime, lpcastr _lpszFunction, ...) { return 0; } );
	};

}//xst

#endif
