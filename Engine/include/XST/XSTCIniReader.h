#ifndef XST_CINI_READER_H
#define XST_CINI_READER_H

#include "XSTCommon.h"

namespace XST
{
	class CIniReader
	{
		public:

						CIniReader();
			explicit	CIniReader(lpcastr _lpszFileName);

			u32		ReadInt(lpcastr _lpszAppName, lpcastr _lpszKeyName, i32 _iDefault);

			u32		ReadString(lpcastr _lpszAppName, lpcastr _lpszKeyName, lpcastr _lpszDefault, lpastr _achReturn, ul32 _uiReturnSize);


		private:

			xst_astring	m_strFileName;

	};


}//XST

#endif
