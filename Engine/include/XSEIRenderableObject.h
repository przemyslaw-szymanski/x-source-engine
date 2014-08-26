#ifndef XSE_IRENDERABLE_OBJECT_H
#define XSE_IRENDERABLE_OBJECT_H

#include "XSECommon.h"
#include "XSEIResource.h"
#include "XSECMaterial.h"
#include "XSECObject.h"

namespace XSE
{
	//It is a base interface used by any object that should be rendered (visible on the screen)
	//It inherits from CObject a base class for any object used in whole engine. 
	//If something should be processed in the engine it need to be inherited from CObject
	class XST_API IRenderableObject : public CObject
	{
		friend class CSceneNode;
		friend class CSceneManager;
		friend class CRenderQueue;
		friend class CRenderQueueElement;

		public:

			using CObject::SetPosition;
			using CObject::SetScale;
			using CObject::SetOrientation;
			using CObject::SetDirection;

			typedef CSceneNode*	SceneNodePtr;

		public:
										IRenderableObject()
										{}

										IRenderableObject(ul32 ulType, IInputLayout* pIL, lpcastr strDbgName, bool bManualRendering = false) : 
											CObject( ulType, strDbgName ), m_pInputLayout( pIL ), m_pSceneNode( xst_null ), 
											m_bVisible( true ), m_bManualRendering( bManualRendering ), m_mtxTransform( Mtx4::IDENTITY )
											{}

			virtual						~IRenderableObject() {}

			virtual	void				Render(IRenderSystem* pRS) = 0;

			virtual void				Update(cf32& fTime = 0.0f) xst_implement;
			
            virtual void				SetVisible(bool bVisible);

            virtual xst_fi	bool		IsVisible() const
                                        { return m_bVisible && !this->IsDisabled(); }

			virtual void				SetMaterial(MaterialPtr pMat) 
										{ m_pMaterial = pMat; }

			virtual i32					SetMaterial(xst_castring& strMatName, xst_castring& strMatGroup = ALL_GROUPS);

			virtual MaterialPtr			GetMaterial()
										{ return m_pMaterial; }

			virtual const MaterialPtr&	GetMaterial() const
										{ return m_pMaterial; }

			virtual IInputLayout*		GetInputLayout() const
										{ return m_pInputLayout; }

			virtual	xst_fi	bool		IsManualRendering() const
										{ return m_bManualRendering; }

			virtual xst_fi	CRenderQueue*	GetRenderQueue() const
											{ return m_pRenderQueue; }

			xst_fi	CSceneNode*			GetSceneNode()
										{ return m_pSceneNode; }

			xst_fi	const CSceneNode*	GetSceneNode() const
										{ return m_pSceneNode; }

			virtual xst_fi const Mtx4&	GetTransformMatrix() const
										{ return m_mtxTransform; }

			virtual xst_fi void			GetTransformMatrix(Mtx4* pMtxOut) const
										{ pMtxOut->Set( m_mtxTransform ); }

			virtual xst_fi void			SetTransformMatrix(const Mtx4& mtxTransform)
										{ m_mtxTransform = mtxTransform; }

			virtual void				SetRenderableObject(const IRenderableObject* pOther);

			virtual void				CalcWorldPosition(Vec3* pVecOut);

			virtual void				CalcWorldScale(Vec3* pVecOut);

			virtual void				CalcWorldRotation(Vec4* pVecOut);

			virtual xst_i
			const Vec3&					GetWorldPosition() const
										{ return m_vecWorldPosition; }

		protected:

			virtual void			_OnAddToRenderQueue(CRenderQueueElement* pQueue) { return; }

			virtual void			_SetRenderQueue(CRenderQueue* pRenderQueue)
									{ m_pRenderQueue = pRenderQueue; }

			virtual void			_SetSceneNode(CSceneNode* pNode)
									{ xst_assert( m_pSceneNode == xst_null, "(IRenderableObject::_SetSceneNode) This object already has a scene node" ); m_pSceneNode = pNode; }

		protected:

			Mtx4					m_mtxTransform;
			MaterialPtr				m_pMaterial;
			IInputLayout*			m_pInputLayout;
			CRenderQueue*			m_pRenderQueue;
			CSceneNode*				m_pSceneNode;
			Vec3					m_vecWorldPosition;
			bool					m_bManualRendering;
			bool					m_bVisible;
	};

	typedef XST::TCObjectSmartPointer< IRenderableObject > RenderableObjectPtr;
	XST_TEMPLATE_CLASS XST::TCObjectSmartPointer< IRenderableObject >;
	typedef XST::TCWeakPointer< IRenderableObject > RenderableObjectWeakPtr;
	XST_TEMPLATE_CLASS XST::TCWeakPointer< IRenderableObject >;

}//xse

#endif