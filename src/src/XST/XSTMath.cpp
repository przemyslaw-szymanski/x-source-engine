#include "XST/XSTMath.h"
#include "XST/XSTCMatrix4.h"
#include "XST/XSTCPoint.h"

namespace XST
{
	namespace Math
	{

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




////#if defined (XST_SSE)
//		//SSE2
//		xst_fi Vec3 MulSSE2(const Vec3& _vecLeft, const Vec3& _vecRight)
//		{
//			return _mm_mul_ps(_vecLeft.xyzw, _vecRight.xyzw);
//		}
//
//		xst_fi Vec3 AddSSE2(const Vec3& _vecLeft, const Vec3& _vecRight)
//		{
//			return _mm_add_ps(_vecLeft.xyzw, _vecRight.xyzw);
//		}
//
//		xst_fi Vec3 SubSSE2(const Vec3& _vecLeft, const Vec3& _vecRight)
//		{
//			return _mm_sub_ps(_vecLeft.xyzw, _vecRight.xyzw);
//		}
//
//		xst_fi Vec3 DivSSE2(const Vec3& _vecLeft, const Vec3& _vecRight)
//		{
//			return _mm_div_ps(_vecLeft.xyzw, _vecRight.xyzw);
//		}
//
//		xst_fi Vec3 MulSSE2(const Vec3& _vecLeft, const f32& _fScalar)
//		{
//			return _mm_mul_ps( _vecLeft.xyzw, _mm_load1_ps(&_fScalar) );
//		}
//
//		xst_fi Vec3 AddSSE2(const Vec3& _vecLeft, const f32& _fScalar)
//		{
//			return _mm_add_ps( _vecLeft.xyzw, _mm_load1_ps(&_fScalar) );
//		}
//
//		xst_fi Vec3 SubSSE2(const Vec3& _vecLeft, const f32& _fScalar)
//		{
//			return _mm_sub_ps( _vecLeft.xyzw, _mm_load1_ps(&_fScalar) );
//		}
//
//		xst_fi Vec3 DivSSE2(const Vec3& _vecLeft, const f32& _fScalar)
//		{
//			return _mm_div_ps( _vecLeft.xyzw, _mm_load1_ps(&_fScalar) );
//		}
//
//		xst_fi Vec3 SqrtSSE2(const Vec3& _vec)
//		{
//			return _mm_sqrt_ps( _vec.xyzw );
//		}
//
//		xst_i f32 DotSSE2(const Vec3& _vecLeft, const Vec3& _vecRight)
//		{
//			/*
//			013673AA  mov         eax,dword ptr [ebx+0Ch]
//			013673AD  movaps      xmm0,xmmword ptr [eax]
//			013673B0  mov         ecx,dword ptr [ebx+8]
//			013673B3  movaps      xmm1,xmmword ptr [ecx]
//			013673B6  mulps       xmm1,xmm0
//			013673B9  movaps      xmmword ptr [ebp-180h],xmm1
//			013673C0  movaps      xmm0,xmmword ptr [ebp-180h]
//			013673C7  movaps      xmmword ptr [ebp-20h],xmm0
//			*/
//			xst_m128 r1 = _mm_mul_ps( _vecLeft.xyzw , _vecRight.xyzw );
//			/*
//			013673CB  movaps      xmm0,xmmword ptr [ebp-20h]
//			013673CF  movaps      xmm1,xmmword ptr [ebp-20h]
//			013673D3  movhlps     xmm1,xmm0
//			013673D6  movaps      xmmword ptr [ebp-160h],xmm1
//			013673DD  movaps      xmm0,xmmword ptr [ebp-160h]
//			013673E4  movaps      xmm1,xmmword ptr [ebp-20h]
//			013673E8  addps       xmm1,xmm0
//			013673EB  movaps      xmmword ptr [ebp-140h],xmm1
//			013673F2  movaps      xmm0,xmmword ptr [ebp-140h]
//			013673F9  movaps      xmmword ptr [ebp-20h],xmm0
//			*/
//			r1 = _mm_add_ps( r1, _mm_movehl_ps(r1, r1));
//			/*
//			013673FD  movaps      xmm0,xmmword ptr [ebp-20h]
//			01367401  movaps      xmm1,xmmword ptr [ebp-20h]
//			01367405  shufps      xmm1,xmm0,0E5h
//			01367409  movaps      xmmword ptr [ebp-120h],xmm1
//			01367410  movaps      xmm0,xmmword ptr [ebp-120h]
//			01367417  movaps      xmm1,xmmword ptr [ebp-20h]
//			0136741B  addps       xmm1,xmm0
//			0136741E  movaps      xmmword ptr [ebp-100h],xmm1
//			01367425  movaps      xmm0,xmmword ptr [ebp-100h]
//			0136742C  movaps      xmmword ptr [ebp-20h],xmm0
//			*/
//			r1 = _mm_add_ps( r1, _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(3, 2, 1, 1)));
//			//f32 fResult;
//			//_mm_store_ss( &fResult, r1 ); // zapisz 1 float
//			//return fResult;
//			return r1.m128_f32[0];
//		}
//
//		xst_fi f32 LengthSSE2(const Vec3& _vecLeft)
//		{
//			xst_m128 r1 = _mm_mul_ps( _vecLeft.xyzw , _vecLeft.xyzw );
//			r1 = _mm_add_ps( r1, _mm_movehl_ps(r1, r1));
//			r1 = _mm_add_ps( r1, _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(3, 2, 1, 1)));
//			r1 = _mm_sqrt_ps( r1 );
//			//f32 fResult;
//			//_mm_store_ss( &fResult, r1 ); // zapisz 1 float
//			//return fResult;
//			return r1.m128_f32[0];
//		}
//
//		xst_fi Vec3 NormalizeSSE2(const Vec3& _vecLeft)
//		{
//			//_vecLeft->xyzw / LengthSSE2(_vecLeft)
//
//			//Calc vector length
//			m128 l = _mm_mul_ps( _vecLeft.xyzw , _vecLeft.xyzw );
//			l = _mm_add_ps( l, _mm_movehl_ps(l, l));
//			l = _mm_add_ps( l, _mm_shuffle_ps(l, l, _MM_SHUFFLE(3, 2, 1, 1)));
//			l = _mm_sqrt_ps( l );
//
//			return _mm_div_ps( _vecLeft.xyzw, l );
//		}
//
//		//SSE2 (for class use)
//		xst_fi Vec3 MulSSE2(const Vec3* _vecLeft, const Vec3& _vecRight)
//		{
//			return _mm_mul_ps(_vecLeft->xyzw, _vecRight.xyzw);
//		}
//
//		xst_fi Vec3 AddSSE2(const Vec3* _vecLeft, const Vec3& _vecRight)
//		{
//			return _mm_add_ps(_vecLeft->xyzw, _vecRight.xyzw);
//		}
//
//		xst_fi Vec3 SubSSE2(const Vec3* _vecLeft, const Vec3& _vecRight)
//		{
//			return _mm_sub_ps(_vecLeft->xyzw, _vecRight.xyzw);
//		}
//
//		xst_fi Vec3 DivSSE2(const Vec3* _vecLeft, const Vec3& _vecRight)
//		{
//			return _mm_div_ps(_vecLeft->xyzw, _vecRight.xyzw);
//		}
//
//		xst_fi Vec3 MulSSE2(const Vec3* _vecLeft, const f32& _fScalar)
//		{
//			return _mm_mul_ps( _vecLeft->xyzw, _mm_load1_ps(&_fScalar) );
//		}
//
//		xst_fi Vec3 AddSSE2(const Vec3* _vecLeft, const f32& _fScalar)
//		{
//			return _mm_add_ps( _vecLeft->xyzw, _mm_load1_ps(&_fScalar) );
//		}
//
//		xst_fi Vec3 SubSSE2(const Vec3* _vecLeft, const f32& _fScalar)
//		{
//			return _mm_sub_ps( _vecLeft->xyzw, _mm_load1_ps(&_fScalar) );
//		}
//
//		xst_fi Vec3 DivSSE2(const Vec3* _vecLeft, const f32& _fScalar)
//		{
//			return _mm_div_ps( _vecLeft->xyzw, _mm_load1_ps(&_fScalar) );
//		}
//
//		xst_fi Vec3 SqrtSSE2(const Vec3* _vec)
//		{
//			return _mm_sqrt_ps( _vec->xyzw );
//		}
//
//		xst_fi f32 DotSSE2(const Vec3* _vecLeft, const Vec3& _vecRight)
//		{
//			m128 r1 = _mm_mul_ps( _vecLeft->xyzw , _vecRight.xyzw );
//			r1 = _mm_add_ps( r1, _mm_movehl_ps(r1, r1));
//			r1 = _mm_add_ps( r1, _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(3, 2, 1, 1)));
//			f32 fResult;
//			_mm_store_ss( &fResult, r1 ); // zapisz 1 float
//			return fResult;
//		}
//
//		xst_fi void AddEqSSE2(Vec3* _vecLeft, const Vec3& _vecRight)
//		{
//			_vecLeft->xyzw = _mm_add_ps( _vecLeft->xyzw, _vecRight.xyzw );
//		}
//
//		xst_fi void SubEqSSE2(Vec3* _vecLeft, const Vec3& _vecRight)
//		{
//			_vecLeft->xyzw = _mm_sub_ps( _vecLeft->xyzw, _vecRight.xyzw );
//		}
//
//		xst_fi void MulEqSSE2(Vec3* _vecLeft, const Vec3& _vecRight)
//		{
//			_vecLeft->xyzw = _mm_mul_ps( _vecLeft->xyzw, _vecRight.xyzw );
//		}
//
//		xst_fi void DivEqSSE2(Vec3* _vecLeft, const Vec3& _vecRight)
//		{
//			_vecLeft->xyzw = _mm_div_ps( _vecLeft->xyzw, _vecRight.xyzw );
//		}
//
//		xst_fi void AddEqSSE2(Vec3* _vecLeft, const f32& _fScalar)
//		{
//			_vecLeft->xyzw = _mm_add_ps( _vecLeft->xyzw, _mm_load1_ps( &_fScalar ) );
//		}
//
//		xst_fi void SubEqSSE2(Vec3* _vecLeft, const f32& _fScalar)
//		{
//			_vecLeft->xyzw = _mm_sub_ps( _vecLeft->xyzw, _mm_load1_ps( &_fScalar ) );
//		}
//
//		xst_fi void MulEqSSE2(Vec3* _vecLeft, const f32& _fScalar)
//		{
//			_vecLeft->xyzw = _mm_mul_ps( _vecLeft->xyzw, _mm_load1_ps( &_fScalar ) );
//		}
//
//		xst_fi void DivEqSSE2(Vec3* _vecLeft, const f32& _fScalar)
//		{
//			_vecLeft->xyzw = _mm_div_ps( _vecLeft->xyzw, _mm_load1_ps( &_fScalar ) );
//		}
//
//		xst_fi f32 LengthSSE2(const Vec3* _vecLeft)
//		{
//			//return sqrtf( _vecLeft->x * _vecLeft->x + _vecLeft->y * _vecLeft->y + _vecLeft->z * _vecLeft->z );
//			m128 r1 = _mm_mul_ps( _vecLeft->xyzw , _vecLeft->xyzw );
//			//r1 = _mm_add_ps( r1, _mm_movehl_ps(r1, r1) );
//			//r1 = _mm_add_ps( r1, _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(3, 2, 1, 1)) );
//			r1 = _mm_sqrt_ps( r1 );
//			f32 fResult;
//			_mm_store_ss( &fResult, r1 ); // zapisz 1 float
//			return fResult;
//		}
//
//		xst_fi Vec3 NormalizeSSE2(const Vec3* _vecLeft)
//		{
//			//_vecLeft->xyzw / LengthSSE2(_vecLeft)
//
//			//Calc vector length
//			m128 l = _mm_mul_ps( _vecLeft->xyzw , _vecLeft->xyzw );
//			l = _mm_add_ps( l, _mm_movehl_ps(l, l));
//			l = _mm_add_ps( l, _mm_shuffle_ps(l, l, _MM_SHUFFLE(3, 2, 1, 1)));
//			l = _mm_sqrt_ps( l );
//
//			return _mm_div_ps( _vecLeft->xyzw, l );
//		}
//
//		//SSE4
//		xst_fi f32	DotSSE4(const Vec3& _vecLeft, const Vec3& _vecRight)
//		{
//			return xst_m128( _mm_dp_ps( _vecLeft.xyzw, _vecRight.xyzw, 0x55 ) ).m128_f32[0];
//		}
//
//		//SSE4 (class)
//		xst_fi f32	DotSSE4(const Vec3* _vecLeft, const Vec3& _vecRight)
//		{
//			return xst_m128( _mm_dp_ps( _vecLeft->xyzw, _vecRight.xyzw, 0x55 ) ).m128_f32[0];
//		}

//#endif //XST_SS