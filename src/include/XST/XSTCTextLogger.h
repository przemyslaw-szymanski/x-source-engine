#ifndef XST_TEXT_LOGGER_H
#define XST_TEXT_LOGGER_H

#include "XSTILogger.h"
#include "XSTCTime.h"

namespace XST
{
/*
	class CTextLogger : public ILogger
	{
		public:

							CTextLogger()
							{
							}

							~CTextLogger()
							{
								m_Writer.Close();
							}

			virtual i32		Init(xst_castring& _strFileName)
			{
				return m_Writer.Open( _strFileName );
			}

			virtual i32		Write(xst_castring& _strMessage)
			{
				m_iResult = m_Writer.WriteLine( _strMessage );
				m_Writer.Flush();
				return m_iResult;
			}

			virtual i32	Write(lpcastr _lpszFormat, ...)
			{
				va_list vlArgs;
				va_start( vlArgs, _lpszFormat );
				ch8 achBuff[1024];
				vsprintf_s(achBuff, 1024, _lpszFormat, vlArgs); 
				va_end( vlArgs );

				m_iResult = m_Writer.WriteLine( achBuff );
				m_Writer.Flush();
				return m_iResult;
			}

			virtual i32	Write(lpcastr _lpszFunction, const i32& _iLine, lpcastr _lpszFormat, ...)
			{
				va_list vlArgs;
				va_start( vlArgs, _lpszFormat );
				ch8 achBuff[1024];
				vsprintf_s(achBuff, 1024, _lpszFormat, vlArgs); 
				va_end( vlArgs );

				XST::CTime::SSystemTime SysTime;
				XST::CTime::GetSystemTime( &SysTime );

				std::stringstream ss;
				ss<< "<" << SysTime.usHour << ":" << SysTime.usMinute << ":" << SysTime.usSecond << ">(" << _lpszFunction << ")[" << _iLine << "] : " << achBuff; 

				m_iResult = m_Writer.WriteLine( ss.str() );
				m_Writer.Flush();
				return m_iResult;
			}

		private:

			i32 m_iResult;
	};
*/

}//XST

#endif