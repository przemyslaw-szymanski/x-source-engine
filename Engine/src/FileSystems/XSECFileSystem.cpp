#include "XSECFileSystem.h"
#include "XSTPath.h"

namespace XSE
{
#if defined (XST_WINDOWS)
	
	WIN32_FIND_DATAA g_FindFileData;

	xst_fi static bool IsWin32Directory(const WIN32_FIND_DATAA& Data, bool* bIsDot)
	{
		*bIsDot = !( Data.cFileName[0] != '.' && Data.cFileName[1] != '.' );
		return	Data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && (*bIsDot);
	}

	xst_fi static void CopyWin32Name(ch8* pDst, u32 uDstSize, lpcastr pSrc, u32 uSrcSize )
	{
		u32 uSize = ( uSrcSize >= XSE_MAX_DIR_PATH_LENGTH - 1 ) ? XSE_MAX_DIR_PATH_LENGTH - 2 : uSrcSize;
		std::copy( pSrc, pSrc + uSize, pDst );
		pDst[ uSize ] = 0;
	}

	typedef bool(* pfnOnFile)(const WIN32_FIND_DATAA& Data, xst_castring& strPath, bool bIsDir, xst_unknown pUserData);

	i32 ForFeachFile(xst_castring& strPath, LPWIN32_FIND_DATAA pFileData, bool bRecursive, pfnOnFile OnFile, xst_unknown pUserData)
	{
		xst_astring strPath2 = XST::Path::Combine( strPath, "*" );
		HANDLE hFile = ::FindFirstFileA( strPath2.data(), pFileData );
		
		if( hFile != INVALID_HANDLE_VALUE )
		{
			bool bIsDot = pFileData->cFileName[ 0 ] == '.';
			// If directory
			if( !bIsDot )
			{
				if( pFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
					OnFile( *pFileData, strPath, true, pUserData );
					if( bRecursive )
						ForFeachFile( XST::Path::Combine( strPath, "*" ), pFileData, bRecursive, OnFile );
				}
				else
				{
					while( ::FindNextFileA( hFile, pFileData ) )
					{
						bIsDot = pFileData->cFileName[ 0 ] == '.';
						if( !bIsDot )
						{
							if( pFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
							{
								OnFile( *pFileData, strPath, true, pUserData );
								if( bRecursive )
									ForFeachFile( XST::Path::Combine( strPath, "*" ), pFileData, bRecursive, OnFile );
							}
							else
							{
								OnFile( *pFileData, strPath, false, pUserData );
							}
						}
					}
				}
			}
			FindClose( hFile );
			return XST_OK;
		}
		return XST_FAIL;
	}

	i32 GetDirInfo(xst_castring& strDir, IFileSystem::SDirInfo* pOut)
	{
		xst_astring strPath = XST::Path::Combine( strDir, "*" );
		HANDLE hFile = ::FindFirstFileA( strPath.data(), &g_FindFileData );
		auto& FindFile = g_FindFileData;
		pOut->uDirCount = pOut->uFileCount = 0;
		
		if( hFile != INVALID_HANDLE_VALUE )
		{
			bool bIsDot;
			// If directory
			if( IsWin32Directory( FindFile, &bIsDot ) )
			{
				CopyWin32Name( pOut->strPath, XSE_MAX_DIR_PATH_LENGTH, strDir.c_str(), strDir.length() );
				while( ::FindNextFileA( hFile, &FindFile ) )
				{
					if( IsWin32Directory( FindFile, &bIsDot ) )
					{
						pOut->uDirCount++;
					}
					else if( !bIsDot )
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
					if( IsWin32Directory( FindFile, &bIsDot ) )
					{
						pOut->uDirCount++;
					}
					else if( !bIsDot )
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
			bool bIsDot;
			IFileSystem::SDirInfo Info = { 0,0,0,0 };
			// If directory
			if( IsWin32Directory( g_FindFileData, &bIsDot ) )
			{
				CopyWin32Name( Info.strPath, XSE_MAX_DIR_PATH_LENGTH, strDir.c_str(), strDir.length() );
				while( ::FindNextFileA( hFile, &g_FindFileData ) )
				{
					if( IsWin32Directory( g_FindFileData, &bIsDot ) )
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
					if( IsWin32Directory( g_FindFileData, &bIsDot ) )
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

	i32 GetDirInfos(xst_castring& strDir, IFileSystem::DirInfoVec* pOut)
	{
		xst_astring strPath = XST::Path::Combine( strDir, "*" );
		HANDLE hFile = ::FindFirstFileA( strPath.data(), &g_FindFileData );

		if( hFile != INVALID_HANDLE_VALUE )
		{
			bool bIsDot;
			IFileSystem::SDirInfo Info = { 0,0,0,0 };
			// If directory
			if( IsWin32Directory( g_FindFileData, &bIsDot ) )
			{
				CopyWin32Name( Info.strPath, XSE_MAX_DIR_PATH_LENGTH, strDir.c_str(), strDir.length() );
				while( ::FindNextFileA( hFile, &g_FindFileData ) )
				{
					if( IsWin32Directory( g_FindFileData, &bIsDot ) )
					{
						Info.uDirCount++;
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
					if( IsWin32Directory( g_FindFileData, &bIsDot ) )
					{
						Info.uDirCount++;
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
		return 0;
	}

	i32 GetFileExtLength(lpcastr strPath, u32 uPathLen)
	{
		for( i32 i = uPathLen; i-- > 0; )
		{
			if( strPath[i] == '.' )
				return uPathLen - i;
		}
		return 0;
	}

	xst_fi void SetFileInfoFromFileData(IFileSystem::SFileInfo* pInfo, const WIN32_FIND_DATAA& Data)
	{
		u32 uLen = strlen( Data.cFileName );
		CopyWin32Name( pInfo->strPath, XSE_MAX_FILE_PATH_LENGTH, Data.cFileName, uLen );
		pInfo->uPathLength = GetFilePathLength( Data.cFileName, uLen );
		pInfo->uFileSize = Data.nFileSizeHigh;
		pInfo->uExtLength = GetFileExtLength( Data.cFileName, uLen );
		pInfo->uNameLength = uLen - pInfo->uPathLength;
	}

	bool OnFileInfo(const WIN32_FIND_DATAA& Data, xst_castring& strPath, bool bDir)
	{
		
	}

	void GetFileInfosRecursived(xst_castring& strDir, IFileSystem::FileInfoVec* pOut, u32 uCurrFile)
	{
		xst_astring strPath = XST::Path::Combine( strDir, "*" );
		HANDLE hFile = ::FindFirstFileA( strPath.data(), &g_FindFileData );
		auto& FindFileData = g_FindFileData;
		IFileSystem::SFileInfo Info = { 0, 0, 0, 0, 0, 0 };

		if( hFile != INVALID_HANDLE_VALUE )
		{
			bool bIsDot = FindFileData.cFileName[0] == '.';
			if( !bIsDot )
			{
				if( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
					GetFileInfosRecursived( XST::Path::Combine( strDir, FindFileData.cFileName ), pOut, ++uCurrFile );
				}
				else
				{
					SetFileInfoFromFileData( &Info, FindFileData );
					pOut->push_back( Info );
				}
			}
			
			while( ::FindNextFileA( hFile, &FindFileData ) )
			{
				bIsDot = FindFileData.cFileName[0] == '.';
				if( !bIsDot )
				{
					if( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
					{
						GetFileInfosRecursived( XST::Path::Combine( strDir, FindFileData.cFileName ), pOut, ++uCurrFile );
					}
					else
					{
						SetFileInfoFromFileData( &Info, FindFileData );
						pOut->push_back( Info );
					}
				}
			}
		}
		
		FindClose( hFile );
	}

	i32 GetFileInfos(xst_castring& strDir, IFileSystem::FileInfoVec* pOut)
	{
		xst_astring strPath = XST::Path::Combine( strDir, "*" );
		HANDLE hFile = ::FindFirstFileA( strPath.data(), &g_FindFileData );
		auto& FindFileData = g_FindFileData;
		IFileSystem::SFileInfo Info = { 0, 0, 0, 0, 0, 0 };

		if( hFile != INVALID_HANDLE_VALUE )
		{
			bool bIsDot;
			if( IsWin32Directory( FindFileData, &bIsDot ) )
			{
				
			}
			else
			{
				SetFileInfoFromFileData( &Info, FindFileData );
				pOut->push_back( Info );
			}
			
			while( ::FindNextFileA( hFile, &FindFileData ) )
			{
				if( IsWin32Directory( FindFileData, &bIsDot ) )
				{
					
				}
				else
				{
					SetFileInfoFromFileData( &Info, FindFileData );
				}
			}
			FindClose( hFile );
			return XST_OK;
		}
		return XST_FAIL;
	}

	i32 GetFileInfo(xst_castring& strDir, IFileSystem::SFileInfo* pOut)
	{
		xst_astring strPath = XST::Path::Combine( strDir, "*" );
		HANDLE hFile = ::FindFirstFileA( strPath.data(), &g_FindFileData );
		auto& FindFileData = g_FindFileData;

		if( hFile != INVALID_HANDLE_VALUE )
		{
			bool bIsDot;
			if( IsWin32Directory( FindFileData, &bIsDot ) )
			{
				
			}
			else
			{
				SetFileInfoFromFileData( pOut, FindFileData );
			}
			
			/*while( ::FindNextFileA( hFile, &FindFileData ) )
			{
				if( IsWin32Directory( FindFileData ) )
				{
					GetFileInfosRecursived( XST::Path::Combine( strDir, FindFileData.cFileName ), pOut, ++uCurrFile );
				}
				else
				{
					SetFileInfoFromFileData( &Info, FindFileData );

				}
			}*/
			return XST_OK;
			FindClose( hFile );
		}
		return XST_FAIL;
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
		return XSE::GetDirInfos( strDirPath, pOut );
	}

	i32	CFileSystem::LoadFile(const SFileInfo& Info, u8** ppData)
	{ return 0; }

	u32	CFileSystem::GetFileCount(xst_castring& strDirPath)
	{ return 0; }

	i32	CFileSystem::GetFileInfos(xst_castring& strDirPath, bool bRecursive, FileInfoVec* pOut)
	{ 
		if( bRecursive )
			XSE::GetFileInfosRecursived( strDirPath, pOut, 0 );
		else
			XSE::GetFileInfos( strDirPath, pOut );
		return XST_OK;
	}

	i32	CFileSystem::LoadFiles(const SFileInfo* aInfos, u32 uInfoCount, u8** ppOut)
	{ return 0; }

} // xse