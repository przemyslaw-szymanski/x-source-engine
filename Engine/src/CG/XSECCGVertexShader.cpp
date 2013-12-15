#include "XSECCGVertexShader.h"
#include "CG/XSECGTypes.h"
#include "CG/XSECCGShaderSystem.h"

namespace XSE
{
	namespace Resources
	{
		CCGVertexShader::CCGVertexShader(IRenderSystem* pRS, IShaderSystem* pSS, IInputLayout* pIL, XSE_IRESOURCE_DECL_PARAMS) : IVertexShader( pSS, pIL, XSE_IRESOURCE_PARAMS ), 
			m_pSS( (CCGShaderSystem*)pSS ), m_ulShaderFlags( 0 ), m_CGShader( xst_null ), m_CGProfile( CG_PROFILE_UNKNOWN ),
			m_pRS( pRS ), m_bCompiled( false )
		{
		}

		CCGVertexShader::~CCGVertexShader()
		{
			
		}

		void CCGVertexShader::_Destroy()
		{
			m_pSS->DestroyVertexShader( this );
		}

		i32 CCGVertexShader::Compile(xst_castring &strEntryPoint, SHADER_PROFILE eProfile)
		{
			xst_assert( m_pSS, "(CCGVertexShader::Compile)" );
			this->m_strEntryPoint = strEntryPoint;
			this->m_eProfile = eProfile;
			return m_pSS->CompileVertexShader( this );
		}

		i32 CCGVertexShader::Compile(xst_castring& strEntryPoint, CGprofile Profile)
		{
			xst_assert( Profile, "(CCGVertexShader::Compile)" );
			xst_assert( m_pSS, "(CCGVertexShader::Compile)" );
			this->m_strEntryPoint = strEntryPoint;
			this->m_CGProfile = Profile;
			return m_pSS->CompileVertexShader( this );
		}

		i32	CCGVertexShader::Compile(xst_castring& strEntryPoint, SHADER_PROFILE eProfile, xst_castring& strCode)
		{
			this->m_strEntryPoint = strEntryPoint;
			this->m_eProfile = eProfile;
			return m_pSS->CompileVertexShader( strCode.data(), strCode.size(), this );
		}
		
		i32	CCGVertexShader::Compile(xst_castring& strEntryPoint, CGprofile Profile, xst_castring& strCode)
		{
			this->m_strEntryPoint = strEntryPoint;
			this->m_CGProfile = Profile;
			return m_pSS->CompileVertexShader( strCode.data(), strCode.size(), this );
		}

	}//resources

}//xse