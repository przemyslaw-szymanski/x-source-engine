
#include "CSampleMgr.h"
#include "ui/CMainWindow.h"

CSampleMgr g_SampleMgr;

//int xse_main()
int main(int argc, char** argv)
{
	//Get params
	//printf( "start\n" );

	QApplication Application( argc, argv );
	CMainWindow MainWnd;
	
	MainWnd.Init( &g_SampleMgr );
	MainWnd.show();
	Application.exec();

	g_SampleMgr.UnregisterSamples();
	g_SampleMgr.DestroyEngine();

	return 0;
}