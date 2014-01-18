#ifndef XST_MATH_H
#define XST_MATH_H


#include "XSTCommonTypes.h"
#include <cmath>

namespace XST
{
	namespace Math
	{
		using namespace XST::Types;

		#define XST_PI				3.141592653589793f
		#define XST_PI_D			3.14159265358979323846264338327950288
		#define XST_HALF_PI			1.5707963267948966192313216916398f
		#define XST_HALF_PI_D		1.5707963267948966192313216916398
		#define XST_2PI				6.283185307179586476925286766559f
		#define XST_2PI_D			6.283185307179586476925286766559
		#define XST_EPSILON			1e-03f
		
		static cf32 PI			= XST_PI;
		static cf64 PID			= XST_PI_D;
		static cf32 HALF_PI		= XST_HALF_PI;
		static cf64 HALF_PI_D	= XST_HALF_PI_D;
		static cf32 MUL_2PI		= XST_2PI;
		static cf64	MUL_2PI_D	= XST_2PI_D;
		static cf32 DIV_180_PI	= 180.0f / PI;
		static cf32 DIV_PI_180	= PI / 180.0f;
		static cf32 EPSILON		= XST_EPSILON;

		class CRadian;
		class CDegree;

		// SIMD
		#define XST_SSE_PERMUTE_PS( v, c ) _mm_shuffle_ps( v, v, c )

		class XST_API CRadian
		{
		public:

			f32		fValue;

		public:

		public:
			explicit CRadian ( f32 r=0 ) : fValue(r) {}
			CRadian ( const CDegree& d );
			CRadian& operator = ( const f32& f ) { fValue = f; return *this; }
			CRadian& operator = ( const CRadian& r ) { fValue = r.fValue; return *this; }
			CRadian& operator = ( const CDegree& d );

			f32 valueDegrees() const; // see bottom of this file
			f32 valueRadians() const { return fValue; }
			f32 valueAngleUnits() const;

			const CRadian& operator + () const { return *this; }
			CRadian operator + ( const CRadian& r ) const { return CRadian ( fValue + r.fValue ); }
			CRadian operator + ( const CDegree& d ) const;
			CRadian& operator += ( const CRadian& r ) { fValue += r.fValue; return *this; }
			CRadian& operator += ( const CDegree& d );
			CRadian operator - () const { return CRadian(-fValue); }
			CRadian operator - ( const CRadian& r ) const { return CRadian ( fValue - r.fValue ); }
			CRadian operator - ( const CDegree& d ) const;
			CRadian& operator -= ( const CRadian& r ) { fValue -= r.fValue; return *this; }
			CRadian& operator -= ( const CDegree& d );
			CRadian operator * ( f32 f ) const { return CRadian ( fValue * f ); }
			CRadian operator * ( const CRadian& f ) const { return CRadian ( fValue * f.fValue ); }
			CRadian& operator *= ( f32 f ) { fValue *= f; return *this; }
			CRadian operator / ( f32 f ) const { return CRadian ( fValue / f ); }
			CRadian& operator /= ( f32 f ) { fValue /= f; return *this; }

			bool operator <  ( const CRadian& r ) const { return fValue <  r.fValue; }
			bool operator <= ( const CRadian& r ) const { return fValue <= r.fValue; }
			bool operator == ( const CRadian& r ) const { return fValue == r.fValue; }
			bool operator != ( const CRadian& r ) const { return fValue != r.fValue; }
			bool operator >= ( const CRadian& r ) const { return fValue >= r.fValue; }
			bool operator >  ( const CRadian& r ) const { return fValue >  r.fValue; }

			xst_fi operator f32() const
			{
				return fValue;
			}
		};

		class XST_API CDegree
		{
			f32 fValue; // if you get an error here - make sure to define/typedef 'f32' first

		public:
			explicit CDegree ( f32 d=0 ) : fValue(d) {}
			CDegree ( const CRadian& r ) : fValue(r.valueDegrees()) {}
			CDegree& operator = ( const f32& f ) { fValue = f; return *this; }
			CDegree& operator = ( const CDegree& d ) { fValue = d.fValue; return *this; }
			CDegree& operator = ( const CRadian& r ) { fValue = r.valueDegrees(); return *this; }

			f32 valueDegrees() const { return fValue; }
			f32 valueRadians() const; // see bottom of this file
			f32 valueAngleUnits() const;

			const CDegree& operator + () const { return *this; }
			CDegree operator + ( const CDegree& d ) const { return CDegree ( fValue + d.fValue ); }
			CDegree operator + ( const CRadian& r ) const { return CDegree ( fValue + r.valueDegrees() ); }
			CDegree& operator += ( const CDegree& d ) { fValue += d.fValue; return *this; }
			CDegree& operator += ( const CRadian& r ) { fValue += r.valueDegrees(); return *this; }
			CDegree operator - () const { return CDegree(-fValue); }
			CDegree operator - ( const CDegree& d ) const { return CDegree ( fValue - d.fValue ); }
			CDegree operator - ( const CRadian& r ) const { return CDegree ( fValue - r.valueDegrees() ); }
			CDegree& operator -= ( const CDegree& d ) { fValue -= d.fValue; return *this; }
			CDegree& operator -= ( const CRadian& r ) { fValue -= r.valueDegrees(); return *this; }
			CDegree operator * ( f32 f ) const { return CDegree ( fValue * f ); }
			CDegree operator * ( const CDegree& f ) const { return CDegree ( fValue * f.fValue ); }
			CDegree& operator *= ( f32 f ) { fValue *= f; return *this; }
			CDegree operator / ( f32 f ) const { return CDegree ( fValue / f ); }
			CDegree& operator /= ( f32 f ) { fValue /= f; return *this; }

			bool operator <  ( const CDegree& d ) const { return fValue <  d.fValue; }
			bool operator <= ( const CDegree& d ) const { return fValue <= d.fValue; }
			bool operator == ( const CDegree& d ) const { return fValue == d.fValue; }
			bool operator != ( const CDegree& d ) const { return fValue != d.fValue; }
			bool operator >= ( const CDegree& d ) const { return fValue >= d.fValue; }
			bool operator >  ( const CDegree& d ) const { return fValue >  d.fValue; }
		};

		// CRadian because they required class CDegree to be defined
		xst_fi CRadian::CRadian ( const CDegree& d ) : fValue(d.valueRadians()) {
		}
		xst_fi CRadian& CRadian::operator = ( const CDegree& d ) {
			fValue = d.valueRadians(); return *this;
		}
		xst_fi CRadian CRadian::operator + ( const CDegree& d ) const {
			return CRadian ( fValue + d.valueRadians() );
		}
		xst_fi CRadian& CRadian::operator += ( const CDegree& d ) {
			fValue += d.valueRadians();
			return *this;
		}
		xst_fi CRadian CRadian::operator - ( const CDegree& d ) const {
			return CRadian ( fValue - d.valueRadians() );
		}
		xst_fi CRadian& CRadian::operator -= ( const CDegree& d ) {
			fValue -= d.valueRadians();
			return *this;
		}

		template<typename _T_>
		static xst_fi _T_ Min(const _T_& _t1, const _T_& _t2) 
		{ 
			return ( _t1 < _t2 )? _t1 : _t2; 
		}

		template<typename _T_>
		static xst_fi _T_ Max(const _T_& _t1, const _T_& _t2) 
		{ 
			return ( _t1 < _t2 )? _t2 : _t1; 
		}

		template<typename _T_>
		static xst_fi _T_ Clamp(const _T_& x, const _T_& a, const _T_& b) 
		{ 
			return x < a ? a : (x > b ? b : x); 
		}

		template<typename _T_>
		static xst_fi _T_ Sqr(const _T_& tValue) 
		{ 
			return tValue * tValue; 
		}

		static xst_fi f32 Abs(cf32& _fValue)
		{
			union { float a; unsigned int b; } f;
			f.a = _fValue; f.b &= 0x7FFFFFFFU;
			return f.a;
		}

		template<typename _T_>
		static xst_fi _T_ Abs(const _T_& tValue)
		{ 
			return (tValue < 0)? -tValue : tValue; 
		}

		static xst_fi f32 Lerp(cf32& fLeft, cf32& fRight, cf32& fWeight)
		{
			return fLeft * ( 1.0f - fWeight ) + fRight * fWeight;
		}

		static xst_fi f32 Lerp2(cf32& fLeft, cf32& fRight, cf32& fWeight)
		{
			return fLeft + fWeight * ( fRight - fLeft );
		}

		static xst_fi void SetMatrix4x4Identity(f32** afMtx)
		{
			afMtx[0][0] = 0.0f; afMtx[0][1] = 0.0f; afMtx[0][2] = 0.0f; afMtx[0][3] = 1.0f;
			afMtx[0][0] = 0.0f; afMtx[0][1] = 0.0f; afMtx[0][2] = 1.0f; afMtx[0][3] = 0.0f;
			afMtx[0][0] = 0.0f; afMtx[0][1] = 1.0f; afMtx[0][2] = 0.0f; afMtx[0][3] = 0.0f;
			afMtx[0][0] = 1.0f; afMtx[0][1] = 0.0f; afMtx[0][2] = 0.0f; afMtx[0][3] = 0.0f;
		}

		static xst_fi void SetMatrix4x4Identity(f32* afMtx[4][4])
		{
			(*afMtx)[0][0] = 0.0f; (*afMtx)[0][1] = 0.0f; (*afMtx)[0][2] = 0.0f; (*afMtx)[0][3] = 1.0f;
			(*afMtx)[0][0] = 0.0f; (*afMtx)[0][1] = 0.0f; (*afMtx)[0][2] = 1.0f; (*afMtx)[0][3] = 0.0f;
			(*afMtx)[0][0] = 0.0f; (*afMtx)[0][1] = 1.0f; (*afMtx)[0][2] = 0.0f; (*afMtx)[0][3] = 0.0f;
			(*afMtx)[0][0] = 1.0f; (*afMtx)[0][1] = 0.0f; (*afMtx)[0][2] = 0.0f; (*afMtx)[0][3] = 0.0f;
		}

		static xst_fi f32 DegreesToRadians(cf32& fDegrees)
		{ return fDegrees * DIV_PI_180; }

		static xst_fi f32 RadiansToDegrees(cf32& fRadians)
		{ return fRadians * DIV_180_PI; }

		static xst_fi CRadian AngleToRadian(cf32& fDegreeAngle)
		{ 
			return CRadian( DegreesToRadians( fDegreeAngle ) ); 
		}

		f32 Sqrt(cf32& fValue);


		static xst_fi f32 Trunc(cf32& fValue)
		{
			return ( fValue < 0.0f )? ceilf( fValue ) : floorf( fValue );
		}

		static xst_fi f32 Frac(cf32& fValue)
		{
			return fValue - Trunc( fValue );
		}

		static xst_fi CRadian ACos (cf32& fValue)
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

		static xst_fi CRadian ASin (cf32& fValue)
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
		}

		static inline CRadian ATan (cf32& fValue) { return CRadian(atan(fValue)); }
		static inline CRadian ATan2 (cf32& fY, cf32& fX) { return CRadian(atan2(fY,fX)); }

		static xst_fi f32 Cos(cf32& fValue)
		{
			return cosf( fValue );
		}

		static xst_fi f32 Sin(cf32& fValue)
		{
			return sinf( fValue );
		}

		static xst_fi f32 InvSqrt(f32 fValue)
		{
			float xhalf = 0.5f*fValue;
			int i = *(int*)&fValue;
			i = 0x5f3759df - (i>>1);
			fValue = *(float*)&i;
			fValue = fValue*(1.5f - xhalf*fValue*fValue);
			return fValue;
		}

		xst_fi f32 CRadian::valueDegrees() const
		{
			return Math::RadiansToDegrees ( fValue );
		}

		xst_fi f32 CRadian::valueAngleUnits() const
		{
			return Math::RadiansToDegrees( fValue );
		}

		xst_fi f32 CDegree::valueRadians() const
		{
			return Math::DegreesToRadians ( fValue );
		}

		xst_fi f32 CDegree::valueAngleUnits() const
		{
			return Math::DegreesToRadians( fValue );
		}


	}//Math


}//XST

#endif
