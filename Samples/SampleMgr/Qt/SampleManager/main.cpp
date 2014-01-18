#include "CMainWindow.h"
#include <QApplication>
#if defined(_MSC_VER)
#include "CSampleMgr.h"

CSampleMgr g_SampleMgr;
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CMainWindow w;
#if defined(_MSC_VER)
	w.Init(&g_SampleMgr);
#endif
	w.show();
	//w.showMaximized();

    return a.exec();
}
