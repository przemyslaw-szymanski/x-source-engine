#ifndef XSE_CTEXTURE_MANAGER_H
#define XSE_CTEXTURE_MANAGER_H

#include "XSEIResourceManager.h"

namespace XSE
{
	class CTextureManager : public XST::TCSingleton< CTextureManager >, public IResourceManager
	{
		public:

				i32						PrepareResource(ResourcePtr pRes) { return 0; }

		protected:

				Resources::IResource*	_CreateResource(xst_castring& strName, cul32& ulHandle, GroupPtr pGroup)
										{ return xst_null; }

				i32						_CreateMemoryPool(cul32& ulObjCount, XST::IAllocator* pAllocator = xst_null)
										{ return 0; }
	};
};

#endif