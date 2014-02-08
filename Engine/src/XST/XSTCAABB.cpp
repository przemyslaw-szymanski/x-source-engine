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

	Vec3 CAABB::CalcNearestCorner(const Vec3& vecPos) const
	{
		Vec3 aCorners[ 8 ];
		CalcCorners( aCorners );
		f32 fMinDist = 99999;
		u32 uiMinCorner = 0;

		// Calc min of the distances
		for( u32 i = 8; i --> 0; )
		{
			f32 fDist = vecPos.Distance( aCorners[ i ] );
			if( fDist < fMinDist )
			{
				fMinDist = fDist;
				uiMinCorner = i;
			}
		}

		return aCorners[ uiMinCorner ];
	}

	void CAABB::CalcNearestCorner(const Vec3& vecPoint, Vec3 (*pavecCornersOut)[ 8 ], u32* puiCornerIdOut, f32* pfDistanceOut) const
	{
		CalcCorners( *pavecCornersOut );
		*pfDistanceOut = 99999;
		*puiCornerIdOut = 0;

		// Calc min of the distances
		for( u32 i = 8; i --> 0; )
		{
			f32 fDist = vecPoint.Distance( (*pavecCornersOut)[ i ] );
			if( fDist < *pfDistanceOut )
			{
				*pfDistanceOut = fDist;
				*puiCornerIdOut = i;
			}
		}
	}

	// TODO: pointer to an array there should be
	void CAABB::CalcCorners(Vec3* aCornersOut) const
	{
		aCornersOut[ Corners::LEFT_BOTTOM_BACK ].Set( vecMin.x, vecMin.y, vecMin.z );
		aCornersOut[ Corners::LEFT_BOTTOM_FRONT ].Set( vecMin.x, vecMin.y, vecMax.z );
		aCornersOut[ Corners::LEFT_TOP_BACK ].Set( vecMin.x, vecMax.y, vecMin.z );
		aCornersOut[ Corners::LEFT_TOP_FRONT ].Set( vecMin.x, vecMax.y, vecMax.z );

		aCornersOut[ Corners::RIGHT_BOTTOM_BACK ].Set( vecMax.x, vecMin.y, vecMin.z );
		aCornersOut[ Corners::RIGHT_BOTTOM_FRONT].Set( vecMax.x, vecMin.y, vecMax.z );
		aCornersOut[ Corners::RIGHT_TOP_BACK ].Set( vecMax.x, vecMax.y, vecMin.z );
		aCornersOut[ Corners::RIGHT_TOP_FRONT ].Set( vecMax.x, vecMax.y, vecMax.z );
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