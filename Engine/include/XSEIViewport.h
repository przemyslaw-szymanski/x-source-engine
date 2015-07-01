#ifndef XSE_IVIEWPORT_H
#define XSE_IVIEWPORT_H

#include "XSECommon.h"
#include "XSERenderSystemCommon.h"

namespace XSE
{
	class IViewport
	{
		public:

									IViewport(IRenderSystem* pRS) : m_pRenderSystem( pRS ), m_bViewportCreated( false ) {}
			virtual					~IViewport() {}

			xst_fi	IRenderSystem*	GetRenderSystem() const
									{ return m_pRenderSystem; }

			xst_fi	bool			IsCreated() const
									{ return m_bViewportCreated; }

			virtual void			Clear() = 0;
			virtual void			EnableZBuffer() = 0;
			virtual void			DisableZBuffer() = 0;
			virtual FILL_MODE		GetFillMode() const = 0;
			virtual void			SetFillMode(FILL_MODE eMode) = 0;

			virtual	void			SetBackgroundColor(const XST::CColor& Color)
									{ m_BGColor = Color; }

			virtual const XST::CColor&	GetBackgroundColor()
										{ return m_BGColor; }

		protected:

			IRenderSystem*		m_pRenderSystem;
			bool				m_bViewportCreated; u8 m_padding[ 3 ];
			XST::CColor			m_BGColor; 
	};

}//xse

#endif //XSE_IVIEWPORT_H