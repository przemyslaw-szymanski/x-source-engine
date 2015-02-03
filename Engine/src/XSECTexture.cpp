#include "XSECTexture.h"
#include "XSEIRenderSystem.h"

namespace XSE
{
	namespace Resources
	{
		CTexture::CTexture(IRenderSystem* pRS, XSE_IRESOURCE_DECL_PARAMS) :
			IResource( XSE_IRESOURCE_PARAMS )
			, m_pRS( pRS )
		{

		}

		CTexture::~CTexture()
		{
			xst_assert2( m_pRS );
			m_pRS->DestroyTexture( &m_RSHandle );
		}
	} // resources
} //xse