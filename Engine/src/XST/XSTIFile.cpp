#include "XSTIFile.h"
#include "XSTCLogger.h"

namespace XST
{

	bool IFile::IsFileExists(xst_castring& _strFilename)
	{
		#if defined (XST_WINDOWS)
			return (::GetFileAttributesA(_strFilename.c_str()) != 0xFFFFFFFF ) && !IsDirectoryExists( _strFilename.data() );
		#else
			return false;
		#endif
	}

	bool IFile::IsFileExists(xst_cwstring& _strFilename)
	{
		#if defined (XST_WINDOWS)
			return ( ::GetFileAttributesW(_strFilename.c_str()) != 0xFFFFFFFF )  && !IsDirectoryExists( _strFilename.data() );
		#else
			return false;
		#endif
	}

	bool	IFile::IsDirectoryExists(xst_castring& strDirPath)
	{
		#if defined (XST_WINDOWS)
			DWORD dwAttrs = ::GetFileAttributesA( strDirPath.c_str() );
			if( dwAttrs == (DWORD)INVALID_HANDLE_VALUE ) return false;
			return dwAttrs & FILE_ATTRIBUTE_DIRECTORY;
		#else
			return false;
		#endif
	}

	bool	IFile::IsDirectoryExists(xst_cwstring& strDirPath)
	{
		#if defined (XST_WINDOWS)
			DWORD dwAttrs = ::GetFileAttributesW( strDirPath.c_str() );
			if( dwAttrs == (DWORD)INVALID_HANDLE_VALUE ) return false;
			return dwAttrs & FILE_ATTRIBUTE_DIRECTORY;
		#else
			return false;
		#endif
	}

	xst_unknown	IFile::Create(xst_castring& _strFilename, MODE _eMode, bool bLockFile)
	{
		#if defined(XST_WINDOWS)

		HANDLE hFile = xst_null;

		if( _eMode == DIRECTORY )
		{
			if( ::CreateDirectoryA( _strFilename.data(), 0 ) == TRUE )
			{
				hFile = (HANDLE)1;
			}
		}
		else
		{
			DWORD dwShareMode = ( !bLockFile )? FILE_SHARE_WRITE : 0;
			hFile = ::CreateFileA(	_strFilename.data(),
									_eMode, dwShareMode, 0,
									CREATE_ALWAYS,
									FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 0);
		}

		if( hFile == INVALID_HANDLE_VALUE )
		{
			hFile = xst_null;
		}

		return hFile;

		#else
			return 0;
		#endif
	}

	xst_unknown	IFile::Create(lpcastr _lpszFilename, MODE _eMode, bool bLockFile)
	{
		#if defined(XST_WINDOWS)
		
			HANDLE hFile = xst_null;

			if( _eMode == DIRECTORY )
			{
				if( ::CreateDirectoryA( _lpszFilename, 0 ) == TRUE )
				{
					hFile = (HANDLE)1;
				}
			}
			else
			{
				DWORD dwShareMode = ( !bLockFile )? FILE_SHARE_WRITE : 0;
				hFile = ::CreateFileA(	_lpszFilename,
										_eMode, dwShareMode, 0,
										CREATE_ALWAYS,
										FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 0);
			}

			if( hFile == INVALID_HANDLE_VALUE )
			{
				hFile = xst_null;
			}

			return hFile;

		#else
			return 0;
		#endif
	}

	xst_unknown	IFile::Create(xst_cwstring& _strFilename, MODE _eMode, bool bLockFile)
	{
		#if defined(XST_WINDOWS)

			HANDLE hFile = xst_null;

			if( _eMode == DIRECTORY )
			{
				if( ::CreateDirectoryW( _strFilename.data(), 0 ) == TRUE )
				{
					hFile = (HANDLE)1;
				}
			}
			else
			{
				DWORD dwShareMode = ( !bLockFile )? FILE_SHARE_WRITE : 0;
				hFile = ::CreateFileW(	_strFilename.data(),
										_eMode, dwShareMode, 0,
										CREATE_ALWAYS,
										FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 0);
			}

			if( hFile == INVALID_HANDLE_VALUE )
			{
				hFile = xst_null;
			}

			return hFile;

		#else
			return 0;
		#endif
	}

	xst_unknown	IFile::Create(lpcwstr _lpszFilename, MODE _eMode, bool bLockFile)
	{
		#if defined(XST_WINDOWS)

			HANDLE hFile = xst_null;

			if( _eMode == DIRECTORY )
			{
				if( ::CreateDirectoryW( _lpszFilename, 0 ) == TRUE )
				{
					hFile = (HANDLE)1;
				}
			}
			else
			{
				DWORD dwShareMode = ( !bLockFile )? FILE_SHARE_WRITE : 0;
				hFile = ::CreateFileW(	_lpszFilename,
										_eMode, dwShareMode, 0,
										CREATE_ALWAYS,
										FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 0);
			}

			if( hFile == INVALID_HANDLE_VALUE )
			{
				hFile = xst_null;
			}

			return hFile;

		#else
			return 0;
		#endif
	}

	bool IFile::Delete(xst_castring& _strFileName, MODE _eMode)
	{
		#if defined(XST_WINDOWS)
			if( _eMode == DIRECTORY )
			{
				return ::RemoveDirectoryA( _strFileName.data() );
			}
			else
			{
				return ::DeleteFileA( _strFileName.data() );
			}
		#else
			return false;
		#endif
	}

	bool IFile::Delete(lpcastr _strFileName, MODE _eMode)
	{
		#if defined(XST_WINDOWS)
			if( _eMode == DIRECTORY )
			{
				return ::RemoveDirectoryA( _strFileName );
			}
			else
			{
				return ::DeleteFileA( _strFileName );
			}
		#else
			return false;
		#endif
	}

	bool IFile::Delete(xst_cwstring& _strFileName, MODE _eMode)
	{
		#if defined(XST_WINDOWS)
			if( _eMode == DIRECTORY )
			{
				return ::RemoveDirectoryW( _strFileName.data() );
			}
			else
			{
				return ::DeleteFileW( _strFileName.data() );
			}
		#else
			return false;
		#endif
	}

	bool IFile::Delete(lpcwstr _strFileName, MODE _eMode)
	{
		#if defined(XST_WINDOWS)
			if( _eMode == DIRECTORY )
			{
				return ::RemoveDirectoryW( _strFileName );
			}
			else
			{
				return ::DeleteFileW( _strFileName );
			}
		#else
			return false;
		#endif
	}

	bool	IFile::Write(xst_unknown _hFile, void *pData, ul32 ulDataSize)
	{
		xst_assert(_hFile != 0, "File not opened");
		#if defined(XST_WINDOWS)
			DWORD ulBytesRead;
			::WriteFile(_hFile, pData, ulDataSize, &ulBytesRead, 0);
			return ulDataSize == ulBytesRead;
		#else
			return 0;
		#endif
	}

	bool	IFile::Read(xst_unknown hFile, u8** ppData, ul32 ulDataSize)
	{
		#if defined(XST_WINDOWS)
			DWORD ulBytesRead;
			if( ::ReadFile(hFile, (*ppData), ulDataSize, &ulBytesRead, 0) != TRUE )
				return false;

			return ulDataSize == ulBytesRead;
		#else
			return 0;
		#endif
	}

	bool	IFile::ReadAll(xst_unknown _hFile, u8 **_ppOutData, ul32 *_pulOutDataSize)
	{
		cl32 lSize = GetSize( _hFile );
		if( lSize < 0 )
		{
			return false;
		}

		u8* pBuffer = xst_new u8[ lSize + 1 ];

		if( !Read( _hFile, &pBuffer, lSize ) )
		{
			xst_deletea( pBuffer );
			pBuffer = 0;
			return false;
		}

		pBuffer[ lSize ] = 0;
		(*_ppOutData) = pBuffer;
		(*_pulOutDataSize) = lSize;

		return true;
	}

	bool	IFile::ReadAll(xst_unknown hFile, u8** ppOutData, cul32& pulOutDataSize)
	{
		if(!Read( hFile, ppOutData, pulOutDataSize))
		{
			return false;
		}

		return true;
	}

	bool	IFile::Close(xst_unknown _hFile)
	{
		#if defined(XST_WINDOWS)
			if( _hFile != INVALID_HANDLE_VALUE )
				return (bool)::CloseHandle(_hFile);
			_hFile = xst_null;
			return false;
		#else
			return false;
		#endif
	}

	IFile::FileHandle	IFile::OpenOrCreate(lpcastr _lpszFileName, MODE _eMode, bool bLockFile)
	{
		return Open( _lpszFileName, _eMode, bLockFile, true );
	}

	IFile::FileHandle	IFile::OpenOrCreate(lpcwstr _lpszFileName, MODE _eMode, bool bLockFile)
	{
		return Open( _lpszFileName, _eMode, bLockFile, true );
	}

	IFile::FileHandle	IFile::OpenOrCreate(xst_castring& _strFilename, MODE _eMode, bool bLockFile)
	{
		return Open( _strFilename.data(), _eMode, bLockFile, true );
	}

	IFile::FileHandle	IFile::OpenOrCreate(xst_cwstring& _strFilename, MODE _eMode, bool bLockFile)
	{
		return Open( _strFilename.data(), _eMode, bLockFile, true );
	}

	xst_unknown IFile::Open(lpcastr _lpszFilename, MODE _eMode, bool bLockFile)
	{
		return Open( _lpszFilename, _eMode, bLockFile, false );
	}

	xst_unknown IFile::Open(lpcwstr _lpszFilename, MODE _eMode, bool bLockFile)
	{
		return Open( _lpszFilename, _eMode, bLockFile, false );
	}

	xst_unknown IFile::Open(xst_castring& _strFilename, MODE _eMode, bool bLockFile)
	{
		return Open( _strFilename.data(), _eMode, bLockFile, false );
	}

	xst_unknown IFile::Open(xst_cwstring& _strFilename, MODE _eMode, bool bLockFile)
	{
		return Open( _strFilename.data(), _eMode, bLockFile, false );
	}

	IFile::FileHandle IFile::Open(XST::Types::lpcastr _lpszFileName, const XST::IFile::MODE &eMode, bool bLockFile, bool bCreateIfNotExists)
	{
		#if defined(XST_WINDOWS)
			DWORD dwShareMode = 0;
			if( ( eMode == READ || eMode == READ_BINARY ) && !bLockFile )
			{
				dwShareMode = FILE_SHARE_READ;
			}
			else if( ( eMode == WRITE || eMode == WRITE_BINARY ) && !bLockFile )
			{
				dwShareMode = FILE_SHARE_WRITE;
			}
			DWORD dwDesc = ( bCreateIfNotExists )? OPEN_ALWAYS : OPEN_EXISTING;
			::HANDLE hFile = ::CreateFileA( _lpszFileName, eMode, dwShareMode, 0, dwDesc, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 0);
			if(hFile == INVALID_HANDLE_VALUE)
			{
				hFile = xst_null;
			}	
			//hFile = Create(_strFilename.data(), _eMode);

			return hFile;
		#else
			return 0;
		#endif
	}

	IFile::FileHandle IFile::Open(XST::Types::lpcwstr _lpszFileName, const XST::IFile::MODE &eMode, bool bLockFile, bool bCreateIfNotExists)
	{
		#if defined(XST_WINDOWS)
			DWORD dwShareMode = 0;
			if( ( eMode == READ || eMode == READ_BINARY ) && !bLockFile )
			{
				dwShareMode = FILE_SHARE_READ;
			}
			else if( ( eMode == WRITE || eMode == WRITE_BINARY ) && !bLockFile )
			{
				dwShareMode = FILE_SHARE_WRITE;
			}
			DWORD dwDesc = ( bCreateIfNotExists )? OPEN_ALWAYS : OPEN_EXISTING;
			::HANDLE hFile = ::CreateFileW( _lpszFileName, eMode, dwShareMode, 0, dwDesc, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 0);
			if(hFile == INVALID_HANDLE_VALUE)
			{
				hFile = xst_null;
			}	
			//hFile = Create(_strFilename.data(), _eMode);

			return hFile;
		#else
			return 0;
		#endif
	}

	l32	IFile::GetSize(xst_unknown _hFile)
	{
		#if defined(XST_WINDOWS)
			return ::GetFileSize(_hFile, 0);
		#else
			return 0;
		#endif
	}

	l32	IFile::GetSize(lpcastr _lpszFileName)
	{
		#if defined(XST_WINDOWS)

			xst_unknown hFile = Open(_lpszFileName, READ);
			if(IsBadHandle(hFile))
			{
				Close(hFile);
				return -1;
			}

			l32 lSize = GetSize(hFile);
			Close(hFile);
			return lSize;

		#else
			return 0;
		#endif
	}

	xst_fi	bool	IFile::IsBadHandle(xst_unknown _hFile)
	{
		#if defined(XST_WINDOWS)
			return ((HANDLE)_hFile) == INVALID_HANDLE_VALUE;
		#else
			return ((FILE*)_hFile) == 0;
		#endif
	}

	bool IFile::Flush(xst_unknown _hFile)
	{
		#if defined(XST_WINDOWS)
			return ::FlushFileBuffers( (HANDLE)_hFile );
		#else
			flush( (FILE*)_hFile );
		#endif
	}

	i32 IFile::GetFileExtension(xst_castring& _strName, ch8* _pExtOut, ul32* _pulExtSizeInOut)
	{
		xst_astring::size_type uiPos = _strName.find_last_of( "." );
		if( uiPos == xst_astring::npos )
		{
			return RESULT::FAILED;
		}

		xst_memcpy( _pExtOut, *_pulExtSizeInOut, _strName.substr( uiPos + 1 ).data(), _strName.length() - uiPos );
		*_pulExtSizeInOut = _strName.length() - uiPos;

		return RESULT::OK;
	}

	xst_astring IFile::GetFileExtension(XST::Types::xst_castring &strName)
	{
		xst_astring::size_type uiPos = strName.find_last_of( "." );
		if( uiPos == xst_astring::npos )
		{
			return "";
		}

		return strName.substr( uiPos + 1 );
	}

	i32 IFile::SetPosition(xst_unknown _hFile, cul32& ulPosition, const SEEK_POSITION& eSeekPos)
	{
		#if defined(XST_WINDOWS)

			if( SetFilePointer( _hFile, ulPosition, 0, eSeekPos ) == INVALID_SET_FILE_POINTER )
			{
				XST::CLastError::Set("Invalid set file pointer");
				return RESULT::FAILED;
			}
		
			return RESULT::OK;
		#else
			return RESULT::FAILED;
		#endif
	}

	i32 IFile::GetPosition(xst_unknown hFile, l32* plPos)
	{
		#if defined (XST_WINDOWS)
			if( SetFilePointer( hFile, 0, plPos, IFile::CURRENT ) == INVALID_SET_FILE_POINTER )
			{
				XST_LOG_ERR( "Invalid set file pointer" );
				return RESULT::FAILED;
			}
		#else
			return RESULT::FAILED;
		#endif

			return RESULT::FAILED;
	}

}//XST
