#include "../../include/XST/XSTCAABB.h"

namespace XST
{
	bool IncludeTest_SSE(const CAABB& Left, const CAABB& Right)
	{
		m128 cmp = _mm_cmple_ps( Left.vecMin.v, Right.vecMax.v );
		return _mm_movemask_ps( cmp ) == 15;
	}

	void CAABB::Create(const Vec3& vecSize, const Vec3& vecPosition)
	{
		const Vec3 vecHalfSize( vecSize * 0.5f );

		vecMin.x = vecPosition.x - vecHalfSize.x;
		vecMin.y = vecPosition.y - vecHalfSize.y;
		vecMin.z = vecPosition.z - vecHalfSize.z;

		vecMax.x = vecPosition.x + vecHalfSize.x;
		vecMax.y = vecPosition.y + vecHalfSize.y;
		vecMax.z = vecPosition.z + vecHalfSize.z;
	}

	void CAABB::CreateFromLeftBottomFrontCorner(const Vec3& vecCornerPos, const Vec3& vecSize)
	{
		vecMin.x = vecCornerPos.x;
		vecMin.y = vecCornerPos.y;
		vecMin.z = vecCornerPos.z;

		vecMax.x = vecCornerPos.x + vecSize.x;
		vecMax.y = vecCornerPos.y + vecSize.y;
		vecMax.z = vecCornerPos.z + vecSize.z;
	}

	Vec3 CAABB::CalcNearestCorner(const Vec3& vecPos)
	{
		if( vecPos > vecMax ) return vecMax;
		if( vecPos < vecMin ) return vecMin;

		const Vec3 vecCenter = CalcCenter();
		Vec3 vecCorner;

		if( vecPos.x > vecCenter.x )
		{
			vecCorner.x = vecMax.x;
		}
		else
		{
			vecCorner.x = vecMin.x;
		}

		if( vecPos.y > vecCenter.y )
		{
			vecCorner.y = vecMax.y;
		}
		else
		{
			vecCorner.y = vecMin.y;
		}

		if( vecPos.z > vecCenter.z )
		{
			vecCorner.z = vecMax.z;
		}
		else
		{
			vecCorner.z = vecMin.z;
		}

		return vecCorner;
	}

	void CAABB::CalcCorners(Vec3* aCornersOut)
	{
		aCornersOut[ Corners::LEFT_BOTTOM_BACK ]	= Vec3( vecMin.x, vecMin.y, vecMin.z );
		aCornersOut[ Corners::LEFT_BOTTOM_FRONT ]	= Vec3( vecMin.x, vecMin.y, vecMax.z );
		aCornersOut[ Corners::LEFT_TOP_BACK ]		= Vec3( vecMin.x, vecMax.y, vecMin.z );
		aCornersOut[ Corners::LEFT_TOP_FRONT ]		= Vec3( vecMin.x, vecMax.y, vecMax.z );

		aCornersOut[ Corners::RIGHT_BOTTOM_BACK ]	= Vec3( vecMax.x, vecMin.y, vecMin.z );
		aCornersOut[ Corners::RIGHT_BOTTOM_FRONT]	= Vec3( vecMax.x, vecMin.y, vecMax.z );
		aCornersOut[ Corners::RIGHT_TOP_BACK ]		= Vec3( vecMax.x, vecMax.y, vecMin.z );
		aCornersOut[ Corners::RIGHT_TOP_FRONT ]		= Vec3( vecMax.x, vecMax.y, vecMax.z );
	}

	/*bool CAABB::TestAABB(const CAABB& AABB)
	{
		const Vec4& vecMin = AABB.GetMin();
		const Vec4& vecMax = AABB.GetMax();

		return	( vecMin.x >= vecMin.x && vecMax.x <= vecMax.x ) &&
				( vecMin.y >= vecMin.y && vecMax.y <= vecMax.y ) &&
				( vecMin.z <= vecMin.z && vecMax.z >= vecMax.z );
	}*/

}//xst