#ifndef XSE_CRENDER_SYSTEM_FACTORY_H
#define XSE_CRENDER_SYSTEM_FACTORY_H

#include "XSEIRenderSystem.h"

namespace XSE
{
	typedef IRenderSystem*(*XST_PFNCREATERENDERSYSTEM)();

	class XST_API CRenderSystemFactory : public XST::TCSingleton< CRenderSystemFactory >
	{
		public:

			xst_tmapx( xst_astring, XST_PFNCREATERENDERSYSTEM )	RenderSystemMap;

		public:

			i32					RegisterRenderSystem(xst_castring& strName, XST_PFNCREATERENDERSYSTEM pfnCreateRenderSystem);

			IRenderSystem*		CreateRenderSystem(xst_castring& strName);

		protected:

			RenderSystemMap	m_mRenderSystems;
	};

}//xse

#endif