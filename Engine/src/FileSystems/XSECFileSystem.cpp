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

	xst_fi static void CopyWin32Name(ch8* pDst, u32 uDstSize, lpcastr pSrc, u32 uSrcSize )
	{
		u32 uSize = ( uSrcSize >= XSE_MAX_DIR_PATH_LENGTH - 1 ) ? XSE_MAX_DIR_PATH_LENGTH - 2 : uSrcSize;
		std::copy( pSrc, pSrc + uSize, pDst );
		pDst[ uSize ] = 0;
	}

	i32 GetDirInfo(xst_castring& strDir, IFileSystem::SDirInfo* pOut)
	{
		xst_astring strPath = XST::Path::Combine( strDir, "*" );
		HANDLE hFile = ::FindFirstFileA( strPath.data(), &g_FindFileData );
		auto& FindFile = g_FindFileData;
		pOut->uDirCount = pOut->uFileCount = 0;
		
		if( hFile != INVALID_HANDLE_VALUE )
		{
			// If directory
			if( IsWin32Directory( FindFile ) )
			{
				CopyWin32Name( pOut->strPath, XSE_MAX_DIR_PATH_LENGTH, strDir.c_str(), strDir.length() );
				while( ::FindNextFileA( hFile, &FindFile ) )
				{
					if( IsWin32Directory( FindFile ) )
					{
						pOut->uDirCount++;
					}
					else
					{
						pOut->uFileCount++;
					}
				}	
			}
			else
			{
				CopyWin32Name( pOut->strPath, XSE_MAX_DIR_PATH_LENGTH, strDir.c_str(), strDir.length() );
				while( ::FindNextFileA( hFile, &FindFile ) )
				{
					if( IsWin32Directory( FindFile ) )
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

	i32 GetDirInfoRecursived(xst_castring& strDir, IFileSystem::DirInfoVec* pOut, u32 uCurrDir)
	{
		xst_astring strPath = XST::Path::Combine( strDir, "*" );
		HANDLE hFile = ::FindFirstFileA( strPath.data(), &g_FindFileData );

		if( hFile != INVALID_HANDLE_VALUE )
		{
			IFileSystem::SDirInfo Info = { 0,0,0,0 };
			// If directory
			if( IsWin32Directory( g_FindFileData ) )
			{
				CopyWin32Name( Info.strPath, XSE_MAX_DIR_PATH_LENGTH, strDir.c_str(), strDir.length() );
				while( ::FindNextFileA( hFile, &g_FindFileData ) )
				{
					if( IsWin32Directory( g_FindFileData ) )
					{
						Info.uDirCount++;
						GetDirInfoRecursived( XST::Path::Combine( strDir, g_FindFileData.cFileName ), pOut, ++uCurrDir );
					}
					else
					{
						Info.uFileCount++;
					}
				}	
			}
			else
			{
				while( ::FindNextFileA( hFile, &g_FindFileData ) )
				{
					if( IsWin32Directory( g_FindFileData ) )
					{
						Info.uDirCount++;
						GetDirInfoRecursived( XST::Path::Combine( strDir, g_FindFileData.cFileName ), pOut, ++uCurrDir );
					}
					else
					{
						Info.uFileCount++;
					}
				}	
			}
			pOut->push_back( Info );
			FindClose( hFile );
			return XST_OK;
		}
		return XST_FAIL;
	}

	i32 GetFilePathLength(lpcastr strPath, u32 uPathLen)
	{
		for( i32 i = uPathLen; i-- > 0; )
		{
			if( strPath[i] == '/' || strPath[i] == '\\' )
				return uPathLen - i;
		}
		return -1;
	}

	i32 GetFileExtLength(lpcastr strPath, u32 uPathLen)
	{
		for( i32 i = uPathLen; i-- > 0; )
		{
			if( strPath[i] == '.' )
				return uPathLen - i;
		}
		return -1;
	}

	void GetFileInfosRecursived(xst_castring& strDir, IFileSystem::FileInfoVec* pOut, u32 uCurrFile)
	{
		xst_astring strPath = XST::Path::Combine( strDir, "*" );
		HANDLE hFile = ::FindFirstFileA( strPath.data(), &g_FindFileData );
		auto& FindFileData = g_FindFileData;
		IFileSystem::SFileInfo Info = { 0, 0, 0, 0, 0, 0 };

		if( hFile != INVALID_HANDLE_VALUE )
		{
			if( IsWin32Directory( FindFileData ) )
			{
				GetFileInfosRecursived( XST::Path::Combine( strDir, FindFileData.cFileName ), pOut, ++uCurrFile );
			}
			else
			{
				u32 uLen = strlen( FindFileData.cFileName );
				CopyWin32Name( Info.strPath, XSE_MAX_FILE_PATH_LENGTH, FindFileData.cFileName, uLen );
				Info.uNameLength = uLen;
				Info.uFileSize = FindFileData.nFileSizeHigh;

			}
			
			while( ::FindNextFileA( hFile, &FindFileData ) )
			{
				if( IsWin32Directory( FindFileData ) )
				{
					GetFileInfosRecursived( XST::Path::Combine( strDir, FindFileData.cFileName ), pOut, ++uCurrFile );
				}
				else
				{
					
				}
			}
		}
		pOut->push_back( Info );
		FindClose( hFile );
	}
#else
#error "FileSystem not implemented"
#endif

	CFileSystem::CFileSystem()
	{

	}

	CFileSystem::~CFileSystem()
	{

	}

	i32 CFileSystem::Init()
	{
		return XST_OK;
	}

	i32 CFileSystem::GetFileInfo(xst_castring& strFilePath, SFileInfo* pOut) 
	{
		return 0; //XSE::GetFileInfo( strFilePath, pOut );
	}

	i32 CFileSystem::GetDirInfo(xst_castring& strDirPath, SDirInfo* pOut)
	{
		return XSE::GetDirInfo( strDirPath, pOut );
	}

	i32 CFileSystem::GetDirInfos(xst_castring& strDirPath, bool bRecursive, DirInfoVec* pOut)
	{
		if( bRecursive )
			return XSE::GetDirInfoRecursived( strDirPath, pOut, 0 );
		return XSE::GetDirInfo( strDirPath, pOut );
	}

	i32	CFileSystem::LoadFile(const SFileInfo& Info, u8** ppData)
	{ return 0; }

	u32	CFileSystem::GetFileCount(xst_castring& strDirPath)
	{ return 0; }

	i32	CFileSystem::GetFileInfos(xst_castring& strDirPath, bool bRecursive, SFileInfo* ppOut)
	{ return 0; }

	i32	CFileSystem::LoadFiles(const SFileInfo* aInfos, u32 uInfoCount, u8** ppOut)
	{ return 0; }

} // xse