#ifndef XST_CTIME_H
#define XST_CTIME_H

#include "XSTCommonInclude.h"
#include "XSTCommonTypes.h"

namespace XST
{
	using namespace Types;

	class XST_API CTime
	{
		public:

			struct SSystemTime
			{
				u16	usHour;
				u16	usMinute;
				u16	usSecond;
				u16 usMillisecond;
				u16	usDay;
				u16	usDayOfWeek;
				u16	usYear;
				u16	usMonth;

				xst_astring ToString(lpcastr lpszFormat = "Y-M-D h:m:s")
				{
					std::stringstream ss;
					for(i32 i = 0; lpszFormat[ i ] != 0; ++i)
					{
						if( lpszFormat[i] == 'Y' ) ss << usYear << lpszFormat[i+1];
						else if( lpszFormat[i] == 'M' ) ss << usMonth << lpszFormat[i+1];
						else if( lpszFormat[i] == 'D' ) ss << usDay << lpszFormat[i+1];
						else if( lpszFormat[i] == 'h' ) ss << usHour << lpszFormat[i+1];
						else if( lpszFormat[i] == 'm' ) ss << usMinute << lpszFormat[i+1];
						else if( lpszFormat[i] == 's' ) ss << usSecond << lpszFormat[i+1];
						else if( lpszFormat[i] == 'i' ) ss << usMillisecond << lpszFormat[i+1];
					}

					return ss.str();
				}

				xst_astring TimeToString(lpcastr lpszFormat = "h:m:s")
				{
					std::stringstream ss;
					for(i32 i = 0; lpszFormat[ i ] != 0; ++i)
					{
						if( lpszFormat[i] == 'h' ) ss << usHour << lpszFormat[i+1];
						else if( lpszFormat[i] == 'm' ) ss << usMinute << lpszFormat[i+1];
						else if( lpszFormat[i] == 's' ) ss << usSecond << lpszFormat[i+1];
						else if( lpszFormat[i] == 'i' ) ss << usMillisecond << lpszFormat[i+1];
					}

					return ss.str();
				}

				xst_astring DateToString(lpcastr lpszFormat = "Y-M-D")
				{
					std::stringstream ss;
					for(i32 i = 0; lpszFormat[ i ] != 0; ++i)
					{
						if( lpszFormat[i] == 'Y' ) ss << usYear << lpszFormat[i+1];
						else if( lpszFormat[i] == 'M' ) ss << usMonth << lpszFormat[i+1];
						else if( lpszFormat[i] == 'D' ) ss << usDay << lpszFormat[i+1];
					}

					return ss.str();
				}

			};

		public:

			xst_fi static ul32		GetTickCount() 
			{
				#if defined(WIN32)
					return ::GetTickCount();
				#endif
			}

			xst_fi static ul32		GetQPerfTickCount()
			{
				#if defined(WIN32)
					DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);
					LARGE_INTEGER Time;
					::QueryPerformanceCounter(&Time);
					::SetThreadAffinityMask(::GetCurrentThread(), oldmask);
					return (ul32)Time.QuadPart;
				#endif
			}

			xst_i static f64		GetFrequency()
			{
				#if defined(WIN32)
					LARGE_INTEGER Frequency;
					::QueryPerformanceFrequency(&Frequency);
					return 1.0 / Frequency.QuadPart;
				#endif
			}

			xst_i static i32 GetSystemTime(SSystemTime* _pSysTimeOut)
			{
				#if defined(XST_WINDOWS)
					SYSTEMTIME SysTime;
					::GetLocalTime( &SysTime );
					_pSysTimeOut->usDay = SysTime.wDay;
					_pSysTimeOut->usDayOfWeek = SysTime.wDayOfWeek;
					_pSysTimeOut->usHour = SysTime.wHour;
					_pSysTimeOut->usMillisecond = SysTime.wMilliseconds;
					_pSysTimeOut->usMinute = SysTime.wMinute;
					_pSysTimeOut->usYear = SysTime.wYear;
					_pSysTimeOut->usSecond = SysTime.wSecond;
					_pSysTimeOut->usMonth = SysTime.wMonth;
					return RESULT::OK;
				#else
					return RESULT::FAILED;
				#endif
			}
			
	};

}//XST


#endif//XST_CTIME_H