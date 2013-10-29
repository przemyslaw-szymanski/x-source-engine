#ifndef XST_PATH_H
#define XST_PATH_H

#include "XSTCommonInclude.h"
#include "XSTCommonTypes.h"

namespace XST
{
	using namespace Types;

	class Path
	{
		public:

			static XST_API	bool		IsFileExists(xst_castring& strPath);

			static XST_API	xst_astring	GetFileName(xst_castring& strPath);

			static XST_API	xst_astring	GetParentDirectoryPath(xst_castring& strPath);

			static XST_API	i32			GetParentDirPathAndFileName(xst_castring& strPath, xst_astring* pDirPathOut, xst_astring* pFileNameOut);

			static XST_API	xst_astring	Combine(xst_castring& strLeft, xst_castring& strRight);

		protected:
	};

}//xst

#endif