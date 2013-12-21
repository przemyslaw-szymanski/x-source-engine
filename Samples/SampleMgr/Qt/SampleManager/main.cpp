#include "CMainWindow.h"
#include <QApplication>
#include "CSampleMgr.h"

CSampleMgr g_SampleMgr;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CMainWindow w;
	w.Init(&g_SampleMgr);
	w.show();

    return a.exec();
}
