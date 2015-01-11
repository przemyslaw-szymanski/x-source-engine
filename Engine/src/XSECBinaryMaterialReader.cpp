#include "XSECBinaryMaterialReader.h"
#include "../include/XSECMaterial.h"


#define XSE_BR_CHECK_ERR(_reader) if( _reader.IsReadError() ) { XST_LOG_ERR( "Binary material file read error: EOF" ); return RESULT::FAILED; }

namespace XSE
{
	CBinaryMaterialReader::~CBinaryMaterialReader()
	{
	}

	i32 CBinaryMaterialReader::Read(XSE::Resources::CMaterial *pMat, xst_unknown pData, cul32 &ulDataSize)
	{
		XST::CBinaryReader Reader;
		Reader.SetData( (u8*)pData, ulDataSize, false );

		//Read header name
		ch8 strHeader[ 4 ];
		Reader.ReadChars( (ch8**)&strHeader, 3 );
		strHeader[ 3 ] = 0;
		//Version
		u16 uiVer = Reader.ReadUInt16();
		
		//Check header
		if( strcmp( strHeader, XSE_BIN_MAT_VER_HEADER_1_0 ) != 0 )
		{
			XST_LOG_ERR( "Invalid binary material file header: " << strHeader );
			return RESULT::FAILED;
		}

		//Check version
		if( uiVer == XSE_BIN_MAT_VER_1_0 )
		{
			return _Read100( pMat, Reader );
		}
		else
		{
			XST_LOG_ERR( "Invalid binary material file version: " << uiVer );
			return RESULT::FAILED;
		}
		
		return RESULT::OK;
	}

	i32	CBinaryMaterialReader::_Read100(Resources::CMaterial* pMat, XST::CBinaryReader& Reader)
	{
		//Material name
		//pMat->m_strResourceName = Reader.ReadString();
		XST_SET_DBG_NAME( pMat, Reader.ReadString().c_str() );
		//Ambient color
		pMat->m_vecAmbient.x = Reader.ReadFloat32(); XSE_BR_CHECK_ERR( Reader );
		pMat->m_vecAmbient.y = Reader.ReadFloat32(); XSE_BR_CHECK_ERR( Reader );
		pMat->m_vecAmbient.z = Reader.ReadFloat32(); XSE_BR_CHECK_ERR( Reader );
		//pMat->m_vecAmbient.w = Reader.ReadFloat32(); XSE_BR_CHECK_ERR( Reader );
		//Diffuse color
		pMat->m_vecDiffuse.x = Reader.ReadFloat32(); XSE_BR_CHECK_ERR( Reader );
		pMat->m_vecDiffuse.y = Reader.ReadFloat32(); XSE_BR_CHECK_ERR( Reader );
		pMat->m_vecDiffuse.z = Reader.ReadFloat32(); XSE_BR_CHECK_ERR( Reader );
		//pMat->m_vecDiffuse.w = Reader.ReadFloat32(); XSE_BR_CHECK_ERR( Reader );
		//Specular
		pMat->m_vecSpecular.x = Reader.ReadFloat32(); XSE_BR_CHECK_ERR( Reader );
		pMat->m_vecSpecular.y = Reader.ReadFloat32(); XSE_BR_CHECK_ERR( Reader );
		pMat->m_vecSpecular.z = Reader.ReadFloat32(); XSE_BR_CHECK_ERR( Reader );
		//pMat->m_vecSpecular.w = Reader.ReadFloat32(); XSE_BR_CHECK_ERR( Reader );

		//Texture count
		u16 usTexCount = Reader.ReadUInt16(); XSE_BR_CHECK_ERR( Reader );
		
		xst_astring strTexName;
		//strTexName.resize( 20 );

		for(u16 i = usTexCount; i --> 0;)
		{
			//Texture name
			strTexName = Reader.ReadString(); XSE_BR_CHECK_ERR( Reader );

		}

		//Shaders

		return RESULT::OK;
	}

}//xse