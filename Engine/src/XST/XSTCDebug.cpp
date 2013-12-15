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

    void	CDebug::PrintDebug(xst_castring& _strText)
    {
        #if defined(XST_WINDOWS)
            OutputDebugStringA(_strText.c_str());
        #endif
    }

    void	CDebug::PrintDebug(xst_cwstring& _strText)
    {
        #if defined(XST_WINDOWS)
            OutputDebugStringW(_strText.c_str());
        #endif
    }

    void	CDebug::PrintDebugLN(xst_castring& _strText)
    {
        #if defined(XST_WINDOWS)
            OutputDebugStringA(_strText.c_str());
            OutputDebugStringA("\n");
        #endif
    }

    void	CDebug::PrintDebugLN(xst_cwstring& _strText)
    {
        #if defined(XST_WINDOWS)
            OutputDebugStringW(_strText.c_str());
            OutputDebugStringW(L"\n");
        #endif
    }

}//xst
