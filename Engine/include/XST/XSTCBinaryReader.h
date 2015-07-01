#ifndef XST_BINARY_READER_H
#define XST_BINARY_READER_H

#include "XSTIFile.h"

namespace XST
{
	class XST_API CBinaryReader : public IFile
	{
		public:

							CBinaryReader() : m_bIsError( false ), m_hFile(0), m_pData(0), m_ulDataSize(0), m_pCurrPtr(0), m_bDeleteOnClose( true )
							{
							}

							~CBinaryReader()
							{
								Close();
							}

			xst_fi	i32		Open(lpcastr _lpszFilename)
			{
				m_hFile = IFile::Open(_lpszFilename, IFile::READ_BINARY);
				if( !m_hFile )
				{
					return RESULT::FAILED;
				}

				m_ulDataSize = (ul32)IFile::GetSize( m_hFile );

				return RESULT::OK;
			}

			xst_fi cu8*		GetData()
			{
				return m_pData;
			}

			xst_fi	i32		Open(lpcwstr _lpszFilename)
			{
				m_hFile = IFile::Open(_lpszFilename, IFile::READ_BINARY);
				if( !m_hFile )
				{
					return RESULT::FAILED;
				}

				m_ulDataSize = IFile::GetSize( m_hFile );

				return RESULT::OK;
			}

			void	Close()
			{
				IFile::Close(m_hFile);
				m_hFile = 0;
				if(m_bDeleteOnClose)
					xst_deletea(m_pData);
				m_ulDataSize = 0;
				m_pCurrPtr = 0;
				m_bIsError = false;
			}

			xst_i	i32		ReadAll(bool _bDeleteOnClose = true)
			{
				if( m_pData != xst_null && m_bDeleteOnClose )
				{
					xst_deletea( m_pData );
				}

				m_pData = xst_new u8[ m_ulDataSize + 1 ];

				if( IFile::ReadAll(m_hFile, &m_pData, &m_ulDataSize) )
				{
					m_bDeleteOnClose = _bDeleteOnClose;
					m_pCurrPtr = m_pData;
					m_pData[ m_ulDataSize ] = 0;

					return RESULT::OK;
				}
				
				//If failed
				xst_deletea( m_pData );
				return RESULT::FAILED;
			}

			xst_i i32		ReadAll(u8** ppDataBufferOut, cul32& ulBufferSize)
			{
				if( IFile::ReadAll( m_hFile, ppDataBufferOut, ulBufferSize ) )
				{
					return RESULT::OK;
				}
				return RESULT::FAILED;
			}

			void	SetData(u8* _pDataBuffer, cul32& _ulBufferSize, bool _bDeleteOnClose)
			{
				if( m_pData )
				{
					xst_deletea( m_pData );
				}

				m_pData = _pDataBuffer;
				m_pCurrPtr = m_pData;
				m_ulDataSize = _ulBufferSize;
				m_bDeleteOnClose = _bDeleteOnClose;
			}

			xst_fi	u8		ReadByte()
			{
				return ReadSimpleData<u8>();
			}

			xst_fi	ch8		ReadChar()
			{
				return ReadSimpleData<ch8>();
			}

			xst_fi	ch16	ReadWChar()
			{
				return ReadSimpleData<ch16>();
			}

			xst_fi	i16		ReadInt16()
			{
				return ReadSimpleData<i16>();
			}

			xst_fi	u16		ReadUInt16()
			{
				return ReadSimpleData<u16>();
			}

			xst_fi	i32		ReadInt32()
			{
				return ReadSimpleData<i32>();
			}

			xst_fi	u32		ReadUInt32()
			{
				return ReadSimpleData<u32>();
			}

			xst_fi	l32		ReadLong32()
			{
				return ReadSimpleData<l32>();
			}

			xst_fi	ul32	ReadULong32()
			{
				return ReadSimpleData<ul32>();
			}

			xst_fi	f32		ReadFloat32()
			{
				return ReadSimpleData<f32>();
			}

			xst_fi	f64		ReadFloat64()
			{
				return ReadSimpleData<f64>();
			}

			xst_fi	void	ReadChars(ch8** ppOutBuffer, const u32& uiBytesToRead)
			{
				xst_assert2( ppOutBuffer && *ppOutBuffer );
				m_bIsError = false;
				if( m_pData )
				{
					xst_assert(m_pData != 0, "File not read");
					xst_memcpy( ppOutBuffer, uiBytesToRead, m_pCurrPtr, uiBytesToRead );
					m_pCurrPtr += uiBytesToRead;
					(*ppOutBuffer)[ uiBytesToRead ] = 0;
				}
				else
				{
					if( !IFile::Read( m_hFile, (u8**)ppOutBuffer, uiBytesToRead ) )
					{
						m_bIsError = true;
					}
					(*ppOutBuffer)[ uiBytesToRead ] = 0;
					m_pCurrPtr += uiBytesToRead;
				}
			}

			xst_fi	void	ReadBytes(u8** ppOutBuffer, const u32& uiBytesToRead)
			{
				xst_assert2( ppOutBuffer && *ppOutBuffer );
				m_bIsError = false;
				if( m_pData )
				{
					xst_assert(m_pData != 0, "File not read");
					xst_memcpy( ppOutBuffer, uiBytesToRead, m_pCurrPtr, uiBytesToRead );
					m_pCurrPtr += uiBytesToRead;
					(*ppOutBuffer)[ uiBytesToRead ] = 0;
				}
				else
				{
					if( !IFile::Read( m_hFile, ppOutBuffer, uiBytesToRead ) )
					{
						m_bIsError = true;
					}
					(*ppOutBuffer)[ uiBytesToRead ] = 0;
					m_pCurrPtr += uiBytesToRead;
				}
			}

			xst_i xst_astring ReadString()
			{
				m_bIsError = false;
				if( m_pData )
				{
					xst_assert(m_pData != 0, "File not read");
					//Read string size
					u32 uSize = ReadUInt32();
					if( uSize <= 0 )
						return "";

					ch8* pBuff = (ch8*)xst_malloc( uSize + 1 );
					if( !pBuff )
						return XST::StringUtil::EmptyAString;
					ReadChars( &pBuff, uSize );
					xst_astring s( pBuff, uSize );
					xst_free( pBuff );
					return s;
				}
				else
				{
					u32 uSize = ReadUInt32();
					if( uSize <= 0 )
						return "";

					ch8* pBuff = (ch8*)xst_malloc( uSize + 1 );
					if( !pBuff )
						return XST::StringUtil::EmptyAString;
					if( !IFile::Read( m_hFile, (u8**)&pBuff, uSize ) )
					{
						m_bIsError = true;
					}

					m_pCurrPtr += sizeof( ch8 ) * uSize;

					xst_astring s( pBuff, uSize );
					xst_free( pBuff );
					return s;
				}
			}

			template<class _STR_>
			xst_i _STR_ ReadSimpleString()
			{
				m_bIsError = false;
				if( m_pData )
				{
					u32 uiSize = ReadUInt32();
					u32 uSize = ReadUInt32();
					ch8* pBuff = (ch8*)xst_malloc(uSize);
					ReadChars(pBuff, uSize);
					_STR_ s(pBuff, uSize);
					xst_free(pBuff);
					return s;
				}
				else
				{
					u32 uiSize = ReadUInt32();
					ch8* pBuff = (ch8*)xst_malloc(uiSize);
					if( !IFile::Read( m_hFile, pBuff, uiSize ) )
					{
						m_bIsError = true;
					}

					(ul32)m_pCurrPtr += sizeof( ch8 ) * uiSize;
					_STR_ s(pBuff, uiSize);
					xst_free(pBuff);
					return s;
				}
			}

			template<typename _T_>
			xst_fi _T_ ReadSimpleData()
			{
				m_bIsError = false;
				if( m_pData )
				{
					xst_assert(m_pData != 0, "File not read");
					xst_assert( m_pCurrPtr + sizeof( _T_ ) > m_pData + m_ulDataSize, "Out of range" );
					_T_ value = *(_T_*)m_pCurrPtr;
					m_pCurrPtr += sizeof(_T_);
					return value;
				}
				else
				{
					_T_ tValue;
					if( !IFile::Read( m_hFile, (u8**)&tValue, sizeof( _T_ ) ) )
					{
						m_bIsError = true;
					}
					m_pCurrPtr += sizeof( _T_ );
					return tValue;
				}
			}

			template<typename _T_>
			xst_fi i32 Read(_T_* pDataOut)
			{
				m_bIsError = false;
				if( IsEOF() )
				{
					m_bIsError = true;
					return RESULT::FAILED;
				}

				if( m_pData )
				{
					xst_assert(m_pData != 0, "File not read");

					_T_ value = *(_T_*)m_pCurrPtr;
					(*pDataOut) = value;
					m_pCurrPtr += sizeof(_T_);
					return RESULT::OK;
				}
				else
				{
					if( !IFile::Read( m_hFile, pDataOut, sizeof( _T_ ) ) )
					{
						m_bIsError = true;
						return RESULT::FAILED;
					}
					m_pCurrPtr += sizeof( _T_ );
					return RESULT::OK;
				}
			}

			xst_fi bool IsEOF()
			{
				return IsEOF( 0 );
			}

			xst_fi bool IsEOF(cul32& ulOffset)
			{
				if( m_ulDataSize == 0 )
				{
					m_ulDataSize = GetSize();
				}
				ul32 ulCurrPos = (ul32)m_pCurrPtr;
				return ulCurrPos + ulOffset >= m_ulDataSize;
			}

			xst_fi i32 SetPosition(cul32& ulPos, const IFile::SEEK_POSITION& eSeekPos)
			{
				switch( eSeekPos )
				{
					case IFile::BEGIN:		m_pCurrPtr = (u8*)ulPos; break;
					case IFile::CURRENT:	m_pCurrPtr += ulPos; break;
					case IFile::END:		m_pCurrPtr = (u8*)(GetSize() - ulPos); break;
				}
				ul32 ulP = (ul32)m_pCurrPtr;
				if( !m_pData )
				{
					return IFile::SetPosition( m_hFile, ulPos, eSeekPos );
				}

				return RESULT::OK;
			}

			xst_fi ul32	GetPosition() const
			{
				return (ul32)m_pCurrPtr;
			}

			xst_fi bool IsOpened() const
			{
				return m_hFile != xst_null;
			}

			xst_fi bool IsReadError() const
			{
				return m_bIsError;
			}

			xst_fi ul32	GetSize() const
			{
				return m_ulDataSize;
			}

		private:

			xst_unknown		m_hFile;
			u8*				m_pData;
			u8*				m_pCurrPtr;
			ul32			m_ulDataSize;
			bool			m_bDeleteOnClose;
			bool			m_bIsError;
            u8              m_padding[ 2 ];
	};



}//XST

#endif
