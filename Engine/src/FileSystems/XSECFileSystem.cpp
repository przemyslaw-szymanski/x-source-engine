#include "XSECFileSystem.h"
#include "XSTPath.h"
#include "XSTCBinaryReader.h"

namespace XSE
{
#if defined (XST_WINDOWS)

	xst_fi static bool IsWin32Directory(const WIN32_FIND_DATAA& Data, bool* bIsDot)
	{
		*bIsDot = !( Data.cFileName[0] != '.' && Data.cFileName[1] != '.' );
		return	Data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && (*bIsDot);
	}

	xst_fi static void CopyWin32Name(ch8* pDst, u32 uDstSize, lpcastr pSrc, u32 uSrcSize )
	{
		u32 uSize = ( uSrcSize >= XSE_MAX_DIR_PATH_LENGTH - 1 ) ? XSE_MAX_DIR_PATH_LENGTH - 2 : uSrcSize;
		std::copy( pSrc, pSrc + uSize, pDst );
		pDst[ uSize ] = '\0';
	}

	typedef bool(* pfnOnFile)(const WIN32_FIND_DATAA& Data, xst_castring& strPath, bool bIsDir, xst_unknown pUserData);

	i32 ForFeachFile(xst_castring& strPath, LPWIN32_FIND_DATAA pFileData, bool bRecursive, pfnOnFile OnFile, xst_unknown pUserData)
	{
		xst_astring strPath2 = XST::Path::Combine( strPath, "*" );
		HANDLE hFile = ::FindFirstFileA( strPath2.data(), pFileData );
		xst_castring strDir = strPath + "/";
		
		if( hFile != INVALID_HANDLE_VALUE )
		{
			bool bIsDot = pFileData->cFileName[ 0 ] == '.';
			// If directory
			if( !bIsDot )
			{
				if( pFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
					OnFile( *pFileData, strDir, true, pUserData );
					if( bRecursive )
						ForFeachFile( XST::Path::Combine( strPath, pFileData->cFileName ), pFileData, bRecursive, OnFile, pUserData );
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
								OnFile( *pFileData, strDir, true, pUserData );
								if( bRecursive )
									ForFeachFile( XST::Path::Combine( strPath, pFileData->cFileName ), pFileData, bRecursive, OnFile, pUserData );
							}
							else
							{
								OnFile( *pFileData, strDir, false, pUserData );
							}
						}
					}
				}
			}

			while( ::FindNextFileA( hFile, pFileData ) )
			{
				bIsDot = pFileData->cFileName[ 0 ] == '.';
				if( !bIsDot )
				{
					if( pFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
					{
						OnFile( *pFileData, strDir, true, pUserData );
						if( bRecursive )
							ForFeachFile( XST::Path::Combine( strPath, pFileData->cFileName ), pFileData, bRecursive, OnFile, pUserData );
					}
					else
					{
						OnFile( *pFileData, strDir, false, pUserData );
					}
				}
			}

			FindClose( hFile );
			return XST_OK;
		}
		return XST_FAIL;
	}

	bool OnDir( const WIN32_FIND_DATAA& Data, xst_castring& strPath, bool bIsDir, xst_unknown pUserData )
	{
		return true;
	}

	i32 GetDirInfo(xst_castring& strDir, IFileSystem::SDirInfo* pOut)
	{
		WIN32_FIND_DATAA Data;
		return ForFeachFile( strDir, &Data, false, OnDir, xst_null );
	}

	i32 GetDirInfoRecursived(xst_castring& strDir, IFileSystem::DirInfoVec* pOut, u32 uCurrDir)
	{
		WIN32_FIND_DATAA Data;
		return ForFeachFile( strDir, &Data, true, OnDir, pOut );
	}

	i32 GetDirInfos(xst_castring& strDir, IFileSystem::DirInfoVec* pOut)
	{
		WIN32_FIND_DATAA Data;
		return ForFeachFile( strDir, &Data, false, OnDir, pOut );
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

	bool OnFileInfo(const WIN32_FIND_DATAA& Data, xst_castring& strPath, bool bDir, xst_unknown pUserData)
	{
		if( bDir )
			return true;
		IFileSystem::SFileInfo Info;
		XSE::SetFileInfoFromFileData( &Info, Data );
		//xst_memcpy( Info.strPath, XSE_MAX_FILE_PATH_LENGTH, strPath.data(), strPath.length() );
		//xst_memcp
		CopyWin32Name( Info.strPath, XSE_MAX_DIR_PATH_LENGTH, strPath.c_str(), strPath.length() );
		u32 uLen = strlen( Data.cFileName );
		CopyWin32Name( Info.strPath + strPath.length(), XSE_MAX_DIR_PATH_LENGTH, Data.cFileName, uLen );
		Info.uPathLength = strPath.length() + uLen;
		Info.strPath[ Info.uPathLength ] = '\0';
		for(u32 i = 0; i < Info.uPathLength; ++i )
			if( Info.strPath[i] == '\\' ) Info.strPath[i] = '/';
		auto* pVec = (IFileSystem::FileInfoVec*)pUserData;
		pVec->push_back( Info );
		return true;
	}

	void GetFileInfosRecursived(xst_castring& strDir, IFileSystem::FileInfoVec* pOut, u32 uCurrFile)
	{
		WIN32_FIND_DATAA Data;
		ForFeachFile( strDir, &Data, true, &OnFileInfo, pOut );
	}

	i32 GetFileInfos(xst_castring& strDir, IFileSystem::FileInfoVec* pOut)
	{
		WIN32_FIND_DATAA Data;
		return ForFeachFile( strDir, &Data, false, &OnFileInfo, pOut );
	}

	i32 GetFileInfo(xst_castring& strDir, IFileSystem::SFileInfo* pOut)
	{
		WIN32_FIND_DATAA Data;
		return ForFeachFile( strDir, &Data, false, &OnFileInfo, pOut );
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

	i32	CFileSystem::LoadFile(lpcastr pFullPath, u32 uPathLength, u32 uFileSize, u8** ppData)
	{ 
		XST::IFile::FileHandle hFile = XST::IFile::Open( pFullPath, XST::IFile::READ_BINARY );
		if( hFile )
		{
			if( XST::IFile::Read( hFile, ppData, uFileSize ) )
			{
				if( XST::IFile::Close( hFile ) )
				{
					return XST_OK;
				}
				else
				{
					XST_LOG_ERR( "Unable to close file: " << pFullPath );
					return XST_FAIL;
				}
			}
			else
			{
				XST_LOG_ERR( "Unable to read file: " << pFullPath );
				return XST_FAIL;
			}
		}
		else
		{
			XST_LOG_ERR( "Unable to open file: " << pFullPath );
			return XST_FAIL;
		}
		
		return XST_OK; 
	}

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
	{ 
		xst_assert2( ppOut );
		i32 iResult = 0;
		for( u32 i = 0; i < uInfoCount; ++i )
		{
			//iResult = LoadFile( aInfos[ i ], &ppOut[ i ] );
		}
		return iResult; 
	}

	i32 CFileSystem::LoadFiles(const SFileInfo* aInfos, u32 uInfoCount, SFileArray* pOut)
	{
		xst_assert2( pOut && pOut->pData );
		u64 uOffset = 0;
		for( u32 i = 0; i < uInfoCount; ++i )
		{
			auto& Info = aInfos[ i ];
			pOut->pData += uOffset;
			//if( XST_FAILED( LoadFile( Info, &pOut->pData ) ) )
				return XST_FAIL;
			uOffset += Info.uFileSize;
		}
		return XST_OK;
	}

} // xse