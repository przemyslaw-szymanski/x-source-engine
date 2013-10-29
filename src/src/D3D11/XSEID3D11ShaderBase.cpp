#include "XSEID3D11ShaderBase.h"
#include "XSECD3D11RenderSystem.h"

namespace XSE
{
	namespace D3D11
	{
		ul32 GetShaderFlags(cul32& ulFlags);

		IShaderBase::IShaderBase(CRenderSystem* pRS) :	m_pRS( pRS ), m_pBlob( xst_null ), 
														m_ulFlags( 0 ),	m_bIsCompiled( false )
		{
		}

		IShaderBase::~IShaderBase()
		{
		}

		ul32 GetShaderFlags(cul32& ulFlags)
		{
			ul32 ulShaderFlags = 0;

			if( ulFlags & ShaderFlags::DEBUG )
			{
				ulShaderFlags |= D3D10_SHADER_DEBUG;
			}

			if( ulFlags & ShaderFlags::BACKWARD_COMPAPILITY )
			{
				ulShaderFlags |= D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;
				//ulShaderFlags |= D3D10_SHADER_ENABLE_STRICTNESS;
			}

			return ulShaderFlags;
		}

	}//d3d11
}//xse