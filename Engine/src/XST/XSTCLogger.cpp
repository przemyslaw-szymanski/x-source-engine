#include "../../include/XST/XSTCLogger.h"


namespace XST
{
	CLogger::CLogger()
	{
		m_pLogger = 0;
		m_pDisabledLogger = 0;
		m_pEmptyLogger = xst_new CEmptyLogger2();
		m_ulLogMode = LAST_ERROR;
	}

	CLogger::~CLogger()
	{
		Enable();

		if( m_bAutoDestroyLogger )
		{
			xst_delete( m_pLogger );
		}

		xst_delete( m_pEmptyLogger );
	}

	i32	CLogger::Init(XST::ILogger* _pLogger, xst_castring& _strFileName, bool _bAutoDestroyLogger)
	{
		xst_assert( _pLogger != 0, "(XST::CLogger::Init) _pLogger cannot be null" );
		m_pLogger = _pLogger;
		m_bAutoDestroyLogger = _bAutoDestroyLogger;
		return m_pLogger->Init( _strFileName );
	}

	void CLogger::Enable()
	{
		if( m_pLogger != m_pDisabledLogger && m_pDisabledLogger != xst_null )
		{
			m_pLogger = m_pDisabledLogger;
			m_pDisabledLogger = 0;
			return;
		}

		m_bEnabled = true;
	}

	void CLogger::Disable()
	{
		if( m_pLogger != m_pEmptyLogger )
			m_pDisabledLogger = m_pLogger;

		m_pLogger = m_pEmptyLogger;
		m_bEnabled = false;
	}

	//Flush to other logger
	void	CLogger::FlushStream(ILogger* pLogger, const u32& uiMode)
	{
		if( m_bEnabled )
		{
			xst_castring& str = m_StrStream.str();

			if( uiMode & LAST_ERROR )
			{
				XST::CLastError::Set( str );
			}

			if( uiMode & FILE )
			{
				if( pLogger )
				{
					pLogger->Write( str );
				}
			}

			if( uiMode & CONSOLE )
			{
				XST::CDebug::PrintDebugLN( str );
			}
		}

		ClearStream();
	}

	void	CLogger::FlushStreamError(ILogger* pLogger, const u32& uiMode)
	{
		if( m_bEnabled )
		{
			xst_castring& str = m_StrStream.str();

			if( uiMode & LAST_ERROR )
			{
				XST::CLastError::Set( str );
			}

			if( uiMode & FILE )
			{
				if( pLogger )
				{
					pLogger->WriteError( str );
				}
			}

			if( uiMode & CONSOLE )
			{
				XST::CDebug::PrintDebugLN( str );
			}
		}

		ClearStream();
	}

	void	CLogger::FlushStreamWarning(ILogger* pLogger, const u32& uiMode)
	{
		if( m_bEnabled )
		{
			xst_castring& str = m_StrStream.str();

			if( uiMode & LAST_ERROR )
			{
				XST::CLastError::Set( str );
			}

			if( uiMode & FILE )
			{
				if( pLogger )
				{
					pLogger->WriteWarning( str );
				}
			}

			if( uiMode & CONSOLE )
			{
				XST::CDebug::PrintDebugLN( str );
			}
		}
		ClearStream();
	}



	//CTextLogger


	CTextLogger::CTextLogger()
	{
	}

	CTextLogger::~CTextLogger()
	{
	
	}

	i32 CTextLogger::Init(xst_castring& strFilename)
	{
		return m_Writer.Open( strFilename );
	}

	i32 CTextLogger::Write(xst_castring& strMsg)
	{
		CTime::GetSystemTime( &m_Time );
		std::stringstream ss;
		ss << "[" << m_Time.TimeToString() << "]" << strMsg;
		return m_Writer.WriteLine( ss.str() );
	}

	i32 CTextLogger::WriteError(xst_castring& strMsg)
	{
		CTime::GetSystemTime( &m_Time );
		std::stringstream ss;
		ss << "[" << m_Time.TimeToString() << "]" << "[ERROR]: " << strMsg;
		return m_Writer.WriteLine( ss.str() );
	}

	i32 CTextLogger::WriteWarning(xst_castring& strMsg)
	{
		CTime::GetSystemTime( &m_Time );
		std::stringstream ss;
		ss << "[" << m_Time.TimeToString() << "]" << "[WARNING]: " << strMsg;
		return m_Writer.WriteLine( ss.str() );
	}


	CHTMLLogger::CHTMLLogger()
	{
	}

	CHTMLLogger::~CHTMLLogger()
	{
		m_Writer.WriteLine( "</HTML>" );
	}

	i32 CHTMLLogger::Init(xst_castring& strFilename)
	{
		//m_Writer.Delete( strFilename + ".html", IFile::FILE );
		if( m_Writer.Open( strFilename + ".html" ) != RESULT::OK )
		{
			return RESULT::FAILED;
		}

		return m_Writer.WriteLine( "<HTML>" );
	}

	i32 CHTMLLogger::Write(xst_castring& strMsg)
	{
		CTime::GetSystemTime( &m_Time );
		std::stringstream ss;
		ss << "<br/><span><font color='black'>" << strMsg << "</font></span>";
		return m_Writer.Write( ss.str() );
	}

	i32 CHTMLLogger::WriteError(xst_castring& strMsg)
	{
		CTime::GetSystemTime( &m_Time );
		std::stringstream ss;
		ss << "<br/><span><font color='red'>" << "[" << m_Time.TimeToString() << "]" << "[ERROR]: " << strMsg << "</font></span>";
		return m_Writer.Write( ss.str() );
	}

	i32 CHTMLLogger::WriteWarning(xst_castring& strMsg)
	{
		CTime::GetSystemTime( &m_Time );
		std::stringstream ss;
		ss << "<br/><span><font color=#FFD000>" << "[" << m_Time.TimeToString() << "]" << "[WARNING]: " << strMsg << "</font></span>";
		return m_Writer.Write( ss.str() );
	}

}//xst