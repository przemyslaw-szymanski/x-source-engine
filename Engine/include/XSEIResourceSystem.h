#ifndef XSE_IRESOURCE_SYSTEM_H
#define XSE_IRESOURCE_SYSTEM_H

#include "XSECommon.h"

namespace XSE
{
	class IResourceSystem
	{
		friend class IResourceManager;
		public:

			virtual							~IResourceSystem() {}

			virtual i32						Init() { return 0; }
			//virtual	XST::IAllocator*		CreateMemoryManager(ul32 ulObjCount) = 0;
			//virtual void					DestroyMemoryManager(XST::IAllocator* pAllocator) = 0;  
			virtual	Resources::IResource*	CreateResource(IResourceManager* pCreator, ul32 ulResHandle, xst_castring& strResName, XST::IAllocator* pAllocator) = 0;
			virtual void					DestroyResource(Resources::IResource* pResource) = 0;
			virtual i32						PrepareResource(Resources::IResource* pResource) = 0;
			virtual u32						GetResourceObjectSize() const = 0;
			virtual bool					IsAutoDestroy() const = 0;

		protected:
	};
}//xse

#endif