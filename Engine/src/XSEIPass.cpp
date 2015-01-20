#include "XSEIPass.h"

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

}//xse