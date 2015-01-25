#ifndef XSE_CD3D11_VERTEX_BUFFER_H
#define XSE_CD3D11_VERTEX_BUFFER_H

#include "XSEIVertexBuffer.h"
#include "XSED3D11Common.h"
#include "XSETCFreeListAllocator.h"

#if defined (XST_WINDOWS)
namespace XSE
{
	namespace D3D11
	{

		class CVertexBuffer : public IVertexBuffer, public XST::TIAllocableObject< TCFreeListAllocator< CVertexBuffer > >
		{
			friend class CRenderSystem;

			public:	

										CVertexBuffer(CRenderSystem* pRS, XST::IAllocator* pAlloc);

				virtual					~CVertexBuffer();

						i32				Lock();

						bool			IsLocked()
										{ return m_bLocked; }

						bool			IsLocked() const
										{ return m_bLocked; }

						i32				Unlock(bool bCreate);

						i32				SetVertexCount(ul32 ulCount);

						void			SetTopologyType(TOPOLOGY_TYPE eType);

						TOPOLOGY_TYPE	GetTopologyType();

						void			SetUsage(BUFFER_USAGE eUsage);

						BUFFER_USAGE	GetUsage();

						i32				Create();

						void			Update()
										{ Update( 0, this->m_VertexData.GetVertexCount() ); }

						void			Update(cu32& uiStartVertex, cu32& uiVertexCount);

						void			SetInputLayout(const IInputLayout* pInputLayout);
						void			SetInputLayout(ul32 uElements);

						ul32			GetVertexCount() const
										{ return m_VertexData.GetVertexCount(); }

				const	IInputLayout*	GetInputLayout() const
										{ return (IInputLayout*)m_pInputLayout; }

			protected:

				CRenderSystem*				m_pRS;
				D3D11_BUFFER_DESC			m_D3DBufferDesc;
				D3D11_SUBRESOURCE_DATA		m_D3DInitData;
				ID3D11Buffer*				m_pD3DBuffer;
				D3D11_PRIMITIVE_TOPOLOGY	m_eD3DTopologyType;
				TOPOLOGY_TYPE				m_eTopologyType;
				BUFFER_USAGE				m_eUsage;
				D3D11_USAGE					m_eD3DBufferUsage;
				//XST::TCData< Vertex >		m_aVertices;
				//const IInputLayout*			m_pInputLayout;
				CInputLayout*				m_pInputLayout;
				ul32						m_ulVertexCount;
				bool						m_bLocked;
		};

	}//d3d11
}//xse
#endif //XST_WINDOWS
#endif //VERTEX_BUFFER