#include "../../include/XST/XSTCCriticalSection.h"

namespace XST
{

    CCriticalSection2::CCriticalSection2() : m_lLockCount(0), m_lThreadID(0)
    {
    }

    void	CCriticalSection2::Enter()
    {
        #if defined (XST_WINDOWS)
            l32 lThreadID = ::GetCurrentThreadId();
            if(m_lThreadID == lThreadID)
            {
                ++m_lLockCount;
            }
            else
            {
                while(true)
                {
                    if(SetLock(lThreadID))
                        break;

                    ::Sleep(1);
                }
            }
        #else

        #endif

    }

    void	CCriticalSection2::Leave()
    {
        #if defined (XST_WINDOWS)
            l32 lThreadID = ::GetCurrentThreadId();
            if(m_lThreadID == lThreadID)
            {
                if(m_lLockCount > 1)
                {
                    --m_lLockCount;
                }
                else
                {
                    m_lLockCount = 0;
                    ::InterlockedExchange(&m_lThreadID, 0);
                }
            }
        #else

        #endif
    }

    bool	CCriticalSection2::Try()
    {
        #if defined (XST_WINDOWS)
            l32 lThreadID = ::GetCurrentThreadId();
            if(m_lThreadID == lThreadID)
            {
                ++m_lLockCount;
                return true;
            }

            return SetLock(lThreadID);
        #else

        #endif
    }

    bool	CCriticalSection2::SetLock(const l32 _lThreadID)
    {
        #if defined (XST_WINDOWS)
            if(::InterlockedCompareExchange((volatile long*)&m_lThreadID, _lThreadID, 0) == 0)
            {
                m_lLockCount = 1;
                return true;
            }
        #else

        #endif
        return false;
    }

	CCriticalSection::CCriticalSection()
	{
		#if defined (XST_WINDOWS)
			::InitializeCriticalSectionAndSpinCount( &m_CritSection, 0x00000400 );
		#else

		#endif
	}

	CCriticalSection::~CCriticalSection()
	{
		#if defined (XST_WINDOWS)
			::DeleteCriticalSection( &m_CritSection );
		#else

		#endif
	}

	void CCriticalSection::Enter()
	{
		#if defined (XST_WINDOWS)
			::EnterCriticalSection( &m_CritSection );
		#else

		#endif
	}

	void CCriticalSection::Leave()
	{
		#if defined (XST_WINDOWS)
			::LeaveCriticalSection( &m_CritSection );
		#else

		#endif
	}

}//XST
