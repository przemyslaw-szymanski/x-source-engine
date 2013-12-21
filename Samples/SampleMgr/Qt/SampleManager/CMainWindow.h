#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QTreeWidget>

namespace Ui {
class CMainWindow;
}

class CRenderWidget;
class CSampleMgr;

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CMainWindow(QWidget *parent = 0);
    ~CMainWindow();

	void Init(CSampleMgr* pMgr);

	void AddSample(const QString& strName, const QString& strGroup);

private slots:
    void on_SampleTree_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::CMainWindow *ui = nullptr;
	CRenderWidget* m_pRenderWidget = nullptr;
	CSampleMgr* m_pSampleMgr = nullptr;

};

#endif // CMAINWINDOW_H
