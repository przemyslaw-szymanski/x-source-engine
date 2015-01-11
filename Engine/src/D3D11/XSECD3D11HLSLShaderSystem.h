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

				struct SVSOncePerFrame
				{
					DirectX::XMMATRIX	mtxWorld;
					DirectX::XMMATRIX	mtxView;
					DirectX::XMMATRIX	mtxProj;
					DirectX::XMMATRIX	mtxViewProj;
					Vec2		vecScreenSize;
				};

				struct SPSOncePerFrame
				{
					Vec4		vecSceneAmbient;
				};

				struct SVSOncePerObject
				{
					DirectX::XMMATRIX	mtxWorld;
					DirectX::XMMATRIX	mtxWorldViewProj;
					DirectX::XMMATRIX	mtxWorldInvT; // world inverse transpose
				};

				struct SPSOncePerObject
				{
					DirectX::XMMATRIX	mtxTemp;
				};

				enum CONSTANT_BUFFERS
				{
					CB_VS_ONCE_PER_FRAME,
					CB_VS_ONCE_PER_OBJECT,
					CB_PS_ONCE_PER_FRAME,
					CB_PS_ONCE_PER_OBJECT,
					_CB_COUNT
				};

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

						void			UpdateObjectInputs();

						void			UpdateFrameInputs();

						void			SetSceneAmbient(const XST::CColor& Color);

						i32				PrepareResource(Resources::IResource* pRes);

			protected:

						i32				_AddConstantBuffers(IShader** ppShader, xst_castring& strPerFrameCB, xst_castring& strPerObjCB);

			protected:

				CRenderSystem*		m_pRS;
				XST::xst_astr16		m_astrProfiles[ ShaderProfiles::_ENUM_COUNT ];
				ID3D11Buffer*		m_apD3DConstantBuffers[ _CB_COUNT ];
		};
	}//d3d11
}//xse
#endif //XST_WINDOWS
#endif