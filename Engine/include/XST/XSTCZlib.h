#ifndef XST_CZLIB_H
#define XST_CZLIB_H

#include "XSTIArchive.h"

namespace XST
{
	class CZLib : public IArchive
	{
		public:

								CZLib(lpcastr lpszLibFile);
			virtual				~CZLib();

			virtual	i32			Open(xst_castring& strArchiveName);
			virtual void		Close();
			virtual bool		IsOpened();
			virtual bool		IsLoaded();
			virtual i32			Compress(u8* pDataOut, cul32& ulDataSize, cu8* pCompressedData, cul32& ulCompressedDataSize);
			virtual i32			Uncompress(u8* pDataOut, cul32& ulDataSize, cu8* pCompressedData, cul32& ulCompressedDataSize);
			virtual ul32		GetCompressSize(cu8* pData, cul32& ulDataSize);

		protected:

			xst_unknown		m_hDll;
	};

}//xst

#endif