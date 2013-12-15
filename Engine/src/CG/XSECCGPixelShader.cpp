#include "XSECCGPixelShader.h"
#include "CG/XSECGTypes.h"
#include "CG/XSECCGShaderSystem.h"

namespace XSE
{
	namespace Resources
	{
		CCGPixelShader::CCGPixelShader(IRenderSystem* pRS, IShaderSystem* pSS, XSE_IRESOURCE_DECL_PARAMS) : IPixelShader( pSS, XSE_IRESOURCE_PARAMS ), 
			m_pSS( (CCGShaderSystem*)pSS ), m_ulShaderFlags( 0 ), m_CGShader( xst_null ), m_CGProfile( CG_PROFILE_UNKNOWN ), 
			m_pRS( pRS ), m_bCompiled( false )
		{
		}

		CCGPixelShader::~CCGPixelShader()
		{
			
		}

		void CCGPixelShader::_Destroy()
		{
			m_pSS->DestroyPixelShader( this );
		}

		i32 CCGPixelShader::Compile(xst_castring &strEntryPoint, SHADER_PROFILE eProfile)
		{
			this->m_strEntryPoint = strEntryPoint;
			this->m_eProfile = eProfile;
			return m_pSS->CompilePixelShader( this );
		}

		i32 CCGPixelShader::Compile(xst_castring& strEntryPoint, CGprofile Profile)
		{
			xst_assert( Profile, "(CCGPixelShader::Compile)" );
			xst_assert( m_pSS, "(CCGPixelShader::Compile)" );
			this->m_strEntryPoint = strEntryPoint;
			this->m_CGProfile = Profile;
			return m_pSS->CompilePixelShader( this );
		}

		i32	CCGPixelShader::Compile(xst_castring& strEntryPoint, SHADER_PROFILE eProfile, xst_castring& strCode)
		{
			this->m_strEntryPoint = strEntryPoint;
			this->m_eProfile = eProfile;
			return m_pSS->CompilePixelShader( strCode.data(), strCode.size(), this );
		}
		
		i32	CCGPixelShader::Compile(xst_castring& strEntryPoint, CGprofile Profile, xst_castring& strCode)
		{
			this->m_strEntryPoint = strEntryPoint;
			this->m_CGProfile = Profile;
			return m_pSS->CompilePixelShader( strCode.data(), strCode.size(), this );
		}

	}//resources

}//xse