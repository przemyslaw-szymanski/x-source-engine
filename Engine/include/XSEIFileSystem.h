#ifndef XSE_IFILE_SYSTEM_H
#define XSE_IFILE_SYSTEM_H

#include "XSECommon.h"

namespace XSE
{
	using namespace XST::Types;

	class XST_API IFileSystem
	{
		public:

			struct SDirInfo
			{
				ch8		strPath[ XSE_MAX_DIR_PATH_LENGTH ];
				u32		uFileCount;
				u32		uDirCount;
				u16		uParentDirId; u8 m_padding[ 2 ];
			};

			struct SFileInfo
			{
				ch8			strPath[ XSE_MAX_FILE_PATH_LENGTH ];
				ul32		uFileSize;
				u16			uPathLength : 16;
				u16			uNameLength : 8;
				u16			uExtLength	: 8;
				u16			uParentDirId; u8 m_padding[ 2 ];
			};

			typedef xst_vector< SDirInfo >	DirInfoVec;
			typedef xst_vector< SFileInfo > FileInfoVec;

			struct SFileArray
			{
				u8*		pData;
			};

		protected:

		public:
								IFileSystem() {}
			virtual				~IFileSystem() {}

			virtual i32			Init() { return XST_OK; }

			virtual i32			GetFileInfo(xst_castring& strFilePath, SFileInfo* pOut) { return XST_FAIL; }
			virtual i32			GetDirInfo(xst_castring& strDirPath, SDirInfo* pOut) { return XST_FAIL; }
			virtual i32			GetDirInfos(xst_castring& strDirPath, bool bRecursive, DirInfoVec* pvOut) { return XST_FAIL; }
			virtual	i32			LoadFile(lpcastr pFullPath, u32 uPathLength, u32 uFileSize, u8** ppData) { return XST_FAIL; }
			virtual u32			GetFileCount(xst_castring& strDirPath) { return 0; }
			virtual i32			GetFileInfos(xst_castring& strDirPath, bool bRecursive, FileInfoVec* pvOut) { return XST_FAIL; }
			virtual i32			LoadFiles(const SFileInfo* aInfos, u32 uInfoCount, u8** ppOut) { return XST_FAIL; }
			virtual i32			LoadFiles(const SFileInfo* aInfos, u32 uInfoCount, SFileArray* pOut) { return XST_FAIL; }
			virtual ul32		GetFileSize(lpcastr pFullPath, u32 uPathLength) { return 0; }

		protected:
	};
} // XSE

#endif // XSE_IFILE_SYSTEM_H