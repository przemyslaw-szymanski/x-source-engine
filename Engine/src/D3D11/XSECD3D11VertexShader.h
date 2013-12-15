#ifndef XSE_CVERTEX_SHADER_H
#define XSE_CVERTEX_SHADER_H

#include "XSEIVertexShader.h"
#include "XSEID3D11ShaderBase.h"

namespace XSE
{
	namespace D3D11
	{
		class CVertexShader : public XSE::Resources::IVertexShader, public IShaderBase
		{
			friend class CRenderSystem;
			friend class CHLSLShaderSystem;
			friend class CCGShaderSystem;
			friend class CInputLayout;

			public:

										CVertexShader(CRenderSystem* pRS, IShaderSystem* pSS, IInputLayout* pIL, XSE_IRESOURCE_DECL_PARAMS);
				virtual					~CVertexShader();

						i32				Compile(xst_castring& strEntryPoint, SHADER_PROFILE eProfile);

						i32				Compile(xst_castring& strEntryPoint, SHADER_PROFILE eProfile, xst_castring& strCode);

						IRenderSystem*	GetRenderSystem()
										{ return (IRenderSystem*)m_pRS; }

						bool			IsCompiled()
										{ return m_bIsCompiled; }

						xst_castring&	GetShaderName() const
										{ return this->GetResourceName(); }


			
			protected:

						void			_Destroy();

						lpcastr			_GetEntryPoint()
										{ return this->m_strShaderEntryPoint.data(); }

						cu8*			_GetShaderData() 
										{ xst_assert( this->m_pResourceFile != xst_null, "(CD3D11VertexShader::_GetShaderData)" ); return this->m_pResourceFile->GetData().GetData(); }

						ul32			_GetShaderDataSize()
										{ xst_assert( this->m_pResourceFile != xst_null, "(CD3D11VertexShader::_GetShaderData)" ); return this->m_pResourceFile->GetData().GetSize(); }

			protected:

				ID3D11VertexShader*	m_pD3DVertexShader;
				ul32				m_ulShaderType;
		};

	}//d3d11
}//xse

#endif