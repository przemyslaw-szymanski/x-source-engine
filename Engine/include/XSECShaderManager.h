#ifndef XSE_CSHADER_MANAGER_H
#define XSE_CSHADER_MANAGER_H

#include "XSEIResourceManager.h"
#include "XSEIVertexShader.h"
#include "XSEIPixelShader.h"

namespace XSE
{
	
	class XST_API CShaderManager : public XST::TCSingleton< CShaderManager >, public IResourceManager
	{
		friend class IRenderSystem;
		friend class CEngine;

		protected:

			typedef xst_map< const IInputLayout*, VertexShaderPtr >	VShaderMap;


		public:

											CShaderManager();
			virtual							~CShaderManager();

                    i32						Init() xst_implement;

					i32						PrepareResource(ResourceWeakPtr *const ppRes) xst_implement;

					VertexShaderPtr			CompileVertexShader(xst_castring& strName, xst_castring& strEntryPoint, u32 uInputLayoutElementIds, xst_castring& strCode, xst_castring& strGroupName = DEFAULT_GROUP);

					PixelShaderPtr			CompilePixelShader(xst_castring& strName, xst_castring& strEntryPoint, xst_castring& strCode, xst_castring& strGroupName = DEFAULT_GROUP);

                    VertexShaderPtr			CompileVertexShader(xst_castring& strName, xst_castring& strEntryPoint, const IInputLayout* pIL, xst_castring& strCode, xst_castring& strGroupName = DEFAULT_GROUP);

					VertexShaderPtr			LoadVertexShader(xst_castring& strFileName, xst_castring& strShaderName, xst_castring& strEntryPoint, SHADER_PROFILE eProfile, xst_castring& strGroupName = ALL_GROUPS);

					PixelShaderPtr			LoadPixelShader(xst_castring& strFileName, xst_castring& strShaderName, xst_castring& strEntryPoint, SHADER_PROFILE eProfile, xst_castring& strGroupName = ALL_GROUPS);

                    i32                     LoadShaders(xst_castring& strFileName, xst_castring& strShaderName, xst_castring& strVSEntryPoint, xst_castring& strPSEntryPoint, SHADER_PROFILE eVSProfile, SHADER_PROFILE ePSProfile, xst_castring& strGroupName = ALL_GROUPS);

			xst_fi	VertexShaderPtr			GetDefaultVertexShader()
											{ return m_pDefaultVS; }

					VertexShaderPtr			GetDefaultVertexShader(const IInputLayout* pIL);
					VertexShaderPtr			GetDefaultVertexShader(const Resources::CMesh* pMesh);

			xst_fi	PixelShaderPtr			GetDefaultPixelShader()
											{ return m_pDefaultPS; }

					xst_astring				CreateShaderCode(u32 uiVSInput, u32 uiPSInput);

					void					SetShaderConstantName(SHADER_CONSTANT eConstant, xst_castring& strName);

					i32						ApplyShaderConstantNames();

			xst_fi	IRenderSystem*			GetRenderSystem() const
											{ return m_pRenderSystem; }

		protected:

				Resources::IResource*	_CreateResource(xst_castring& strName, const ResourceHandle& ulHandle, GroupWeakPtr pGroup) xst_implement;

			xst_fi	void				_SetRenderSystem(IRenderSystem* pRS)
										{ m_pRenderSystem = pRS; }

					/*i32					_CreateMemoryPool(cul32& ulObjCount, XST::IAllocator* pAllocator = xst_null);

					i32					_CreateVSMemoryPool(cul32& ulObjCount, XST::IAllocator* pAllocator = xst_null);

					i32					_CreatePSMemoryPool(cul32& ulObjCount, XST::IAllocator* pAllocator = xst_null);*/

					ShaderPtr			_LoadShader(SHADER_TYPE eType, xst_castring& strFileName, xst_castring& strShaderName, xst_castring& strEntryPoint, SHADER_PROFILE eProfile, xst_castring& strGroupName);

					ShaderPtr			_CreateShader(SHADER_TYPE eType, xst_castring& strName, xst_castring& strEntryPoint, xst_castring& strGroupName);

                    ShaderWeakPtr       _CompileShader(SHADER_TYPE eType, SHADER_PROFILE eProfile, xst_castring& strName, xst_castring& strEntryPoint, const IInputLayout* pIL, xst_castring& strCode, xst_castring& strGroupName);

		protected:

				IRenderSystem*		m_pRenderSystem;
				XST::IAllocator*	m_pAllocator;
				SHADER_TYPE			m_eShaderType;
				SHADER_LANGUAGE		m_eShaderLang;
				SHADER_PROFILE		m_eShaderProfile;
				XST::IAllocator*	m_pVSMemMgr;
				XST::IAllocator*	m_pPSMemMgr;
				XST::IAllocator*	m_pGSMemMgr;
				XST::IAllocator*	m_pCSMemMgr;
				PixelShaderPtr		m_pDefaultPS;
				VertexShaderPtr		m_pDefaultVS;
				xst_astring			m_strEntryPoint;
				VShaderMap			m_mDefaultVShaders;
	};

};

#endif