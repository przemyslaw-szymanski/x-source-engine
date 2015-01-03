#ifndef XSE_CPIXEL_SHADER_H
#define XSE_CPIXEL_SHADER_H

#include "XSEIPixelShader.h"
#include "XSEID3D11ShaderBase.h"

namespace XSE
{
	namespace D3D11
	{
		class CPixelShader : public XSE::Resources::IPixelShader, public IShaderBase
		{
			friend class CRenderSystem;
			friend class CHLSLShaderSystem;
			friend class CCGShaderSystem;
			friend class CInputLayout;

			public:

				using IShaderBase::Name;

			public:

										CPixelShader(CRenderSystem* pRS, IShaderSystem* pSS, XSE_IRESOURCE_DECL_PARAMS);
				virtual					~CPixelShader();

						i32				Compile(xst_castring& strEntryPoint, SHADER_PROFILE eProfile);

						i32				Compile(xst_castring& strEntryPoint, SHADER_PROFILE eProfile, xst_castring& strCode);

						IRenderSystem*	GetRenderSystem()
										{ return (IRenderSystem*)m_pRS; }

						bool			IsCompiled()
										{ return m_bIsCompiled; }

						const Name&	GetShaderName() const
										{ return this->GetResourceName(); }

			protected:

						void			_Destroy();

						cu8*			_GetShaderData() 
										{ return this->m_pResourceFile->GetData().GetData(); }

						ul32			_GetShaderDataSize()
										{ return this->m_pResourceFile->GetData().GetSize(); }

						lpcastr			_GetEntryPoint()
										{ return this->m_strShaderEntryPoint.data(); }

			protected:

				ID3D11PixelShader*	m_pD3DPixelShader;
				ul32				m_ulShaderType;
		};

	}//d3d11
}//xse

#endif