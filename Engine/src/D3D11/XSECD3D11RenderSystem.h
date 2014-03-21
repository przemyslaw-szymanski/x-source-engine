#ifndef XSE_CD3D11_RENDER_SYSTEM_H
#define XSE_CD3D11_RENDER_SYSTEM_H

#include "XSEIRenderSystem.h"
#include "XSED3D11Common.h"

#if defined (XST_WINDOWS)

namespace XSE
{
	namespace D3D11
	{ 
		
		class CRenderSystem : public XSE::IRenderSystem
		{
			enum
			{
				D3D11,
				D3DX11,
				DXGI,
				LIB_COUNT
			};

			friend class CViewport;
			friend class IShaderBase;
			friend class CVertexShader;
			friend class CPixelShader;
			friend class CGeometryShader;
			friend class CComputeShader;
			friend class CInputLayout;
			friend class CVertexBuffer;
			friend class CIndexBuffer;
			friend class CHLSLShaderSystem;
			friend class CCGShaderSystem;
			friend class CHLSLShaderSystem;
			friend class CRenderThread;

			typedef xst_map< ul32, CInputLayout* >	ILMap;

			//Current used data
			struct SCurrent
			{
				const CInputLayout*	pInputLayout;
				const CVertexBuffer*	pVertexBuffer;
				const CIndexBuffer*	pIndexBuffer;
				const CVertexShader*	pVertexShader;
				const CPixelShader*	pPixelShader;
			};

			public:

				/*struct SMatrixBuffer
				{
					DirectX::XMMATRIX	mtxWorld;
					DirectX::XMMATRIX	mtxView;
					DirectX::XMMATRIX	mtxProj;
					DirectX::XMMATRIX	mtxViewProj;
					DirectX::XMMATRIX	mtxWorldViewProj;
				};*/

				

			public:

										CRenderSystem(xst_castring& strName);
				virtual					~CRenderSystem();

						i32				Init(const SRenderSystemOptions& Options);
						
						i32				BeginRender();
						
						i32				EndRender();
						
						i32				ClearScene(const XST::CColor& Color);
						
						IViewport*		CreateViewport(cu32& uiWidth, cu32& uiHeight);
						
						i32				SetViewport(IViewport* pViewport, bool bDeleteOld);

						IViewport*		GetCurrentViewport() const
										{ return (IViewport*)m_pViewport; }
						
						void			DestroyViewport(IViewport* pViewport);

						IInputLayout*	BuildInputLayout(ul32 ulElements);

						IInputLayout*	GetInputLayout(ul32 ulElements);
						
						IViewport*		GetCurrentViewport()
										{ return (IViewport*)m_pViewport; }

						Resources::IVertexShader*	CreateVertexShader(IInputLayout* pIL, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator);

						Resources::IPixelShader*	CreatePixelShader(XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator);

						Resources::IVertexShader*	CreateDefaultVertexShader(xst_unknown pOptions, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator);
			
						Resources::IPixelShader*	CreateDefaultPixelShader(xst_unknown pOptions, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator);

						IVertexBuffer*	CreateVertexBuffer();

						ul32			GetShaderMaxSize();

						i32				DrawVertexBuffer(const IVertexBuffer* pVB);

						i32				DrawVertexBuffer(const IVertexBuffer* pVB, const IIndexBuffer* pIB);

						const StringVector&	GetSupportableVertexShaderProfiles()
											{ return m_avSupportableProfiles[ SHADER_TYPE_VERTEX ]; }

						const StringVector&	GetSupportablePixelShaderProfiles()
											{ return m_avSupportableProfiles[ SHADER_TYPE_PIXEL ]; }

						i32					AddSupportableVertexShaderProfile(xst_castring& strProfile);

						i32					AddSupportablePixelShaderProfile(xst_castring& strProfile);

						i32					SetInputLayout(const IInputLayout* pIL);

						i32					SetVertexBuffer(const IVertexBuffer* pVB);

						i32					SetVertexBuffer(const IVertexBuffer* pVB, const ul32& ulVertexSize);

						i32					SetPixelShader(const Resources::IPixelShader* pPS);

						i32					SetVertexShader(const Resources::IVertexShader* pVS );

						i32					SetInputLayoutWithCheck(const IInputLayout* pIL);

						i32					SetVertexBufferWithCheck(const IVertexBuffer* pVB);

						i32					SetPixelShaderWithCheck(const Resources::IPixelShader* pPS);

						i32					SetVertexShaderWithCheck(const Resources::IVertexShader* pVS );

						i32					SetIndexBufferWithCheck(const IIndexBuffer* pBuff);

						i32					SetIndexBuffer(const IIndexBuffer* pBuff);

						void				SetLookAt(const Vec3& vecPosition, const Vec3& vecPoint, const Vec3& vecUp = Vec3::Y);

						void				UpdateObjectInputs();

						void				Enable2DRendering();
						void				Enable3DRendering();

						void				SetMatrix(const MATRIX_TYPE& eType, const Mtx4& mtx);

						void				UpdateVertexBuffer(IVertexBuffer* pVB, cul32& ulStartVertex, cul32& ulVertexCount, const CVertexData& DstData);

						IRenderSystem::UPDATE_OPTIONS_RESULT	UpdateOptions(const SRenderSystemOptions& Options);
						IRenderSystem::UPDATE_OPTIONS_RESULT	UpdateOptions();
						
						void				SetPerspectiveFOV(cf32& fAngle, cf32& fAspectRation, cf32& fNear, cf32& fFar);

						void				SetRotationX(cf32& fAngle);
						void				SetRotationY(cf32& fAngle);
						void				SetRotationZ(cf32& fAngle);
						void				SetRotation(cf32& fAngle, const Vec3& vecAxis);

						void				SetTranslationX(cf32& fOffset);
						void				SetTranslationY(cf32& fOffset);
						void				SetTranslationZ(cf32& fOffset);
						void				SetTranslation(const Vec3& vecOffset);
						void				SetTranslation(cf32& fX, cf32& fY, cf32& fZ);

						void				SetRotationX(Mtx4* pMtxOut, cf32& fAngle);
						void				SetRotationY(Mtx4* pMtxOut, cf32& fAngle);
						void				SetRotationZ(Mtx4* pMtxOut, cf32& fAngle);
						void				SetRotation(Mtx4* pMtxOut, cf32& fAngle, const Vec3& vecAxis);

						void				SetTranslationX(Mtx4* pMtxOut, cf32& fOffset);
						void				SetTranslationY(Mtx4* pMtxOut, cf32& fOffset);
						void				SetTranslationZ(Mtx4* pMtxOut, cf32& fOffset);
						void				SetTranslation(Mtx4* pMtxOut, const Vec3& vecOffset);
						void				SetTranslation(Mtx4* pMtxOut, cf32& fX, cf32& fY, cf32& fZ);

						void				SetScale(cf32& fX, cf32& fY, cf32& fZ);
						void				SetScale(const Vec3& vecScale)
											{ SetScale( vecScale.x, vecScale.y, vecScale.z ); }
						void				SetScale(Mtx4* pMtxOut, cf32& fX, cf32& fY, cf32& fZ);
						void				SetScale(Mtx4* pMtxOut, const Vec3& vecScale)
											{ SetScale( pMtxOut, vecScale.x, vecScale.y, vecScale.z ); }

						void				GetMatrix(const MATRIX_TYPE& eMtx, DirectX::XMMATRIX* pMtxOut) const;
						void				GetMatrix(const MATRIX_TYPE& eMtx, Mtx4* pMtxOut) const;
				const	DirectX::XMMATRIX*	GetMatrix(const MATRIX_TYPE& eMtx) const;

						i32					ResizeBuffers(cu32& uiWidth, cu32& uiHeight);

						IIndexBuffer*		CreateIndexBuffer();

						i32					CreateShaderSystem(xst_castring& strName);
						i32					CreateShaderSystem(SHADER_LANGUAGE eLanguage);

						i32					SetShaderSystem(xst_castring& strName);
						i32					SetShaderSystem(SHADER_LANGUAGE eLanguage);

						IShaderSystem*		GetShaderSystem(xst_castring& strName);
						IShaderSystem*		GetShaderSystem(cul32& ulId);
						IShaderSystem*		GetShaderSystem() const
											{ return m_pCurrentShaderSystem; }

						CFrustum			CreateViewFrustum() const;

						void				UpdateViewFrustum(CFrustum* pFrustum) const;

						void*				GetDevice() { return m_pDevice; }
						void*				GetDeviceContext() { return m_pDeviceContext; }

						i32					ApplyShaderConstantNames();

						void				SetSceneAmbient(const XST::CColor& Color)
											{ m_pCurrentShaderSystem->SetSceneAmbient( Color ); }

						void				DrawIndexed(cu32& uiIndexCount, cu32& uiStartLocation, ci32& iBaseVertexLocation);
						
						void				SetTopology(TOPOLOGY_TYPE eType);

						i32					SetFeatureLevel(SHADER_MODEL eMinLevel, SHADER_MODEL eMaxLevel = ShaderModels::UNKNOWN);

				const SRSDiagnostics&		GetDiagnostics() const
											{ return m_Diagnostics; }

			protected:

						void		_UnloadLibraries();
						i32			_LoadLibraries();

						i32			_InitInThread();

						ul32		_GetShaderFlags();
						ul32		_GetShaderFlags(cul32& ulFlags);
						i32			_CompileShaderFromFile(xst_castring& strFileName, IShaderBase* pShader);
						i32			_CompileShader(IShaderBase* pShader, lpcastr lpszProfile);
						i32			_CompileShaderFromMemory(cch8* pData, ul32 ulDataSize, lpcastr lpszProfile, IShaderBase* pShader);
						i32			_CompileShaderFromMemory(lpcastr pData, ul32 ulDataSize, lpcastr lpszShaderName, 
													const D3D10_SHADER_MACRO* pMacro, LPD3D10INCLUDE pInclude, 
													lpcastr lpszEntryPoint, lpcastr lpszProfile, u32 uiFlags1, 
													u32 uiFlags2, ID3DX11ThreadPump* pPump, ID3D10Blob** pBlob, 
													HRESULT* pHR); 
						i32			_CreateVertexShader(CVertexShader* pShader);
						i32			_CreateVertexShader(ID3DBlob* pVSBlob, ID3D11VertexShader** pShader);
						i32			_CreatePixelShader(CPixelShader* pShader);
						i32			_CreateIndexBuffer(CIndexBuffer* pIB);

				//XST::IAllocator*	_CreateVertexShaderMemoryPool(cul32& ulObjCount, XST::IAllocator* pAllocator);
				//XST::IAllocator*	_CreatePixelShaderMemoryPool(cul32& ulObjCount, XST::IAllocator* pAllocator);
						i32			_CreateMemoryPools(void* pMemOptions);

						i32			_CreateVertexBuffer(CVertexBuffer* pVB);
						i32			_CreateInputLayout(CInputLayout* pIL);

						lpcastr		_ErrorToString(HRESULT hr);

						i32			_GetCaps(SRSCapapiblities* pCaps);

			protected:

				xst_unknown				m_ahDlls[ LIB_COUNT ];
				D3D_DRIVER_TYPE			m_eDriverType;
				D3D_FEATURE_LEVEL		m_eFeatureLevel;
				D3D_FEATURE_LEVEL		m_eD3DMinFeatureLevel;
				D3D_FEATURE_LEVEL		m_eD3DMaxFeatureLevel;
				SHADER_MODEL			m_eMinFeatureLevel;
				SHADER_MODEL			m_eMaxFeatureLevel;
				ID3D11Device*			m_pDevice;
				ID3D11DeviceContext*	m_pDeviceContext;
				IDXGISwapChain*			m_pSwapChain;
				ID3D11RenderTargetView*	m_pRenderTargetView;
				CViewport*				m_pViewport;
				IDXGIFactory*			m_pD3DFactory;

				//Default adapter
				IDXGIAdapter*			m_pD3DAdapter;

				ILMap					m_mInputLayouts;

				StringVector			m_avSupportableProfiles[ SHADER_TYPE_COUNT ];
				StringVector			m_avUserProfiles[ SHADER_TYPE_COUNT ];

				IShaderSystem*			m_pCurrentShaderSystem;

				//Current
				SCurrent				m_Current;
				//Diagnostics
				SRSDiagnostics			m_Diagnostics;

				//Format mapping from engine to dx10/11
				DXGI_FORMAT				m_aFormats[ RSFormats::_ENUM_COUNT ];
				//Topology types
				D3D11_PRIMITIVE_TOPOLOGY	m_aeTopologies[ TopologyTypes::_ENUM_COUNT ];
				//Fill modes
				D3D11_FILL_MODE			m_aeFillModes[ FillModes::_ENUM_COUNT ];
				//Cull modes
				D3D11_CULL_MODE			m_aeCullModes[ CullModes::_ENUM_COUNT ];

				//Allocators
				XST::IAllocator*		m_pVSMemMgr;
				XST::IAllocator*		m_pPSMemMgr;
				XST::IAllocator*		m_pVBMemMgr;
				XST::IAllocator*		m_pIBMemMgr;
		};

	}//d3d11
}//xse
#endif //XST_VISUAL_STUDIO
#endif //XSE_CD3D11_RENDER_SYSTEM_H