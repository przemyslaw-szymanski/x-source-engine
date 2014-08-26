#ifndef XSE_CD3D11SHADER_BASE_H
#define XSE_CD3D11SHADER_BASE_H

#include "XSEIShader.h"
#include "XSED3D11Common.h"
#include "XSTCString.h"

#if defined (XST_WINDOWS)
namespace XSE
{
	namespace D3D11
	{
		class IShaderBase
		{
			friend class CRenderSystem;

			public:

				typedef XST::xst_resource_name Name;

			public:

										IShaderBase(CRenderSystem* pRS);
				virtual					~IShaderBase();

				virtual const Name&		GetShaderName() const = 0;

			protected:

				virtual	cu8*			_GetShaderData() = 0;

				virtual ul32			_GetShaderDataSize() = 0;

				virtual lpcastr			_GetEntryPoint() = 0;

			protected:

				CRenderSystem*	m_pRS;
				ID3DBlob*		m_pBlob;
				ul32			m_ulFlags;
				xst_astring		m_strProfile;
				bool			m_bIsCompiled;
		};

	}//d3d11
}//xse
#endif //XST_WINDOWS
#endif //XSE_CD3D11SHADER_BASE_H