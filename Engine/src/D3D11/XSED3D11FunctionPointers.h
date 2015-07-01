#ifndef XSE_D3D11_FUNCTION_POINTERS_H
#define XSE_D3D11_FUNCTION_POINTERS_H

#include "XSED3D11FuncTypedefs.h"

#ifndef XSE_D3D11_EXTERN
#   define XSE_D3D11_EXTERN(_func)  extern XST_ADD( pfn, _func ) _func
#endif
namespace XSE
{
    namespace D3D11
    {
        XSE_D3D11_EXTERN( D3D11CreateDeviceAndSwapChain );
        XSE_D3D11_EXTERN( CreateDXGIFactory );
        XSE_D3D11_EXTERN( CreateDXGIFactory1 );
        XSE_D3D11_EXTERN( D3D11CreateDevice );
        XSE_D3D11_EXTERN( D3DCompile );
        XSE_D3D11_EXTERN( D3DReflect );
    } // d3d11
} // xse
#endif // XSE_D3D11_FUNCTION_POINTERS_H