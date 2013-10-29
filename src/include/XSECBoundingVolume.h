#ifndef XSE_BOUNDING_VOLUME_H
#define XSE_BOUNDING_VOLUME_H

#include "XST/XSTCAABB.h"
#include "XST/XSTMath.h"
#include "XSECBoundingSphere.h"

namespace XSE
{
	using namespace XST::Types;

	class CBoundingVolume
	{
		public:

			xst_fi						CBoundingVolume() {}
			xst_fi						CBoundingVolume(const CBoundingSphere& Sphere, const XST::CAABB& AABB) : m_BoundingSphere( Sphere ), m_AABB( AABB ) {}
			
			virtual						~CBoundingVolume() {}

					void				BuildFromMinMax(const Vec3& vecMin, const Vec3& vecMax);

			xst_fi	XST::CAABB&			GetAABB() 
										{ return m_AABB; }

			xst_fi	
			const	XST::CAABB&			GetAABB() const
										{ return m_AABB; }

			xst_fi	CBoundingSphere&	GetSphere()
										{ return m_BoundingSphere; }

			xst_fi	
			const	CBoundingSphere&	GetSphere() const
										{ return m_BoundingSphere; }

		protected:

			XST::CAABB		m_AABB;
			CBoundingSphere	m_BoundingSphere;
	};
}//xse

#endif //XSE_BOUNDING_VOLUME_H