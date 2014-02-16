#ifndef XST_COMMON_TYPES_H
#define XST_COMMON_TYPES_H

#include "XSTCommonInclude.h"

namespace XST
{

	namespace Types
	{
		typedef unsigned char		u8;
		typedef unsigned short		u16;
		typedef unsigned int		u32;
		typedef unsigned long		ul32;
		typedef unsigned long long	ul64;
		typedef char				i8;
		typedef short				i16;
		typedef int					i32;
		typedef long				l32;
		typedef long long			l64;
		typedef unsigned long		dword;
		typedef float				f32;
		typedef double				f64;
		typedef char				ch8;
		typedef wchar_t				ch16;
		typedef const ch16*			lpcwstr;
		typedef const ch8*			lpcastr;
		typedef lpcastr				xst_lpcstr;
		typedef lpcwstr				xst_lpcwstr;
		typedef u8*					pu8;
		typedef u16*				pu16;
		typedef u32*				pu32;
		typedef ul32*				pul32;
		typedef i8*					pi8;
		typedef i16*				pi16;
		typedef i32*				pi32;
		typedef l32*				pl32;
		typedef dword*				pdword;
		typedef f32*				pf32;
		typedef f64*				pf64;
		typedef ch8*				pch8;
		typedef ch16*				pch16;
		typedef void*				xst_unknown;
		typedef ch8*				lpastr;
		typedef ch16*				lpwstr;
		typedef const f32			cf32;
		typedef const f64			cf64;
		typedef const u8			cu8;
		typedef const ch8			cch8;
		typedef const ch16			cch16;
		typedef const u16			cu16;
		typedef const u32			cu32;
		typedef const l32			cl32;
		typedef const ul32			cul32;
		typedef const i16			ci16;
		typedef const i32			ci32;

#if defined( XST_VISUAL_STUDIO )
		typedef __int64				i64;
		typedef unsigned __int64	u64;
#elif defined( XST_GCC )
		typedef int64_t				i64;
		typedef uint64_t			u64;
#endif

		typedef const i64			ci64;
		typedef const u64			cu64;

		#define XST_MIN_I8		 SCHAR_MIN
		#define XST_MIN_U8		 0
		#define XST_MIN_I16		 SHRT_MIN
		#define XST_MIN_U16		 0
		#define XST_MIN_I32		 INT_MIN
		#define XST_MIN_I64		 _I64_MIN
		#define XST_MIN_U64		 0
		#define XST_MIN_U32		 0
		#define XST_MIN_L32		 LONG_MIN
		#define XST_MIN_UL32	 0
		#define XST_MIN_L64		 LLONG_MIN
		#define XST_MIN_UL64	 0
		#define XST_MIN_F32		 FLT_MIN
		#define XST_MIN_F64		 DBL_MIN

		#define XST_MAX_I8		 SCHAR_MAX
		#define XST_MAX_U8		 UCHAR_MAX
		#define XST_MAX_I16		 SHRT_MAX
		#define XST_MAX_U16		 USHRT_MAX
		#define XST_MAX_I32		 INT_MAX
		#define XST_MAX_I64		 _I64_MAX
		#define XST_MAX_U64		 _UI64_MAX
		#define XST_MAX_U32		 UINT_MAX
		#define XST_MAX_L32		 LONG_MIN
		#define XST_MAX_UL32	 ULONG_MAX
		#define XST_MAX_L64		 LLONG_MAX
		#define XST_MAX_UL64	 ULLONG_MAX
		#define XST_MAX_F32		 FLT_MAX
		#define XST_MAX_F64		 DBL_MAX
		

		#if defined (XST_VISUAL_STUDIO)
		#	if defined(XST_NODLL) || defined (XST_NO_DLL)
		#		define XST_API
		#		define XST_TEMPLATE_API	
		#	else//IF NOT DEFINED _XS_NO_DLL
		#		if defined(XST_DLL_EXPORT) || defined (XST_DLLEXPORT)
		#			define XST_API	__declspec(dllexport)
		#			define XST_TEMPLATE_API 
		#		else//IF NOT DEFINTD XS_DLL_EXPORT
		#			define XST_API	__declspec(dllimport)
		#			define XST_TEMPLATE_API extern 
		#		endif//CORE_DLL_EXPORT
		#	endif//_XS_NO_DLL
		#elif defined XST_GCC
		#	if defined(XST_NODLL)
		#		define XST_API
		#		define XST_TEMPLATE_API template class XST_API
		#	else//IF NOT DEFINED _XS_NO_DLL
		#		if defined(XST_DLL_EXPORT)
		#			define XST_API	__attribute__((dllexport))
		#			define XST_TEMPLATE_API 
		#		else//IF NOT DEFINTD XS_DLL_EXPORT
		#			define XST_API	__attribute__((dllimport))
		#			define XST_TEMPLATE_API extern
		#		endif//CORE_DLL_EXPORT
		#	endif//_XS_NO_DLL
		#endif //XST_GCC

		#define XST_TEMPLATE_CLASS		XST_TEMPLATE_API template class XST_API
		#define XST_TEMPLATE_STRUCT		XST_TEMPLATE_API template struct XST_API

		#if defined XST_STL
		#	define xst_vector		std::vector
		#	define xst_list			std::list
		#   if defined (XST_VISUAL_STUDIO)
		#	    define xst_map		stdext::hash_map
		#   else
		#       define xst_map      std::map
		#   endif //XST_VISUAL_STUDIO
		#	define xst_pair			std::pair
		#	define xst_astring		std::string
		#	define xst_wstring		std::wstring
		#	define xst_queue		std::queue
		#	define xst_sort			std::sort
		#	define xst_find			std::find
		#	define xst_stringstream	std::stringstream
		#	define xst_ostream		std::ostream
		#	define xst_istream		std::istream
		#	define xst_fstream		std::fstream
		#	define xst_basic_stream	std::basic_stream
		#	define xst_basic_string	std::basic_string
		#elif defined XST_STLPORT
		#	define xst_vector		_STL::vector
		#	define xst_map			_STL::map
		#	define xst_pair			_STL::pair
		#	define xst_astring		_STL::string
		#	define xst_wstring		_STL::wstring
		#endif //_USING_STL

		#if defined (XST_WINDOWS)
		#	define xst_endl			"\r\n"
		#	define xst_wendl		L"\r\n"
		#	define xst_ends			"\0"
		#else
		#	define xst_endl			"\n"
		#	define xst_wendl		L"\n"
		#	define xst_ends			"\0" 
		#endif

		#define XST_MACRO_SCOPE(_exp) do { (_exp); } while(0)
		#define xst_sort_ex(_container, _func)	xst_sort( (_container).begin(), (_container).end(), &(_func) )
		#define xst_find_ex(_container, _value)	xst_find( (_container).begin(), (_container).end(), (_value) )

		typedef const std::string	xst_castring;
		typedef const std::wstring	xst_cwstring;

		#if( XST_VS_VER > 9 )
		#	define xst_stlx( _container )					XST_TEMPLATE_CLASS _container
		#	define xst_tstlx( _container )					XST_TEMPLATE_CLASS typedef _container
		#	define xst_mapx( _key, _value )					xst_map< _key, _value >
		#	define xst_tmapx( _key, _value )				typedef xst_map< _key, _value >
		#	define xst_vectorx( _type )						xst_vector< _type >
		#	define xst_tvectorx( _type )					typedef xst_vector< _type >
		#	define xst_listx( _type )						xst_list< _type >
		#	define xst_tlistx( _type )						typedef xst_list< _type >
		#	define xst_pairx( _key, _value )				xst_pair< _key, _value >
		#	define xst_tpairx( _key, _value )				typedef xst_pair< _key, _value >
		#	define XST_DEF_EXPORT_CLASS(_class, _member)	typedef _class _member				
		#else
		#	define xst_stlx( _container )					XST_TEMPLATE_CLASS _container; _container
		#	define xst_tstlx( _container )					XST_TEMPLATE_CLASS _container; typedef _container
		#	define xst_mapx( _key, _value )					XST_TEMPLATE_CLASS xst_map< _key, _value >; xst_map< _key, _value >
		#	define xst_tmapx( _key, _value )				XST_TEMPLATE_CLASS xst_map< _key, _value >; typedef xst_map< _key, _value >
		#	define xst_vectorx( _type )						XST_TEMPLATE_CLASS xst_vector< _type >; xst_vector< _type >
		#	define xst_tvectorx( _type )					XST_TEMPLATE_CLASS xst_vector< _type >; typedef xst_vector< _type >
		#	define xst_listx( _type )						XST_TEMPLATE_CLASS xst_list< _type >; xst_list< _type >
		#	define xst_tlistx( _type )						XST_TEMPLATE_CLASS xst_list< _type >; typedef xst_list< _type >
		#	define xst_pairx( _key, _value )				XST_TEMPLATE_STRUCT xst_pair< _key, _value >; xst_pair< _key, _value >
		#	define xst_tpairx( _key, _value )				XST_TEMPLATE_STRUCT xst_pair< _key, _value >; typedef xst_pair< _key, _value >
		#	define XST_DEF_EXPORT_CLASS(_class, _member)	XST_TEMPLATE_CLASS _class; typedef _class _member
		#endif //XST_VS_11

		xst_tvectorx( xst_astring )	StringVector;

		#define xst_stl_foreach(_itr, _container) for(_itr = _container.begin(); _itr != _container.end(); ++_itr)

		struct RESULT
		{
			enum
			{
				OK = 0,
				FAILED,
				FAILED_CRITICAL,
			};
		};

		#define XST_OK	XST::RESULT::OK
		#define XST_FAIL XST::RESULT::FAILED

		#define XST_FAILED(_exp) ( ( _exp ) != XST::RESULT::OK )
		#define XST_F(_exp) XST_FAILED( _exp )

		#define XST_TOSTRING(_type) #_type
		#define XST_ADD(_left, _right) _left##_right

		/*typedef union XST_ALIGN(16) _xst_m128 
		{
			 f32	m128_f32[4];
			 u64	m128_u64[2];
			 i8     m128_i8[16];
			 i16    m128_i16[8];
			 i32    m128_i32[4];
			 i64    m128_i64[2];
			 u8		m128_u8[16];
			 u16	m128_u16[8];
			 u32	m128_u32[4];
		 } xst_m128;*/

		#define xst_i					inline
		#if defined XST_VISUAL_STUDIO //if visual studio
		#	define xst_fi				__forceinline
		#	define XST_ALIGN(_size)		__declspec( align( _size ) )
		#	define XST_FUNCTION			__FUNCTION__
			typedef	__m128				m128;
		#elif defined XST_GCC
		#	define xst_fi				__attribute__((always_inline))
		#	define XST_FUNCTION			__func__
		#	define XST_ALIGN(_bytes)    __attribute__( ( aligned( ( _bytes ) ) ) )
			//typedef f32 				XST_ALIGN( 16 ) m128[4];
			typedef __m128              m128;
		#endif

/*
		union XST_ALIGN( 16 ) _xst_m128
		{
            m128    xyzw;
            f32     m128_f32[4];
            struct  { f32 x,y,z,w; };

            _xst_m128() {}
            _xst_m128(const m128& _m128) : xyzw( _m128 ) {}
            _xst_m128(cf32& _fX, cf32& _fY, cf32& _fZ, cf32& _fW) : x(_fX), y(_fY), z(_fZ), w(_fW) {}
            void operator=(const m128& _m128) { xyzw = _m128; }

            operator m128() const { return xyzw; }
            operator f32() const { return x; }

		};
*/
		

		#if defined (_UNICODE) || (UNICODE)
		#	define XST_TEXT(_text)	L##_text
		#	define xst_string xst_wstring
			typedef lpcastr lpcstr;
		#else
		#	define XST_TEXT(_text) _text
		#	define xst_string xst_astring
			typedef lpcwstr lpcstr;
		#endif //UNICODE

		#if defined (XST_DEBUG)
		#	define xst_assert(_error, _text)	assert((_error) && (_text))
		#	define xst_assert2(_error)			xst_assert( _error, XST_FUNCTION )
		#else
		#	define xst_assert(_error, _text)
		#define xst_assert2(_error)
		#endif//_DEBUG

		//#if defined (XST_VS_10)
		//#	define xst_null		nullptr
		//#else
		//#	define xst_null		0
		//#endif //XST_VS_10

		#define xst_null NULL
        #define xst_implement override
        #define xst_overload

		#define XST_TO_TEXT(_type)	#_type
		#define xst_init(_pData, _DataType)		memset(_pData, 0, sizeof(_DataType))
		#define xst_zero(_pData, _uiDataSize)	memset(_pData, 0, _uiDataSize)

		#define xst_v				virtual
		#define XST_DECL(_type, _func)	_type _func
		#define XST_DECLV(_type, _func) xst_v XST_DECL(_type, _func)
		#define XST_DECLPV(_type, _func) XST_DECLV(_type, _func) = 0
		#define XST_IUNKNOWN	XST::IObject
		#define XST_CLASS(_name) class _name
		#define XST_INTERFACE(_name) XST_CLASS(_name) : public XST_IUNKNOWN

		#if defined XST_STL || XST_STLPORT
		#	define XSTVectorClear(_Vector, _dataType) _Vector.clear(); xst_vector<_dataType>().swap(_Vector)
		#	define xst_vector_clear XSTVectorClear
			#define xst_copy	std::copy
		#endif //_USING_STL || _USING_STLPORT

		#define CE_SELECTION_EXCEPTION	100

		//#define xst_new new
		//#define xst_delete delete
		#if defined(XST_DEBUG) || defined (_DEBUG)
		#   define xst_delete(_pPtr) do { delete (_pPtr); (_pPtr) = xst_null; } while(0)
		#   define xst_deletea(_pArray) do { delete[] (_pArray); (_pArray) = xst_null; } while(0)
		#else//if defined(_DEBUG)
		#   define xst_delete(_pPtr) do { delete (_pPtr); (_pPtr) = xst_null; } while(0)
		#   define xst_deletea(_pArray) do { delete[] (_pArray); (_pArray) = xst_null; } while(0)
		#endif

		#if defined(XST_VISUAL_STUDIO)
		//#	define xst_memcpy(_dst, _dstSize, _src, _maxCount) memcpy_s( (_dst), (_dstSize), (_src), (_maxCount) )
		#	define xst_strcpy(_dst, _sizeInBytes, _src) strcpy_s( _dst, _sizeInBytes, _src )
		#   define xst_wcstombs(_puiCharConverted, _pDstBuffer, _uiDstSize, _pSrcBuffer, _uiSrcSize) wcstombs_s( _puiCharConverted, _pDstBuffer, _uiDstSize, _pSrcBuffer, _uiSrcSize )
		#   define xst_mbstowcs(_puiCharConverted, _pDstBuffer, _uiDstSize, _pSrcBuffer, _uiSrcSize) mbstowcs_s( _puiCharConverted, _pDstBuffer, _uiDstSize, _pSrcBuffer, _uiSrcSize )
		#   define xst_fopen(_pFile, _strFilename, _strMode)    fopen_s( _pFile, _strFilename, _strMode )
		#   define xst_vsprintf(_pBuff, _uiBuffSize, _lpszFormat, _va_list) vsprintf_s( _pBuff, _uiBuffSize, _lpszFormat, _va_list )
		#	define xst_vsnprintf(_pBuff, _uiBuffSize, _uiMaxCount, _strFormat, _va_list) _vsnprintf_s( _pBuff, _uiBuffSize, _uiMaxCount, _strFormat, _va_list )
		#	define xst_wtoi(_wstrText) _wtoi( _wstrText )
		#	define xst_sprintf(_pBuff, _uiBuffSize, _lpszFormat, ...)	sprintf_s( (_pBuff), (_uiBuffSize), _lpszFormat, ##__VA_ARGS__ )
		#else
		//#	define xst_memcpy(_dst, _dstSize, _src, _maxCount)	memcpy(_dst, _src, _maxCount)
		#	define xst_strcpy(_dst, _sizeInBytes, _src) strcpy( _dst, _src )
		#   define xst_wcstombs(_puiCharConverted, _pDstBuffer, _uiDstSize, _pSrcBuffer, _uiSrcSize) ( *_puiCharConverted = wcstombs( _pDstBuffer, _pSrcBuffer, _uiDstSize ) )
		#   define xst_mbstowcs(_puiCharConverted, _pDstBuffer, _uiDstSize, _pSrcBuffer, _uiSrcSize) ( *_puiCharConverted = mbstowcs( _pDstBuffer, _pSrcBuffer, _uiDstSize ) )
		#   define xst_fopen(_pFile, _strFilename, _strMode) ( ( (*_pFile) = fopen( _strFilename, _strMode ) ) != xst_null )? 0 : 1
		#   define xst_vsprintf(_pBuff, _uiBuffSize, _lpszFormat, _va_list) vsprintf( _pBuff, _lpszFormat, _va_list )
		#	define xst_wtoi(_wstrText) wcstol( _wstrText )
		#	define xst_vsnprintf(_pBuff, _uiBuffSize, _uiMaxCount, _strFormat, _va_list) vsnprintf(_pBuff, _uiBuffSize, _strFormat, _va_list)
		#	define xst_sprintf(_pBuff, _uiBuffSize, _lpszFormat, ...)	snprintf( (_pBuff), (_uiBuffSize), _lpszFormat, ##__VA_ARGS__ )
		#endif

		#define xst_out	*

        xst_fi i32 xst_vscprintf(lpcastr lpszFormat, va_list vaList)
        {
            #if defined XST_VISUAL_STUDIO
                return _vscprintf( lpszFormat, vaList );
            #else
                ch8 aBuff[256];
                return vsprintf( aBuff, lpszFormat, vaList );
            #endif
        }

        xst_fi i32 xst_memcpy(void* _pDst, const ul32& _ulDstSize, const void* _pSrc, const ul32& _ulDataSizeToCopy)
        {
            #if defined (XST_VISUAL_STUDIO)
                return memcpy_s( _pDst, _ulDstSize, _pSrc, _ulDataSizeToCopy );
            #else
                return !(memcpy( _pDst, _pSrc, _ulDataSizeToCopy ) == _pDst );
            #endif
        }

		/*static void XSTSetLastError(xst_castring _strError)
		{
			g_strXSTLastError = _strError;
		}

		static xst_castring& XSTGetLastError()
		{
			return g_strXSTLastError;
		}*/

		#define XST_CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))
		#define XST_CALL_MEMBER_FN_PTR(object,ptrToMember)  ((object)->*(ptrToMember))
		#define	XST_CALL_METHOD(_object, _MethodPtr) ( (_object).*(_MethodPtr) )
		#define	XST_CALL_METHODP(_objectPtr, _MethodPtr) ( (_objectPtr)->*(_MethodPtr) )

		// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
		//allocations to be of _CLIENT_BLOCK type
		#if defined (_DEBUG)
		#	define dbg_new new( _NORMAL_BLOCK, __FILE__, __LINE__)
		#	define dbg_new_p(_param) new( _NORMAL_BLOCK, __FILE__, __LINE__, _param)
		#	if defined (XST_VISUAL_STUDIO)
		#		define xst_malloc(_size)			_malloc_dbg(_size, _NORMAL_BLOCK, __FILE__, __LINE__)
		#		define xst_calloc(_count, _size)	_calloc_dbg(_count, _size, _NORMAL_BLOCK, __FILE__, __LINE__)
		#		define xst_free(_ptr)				_free_dbg(_ptr, _NORMAL_BLOCK); _ptr = 0
		#	else //if not visual studio
		#		define xst_malloc(_size)			malloc(_size)
		#		define xst_calloc(_count, _size)	calloc(_count, _size)
		#		define xst_free(_ptr)				free(_ptr)
		#	endif //if visual studio
		#	define xst_new dbg_new
		#	define xst_new_p(_param) dbg_new_p(_param)
		#else //if not debug
		#	define xst_new new(std::nothrow)
		#	define xst_new_p(_param)			new(std::nothrow, _param)
		#	define xst_malloc(_size)			malloc(_size)
		#	define xst_calloc(_count, _size)	calloc(_count, _size)
		#	define xst_free(_ptr)				free(_ptr); _ptr = 0
		#endif//DEBUG

		#if defined (XST_WINDOWS)
		#	define xst_local_alloc(_flags, _size)	::LocalAlloc(_flags, _size)
		#	define xst_local_free(_ptr)				::LocalFree(_ptr); _ptr = 0
		#	define xst_global_alloc(_flags, _size)	::GlobalAlloc(_flags, _size)
		#	define xst_global_free(_ptr)			::GlobalFree(_ptr); _ptr = 0
		#else //if not windows
		#	define xst_local_alloc(_flags, _size)	xst_malloc(_size)
		#	define xst_local_free(_ptr)				xst_free(_ptr); _ptr = 0
		#	define xst_global_alloc(_flags, _size)	xst_malloc(_size)
		#	define xst_global_free(_ptr)			xst_free(_ptr); _ptr = 0
		#endif //if windows

		#define xst_lalloc(_size)				xst_local_alloc(0x0000 | 0x0040, _size)
		#define xst_lfree(_ptr)					xst_local_free(_ptr)
		#define xst_galloc(_size)				xst_global_alloc(0x0000, _size)
		#define xst_gfree(_ptr)					xst_global_free(_ptr)

		#define XST_CALL	__cdecl

		#define XST_SWAP(_left, _right, _tmp) _tmp = _right; _right = _left; _left = _tmp

	}//Types

	#define XST_TYPES XST::Types;

}//XST

#include "XSTTCStack.h"

#endif //XST_COMMON_TYPES_H
