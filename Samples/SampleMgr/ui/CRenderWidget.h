#ifndef CRENDER_WIDGET_H
#define CRENDER_WIDGET_H

#include <QWidget>
#include <QLabel>

class CSampleMgr;
class CRenderInfo;
class CDebugDialog;

class CRenderWidget : public QWidget
{

	public:

						CRenderWidget(QWidget* pParent);
		virtual			~CRenderWidget() {}

		void			SetSampleMgr(CSampleMgr* pMgr)
						{ m_pSampleMgr = pMgr; }

		void			paintEvent(QPaintEvent *event);

		QPaintEngine*	paintEngine() const;

		void			resizeEvent(QResizeEvent *event);

		void			timerEvent(QTimerEvent*);

		void			keyPressEvent(QKeyEvent* pEvent);

		void			mouseMoveEvent(QMouseEvent* pEvent);

		void			mousePressEvent(QMouseEvent* pEvent);

		void			mouseReleaseEvent(QMouseEvent* pEvent);

		void			SetDebugDialog( CDebugDialog* pDlg )
						{ m_pDbgDlg = pDlg; }

	protected Q_SLOTS :



	protected:

		CSampleMgr*	m_pSampleMgr = 0;
		CDebugDialog* m_pDbgDlg = 0;
};

#endif