#ifndef XSE_COBJECT_H
#define XSE_COBJECT_H

#include "XSECommon.h"
#include "XSECBoundingVolume.h"

namespace XSE
{
	class IObjectListener;

	class XST_API CObject : public virtual XST::IObject
	{
		friend class CSceneManager;
		friend class CSceneDebug;
		friend class IObjectListener;

		public:

										CObject();
										CObject(ul32 ulType, lpcastr strDbgName, CObject* pParent = xst_null);
										CObject(ul32 ulType, ul32 ulHandle, lpcastr strDbgName, CObject* pParent = xst_null);

			virtual						~CObject();

			virtual	void				Update(cf32& fTime = 0.0f);

			virtual void				SetObject(const CObject* pOther)
										{
											this->SetBoundingVolume( pOther->GetBoundingVolume() );
											this->Disable( pOther->GetDisableReason() );
											this->SetDbgObject( pOther->IsDbgObject() );
											this->SetDirection( pOther->GetDirection() );
											this->IsDirty( true );
											this->SetOrientation( this->GetOrientation() );
											this->SetPosition( this->GetPosition() );
											this->SetScale( this->GetScale() );
										}

			/*virtual
			xst_fi	xst_castring&		GetObjectName() const
										{ return this->_GetDbgName(); }*/

			virtual xst_i	CObject*	GetParent() const
										{ return m_pParent; }

			virtual	xst_fi	void		SetPosition(const Vec3& vecPos)
										{ SetPosition( vecPos.x, vecPos.y, vecPos.z ); }

			virtual void				SetPosition(cf32& fX, cf32& fY, cf32& fZ)
										{ m_vecPosition.x = fX; m_vecPosition.y = fY; m_vecPosition.z = fZ; IsDirty( true ); }

			virtual xst_fi
			const Vec3&					GetPosition() const
										{ return m_vecPosition; }

			virtual	xst_fi	void		SetDirection(const Vec3& vecDir)
										{ SetDirection( vecDir.x, vecDir.y, vecDir.z ); }

			virtual void				SetDirection(cf32& fX, cf32& fY, cf32& fZ)
										{ m_vecDirection.x = fX; m_vecDirection.y = fY; m_vecDirection.z = fZ; IsDirty( true ); }

			virtual xst_fi	void		SetScale(const Vec3& vecScale)
										{ SetScale( vecScale.x, vecScale.y, vecScale.z ); }

			virtual void				SetScale(cf32& fX, cf32& fY, cf32& fZ)
										{ m_vecScale.x = fX; m_vecScale.y = fY; m_vecScale.z = fZ; IsDirty( true ); }

			virtual	xst_i
			const Vec3&					GetDirection() const
										{ return m_vecDirection; }

			virtual xst_i
			const Vec3&					GetScale() const
										{ return m_vecScale; }

			virtual void				SetOrientation(cf32& fAngle, cf32& fX, cf32& fY, cf32& fZ)
										{ m_quatOrientation.Set( XST::Math::AngleToRadian( fAngle ), fX, fY, fZ ); IsDirty( true ); }

			virtual xst_fi	void		Rotate(cf32& fAngle, const Vec3& vecAxis)
										{ Rotate( fAngle, vecAxis.x, vecAxis.y, vecAxis.z ); }

			virtual void				Rotate(cf32& fAngle, cf32& fX, cf32& fY, cf32& fZ);


			virtual 
			xst_fi	const Quat&			GetOrientation() const
										{ return m_quatOrientation; }

			virtual xst_fi void			SetOrientation(const Quat& quatOrientation)
										{ SetOrientation( quatOrientation.x, quatOrientation.y, quatOrientation.z, quatOrientation.w ); }

			virtual const Vec3&			MoveObject(cf32& fDistance, const Vec3& vecDirection);
			virtual const Vec3&			MoveObject(const Vec3& vecMove);

			virtual	xst_fi ul32			GetObjectHandle() const = 0;

			/*virtual 
			xst_fi xst_castring&		GetObjectName() const
										{ return XST_GET_DBG_NAME( this ); }*/

			xst_fi	ul32				GetObjectType() const
										{ return m_ulObjType; }

			virtual void				Disable(cu32& uiDisableReason) 
										{ m_uiObjDisableReason = uiDisableReason; _OnObjectDisable( uiDisableReason ); }

			virtual xst_fi u32			GetDisableReason() const
										{ return m_uiObjDisableReason; }

			virtual xst_fi bool			IsDisabled() const
										{ return m_uiObjDisableReason != ODR::NOT_DISABLED; }

			virtual 
			xst_fi	CBoundingVolume&	GetBoundingVolume()
										{ return m_ObjBoundingVolume; }

			virtual xst_fi	
			const	CBoundingVolume&	GetBoundingVolume() const
										{ return m_ObjBoundingVolume; }

			virtual void				SetBoundingVolume(const CBoundingVolume& Volume);

			virtual 
			const	CBoundingVolume&	CalcObjectBoundingVolume()
										{ return m_ObjBoundingVolume; }

			virtual void				SetDistanceToCamera(cf32& fDist);

			virtual xst_fi
			f32							GetDistanceToCamera() const
										{ return m_fObjDistToCamera; }
			
			virtual	xst_i	void		IsDirty(bool bDirty)
										{ m_bObjDirty = bDirty; }

			virtual xst_fi
			bool						IsDirty() const
										{ return m_bObjDirty; }

			virtual	void				VisibleAABB(bool bVisible);

			virtual	bool				IsAABBVisibled() const;

			virtual void				SetObjectListener(IObjectListener* pListener);

			virtual void				RemoveObjectListener();

			virtual xst_fi
			IObjectListener*			GetObjectListener() const
										{ return m_pObjListener; }

			xst_fi	Resources::CMesh*	GetAABBMesh() const
										{
											#if defined( XSE_SCENE_DEBUG )
												if( m_pAABBMesh != xst_null )
													return m_pAABBMesh;
											#endif
											return xst_null;
										}

			virtual xst_fi bool			IsDbgObject() const
										{
											#if defined( XSE_SCENE_DEBUG )
												return m_bDbgObject;
											#endif
											return false;
										}

			virtual void				SetDbgObject(bool bDbg)
										{
											#if defined( XSE_SCENE_DEBUG )
												m_bDbgObject = bDbg;
											#endif
										}

			virtual i32					_SetParent(CObject* pParent);

			virtual Vec3				CalcWorldPosition() const;
			virtual void				CalcWorldPosition(Vec3* pOut) const;;

		protected:

			virtual void				_OnObjectDisable(cu32& uDisableReason) {}

		protected:

			CBoundingVolume		m_ObjBoundingVolume;
			Quat				m_quatOrientation = Quat::IDENTITY;
			Vec3				m_vecPosition = Vec3::ZERO; //translate
			Vec3				m_vecDirection = Vec3::Z; 
			Vec3				m_vecScale = Vec3::UNIT;
			IObjectListener*	m_pObjListener = xst_null;
			CObject*			m_pParent = xst_null;
			ul32				m_ulObjType;
			u32					m_uiObjDisableReason = ODR::NOT_DISABLED;
			f32					m_fObjDistToCamera = -1.0f; // object distance to camera
			bool				m_bObjDirty = false;
#if defined( XSE_SCENE_DEBUG )
			Resources::CMesh*	m_pAABBMesh = xst_null;
			bool				m_bDbgObject = false;
#endif
	};

	typedef XST::TCObjectSmartPointer< CObject >	ObjectPtr;

	class IObjectListener
	{
		public:

							IObjectListener() {}
		virtual				~IObjectListener() {}

		virtual void		OnObjectUpdate(CObject*) {}
		virtual void		OnObjectMove(CObject*) {}
		virtual void		OnObjectSetDistanceToCamera(CObject*) {}
	};

}//xse

#endif