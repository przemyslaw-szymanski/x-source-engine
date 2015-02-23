#include "XSECTextureManager.h"
#include "XSECTexture.h"
#include "XSEIRenderSystem.h"
#include "XSECFile.h"
#include "XSECResourceFileManager.h"

namespace XSE
{
	STextureDesc g_aDescs[ 8 ];

	CTextureManager::CTextureManager()
	{

	}

	CTextureManager::~CTextureManager()
	{

	}

	static const ul32 g_DDSHash = XST::CHash::GetCRC( "dds" );

	i32 CTextureManager::PrepareResource(ResourceWeakPtr pRes)
	{
		xst_assert2( pRes.IsValid() );
		xst_assert2( m_pRS );

		auto& Desc = g_aDescs[ 0 ];
		xst_zero( &Desc, sizeof(STextureDesc) );
		
		Resources::CTexture* pTex = (Resources::CTexture*) pRes.GetPtr();
		const Resources::CFile* pFile = pTex->m_pFile.GetPtr();
		if( pFile )
		{
			CResourceFileManager::SFileInfo Info;
			if( !XST_FAILED( m_pResFileMgr->GetFileInfo( pTex->m_pFile, &Info ) ) )
			{
				if( g_DDSHash == Info.uExtHash )
					Desc.bCompressed = true;
				Desc.pData = pFile->GetData().GetPointer();
				Desc.uDataSize = pFile->GetData().GetSize();
				Desc.bRawData = true;
				RSHandle hTex = m_pRS->CreateTexture( Desc );
				if( hTex.uHandle )
				{
					pTex->_SetRenderSystemHandle( hTex );
					pTex->_SetResourceState( Resources::ResourceStates::PREPARED );
					pTex->m_pResourceFile = xst_null;
					pTex->m_pFile = xst_null;
				}
				else
				{
					return XST_FAIL;
				}
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
		Resources::CTexture* pTex = xst_new Resources::CTexture( m_pRS, this, ulHandle, strName, ResourceTypes::TEXTURE, Resources::ResourceStates::CREATED, xst_null );
		if( !pTex )
		{
			XST_LOG_ERR( "Unable to create texture: '" << strName << "'. Memory error." );
			return xst_null;
		}

		return pTex;
	}
} // xse