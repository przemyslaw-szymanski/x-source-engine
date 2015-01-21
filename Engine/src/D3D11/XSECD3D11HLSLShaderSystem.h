#ifndef XSE_CD3D11HLSL_SHADER_SYSTEM_H
#define XSE_CD3D11HLSL_SHADER_SYSTEM_H

#include "XSEIShaderSystem.h"
#include "XSED3D11Common.h"
#include "XST/XSTCString.h"

#if defined (XST_WINDOWS)
namespace XSE
{
	namespace D3D11
	{
		using namespace XSE::Resources;

		class CHLSLShaderSystem : public IShaderSystem
		{

			public:

				XST_ALIGN(16) union UConstantValue
				{
					f32		float4x4[16];
					f32		float4[4];
					f32		float3[3];
					f32		float2[2];
					f32		float1[1];
					i32		int1[1];
				};

				struct SConstantBuffer
				{
					XST::xst_aname	strName;
					ID3D11Buffer*	pBuffer = xst_null;
					xst_unknown		pData = xst_null;
					u32				uDataSize = 0;
					u32				uRegister = 0;
				};

				typedef xst_vector< UConstantValue >	ConstantValueVec;
				typedef xst_vector< f32 >				FloatVec;

			public:

										CHLSLShaderSystem();
				virtual					~CHLSLShaderSystem();

						i32				Init(IRenderSystem* pRS);

						void			Destroy();

						IVertexShader*	CreateVertexShader(IInputLayout* pIL, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator);
						IPixelShader*	CreatePixelShader(XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator);

						IVertexShader*	CreateDefaultVertexShader(xst_unknown pOptions, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator);
						IPixelShader*	CreateDefaultPixelShader(xst_unknown pOptions, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator);


						i32				CompileVertexShader(Resources::IVertexShader* pVS);
						i32				CompilePixelShader(Resources::IPixelShader* pPS);

						i32				CompileVertexShader(Resources::IVertexShader* pVS, lpcastr lpszShader, ul32 ulShaderSize, lpcastr lpszEntryPoint, SHADER_PROFILE eProfile);
						i32				CompilePixelShader(Resources::IPixelShader* pPS, lpcastr lpszShader, ul32 ulShaderSize, lpcastr lpszEntryPoint, SHADER_PROFILE eProfile);

						void			DestroyVertexShader(Resources::IVertexShader* pVS);
						void			DestroyPixelShader(Resources::IPixelShader* pPS);

						i32				SetVertexShader(const Resources::IVertexShader* pVS);
						i32				SetPixelShader(const Resources::IPixelShader* pPS);

						xst_astring		CreateShaderCode(u32 uiVSInput, u32 uiPSInput);

						//void			SetShaderConstantName(SHADER_CONSTANT eConstant, xst_castring& strName);

						i32				ApplyShaderConstantNames();

						i32				SetConstantValue(u32 uConstant, const Mtx3& value) xst_implement;
						i32				SetConstantValue(u32 uConstant, const Mtx4& value) xst_implement;
						i32				SetConstantValue(u32 uConstant, const f32& value) xst_implement;
						i32				SetConstantValue(u32 uConstant, const Vec2& value) xst_implement;
						i32				SetConstantValue(u32 uConstant, const Vec3& value) xst_implement;
						i32				SetConstantValue(u32 uConstant, const Vec4& value) xst_implement;
						i32				SetConstantValue(u32 uConstant, const i32& value) xst_implement;
						i32				SetConstantValue(u32 uConstant, const bool& value) xst_implement;

						void			UpdateDrawCallInputs() xst_implement;

						void			UpdateFrameInputs() xst_implement;

						void			UpdateMaterialInputs() xst_implement;

						void			SetSceneAmbient(const XST::CColor& Color);

						i32				PrepareResource(Resources::IResource* pRes);

			protected:

						//i32				_AddConstantBuffers(IShader** ppShader, xst_castring* astrCBs, u32 uCBCount);

			protected:

				CRenderSystem*		m_pRS;
				XST::xst_astr16		m_astrProfiles[ ShaderProfiles::_ENUM_COUNT ];
				ID3D11Buffer*		m_apD3DConstantBuffers[ ConstantBuffers::_ENUM_COUNT ];
				SConstantBuffer		m_aConstantBuffers[ ConstantBuffers::_ENUM_COUNT ];
				FloatVec			m_vConstantValues[ ConstantBuffers::_ENUM_COUNT ];
				ConstantValueVec	m_vAllConstantValues;
				ConstantValueVec	m_vUserConstantValues;
		};
	}//d3d11
}//xse
#endif //XST_WINDOWS
#endif