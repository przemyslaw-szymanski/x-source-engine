#ifndef XST_CTHREAD_POOL_H
#define XST_CTHREAD_POOL_H

#include "XSTIThread.h"
#include "XSTTCStack.h"

namespace XST
{
	class CThreadPool
	{
		friend class CThread;

		public:

			enum
			{
				MSG_NONE = 0,
				MSG_PAUSE,
				MSG_STOP
			};

			typedef fastdelegate::FastDelegate1< xst_unknown > Delegate;

			class CThread : public IThread
			{
				friend class CThreadPool;

				public:

									CThread(CThreadPool* pTP);
					virtual			~CThread();

							void	Run(xst_unknown pData);

							void	RunRealtime(xst_unknown pData);

							void	Stop();

							void	Pause();

							void	SendMessage(cu32& uiMsg);

							u32		GetMessage();

							u32		PopMessage();

					xst_fi
					CThreadPool::Delegate&	BindDoWork()
					{
						return m_DoWorkDelegate;
					}

				protected:

							i32		OnInit();

							i32		OnStart();

							void	OnKill();

				protected:

					xst_queue< u32 >	m_qMsgs;
					CThreadPool*		m_pThreadPool;
					xst_unknown			m_pUsrData;
					u32					m_uiCurrMsg;
					Delegate			m_DoWorkDelegate;
					bool				m_bRealtime;
			};

		protected:

			typedef xst_vector< IThread* >	ThreadVec;
			typedef xst_stack< IThread* >	ThreadStack;

		public:

								CThreadPool();
			virtual				~CThreadPool();

			virtual CThread*	AddThread();

			virtual void		RemoveThread(CThread* pThread);

			virtual void		DeleteThreads();

			virtual void		RemoveThreads();

		protected:

			ThreadVec		m_vThreads;
			ThreadStack		m_sFreeThreads;
	};


}//xst

#endif //XST_CTHREAD_POOL_H