#ifndef XSE_NIF_LOADER_H
#define XSE_NIF_LOADER_H

#include "../../include/XSEIResourceLoader.h"


namespace XSE
{
	namespace Resources
	{
		class CNIFLoader : public IResourceLoader
		{
			public:

								CNIFLoader();
				virtual			~CNIFLoader();

				i32				Init();

				IResource*		Load(const XST::CBinaryData& Data);

				void			Destroy(IResource* pRes, xst_castring& strResGrpName = ALL_GROUPS);
		};
	}//resources
}//xse

#endif //XSE_NIF_LOADER_H