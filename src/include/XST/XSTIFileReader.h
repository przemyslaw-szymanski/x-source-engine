#ifndef XST_FILE_READER_H
#define XST_FILE_READER_H

#include "XSTCommon.h"
#pragma warning(disable: 4482) //Warning	3	warning C4482: nonstandard extension used: enum 'XST::_FILEENCODING' used in qualified name	

#include "XSTIFile.h"

namespace XST
{

	typedef struct _SFILE
	{
		union
		{
			ch8*	pDataA;
			ch16*	pDataW;
		}Data;

		bool bAscii;
		ul32 ulSize;

	}SFile;

	

	class ITextFile : public virtual IFile
	{
		public:

			typedef enum _FILEENCODING
			{
				UNKNOWN = 0,
				ANSI,
				UTF8,
				UTF16LE,
				UTF16BE,
				UTF32LE,
				UTF32BE
			}FILE_ENCODING;

			static FILE_ENCODING	GetEncoding(cu8* _pBuffer, cul32& _ulBufferSize)
			{
				//Check for utf-8, get first 3 bytes
				cu8* pEncoding = _pBuffer;
				_pBuffer += 3;

				if( _ulBufferSize > 2 && pEncoding[0] == 0xEF && pEncoding[1] == 0xBB && pEncoding[2] == 0xBF)
				{
					return UTF8;
				}

				if( _ulBufferSize > 1 && pEncoding[0] == 0xFF && pEncoding[1] == 0xFE )
				{
					return UTF16LE;
				}

				if( _ulBufferSize > 1 && pEncoding[0] == 0xFE && pEncoding[1] == 0xFF )
				{
					return UTF16BE;
				}

				if( _ulBufferSize > 3 && pEncoding[0] == 0x00 && pEncoding[1] == 0x00 && pEncoding[2] == 0xFE && pEncoding[3] == 0xFF )
				{
					return UTF32BE;
				}

				if( _ulBufferSize > 3 && pEncoding[0] == 0xFE && pEncoding[1] == 0xFF && pEncoding[2] == 0x00 && pEncoding[3] == 0x00 )
				{
					return UTF32LE;
				}

				return ANSI;
			}
			
			static FILE_ENCODING GetEncoding(xst_unknown hFile)
			{
				if( !hFile )
				{
					return ITextFile::UNKNOWN;
				}

				l32 lCurrPos = 0;
				IFile::GetPosition( hFile, &lCurrPos );

				IFile::SetPosition( hFile, 0, IFile::BEGIN );
				u8 abyHeader[ 4 ];
				IFile::Read( hFile, abyHeader, 4 );

				IFile::SetPosition( hFile, lCurrPos, IFile::BEGIN );
				return GetEncoding( abyHeader, 4 );
			}

			static void GetUtf8CharCount(cch8* pBuff, cul32& ulBuffSize, ul32* pulAsciiCountOut, ul32* pulUnicodeCountOut)
			{
				cch8* pPtr = pBuff;
				ul32 ulAsciiCount = 0, ulUnicodeCount = 0;
				
				for(ul32 i = 0; i < ulBuffSize; ++i)
				{
					if( (u8)pBuff[ i ] > 127 )
					{
						++ulUnicodeCount;
						++i;
					}
					else
					{
						++ulAsciiCount;
					}
				}

				*pulAsciiCountOut = ulAsciiCount;
				*pulUnicodeCountOut = ulUnicodeCount;
			}

			xst_fi static bool IsUTF16(const FILE_ENCODING& _eEncoding)
			{
				return _eEncoding == UTF16LE || _eEncoding == UTF16BE;
			}

			xst_fi static bool IsUTF32(const FILE_ENCODING& _eEncoding)
			{
				return _eEncoding == UTF32LE || _eEncoding == UTF32BE;
			}

	};

}//XST

#endif