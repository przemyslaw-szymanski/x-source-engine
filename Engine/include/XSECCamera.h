#ifndef XSE_CCAMERA_H
#define XSE_CCAMERA_H

#include "XSECommon.h"
#include "XSECObject.h"
#include "XSECFrustum.h"
#include "XST/XSTCAABB.h"
#include "XSECBoundingSphere.h"

namespace XSE
{
	class XST_API CCamera : public CObject
	{
		friend class CSceneManager;

		public:

								CCamera(CSceneManager* pSceneMgr, xst_castring& strName, ul32 ulHandle);
			virtual				~CCamera();

			xst_fi	
			CSceneManager*		GetSceneManager() const
								{ return m_pSceneMgr; }

			virtual	void		Update(cf32& fElapsedTime);

			virtual void		SetProperty(cu32& uiCamProperty, xst_unknown pProperty)
								{ return; }

			virtual
			xst_fi	void		SetLookAt(const Vec3& vecPoint)
								{ m_vecLookAt = vecPoint; }

			virtual xst_fi void	SetPosition(const Vec3& vecPos)
								{ this->m_vecCamPosition = vecPos; }

			virtual xst_fi void	SetPosition(cf32& fX, cf32& fY, cf32& fZ)
								{ this->m_vecCamPosition.x = fX; this->m_vecCamPosition.y = fY; this->m_vecCamPosition.z = fZ; }

			virtual
			xst_fi const Vec3&	GetPosition() const
								{ return this->m_vecCamPosition; }

			virtual	const Vec3&	Move(cf32& fDistance, const Vec3& vecDirection)
								{ return Move( vecDirection * fDistance ); }

			xst_fi	const Vec3&	Move(const Vec3& vecMove)
								{ return Move( vecMove.x, vecMove.y, vecMove.z ); }

			virtual const Vec3& Move(cf32& fX, cf32& fY, cf32& fZ);

			virtual
			xst_fi	void		SetAngleY(cf32& fAngle) 
								{ m_vecAngles.y = std::min( std::max( -XST_HALF_PI + 1e-3f, fAngle ), XST_HALF_PI - 1e-3f ); }

			virtual
			xst_fi	void		SetAngleX(cf32& fAngle) 
								{ m_vecAngles.x = _NormalizeAngle( fAngle ); }

			virtual
			xst_fi	void		RotateX(cf32& fAngle)
								{ SetAngleX( m_vecAngles.x + fAngle * m_fHorizontalSpeed * m_fTime ); }

			virtual
			xst_fi	void		RotateY(cf32& fAngle)
								{ SetAngleY( m_vecAngles.y + fAngle * m_fVerticalSpeed * m_fTime ); }

			virtual
			xst_fi	const Vec2&	GetRotationAngles() const
								{ return m_vecAngles; }

			virtual
			xst_fi	const Vec3&	GetRight() const
								{ return m_vecRight; }

			virtual
			const	Vec3&		CalcRight();

			virtual
			xst_fi	const Vec3&	GetUp() const
								{ return m_vecUp; }

			virtual
			const	Vec3&		CalcUp();

			virtual
			xst_fi	const Vec3&	GetLookAt() const
								{ return m_vecLookAt; }

			virtual
			xst_fi	void		SetUpAxis(const Vec3& vecUp)
								{ m_vecUpAxis = vecUp; }

			virtual
			xst_fi	const Vec3&	GetUpAxis() const
								{ return m_vecUpAxis; }

			virtual	void		SetFOV(cf32& fAngle);

			virtual	void		SetFOV(cf32& fAngle, cf32& fNear, cf32& fFar);

			virtual	void		SetNear(cf32& fNear);

			virtual	void		SetFar(cf32& fFar);

			virtual	void		SetClippingPlane(cf32& fNear, cf32& fFar);

			virtual void		GetMatrix(const MATRIX_TYPE& eType, Mtx4* pMtxOut);

			virtual
			const	Mtx4&		GetViewMatrix();

			virtual
			const	Mtx4&		GetViewProjMatrix();

			virtual
			xst_fi	f32			GetNear() const
								{ return m_fNear; }

			virtual
			xst_fi	f32			GetFar() const
								{ return m_fFar; }

			virtual
			xst_fi	f32			GetAspectRatio() const
								{ return m_fAspectRatio; }

			virtual
			xst_fi	void		SetSpeed(cf32& fMoveSpeed, cf32& fHorizontalRotateSpeed, cf32& fVerticalRotateSpeed)
								{ m_fMoveSpeed = fMoveSpeed; m_fHorizontalSpeed = fHorizontalRotateSpeed; m_fVerticalSpeed = fVerticalRotateSpeed; }

			xst_fi	void		SetMoveSpeed(cf32& fMoveSpeed)
								{ m_fMoveSpeed = fMoveSpeed; }

			xst_fi	f32			GetMoveSpeed() const
								{ return m_fMoveSpeed; }

			xst_fi	f32			GetHorizontalRotateSpeed() const
								{ return m_fHorizontalSpeed; }

			xst_fi	f32			GetVerticalRotateSpeed() const
								{ return m_fVerticalSpeed; }
			
			virtual
			xst_fi	xst_castring&	GetObjectName() const
									{ return m_strName; }

			virtual
			xst_fi	ul32		GetObjectHandle() const
								{ return m_ulObjHandle; }

			virtual
			xst_fi	const Vec3&	GetDirection() const
								{ return m_vecCamDirection; }

			xst_fi	bool		IsSphereInFrustum(const Vec3& vecSpherePosition, cf32& fSphereRadius) const
								{ return IsSphereInFrustum( Vec4( vecSpherePosition.x, vecSpherePosition.y, vecSpherePosition.z, 1.0f ), fSphereRadius ); }
			virtual	bool		IsSphereInFrustum(const Vec4& vecSpherePosition, cf32& fSphereRadius) const;
			
			xst_fi	bool		IsSphereInFrustum(const CBoundingSphere& Sphere) const
								{ return IsSphereInFrustum( Sphere.GetCenterPoint(), Sphere.GetRadius() ); }

			virtual	bool		IsAABBInFrustum(const XST::CAABB& AABB) const;

			xst_fi	bool		IsCompute() const
								{ return m_bCompute; }

					void		ShowFrustumMesh(bool bShow);

					bool		IsFrustumMeshVisible() const;

		protected:

			xst_fi	f32			_NormalizeAngle(f32 fAngle)
								{
									fAngle /= XST_2PI;
									fAngle = XST::Math::Frac( fAngle );
									return fAngle * XST_2PI;
								}

			virtual	void		_CreateViewFrustum(const Mtx4& mtxViewProj);

					i32			_CreateFrustumMesh();

					i32			_UpdateVolumeMesh();

					void		_SetCompute(bool bObserving);

		protected:

			Vec3			m_vecUpAxis;
			Vec3			m_vecLookAt;
			Vec3			m_vecRight; //camera right vector
			Vec3			m_vecUp; //real camera up vector, not the up view from look at
			//direction, right, and camera up vectors create the camera space
			Vec2			m_vecAngles; //angle x and y for the camera
			Vec3			m_vecCameraDistance; //distance from the following point
			Vec3			m_vecCamDirection;
			Vec3			m_vecCamPosition;
			Mtx4			m_mtxProjection;
			Mtx4			m_mtxView;
			Mtx4			m_mtxViewProj;
			CFrustum		m_Frustum;
			IRenderSystem*	m_pRS;
			xst_astring		m_strName;
			ul32			m_ulObjHandle;
			f32				m_fTime;
			f32				m_fFOV;
			f32				m_fNear;
			f32				m_fFar;
			f32				m_fAspectRatio;
			f32				m_fMoveSpeed;
			f32				m_fHorizontalSpeed;
			f32				m_fVerticalSpeed;
			CSceneManager*	m_pSceneMgr;
			Resources::CMesh*	m_pVolumeMesh; //for debug
			bool			m_bRightCalc;
			bool			m_bUpCalc;
			bool			m_bCompute;
	};

}//xse

#endif