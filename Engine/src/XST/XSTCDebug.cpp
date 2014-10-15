#include "../../include/XST/XSTCDebug.h"

namespace XST
{

    void CDebug::DumpMemoryLeaks()
    {
        #if defined(_DEBUG)
        #	if defined(XST_VISUAL_STUDIO)
                //_CrtMemDumpAllObjectsSince(0);
                //_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF);
                _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF);
                _CrtSetReportMode(_CRT_ASSERT,_CRTDBG_MODE_FILE);
                _CrtSetReportFile(_CRT_ASSERT,_CRTDBG_FILE_STDERR);
        #	else
        #	endif//XST_VISUAL_STUDIO
        #endif//_DEBUG
    }

    void	CDebug::PrintDebug(lpcastr _strText)
    {
        #if defined(XST_WINDOWS)
            OutputDebugStringA(_strText);
        #endif
    }

    void	CDebug::PrintDebug(lpcwstr _strText)
    {
        #if defined(XST_WINDOWS)
            OutputDebugStringW(_strText);
        #endif
    }

    void	CDebug::PrintDebugLN(lpcastr _strText)
    {
        #if defined(XST_WINDOWS)
            OutputDebugStringA(_strText);
            OutputDebugStringA("\n");
        #endif
    }

    void	CDebug::PrintDebugLN(lpcwstr _strText)
    {
        #if defined(XST_WINDOWS)
            OutputDebugStringW(_strText);
            OutputDebugStringW(L"\n");
        #endif
    }

}//xst
