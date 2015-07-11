#ifndef XSE_ISHADER_COMPILER_H
#define XSE_ISHADER_COMPILER_H

#include "XSECommon.h"
#include "XSERenderSystemCommon.h"
#include "XSEIResourceSystem.h"

namespace XSE
{
	class CSceneManager;

	class XST_API IShaderSystem
	{
		friend class IRenderSystem;

		protected:

			static	xst_astring	CONSTANT_NAMES[ ShaderConstants::_ENUM_COUNT ];

		public:

			struct ShaderCodes
			{
				enum CODE
				{
					//PER_FRAME_ALL_CBUFFER,
					PER_FRAME_VS_CBUFFER,
					PER_DRAWCALL_VS_CBUFFER,
					PER_FRAME_PS_CBUFFER,
					PER_DRAWCALL_PS_CBUFFER,
					PER_MATERIAL_PS_CBUFFER,
					_ENUM_COUNT
				};
			};

		public:

												IShaderSystem(SHADER_LANGUAGE eLang) : m_eLanguage( eLang ) {}
			virtual								~IShaderSystem() {}

			virtual i32							Init(IRenderSystem* pRS) = 0;

			virtual void						Destroy() = 0;

			virtual	Resources::IVertexShader*	CreateVertexShader(IInputLayout* pIL, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator) = 0;
			virtual Resources::IPixelShader*	CreatePixelShader(XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator) = 0;

			//virtual	Resources::IVertexShader*	CreateDefaultVertexShader(xst_unknown pOptions, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator) = 0;
			virtual Resources::IPixelShader*	CreateDefaultPixelShader(xst_unknown pOptions, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator) = 0;

			virtual i32							CompileVertexShader(Resources::IVertexShader* pVS, lpcastr lpszShader, ul32 ulShaderSize, lpcastr lpszEntryPoint, SHADER_PROFILE eProfile) = 0;
			virtual	i32							CompilePixelShader(Resources::IPixelShader* pVS, lpcastr lpszShader, ul32 ulShaderSize, lpcastr lpszEntryPoint, SHADER_PROFILE eProfile) = 0;


			virtual i32							CompileVertexShader(Resources::IVertexShader* pVS) = 0;
			virtual i32							CompilePixelShader(Resources::IPixelShader* pPS) = 0;

			virtual void						DestroyVertexShader(Resources::IVertexShader* pVS) = 0;
			virtual void						DestroyPixelShader(Resources::IPixelShader* pPS) = 0;

			virtual i32							SetVertexShader(const Resources::IVertexShader* pVS) = 0;
			virtual i32							SetPixelShader(const Resources::IPixelShader* pPS) = 0;

			virtual xst_astring					CreateShaderCode(u32 uiVSInput, u32 uiPSInput) = 0;

			static	xst_fi void					SetConstantName(SHADER_CONSTANT eConstant, xst_castring& strName)
												{ CONSTANT_NAMES[ eConstant ] = strName; }

			static	xst_fi	xst_castring&		GetConstantName(SHADER_CONSTANT eConstant)
												{ return CONSTANT_NAMES[ eConstant ]; }

			virtual i32							SetConstantValue(u32 uConstant, const Mtx3& value) = 0;
			virtual i32							SetConstantValue(u32 uConstant, const Mtx4& value) = 0;
			virtual i32							SetConstantValue(u32 uConstant, const f32& value) = 0;
			virtual i32							SetConstantValue(u32 uConstant, const Vec2& value) = 0;
			virtual i32							SetConstantValue(u32 uConstant, const Vec3& value) = 0;
			virtual i32							SetConstantValue(u32 uConstant, const Vec4& value) = 0;
			virtual i32							SetConstantValue(u32 uConstant, const i32& value) = 0;
			virtual i32							SetConstantValue(u32 uConstant, const bool& value) = 0;

			virtual	xst_fi xst_castring&		GetShaderCode(ShaderCodes::CODE eCode)
												{ return m_astrShaderCodes[ eCode ]; }

			virtual	i32							ApplyShaderConstantNames() = 0;

			virtual void						SetSceneAmbient(const XST::CColor& Color) = 0;

			//Updates shader inputs once per draw call
			virtual void						UpdateDrawCallInputs() = 0;

			//Updates shader inputs once per frame only
			virtual	void						UpdateFrameInputs() = 0;

			// Updates shader inputs once per material
			virtual void						UpdateMaterialInputs() = 0;

			virtual SHADER_LANGUAGE				GetType() const
												{ return m_eLanguage; }

			virtual	i32							PrepareResource(Resources::IResource* pRes) = 0;

            virtual xst_fi  void                DoNotValidateNextVertexShaderInput() {}

			/*virtual void			SetWorldMatrix(const Mtx4& mtxWorld) = 0;
			virtual void			SetViewMatrix(const Mtx4& mtxView) = 0;
			virtual void			SetProjectionMatrix(const Mtx4& mtxProj) = 0;
			virtual void			SetViewProjectionMatrix(const Mtx4& mtxViewProj) = 0;
			virtual	void			SetWorldViewProjectionMatrix(const Mtx4& mtxWVP) = 0;*/


		protected:

			SHADER_LANGUAGE	m_eLanguage;
			xst_astring		m_astrShaderCodes[ ShaderCodes::_ENUM_COUNT ];
	};

}//xse

#endif