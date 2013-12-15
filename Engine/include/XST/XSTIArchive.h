#ifndef XST_IARCHIVE_H
#define XST_IARCHIVE_H

#include "XSTCommonTypes.h"

namespace XST
{
	using namespace XST::Types;
	class IArchive
	{
		public:

			virtual				~IArchive() {}

			virtual	i32			Open(xst_castring& strArchiveName) = 0;
			virtual void		Close() = 0;
			virtual bool		IsOpened() = 0;
			virtual bool		IsLoaded() = 0;
			virtual i32			Compress(u8* pDataOut, cul32& ulDataSize, cu8* pCompressedData, cul32& ulCompressedDataSize) = 0;
			virtual i32			Uncompress(u8* pDataOut, cul32& ulDataSize, cu8* pCompressedData, cul32& ulCompressedDataSize) = 0;
			virtual ul32		GetCompressSize(cu8* pData, cul32& ulDataSize) = 0;
	};

}//xst

#endif