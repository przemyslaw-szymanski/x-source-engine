#ifndef XST_CAABB_H
#define XST_CAABB_H

#include "XSTCommonInclude.h"
#include "XSTCommonTypes.h"
#include "XSTCVector4.h"

namespace XST
{
	using namespace Types;
	using namespace Math;

	class CAABB
	{

		public:

			struct Corners
			{
				enum CORNER
				{
					LEFT_BOTTOM_BACK,
					RIGHT_BOTTOM_BACK,
					LEFT_TOP_BACK,
					RIGHT_TOP_BACK,
					LEFT_BOTTOM_FRONT,
					RIGHT_BOTTOM_FRONT,
					LEFT_TOP_FRONT,
					RIGHT_TOP_FRONT,
					_ENUM_COUNT
				};
			};
			typedef Corners::CORNER		CORNER;

			enum
			{
				OVERLAP_CONTAINS = 0,
				OVERLAP_LEFT_OUTSIDE	= 0x00000001,
				OVERLAP_RIGHT_OUTSIDE	= 0x00000002,
				OVERLAP_TOP_OUTSIDE		= 0x00000004,
				OVERLAP_BOTTOM_OUTSIDE	= 0x00000008,
				OVERLAP_BACK_OUTSIDE	= 0x00000010,
				OVERLAP_FRONT_OUTSIDE	= 0x00000020
			};

		public:

			Vec4	vecMin;
			Vec4	vecMax;

			//Vec3	m_vecPos;
			//Vec3	m_vecHalfSize;

		public:

								CAABB() : vecMin( Vec4::ZERO ), vecMax( Vec4::ZERO )/*, m_vecPos( Vec3::ZERO ), m_vecHalfSize( Vec3::ZERO )*/ {}
								CAABB(const Vec3& vecMin, const Vec3& vecMax) : vecMax( vecMax ), vecMin( vecMin ) {}
								//CAABB(const Vec3& vecPos, const Vec3& vecSize) : m_vecPos( vecPos ), m_vecHalfSize( vecSize * 0.5f ) {}
			virtual				~CAABB() {}

					void		Create(const Vec3& vecSize, const Vec3& vecPosition = Vec3::ZERO);

					void		CreateFromLeftBottomFrontCorner(const Vec3& vecCornerPos, const Vec3& vecSize);

			xst_fi
			const	Vec4&		GetMin() const
								{ return vecMin; }

			xst_fi
			const	Vec4&		GetMax() const
								{ return vecMax; }

			xst_fi	Vec4&		GetMin() 
								{ return vecMin; }

			xst_fi	Vec4&		GetMax() 
								{ return vecMax; }

			xst_fi	Vec3		CalcCenter() const
								{  return vecMin + ( vecMax - vecMin ) * 0.5f; }

			xst_fi	Vec4		CalcSize() const
								{ return Vec4::Abs( vecMax - vecMin ); }

					Vec3		CalcNearestCorner(const Vec3& vecPos);

					void		CalcCorners(Vec3* aCornersOut);

			xst_fi	bool		IsZero() const
								{ return ( vecMax - vecMin ) == Vec3::ZERO; }

			xst_fi	bool		Includes(const Vec3& vecPoint) const
								{
									return	( vecPoint.x >= vecMin.x && vecPoint.x <= vecMax.x ) && 
											( vecPoint.y >= vecMin.y && vecPoint.y <= vecMax.y ) &&
											( vecPoint.z <= vecMin.z && vecPoint.z >= vecMax.z );
								}

			xst_fi	bool		Includes(const CAABB& AABB) const
								{
									return	( AABB.vecMin.x >= vecMin.x && AABB.vecMax.x <= vecMax.x ) &&
											( AABB.vecMin.y >= vecMin.y && AABB.vecMax.y <= vecMax.y ) &&
											( AABB.vecMin.z >= vecMin.z && AABB.vecMax.z <= vecMax.z );
								}

			xst_fi	bool		Includes_SSE(const CAABB& AABB) const
								{
									m128 cmp = _mm_cmple_ps( vecMin.v, AABB.vecMax.v );
									return _mm_movemask_ps( cmp ) == 15;
								}

			static
			xst_fi	bool		Includes(const CAABB& Bigger, const CAABB& Smaller)
								{
									return	( Smaller.vecMin.x >= Bigger.vecMin.x && Smaller.vecMax.x <= Bigger.vecMax.x ) &&
											( Smaller.vecMin.y >= Bigger.vecMin.y && Smaller.vecMax.y <= Bigger.vecMax.y ) &&
											( Smaller.vecMin.z >= Bigger.vecMin.z && Smaller.vecMax.z <= Bigger.vecMax.z );
								}

			static
			xst_fi	bool		Includes_SSE(const CAABB& Bigger, const CAABB& Smaller)
								{
									m128 cmp = _mm_cmple_ps( Bigger.vecMin.v, Smaller.vecMax.v );
									return _mm_movemask_ps( cmp ) == 15;
								}
	};

	bool IncludeTest_SSE(CAABB& Left, const CAABB& Right);

}//xst

#endif