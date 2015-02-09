#ifndef XSE_CFILE_SYSTEM_H
#define XSE_CFILE_SYSTEM_H

#include "XSEIFileSystem.h"

namespace XSE
{
	class XST_API CFileSystem : public IFileSystem
	{
		public:

								CFileSystem();
			virtual				~CFileSystem();

			virtual i32			Init() xst_implement;

			virtual i32			GetFileInfo(xst_castring& strFilePath, SFileInfo* pOut)  xst_implement;
			virtual i32			GetDirInfo(xst_castring& strDirPath, SDirInfo* pOut) xst_implement;
			virtual i32			GetDirInfos(xst_castring& strDirPath, bool bRecursive, DirInfoVec* pvOut) xst_implement;
			virtual	i32			LoadFile(const SFileInfo& Info, u8** ppData) xst_implement;
			virtual u32			GetFileCount(xst_castring& strDirPath) xst_implement;
			virtual i32			GetFileInfos(xst_castring& strDirPath, bool bRecursive, FileInfoVec* pvOut) xst_implement;
			virtual i32			LoadFiles(const SFileInfo* aInfos, u32 uInfoCount, u8** ppOut) xst_implement;

	};
} // xse

#endif // XSE_CFILE_SYSTEM_H