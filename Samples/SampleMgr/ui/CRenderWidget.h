#ifndef CRENDER_WIDGET_H
#define CRENDER_WIDGET_H

#include <QWidget>
#include <QLabel>

class CSampleMgr;
class CRenderInfo;

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

	protected Q_SLOTS :



	protected:

		CSampleMgr*	m_pSampleMgr = 0;
};

#endif