#include "XSEIPass.h"
#include "XSECTextureManager.h"
#include "XSEIRenderSystem.h"

namespace XSE
{
	/*void IPass::SetVertexShader(Resources::IVertexShader* pShader)
	{
	m_pVS = pShader;
	}

	void IPass::SetPixelShader(Resources::IPixelShader* pShader)
	{
	m_pPS = pShader;
	}*/

	i32 IPass::Compare(const IPass* pOther) const
	{
		if( AttribEquals( m_Attribs, pOther->m_Attribs ) )
			return 0;
		return 1;
	}

	bool IPass::AttribEquals(const SMaterialAttributes& Left, const SMaterialAttributes& Right)
	{
		if( Left.vecDiffuseColor != Right.vecDiffuseColor )
			return false;
		if( Left.vecEmissiveColor != Right.vecEmissiveColor )
			return false;
		if( Left.vecAmbientColor != Right.vecAmbientColor )
			return false;
		if( Left.vecSpecularColor != Right.vecSpecularColor )
			return false;
		if( Left.fShininess != Right.fShininess )
			return false;
		if( Left.fFogStart != Right.fFogStart )
			return false;
		if( Left.fFogEnd != Right.fFogEnd )
			return false;
		if( Left.vecFogColor != Right.vecFogColor )
			return false;
		return true;
	}

	i32	IPass::SetTexture(MATERIAL_TEXTURE_TYPE eType, xst_castring& strName, xst_castring& strGroup)
	{
		return SetTexture( strName, strGroup, &m_apTextures[ eType ] );
	}
			
	i32	IPass::SetTexture(MATERIAL_TEXTURE_TYPE eType, TextureWeakPtr pTex)
	{
		m_apTextures[ eType ] = pTex;
		return XST_OK;
	}

	i32	IPass::SetTexture( xst_castring& strName, xst_castring& strGroup, TexturePtr* ppOut )
	{
		xst_assert2( ppOut );
		TextureWeakPtr pTex = CTextureManager::GetSingletonPtr()->LoadResource( strName, strName, strGroup, true );
		if( pTex.IsValid() )
		{
			( *ppOut ) = pTex;
			return XST_OK;
		}
		return XST_FAIL;
	}

	const RSHandleRef IPass::GetSamplerHandle(const STextureSamplingMode& Mode)
	{
		return CTextureManager::GetSingletonPtr()->GetRenderSystem()->CreateSampler( Mode );
	}

	void IPass::SetTextureSamplingMode(MATERIAL_TEXTURE_TYPE eType, const STextureSamplingMode& Mode)
	{
		SetTextureSamplingMode( eType, GetSamplerHandle( Mode ) );
	}

}//xse