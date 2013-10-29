#ifndef XSE_CGUI_MANAGER_H
#define XSE_CGUI_MANAGER_H

#include "XSEGUICommon.h"

namespace XSE
{
	namespace GUI
	{
		class IGUIRenderer;

		class XST_API CGUIManager : public XST::TCSingleton< CGUIManager >
		{
			public:

				typedef xst_vector< CComponent* >	CompVec;

			public:

										CGUIManager();
				virtual					~CGUIManager();

						i32				Init(IRenderSystem* pRS, CSceneManager* pSceneMgr, CMeshManager* pMeshMgr);

				xst_fi	CComponent*		GetMainComponent()
										{ return m_pMainComp; }

						i32				Build(IInputLayout* pIL, xst_castring& strGroup);

				xst_fi	IGUIRenderer*	GetRenderer()
										{ return m_pRenderer; }


			protected:

				CompVec			m_vComps;
				IGUIRenderer*	m_pRenderer;
				CComponent*		m_pMainComp;
		};
	}//gui
}//xse

#endif