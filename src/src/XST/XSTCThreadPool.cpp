#include "XSt/XSTCThreadPool.h"

namespace XST
{
	CThreadPool::CThreadPool()
	{
		m_vThreads.reserve( 100 );
	}

	CThreadPool::~CThreadPool()
	{
		DeleteThreads();
	}

	void CThreadPool::DeleteThreads()
	{
		ThreadVec::iterator Itr;
		xst_stl_foreach( Itr, m_vThreads )
		{
			xst_delete( (*Itr) );
		}

		xst_vector_clear( m_vThreads, IThread* );

		for(u32 i = 0; i < m_sFreeThreads.size(); ++i)
		{
			IThread* pThread = m_sFreeThreads[ i ];
			xst_delete( pThread );
		}

		m_sFreeThreads.clear();
	}

	void CThreadPool::RemoveThreads()
	{
		ThreadVec::iterator Itr;
		xst_stl_foreach( Itr, m_vThreads )
		{
			m_sFreeThreads.push( (*Itr) );
		}

		xst_vector_clear( m_vThreads, IThread* );
	}

	template<class _T_>
xst_fi i32 VectorErase(xst_vector< _T_ >* pVec, const _T_& tValue)
{
	xst_vector< _T_ >::iterator Itr = xst_find( pVec->begin(), pVec->end(), tValue );
	if( Itr == pVec->end() )
		return XST_FAIL;

	(*Itr) = pVec->back();
	pVec->pop_back();

	return XST_OK;
}

	void CThreadPool::RemoveThread(CThread* pThread)
	{
		IThread* pTh = (IThread*)pThread;
		if( !XST_FAILED( VectorErase( &m_vThreads, pTh ) ) )
		{
			pThread->Kill();
			m_sFreeThreads.push( pTh );
		}
	}

	CThreadPool::CThread* CThreadPool::AddThread()
	{
		IThread* pThread;

		if( m_sFreeThreads.empty() )
		{
			pThread = xst_new CThread( this );
		}
		else
		{
			pThread = m_sFreeThreads.top_pop();
		}

		pThread->Create();
	
		m_vThreads.push_back( pThread );
		return (CThread*)pThread;
	}


	//////////////////////////////////////////////////////////////////////////////

	CThreadPool::CThread::CThread(CThreadPool* pTP) :
		m_pUsrData( xst_null ),
		m_bRealtime( false ),
		m_uiCurrMsg( 0 ),
		m_pThreadPool( pTP )
	{

	}

	CThreadPool::CThread::~CThread()
	{
		m_bRealtime = false;
		m_uiCurrMsg = 0 ;
		m_pUsrData = xst_null;
	}

	void CThreadPool::CThread::Run(xst_unknown pData)
	{
		m_pUsrData = pData;
		m_bRealtime = false;
		this->Start();
	}

	void CThreadPool::CThread::RunRealtime(xst_unknown pData)
	{
		m_pUsrData = pData;
		m_bRealtime = true;
		this->Start();
	}

	void CThreadPool::CThread::Stop()
	{
		SendMessage( CThreadPool::MSG_STOP );
	}

	void CThreadPool::CThread::Pause()
	{
		SendMessage( CThreadPool::MSG_PAUSE );
	}

	void CThreadPool::CThread::SendMessage(cu32& uiMsg)
	{
		m_qMsgs.push( uiMsg );
	}

	u32 CThreadPool::CThread::GetMessage()
	{
		return m_uiCurrMsg;
	}

	u32 CThreadPool::CThread::PopMessage()
	{
		m_uiCurrMsg = m_qMsgs.front();
		m_qMsgs.pop();
		return m_uiCurrMsg;
	}

	i32 CThreadPool::CThread::OnStart()
	{
		if( !m_bRealtime )
		{
			if( m_DoWorkDelegate )
				m_DoWorkDelegate( m_pUsrData );
			return XST_OK;
		}

		u32 uiMsg = 0;

		while( ( uiMsg = PopMessage() ) != CThreadPool::MSG_STOP )
		{
			IThread::Sleep( 1 );

			switch( uiMsg )
			{
				case CThreadPool::MSG_PAUSE:
				{
					continue;
				}
				break;
			}

			if( m_DoWorkDelegate )
				m_DoWorkDelegate( m_pUsrData );
		}

		return XST_OK;
	}

	i32 CThreadPool::CThread::OnInit()
	{
		return XST_OK;
	}

	void CThreadPool::CThread::OnKill()
	{

	}

}//xst