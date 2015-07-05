#ifndef XSED3D11_FUNC_TYPEDEFS_H
#define XSED3D11_FUNC_TYPEDEFS_H

#include "XSED3D11Common.h"

typedef HRESULT (WINAPI * pfnD3D11CreateDeviceAndSwapChain)(IDXGIAdapter*,D3D_DRIVER_TYPE,HMODULE,UINT, CONST D3D_FEATURE_LEVEL*,UINT,UINT,CONST DXGI_SWAP_CHAIN_DESC*,IDXGISwapChain**,ID3D11Device**,D3D_FEATURE_LEVEL*,ID3D11DeviceContext** );
typedef HRESULT (WINAPI * pfnCreateDXGIFactory)(REFIID riid, void **ppFactory);
typedef HRESULT (WINAPI * pfnCreateDXGIFactory1)(REFIID riid, void **ppFactory);
typedef HRESULT (WINAPI * pfnD3D11CreateDevice)(__in_opt IDXGIAdapter* pAdapter,D3D_DRIVER_TYPE DriverType,HMODULE Software,UINT Flags,__in_ecount_opt( FeatureLevels ) CONST D3D_FEATURE_LEVEL* pFeatureLevels,UINT FeatureLevels,UINT SDKVersion,__out_opt ID3D11Device** ppDevice,__out_opt D3D_FEATURE_LEVEL* pFeatureLevel,__out_opt ID3D11DeviceContext** ppImmediateContext );
typedef HRESULT (WINAPI * pfnD3DCompile)(_In_reads_bytes_(SrcDataSize) LPCVOID pSrcData, _In_ SIZE_T SrcDataSize, _In_opt_ LPCSTR pSourceName, _In_reads_opt_(_Inexpressible_(pDefines->Name != NULL)) CONST D3D_SHADER_MACRO* pDefines, _In_opt_ ID3DInclude* pInclude, _In_opt_ LPCSTR pEntrypoint, _In_ LPCSTR pTarget, _In_ UINT Flags1, _In_ UINT Flags2, _Out_ ID3DBlob** ppCode, _Out_opt_ ID3DBlob** ppErrorMsgs);
typedef HRESULT (WINAPI * pfnD3DReflect)(_In_reads_bytes_(SrcDataSize) LPCVOID pSrcData, _In_ SIZE_T SrcDataSize, _In_ REFIID pInterface, _Out_ void** ppReflector);


#endif //XSED3D11_FUNC_TYPEDEFS_H