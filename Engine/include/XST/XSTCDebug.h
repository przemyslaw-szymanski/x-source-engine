#ifndef XST_CDEBUG_H
#define XST_CDEBUG_H

#include "XSTCommon.h"

namespace XST
{

	class XST_API CDebug
	{
		public:

			static	void    DumpMemoryLeaks();

            static	void	PrintDebug(lpcastr _strText);

			static	void	PrintDebug(lpcwstr _strText);

			static	void	PrintDebugLN(lpcastr _strText);

			static	void	PrintDebugLN(lpcwstr _strText);

			static	
            xst_fi  void	PrintDebug( xst_castring& _strText )
                            { PrintDebug( _strText.c_str() ); }

			static	
            xst_fi  void	PrintDebug( xst_cwstring& _strText )
                            { PrintDebug( _strText.c_str() ); }

			static	
            xst_fi  void	PrintDebugLN( xst_castring& _strText )
                            { PrintDebugLN( _strText.c_str() ); }

			static	
            xst_fi  void	PrintDebugLN( xst_cwstring& _strText )
                            { PrintDebugLN( _strText.c_str() ); }
	};

}//XST


#endif//XST_CDEBUG_H
