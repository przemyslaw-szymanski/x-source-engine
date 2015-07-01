#ifndef XST_COMMON_INCLUDE_H
#define XST_COMMON_INCLUDE_H

#pragma warning(disable: 4251)
#pragma warning(disable: 4503)

#include "XSTCommonPreprocesor.h"

#include "STL/XSTTCAlignedAllocator.h"

// system headers warnings
#pragma warning(push)
#pragma warning(disable: 4820)
#pragma warning(disable: 4668)
#pragma warning(disable: 4251)

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
#include <unordered_map>
#include <unordered_set>
#include <hash_map>
#include <hash_set>
#include <functional>
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

#pragma warning(pop)

#endif//XST_COMMON_INCLUDE_H
