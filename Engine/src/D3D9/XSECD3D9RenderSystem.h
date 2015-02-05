#ifndef XSE_CD3D9_RENDER_SYSTEM_H
#define XSE_CD3D9_RENDER_SYSTEM_H

#include "XSEIRenderSystem.h"

#if defined (XST_WINDOWS)
#include <d3d9.h>
//#include <d3dx9.h>

namespace XSE
{
	namespace D3D9
	{
		class CRenderSystem : public XSE::IRenderSystem
		{
			enum
			{
				D3D9,
				D3DX9,
				LIB_COUNT
			};

			public:

									CRenderSystem(xst_castring& strName);
				virtual				~CRenderSystem();

						i32			Init(const SRenderSystemSettings& Options);

						i32			BeginRender();

						i32			EndRender();

						i32			ClearScene(const XST::CColor& Color);

						IViewport*	CreateViewport(cu32& uiWidth, cu32& uiHeight) { return xst_null; }
						i32			SetViewport(IViewport* pViewport, bool bDeleteOld) { return 1; }
						IViewport*		GetCurrentViewport() const { return 0; }
						void			DestroyViewport(IViewport* pViewport) { return ; }
						IViewport*			GetCurrentViewport() { return xst_null; }
						IInputLayout*		BuildInputLayout(INPUT_LAYOUT_ELEMENT* _aElements, cu32& ulElementCount) { return xst_null; }
						Resources::IVertexShader*	CreateVertexShader(IInputLayout* pIL, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator) { return xst_null; }
						Resources::IPixelShader*	CreatePixelShader(XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator) { return xst_null; }
						Resources::IVertexShader*	CreateDefaultVertexShader(xst_unknown pOptions, XSE::IResourceManager* pResourceMgr,cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator) { return xst_null; }
						Resources::IPixelShader*	CreateDefaultPixelShader(xst_unknown pOptions, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator) { return xst_null; }
						void				GetMatrix(const MATRIX_TYPE& eMtx, Mtx4* pMtxOut) const {}
						IVertexBuffer*	CreateVertexBuffer() { return 0; }
						IInputLayout*		BuildInputLayout(ul32 ulElements) { return 0; }
						IInputLayout*	GetInputLayout(ul32 ulElements) { return 0; }
						i32					DrawVertexBuffer(const IVertexBuffer* pVB) { return 0; }
						i32					DrawVertexBuffer(const IVertexBuffer* pVB, const IIndexBuffer* pIB) { return 0; }
						const StringVector&	GetSupportableVertexShaderProfiles() { return StringVector(); }
						const StringVector&	GetSupportablePixelShaderProfiles() { return StringVector(); }
						i32					AddSupportableVertexShaderProfile(xst_castring& strProfile) { return 0; }
						i32					AddSupportablePixelShaderProfile(xst_castring& strProfile) { return 0; }
						i32					SetInputLayout(const IInputLayout* pIL) { return 0; }
						i32					SetVertexBuffer(const IVertexBuffer* pVB) { return 0; }
						i32					SetVertexBuffer(const IVertexBuffer* pVB, const ul32& ulVertexSize) { return 0; }
						i32					SetPixelShader(const Resources::IPixelShader* pPS) { return 0; }
						i32					SetVertexShader(const Resources::IVertexShader* pVS ) { return 0; }
						void				SetLookAt(const Vec3& vecPosition, const Vec3& vecPoint, const Vec3& vecUp = Vec3::Y) { return; }
						void				GetWorldMatrix(Mtx4* pMtxOut) const {}
						void				GetViewMatrix(Mtx4* pMtxOut) const {}
						void				GetProjectionMatrix(Mtx4* pMtxOut) const {}
						void				GetTransposedWorldMatrix(Mtx4* pMtxOut) const {}
						void				GetTransposedViewMatrix(Mtx4* pMtxOut) const {}
						void				GetTransposedProjectionMatrix(Mtx4* pMtxOut) const {}
						void				GetTransposedViewProjectionMatrix(Mtx4* pMtxOut) const {}
						void				GetTransposedWorldViewProjectionMatrix(Mtx4* pMtxOut) const {}
						void				SetRotationX(cf32& fAngle) {}
						void				SetRotationY(cf32& fAngle) {};
						void				SetRotationZ(cf32& fAngle) {};
						void				SetRotation(cf32& fAngle, const Vec3& vecAxis) {};
						i32					ResizeBuffers( cu32& uiWidth, cu32& uiHeight ) { return 0; }

						void				SetTranslationX(cf32& fOffset) {};
						void				SetTranslationY(cf32& fOffset) {};
						void				SetTranslationZ(cf32& fOffset) {};
						void				SetTranslation(const Vec3& vecOffset) {};
						void				SetTranslation(cf32& fX, cf32& fY, cf32& fZ) {};
						IIndexBuffer*		CreateIndexBuffer() { return 0; }
						i32					SetIndexBuffer(const IIndexBuffer* pBuff) { return 0; }
						i32					CreateShaderSystem(xst_castring& strName) { return 0; }
						i32					CreateShaderSystem(SHADER_LANGUAGE eLang) { return 0; }
						i32					SetShaderSystem(xst_castring& strName) { return 0; }
						i32					SetShaderSystem(SHADER_LANGUAGE eLanguage) { return 0; }
						void				SetSceneAmbient(const XST::CColor& Color) {}
						i32					SetFeatureLevel(SHADER_MODEL eMinLevel, SHADER_MODEL eMaxLevel = ShaderModels::UNKNOWN) { return 0; }
						i32					SetInputLayoutWithCheck(const IInputLayout* pIL) { return 0; }
						i32					SetVertexBufferWithCheck(const IVertexBuffer* pVB) { return 0; }
						i32					SetPixelShaderWithCheck(const Resources::IPixelShader* pPS) { return 0; }
						i32					SetVertexShaderWithCheck(const Resources::IVertexShader* pVS ) { return 0; }
						i32					SetIndexBufferWithCheck(const IIndexBuffer* pBuff) { return 0; }
						void				UpdateDrawCallInputs() {}
						IShaderSystem*		GetShaderSystem() const { return 0; }
						void				Enable2DRendering() {};
						void				Enable3DRendering() {};
						void				UpdateVertexBuffer(IVertexBuffer* pVB, cul32& ulStartVertex, cul32& ulVertexCount, const CVertexData& DstData) {}

						const SRSDiagnostics&		GetDiagnostics() const { return SRSDiagnostics(); }

						CFrustum			CreateViewFrustum() const { return CFrustum(); }

						void				UpdateViewFrustum(CFrustum* pFrustum) const {}

						void				DrawIndexed(cu32& uiIndexCount, cu32& uiStartIndex, ci32& iBeginVertex) {}
						void				SetTopology(TOPOLOGY_TYPE eType) {}

						virtual void*				GetDevice() { return 0; }
						virtual void*				GetDeviceContext() { return 0; }

						IRenderSystem::UPDATE_OPTIONS_RESULT	UpdateOptions(const SRenderSystemSettings& Options) { return IRenderSystem::UpdateOptionsResults::OK; }
						IRenderSystem::UPDATE_OPTIONS_RESULT	UpdateOptions() { return IRenderSystem::UpdateOptionsResults::OK; }
			
						void				SetPerspectiveFOV(cf32& fAngle, cf32& fAspectRation, cf32& fNear, cf32& fFar) {}


						void				SetShaderConstantName(SHADER_CONSTANT eConstant, xst_castring& strName) {}
						i32					ApplyShaderConstantNames() { return XST_OK; }

						ul32		GetShaderMaxSize() { return 0; }

			protected:

						void		_UnloadLibraries();
						i32			_LoadLibraries();

						virtual	XST::IAllocator*	_CreateVertexShaderMemoryPool(cul32& ulObjCount, XST::IAllocator* pAllocator = xst_null) { return 0; }
						virtual XST::IAllocator*	_CreatePixelShaderMemoryPool(cul32& ulObjCount, XST::IAllocator* pAllocator = xst_null) { return 0; }


			protected:

				xst_unknown			m_ahDlls[ LIB_COUNT ];
	
		};

	}//d3d9
}//xse
#endif //XST_VISUAL_STUDIO
#endif //XSE_CD3D11_RENDER_SYSTEM_H