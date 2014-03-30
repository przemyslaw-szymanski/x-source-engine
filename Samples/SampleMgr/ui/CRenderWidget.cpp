#include "CRenderWidget.h"
#include "../CSampleMgr.h"
#include <QtGui\qevent.h>
#include "CRenderInfo.h"
#include "../Qt/SampleManager/CMainWindow.h"
#include "../Qt/SampleManager/CDebugDialog.h"
#include <QtWidgets/qlayout.h>

XSE::SMouseEvent g_Event;
XSE::CPoint g_lastMousePos;

CRenderWidget::CRenderWidget(QWidget* pParent) : QWidget( pParent )
{
	//setAttribute(Qt::WA_NoSystemBackground);
	//setAttribute( Qt::WA_NoBackground );
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_StyleSheet, false);
	setAttribute( Qt::WA_PaintOnScreen, true );
	startTimer( 0 );

	QVBoxLayout* pLayout = new QVBoxLayout( this );
	this->setLayout( pLayout );

	/*CRenderInfo& Info = CRenderInfo::GetSingleton();
	Info.m_pParent = this;

	Info.AddLabel( "CameraPos" );
	Info.AddLabel( "CameraAngles" );
	Info.AddLabel( "CameraDirection" );
	Info.AddLabel( "CameraRight" );*/
	//Info.AddLabel( "CameraSpeed" );
	//pLabel = new QLabel( this );
	//pLabel->setMinimumWidth( 100 );
	//pLabel->setStyleSheet( QString( "QLabel { background-color : red; color : blue; }" ) );

	g_lastMousePos = XSE::CPoint( 0, 0 );
}

void CRenderWidget::timerEvent(QTimerEvent* pEvent)
{
	update();
}

void CRenderWidget::paintEvent(QPaintEvent *pEvent)
{
	//QWidget::paintEvent( pEvent );

	if( m_pSampleMgr != xst_null && m_pSampleMgr->GetCurrentSample() != NULL && m_pSampleMgr->GetCurrentSample()->IsReady() )
	{
		m_pSampleMgr->RenderSample();
		if( m_pSampleMgr->GetEngine()->GetSceneManager() != xst_null )
		{
			XSE::CCamera* pCam = m_pSampleMgr->GetEngine()->GetSceneManager()->GetCurrentCamera();
			m_pDbgDlg->SetDrawCallCount( m_pSampleMgr->GetEngine()->GetRenderSystem()->GetDiagnostics().ulDrawCallCount );
			m_pDbgDlg->SetIndexedDrawCallCount( m_pSampleMgr->GetEngine()->GetRenderSystem()->GetDiagnostics().ulIndexedDrawCallCount );
            m_pDbgDlg->SetFrameTime( m_pSampleMgr->GetEngine()->GetRenderWindow( 0 )->GetFrameTime() );
		}
	}
	
}

void CRenderWidget::keyPressEvent(QKeyEvent* pEvent)
{
	if( m_pSampleMgr && m_pSampleMgr->GetCurrentSample() )
	{
		m_pSampleMgr->GetCurrentSample()->OnKeyPressEvent( pEvent->key() );
	}
	pEvent->accept();	
}

void CRenderWidget::mouseMoveEvent(QMouseEvent* pEvent)
{
	if( m_pSampleMgr && m_pSampleMgr->GetCurrentSample() )
	{
		g_Event.Position = XSE::CPoint( pEvent->pos().x(), pEvent->pos().y() );
		g_Event.Delta = g_lastMousePos - g_Event.Position;
		g_lastMousePos = g_Event.Position;
		m_pSampleMgr->GetCurrentSample()->OnMouseMove( g_Event );
	}
	pEvent->accept();
}

void CRenderWidget::mousePressEvent(QMouseEvent* pEvent)
{
	g_lastMousePos = XSE::CPoint( pEvent->pos().x(), pEvent->pos().y() );
	g_Event.Position = g_lastMousePos;
	pEvent->accept();
}

void CRenderWidget::mouseReleaseEvent(QMouseEvent* pEvent)
{
	pEvent->accept();
}

QPaintEngine* CRenderWidget::paintEngine() const
{
	return 0;
}

void CRenderWidget::resizeEvent(QResizeEvent *event)
{
	if( m_pSampleMgr != xst_null /*&& m_pSampleMgr->GetCurrentSample()*/ )
	{
		assert(m_pSampleMgr->GetRenderWindow());
		m_pSampleMgr->GetRenderWindow()->SetSize( event->size().width(), event->size().height() );
	}
}