#ifndef XSE_COBJ_LOADER_H
#define XSE_COBJ_LOADER_H

#include "XSEIModelLoader.h"

namespace XSE
{
	class COBJLoader : public IModelLoader
	{
		public:

							COBJLoader();
			virtual			~COBJLoader();

			virtual i32		LoadModel(Resources::CModel** ppOut, cu8* pData, cul32 uDataSize) xst_implement;
	};
} //xse

#endif //XSE_COBJ_LOADER_H