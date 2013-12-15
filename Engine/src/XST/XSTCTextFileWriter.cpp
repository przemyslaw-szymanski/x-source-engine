#include "../../include/XST/XSTCTextFileWriter.h"

namespace XST
{

    CTextFileWriter::CTextFileWriter(xst_castring& _strFileName)
    {
        m_pHFile = IFile::Open(_strFileName.c_str(), IFile::WRITE);
    }

    CTextFileWriter::CTextFileWriter() : m_pHFile( xst_null )
    {
    }

    CTextFileWriter::~CTextFileWriter()
    {
		Close();
    }

    i32	CTextFileWriter::Open(xst_castring& _strFileName)
    {
        m_pHFile = IFile::Create(_strFileName, IFile::WRITE);
        if(m_pHFile)
            return RESULT::OK;

		XST::CLastError::Set( "Can't open file for write." );
        return RESULT::FAILED;
    }

    i32	CTextFileWriter::Open(xst_cwstring& _strFileName)
    {
        m_pHFile = IFile::Create(_strFileName, IFile::WRITE);
        if(m_pHFile)
            return RESULT::OK;

		XST::CLastError::Set( "Can't open file for write." );
        return RESULT::FAILED;
    }

    i32	CTextFileWriter::OpenAppend(xst_castring& _strFileName)
    {
        m_pHFile = IFile::Open(_strFileName, IFile::APPEND);
        if(m_pHFile)
            return RESULT::OK;

		XST::CLastError::Set( "Can't open file for append write." );
        return RESULT::FAILED;
    }

    i32	CTextFileWriter::OpenAppend(xst_cwstring& _strFileName)
    {
        m_pHFile = IFile::Open(_strFileName, IFile::APPEND);
        if(m_pHFile)
            return RESULT::OK;

		XST::CLastError::Set( "Can't open file append write." );
        return RESULT::FAILED;
    }

    void	CTextFileWriter::Close()
    {
        IFile::Close(m_pHFile);
        m_pHFile = 0;
    }

    i32	CTextFileWriter::WriteLine(xst_castring& _strText)
    {
        xst_assert(m_pHFile, "File not opened/created");
        if( !IFile::Write(m_pHFile, (void*)_strText.data(), _strText.length() * sizeof(ch8)) )
		{
			XST::CLastError::Set( "Can't write file." );
            return RESULT::FAILED;
		}

		IFile::Write(m_pHFile, (void*)"\r\n", 2);
        return RESULT::OK;
    }

	i32	CTextFileWriter::WriteLine(xst_cwstring& strText)
    {
        xst_assert(m_pHFile, "File not opened/created");
		if( !IFile::Write(m_pHFile, (void*)strText.data(), strText.length() * sizeof(ch16) ) )
		{
			XST::CLastError::Set( "Can't write file." );
            return RESULT::FAILED;
		}

        IFile::Write(m_pHFile, (void*)(L"\r\n"), 4);
        return RESULT::OK;
    }

	i32	CTextFileWriter::Write(xst_castring& strText)
    {
        xst_assert(m_pHFile, "File not opened/created");
        if( !IFile::Write(m_pHFile, (void*)strText.data(), strText.length() * sizeof(ch8) ) )
		{
			XST::CLastError::Set( "Can't write file." );
            return RESULT::FAILED;
		}

        return RESULT::OK;
    }

	i32	CTextFileWriter::Write(xst_cwstring& strText)
    {
        xst_assert(m_pHFile, "File not opened/created");
        if( !IFile::Write(m_pHFile, (void*)strText.data(), strText.length() * sizeof(ch16) ) )
		{
			XST::CLastError::Set( "Can't write file." );
            return RESULT::FAILED;
		}

        return RESULT::OK;
    }

    i32 CTextFileWriter::Write(lpcastr _lpszFormat, ...)
    {
        va_list vl;
        va_start( vl, _lpszFormat );
        ch8 achBuff[1024];
        //vsprintf_s( achBuff, 1024, _lpszFormat, vl );
        xst_vsprintf( achBuff, 1024, _lpszFormat, vl );
        va_end( vl );

        return !IFile::Write( m_pHFile, achBuff, 1024 );
    }

    i32 CTextFileWriter::Flush()
    {
        xst_assert(m_pHFile, "File not opened/created");
        return !IFile::Flush( m_pHFile );
        //return 0;
    }

}//XST
