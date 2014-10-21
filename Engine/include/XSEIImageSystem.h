#ifndef XSE_IIMAGE_SYSTEM_H
#define XSE_IIMAGE_SYSTEM_H

#include "XSEIImage.h"
#include "XSEIResourceSystem.h"

namespace XSE
{
	class IImageSystem : public IResourceSystem
	{
		friend class CImageManager;
		public:

			virtual							~IImageSystem() {}

			virtual	i32						Init() { return 0; }

			virtual	Resources::IImage*		CreateImage(IResourceManager* pImgMgr, ul32 ulHandle, xst_castring& strName, XST::IAllocator* pAllocator) 
											{ return (Resources::IImage*)CreateResource( pImgMgr, ulHandle, strName, pAllocator ); }

			virtual i32						CloneImage( Resources::IImage** ppDstOut, const Resources::IImage* pSrc, bool bFullClone )
											{ return CloneResource( (Resources::IResource**)ppDstOut, pSrc, bFullClone ); }

			virtual void					DestroyImage(Resources::IImage* pImg)
											{ DestroyResource( pImg ); }

			virtual i32						PrepareImage(Resources::IImage* pImg) 
											{ return PrepareResource( pImg ); }

			virtual i32						SaveImage(xst_castring& strFileName, Resources::IImage* pImg) = 0;
			virtual bool					IsAutoDestroy() const = 0;
			//virtual	XST::IAllocator*		CreateMemoryManager(ul32 ulObjCount) = 0;
			//virtual void					DestroyMemoryManager(XST::IAllocator* pAllocator) = 0;  
			virtual	Resources::IResource*	CreateResource(IResourceManager* pCreator, ul32 ulResHandle, xst_castring& strResName, XST::IAllocator* pAllocator) = 0;
			virtual void					DestroyResource(Resources::IResource* pResource) = 0;
			virtual u32						GetResourceObjectSize() const = 0;
			virtual i32						PrepareResource(Resources::IResource* pResource) = 0;
			virtual i32						CreateImageData(Resources::IImage* pImg) = 0;
			virtual i32						CloneResource(Resources::IResource** ppDstOut, const Resources::IResource* pSrc, bool bFullClone) = 0;

		protected:

	};

}//xse

#endif