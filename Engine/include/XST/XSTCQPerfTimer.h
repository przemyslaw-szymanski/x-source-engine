#ifndef XST_CQPERFTIMER_H
#define XST_CQPERFTIMER_H

#include "XSTCTime.h"

namespace XST
{

	class CQPerfTimer
	{
		public:

								CQPerfTimer() : m_ulStartTime(0), m_ulStopTime(0) 
								{

								}

			xst_i	ul32		Start()
			{
				m_ulStartTime = CTime::GetTickCount();
				return m_ulStartTime;
			}

			xst_i	ul32		Stop()
			{
				m_ulStopTime = CTime::GetTickCount();
				return m_ulStopTime;
			}

			xst_i	ul32		GetElapsedTime()
			{
				Stop();
				return (m_ulStopTime - m_ulStartTime);
			}

		private:

			ul32	m_ulStartTime;
			ul32	m_ulStopTime;

		#if defined (WIN32)

			//LARGE_INTEGER		m_StartTime;
			//LARGE_INTEGER		m_StopTime;

		#endif
	};

}//XST


#endif//XST_CQPERFTIMER_H