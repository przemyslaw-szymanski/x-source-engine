#ifndef XSE_CENGINE_H
#define XSE_CENGINE_H

#include "XSECommon.h"
#include "XSEIRenderSystem.h"

namespace XSE
{
	struct XST_API SFeatures
	{
		bool	bSSE;
		bool	bSSE2;
		bool	bSSE3;
		bool	bSSE4;
		bool	bSSE41;
		bool	bSSE42;
		bool	bSSE42a;
		bool	bSSE5;
		bool	bMMX;
		bool	b3DNow;
		bool	b3DNowExt;
		u32		uiCoreCount;
		
		struct
		{
			bool	bDirect3D9;
			bool	bDirect3D10;
			bool	bDirect3D11;
			bool	bOpenGL;
		}		RenderSystem;

		u32		uiCpuSpeed;
		u32		uiMemorySize;

		RSFeatures	RSFeatures;

	};

	class XST_API CEngineOptions
	{
		public:
			CEngineOptions() : strRenderSystem( "" ), strRenderSystemPlugin( "" ) {}

			xst_astring strRenderSystem;
			xst_astring strRenderSystemPlugin;
			SRenderSystemOptions	RSOptions;
	};

	class XST_API CEngine : public XST::TCSingleton< CEngine >, public XST::IObject
	{
		public:

			xst_tmapx( xst_astring, IRenderWindow* )		WindowMap;
			typedef XST::TCPairIterator< WindowMap >		WindowIterator;
			xst_tmapx( xst_astring, CSceneManager* )		SceneMgrMap;
			typedef SceneMgrMap::iterator					SceneIterator;

			static xst_castring		DIRECT3D11;
			static xst_castring		DIRECT3D9;
			static xst_castring		BEST_RENDER_SYSTEM;
			static CEngineOptions	DEFAULT_OPTIONS;

		public:

									CEngine();
			virtual					~CEngine();

					i32				Init(const CEngineOptions& Options = DEFAULT_OPTIONS);

					void			Update(cf32& fFrameTime);

					IRenderWindow*	CreateRenderWindow(xst_castring& strName, u32 uiExternalHandle = 0);

					IRenderWindow*	GetRenderWindow(xst_castring& strName);
					IRenderWindow*	GetRenderWindow(cul32& ulHWND);
					
					void			DestroyWindows();

					WindowIterator	GetWindowIterator();

					bool			CheckCaps();

					SFeatures		GetCaps();

			xst_fi	IRenderSystem*	GetRenderSystem()
									{ return m_pRenderSystem; }

					CSceneManager*	CreateSceneManager(xst_castring& strName, f32 fSceneSize = 0, bool bSetAsCurrent = false);
					
					i32				SetSceneManager(xst_castring& strName);
					i32				SetSceneManager(CSceneManager* pSceneMgr);

					void			DestroySceneManager(xst_castring& strName);
					void			DestroySceneManager(CSceneManager* pSceneMgr);

			xst_fi	CSceneManager*	GetSceneManager(xst_castring& strName)
									{ return m_mScenes[ strName ]; }

					IRenderSystem*	CreateRenderSystem(xst_castring& strName);

					IRenderSystem*	CreateRenderSystemFromPlugin(xst_castring& strPluginName);

					i32				RegisterShaderSystem(xst_castring& strName);
					i32				RegisterShaderSystem(SHADER_LANGUAGE eLang);

					i32				SetShaderSystem(SHADER_LANGUAGE eLang);
					i32				SetShaderSystem(xst_castring& strName);

			xst_fi	const SFeatures&	GetFeatures() const
										{ return m_Features; }

			xst_fi	CLuaScriptManager*	GetLuaManager() const
										{ return m_pLuaScriptMgr; }

			xst_fi	CMaterialManager*	GetMaterialManager() const
										{ return m_pMatMgr; }

			xst_fi	CResourceFileManager*	GetResourceFileManager() const
											{ return m_pResFileMgr; }

			xst_fi	CTextureManager*	GetTextureManager() const
										{ return m_pTexMgr; }

			xst_fi	CShaderManager*		GetShaderManager() const
										{ return m_pShaderMgr; }

			xst_fi	CMeshManager*		GetMeshManager() const
										{ return m_pMeshMgr; }

			xst_fi	CImageManager*		GetImageManager() const
										{ return m_pImgMgr; }

			xst_fi	CSceneManager*		GetSceneManager()
										{ xst_assert( m_pCurrentScene, "(CEngine::GetSceneManager) SceneManager is not set" ); return m_pCurrentScene; }

					i32					SetScreenResolution(u32 uiWidth, u32 uiHeight, u32 uiRefreshRate);

		protected:

					void				_DestroySceneManager(CSceneManager* pMgr);

		protected:

			CLuaScriptManager*		m_pLuaScriptMgr;
			CMaterialManager*		m_pMatMgr;
			XST::CFileManager*		m_pFileMgr;
			CResourceFileManager*	m_pResFileMgr;
			CTextureManager*		m_pTexMgr;
			CModelManager*			m_pModelMgr;
			CShaderManager*			m_pShaderMgr;
			CRenderSystemFactory*	m_pRenderSystemFactory;
			CMeshManager*			m_pMeshMgr;
			CImageManager*			m_pImgMgr;

			WindowMap				m_mWindows;
			IRenderWindow*			m_pCurrRenderWindow;
			SceneMgrMap				m_mScenes;
			CSceneManager*			m_pCurrentScene;

			IRenderSystem*			m_pRenderSystem;
			xst_unknown				m_hRenderSystemDll;

			SFeatures				m_Features;
	};

}//xse

#endif