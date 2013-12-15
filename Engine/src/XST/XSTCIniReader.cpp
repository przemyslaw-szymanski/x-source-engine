#include "../../include/XST/XSTCIniReader.h"

namespace XST
{
    CIniReader::CIniReader()
    {
    }

	CIniReader::CIniReader(lpcastr _lpszFileName) : m_strFileName(_lpszFileName)
	{
	}

    u32		CIniReader::ReadInt(lpcastr _lpszAppName, lpcastr _lpszKeyName, i32 _iDefault)
    {
        xst_assert(m_strFileName.length() != 0, "FileName not set");
        #if defined (XST_WINDOWS)
            return ::GetPrivateProfileIntA(_lpszAppName, _lpszKeyName, _iDefault, m_strFileName.c_str());
        #else
            return 0;
        #endif
    }

    u32		CIniReader::ReadString(lpcastr _lpszAppName, lpcastr _lpszKeyName, lpcastr _lpszDefault, lpastr _achReturn, ul32 _uiReturnSize)
    {
        xst_assert(m_strFileName.length() != 0, "FileName not set");
        #if defined (XST_WINDOWS)
            return ::GetPrivateProfileStringA(_lpszAppName, _lpszKeyName, _lpszDefault, _achReturn, _uiReturnSize, m_strFileName.c_str());
        #else
            return 0;
        #endif
    }
}//xst
