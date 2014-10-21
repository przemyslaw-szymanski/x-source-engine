#ifndef XSE_CIMAGE_MANAGER_H
#define XSE_CIMAGE_MANAGER_H

#include "XSECommon.h"
#include "XSEIResourceManager.h"
#include "XSEIImage.h"

namespace XSE
{
	class XST_API CImageManager : public IResourceManager, public XST::TCSingleton< CImageManager >
	{
		public:

									CImageManager();
			virtual					~CImageManager();

                    i32				Init() xst_implement;    

					/*ImagePtr		CreateImage(xst_castring& strName, xst_castring& strGroupName = DEFAULT_GROUP)
									{ return this->CreateResource( strName, strGroupName ); }*/

					/*i32				LoadImage(ImagePtr pImg, xst_castring& strGroupName = ALL_GROUPS)
									{ return this->LoadResource( pImg, strGroupName ); }*/

					i32				RegisterImageSystem(IImageSystem* pImgSystem, bool bAutoDestroy = true);

					i32				PrepareResource(ResourcePtr pRes) xst_implement;

			virtual	void			DestroyResources() xst_implement;

			/*xst_fi	i32				PrepareImage(ImagePtr pImg)
									{ return PrepareResource( pImg ); }*/

					i32				CreateImageData(ImagePtr pImg);

			xst_fi	IImageSystem*	GetImageSystem() const
									{ return m_pImgSystem; }

		protected:

					Resources::IResource*	_CreateResource(xst_castring& strName, const ResourceHandle& ulHandle, GroupWeakPtr pGroup) xst_implement;
					//i32						_CreateMemoryPool(cul32& ulObjCount, XST::IAllocator* pAllocator = xst_null);

					i32						_CloneResource(Resources::IResource** ppDstOut, const Resources::IResource* pSrc, bool bFullClone) xst_implement;

					void					_OnDestroy() xst_implement;

		protected:

					IImageSystem*	m_pImgSystem;
	};

}//xse

#endif