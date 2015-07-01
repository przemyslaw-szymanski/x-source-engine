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

											IVertexShader(IShaderSystem* pSS, const IInputLayout* pIL, XSE_IRESOURCE_DECL_PARAMS_DEFAULT) : IShader( pSS, ShaderTypes::VERTEX, XSE_IRESOURCE_PARAMS ), m_pIL( pIL ) { }
				virtual						~IVertexShader() {}

				xst_fi	const IInputLayout*	GetInputLayout() const
											{ return m_pIL; }

				xst_fi	void				SetInputLayout(const IInputLayout* pIL)
                {
                    xst_assert( m_pIL == xst_null, "(IVertexShader::SetInputLayout) InputLayout can't be set twice." ); 
                    m_pIL = pIL;
                }

			protected:

				const IInputLayout*	m_pIL = xst_null;
		};
	}//resources

	typedef XST::TCObjectSmartPointer< Resources::IVertexShader >	VertexShaderPtr;
	XST_TEMPLATE_CLASS XST::TCObjectSmartPointer< Resources::IVertexShader >;

    typedef XST::TCWeakPointer< Resources::IVertexShader >	VertexShaderWeakPtr;
	XST_TEMPLATE_CLASS XST::TCWeakPointer< Resources::IVertexShader >;

}//xse

#endif