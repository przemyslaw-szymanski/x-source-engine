#include "XSECD3D11VertexShader.h"
#include "XSECD3D11RenderSystem.h"

namespace XSE
{
	namespace D3D11
	{
		CVertexShader::CVertexShader(CRenderSystem* pRS, IShaderSystem* pSS, IInputLayout* pIL, XSE_IRESOURCE_DECL_PARAMS) : 
			IVertexShader( pSS, pIL, XSE_IRESOURCE_PARAMS ), 
			IShaderBase( pRS ), 
			m_pD3DVertexShader( xst_null )
		{
		}

		CVertexShader::~CVertexShader()
		{
			
		}

		void CVertexShader::_Destroy()
		{
			xst_release( m_pD3DVertexShader );
			xst_release( this->m_pBlob );
		}

		i32	CVertexShader::Compile(xst_castring& strEntryPoint, SHADER_PROFILE eProfile, xst_castring& strCode)
		{
			if( m_bIsCompiled )
			{
				return XST_OK;
			}

			this->m_strShaderEntryPoint = strEntryPoint;
			this->m_eProfile = eProfile;

			if( XST_FAILED( this->m_pShaderSystem->CompileVertexShader( this, strCode.data(), strCode.size(), strEntryPoint.data(), eProfile ) ) )
			{
				xst_release( this->m_pD3DVertexShader );
				xst_release( this->m_pBlob );
				return XST_FAIL;
			}

			m_bIsCompiled = true;
			return XST_OK;
		}

		i32 CVertexShader::Compile(xst_castring& strEntryPoint, SHADER_PROFILE eProfile)
		{
			if( m_bIsCompiled )
			{
				return XST_OK;
			}

			this->m_strShaderEntryPoint = strEntryPoint;
			this->m_eProfile = eProfile;

			if( XST_FAILED( this->m_pShaderSystem->CompileVertexShader( this ) ) )
			{
				xst_release( this->m_pD3DVertexShader );
				xst_release( this->m_pBlob );
				return XST_FAIL;
			}

			m_bIsCompiled = true;
			return XST_OK;
		}

	}//d3d11
}//xse