#include "../include/XSourceEngine.h"

XST_API XSE::CEngine*	XSECreateEngine()
{
	return xst_new XSE::CEngine();
}

XST_API void XSEDestroyEngine(XSE::CEngine** ppEngine)
{
	(*ppEngine)->Release();
	(*ppEngine) = xst_null;
	XST::CDebug::DumpMemoryLeaks();
}

namespace XSE
{
}//xse