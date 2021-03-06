#ifndef XSE_IRESOURCE_LOADER_H
#define XSE_IRESOURCE_LOADER_H

#include "XSEIResource.h"
#include "XST/XSTCBinaryData.h"

namespace XSE
{
	namespace Resources
	{
		class IResourceLoader
		{
			public:

				virtual					~IResourceLoader() {}

				virtual i32				Init(xst_unknown pData) = 0;

				virtual i32				Load(IResource** ppResOut, cu8* pData, cul32 uDataSize) = 0;

				virtual void			Destroy(IResource* pRes) = 0;

				virtual bool			IsManualDestroy() const { return false; }
		};
	}//resources
}//xse

#endif //XSE_IRESOURCE_LOADER_H