#include "XSECLuaMaterialReader.h"
#include "XSECLuaMaterialParser.h"

namespace XSE
{
	CLuaMaterialReader::CLuaMaterialReader()
	{
	}

	CLuaMaterialReader::~CLuaMaterialReader()
	{
	}

	i32 CLuaMaterialReader::Read(Resources::CMaterial* pMat, xst_unknown pData, cul32& ulDataSize)
	{
		CLuaMaterialParser Parser;
		XST::CLuaTable* pTable = (XST::CLuaTable*)pData;
		//return Parser.Parse( pMat, pTable );
		return Parser.Parse( pMat, pTable );
	}

}//xse