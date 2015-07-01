#ifndef XST_THREAD_H
#define XST_THREAD_H

#include "XSTCommonInclude.h"
#include "XSTCommonTypes.h"
#include "XSTCLock.h"


namespace XST
{
	using namespace XST::Types;

	class IThread
	{
		public:

							IThread() : 
								m_uiThreadDying(0), m_hThread(0), m_dwThreadID(-1), 
								m_CriticalSection( CCriticalSection() ), m_bIsThreadStarted(false),
								m_ulWaitForDeathTime( 2000 ), m_bFinished( false )
							{
							}

			virtual			~IThread();

					i32		Create(ul32 ulWaitForDeathTime = 2000);

					void	Kill();

					void	Resume();

			virtual	void	Wait();

			virtual
			xst_fi	bool	IsFinished() const
							{ return m_bFinished; }

			static	void	Sleep(cu32& uiMiliseconds);

					bool	IsStarted() const;
							//{ return m_bIsThreadStarted; }

					void	Start();

			xst_fi	bool	IsDying() const;

			XST_DECLV(i32,	OnInit() { return RESULT::OK; } );
			XST_DECLV(i32,	OnStart() { return RESULT::OK; } );
			XST_DECLV(void, OnKill() {} );

		private:

			#if defined(XST_WINDOWS)
			static DWORD WINAPI	ThreadEntry(xst_unknown _pArgs)
			{
				IThread* pThread = (IThread*)_pArgs;

				if( pThread->OnInit() != RESULT::OK )
					return RESULT::FAILED;

				i32 iResult = pThread->OnStart();
				pThread->m_bFinished = true;
				return iResult;
			}
			#endif //XST_WINDOWS

			xst_fi	void	WaitForDeath();

		
		protected:

			u32		m_uiThreadDying;
			ul32	m_ulWaitForDeathTime;
			dword	m_dwThreadID;
			bool	m_bIsThreadStarted;
			bool	m_bFinished;
            u8      m_padding[ 2 ];

			XST::CCriticalSection m_CriticalSection;

		#if defined(XST_WINDOWS)
			HANDLE	m_hThread;
		#else
			xst_unknown m_hThread;
		#endif
	};

	class CThreadEvent
	{
		public:

							CThreadEvent()
							{
								#if defined (XST_WINDOWS)
									m_hEvent = CreateEvent( 0, false, false, 0 );
								#else

								#endif
							}

							~CThreadEvent()
							{
								#if defined (XST_WINDOWS)
									CloseHandle( m_hEvent );
								#else

								#endif
							}

					void	Release()
					{
						#if defined (XST_WINDOWS)
							SetEvent( m_hEvent );
						#else

						#endif
					}

					void	Wait()
					{
						#if defined (XST_WINDOWS)
							WaitForSingleObject( m_hEvent, INFINITE );
						#else

						#endif
					}

		private:

		#if defined (XST_WINDOWS)
			HANDLE	m_hEvent;
		#else

		#endif
	};

}//xst

#endif