#ifndef CMAIN_WINDOW_H
#define CMAIN_WINDOW_H

#include <QtGui/QWindow>
#include <QtWidgets/QPushButton.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qlistview.h>
#include "main.ui.h"
#include "CSampleToolBox.h"

class CSampleMgr;
class CRenderWidget;

class CMainWindow : public QWindow
{
	Q_OBJECT

		typedef QListWidget								GroupWidget;
		typedef std::map< std::string, GroupWidget* >	GroupMap;

	public:

		CMainWindow(QWidget* pParent = NULL);
		~CMainWindow();

		bool			Init(CSampleMgr* pMgr);

		void			AddSample(const QString& strName, const QString& strGroup);

		GroupWidget*	GetTab(const std::string& strName);

		void			keyPressEvent(QKeyEvent* pEvent);

	private Q_SLOTS:

		void			OnSampleListCurrentChanged(int id);
		void			OnSampleListClick() {}
		void			OnItemDoubleClicked(QListWidgetItem* pItem);
		void			OnMenuSolidActionChanged(bool);
		void			OnMenuWireframeActionChanged(bool);
		void			OnMenuCameraResetPositionActionClicked(bool);

	private:

		Ui::CMainWindow	m_MainWndUi;
		GroupMap		m_mGroups;
		CSampleToolBox*	m_pToolBox;
		CSampleMgr*		m_pSampleMgr;
		CRenderWidget*	m_pRenderWidget;
};

#endif