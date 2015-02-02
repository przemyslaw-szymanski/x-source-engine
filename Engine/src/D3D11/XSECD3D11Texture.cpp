#include "XSECD3D11Texture.h"
#include "XSECD3D11RenderSystem.h"

namespace XSE
{
	namespace D3D11
	{
		CTexture::CTexture(CRenderSystem* pRS, XSE_IRESOURCE_DECL_PARAMS) :
			XSE::Resources::ITexture( XSE_IRESOURCE_PARAMS )
			, m_pRS( pRS )
		{

		}

		CTexture::~CTexture()
		{
			xst_assert2( m_pRS );
			m_pRS->_DestroyTexture();
			m_pRS = xst_null;
		}
	} // D3D11
} // XSE