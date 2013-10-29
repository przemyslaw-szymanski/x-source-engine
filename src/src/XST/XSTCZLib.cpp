#include "../../include/XST/XSTCZLib.h"
#include "../../include/XST/XSTPlatform.h"
#include "../../include/XST/XSTCommonInclude.h"
#include "../../include/XST/XSTCLogger.h"
#include "zlib.h"

/*ZEXTERN int ZEXPORT compress OF((Bytef *dest,   uLongf *destLen,
                                 const Bytef *source, uLong sourceLen));*/

namespace XST
{
	using namespace XST::Types;

	typedef int(*PFNUNCOMPRESS)(Bytef *, uLongf *, const Bytef *, uLong);
	typedef uLong(*PFNCOMPRESSBOUND)(uLong);

	PFNUNCOMPRESS ZLibUncompress;
	PFNUNCOMPRESS ZLibCompress;
	PFNCOMPRESSBOUND ZLibCompressBound;

	CZLib::CZLib(lpcastr lpszLibFile) : m_hDll( 0 )
	{
		m_hDll = XST::Platform::LoadLibrary( lpszLibFile );
		if( m_hDll )
		{
			ZLibCompress = (PFNUNCOMPRESS)XST::Platform::GetProcAddress( m_hDll, "compress" );
			ZLibUncompress = (PFNUNCOMPRESS)XST::Platform::GetProcAddress( m_hDll, "uncompress" );
		}
		else
		{
			XST_LOG_ERR( "Could not load library: " << lpszLibFile );
		}
	}

	CZLib::~CZLib()
	{
		XST::Platform::CloseLibrary( m_hDll );
	}

	i32			CZLib::Open(xst_castring& strArchiveName)
	{
		if( m_hDll != xst_null )
		{
			return RESULT::OK;
		}
		
		return RESULT::FAILED;
	}

	void		CZLib::Close() 
	{
	}

	bool		CZLib::IsOpened()
	{
		return m_hDll != xst_null;
	}

	bool		CZLib::IsLoaded() 
	{
		return m_hDll != xst_null;
	}

	i32			CZLib::Compress(u8* pDataOut, cul32& ulDataSize, cu8* pCompressedData, cul32& ulCompressedDataSize) 
	{
		return ZLibCompress( (Bytef*)pDataOut, (uLongf*)&ulDataSize, (Bytef*)pCompressedData, (uLong)ulCompressedDataSize );
	}

	i32			CZLib::Uncompress(u8* pDataOut, cul32& ulDataSize, cu8* pCompressedData, cul32& ulCompressedDataSize) 
	{
		i32 iResult = ZLibUncompress( (Bytef*)pDataOut, (uLongf*)&ulDataSize, (Bytef*)pCompressedData, (uLong)ulCompressedDataSize );
		switch( iResult )
		{
			case Z_MEM_ERROR: XST::CLastError::Set( "Not enough memory to uncompress data" ); break;
			case Z_BUF_ERROR: XST::CLastError::Set( "Too small output buffer" ); break;
			case Z_DATA_ERROR: XST::CLastError::Set( "Corrupted or incomplete input data" ); break;
		}

		return iResult;
	}

	ul32		CZLib::GetCompressSize(cu8* pData, cul32& ulDataSize)
	{
		return ZLibCompressBound( ulDataSize );
	}

}//xst