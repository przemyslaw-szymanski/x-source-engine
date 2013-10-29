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

				virtual i32				Init() = 0;

				virtual IResource*		Load(const XST::CBinaryData& Data) = 0;

				virtual void			Destroy(IResource* pRes) = 0;
		};
	}//resources
}//xse

#endif //XSE_IRESOURCE_LOADER_H