#ifndef XSE_CDEVIL_SYSTEM_H
#define XSE_CDEVIL_SYSTEM_H

#include "XSEIImageSystem.h"

namespace XSE
{
	namespace DevIL
	{
		using namespace Resources;

		class XST_API CImageSystem : public IImageSystem
		{
			friend class CImage;

			public:

												CImageSystem();
				virtual							~CImageSystem();

						i32						Init();

						ImagePtr				CloneImage(const Resources::IImage* pImg);

						i32						SaveImage(xst_castring& strFileName, Resources::IImage* pImg);

						void					ScaleImage(Resources::IImage* pImage, cu32& uiWidth, cu32& uiHeight);

						XST::IAllocator*		CreateMemoryManager(ul32 ulObjCount);
						
						void					DestroyMemoryManager(XST::IAllocator* pAllocator);  
						
						Resources::IResource*	CreateResource(IResourceManager* pCreator, ul32 ulResHandle, xst_castring& strResName, XST::IAllocator* pAllocator);
						
						void					DestroyResource(Resources::IResource* pResource);
						
						u32						GetResourceObjectSize() const;

						bool					IsAutoDestroy() const
												{ return true; }
						
						i32						PrepareResource(Resources::IResource* pResource);

						i32						CreateImageData(Resources::IImage* pImg);

			protected:

						i32				_LoadLibraries();
						void			_UnloadLibraries();
						u32				_GetImageType(u32 uiResourceType);
						u32				_GetImageType(xst_castring& strFileExt);

			protected:

				xst_unknown			m_hDll;
				xst_unknown			m_hILUDll;
				ul32				m_aulExtHashes[ ImageTypes::_ENUM_COUNT ];
				i32					m_aiFormats[ ImageFormats::_ENUM_COUNT ];
				//
				i32					m_aiDataTypes[ ImageDataTypes::_ENUM_COUNT ];
				//Channel count for image formats
				i32					m_aiChannelCounts[ ImageFormats::_ENUM_COUNT ];
				//Data size of the enums
				i32					m_aiDataSizes[ ImageDataTypes::_ENUM_COUNT ];
				//Standard size (in bytes) of the image format
				i32					m_aiFormatSizes[ ImageFormats::_ENUM_COUNT ];
				XST::IAllocator*	m_pMemMgr;
		};
	}//devil
}//xse

#endif