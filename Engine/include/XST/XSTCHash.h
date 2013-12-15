#ifndef XST_CHASH_H
#define XST_CHASH_H

#include "XSTCommon.h"


#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define xst_get16bits(d) ( *( (const u16 *) (d) ) )
#endif

#if !defined (xst_get16bits)
#define xst_get16bits(d) ((((u32)(((const uint8_t *)(d))[1])) << 8) +(u32)(((const uint8_t *)(d))[0]) )
#endif


namespace XST
{
	class CHash
	{
		public:

			static	ul32	GetCRC(lpcastr _lpszString, u32 _uiStringLength);

			static	ul32	GetCRC(lpcwstr _lpszString, u32 _uiStringLength);

			static xst_fi ul32 GetCRC(xst_castring& _strString)
			{
				return GetCRC(_strString.c_str(), _strString.length());
			}

			static xst_fi ul32 GetCRC(xst_cwstring& _strString)
			{
				return GetCRC(_strString.c_str(), _strString.length());
			}

	};
}//XST

#endif //XST_CHASH_H
