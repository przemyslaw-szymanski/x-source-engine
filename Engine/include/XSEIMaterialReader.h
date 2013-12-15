#ifndef XSE_IMATERIAL_READER_H
#define XSE_IMATERIAL_READER_H

#include "XSECommon.h"

namespace XSE
{
	namespace Resources
	{
		class CMaterial;
	}

	class IMaterialReader
	{
		public:

			virtual			~IMaterialReader() {}

			virtual	i32		Read(Resources::CMaterial* pMat, xst_unknown pData, cul32& ulDataSize) = 0;

		protected:
	};

}//xse

#endif