#ifndef XSE_CTEXTURE_MANAGER_H
#define XSE_CTEXTURE_MANAGER_H

#include "XSEIResourceManager.h"
#include "XSECTexture.h"

namespace XSE
{
	class XST_API CTextureManager : public XST::TCSingleton< CTextureManager >, public IResourceManager
	{
		friend class CEngine;

		public:

										CTextureManager();
				virtual					~CTextureManager();

				i32						PrepareResource(ResourceWeakPtr *const ppRes) xst_implement;

				xst_fi IRenderSystem*	GetRenderSystem() const
										{ return m_pRS; }

		protected:

				Resources::IResource*	_CreateResource(xst_castring& strName, const ResourceHandle& ulHandle, GroupWeakPtr pGroup) xst_implement;

				i32						_CreateMemoryPool(cul32& ulObjCount, XST::IAllocator* pAllocator = xst_null)
										{ return 0; }

				void					_SetRenderSystem(IRenderSystem* pRS)
										{ m_pRS = pRS; }

		protected:

			IRenderSystem*		m_pRS = xst_null;
	};
};

#endif