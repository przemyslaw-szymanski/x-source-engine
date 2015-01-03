#ifndef CRENDER_THREAD_H
#define CRENDER_THREAD_H

#include <QtCore\qthread.h>
#include <XSECEngine.h>

class ISample;

class CRenderThread : public QThread
{
	Q_OBJECT

	public:

	enum class MESSAGE
	{
		NONE,
		SET_RENDER_ONE_FRAME,
		SET_RENDER_TYPE_WIREFRAME,
		SET_RENDER_TYPE_SOLID,
	};

	public:

	CRenderThread();
	virtual ~CRenderThread();

		void	Init(XSE::CEngine* pEngine);

		void	SetSample(ISample* pSample);

		void	StartRendering(bool bStart);

		void	Lock();

		void	Unlock();

		void	SendMessage(MESSAGE uMsg);

	private:

		void	ProcessMessages();

		void	run();

	private:

		QMutex*			m_pMutex;
		std::stack<MESSAGE> m_sMessages;
		XSE::CEngine*	m_pEngine = xst_null;
		ISample*		m_pSample = xst_null;
		bool			m_bRunning = false;
};

#endif // CRENDER_THREAD_H