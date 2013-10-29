#ifndef XST_BINARY_WRITER_H
#define XST_BINARY_WRITER_H

#include "XSTIFile.h"
#include "XSTIThread.h"

namespace XST
{
	class CBinaryWriter : public IFile
	{
		public:

							CBinaryWriter() : m_hFile(0)
							{
							}

							~CBinaryWriter()
							{
								Close();
							}

			xst_fi	void	Close()
			{
				IFile::Close(m_hFile);
				m_hFile = 0;
			}

			xst_fi	i32		Open(lpcastr _lpszFileName)
			{
				m_hFile = IFile::Open(_lpszFileName, IFile::WRITE_BINARY);
				if(m_hFile)
					return RESULT::OK;

				return RESULT::FAILED;
			}

			xst_fi	i32		Open(lpcwstr _lpszFileName)
			{
				m_hFile = IFile::Open(_lpszFileName, IFile::WRITE_BINARY);
				if(m_hFile)
					return RESULT::OK;

				return RESULT::FAILED;
			}

			xst_fi	i32		OpenAppend(lpcastr _lpszFileName)
			{
				m_hFile = IFile::Open(_lpszFileName, IFile::APPEND_BINARY);
				if(m_hFile)
					return RESULT::OK;

				return RESULT::FAILED;
			}

			xst_fi	i32		OpenAppend(lpcwstr _lpszFileName)
			{
				m_hFile = IFile::Open(_lpszFileName, IFile::APPEND_BINARY);
				if(m_hFile)
					return RESULT::OK;

				return RESULT::FAILED;
			}

			xst_fi	i32		Open(xst_castring& _strFileName)
			{
				m_hFile = IFile::Open(_strFileName.data(), IFile::WRITE_BINARY);
				if(m_hFile)
					return RESULT::OK;

				return RESULT::FAILED;
			}

			xst_fi	i32		Open(xst_cwstring& _strFileName)
			{
				m_hFile = IFile::Open(_strFileName.data(), IFile::WRITE_BINARY);
				if(m_hFile)
					return RESULT::OK;

				return RESULT::FAILED;
			}

			xst_fi	i32		OpenAppend(xst_castring& _strFileName)
			{
				m_hFile = IFile::Open(_strFileName.data(), IFile::APPEND_BINARY);
				if(m_hFile)
					return RESULT::OK;

				return RESULT::FAILED;
			}

			xst_fi	i32		OpenAppend(xst_cwstring& _strFileName)
			{
				m_hFile = IFile::Open(_strFileName.data(), IFile::APPEND_BINARY);
				if(m_hFile)
					return RESULT::OK;

				return RESULT::FAILED;
			}

			xst_fi	i32		Write(const u8 _data)
							{ 
								return !IFile::Write(m_hFile, (void*)&_data, sizeof(u8)); 
							}

			xst_fi	i32		Write(const ch8 _data)
							{ 
								return !IFile::Write(m_hFile, (void*)&_data, sizeof(ch8)); 
							}

			xst_fi	i32		Write(const ch16 _data)
							{ 
								return !IFile::Write(m_hFile, (void*)&_data, sizeof(ch16)); 
							}

			xst_fi	i32		Write(const u16 _data)
							{ 
								return !IFile::Write(m_hFile, (void*)&_data, sizeof(u16)); 
							}

			xst_fi	i32		Write(const i16 _data)
							{ 
								return !IFile::Write(m_hFile, (void*)&_data, sizeof(i16)); 
							}

			xst_fi	i32		Write(const u32 _data)
							{ 
								return !IFile::Write(m_hFile, (void*)&_data, sizeof(u32)); 
							}

			xst_fi	i32		Write(const i32 _data)
							{ 
								return !IFile::Write(m_hFile, (void*)&_data, sizeof(i32)); 
							}

			xst_fi	i32		Write(const ul32 _data)
							{ 
								return !IFile::Write(m_hFile, (void*)&_data, sizeof(ul32)); 
							}

			xst_fi	i32		Write(const l32 _data)
							{ 
								return !IFile::Write(m_hFile, (void*)&_data, sizeof(l32)); 
							}

			xst_fi	i32		Write(const f32 _data)
							{ 
								return !IFile::Write(m_hFile, (void*)&_data, sizeof(f32)); 
							}

			xst_fi	i32		Write(const f64 _data)
							{ 
								return !IFile::Write(m_hFile, (void*)&_data, sizeof(f64)); 
							}

			xst_fi	i32		Write(lpcastr _data, const u32 _uiDataLength)
							{ 
								return !IFile::Write(m_hFile, (void*)_data, _uiDataLength); 
							}

			xst_fi	i32		Write(lpcwstr _data, const u32 _uiDataLength)
							{ 
								return !IFile::Write(m_hFile, (void*)_data, _uiDataLength * sizeof(ch16)); 
							}

			xst_fi	i32		Write(xst_castring& _data)
							{ 
								const u32 uiLen = _data.length();
								if( !IFile::Write(m_hFile, (void*)&uiLen, sizeof(u32)) )
									return RESULT::FAILED;

								return !IFile::Write(m_hFile, (void*)_data.data(), sizeof(ch8) * uiLen); 
							}

			xst_fi	i32		Write(xst_cwstring& _data)
							{ 
								const u32 uiLen = _data.length();
								if( !IFile::Write(m_hFile, (void*)&uiLen, sizeof(u32)) )
									return RESULT::FAILED;

								return !IFile::Write(m_hFile, (void*)_data.data(), sizeof(ch16) * uiLen); 
							}

			xst_fi	i32		Write(cu8* _pData, ul32 _ulDataSize)
							{
								return !IFile::Write( m_hFile, (void*)_pData, _ulDataSize );
							}

			template<class _T_>
			xst_fi	i32		WriteAny(const _T_& _tData)
							{
								return !IFile::Write( m_hFile, (void*)&_tData, sizeof(_T_) ) ;
							}

			template<class _T_>
			xst_fi	i32		WriteAny(const _T_& _tData, ul32 _ulDataSize)
							{
								return !IFile::Write( m_hFile, (void*)&_tData, _ulDataSize) ;
							}

		private:

			xst_unknown		m_hFile;
	};


	class CBinaryWriterTS : public IFile
	{
		public:

							CBinaryWriterTS() : m_hFile(0), m_CritSection( XST::CCriticalSection() )
							{
							}

							~CBinaryWriterTS()
							{
								Close();
							}

			xst_fi	void	Close()
			{
				XST::CLockSection Lock( m_CritSection );
				IFile::Close(m_hFile);
				m_hFile = 0;
			}

			xst_fi	i32		Open(lpcastr _lpszFileName)
			{
				XST::CLockSection Lock( m_CritSection );
				m_hFile = IFile::Open(_lpszFileName, IFile::WRITE_BINARY);
				if(m_hFile)
					return RESULT::OK;

				return RESULT::FAILED;
			}

			xst_fi	i32		Open(lpcwstr _lpszFileName)
			{
				XST::CLockSection Lock( m_CritSection );
				m_hFile = IFile::Open(_lpszFileName, IFile::WRITE_BINARY);
				if(m_hFile)
					return RESULT::OK;

				return RESULT::FAILED;
			}

			xst_fi	i32		OpenAppend(lpcastr _lpszFileName)
			{
				XST::CLockSection Lock( m_CritSection );
				m_hFile = IFile::Open(_lpszFileName, IFile::APPEND_BINARY);
				if(m_hFile)
					return RESULT::OK;

				return RESULT::FAILED;
			}

			xst_fi	i32		OpenAppend(lpcwstr _lpszFileName)
			{
				XST::CLockSection Lock( m_CritSection );
				m_hFile = IFile::Open(_lpszFileName, IFile::APPEND_BINARY);
				if(m_hFile)
					return RESULT::OK;

				return RESULT::FAILED;
			}

			xst_fi	i32		Open(xst_castring& _strFileName)
			{
				XST::CLockSection Lock( m_CritSection );
				m_hFile = IFile::Open(_strFileName.data(), IFile::WRITE_BINARY);
				if(m_hFile)
					return RESULT::OK;

				return RESULT::FAILED;
			}

			xst_fi	i32		Open(xst_cwstring& _strFileName)
			{
				XST::CLockSection Lock( m_CritSection );
				m_hFile = IFile::Open(_strFileName.data(), IFile::WRITE_BINARY);
				if(m_hFile)
					return RESULT::OK;

				return RESULT::FAILED;
			}

			xst_fi	i32		OpenAppend(xst_castring& _strFileName)
			{
				XST::CLockSection Lock( m_CritSection );
				m_hFile = IFile::Open(_strFileName.data(), IFile::APPEND_BINARY);
				if(m_hFile)
					return RESULT::OK;

				return RESULT::FAILED;
			}

			xst_fi	i32		OpenAppend(xst_cwstring& _strFileName)
			{
				XST::CLockSection Lock( m_CritSection );
				m_hFile = IFile::Open(_strFileName.data(), IFile::APPEND_BINARY);
				if(m_hFile)
					return RESULT::OK;

				return RESULT::FAILED;
			}

			xst_fi	i32		Write(const u8 _data)
							{ 
								XST::CLockSection Lock( m_CritSection );
								return !IFile::Write(m_hFile, (void*)&_data, sizeof(u8)); 
							}

			xst_fi	i32		Write(const ch8 _data)
							{ 
								XST::CLockSection Lock( m_CritSection );
								return !IFile::Write(m_hFile, (void*)&_data, sizeof(ch8)); 
							}

			xst_fi	i32		Write(const ch16 _data)
							{ 
								XST::CLockSection Lock( m_CritSection );
								return !IFile::Write(m_hFile, (void*)&_data, sizeof(ch16)); 
							}

			xst_fi	i32		Write(const u16 _data)
							{ 
								XST::CLockSection Lock( m_CritSection );
								return !IFile::Write(m_hFile, (void*)&_data, sizeof(u16)); 
							}

			xst_fi	i32		Write(const i16 _data)
							{ 
								XST::CLockSection Lock( m_CritSection );
								return !IFile::Write(m_hFile, (void*)&_data, sizeof(i16)); 
							}

			xst_fi	i32		Write(const u32 _data)
							{ 
								XST::CLockSection Lock( m_CritSection );
								return !IFile::Write(m_hFile, (void*)&_data, sizeof(u32)); 
							}

			xst_fi	i32		Write(const i32 _data)
							{ 
								XST::CLockSection Lock( m_CritSection );
								return !IFile::Write(m_hFile, (void*)&_data, sizeof(i32)); 
							}

			xst_fi	i32		Write(const ul32 _data)
							{ 
								XST::CLockSection Lock( m_CritSection );
								return !IFile::Write(m_hFile, (void*)&_data, sizeof(ul32)); 
							}

			xst_fi	i32		Write(const l32 _data)
							{ 
								XST::CLockSection Lock( m_CritSection );
								return !IFile::Write(m_hFile, (void*)&_data, sizeof(l32)); 
							}

			xst_fi	i32		Write(const f32 _data)
							{ 
								XST::CLockSection Lock( m_CritSection );
								return !IFile::Write(m_hFile, (void*)&_data, sizeof(f32)); 
							}

			xst_fi	i32		Write(const f64 _data)
							{ 
								XST::CLockSection Lock( m_CritSection );
								return !IFile::Write(m_hFile, (void*)&_data, sizeof(f64)); 
							}

			xst_fi	i32		Write(lpcastr _data, const u32 _uiDataLength)
							{ 
								XST::CLockSection Lock( m_CritSection );
								return !IFile::Write(m_hFile, (void*)_data, _uiDataLength); 
							}

			xst_fi	i32		Write(lpcwstr _data, const u32 _uiDataLength)
							{ 
								XST::CLockSection Lock( m_CritSection );
								return !IFile::Write(m_hFile, (void*)_data, _uiDataLength * sizeof(ch16)); 
							}

			xst_fi	i32		Write(xst_castring& _data)
							{ 
								XST::CLockSection Lock( m_CritSection );
								const u32 uiLen = _data.length();
								if( !IFile::Write(m_hFile, (void*)&uiLen, sizeof(u32)) )
									return RESULT::FAILED;

								return !IFile::Write(m_hFile, (void*)_data.data(), sizeof(ch8) * uiLen); 
							}

			xst_fi	i32		Write(xst_cwstring& _data)
							{ 
								XST::CLockSection Lock( m_CritSection );
								const u32 uiLen = _data.length();
								if( !IFile::Write(m_hFile, (void*)&uiLen, sizeof(u32)) )
									return RESULT::FAILED;

								return !IFile::Write(m_hFile, (void*)_data.data(), sizeof(ch16) * uiLen); 
							}

			xst_fi	i32		Write(cu8* _pData, ul32 _ulDataSize)
							{
								XST::CLockSection Lock( m_CritSection );
								return !IFile::Write( m_hFile, (void*)_pData, _ulDataSize );
							}

			template<class _T_>
			xst_fi	i32		WriteAny(const _T_& _tData)
							{
								XST::CLockSection Lock( m_CritSection );
								return !IFile::Write( m_hFile, (void*)&_tData, sizeof(_T_) ) ;
							}

			template<class _T_>
			xst_fi	i32		WriteAny(const _T_& _tData, ul32 _ulDataSize)
							{
								XST::CLockSection Lock( m_CritSection );
								return !IFile::Write( m_hFile, (void*)&_tData, _ulDataSize) ;
							}

		private:

			xst_unknown		m_hFile;
			XST::CCriticalSection	m_CritSection;
	};

}//XST

#endif