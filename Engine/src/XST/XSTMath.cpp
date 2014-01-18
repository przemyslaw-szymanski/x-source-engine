#include "XST/XSTMath.h"
#include "XST/XSTCMatrix4.h"
#include "XST/XSTCPoint.h"

namespace XST
{
	namespace Math
	{
		const u32 SELECT_0          = 0x00000000;
		const u32 SELECT_1          = 0xFFFFFFFF;
		const CVector2 CVector2::ZERO = CVector2( 0, 0 );
		const CVector2 CVector2::X = CVector2( 1, 0);
		const CVector2 CVector2::Y = CVector2( 0, 1 );
		const CVector2 CVector2::NEGATIVE_X = CVector2( -1, 0 );
		const CVector2 CVector2::NEGATIVE_Y = CVector2( 0, -1 );
		const CVector2 CVector2::SIGN_MASK( -0.f ); // 1 << 31
		const CVector2 CVector2::UNIT( 1.0f, 1.0f );

		const CVector3 CVector3::ZERO = CVector3( 0, 0, 0 );
		const CVector3 CVector3::X = CVector3( 1, 0, 0 );
		const CVector3 CVector3::Y = CVector3( 0, 1, 0 );
		const CVector3 CVector3::Z = CVector3( 0, 0, 1 );
		const CVector3 CVector3::NEGATIVE_X = CVector3( -1, 0, 0 );
		const CVector3 CVector3::NEGATIVE_Y = CVector3( 0, -1, 0 );
		const CVector3 CVector3::NEGATIVE_Z = CVector3( 0, 0, -1 );
		const CVector3 CVector3::SIGN_MASK( -0.f ); // 1 << 31
		const CVector3 CVector3::UNIT( 1.0f, 1.0f, 1.0f );

		const CVector4 CVector4::ZERO = CVector4( 0, 0, 0, 0 );
		const CVector4 CVector4::X = CVector4( 1, 0, 0, 0 );
		const CVector4 CVector4::Y = CVector4( 0, 1, 0, 0 );
		const CVector4 CVector4::Z = CVector4( 0, 0, 1, 0 );
		const CVector4 CVector4::W = CVector4( 0, 0, 0, 1 );
		const CVector4 CVector4::NEGATIVE_X = CVector4( -1, 0, 0, 0 );
		const CVector4 CVector4::NEGATIVE_Y = CVector4( 0, -1, 0, 0 );
		const CVector4 CVector4::NEGATIVE_Z = CVector4( 0, 0, -1, 0 );
		const CVector4 CVector4::NEGATIVE_W = CVector4( 0, 0, 0, -1 );
		const CVector4 CVector4::SIGN_MASK( -0.f ); // 1 << 31
		const CVector4 CVector4::UNIT( 1.0f, 1.0f, 1.0f, 1.0f );
		const CVector4 CVector4::INF( 0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000 );
		const CVector4 CVector4::NOT_A_NUMBER( 0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000 );

		const CPoint CPoint::ZERO = CPoint( 0, 0 );
		const CPoint CPoint::X = CPoint( 1, 0);
		const CPoint CPoint::Y = CPoint( 0, 1 );
		const CPoint CPoint::NEGATIVE_X = CPoint( -1, 0 );
		const CPoint CPoint::NEGATIVE_Y = CPoint( 0, -1 );

		const CMatrix4 CMatrix4::IDENTITY = CMatrix4(	1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1 );

		const m128 CMatrix4::IDENTITY_R0 = { 1.0f, 0.0f, 0.0f, 0.0f };
		const m128 CMatrix4::IDENTITY_R1 = { 0.0f, 1.0f, 0.0f, 0.0f };
		const m128 CMatrix4::IDENTITY_R2 = { 0.0f, 0.0f, 1.0f, 0.0f };
		const m128 CMatrix4::IDENTITY_R3 = { 0.0f, 0.0f, 0.0f, 1.0f };

		const CMatrix3 CMatrix3::IDENTITY = CMatrix3(	1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1 );

		const m128 CMatrix3::IDENTITY_R0 = { 1.0f, 0.0f, 0.0f, 0.0f };
		const m128 CMatrix3::IDENTITY_R1 = { 0.0f, 1.0f, 0.0f, 0.0f };
		const m128 CMatrix3::IDENTITY_R2 = { 0.0f, 0.0f, 1.0f, 0.0f };
		const m128 CMatrix3::IDENTITY_R3 = { 0.0f, 0.0f, 0.0f, 1.0f };

		Vec2 CVector2::Abs(const Vec2& vecOther)
		{
#if defined( XST_NOT_USING_SSE )
			return Vec2( Math::Abs( vecOther.x ), Math::Abs( vecOther.y ) );
#else
			return Vec2( Math::Abs( vecOther.x ), Math::Abs( vecOther.y ) );
#endif
		}

		Vec2 CVector2::Abs() const
		{
#if defined( XST_NOT_USING_SSE )
			return Vec2( Math::Abs( x ), Math::Abs( y ) );
#else
			return Vec2( Math::Abs( x ), Math::Abs( y ) );
#endif
		}

		void CVector2::Abs()
		{
#if defined( XST_NOT_USING_SSE )
			x = Math::Abs( x );
			y = Math::Abs( y );
#else
			x = Math::Abs( x );
			y = Math::Abs( y );
#endif
		}

		Vec3 CVector3::Abs(const Vec3& vecOther)
		{
#if defined( XST_NOT_USING_SSE )
			return Vec3( Math::Abs( vecOther.x ), Math::Abs( vecOther.y ), Math::Abs( vecOther.z ) );
#else
			return Vec3( Math::Abs( vecOther.x ), Math::Abs( vecOther.y ), Math::Abs( vecOther.z ) );
#endif
		}

		Vec3 CVector3::Abs() const
		{
#if defined( XST_NOT_USING_SSE )
			return Vec3( Math::Abs( x ), Math::Abs( y ), Math::Abs( z ) );
#else
			return Vec3( Math::Abs( x ), Math::Abs( y ), Math::Abs( z ) );
#endif
		}

		void CVector3::Abs()
		{
#if defined( XST_NOT_USING_SSE )
			x = Math::Abs( x );
			y = Math::Abs( y );
			z = Math::Abs( z );
#else
			x = Math::Abs( x );
			y = Math::Abs( y );
			z = Math::Abs( z );
#endif
		}

		Vec4 CVector4::Abs(const Vec4& vecOther)
		{
#if defined( XST_NOT_USING_SSE )
			return Vec4( Math::Abs( vecOther.x ), Math::Abs( vecOther.y ), Math::Abs( vecOther.z ), Math::Abs( vecOther.w ) );
#else
			return _mm_andnot_ps( SIGN_MASK.v, vecOther.v );
#endif
		}

		Vec4 CVector4::Abs() const
		{
#if defined( XST_NOT_USING_SSE )
			return Vec4( Math::Abs( x ), Math::Abs( y ), Math::Abs( z ), Math::Abs( w ) );
#else
			return _mm_andnot_ps( SIGN_MASK.v, v );
#endif
		}

		void CVector4::Abs()
		{
#if defined( XST_NOT_USING_SSE )
			x = Math::Abs( x );
			y = Math::Abs( y );
			z = Math::Abs( z );
			w = Math::Abs( w );
#else
			v = _mm_andnot_ps( SIGN_MASK.v, v );
#endif
		}
		/*
		void CVector4::Transform(Vec4* pvecOut, const Vec4& vecVector, const Mtx4& mtxProj)
		{
#if defined(XST_SSE2)
			// Splat x,y,z and w
			Vec4 vTempX = XST_SSE_PERMUTE_PS( vecVector.v,_MM_SHUFFLE(0,0,0,0));
			Vec4 vTempY = XST_SSE_PERMUTE_PS( vecVector.v,_MM_SHUFFLE(1,1,1,1));
			Vec4 vTempZ = XST_SSE_PERMUTE_PS( vecVector.v,_MM_SHUFFLE(2,2,2,2));
			Vec4 vTempW = XST_SSE_PERMUTE_PS( vecVector.v,_MM_SHUFFLE(3,3,3,3));
			// Mul by the matrix
			vTempX = _mm_mul_ps(vTempX,mtxProj.r[0]);
			vTempY = _mm_mul_ps(vTempY,mtxProj.r[1]);
			vTempZ = _mm_mul_ps(vTempZ,mtxProj.r[2]);
			vTempW = _mm_mul_ps(vTempW,mtxProj.r[3]);
			// Add them all together
			vTempX = _mm_add_ps(vTempX,vTempY);
			vTempZ = _mm_add_ps(vTempZ,vTempW);
			vTempX = _mm_add_ps(vTempX,vTempZ);
			pvecOut->v = vTempX.v;
#else // XST_SSE2
			pvecOut->x = (mtxProj.m[0][0]*pvecOut->m128_f32[0])+(mtxProj.m[1][0]*pvecOut->m128_f32[1])+(mtxProj.m[2][0]*pvecOut->m128_f32[2])+(mtxProj.m[3][0]*pvecOut->m128_f32[3]);
			pvecOut->y = (mtxProj.m[0][1]*pvecOut->m128_f32[0])+(mtxProj.m[1][1]*pvecOut->m128_f32[1])+(mtxProj.m[2][1]*pvecOut->m128_f32[2])+(mtxProj.m[3][1]*pvecOut->m128_f32[3]);
			pvecOut->z = (mtxProj.m[0][2]*pvecOut->m128_f32[0])+(mtxProj.m[1][2]*pvecOut->m128_f32[1])+(mtxProj.m[2][2]*pvecOut->m128_f32[2])+(mtxProj.m[3][2]*pvecOut->m128_f32[3]);
			pvecOut->w = (mtxProj.m[0][3]*pvecOut->m128_f32[0])+(mtxProj.m[1][3]*pvecOut->m128_f32[1])+(mtxProj.m[2][3]*pvecOut->m128_f32[2])+(mtxProj.m[3][3]*pvecOut->m128_f32[3]);			
#endif
		}
		*/
		f32 Sqrt(cf32& fValue)
		{
	#if defined( XST_NOT_USING_SSE )
			return sqrtf( fValue );
	#else
			return _mm_sqrt_ss( _mm_load_ss( &fValue ) ).m128_f32[ 0 ];
	#endif
		}

		/*CRadian ACos(cf32& fValue)
		{
			if ( -1.0 < fValue )
			{
				if ( fValue < 1.0 )
					return CRadian(acos(fValue));
				else
					return CRadian(0.0);
			}
			else
			{
				return CRadian(PI);
			}
		}

		CRadian ASin(cf32& fValue)
		{
			if ( -1.0 < fValue )
			{
				if ( fValue < 1.0 )
					return CRadian(asin(fValue));
				else
					return CRadian(HALF_PI);
			}
			else
			{
				return CRadian(-HALF_PI);
			}
		}*/

	}//Math
}//xst


//#endif //XST_SS