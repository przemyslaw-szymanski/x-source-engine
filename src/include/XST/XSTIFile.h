#ifndef XST_IFILE_H
#define XST_IFILE_H

#include "XSTCommon.h"
#include "XSTCString.h"

#pragma warning(disable : 4800)

namespace XST
{

	class XST_API IFile
	{
		public:

            typedef xst_unknown FileHandle;

		#if defined(XST_WINDOWS)
			enum MODE
			{
				READ = GENERIC_READ,
				WRITE = GENERIC_WRITE,
				READ_BINARY = GENERIC_READ,
				WRITE_BINARY = GENERIC_WRITE,
				READ_WRITE = GENERIC_READ | GENERIC_WRITE,
				APPEND = FILE_APPEND_DATA,
				ALL = FILE_ALL_ACCESS,
				APPEND_BINARY = WRITE_BINARY | APPEND,
				DIRECTORY,
				FILE
			};

			enum SEEK_POSITION
			{
				BEGIN,
				CURRENT,
				END
			};
		#else
			enum MODE
			{
			};
		#endif

		public:

			static  bool IsFileExists(xst_castring& _strFilename);

			static  bool IsFileExists(xst_cwstring& _strFilename);

			static  bool	IsDirectoryExists(xst_castring& strDirPath);

			static  bool	IsDirectoryExists(xst_cwstring& strDirPath);

			static  FileHandle	Create(xst_castring& _strFilename, MODE _eMode, bool bLockFile = true);

			static  FileHandle	Create(lpcastr _lpszFilename, MODE _eMode, bool bLockFile = true);

			static  FileHandle	Create(xst_cwstring& _strFilename, MODE _eMode, bool bLockFile = true);

			static  FileHandle	Create(lpcwstr _lpszFilename, MODE _eMode, bool bLockFile = true);

			static  FileHandle	OpenOrCreate(lpcastr _lpszFileName, MODE _eMode, bool bLockFile = false);

			static  FileHandle	OpenOrCreate(lpcwstr _lpszFileName, MODE _eMode, bool bLockFile = false);

			static  FileHandle	OpenOrCreate(xst_castring& _strFilename, MODE _eMode, bool bLockFile = false);

			static  FileHandle	OpenOrCreate(xst_cwstring& _strFilename, MODE _eMode, bool bLockFile = false);

			static  bool Delete(xst_castring& _strFileName, MODE _eMode);

			static  bool Delete(lpcastr _strFileName, MODE _eMode);

			static  bool Delete(xst_cwstring& _strFileName, MODE _eMode);

			static  bool Delete(lpcwstr _strFileName, MODE _eMode);

			static  bool	Write(FileHandle _hFile, void *_pData, ul32 _ulDataSize);

			static  bool	Read(FileHandle _hFile, void *_pData, ul32 _ulDataSize);

			static  bool	ReadAll(FileHandle _hFile, u8 **_ppOutData, ul32 *_pulOutDataSize);

			static  bool	ReadAll(FileHandle _hFile, u8 *_ppOutData, cul32& _pulOutDataSize);

			static  bool	Close(FileHandle _hFile);

			static  FileHandle Open(lpcastr _lpszFilename, MODE _eMode, bool bLockFile = false);

			static  FileHandle Open(lpcwstr _lpszFilename, MODE _eMode, bool bLockFile = false);

			static  FileHandle Open(xst_castring& _strFilename, MODE _eMode, bool bLockFile = false);

			static  FileHandle Open(xst_cwstring& _strFilename, MODE _eMode, bool bLockFile = false);

			static  FileHandle	Open(lpcastr _lpszFileName, const MODE& eMode, bool bLockFile, bool bCreateIfNotExists);

			static  FileHandle	Open(lpcwstr _lpszFileName, const MODE& eMode, bool bLockFile, bool bCreateIfNotExists);

			static  l32	GetSize(FileHandle _hFile);

			static  l32	GetSize(lpcastr _lpszFileName);

			xst_fi	static  bool	IsBadHandle(FileHandle _hFile);

			static  bool Flush(FileHandle _hFile);

			static  i32 GetFileExtension(xst_castring& _strName, ch8* _pExtOut, ul32* _pulExtSizeInOut);

			static  xst_astring GetFileExtension(xst_castring& strName);

			static  i32 SetPosition(xst_unknown _hFile, cul32& ulPosition, const SEEK_POSITION& eSeekPos);

			static  i32 GetPosition(xst_unknown hFile, l32* pulPos);

			template<class _XST_STRING_>
			static  i32 GetFileExtension(const _XST_STRING_& _strName, ch8* _pExtOut, ul32* _pulExtSizeInOut)
			{
				i32 uiPos = _strName.find_last_of( '.' );
				if( uiPos == -1 )
				{
					return RESULT::FAILED;
				}


				xst_memcpy( _pExtOut, *_pulExtSizeInOut, _strName.substr( uiPos + 1 ).data(), _strName.length() - uiPos );
				*_pulExtSizeInOut = _strName.length() - uiPos;
			}
/*
			template<class _XST_SRC_STRING_, class _XST_DST_STRING_>
			static i32 GetFileExtension(const _XST_SRC_STRING_& _strName, _XST_DST_STRING_* _pStrOut)
			{
				i32 uiPos = _strName.find_last_of( '.' );
				if( uiPos == -1 )
				{
					return RESULT::FAILED;
				}

				_XST_DST_STRING_::CHAR* pTmp = _pStrOut->data();
				xst_memcpy( pTmp, _pStrOut->capacity(), _strName.substr( uiPos + 1 ).data(), _pStrOut->capacity() );
				*_pStrOut = pTmp;

				return RESULT::OK;
			}
*/

		protected:



	};

}//XST

#endif
