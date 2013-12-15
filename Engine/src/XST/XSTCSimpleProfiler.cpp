#include "XST/XSTCSimpleProfiler.h"
#include "XST/XSTCConsole.h"
#include "XST/XSTCProfilerLogger.h"

namespace XST
{
	CSimpleProfiler::CSimpleProfiler(lpcastr _strName)
	{
		//xst_assert(m_pFile != 0, "Profiler log file not created!");
		m_strName = _strName;
		//m_ulStartTime = CTime::GetTickCount();
		m_Timer.StartQPerf();
	}

	CSimpleProfiler::CSimpleProfiler(lpcastr _strFunc, lpcastr _strTime)
	{
		//xst_assert(m_pFile != 0, "Profiler log file not created!");
		m_strName = _strFunc;
		m_strTime = _strTime;
		//m_uiLine = _uiLine;
		//m_ulStartTime = CTime::GetTickCount();
		m_Timer.StartQPerf();
	}

	CSimpleProfiler::~CSimpleProfiler()
	{
		f64 fElapsedTime = m_Timer.GetQPerfElapsedTime();
		xst_astr128 strOut = m_strName + ":\t\t\t";
		//m_strName + ":\t\t\t\t";
		if( XST::CProfilerLogger::pFile )
		{
			strOut += StringUtil::ToString(fElapsedTime, 16);
			XST::CProfilerLogger::Log(strOut.data());//, __FILE__, __LINE__);
		}
		else
		{
			strOut += StringUtil::ToString(fElapsedTime, 16);
			CConsole::WriteLN(strOut.data());
		}
	}

}//xst