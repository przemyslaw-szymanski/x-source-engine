#ifndef XSE_CCGPIXEL_SHADER_H
#define XSE_CCGPIXEL_SHADER_H

#include "XSEIPixelShader.h"

#include <CG/cg.h>

namespace XSE
{
	class CCGShaderSystem;

	namespace Resources
	{
		class CCGPixelShader : public IPixelShader
		{
			friend class XSE::CCGShaderSystem;

			public:

										CCGPixelShader(IRenderSystem* pRS, IShaderSystem* pSS, XSE_IRESOURCE_DECL_PARAMS_DEFAULT);
				virtual					~CCGPixelShader();

						i32				Compile(xst_castring& strEntryPoint, SHADER_PROFILE eProfile);
						i32				Compile(xst_castring& strEntryPoint, CGprofile Profile);

						i32				Compile(xst_castring& strEntryPoint, SHADER_PROFILE eProfile, xst_castring& strCode);
						i32				Compile(xst_castring& strEntryPoint, CGprofile Profile, xst_castring& strCode);

						lpcastr			GetEntryPoint()
										{ return m_strEntryPoint.data(); }

						bool			IsCompiled()
										{ return m_bCompiled; }

				xst_fi	ul32			GetShaderFlags()
										{ return m_ulShaderFlags; }

				xst_fi	CGprogram		GetCGShader()
										{ return m_CGShader; }

						IRenderSystem*	GetRenderSystem()
										{ return m_pRS; }

				xst_fi	void			SetCGProfile(CGprofile eProfile)
										{ m_CGProfile = eProfile; }

				xst_fi	CGprofile		GetCGProfile()
										{ return m_CGProfile; }

			protected:

						void			_Destroy();

			protected:

				CGprogram			m_CGShader;
				CGprofile			m_CGProfile;
				CCGShaderSystem*	m_pSS;
				IRenderSystem*		m_pRS;
				xst_astring			m_strEntryPoint;
				ul32				m_ulShaderFlags;
				bool				m_bCompiled;
		};
	}//resources

}//xse

#endif //XSE_CCGVERTEX_SHADER_H