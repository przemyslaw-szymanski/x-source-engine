#ifndef XST_LOGGER_H
#define XST_LOGGER_H

#include "XSTCLock.h"
#include "XSTTCSingleton.h"
#include "XSTCDebug.h"
#include "XSTILogger.h"
#include "XSTCTextFileWriter.h"
#include "XSTCLastError.h"
#include "XSTCTime.h"


namespace XST
{

	class XST_API CLogger //: public XST::TCSingleton<CLogger>
	{

		public:

			enum
			{
				FILE		= 0x00000001,
				CONSOLE		= 0x00000010,
				LAST_ERROR	= 0x00000100,
				ALL			= FILE | CONSOLE | LAST_ERROR
			};

		public:

								CLogger();

				virtual			~CLogger();

					i32			Init(XST::ILogger* _pLogger, xst_castring& _strFileName, bool _bAutoDestroyLogger = true);

					void		SetMode(ul32 ulMode)
								{ m_ulLogMode |= ulMode; }

			xst_fi	ul32		GetMode()
								{ return m_ulLogMode; }

			xst_fi	ILogger*	GetLogger()
								{ return m_pLogger; }

			static	CLogger&	GetSingleton()
			{
				static CLogger Singleton;
				return Singleton;
			}

			template<typename _T_>
			xst_fi CLogger&	operator<<(const _T_& tMsg)
			{
				m_StrStream << tMsg;
				return *this;
			}

			xst_fi	void	FlushStream()
			{
				FlushStream( m_pLogger, m_ulLogMode );
			}

			xst_fi	void	FlushStream(ILogger* pLogger)
			{
				FlushStream( pLogger, m_ulLogMode );
			}

			xst_fi	void	FlushStream(const u32& uiMode)
			{
				FlushStream( m_pLogger, uiMode );
			}

			//Flush to other logger
			void	FlushStream(ILogger* pLogger, const u32& uiMode);

			xst_fi	void	FlushStreamError(const u32& uiMode = FILE)
			{
				FlushStreamError( m_pLogger, uiMode );
			}

			void	FlushStreamError(ILogger* pLogger, const u32& uiMode);

			xst_fi	void	FlushStreamWarning(const u32& uiMode = FILE)
			{
				FlushStreamWarning( m_pLogger, uiMode );
			}

			//Flush to other logger
			void	FlushStreamWarning(ILogger* pLogger, const u32& uiMode);

			xst_fi	void	ClearStream()
			{
				m_StrStream.clear();
				m_StrStream.str( "" );
			}

			xst_fi xst_astring	GetStream() const
			{
				return m_StrStream.str();
			}

			void Enable();

			void Disable();

		protected:



		private:

			ILogger*	m_pLogger;
			ILogger*	m_pEmptyLogger;
			ILogger*	m_pDisabledLogger;
			bool		m_bAutoDestroyLogger;
			std::stringstream	m_StrStream;
			ul32		m_ulLogMode;
	};


	class CTextLogger : public ILogger
	{
		public:

							CTextLogger();
			virtual			~CTextLogger();

			i32				Init(xst_castring& strFilename);

			i32				Write(xst_castring& strMsg);
			i32				WriteError(xst_castring& strMsg);
			i32				WriteWarning(xst_castring& strMsg);

		protected:

			CTextFileWriter	m_Writer;
			CTime::SSystemTime	m_Time;
	};

	class CHTMLLogger : public ILogger
	{
		public:
							CHTMLLogger();
			virtual			~CHTMLLogger();

			i32				Init(xst_castring& strFilename);

			i32				Write(xst_castring& strMsg);
			i32				WriteError(xst_castring& strMsg);
			i32				WriteWarning(xst_castring& strMsg);

		protected:

			CTextFileWriter	m_Writer;
			CTime::SSystemTime	m_Time;
	};

	class CEmptyLogger2 : public ILogger
	{
		public:

			i32		Init(xst_castring& strFileName) { return RESULT::OK; }
			i32		Write(xst_castring& strMsg) { return RESULT::OK; }
			i32		WriteError(xst_castring& strMsg) { return RESULT::OK; }
			i32		WriteWarning(xst_castring& strMsg) { return RESULT::OK; }
	};


#define XST_LOG1a( _lpszFormat, _args )	XST::CLogger::GetSingletonPtr()->GetLogger()->Write( _lpszFormat, _args )
#define XST_LOG1( _lpszMessage )		XST::CLogger::GetSingletonPtr()->GetLogger()->Write( _lpszMessage )
#define XST_LOG2a( _lpszFormat, _args )	XST::CLogger::GetSingletonPtr()->GetLogger()->Write( __FUNCTION__, __LINE__, _lpszFormat, _args )
#define XST_LOG2( _lpszMessage )		XST::CLogger::GetSingletonPtr()->GetLogger()->Write( __FUNCTION__, __LINE__, _lpszMessage )
#define XST_LOGERR( _lpszMessage )		XST::CLogger::GetSingletonPtr()->GetLogger()->Write( __FUNCTION__, __LINE__, "[ERROR]: %s", _lpszMessage );
#define XST_LOGERRa( _lpszMessage, _args ) XST::CLogger::GetSingletonPtr()->GetLogger()->Write( __FUNCTION__, __LINE__, "[ERROR]: %s %s", _lpszMessage, _args );

#define XST_LOGGER_STREAM	XST::CLogger::GetSingleton().GetStream()
#define XST_LOG_STREAM(_strStream) XST::CLogger::GetSingleton() << _strStream
#define XST_LOGGER_FLUSH(_uiMode) XST::CLogger::GetSingleton().FlushStream( (_uiMode) )
#define XST_LOGGER_FLUSH2()	XST::CLogger::GetSingleton().FlushStream()

#define XST_LOG_BASE_INFO(_strStream, _uiFlags) XST_LOG_STREAM( _strStream ); XST::CLogger::GetSingleton().FlushStream( ( _uiFlags ) )
#define XST_LOG_BASE_WARN(_strStream, _uiFlags) XST_LOG_STREAM( _strStream ); XST::CLogger::GetSingleton().FlushStreamWarning( ( _uiFlags ) )
#define XST_LOG_BASE_ERR(_strStream, _uiFlags) XST_LOG_STREAM( _strStream ); XST::CLogger::GetSingleton().FlushStreamError( ( _uiFlags ) )

//#define XST_LOG_FILE_CONSOLE(_strStream, _strType) XST_LOG_STREAM( _strStream ); XST::CLogger::GetSingleton().FlushStream( XST::CLogger::FILE | XST::CLogger::CONSOLE | XST::CLogger::LAST_ERROR )
//#define XST_LOG_FILE(_strStream) XST_LOG_STREAM( _strStream ); XST::CLogger::GetSingleton().FlushStream( XST::CLogger::FILE | XST::CLogger::LAST_ERROR )
//#define XST_LOG_CONSOLE(_strStream) XST_LOG_STREAM( _strStream ); XST::CLogger::GetSingleton().FlushStream( XST::CLogger::CONSOLE | XST::CLogger::LAST_ERROR )
//
//#define XST_LOG_FILE_CONSOLE_ERR(_strStream, _strType) XST_LOG_STREAM( _strStream ); XST::CLogger::GetSingleton().FlushStreamError( XST::CLogger::FILE | XST::CLogger::CONSOLE | XST::CLogger::LAST_ERROR )
//#define XST_LOG_FILE_ERR(_strStream) XST_LOG_STREAM( _strStream ); XST::CLogger::GetSingleton().FlushStreamError( XST::CLogger::FILE | XST::CLogger::LAST_ERROR )
//#define XST_LOG_CONSOLE_ERR(_strStream) XST_LOG_STREAM( _strStream ); XST::CLogger::GetSingleton().FlushStreamError( XST::CLogger::CONSOLE | XST::CLogger::LAST_ERROR )
//
//#define XST_LOG_FILE_CONSOLE_WRN(_strStream, _strType) XST_LOG_STREAM( _strStream ); XST::CLogger::GetSingleton().FlushStreamWarning( XST::CLogger::FILE | XST::CLogger::CONSOLE | XST::CLogger::LAST_ERROR )
//#define XST_LOG_FILE_WRN(_strStream) XST_LOG_STREAM( _strStream ); XST::CLogger::GetSingleton().FlushStreamWarning( XST::CLogger::FILE | XST::CLogger::LAST_ERROR )
//#define XST_LOG_CONSOLE_WRN(_strStream) XST_LOG_STREAM( _strStream ); XST::CLogger::GetSingleton().FlushStreamWarning( XST::CLogger::CONSOLE | XST::CLogger::LAST_ERROR )
//
//#define XST_LOG_LAST_ERROR(_strStream) XST_LOG_STREAM(_strStream); XST_LOGGER_FLUSH2()// ( XST::CLogger::LAST_ERROR )

#if defined (XST_ENABLE_FILE_LOGGER)
#	if	defined (XST_ENABLE_CONSOLE_LOGGER)
#		define XST_LOG(_strStream)		XST_LOG_BASE_INFO(	_strStream,	XST::CLogger::FILE | XST::CLogger::CONSOLE | XST::CLogger::LAST_ERROR )
#		define XST_LOG_ERR(_strStream)	XST_LOG_BASE_ERR(	_strStream,	XST::CLogger::FILE | XST::CLogger::CONSOLE | XST::CLogger::LAST_ERROR )
#		define XST_LOG_WRN(_strStream)	XST_LOG_BASE_WARN(	_strStream,	XST::CLogger::FILE | XST::CLogger::CONSOLE | XST::CLogger::LAST_ERROR )
#	else
#		define XST_LOG(_strStream)		XST_LOG_BASE_INFO(	_strStream,	XST::CLogger::FILE | XST::CLogger::LAST_ERROR )
#		define XST_LOG_ERR(_strStream)	XST_LOG_BASE_ERR(	_strStream,	XST::CLogger::FILE | XST::CLogger::LAST_ERROR )
#		define XST_LOG_WRN(_strStream)	XST_LOG_BASE_WARN(	_strStream,	XST::CLogger::FILE | XST::CLogger::LAST_ERROR )
#	endif
#elif defined (XST_ENABLE_CONSOLE_LOGGER)
#		define XST_LOG(_strStream)		XST_LOG_BASE_INFO(	_strStream, XST::CLogger::CONSOLE | XST::CLogger::LAST_ERROR )
#		define XST_LOG_ERR(_strStream)	XST_LOG_BASE_ERR(	_strStream,	XST::CLogger::CONSOLE | XST::CLogger::LAST_ERROR )
#		define XST_LOG_WRN(_strStream)	XST_LOG_BASE_WARN(	_strStream, XST::CLogger::CONSOLE | XST::CLogger::LAST_ERROR )
#else
#		define XST_LOG(_strStream)		XST_LOG_BASE_INFO(	_strStream,	XST::CLogger::LAST_ERROR )
#		define XST_LOG_ERR(_strStream)	XST_LOG_BASE_ERR(	_strStream,	XST::CLogger::LAST_ERROR )
#		define XST_LOG_WRN(_strStream)	XST_LOG_BASE_WARN(	_strStream,	XST::CLogger::LAST_ERROR )
#endif


}//xst

#endif
