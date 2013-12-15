#ifndef XSE_CRENDER_LAYER_H
#define XSE_CRENDER_LAYER_H

#include "XSEIRenderLayer.h"

namespace XSE
{
	class CRenderLayer : public IRenderLayer
	{
		typedef	xst_vector< IRenderableObject* >	ObjVec;

		public:

										CRenderLayer(xst_castring& strName, cu32 uiPriority);
			virtual						~CRenderLayer();

			virtual	i32					AddObject(IRenderableObject* pObj);

			virtual void				RemoveObject(IRenderableObject* pObj);

			virtual	void				Render();

			virtual	void				RenderChildLayer(xst_castring& strChildName);

			virtual void				RenderChildLayer(cu32& uiPriority);

			virtual IRenderLayer*		GetLayer(xst_castring& strName);

			virtual IRenderLayer*		GetLayer(cu32& uiPriority);

			virtual	IRenderLayer*		CreateLayer(cu32& uiPriority, xst_castring& strName);

			virtual	i32					InsertLayer(IRenderLayer* pLayer);

					void				SetParent(IRenderLayer* pParent);

			xst_fi	bool				operator<(const IRenderLayer* pRight) const
										{ return this->m_uiLayerPriority < pRight->GetPriority(); }

		protected:

			XST::TCSortedDynamicArray< IRenderLayer* >	m_aLayers;
			ObjVec		m_vObjects;
	};
}//xse

#endif //XSE_CRENDER_LAYER_H