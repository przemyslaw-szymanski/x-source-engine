#ifndef XSE_CBINARY_MATERIAL_READER_H
#define XSE_CBINARY_MATERIAL_READER_H

#include "../include/XSEIMaterialReader.h"

#define XSE_BIN_MAT_VER_1_0			100
#define XSE_BIN_MAT_VER_HEADER_1_0	"XBM"

namespace XSE
{
	class CBinaryMaterialReader : public IMaterialReader
	{
		public:

			virtual			~CBinaryMaterialReader();

			virtual	i32		Read(Resources::CMaterial* pMat, xst_unknown pData, cul32& ulDataSize);

		protected:

					i32		_Read100(Resources::CMaterial* pMat, XST::CBinaryReader& Reader);
	};

}//xse

#endif