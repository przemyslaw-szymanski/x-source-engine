#ifndef XSE_IRENDER_LAYER_H
#define XSE_IRENDER_LAYER_H

#include "XSECommon.h"
#include "XSEIRenderSystem.h"

namespace XSE
{
	using namespace XST::Types;

	class IRenderLayer
	{
		friend class CSceneManager;

		public:

										IRenderLayer(xst_castring& strName, cu32 uiPriority) : m_uiLayerPriority( uiPriority ), m_strLayerName( strName ), m_pRenderSystem( xst_null ) {}	
			virtual						~IRenderLayer() {}

			xst_fi	IRenderSystem*		GetRenderSystem() const
										{ return m_pRenderSystem; }

			virtual	i32					AddObject(IRenderableObject* pObj) = 0;

			virtual void				RemoveObject(IRenderableObject* pObj) = 0;

			virtual	void				Render() = 0;

			virtual	void				RenderChildLayer(xst_castring& strChildName) = 0;

			virtual void				RenderChildLayer(cu32& uiPriority) = 0;

			virtual IRenderLayer*		GetLayer(xst_castring& strName) = 0;

			virtual IRenderLayer*		GetLayer(cu32& uiPriority) = 0;

			xst_fi	IRenderLayer*		GetParent() const
										{ return m_pParentLayer; }

			virtual	IRenderLayer*		CreateLayer(cu32& uiPriority, xst_castring& strName) = 0;

			virtual	i32					InsertLayer(IRenderLayer* pLayer) = 0;

			virtual	void				Enable(bool bEnable)
										{ m_bEnabled = true; }

			xst_fi	bool				IsEnabled() const
										{ return m_bEnabled; }

			xst_fi	u32					GetPriority() const
										{ return m_uiLayerPriority; }

			xst_fi	xst_castring&		GetName() const
										{ return m_strLayerName; }

			virtual	void				SetParent(IRenderLayer* pParent) = 0;

		protected:

			IRenderLayer*		m_pParentLayer;
			IRenderSystem*		m_pRenderSystem;
			xst_astring			m_strLayerName;
			u32					m_uiLayerPriority;
			bool				m_bEnabled;
	};
}//xse

#endif //XSE_IRENDER_LAYER_H