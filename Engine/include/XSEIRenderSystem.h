#ifndef XSE_IRENDER_SYSTEM_H
#define XSE_IRENDER_SYSTEM_H

#include "XSECommon.h"
#include "XSERenderSystemCommon.h"
#include "XSEIShaderSystem.h"
#include "XSEIShader.h"
#include "XSECVertexData.h"
#include "XSECFrustum.h"

namespace XSE
{
	typedef IRenderSystem*(*XSE_PFN_CREATERENDERSYSTEM)();
	typedef void(*XSE_PFN_DESTROYRENDERSYSTEM)(IRenderSystem**);
	typedef const char*(*XSE_PFN_GETRENDERSYSTEMNAME)();

	class IRenderSystem
	{
		friend class CShaderManager;
		friend class CEngine;

		protected:

			typedef xst_map< ul32, IShaderSystem* >		ShaderSystemMap;
			typedef ShaderSystemMap::iterator			_ShaderSystemIterator;

		public:

			struct UpdateOptionsResults
			{
				enum RESULT
				{
					OK = 0,
					FAILED,
					NEED_RESTART,
					_ENUM_COUNT
				};
			};
			typedef UpdateOptionsResults::RESULT	UPDATE_OPTIONS_RESULT;

		public:

										IRenderSystem(xst_castring& strName) : m_strName( strName ) {}
			virtual						~IRenderSystem() {}

			xst_fi	xst_castring&		GetName() const
										{ return m_strName; }

			xst_fi	bool				IsDebugModeEnabled()
										{ return m_Options.bDebugMode; }

			virtual	i32					Init(const SRenderSystemSettings& Options) = 0;
			virtual	i32					BeginRender() = 0;
			virtual i32					EndRender() = 0;
			virtual i32					ClearScene(const XST::CColor& Color) = 0;
			virtual IViewport*			CreateViewport(cu32& uiWidth, cu32& uiHeight) = 0;
			virtual i32					SetViewport(IViewport* pViewport, bool bDeleteOld = true) = 0;
			virtual IViewport*			GetCurrentViewport() const = 0;
			virtual void				DestroyViewport(IViewport* pViewport) = 0;
			virtual IViewport*			GetCurrentViewport() = 0;
			virtual IInputLayout*		BuildInputLayout(ul32 ulElements) = 0;
			virtual Resources::IVertexShader*		CreateVertexShader(IInputLayout* pIL, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator) = 0;
			virtual	Resources::IPixelShader*		CreatePixelShader(XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator) = 0;
			//virtual Resources::IVertexShader*		CreateDefaultVertexShader(xst_unknown pOptions, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator) = 0;
			virtual	Resources::IPixelShader*		CreateDefaultPixelShader(xst_unknown pOptions, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator) = 0;
			virtual RSHandleRef			CreateTexture(const STextureDesc& Desc) = 0;
			virtual i32					DestroyTexture(RSHandlePtr pTexHandle) = 0;
			virtual const RSHandleRef	CreateSampler(const STextureSamplingMode& Mode) = 0;
			virtual	i32					DestroySampler(RSHandlePtr pHandle) = 0;
			virtual void				SetSampler(u32 uSlot, const RSHandleRef Handle) = 0;
			virtual IVertexBuffer*		CreateVertexBuffer() = 0;
			virtual i32					CreateVertexBuffers(XSE::IVertexBuffer* *const paArray, cu32& uVBCount) = 0;
			virtual void				DestroyVertexBuffers(XSE::IVertexBuffer* *const paArray, cu32& uVBCount) = 0;
			virtual IInputLayout*		GetInputLayout(ul32 ulElements) = 0;
			virtual i32					DrawVertexBuffer(const IVertexBuffer* pVB) = 0;
			virtual i32					DrawVertexBuffer(const IVertexBuffer* pVB, const IIndexBuffer* pIB) = 0;
			virtual const StringVector&	GetSupportableVertexShaderProfiles() = 0;
			virtual const StringVector&	GetSupportablePixelShaderProfiles() = 0;
			virtual i32					AddSupportableVertexShaderProfile(xst_castring& strProfile) = 0;
			virtual i32					AddSupportablePixelShaderProfile(xst_castring& strProfile) = 0;
			virtual i32					SetInputLayout(const IInputLayout* pIL) = 0;
			virtual i32					SetVertexBuffer(const IVertexBuffer* pVB) = 0;
			virtual i32					SetVertexBuffer(const IVertexBuffer* pVB, cul32& ulVertexSize, cul32& ulOffset = 0) = 0;
			virtual i32					SetPixelShader(const Resources::IPixelShader* pPS) = 0;
			virtual i32					SetVertexShader(const Resources::IVertexShader* pVS ) = 0;
			virtual i32					SetInputLayoutWithCheck(const IInputLayout* pIL) = 0;
			virtual i32					SetVertexBufferWithCheck(const IVertexBuffer* pVB) = 0;
			virtual i32					SetPixelShaderWithCheck(const Resources::IPixelShader* pPS) = 0;
			virtual i32					SetVertexShaderWithCheck(const Resources::IVertexShader* pVS ) = 0;
			virtual i32					SetIndexBufferWithCheck(const IIndexBuffer* pBuff) = 0;
			virtual void				SetLookAt(const Vec3& vecPosition, const Vec3& vecPoint, const Vec3& vecUp = Vec3::Y) = 0;
			virtual void				SetPerspectiveFOV(cf32& fAngle, cf32& fAspectRation, cf32& fNear, cf32& fFar) = 0;
			virtual void				SetRotationX(cf32& fAngle) = 0;
			virtual void				SetRotationY(cf32& fAngle) = 0;
			virtual void				SetRotationZ(cf32& fAngle) = 0;
			virtual void				SetRotation(cf32& fAngle, const Vec3& vecAxis) = 0;
			virtual void				SetTranslationX(cf32& fOffset) = 0;
			virtual void				SetTranslationY(cf32& fOffset) = 0;
			virtual void				SetTranslationZ(cf32& fOffset) = 0;
			virtual void				SetTranslation(const Vec3& vecOffset) = 0;
			virtual void				SetTranslation(cf32& fX, cf32& fY, cf32& fZ) = 0;
			virtual void				SetRotationX(Mtx4* pMtxOut, cf32& fAngle) = 0;
			virtual void				SetRotationY(Mtx4* pMtxOut, cf32& fAngle) = 0;
			virtual void				SetRotationZ(Mtx4* pMtxOut, cf32& fAngle) = 0;
			virtual void				SetRotation(Mtx4* pMtxOut, cf32& fAngle, const Vec3& vecAxis) = 0;
			virtual void				SetTranslationX(Mtx4* pMtxOut, cf32& fOffset) = 0;
			virtual void				SetTranslationY(Mtx4* pMtxOut, cf32& fOffset) = 0;
			virtual void				SetTranslationZ(Mtx4* pMtxOut, cf32& fOffset) = 0;
			virtual void				SetTranslation(Mtx4* pMtxOut, const Vec3& vecOffset) = 0;
			virtual void				SetTranslation(Mtx4* pMtxOut, cf32& fX, cf32& fY, cf32& fZ) = 0;
			virtual void				SetScale(cf32& fX, cf32& fY, cf32& fZ) = 0 ;
			virtual void				SetScale(const Vec3& vecScale) = 0;
			virtual void				SetScale(Mtx4* pMtxOut, cf32& fX, cf32& fY, cf32& fZ) = 0;
			virtual void				SetScale(Mtx4* pMtxOut, const Vec3& vecScale) = 0;
			virtual void				SetMatrix(const MATRIX_TYPE& eType, const Mtx4& mtx) = 0;
			virtual i32					ResizeBuffers(cu32& uiWidth, cu32& uiHeight) = 0;
			virtual IIndexBuffer*		CreateIndexBuffer() = 0;
			virtual i32					SetIndexBuffer(const IIndexBuffer* pBuff) = 0;
			virtual i32					CreateShaderSystem(xst_castring& strName) = 0;
			virtual i32					CreateShaderSystem(SHADER_LANGUAGE eLanguage) = 0;
			virtual i32					SetShaderSystem(xst_castring& strName) = 0;
			virtual i32					SetShaderSystem(SHADER_LANGUAGE eLanguage) = 0;
			virtual IShaderSystem*		GetShaderSystem() const = 0;
			virtual void				GetMatrix(const MATRIX_TYPE& eMtx, Mtx4* pMtxOut) const = 0;
			virtual xst_fi const RSFeatures&	GetFeatures() const 
												{ return m_Caps.Features; }
			virtual	i32					SetFeatureLevel(SHADER_MODEL eMinLevel, SHADER_MODEL eMaxLevel = ShaderModels::UNKNOWN) = 0;

			virtual	void				Enable2DRendering() = 0;
			virtual	void				Enable3DRendering() = 0;

			virtual CFrustum			CreateViewFrustum() const = 0;

			virtual void				UpdateViewFrustum(CFrustum* pFrustumOut) const = 0;

			virtual void				UpdateVertexBuffer(IVertexBuffer* pVB, cul32& ulStartVertex, cul32& ulVertexCount, const CVertexData& DstData) = 0;

			virtual void				DrawIndexed(cu32& uiIndexCount, cu32& uiStartLocation, ci32& iBaseVertexLocation) = 0;
			virtual void				SetTopology(TOPOLOGY_TYPE eType) = 0;
	
			virtual	xst_fi void			SetShaderConstantName(SHADER_CONSTANT eConstant, xst_castring& strName)
										{ IShaderSystem::SetConstantName( eConstant, strName ); }

			virtual const SRSDiagnostics&		GetDiagnostics() const = 0;

			//Updates any per-object inputs like world matrix for translate/rotate
			//Updates shader states
			virtual	void				UpdateDrawCallInputs() = 0;

			virtual i32					ApplyShaderConstantNames() = 0;

			virtual void				SetSceneAmbient(const XST::CColor& Color) = 0;

			virtual xst_fi const SRSCapapiblities&	GetCaps() const
													{ return m_Caps; }

			//TEMP
			virtual void*				GetDevice() = 0;
			virtual void*				GetDeviceContext() = 0;


			virtual UPDATE_OPTIONS_RESULT				UpdateOptions(const SRenderSystemSettings& Options) = 0;
			virtual UPDATE_OPTIONS_RESULT				UpdateOptions() = 0;

					SRenderSystemSettings&	GetOptions()
											{ return m_Options; }
			
		protected:

					void				_Init(const SRenderSystemSettings& Options)
										{ m_Options = Options; }

					void				_SetShaderSystem(Resources::IShader* pShader, IShaderSystem* pShaderSystem)
										{ pShader->m_pShaderSystem = pShaderSystem; }

			//virtual	XST::IAllocator*	_CreateVertexShaderMemoryPool(cul32& ulObjCount, XST::IAllocator* pAllocator = xst_null) = 0;
			//virtual XST::IAllocator*	_CreatePixelShaderMemoryPool(cul32& ulObjCount, XST::IAllocator* pAllocator = xst_null) = 0;
			virtual	i32					_CreateMemoryPools(void* pMemOptions) = 0;

		protected:

			xst_astring					m_strName;
			SRenderSystemSettings		m_Options;
            SRSCapapiblities			m_Caps;
			ShaderSystemMap				m_mpShaderSystems;
			//RSFeatures					m_Features;
	};

	typedef IRenderSystem::UpdateOptionsResults	RSUpdateOptionsResults;

}//xse

#endif