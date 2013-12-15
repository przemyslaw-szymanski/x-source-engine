#ifndef XSE_CLUA_MATERIAL_READER_H
#define XSE_CLUA_MATERIAL_READER_H

#include "../include/XSEIMaterialReader.h"

namespace XSE
{
	class CLuaMaterialReader : public IMaterialReader
	{
		public:

								CLuaMaterialReader();
			virtual				~CLuaMaterialReader();

			virtual	i32			Read(Resources::CMaterial* pMat, xst_unknown pData, cul32& ulDataSize);

		protected:
	};

}//xse

#endif