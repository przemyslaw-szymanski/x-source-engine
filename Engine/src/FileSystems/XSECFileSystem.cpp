#include "XSECFileSystem.h"
#include "XSTPath.h"

namespace XSE
{
#if defined (XST_WINDOWS)
	
	WIN32_FIND_DATAA g_FindFileData;

	xst_fi static bool IsWin32Directory(const WIN32_FIND_DATAA& Data)
	{
		return	Data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && 
				( Data.cFileName[0] != '.' && Data.cFileName[1] != '.' );
	}

	i32 GetDirInfo(xst_castring& strDir, IFileSystem::SDirInfo* pOut)
	{
		xst_astring strPath = XST::Path::Combine( strDir, "*" );
		HANDLE hFile = ::FindFirstFileA( strPath.data(), &g_FindFileData );
		
		if( hFile != INVALID_HANDLE_VALUE )
		{
			// If directory
			if( IsWin32Directory( g_FindFileData ) )
			{
				u32 uSize = ( strDir.length() >= XSE_MAX_DIR_PATH_LENGTH ) ? XSE_MAX_DIR_PATH_LENGTH - 1 : strDir.length();
				std::copy( strDir.begin(), strDir.begin() + uSize, pOut->strPath );
				while( ::FindNextFileA( hFile, &g_FindFileData ) )
				{
					if( IsWin32Directory( g_FindFileData ) )
					{
						pOut->uDirCount++;
					}
					else
					{
						pOut->uFileCount++;
					}
				}	
			}
			FindClose( hFile );
			return XST_OK;
		}
		return XST_FAIL;
	}

	i32 GetDirInfoRecursived(xst_castring& strDir, IFileSystem::SDirInfo** ppOut, u32 uCurrDir)
	{
		xst_astring strPath = XST::Path::Combine( strDir, "*" );
		HANDLE hFile = ::FindFirstFileA( strPath.data(), &g_FindFileData );
		
		auto* pCurrDir = ppOut[ uCurrDir ];

		if( hFile != INVALID_HANDLE_VALUE )
		{
			// If directory
			if( IsWin32Directory( g_FindFileData ) )
			{
				u32 uSize = ( strDir.length() >= XSE_MAX_DIR_PATH_LENGTH ) ? XSE_MAX_DIR_PATH_LENGTH - 1 : strDir.length();
				std::copy( strDir.begin(), strDir.begin() + uSize, pCurrDir->strPath );
				while( ::FindNextFileA( hFile, &g_FindFileData ) )
				{
					if( IsWin32Directory( g_FindFileData ) )
					{
						pCurrDir->uDirCount++;
						GetDirInfoRecursived( XST::Path::Combine( strDir, g_FindFileData.cFileName ), ppOut, ++uCurrDir );
					}
					else
					{
						pCurrDir->uFileCount++;
					}
				}	
			}
			FindClose( hFile );
			return XST_OK;
		}
		return XST_FAIL;
	}

	void GetFileInfosRecursived(xst_castring& strDir, IFileSystem::SFileInfo** ppOut, u32 uCurrFile)
	{
		xst_astring strPath = XST::Path::Combine( strDir, "*" );
		HANDLE hFile = ::FindFirstFileA( strPath.data(), &g_FindFileData );

		if( hFile != INVALID_HANDLE_VALUE )
		{
			if( IsWin32Directory( g_FindFileData ) )
			{
				ppOut[ uCurrFile ]->uParentDirHandle = (u64)hFile;
				GetFileInfosRecursived( XST::Path::Combine( strDir, g_FindFileData.cFileName ), ppOut, ++uCurrFile );
			}
			else
			{
				auto* pCurrFile = ppOut[ ++uCurrFile ];
				pCurrFile->uParentDirHandle = 0;
			}
			
			while( ::FindNextFileA( hFile, &g_FindFileData ) )
			{
				if( IsWin32Directory( g_FindFileData ) )
				{
					GetFileInfosRecursived( XST::Path::Combine( strDir, g_FindFileData.cFileName ), ppOut, ++uCurrFile );
				}
				else
				{
					auto* pCurrFile = ppOut[ ++uCurrFile ];
					pCurrFile->uParentDirHandle = 0;
				}
			}
		}

		FindClose( hFile );
	}
#else
#error "FileSystem not implemented"
#endif

	i32 CFileSystem::Init(){}

	i32 CFileSystem::GetFileInfo(xst_castring& strFilePath) 
	{}

	i32 CFileSystem::GetDirInfo(xst_castring& strDirPath, SDirInfo* pOut)
	{}

	i32 CFileSystem::GetDirInfos(xst_castring& strDirPath, bool bRecursive, SDirInfo** ppOut)
	{}

	i32	CFileSystem::LoadFile(const SFileInfo& Info, u8** ppData)
	{}

	u32	CFileSystem::GetFileCount(xst_castring& strDirPath)
	{}

	i32	CFileSystem::GetFileInfos(xst_castring& strDirPath, bool bRecursive, SFileInfo** ppOut)
	{}

	i32	CFileSystem::LoadFiles(const SFileInfo* aInfos, u32 uInfoCount, u8** ppOut)
	{}

} // xse