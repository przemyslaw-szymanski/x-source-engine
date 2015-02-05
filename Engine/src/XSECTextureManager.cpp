#include "XSECTextureManager.h"
#include "XSECTexture.h"
#include "XSEIRenderSystem.h"

namespace XSE
{
	using namespace XSE::Resources;

	STextureDesc g_aDescs[ 8 ];

	CTextureManager::CTextureManager()
	{

	}

	CTextureManager::~CTextureManager()
	{

	}

	i32 CTextureManager::PrepareResource(ResourceWeakPtr pRes)
	{
		xst_assert2( pRes.IsValid() );
		xst_assert2( m_pRS );

		auto& Desc = g_aDescs[ 0 ];
		xst_zero( &Desc, sizeof(STextureDesc) );
		
		CTexture* pTex = (CTexture*) pRes.GetPtr();
		const XST::Resources::CFile* pFile = pTex->GetResourceFile().GetPtr();
		if( pFile )
		{
			xst_castring& strExt = pFile->GetExtension();
			if( strExt == "dds" )
				Desc.bCompressed = true;
			Desc.pData = pFile->GetData().GetPointer();
			Desc.uDataSize = pFile->GetSize();
			Desc.bRawData = true;
			RSHandle hTex = m_pRS->CreateTexture( Desc );
			if( hTex.uHandle )
			{
				pTex->_SetRenderSystemHandle( hTex );
				pTex->_SetResourceState( ResourceStates::PREPARED );
				pTex->m_pResourceFile = xst_null;
			}
			else
			{
				return XST_FAIL;
			}
		}
		else
		{
			XST_LOG_ERR( "No file loaded for texture resource." );
			return XST_FAIL;
		}

		return XST_OK;
	}

	Resources::IResource* CTextureManager::_CreateResource( xst_castring& strName, const ResourceHandle& ulHandle, GroupWeakPtr pGroup )
	{
		CTexture* pTex = xst_new CTexture( m_pRS, this, ulHandle, strName, ResourceTypes::TEXTURE, ResourceStates::CREATED, xst_null );
		if( !pTex )
		{
			XST_LOG_ERR( "Unable to create texture: '" << strName << "'. Memory error." );
			return xst_null;
		}

		return pTex;
	}
} // xse