#include "XST/XSTIThread.h"

namespace XST
{
	IThread::~IThread()
	{
		#if defined (XST_WINDOWS)
			CloseHandle( m_hThread );
		#else

		#endif
	}

	i32		IThread::Create(ul32 ulWaitForDeathTime)
	{
		m_uiThreadDying = 0;
		m_hThread = 0;
		m_dwThreadID = -1;
		m_CriticalSection = CCriticalSection();
		m_bIsThreadStarted = false;
		m_ulWaitForDeathTime = 2000;
		m_ulWaitForDeathTime = ulWaitForDeathTime;

		#if defined (XST_WINDOWS)
			m_hThread = CreateThread(0, 0, &ThreadEntry, this, CREATE_SUSPENDED, &m_dwThreadID);
			if( m_hThread == INVALID_HANDLE_VALUE )
				return RESULT::FAILED;
			return RESULT::OK;
		#else
			return RESULT::FAILED;
		#endif
	}

	void	IThread::Kill()
	{
		m_uiThreadDying++;
		m_bIsThreadStarted = false;
		OnKill();
		WaitForDeath();
	}

	void	IThread::Wait()
	{
		while( !this->IsDying() && !this->IsFinished() )
		{
			IThread::Sleep( 1 );
		}
	}

	void	IThread::Resume()
	{
		#if defined (XST_WINDOWS)
			ResumeThread( m_hThread );
		#else

		#endif

		m_bIsThreadStarted = true;
	}

	bool	IThread::IsStarted() const
			{ return m_bIsThreadStarted; }

	void	IThread::Start()
	{
		Resume();
	}

	bool	IThread::IsDying() const
	{ 
		return m_uiThreadDying != 0; 
	}

	void IThread::Sleep(cu32& uiMiliseconds)
	{
#if defined( XST_WINDOWS )
		::Sleep( uiMiliseconds );
#else
		usleep( uiMiliseconds * 1000 );
#endif
	}

	void	IThread::WaitForDeath()
	{
		#if defined (XST_WINDOWS)
			WaitForSingleObject( m_hThread, m_ulWaitForDeathTime );
		#else

		#endif
	}
}//xst