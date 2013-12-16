#ifndef XST_COMMON_INCLUDE_H
#define XST_COMMON_INCLUDE_H

#include "XSTCommonPreprocesor.h"

//Disable warning ''extern' before template explicit instantiation
#pragma warning (disable : 4231)
//Disable warning 'decorated name length exceeded, name was truncated' for stl
#pragma warning( disable: 4503 )
//Disable warning 'class needs to have dll-interface to be used by clients of class' only for stl
#pragma warning( disable: 4251 )
#pragma warning( disable: 4275 )

#include "STL/XSTTCAlignedAllocator.h"

#if defined XST_STL
#include <vector>
#include <map>
#include <hash_map>
#include <string>
#include <list>
#include <string>
#include <sstream>
#include <stack>
#include <queue>
#else
#error "Define XST_STL or XST_STLPORT to use stl"
#endif//XST_STL
#include "ThirdParty/FastDelegate/FastDelegate.h"

#include <climits>
#include <cfloat>

#if defined (XST_WINDOWS)
#	define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#	include <xmmintrin.h>
#	include <smmintrin.h>
#elif defined (XST_LINUX)
#	include <unistd.h>
#endif //XST_WINDOWS


#if defined (XST_DEBUG) && defined (XST_VISUAL_STUDIO)

#	ifndef _CRTDBG_MAP_ALLOC
#		define _CRTDBG_MAP_ALLOC
#	endif //_CRTDBG_MAP_ALLOC

#	define _INC_MALLOC

#	include <crtdbg.h>

#endif//DEBUG

#include <cassert>
#include <cstdio>
#include <cstdlib>


#endif//XST_COMMON_INCLUDE_H
