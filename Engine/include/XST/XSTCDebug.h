#ifndef XST_CDEBUG_H
#define XST_CDEBUG_H

#include "XSTCommon.h"

namespace XST
{

	class XST_API CDebug
	{
		public:

			static	void    DumpMemoryLeaks();

			static	void	PrintDebug(xst_castring& _strText);

			static	void	PrintDebug(xst_cwstring& _strText);

			static	void	PrintDebugLN(xst_castring& _strText);

			static	void	PrintDebugLN(xst_cwstring& _strText);
	};

}//XST


#endif//XST_CDEBUG_H
