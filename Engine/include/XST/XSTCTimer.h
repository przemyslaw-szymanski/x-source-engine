#ifndef XST_CTIMER_H
#define XST_CTIMER_H

#include "XSTCTime.h"

namespace XST
{

	class XST_API CTimer
	{
		public:

								CTimer() : m_fFrequency(CTime::GetFrequency()), m_ulQPerfStartTime(0), m_ulQPerfStopTime(0) 
								{

								}

			xst_fi	ul32		StartQPerf()
			{
				m_ulQPerfStartTime = CTime::GetQPerfTickCount();
				return m_ulQPerfStartTime;
			}

			xst_fi	ul32		StopQPerf()
			{
				m_ulQPerfStopTime = CTime::GetQPerfTickCount();
				return m_ulQPerfStopTime;
			}

			xst_fi	f64		GetQPerfElapsedTime()
			{
				StopQPerf();
				return (f64)(m_ulQPerfStopTime - m_ulQPerfStartTime) * m_fFrequency;
			}

			xst_fi	ul32		Start()
			{
				m_ulStartTime = CTime::GetTickCount();
				return m_ulStartTime;
			}

			xst_fi	ul32		Stop()
			{
				m_ulStopTime = CTime::GetTickCount();
				return m_ulStopTime;
			}

			xst_fi	ul32		GetElapsedTime()
			{
				Stop();
				return (m_ulStopTime - m_ulStartTime);
			}

		private:

			ul32	m_ulQPerfStartTime;
			ul32	m_ulQPerfStopTime;
			ul32	m_ulStartTime;
			ul32	m_ulStopTime;
			f64		m_fFrequency;

		#if defined (WIN32)

			//LARGE_INTEGER		m_StartTime;
			//LARGE_INTEGER		m_StopTime;

		#endif
	};

}//XST


#endif//XST_CTIMER_H