#ifndef XSE_IGUI_RENDERER_H
#define XSE_IGUI_RENDERER_H

#include "XSEGUICommon.h"
#include "XSECMaterial.h"
#include "XSECDynamicGeometry.h"

namespace XSE
{
	namespace GUI
	{

		class XST_API IGUIRenderer
		{
			protected:

				typedef xst_vector< CComponent* >					CompVector;
				//typedef xst_map< const MaterialPtr, CompVector >	CompMap;

			public:

									IGUIRenderer(IRenderSystem* pRS, CSceneManager* pSceneMgr, CMeshManager* pMeshMgr);
				virtual				~IGUIRenderer();

				virtual	i32			Init();

				virtual	i32			AddComponent(CComponent* pComp);
									
				virtual	i32			BuildGUI(IInputLayout* pIL, xst_castring& strGroup);

				virtual	void		RenderGUI();

				virtual void		SetSceneManager(CSceneManager* pSM);

				virtual void		SetComponentPosition(const CComponent* pCmp);

			protected:

				void				_OnComponentBuild(xst_unknown pCmp, const CDynamicGeometryObject* pObj);

			protected:

				CompVector			m_vComps;
				IRenderSystem*		m_pRS;
				DynamicGeometryPtr	m_pDynamicGeometry;
				CMeshManager*		m_pMeshMgr;
				CSceneManager*		m_pSceneMgr;
				//CompMap				m_mComps;
				MaterialPtr			m_pDefaultMaterial;
		};

	}//gui
}//xse

#endif