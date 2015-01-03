#ifndef XSE_CD3D11_INPUT_LAYOUT_H
#define XSE_CD3D11_INPUT_LAYOUT_H

#include "XSEIInputLayout.h"
#include "XSED3D11Common.h"

#if defined (XST_WINDOWS)
namespace XSE
{

	namespace D3D11
	{
		class CInputLayout : public IInputLayout
		{
			friend class CRenderSystem;

			public:

												CInputLayout(CRenderSystem* pRS);
				virtual							~CInputLayout();

						//i32						AddElement(const INPUT_LAYOUT_ELEMENT& _eElement);

						i32						Create(cul32& ulElements);

						ul32					GetHandle() const xst_implement
												{ return m_ulHandle; }

						xst_castring&			GetVertexShaderCode() const
												{ return m_strVSCode; }

				const	VertexShaderPtr			GetVertexShader() const xst_implement
												{ return m_pVS; }

						VertexShaderPtr			GetVertexShader() xst_implement
												{ return m_pVS; }

				const	PixelShaderPtr			GetPixelShader() const xst_implement
												{ return m_pPS; }

						PixelShaderPtr			GetPixelShader() xst_implement
												{ return m_pPS; }
			
				const	SInputLayoutElement*	GetElements() const
												{ return &m_Elements[ 0 ]; } 

						u32						GetElementCount()
												{ return m_Elements.size(); }

						cu32					GetElementCount() const
												{ return m_Elements.size(); }	

						ul32					GetElementSize(cul32& ulElementId) const
												{ return m_Elements[ ulElementId ].ulSize; }

						ul32					GetElementOffset(cul32& ulElementId) const
												{ return m_Elements[ ulElementId ].ulOffset; }

						ul32					GetVertexSize()
												{ return m_ulVertexSize; }

						cul32					GetVertexSize() const
												{ return m_ulVertexSize; }


			protected:

						i32						_Build();
						i32						_FindElement(INPUT_LAYOUT_ELEMENT eElement);

			protected:

				//XST::TCSortedConstantArray< INPUT_LAYOUT_ELEMENT, InputLayoutElements::_ENUM_COUNT >	m_Elements;
				//xst_vector< SInputLayoutElement >	m_Elements;
				//XST::TCSortedConstantArray< SInputLayoutElement, XSE_IL_ELEMENT_COUNT >	m_Elements;
				XST::TCConstantArray< SInputLayoutElement, InputLayoutElements::_ENUM_COUNT >	m_Elements;
				ul32								m_ulCurrentOffset;
				D3D11_INPUT_ELEMENT_DESC			m_aInputElements[ InputLayoutElements::_ENUM_COUNT ];
				ID3D11InputLayout*					m_pD3DInputLayout;
				ID3DBlob*							m_pD3DVertexShaderBlob;
				CRenderSystem*						m_pRS;
				xst_astring							m_strVSCode;
				xst_astring							m_strPSCode;
				VertexShaderPtr						m_pVS;
				PixelShaderPtr						m_pPS;
				ul32								m_ulVertexSize;
				ul32								m_ulHandle;
		};

	}//d3d11
}//xse
#endif //XST_WINDOWS
#endif