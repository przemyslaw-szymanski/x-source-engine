#ifndef XSE_CD3D11CG_SHADER_SYSTEM_H
#define XSE_CD3D11CG_SHADER_SYSTEM_H

#include "CG/XSECCGShaderSystem.h"
#include "XSED3D11Common.h"



#if defined (XST_WINDOWS)
#include <CG/cgD3D11.h>
namespace XSE
{
	namespace D3D11
	{
		using namespace XSE::Resources;

		class XST_API CCGShaderSystem : public XSE::CCGShaderSystem
		{
			public:

										CCGShaderSystem(CRenderSystem* pRS);
				virtual					~CCGShaderSystem();

						i32				Init(IRenderSystem* pRS);

						void			Destroy();

						IVertexShader*	CreateVertexShader(IInputLayout* pIL, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator);
						IPixelShader*	CreatePixelShader(XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator);

						IVertexShader*	CreateDefaultVertexShader(xst_unknown pOptions, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator);
						IPixelShader*	CreateDefaultPixelShader(xst_unknown pOptions, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator);

						i32				CompileVertexShader(Resources::IVertexShader* pVS, lpcastr lpszShader, ul32 ulShaderSize, lpcastr lpszEntryPoint, SHADER_PROFILE eProfile);
						i32				CompilePixelShader(Resources::IPixelShader* pVS, lpcastr lpszShader, ul32 ulShaderSize, lpcastr lpszEntryPoint, SHADER_PROFILE eProfile);

						i32				CompileVertexShader(Resources::IVertexShader* pVS);
						i32				CompilePixelShader(Resources::IPixelShader* pPS);

						i32				SetVertexShader(const Resources::IVertexShader* pVS);
						i32				SetPixelShader(const Resources::IPixelShader* pPS);

						xst_astring		CreateShaderCode(u32 uiVSInput, u32 uiPSInput);

						void			UpdateObjectInputs() xst_implement;

						void			UpdateFrameInputs() xst_implement;

						void			UpdateMaterialInputs() xst_implement;

						i32				LoadLibraries();
						void			UnloadLibraries();

						//void			SetShaderConstantName(SHADER_CONSTANT eConstant, xst_castring& strName);
						i32				ApplyShaderConstantNames();

						void			SetSceneAmbient(const XST::CColor& Color) {}

			protected:

				CRenderSystem*		m_pRenderSystem;
				CGcontext			m_CGContext;
				CGprofile			m_CGVertexProfile;
				CGprofile			m_CGPixelProfile;
				xst_unknown			m_hD3D11Dll;
				CGprofile			m_aeProfiles[ ShaderProfiles::_ENUM_COUNT ];
		};
	}//d3d11
}//xse
#endif //XST_WINDOWS
#endif