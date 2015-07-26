#ifndef XST_PLATFORM_H
#define XST_PLATFORM_H

#include "XSTCommon.h"
#if defined(XST_WINDOWS)
#   if defined( XST_VISUAL_STUDIO )
#	    include <intrin.h>
#   endif //XST_VISUAL_STUDIO
#	define XST_EDX_MMX_bit      0x800000   // 23 bit
#	define XST_EDX_SSE_bit      0x2000000  // 25 bit
#	define XST_EDX_SSE2_bit     0x4000000  // 26 bit
#	define XST_EDX_3DnowExt_bit 0x40000000 // 30 bit
#	define XST_EDX_3Dnow_bit	0x80000000 // 31 bit
#	define XST_EDX_MMXplus_bit	0x400000 // 22 bit

#	define XST_ECX_SSE3_bit     0x1        // 0 bit
#	define XST_ECX_SSSE3_bit    0x200      // 9 bit
#	define XST_ECX_SSE41_bit    0x80000    // 19 bit
#	define XST_ECX_SSE42_bit    0x100000   // 20 bit

#	define XST_ECX_SSE4A_bit    0x40    // 6 bit
#	define XST_ECX_SSE5_bit		0x800    // 11 bit

#	include <Psapi.h>
//#	pragma comment(lib, "Psapi.lib")
/*
    #if PSAPI_VERSION == 1
        #pragma comment(lib, "Psapi.lib")
    #else //if not PSAPI_VERSION == 1
        #if defined(PSAPI_VERSION)
            #pragma comment(lib, "Psapi.lib")
        #endif//defined(PSAPI_VERSION)
    #endif
    */
#endif

#if defined(XST_VISUAL_STUDIO)
#	define xst_cpuid(_aCPUInfo, _infoType) __cpuid(_aCPUInfo, _infoType)

#else //if GCC - not tested
#	define xst_cpuid(_aCPUInfo, _infoType) \
            __asm__ __volatile__ ("cpuid":\
            "=a" (_aCPUInfo[0]), "=b" (_aCPUInfo[1]), "=c" (_aCPUInfo[2]), "=d" (_aCPUInfo[3]) : "a" (_infoType));

#endif

#pragma warning(disable: 4800)// warning C4800: 'BOOL' : forcing value to bool 'true' or 'false' (performance warning)

namespace XST
{
    using namespace XST::Types;

    struct XST_API SCPUInfo
    {
        enum
        {
            CPU_INTEL = 1,
            CPU_AMD
        };



        bool bFeatureMMX;
        bool bFeatureMMXPlus;
        bool bFeatureSSE;
        bool bFeatureSSE2;
        bool bFeatureSSE3;
        bool bFeatureSSSE3;
        bool bFeatureSSE41;
        bool bFeatureSSE42;
        bool bFeatureSSE4a;
        bool bFeatureSSE5;
        bool bFeature3DNow;
        bool bFeature3DNowExt;
        bool bFeatureCMPXCHG;
        bool bFeatureCMPXCHG16b;
        bool bFeatureCMP8XCHG16;
    };

    struct XST_API ProcessorArchitecture
    {
        enum
        {
            UNKNOWN = 0,
            X86,
            X64
        };
    };

    struct XST_API ProcessorType
    {
        enum
        {
            UNKNOWN = 0,
            I386,
            I486,
            I586,
            IA64,
            X8664
        };
    };

    struct XST_API SystemType
    {
        enum
        {
            UNKNOWN = 0,
            WINDOWS_2000,
            WINDOWS_XP,
            WINDOWS_XP_64,
            WINDOWS_SERVER_2003,
            WINDOWS_SERVER_2003_R2,
            WINDOWS_VISTA,
            WINDOWS_SERVER_2008,
            WINDOWS_SERVER_2008_R2,
            WINDOWS_7
        };
    };

    struct XST_API SPlatformInfo
    {
        u32		uiProcessorArchitecture;
        u32		uiProcessorType;
        u32		uiProcessorCount;
        u32		uiSystemType;
    };

    class XST_API Platform
    {
        public:

        #if defined(XST_WINDOWS)

            inline static void	MsgBoxInfo(ul32 _hWnd, lpcastr _lpszTitle, lpcastr _lpszText)
            {
                MessageBoxA( (HWND)_hWnd, _lpszText, _lpszTitle, MB_OK | MB_ICONINFORMATION);
            }

            inline static void	MsgBoxInfo(ul32 _hWnd, lpcwstr _lpszTitle, lpcwstr _lpszText)
            {
                MessageBoxW( (HWND)_hWnd, _lpszText, _lpszTitle, MB_OK | MB_ICONINFORMATION);
            }

            inline static void	MsgBoxError(ul32 _hWnd, lpcastr _lpszTitle, lpcastr _lpszText)
            {
                MessageBoxA( (HWND)_hWnd, _lpszText, _lpszTitle, MB_OK | MB_ICONERROR);
            }

            inline static void	MsgBoxError(ul32 _hWnd, lpcwstr _lpszTitle, lpcwstr _lpszText)
            {
                MessageBoxW( (HWND)_hWnd, _lpszText, _lpszTitle, MB_OK | MB_ICONERROR);
            }

            inline static void	MsgBoxWarning(ul32 _hWnd, lpcastr _lpszTitle, lpcastr _lpszText)
            {
                MessageBoxA( (HWND)_hWnd, _lpszText, _lpszTitle, MB_OK  | MB_ICONWARNING);
            }

            inline static void	MsgBoxWarning(ul32 _hWnd, lpcwstr _lpszTitle, lpcwstr _lpszText)
            {
                MessageBoxW( (HWND)_hWnd, _lpszText, _lpszTitle, MB_OK | MB_ICONWARNING);
            }

            inline static i32 MsgBoxConfirm(ul32 _hWnd, lpcastr _lpszTitle, lpcastr _lpszText)
            {
                return MessageBoxA( (HWND)_hWnd, _lpszText, _lpszTitle, MB_YESNO | MB_ICONQUESTION);
            }

            inline static i32 MsgBoxConfirm(ul32 _hWnd, lpcwstr _lpszTitle, lpcwstr _lpszText)
            {
                return MessageBoxW( (HWND)_hWnd, _lpszText, _lpszTitle, MB_YESNO | MB_ICONQUESTION);
            }

            inline static void AssertBox(lpcastr lpszExpression, lpcastr lpszFile, u32 iLine, lpcastr lpszFunction, ul32 hWnd = 0)
            {
                static ch8 strBuff[ 1024 ];
                #if defined (XST_VISUAL_STUDIO)
                    sprintf_s( strBuff, 1024, "file: %s\nline: %d\nfunc: %s\nreason: %s", lpszFile, iLine, lpszFunction, lpszExpression );
                #else
                    sprintf( strBuff, "file: %s\nline: %d\nfunc: %s\nreason: %s", lpszFile, iLine, lpszFunction, lpszExpression );
                #endif
                if( MessageBoxA( (HWND)hWnd, strBuff, "Error", MB_ICONERROR | MB_TOPMOST | MB_CANCELTRYCONTINUE ) == IDABORT )
                {
                    exit( XST_FAIL );
                }
            }

            static u32 GetMemoryUsage()
            {
                ::PROCESS_MEMORY_COUNTERS ProcCounters;
                xst_zero(&ProcCounters, sizeof(::PROCESS_MEMORY_COUNTERS));
                ProcCounters.cb = sizeof(ProcCounters);

                HANDLE hProc = ::GetCurrentProcess();
                ::GetProcessMemoryInfo(hProc, &ProcCounters, ProcCounters.cb);
                return (u32)ProcCounters.WorkingSetSize;
            }

            static i32 GetCPUInfo(SCPUInfo* _pOutInfo)
            {
                xst_zero(_pOutInfo, sizeof(SCPUInfo));

                //_pOutInfo->bFeature3DNOW		= (bool)::IsProcessorFeaturePresent(PF_3DNOW_INSTRUCTIONS_AVAILABLE);
                _pOutInfo->bFeatureCMPXCHG		= (bool)::IsProcessorFeaturePresent( PF_COMPARE_EXCHANGE_DOUBLE );
                #if defined (PF_COMPARE_EXCHANGE128)
                _pOutInfo->bFeatureCMPXCHG16b	= (bool)::IsProcessorFeaturePresent( PF_COMPARE_EXCHANGE128 );
                #endif
                #if defined (PF_COMPARE64_EXCHANGE128)
                _pOutInfo->bFeatureCMP8XCHG16	= (bool)::IsProcessorFeaturePresent( PF_COMPARE64_EXCHANGE128 );
                #endif
                /*_pOutInfo->bFeatureMMX			= (bool)::IsProcessorFeaturePresent(PF_MMX_INSTRUCTIONS_AVAILABLE);
                _pOutInfo->bFeatureSSE3			= (bool)::IsProcessorFeaturePresent(PF_SSE3_INSTRUCTIONS_AVAILABLE);
                _pOutInfo->bFeatureSSE			= (bool)::IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE);
                _pOutInfo->bFeatureSSE2			= (bool)::IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE);*/


                i32 aiCPUInfo[4];
                i32 aiCPUInfoExt[4];
                DWORD dwECX = 0;
                DWORD dwEDX = 0;

                xst_cpuid(aiCPUInfo, 0);

                if( aiCPUInfo[0] >= 1 )
                {
                    xst_cpuid(aiCPUInfo, 1);
                    dwECX = (u32)aiCPUInfo[2];
                    dwEDX = (u32)aiCPUInfo[3];
                }

                xst_cpuid(aiCPUInfo, 0x80000000);

                if( aiCPUInfo[0] >= 0x80000001 )
                    xst_cpuid(aiCPUInfoExt, 0x80000001);

                if( XST_EDX_MMX_bit & dwEDX )
                    _pOutInfo->bFeatureMMX = true;

                if( XST_EDX_SSE_bit & dwEDX )
                    _pOutInfo->bFeatureSSE = true;

                if( XST_EDX_SSE2_bit & dwEDX )
                    _pOutInfo->bFeatureSSE2 = true;

                if( XST_ECX_SSE3_bit & dwECX )
                    _pOutInfo->bFeatureSSE3 = true;

                if( XST_ECX_SSSE3_bit & dwECX )
                    _pOutInfo->bFeatureSSSE3 = true;

                if( XST_ECX_SSE41_bit & dwECX )
                    _pOutInfo->bFeatureSSE41 = true;

                if( XST_ECX_SSE42_bit & dwECX )
                    _pOutInfo->bFeatureSSE42 = true;

                if( XST_EDX_3DnowExt_bit & aiCPUInfoExt[3] )
                    _pOutInfo->bFeature3DNowExt = true;

                if( XST_EDX_3Dnow_bit & aiCPUInfoExt[3] )
                    _pOutInfo->bFeature3DNow = true;

                if( XST_EDX_MMXplus_bit & aiCPUInfoExt[3] )
                    _pOutInfo->bFeatureMMXPlus = true;

                if( XST_ECX_SSE4A_bit & aiCPUInfoExt[2] )
                    _pOutInfo->bFeatureSSE4a = true;

                if( XST_ECX_SSE5_bit & aiCPUInfoExt[2] )
                    _pOutInfo->bFeatureSSE5 = true;

                return RESULT::OK;
            }

            static i32	GetPlatformInfo(SPlatformInfo* _pOutInfo);

            static u32 GetCurrentWindowHandle()
            {
                return 0;
            }

            static xst_fi xst_unknown GetCurrentWindowProcess()
            {
                return ::GetCurrentProcess();
            }

            static xst_fi xst_unknown LoadLibrary(lpcastr lpszLibraryName)
            {
                HMODULE hLib = ::LoadLibraryA( lpszLibraryName );
                return hLib;
            }

            static xst_fi i32 CloseLibrary(xst_unknown hLibrary)
            {
                return ::FreeLibrary( (HMODULE)hLibrary );
            }

            static xst_fi xst_unknown GetProcAddress(xst_unknown hLibrary, lpcastr lpszProcName)
            {
                return (xst_unknown)::GetProcAddress( (HMODULE)hLibrary, lpszProcName );
            }

            static xst_fi void Delay(ul32 ulMiliseconds)
            {
                Sleep( ulMiliseconds );
            }

        #if defined(XST_VISUAL_STUDIO)



        #else

        #endif//MSC_VER

        #endif//XST_WINDOWS

        public:

            u32		uiHWND;
    };

}//XST

#endif//XST_LPATFORM_H
