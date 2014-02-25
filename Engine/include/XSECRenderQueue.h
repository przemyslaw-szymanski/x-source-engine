#ifndef XSE_CRENDER_QUEUE_H
#define XSE_CRENDER_QUEUE_H

#include "XSECommon.h"
#include "XSECRenderArray.h"
#include "XSECRenderQueueElement.h"

namespace XSE
{
	/*struct RenderQueueTypes
	{
		enum TYPE
		{
			TERRAIN,
			STATIC,
			ENVIRONMENT,
			DYNAMIC,
			DEFAULT,
			SCREEN_SPACE,
			GUI,
			_ENUM_COUNT
		};
	};
	typedef RenderQueueTypes::TYPE	RENDER_QUEUE_TYPE;*/

	
	class XST_API CRenderQueue
	{
		friend class CSceneManager;

		public:

			typedef IRenderableObject			Object;
			typedef	xst_list< Object* >			ObjectList;
			typedef ObjectList::const_iterator	ObjectIterator;
			typedef fastdelegate::FastDelegate2< ObjectList&, IRenderSystem* >	RenderDelegate;
			typedef xst_vector< CRenderQueueElement* >	ElementVector;

		public:

										CRenderQueue();
			virtual						~CRenderQueue();

			virtual void				AddObject(const RenderableObjectPtr& pObj)
										{ AddObject( pObj.GetPointer() ); }
			virtual void				AddObject(IRenderableObject* pObj);

			virtual	void				AddObject(RENDER_QUEUE_TYPE eType, const RenderableObjectPtr& pObj)
										{ AddObject( eType, pObj.GetPointer() ); }
			virtual	void				AddObject(RENDER_QUEUE_TYPE eType, IRenderableObject* pObj);

			virtual void				Remove(RENDER_QUEUE_TYPE eType, IRenderableObject* pObject);
			virtual	void				Remove(RENDER_QUEUE_TYPE eType, const RenderableObjectPtr& pObject)
										{ Remove( eType, pObject.GetPointer() ); }

			virtual	void				Render(CSceneManager* pSceneMgr);

		protected:

					void				_SetRenderSystem(IRenderSystem* pRS)
										{ m_pRS = pRS; }

		protected:

			IRenderSystem*		m_pRS;
			ObjectIterator		m_ObjItr;
			ObjectList			m_lObjects; //standard rendering
			ObjectList			m_lManualRenderObjects; //render manual
			CRenderTree			m_RenderTree;
			RENDER_QUEUE_TYPE	m_eType;
			RenderDelegate		m_Render;
			ElementVector		m_vElements;
	};

}//xse

#endif