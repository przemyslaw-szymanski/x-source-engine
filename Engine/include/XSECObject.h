#ifndef XSE_COBJECT_H
#define XSE_COBJECT_H

#include "XSECommon.h"
#include "XSECBoundingVolume.h"

namespace XSE
{
	class XST_API CObject : public virtual XST::IObject
	{
		friend class CSceneManager;
		friend class CSceneDebug;

		public:

										CObject(ul32 ulType, lpcastr strDbgName) : 
											m_ulObjType( ulType ), m_vecPosition( Vec3::ZERO ), m_vecDirection( Vec3::Z ), 
											m_uiObjDisableReason( ODR::NOT_DISABLED ), m_vecScale( 1.0f ), m_quatOrientation( Quat::ZERO ) 
										{
											XST_SET_DBG_NAME( this, strDbgName );
											#if defined( XSE_SCENE_DEBUG)
												m_pAABBMesh = xst_null;
												m_bDbgObject = false;
											#endif //XSE_SCENE_DEBUG
										}

										CObject(ul32 ulType, ul32 ulHandle, lpcastr strDbgName) : m_ulObjType( ulType ), m_vecPosition( Vec3::ZERO ), m_vecDirection( Vec3::ZERO ), m_uiObjDisableReason( ODR::NOT_DISABLED ) 
										{
											XST_SET_DBG_NAME( this, strDbgName );
											#if defined( XSE_SCENE_DEBUG)
												m_pAABBMesh = xst_null;
												m_bDbgObject = false;
											#endif //XSE_SCENE_DEBUG
										}

			virtual						~CObject() 
										{
											#if defined( XSE_SCENE_DEBUG)
												m_pAABBMesh = xst_null;
												m_bDbgObject = false;
											#endif //XSE_SCENE_DEBUG
										}

			virtual	void				Update()
										{ return;}

			virtual void				SetObject(const CObject* pOther)
										{
											this->SetObjectBoundingVolume( pOther->GetObjectBoundingVolume() );
											this->DisableObject( pOther->GetObjectDisableReason() );
											this->SetDbgObject( pOther->IsDbgObject() );
											this->SetObjectDirection( pOther->GetObjectDirection() );
											this->SetObjectDirty( true );
											this->SetObjectOrientation( this->GetObjectOrientation() );
											this->SetObjectPosition( this->GetObjectPosition() );
											this->SetObjectScale( this->GetObjectScale() );
										}

			/*virtual
			xst_fi	xst_castring&		GetObjectName() const
										{ return this->_GetDbgName(); }*/

			virtual	xst_fi	void		SetObjectPosition(const Vec3& vecPos)
										{ SetObjectPosition( vecPos.x, vecPos.y, vecPos.z ); }

			virtual void				SetObjectPosition(cf32& fX, cf32& fY, cf32& fZ)
										{ m_vecPosition.x = fX; m_vecPosition.y = fY; m_vecPosition.z = fZ; }

			virtual const Vec3&			GetObjectPosition() const
										{ return m_vecPosition; }

			virtual	xst_fi	void		SetObjectDirection(const Vec3& vecDir)
										{ SetObjectDirection( vecDir.x, vecDir.y, vecDir.z ); }

			virtual void				SetObjectDirection(cf32& fX, cf32& fY, cf32& fZ)
										{ m_vecDirection.x = fX; m_vecDirection.y = fY; m_vecDirection.z = fZ; }

			virtual xst_fi	void		SetObjectScale(const Vec3& vecScale)
										{ SetObjectScale( vecScale.x, vecScale.y, vecScale.z ); }

			virtual void				SetObjectScale(cf32& fX, cf32& fY, cf32& fZ)
										{ m_vecScale.x = fX; m_vecScale.y = fY; m_vecScale.z = fZ; }

			virtual	const Vec3&			GetObjectDirection() const
										{ return m_vecDirection; }

			virtual const Vec3&			GetObjectScale() const
										{ return m_vecScale; }

			virtual void				SetObjectOrientation(cf32& fAngle, cf32& fX, cf32& fY, cf32& fZ)
										{ m_quatOrientation.Set( XST::Math::AngleToRadian( fAngle ), fX, fY, fZ ); }

			virtual xst_fi void			RotateObject(cf32& fAngle, const Vec3& vecAxis)
										{ RotateObject( fAngle, vecAxis.x, vecAxis.y, vecAxis.z ); }

			virtual void				RotateObject(cf32& fAngle, cf32& fX, cf32& fY, cf32& fZ);


			virtual 
			xst_fi	const Quat&			GetObjectOrientation() const
										{ return m_quatOrientation; }

			virtual xst_fi void			SetObjectOrientation(const Quat& quatOrientation)
										{ SetObjectOrientation( quatOrientation.x, quatOrientation.y, quatOrientation.z, quatOrientation.w ); }

			virtual const Vec3&			MoveObject(cf32& fDistance, const Vec3& vecDirection);
			virtual const Vec3&			MoveObject(const Vec3& vecMove);

			virtual	xst_fi ul32			GetObjectHandle() const = 0;

			/*virtual 
			xst_fi xst_castring&		GetObjectName() const
										{ return XST_GET_DBG_NAME( this ); }*/

			virtual void				UpdateObject(cf32& fElapsedTime) {}

			xst_fi	ul32				GetObjectType() const
										{ return m_ulObjType; }

			virtual void				DisableObject(cu32& uiDisableReason) 
										{ m_uiObjDisableReason = uiDisableReason; }

			virtual xst_fi u32			GetObjectDisableReason() const
										{ return m_uiObjDisableReason; }

			virtual xst_fi bool			IsObjectDisabled() const
										{ return m_uiObjDisableReason != ODR::NOT_DISABLED; }

			virtual 
			xst_fi	CBoundingVolume&	GetObjectBoundingVolume()
										{ return m_ObjBoundingVolume; }

			virtual xst_fi	
			const	CBoundingVolume&	GetObjectBoundingVolume() const
										{ return m_ObjBoundingVolume; }

			virtual void				SetObjectBoundingVolume(const CBoundingVolume& Volume);

			virtual 
			const	CBoundingVolume&	CalcObjectBoundingVolume()
										{ return m_ObjBoundingVolume; }

			virtual	void				SetObjectDirty(bool bDirty)
										{ m_bObjDirty = bDirty; }

			virtual xst_fi
			bool						IsObjectDirty() const
										{ return m_bObjDirty; }

			virtual	void				VisibleAABB(bool bVisible);

			virtual	bool				IsAABBVisibled() const;

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

		protected:

			CBoundingVolume		m_ObjBoundingVolume;
			Vec3				m_vecPosition; //translate
			Vec3				m_vecDirection; 
			Vec3				m_vecScale;
			Quat				m_quatOrientation;
			ul32				m_ulObjType;
			u32					m_uiObjDisableReason;
			bool				m_bObjDirty;
#if defined( XSE_SCENE_DEBUG )
			Resources::CMesh*	m_pAABBMesh;
			bool				m_bDbgObject;
#endif
	};

	typedef XST::TCObjectSmartPointer< CObject >	ObjectPtr;

}//xse

#endif