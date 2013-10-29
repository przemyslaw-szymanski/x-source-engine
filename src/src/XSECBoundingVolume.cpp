#include "../include/XSECBoundingVolume.h"

namespace XSE
{
	void CBoundingVolume::BuildFromMinMax(const Vec3& vecMin, const Vec3& vecMax)
	{
		m_AABB = XST::CAABB( vecMin, vecMax );
		const Vec3 vecCenter( m_AABB.CalcCenter() );
		f32 fDistance = vecCenter.Distance( vecMax );
		m_BoundingSphere = CBoundingSphere( fDistance, vecCenter );
	}
}//xse