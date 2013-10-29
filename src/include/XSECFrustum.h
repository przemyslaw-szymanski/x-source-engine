#ifndef XSE_CFRUSTUM_H
#define XSE_CFRUSTUM_H

#include "XSECommon.h"
#include "XST/XSTCPlane.h"

namespace XSE
{
	struct FrustumPlanes
	{
		enum PLANE
		{
			LEFT_PLANE = 0,
			RIGHT_PLANE,
			TOP_PLANE,
			BOTTOM_PLANE,
			FAR_PLANE,
			NEAR_PLANE,
			_MAX_PLANES
		};
	};

	typedef FrustumPlanes::PLANE	FRUSTUM_PLANE;

	class CFrustum
	{
		public:

									CFrustum() : m_vecPosition( Vec3::ZERO ), m_vecDirection( Vec3::ZERO ) {}
			virtual					~CFrustum() {}

			xst_fi	void			SetPlanes(const XST::CPlane* aPlanes)
									{ memcpy( this->aPlanes, aPlanes, sizeof( XST::CPlane ) * FrustumPlanes::_MAX_PLANES ); }

			xst_fi	XST::CPlane*	GetPlanes()
									{ return this->aPlanes; }

			xst_fi	
			const	XST::CPlane*	GetPlanes() const
									{ return this->aPlanes; }

			xst_fi	XST::CPlane&	GetPlane(const FRUSTUM_PLANE& ePlane)
									{ return this->aPlanes[ ePlane ]; }

			xst_fi	
			const	XST::CPlane&	GetPlane(const FRUSTUM_PLANE& ePlane) const
									{ return this->aPlanes[ ePlane ]; }

			xst_fi	void			SetPlane(const FRUSTUM_PLANE& ePlane, const XST::CPlane& Plane)
									{ this->aPlanes[ ePlane ] = Plane; }

			xst_fi	void			SetPosition(const Vec3& vecPos)
									{ m_vecPosition = vecPos; }

			xst_fi	const Vec3&		SetPosition() const
									{ return m_vecPosition; }

			xst_fi	void			SetDirection(const Vec3& vecDir)
									{ m_vecDirection = vecDir; }

			xst_fi	const Vec3&		SetDirection() const
									{ return m_vecDirection; }

		public:

			Vec3	m_vecPosition;
			Vec3	m_vecDirection;

			union
			{
				struct
				{
					XST::CPlane LeftPlane;
					XST::CPlane RightPlane;
					XST::CPlane TopPlane;
					XST::CPlane BottomPlane;
					XST::CPlane FarPlane;
					XST::CPlane NearPlane;
				};
				struct
				{
					XST::CPlane	aPlanes[ 6 ];
				};
			};
	};
}//xse

#endif