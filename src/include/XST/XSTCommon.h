
#ifndef XST_COMMON_H
#define XST_COMMON_H

//If disabled C++ exceptions
#pragma warning(disable: 4530)


#include "XSTCommonInclude.h"
#include "XSTCommonTypes.h"
#include "XSTTCStack.h"
#include "XSTCLastError.h"
#include "XSTCSimpleProfiler.h"

/*
#include "XSTTCStack.h"
#include "XSTCLastError.h"
#include "XSTTCSingleton.h"
#include "XSTCSimpleProfiler.h"
#include "XSTPlatform.h"
#include "XSTCLogger.h"
#include "XSTCDebug.h"
#include "XSTCMath.h"
#include "XSTCRandom.h"
//#include "XSTCString.h"
*/

#if defined (XST_ENABLE_FILE_LOGGER)
#define XST_SET_ERR(_strStream)	if( XST::CLogger::IsSingletonCreated() ) { XST::CLogger::GetSingleton() << "[ERROR]: " << _strStream; XST::CLogger::GetSingletonPtr()->FlushStreamError( XST::CLogger::ALL ); } else { std::stringstream ss; ss << _strStream; XST::CLastError::Set( ss.str() ); }
#define XST_SET_WRN(_strStream) if( XST::CLogger::IsSingletonCreated() ) { XST::CLogger::GetSingleton() << "[ERROR]: " << _strStream; XST::CLogger::GetSingletonPtr()->FlushStreamWarning( XST::CLogger::ALL ); } else { std::stringstream ss; ss << _strStream; XST::CLastError::Set( ss.str() ); }
#else	
#define XST_SET_ERR(_strStream) { std::stringstream xst_ss; xst_ss << _strStream; XST::CLastError::Set( xst_ss.str() ); }
#define XST_SET_WRN(_strStream) XST_SET_ERR( _strStream )
#endif

namespace XST
{

		using namespace XST::Types;

		static xst_fi f32 ByToKB(const u32& _uiBytes)
		{
			return (f32)_uiBytes / 1024.0f;
		}

		static xst_fi u32 KBToBy(const f32& _fKBytes)
		{
			return (u32)(_fKBytes * 1024);
		}

		static xst_fi f32 KBToMB(const f32& _fKBytes)
		{
			return _fKBytes / 1024.0f;
		}

		static xst_fi u32 MBToKB(const f32& _fMBytes)
		{
			return (u32)(_fMBytes * 1024);
		}

		static xst_fi f32 ByToMB(const u32& _uiBytes)
		{
			return KBToMB( ByToKB(_uiBytes) );
		}

		static xst_fi u32 MBToBy(const f32& _fMBytes)
		{
			return KBToBy( (f32)MBToKB(_fMBytes) );
		}

		struct BoxCorners
		{
			enum CORNER
			{
				LEFT_BOTTOM_BACK,
				RIGHT_BOTTOM_BACK,
				LEFT_TOP_BACK,
				RIGHT_TOP_BACK,
				LEFT_BOTTOM_FRONT,
				RIGHT_BOTTOM_FRONT,
				LEFT_TOP_FRONT,
				RIGHT_TOP_FRONT,
				_ENUM_COUNT
			};
		};
		typedef BoxCorners::CORNER	BOX_CORNER;

}//XST



#endif //XST_COMMON_H
