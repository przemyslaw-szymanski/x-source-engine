#ifndef XSE_IMODEL_LOADER_H
#define XSE_IMODEL_LOADER_H

#include "XSEIResourceLoader.h"
#include "XSTCBinaryData.h"

namespace XSE
{
	class Resources::CModel;

	class IModelLoader : public Resources::IResourceLoader
	{
		public:

			virtual			~IModelLoader() {}

			virtual i32		Init(xst_unknown pData) { return XST_OK; }

			virtual i32		Load(Resources::IResource** ppResOut, const XST::CBinaryData& Data) xst_implement
							{ return LoadModel( (Resources::CModel**)ppResOut, Data ); }
							
			virtual void	Destroy(Resources::IResource* pRes) xst_implement
							{ DestroyModel( (Resources::CModel*)pRes ); }

			virtual void	DestroyModel(Resources::CModel* pModel) { return; }
			virtual i32		LoadModel(Resources::CModel** ppModelOut, const XST::CBinaryData& Data) = 0;
	};
} // xse

#endif //XSE_IMODEL_LOADER_H