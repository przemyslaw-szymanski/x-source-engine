#ifndef XSE_CGEOMETRY_BATCH_H
#define XSE_CGEOMETRY_BATCH_H

#include "XSEIRenderableObject.h"

namespace XSE
{
	class IRenderSystem;

	class CGeometryBatch : public IRenderableObject
	{
		friend class CSceneManager;

		public:

			typedef i32 (CGeometryBatch::*pfnRender)();

		public:

									CGeometryBatch(IRenderSystem* pRS, CSceneManager* pMgr, lpcastr strDbgName);
			virtual					~CGeometryBatch();

					void			SetVisible(bool bVisible);

					void			DisableObject(cu32& uiReason);

		protected:

					i32				_RenderNormal();

					i32				_RenderEmpty() { return XST_OK; }

		protected:

			pfnRender				m_RenderMethod;
			CSceneManager*			m_pSceneMgr;
	};

}//xse

#endif //XSE_CGEOMETRY_BATCH_H