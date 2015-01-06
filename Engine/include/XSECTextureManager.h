#ifndef XSE_CTEXTURE_MANAGER_H
#define XSE_CTEXTURE_MANAGER_H

#include "XSEIResourceManager.h"

namespace XSE
{
	class CTextureManager : public XST::TCSingleton< CTextureManager >, public IResourceManager
	{
		public:

				i32						PrepareResource(ResourceWeakPtr pRes) xst_implement { return 0; }

		protected:

				Resources::IResource*	_CreateResource(xst_castring& strName, const ResourceHandle& ulHandle, GroupWeakPtr pGroup) xst_implement
										{ return xst_null; }

				i32						_CreateMemoryPool(cul32& ulObjCount, XST::IAllocator* pAllocator = xst_null)
										{ return 0; }
	};
};

#endif