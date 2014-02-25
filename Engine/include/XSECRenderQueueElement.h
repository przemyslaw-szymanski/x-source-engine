#ifndef XSE_CRENDER_QUEUE_ELEMENT_H
#define XSE_CRENDER_QUEUE_ELEMENT_H

#include "XSECommon.h"
#include "XSEIRenderableObject.h"

namespace XSE
{
	struct XST_API RenderQueueTypes
	{
		static u32	TERRAIN;
		static u32	STATIC;
		static u32	ENVIRONMENT;
		static u32	DYNAMIC;
		static u32	SCREEN_SPACE;
		static u32	GUI;
	};
	typedef u32		RENDER_QUEUE_TYPE;

	struct RenderableSortTypes
	{
		enum TYPE
		{
			NONE,
			BACK_TO_FRONT,
			FRONT_TO_BACK,
			_ENUM_COUNT
		};
	};
	typedef RenderableSortTypes::TYPE	RENDERABLE_SORT_TYPE;


	class XST_API CRenderQueueElement
	{
		public:

			typedef	xst_vector< IRenderableObject* >	ObjectVector;

		public:

									CRenderQueueElement(RENDER_QUEUE_TYPE eElementType, bool bManualDestroy = false);
			virtual					~CRenderQueueElement();

			virtual void			Render(CSceneManager* pSceneMgr, IRenderSystem* pRenderSystem);

			virtual void			SortObjects();

			xst_fi	RENDER_QUEUE_TYPE	GetElementType() const
										{ return m_uiElementType; }

			virtual void			AddObject(IRenderableObject* pObject);
			virtual	void			AddObject(RenderableObjectPtr pObject)
									{ AddObject( pObject.GetPtr() ); }

			virtual void			RemoveObject(IRenderableObject* pObject);
			virtual void			RemoveObject(RenderableObjectPtr pObject)
									{ RemoveObject( pObject.GetPtr() ); }

			xst_fi	bool			IsManualDestroy() const
									{ return m_bManualDestroy; }

		protected:

			ObjectVector		m_vObjects;
			ObjectVector		m_vManualRenderObjects;
			RENDER_QUEUE_TYPE	m_uiElementType;
			bool				m_bManualDestroy;
	};

}//xse

#endif