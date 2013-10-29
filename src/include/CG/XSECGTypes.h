#ifndef XSE_CG_TYPES_H
#define XSE_CG_TYPES_H

#include <CG/cg.h>

namespace XSE
{
	#define XSE_CG_ADD(_left, _right) _left##_right
	#define XSE_CG_TDF(_retType, _func) typedef _retType (CGENTRY * XSE_CG_ADD( pfn, _func ) )
	#define XSE_CG_DF(_func) static XSE_CG_ADD( pfn, _func )	_func = xst_null

	//Typedefs
	typedef CGcontext (CGENTRY * pfncgCreateContext)(void);
	typedef const char * (CGENTRY * pfncgGetLastListing)(CGcontext context);
	typedef const char * (CGENTRY * pfncgGetLastErrorString)(CGerror *error);
	XSE_CG_TDF( void, cgDestroyContext )(CGcontext);
	XSE_CG_TDF( void, cgDestroyProgram )(CGprogram);
	XSE_CG_TDF( CGprogram, cgCreateProgram )(CGcontext context, CGenum program_type, const char *program, CGprofile profile, const char *entry, const char **args);
	XSE_CG_TDF( void, cgSetMatrixParameterfr )(CGparameter, const float* matrix);

	//Functions
	static pfncgCreateContext			cgCreateContext = xst_null;
	static pfncgGetLastListing			cgGetLastListing = xst_null;
	static pfncgGetLastErrorString		cgGetLastErrorString = xst_null;
	XSE_CG_DF( cgDestroyContext );
	XSE_CG_DF( cgDestroyProgram );
	XSE_CG_DF( cgCreateProgram );
	XSE_CG_DF( cgSetMatrixParameterfr );

}//xse

#endif