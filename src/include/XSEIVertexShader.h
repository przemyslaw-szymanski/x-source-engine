#ifndef XSE_IVERTEX_SHADER_H
#define XSE_IVERTEX_SHADER_H

#include "XSEIShader.h"
#include "XSECommon.h"

namespace XSE
{
	namespace Resources
	{
		class IVertexShader : public IShader
		{
			friend class CShaderManager;
			public:

											IVertexShader(IShaderSystem* pSS, IInputLayout* pIL, XSE_IRESOURCE_DECL_PARAMS_DEFAULT) : IShader( pSS, ShaderTypes::VERTEX, XSE_IRESOURCE_PARAMS ), m_pIL( pIL ) { xst_assert2( pIL != xst_null ); }
				virtual						~IVertexShader() {}

				xst_fi	const IInputLayout*	GetInputLayout()
											{ return m_pIL; }

				xst_fi	void				SetInputLayout(IInputLayout* pIL)
											{ m_pIL = pIL; }

			protected:

				IInputLayout*	m_pIL;
		};
	}//resources

	typedef XST::TCObjectSmartPointer< Resources::IVertexShader >	VertexShaderPtr;
	XST_TEMPLATE_CLASS XST::TCObjectSmartPointer< Resources::IVertexShader >;

}//xse

#endif