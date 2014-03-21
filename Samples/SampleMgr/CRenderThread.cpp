#include "CRenderThread.h"
#include "XSEIRenderWindow.h"
#include "ISample.h"

XSE::IRenderWindow* g_pWnd;

CRenderThread::CRenderThread()
{
	m_pMutex = xst_new QMutex();
}

CRenderThread::~CRenderThread()
{
	xst_delete( m_pMutex );
}

void CRenderThread::Init(XSE::CEngine* pEngine)
{
	m_pEngine = pEngine;
	g_pWnd = m_pEngine->GetRenderWindow( 0 );
}

void CRenderThread::SetSample( ISample* pSample )
{
	m_pSample = pSample;
}

void CRenderThread::StartRendering(bool bStart)
{
	m_bRunning = bStart;
}

void CRenderThread::Lock()
{
	m_pMutex->lock();
}

void CRenderThread::Unlock()
{
	m_pMutex->unlock();
}

void CRenderThread::run()
{
	while( true )
	{
		while( m_bRunning && m_pSample )
		{
			Lock();
			m_pSample->Update();
			g_pWnd->BeginRenderFrame();
			g_pWnd->EndRenderFrame();
			Unlock();
		}

		Sleep( 1 );
	}
}