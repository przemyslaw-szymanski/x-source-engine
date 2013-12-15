#include "../../include/xst/XSTPlatform.h"

namespace XST
{
#if defined (XST_WINDOWS)

	i32 Platform::GetPlatformInfo(SPlatformInfo* pOutInfo)
	{
		SYSTEM_INFO SysInfo;
		GetSystemInfo( &SysInfo );

		SPlatformInfo Info;
		xst_zero( &Info, sizeof( SPlatformInfo ) );

		Info.uiProcessorArchitecture = SysInfo.wProcessorArchitecture;
		Info.uiProcessorType = SysInfo.dwProcessorType;
		Info.uiProcessorCount = SysInfo.dwNumberOfProcessors;

		OSVERSIONINFOEXA VerInfo;
		xst_zero( &VerInfo, sizeof( OSVERSIONINFOEXA ) );
		VerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);

		if( GetVersionExA( (OSVERSIONINFOA*)&VerInfo ) )
		{
			if( VerInfo.dwMajorVersion == 6 )
			{
				if( VerInfo.dwMinorVersion == 1 )
				{
					//Win7
					if( VerInfo.wProductType == VER_NT_WORKSTATION )
					{
						Info.uiSystemType = SystemType::WINDOWS_7;
					}
					else
					{
						Info.uiSystemType = SystemType::WINDOWS_SERVER_2008_R2;
					}
				}
				else 
				if( VerInfo.dwMinorVersion == 0 )
				{
					//VISTA
					if( VerInfo.wProductType == VER_NT_WORKSTATION )
					{
						Info.uiSystemType = SystemType::WINDOWS_VISTA;
					}
					else
					{
						Info.uiSystemType = SystemType::WINDOWS_SERVER_2008;
					}
				}
			}
			else
			if( VerInfo.dwMajorVersion == 5 )
			{
				if( VerInfo.dwMinorVersion == 0 )
				{
					Info.uiSystemType = SystemType::WINDOWS_2000;
				}
				else
				if( VerInfo.dwMinorVersion == 1 )
				{
					Info.uiSystemType = SystemType::WINDOWS_XP;
				}
				else
				if( VerInfo.dwMinorVersion == 2 )
				{
					if( GetSystemMetrics( SM_SERVERR2 ) != 0 )
					{
						Info.uiSystemType = SystemType::WINDOWS_SERVER_2003_R2;
					}
					else
					{
						Info.uiSystemType = SystemType::WINDOWS_SERVER_2003;
					}
				}
			}
		}

		*pOutInfo = Info;

		return RESULT::OK;
	}

#else

	i32 Platform::GetPlatformInfo(SPlatformInfo* _pOutInfo)
	{
		#error "Not implemented for this OS"
	}

#endif //XST_WINDOWS

}//xst