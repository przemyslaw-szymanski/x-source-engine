#ifndef XSE_CBOUNDING_SPHERE_H
#define XSE_CBOUNDING_SPHERE_H

#include "XSECommon.h"

namespace XSE
{
	class CBoundingSphere
	{
		public:

			f32		fRadius;
			Vec4	vecCenter;

		public:

								CBoundingSphere() : fRadius( 0.0f ), vecCenter( Vec4::ZERO ) {}
								CBoundingSphere(cf32& fRadius, const Vec3& vecCenter) : fRadius( fRadius ), vecCenter( vecCenter ) {}
			virtual				~CBoundingSphere() {}

			xst_fi	void		SetRadius(cf32& fRadius)
								{ this->fRadius = fRadius; }

			xst_fi	f32			GetRadius() const
								{ return fRadius; }

			xst_fi	void		SetCenterPoint(const Vec3& vecCenterPoint)
								{ vecCenter = vecCenterPoint; }

			xst_fi	Vec3		GetCenterPoint3() const
								{ return vecCenter; }

			xst_fi const Vec4&	GetCenterPoint() const
								{ return vecCenter; }

            xst_fi  f32         CalcDistance(const Vec4& vecPoint) const
                                { return vecCenter.Distance( vecPoint ); }
            xst_fi	f32			CalcDistance(const CBoundingSphere& Sphere) const
                                { return CalcDistance( Sphere.vecCenter ); }

			xst_fi	bool		Intersects(const CBoundingSphere& Sphere) const
								{
									return vecCenter.Distance( Sphere.vecCenter ) < fRadius + Sphere.fRadius;
								}
	};

}//xse

#endif