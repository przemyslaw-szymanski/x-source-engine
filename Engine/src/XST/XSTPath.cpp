#include "../../include/XST/XSTPath.h"
#include "../../include/XST/XSTIFile.h"

namespace XST
{
	bool Path::IsFileExists(xst_castring &strPath)
	{
		return XST::IFile::IsFileExists( strPath );
	}

	xst_astring Path::GetFileName(xst_castring& strPath)
	{
		//Find last index of \ or /
		u32 uId = strPath.find_last_of( "\\/" );
		if( uId == xst_castring::npos )
		{
			return strPath;
		}

		return strPath.substr( uId + 1 );
	}

	xst_astring	Path::GetParentDirectoryPath(xst_castring& strPath)
	{
		//Find last index of \ or /
		u32 uId = strPath.find_last_of( "\\/" );
		if( uId == xst_castring::npos )
		{
			return strPath;
		}

		return strPath.substr( 0, uId );
	}

	i32 Path::GetParentDirPathAndFileName(xst_castring& strPath, xst_astring* pDirPathOut, xst_astring* pFileNameOut)
	{
		u32 uId = strPath.find_last_of( "\\/" );
		if( uId == xst_castring::npos )
		{
			(*pDirPathOut) = strPath;
			(*pFileNameOut) = "";
		}

		(*pDirPathOut) = strPath.substr( 0, uId );
		(*pFileNameOut) = strPath.substr( uId + 1 );

		return RESULT::OK;
	}

	xst_astring Path::Combine(xst_castring& strLeft, xst_castring& strRight)
	{
		xst_astring str( strLeft + "\\" );
		str += strRight;
		return str;
	}

}//xst