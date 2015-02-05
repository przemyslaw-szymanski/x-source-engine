#ifndef XSE_D3D11_COMMON_H
#define XSE_D3D11_COMMON_H

#include "XSECommon.h"
#include "XSERenderSystemCommon.h"

#if defined (XST_WINDOWS)

#if defined (XST_VISUAL_STUDIO)
#	pragma warning( push )
#	pragma warning( disable : 4005 ) //macro redefinition
#endif

#include <d3d11.h>
#include <d3dcompiler.h>
//#include <d3dx11.h>
//#include <xnamath.h>
#include <DirectXCollision.h>
#include "ThirdParty/DirectXTex/DirectXTex.h"

namespace XSE
{
	namespace D3D11
	{
		#define SHADER_TYPE_VERTEX		0
		#define SHADER_TYPE_PIXEL		1
		#define SHADER_TYPE_GEOMETRY	2
		#define SHADER_TYPE_COMPUTE		3
		#define SHADER_TYPE_COUNT		4

		class CRenderSystem;
		class CViewport;
		class IShaderBase;
		class CVertexShader;
		class CPixelShader;
		class CGeometryShader; //undefined
		class CComputeShader; //undefined
		class CVertexBuffer;
		class CInputLayout;
		class CIndexBuffer;
		class CHLSLShaderSystem;
		class CCGShaderSystem;

		struct SMatrixBuffer
		{
			DirectX::XMMATRIX	mtxWorld;
			DirectX::XMMATRIX	mtxView;
			DirectX::XMMATRIX	mtxProjection;
		};

	}//d3d11

	xst_fi DirectX::XMVECTOR	VecToXMVec( const Vec2& vec )
	{
		return DirectX::XMVectorSet( vec.x, vec.y, 0.0f, 0.0f );
	}

	xst_fi DirectX::XMVECTOR	VecToXMVec( const Vec3& vec )
	{
		return DirectX::XMVectorSet( vec.x, vec.y, vec.z, 0.0f );
	}

	xst_fi DirectX::XMVECTOR	VecToXMVec( const Vec4& vec )
	{
		return DirectX::XMVectorSet( vec.x, vec.y, vec.z, vec.w );
	}

}//xse

#if defined (XST_VISUAL_STUDIO)
#	pragma warning( pop ) //macro redefinition
#endif

#endif //XSE_WINDOWS

#endif //XSE_D3D11_COMMON_H