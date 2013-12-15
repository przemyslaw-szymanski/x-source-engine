#ifndef XSE_CCGSHADER_SYSTEM_H
#define XSE_CCGSHADER_SYSTEM_H

#if defined(XSE_CG)

#include "XSECommon.h"
#include "XSEIShaderSystem.h"

#include <CG/cg.h>

namespace XSE
{
	#define CHECK_CG_ERR(_situation, _shaderName)	if( XST_FAILED( this->CheckForErrors( _situation, _shaderName ) ) ) { return XST_FAIL; }

	class XST_API CCGShaderSystem : public IShaderSystem
	{
		public:

									CCGShaderSystem();
			virtual					~CCGShaderSystem();

			virtual	i32				Init(IRenderSystem* pRS);

			virtual void			Destroy();

			virtual	Resources::IVertexShader*	CreateVertexShader(IInputLayout* pIL, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator);
			virtual	Resources::IPixelShader*	CreatePixelShader(XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator);

			virtual	Resources::IVertexShader*	CreateDefaultVertexShader(xst_unknown pOptions, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator) { return xst_null; }
			virtual	Resources::IPixelShader*	CreateDefaultPixelShader(xst_unknown pOptions, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator) { return xst_null; }

			virtual	i32				CompileVertexShader(Resources::IVertexShader* pVS);
			virtual	i32				CompilePixelShader(Resources::IPixelShader* pPS);

			virtual	void			DestroyVertexShader(Resources::IVertexShader* pVS);
			virtual	void			DestroyPixelShader(Resources::IPixelShader* pPS);

			virtual	i32				SetVertexShader(Resources::IVertexShader* pVS);
			virtual	i32				SetPixelShader(Resources::IPixelShader* pPS);

					i32				CompileVertexShader(lpcastr lpszShaderCode, ul32 ulCodeSize, Resources::IVertexShader* pShader);
					i32				CompilePixelShader(lpcastr lpszShaderCode, ul32 ulCodeSize, Resources::IPixelShader* pShader);

					i32				LoadLibraries();
					void			UnloadLibraries();

					i32				CheckForErrors(lpcastr lpszSituation, lpcastr lpszShaderName = "unknown" );

					CGprogram		CreateProgram(lpcastr lpszShaderCode, CGprofile Profile, lpcastr lpszEntryPoint, lpcastr lpszShaderName);

					CGprofile		GetCGProfile(xst_castring& strProfile);

			virtual void			SetConstantBuffers();

			virtual void			SetWorldMatrix(const Mtx4& mtxWorld)
									{ m_mtxWorld = mtxWorld; }

			virtual void			SetViewMatrix(const Mtx4& mtxView)
									{ m_mtxView = mtxView; }

			virtual void			SetProjectionMatrix(const Mtx4& mtxProj)
									{ m_mtxProj = mtxProj; }

			virtual void			SetViewProjectionMatrix(const Mtx4& mtxViewProj)
									{ m_mtxViewProj = mtxViewProj; }

			virtual	void			SetWorldViewProjectionMatrix(const Mtx4& mtxWVP)
									{ m_mtxWVP = mtxWVP; }

			virtual i32				PrepareResource(Resources::IResource* pRes);

		protected:

			xst_unknown		m_hDll;
			CGcontext		m_CGContext;
			IRenderSystem*	m_pRS;
			CGparameter		m_CGMtxWorldParam;
			CGparameter		m_CGMtxViewParam;
			CGparameter		m_CGMtxProjParam;
			CGparameter		m_CGMtxViewProjParam;
			CGparameter		m_CGMtxWVPParam;
			Mtx4			m_mtxWorld;
			Mtx4			m_mtxView;
			Mtx4			m_mtxProj;
			Mtx4			m_mtxViewProj;
			Mtx4			m_mtxWVP;
	};

}//xse
#endif // XSE_CG

#endif