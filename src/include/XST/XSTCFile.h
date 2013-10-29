#ifndef XST_CFILE_H
#define XST_CFILE_H

#include "XSTCommon.h"

namespace XST
{

/*
		class CFile//: public TCSingleton<CFileManager>
		{
			public:

				static ::HANDLE		CreateA(lpcastr _lpszFilename, ul32 _ulMode);

				static bool			Write(::HANDLE _hFile, void *_pData, ul32 _ulDataSize);

				static bool			Read(::HANDLE _hFile, void *_pData, ul32 _ulDataSize);

				static bool			ReadAll(::HANDLE _hFile, u8 **_ppOutData, ul32 *_pulOutDataSize);

				static bool			Close(::HANDLE _hFile);

				static ::HANDLE		OpenA(lpcastr _lpszFilename, ul32 _ulMode);

				static ul32			GetSize(::HANDLE _hFile);

			xst_fi	static bool		IsBadHandle(::HANDLE _hFile)
									{ return _hFile == INVALID_HANDLE_VALUE; }

			private:

				static ul32			m_ulBytesRead;
		};
*/
}//XST

#endif //XST_CFILE_H
