#include "XSED3D11FuncTypedefs.h"

#define XSE_DX_FUNC_DECL_HR(_name) typedef HRESULT (WINAPI * pfn##_name)
#define XSE_DX_FUNC_DEF(_name) pfn##_name _name = xst_null

namespace XSE
{
    namespace D3D11
    {
        XSE_DX_FUNC_DEF( D3D11CreateDeviceAndSwapChain );
        XSE_DX_FUNC_DEF( CreateDXGIFactory );
        XSE_DX_FUNC_DEF( CreateDXGIFactory1 );
        XSE_DX_FUNC_DEF( D3D11CreateDevice );
        XSE_DX_FUNC_DEF( D3DCompile );
        XSE_DX_FUNC_DEF( D3DReflect );
    } // d3d11
} // xse