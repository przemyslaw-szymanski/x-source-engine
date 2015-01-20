#include "XSECLuaMaterialParser.h"
#include "../include/XSECMaterial.h"

namespace XSE
{
	CLuaMaterialParser::CLuaMaterialParser()
	{
	}

	CLuaMaterialParser::~CLuaMaterialParser()
	{
	}

	i32 CLuaMaterialParser::Parse(Resources::CMaterial* pMat, XST::CLuaTable* pTable)
	{
		//pMat->m_strResourceName = pTable->GetTableName();
		XST_SET_DBG_NAME( pMat, pTable->GetTableName().c_str() );
		
		/*_SetColor( &pMat->m_vecAmbient, pTable->GetChildTable( "ambient" ) );
		_SetColor( &pMat->m_vecDiffuse, pTable->GetChildTable( "diffuse" ) );
		_SetColor( &pMat->m_vecSpecular, pTable->GetChildTable( "specular" ) );*/

		//Get textures
		XST::CLuaTable* pTmpTable = pTable->GetChildTable( "textures" );
		if( pTmpTable )
		{
			XST::CLuaTable::ChildTableItr TableItr = pTmpTable->GetChildTableMap().begin();
			for(; TableItr != pTmpTable->GetChildTableMap().end(); ++TableItr)
			{
				_AddTexture( pMat, pTmpTable );
			}
		}

		return RESULT::OK;
	}

	void CLuaMaterialParser::_AddTexture(Resources::CMaterial* pMat, XST::CLuaTable* pTable)
	{
		if( !pTable )
			return;

		lpcastr lpszName = pTable->GetTableName().data();
		XST::CLuaTable::SString strFileName;
		pTable->GetString( "file", &strFileName );
	}

	void CLuaMaterialParser::_SetColor(XST::Math::CVector3* pvecOut, XST::CLuaTable* pTable)
	{
		if( pTable )
		{
			pTable->GetFloat( "r", &pvecOut->x );
			pTable->GetFloat( "g", &pvecOut->y );
			pTable->GetFloat( "b", &pvecOut->z );
		}
	}

}//xse