#ifndef XST_CSIMPLE_PROFILER_H
#define XST_CSIMPLE_PROFILER_H

#include <cstdio>
#include "XSTCTimer.h"
#include "XSTCString.h"

namespace XST
{
	class XST_API CSimpleProfiler
	{
		public:

							CSimpleProfiler(lpcastr _strName);

							CSimpleProfiler(lpcastr _strFunc, lpcastr _strTime);

							~CSimpleProfiler();

			xst_fi	f64		GetElapsedTime()
							{
								return m_Timer.GetQPerfElapsedTime();
							}


		private:

			CTimer		m_Timer;
			//xst_astring	m_strName;
			//xst_astring m_strTime;
			xst_astr128	m_strName;
			xst_astr16	m_strTime;
			u32			m_uiLine;
	};

#if defined (XST_DEBUG) || (XST_ENABLE_PROFILER)
	#define XSTSimpleProfiler()	XST::CSimpleProfiler _XSTSimpleProfiler(__FUNCTION__)
	#define XSTSimpleProfiler2(_name) XST::CSimpleProfiler _XSTSimpleProfiler( _name )
#else
	#define XSTSimpleProfiler()
	#define XSTSimpleProfiler2(_name)
#endif

}//XST




#endif
