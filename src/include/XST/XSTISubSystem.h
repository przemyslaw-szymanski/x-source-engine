#ifndef XST_SUB_SYSTEM_H
#define XST_SUB_SYSTEM_H

#include "XSTCommonInclude.h"
#include "XSTCommonTypes.h"

namespace XST
{
	class ISubSystem
	{
		public:

									ISubSystem() { }
			virtual					~ISubSystem() { }

			XST_DECLPV(i32,			Init() );
			XST_DECLV(void,			Destroy() {} );
			XST_DECLV(void,			Reset() {} );
	};

}//xst

#endif//XST_SUB_SYSTEM_H