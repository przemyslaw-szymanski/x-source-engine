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

    void on_ActionSolid_toggled(bool arg1);

    void on_ActionWireframe_toggled(bool arg1);

    void on_ActionWireframe_changed();

    void on_ActionWireframe_triggered(bool checked);

    void on_ActionSolid_triggered(bool checked);

private:
    Ui::CMainWindow *ui = 0;
    CRenderWidget* m_pRenderWidget = 0;
    CSampleMgr* m_pSampleMgr = 0;

};

#endif // CMAINWINDOW_H
