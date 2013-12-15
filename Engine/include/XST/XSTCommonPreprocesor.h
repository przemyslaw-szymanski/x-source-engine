#ifndef XST_COMMON_PREPROCESOR_H
#define XST_COMMON_PREPROCESOR_H

#if defined(_M_IX86) || defined(__i386__) || defined(_X86_)
#   define XST_X86 1
#elif defined (_M_X64) || defined(_M_IA64) || defined(__ia64__) || defined(_M_AMD64) || defined(_AMD64_)
#   define XST_X64 1
#endif //_M_IX86

#if defined (XST_X86) || defined (XST_X64)
#	define XST_SSE2
#endif //XST_X86

#if defined (WIN32) || defined (WIN64) || defined (WINDOWS) || defined (_WINDOWS)
#	define XST_WINDOWS	1
#elif defined (LINUX) || defined (_LINUX )
#	define XST_LINUX	1
#else
#	error	"This OS is not supported"
#endif

#if defined (_MSC_VER)
#	define XST_VISUAL_STUDIO	1
#elif defined (__GNUG__)
#	define XST_GCC	1
#elif defined ( __INTEL_COMPILER )
#   define  XST_INTEL 1
#endif //_MSC_VER

#if defined XST_VISUAL_STUDIO
#	if( _MSC_VER == 1500 )
#		define XST_VS_9	1
#		define XST_VS_VER 9
#	elif( _MSC_VER == 1600 )
#		define	XST_VS_10	1
#		define	XST_VS_VER	10
#	elif( _MSC_VER >= 1700 )
#		define XST_VS_11	1
#		define XST_VS_VER	11
#	endif//_MSC_VER
#endif //XST_VISUAL_STUDIO

#if defined (DEBUG) || defined (_DEBUG)
#	define XST_DEBUG	1
#endif//DEBUG

#if defined (_UNICODE) || defined (UNICODE)
#	define XST_UNICODE 1
#endif

#if defined( XST_DEBUG )
#	define XST_ENABLE_FILE_LOGGER		1
#	define XST_ENABLE_CONSOLE_LOGGER	1
#	define XST_OBJ_DEBUG				1
#endif //XST_DEBUG

// Newer processor features than what is currently defined in
//   Visual Studio 2003
#ifndef PF_SSE_DAZ_MODE_AVAILABLE
#define PF_SSE_DAZ_MODE_AVAILABLE               11
#endif
#ifndef PF_NX_ENABLED
#define PF_NX_ENABLED                           12
#endif
#ifndef PF_SSE3_INSTRUCTIONS_AVAILABLE
#define PF_SSE3_INSTRUCTIONS_AVAILABLE          13
#endif
#ifndef PF_COMPARE_EXCHANGE128
#define PF_COMPARE_EXCHANGE128                  14
#endif
#ifndef PF_COMPARE64_EXCHANGE128
#define PF_COMPARE64_EXCHANGE128                15
#endif
#ifndef PF_CHANNELS_ENABLED
#define PF_CHANNELS_ENABLED                     16
#endif


// Newer product types than what is currently defined in
//   Visual Studio 2005
#ifndef PRODUCT_ULTIMATE
#define PRODUCT_ULTIMATE                        0x00000001
#endif
#ifndef PRODUCT_HOME_BASIC
#define PRODUCT_HOME_BASIC                      0x00000002
#endif
#ifndef PRODUCT_HOME_PREMIUM
#define PRODUCT_HOME_PREMIUM                    0x00000003
#endif
#ifndef PRODUCT_ENTERPRISE
#define PRODUCT_ENTERPRISE                      0x00000004
#endif
#ifndef PRODUCT_HOME_BASIC_N
#define PRODUCT_HOME_BASIC_N                    0x00000005
#endif
#ifndef PRODUCT_BUSINESS
#define PRODUCT_BUSINESS                        0x00000006
#endif
#ifndef PRODUCT_STANDARD_SERVER
#define PRODUCT_STANDARD_SERVER                 0x00000007
#endif
#ifndef PRODUCT_DATACENTER_SERVER
#define PRODUCT_DATACENTER_SERVER               0x00000008
#endif
#ifndef PRODUCT_SMALLBUSINESS_SERVER
#define PRODUCT_SMALLBUSINESS_SERVER            0x00000009
#endif
#ifndef PRODUCT_ENTERPRISE_SERVER
#define PRODUCT_ENTERPRISE_SERVER               0x0000000A
#endif
#ifndef PRODUCT_STARTER
#define PRODUCT_STARTER                         0x0000000B
#endif
#ifndef PRODUCT_DATACENTER_SERVER_CORE
#define PRODUCT_DATACENTER_SERVER_CORE          0x0000000C
#endif
#ifndef PRODUCT_STANDARD_SERVER_CORE
#define PRODUCT_STANDARD_SERVER_CORE            0x0000000D
#endif
#ifndef PRODUCT_ENTERPRISE_SERVER_CORE
#define PRODUCT_ENTERPRISE_SERVER_CORE          0x0000000E
#endif
#ifndef PRODUCT_ENTERPRISE_SERVER_IA64
#define PRODUCT_ENTERPRISE_SERVER_IA64          0x0000000F
#endif
#ifndef PRODUCT_BUSINESS_N
#define PRODUCT_BUSINESS_N                      0x00000010
#endif
#ifndef PRODUCT_WEB_SERVER
#define PRODUCT_WEB_SERVER                      0x00000011
#endif
#ifndef PRODUCT_CLUSTER_SERVER
#define PRODUCT_CLUSTER_SERVER                  0x00000012
#endif
#ifndef PRODUCT_HOME_SERVER
#define PRODUCT_HOME_SERVER                     0x00000013
#endif
#ifndef PRODUCT_STORAGE_EXPRESS_SERVER
#define PRODUCT_STORAGE_EXPRESS_SERVER          0x00000014
#endif
#ifndef PRODUCT_STORAGE_STANDARD_SERVER
#define PRODUCT_STORAGE_STANDARD_SERVER         0x00000015
#endif
#ifndef PRODUCT_STORAGE_WORKGROUP_SERVER
#define PRODUCT_STORAGE_WORKGROUP_SERVER        0x00000016
#endif
#ifndef PRODUCT_STORAGE_ENTERPRISE_SERVER
#define PRODUCT_STORAGE_ENTERPRISE_SERVER       0x00000017
#endif
#ifndef PRODUCT_SERVER_FOR_SMALLBUSINESS
#define PRODUCT_SERVER_FOR_SMALLBUSINESS        0x00000018
#endif
#ifndef PRODUCT_SMALLBUSINESS_SERVER_PREMIUM
#define PRODUCT_SMALLBUSINESS_SERVER_PREMIUM    0x00000019
#endif


#endif //XST_COMMIN_PREPROCESOR_H
