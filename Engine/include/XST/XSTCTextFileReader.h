#ifndef XST_TEXT_FILE_READER_H
#define XST_TEXT_FILE_READER_H

#include "XSTIFileReader.h"
#include "XSTIAllocator.h"

namespace XST
{
	//Supports only ascii and unicode
	class CTextFileReader : public ITextFile
	{
		public:

			CTextFileReader() : m_hFile(0), m_bDeleteOnClose(true), m_ulSize( 0 ), m_pAsciiFile( xst_null ), m_pUnicodeFile( xst_null )
			{
			}

			~CTextFileReader()
			{
				Close();
			}

			void Close()
			{
				if( !m_bDeleteOnClose )
					return;

				/*if( m_eEncoding == ITextFile::ANSI || m_eEncoding == ITextFile::UTF8 )
				{
					xst_deletea( m_pAsciiFile );
				}
				else
				{
					if( m_eEncoding == ITextFile::UTF16LE || m_eEncoding == ITextFile::UTF16BE || 
						m_eEncoding == ITextFile::UTF32BE || m_eEncoding == ITextFile::UTF32LE )
					{
						xst_deletea( m_pUnicodeFile );
					}
				}*/

				xst_deletea( m_pAsciiFile );
				xst_deletea( m_pUnicodeFile );

				m_ulSize = 0;

				IFile::Close( m_hFile );
				m_hFile = xst_null;
			}

			i32 Open(lpcastr _lpszFileName)
			{
				m_hFile = IFile::Open( _lpszFileName, IFile::READ );
				if( !m_hFile )
				{
					return RESULT::FAILED;
				}
				m_ulSize = IFile::GetSize( m_hFile );
				return RESULT::OK;
			}

			i32 Open(xst_castring& _strFileName)
			{
				m_hFile = IFile::Open( _strFileName.data(), IFile::READ );
				if( !m_hFile )
				{
					return RESULT::FAILED;
				}
				m_ulSize = IFile::GetSize( m_hFile );
				return RESULT::OK;
			}

			i32 Open(lpcwstr _lpszFileName)
			{
				m_hFile = IFile::Open( _lpszFileName, IFile::READ );
				if( !m_hFile )
				{
					return RESULT::FAILED;
				}
				m_ulSize = IFile::GetSize( m_hFile );
				return RESULT::OK;
			}

			i32 Open(xst_cwstring& _strFileName)
			{
				m_hFile = IFile::Open( _strFileName.data(), IFile::READ );
				if( !m_hFile )
				{
					return RESULT::FAILED;
				}
				m_ulSize = IFile::GetSize( m_hFile );
				return RESULT::OK;
			}


			i32 ReadAll(bool _bDeleteOnClose = true)
			{
				m_bDeleteOnClose = _bDeleteOnClose;
				//Get encoding
				if( m_ulSize > 0 )
				{
					m_eEncoding = ITextFile::GetEncoding( m_hFile );
					if( m_eEncoding == ITextFile::ANSI)
					{
						m_ulCharCount = m_ulSize;
						m_pAsciiFile = xst_new ch8[ m_ulCharCount + 1 ];
						IFile::Read( m_hFile, m_pAsciiFile, m_ulSize );
						m_pAsciiFile[ m_ulCharCount ] = 0;
					}
					else if( m_eEncoding == ITextFile::UTF8 )
					{
						m_ulSize -= 3;
						m_ulCharCount = m_ulSize;
						m_pAsciiFile = xst_new ch8[ m_ulCharCount + 1 ];
						ch8* pTmp = xst_new ch8[ m_ulCharCount + 1 ];
						IFile::SetPosition( m_hFile, 3, IFile::BEGIN );
						IFile::Read( m_hFile, pTmp, m_ulSize );
						pTmp[ m_ulCharCount ] = 0;
						
						u8* pSrc = (u8*)pTmp, *pDst = (u8*)m_pAsciiFile;
						while( *pSrc )
						{
							u8 b = (u8)*pSrc;
							*pDst = *pSrc;

							if( b > 127 )
							{	
								++pDst;
								++pSrc;
							}

							++pDst;
							++pSrc;
						}

						m_pAsciiFile[ m_ulCharCount ] = 0;

						xst_deletea( pTmp );
					}
					else if( ITextFile::IsUTF16( m_eEncoding ) )
					{
						m_ulCharCount = ( m_ulSize - 2 ) / sizeof( ch16 );
						m_pUnicodeFile = xst_new ch16[ m_ulCharCount + 1 ];
						IFile::SetPosition( m_hFile, 2, IFile::BEGIN );
						IFile::Read( m_hFile, m_pUnicodeFile, m_ulSize - 2 );
						m_pUnicodeFile[ m_ulCharCount ] = 0;
					}

					return RESULT::OK;
				}

				return RESULT::FAILED;
			}

			i32 FromBytes(cu8* _pData, cul32& _ulDataSize, bool _bDeleteOnClose = true)
			{
				m_bDeleteOnClose = _bDeleteOnClose;
				//Check encoding
				cu8* pFile = _pData;

				m_eEncoding = ITextFile::GetEncoding( _pData, _ulDataSize );
				if( m_eEncoding == ITextFile::ANSI )
				{
					m_ulSize = _ulDataSize;
					m_pAsciiFile = xst_new ch8[ m_ulSize + 1 ];
					//memcpy_s( m_pAsciiFile, m_ulSize, pFile, _ulDataSize );
					xst_memcpy( m_pAsciiFile, m_ulSize, pFile, _ulDataSize );
					m_pAsciiFile[ m_ulSize ] = 0;
					return RESULT::OK;
				}

				if( ITextFile::IsUTF16( m_eEncoding ) )
				{
					//Skip BOM
					pFile += 2;
					m_ulSize = (_ulDataSize - 2) / sizeof( ch16 ) ;
					m_pUnicodeFile = xst_new ch16[ m_ulSize ];
					//memcpy_s( m_pUnicodeFile, _ulDataSize - 2, pFile, _ulDataSize - 2 );
					xst_memcpy( m_pUnicodeFile, _ulDataSize - 2, pFile, _ulDataSize - 2 );
					m_pUnicodeFile[ m_ulSize ] = 0;
					return RESULT::OK;
				}

				//Utf32 is not supported
				return RESULT::FAILED;

			}

			xst_fi const ch8* GetAsciiData()
			{
				return m_pAsciiFile;
			}

			xst_fi const ch16* GetUnicodeData()
			{
				return m_pUnicodeFile;
			}

			xst_fi ul32 GetSize()
			{
				return m_ulSize;
			}

			FILE_ENCODING GetEncoding()
			{
				return m_eEncoding;
			}

			xst_fi bool IsOpened()
			{
				return m_hFile != xst_null;
			}

			xst_fi bool	IsEncodingSupported(const ITextFile::FILE_ENCODING& _eEncoding)
			{
				return _eEncoding == ITextFile::ANSI || _eEncoding == ITextFile::UTF16BE || _eEncoding == ITextFile::UTF16LE;
			}

		private:

			bool m_bDeleteOnClose;
			xst_unknown	m_hFile;
			FILE_ENCODING m_eEncoding;
			ul32 m_ulSize;
			ul32 m_ulCharCount;
			
			ch8*	m_pAsciiFile;
			ch16*	m_pUnicodeFile;
			
			union
			{
				
			};

			//SFile		m_File;
			//IAllocator*	m_pAllocator;
	};

}//XST

#endif
