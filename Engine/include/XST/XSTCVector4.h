#ifndef XST_CVECTOR4_H
#define XST_CVECTOR4_H

#include "XSTMath.h"
#include "XSTCVector3.h"

namespace XST
{
	namespace Math
	{
		class XST_ALIGN(16) CVector4;
		typedef XST_ALIGN(16) CVector4 Vec4;

		class XST_API XST_ALIGN( 16 ) CVector4
		{
			public:

				union
				{
					struct	{ f32 x, y, z, w; };
					f32		a[ 4 ];
					m128	v;
				};

			public:

				static const CVector4	ZERO;
				static const CVector4	UNIT;
				static const CVector4	SIGN_MASK; // 1 << 31
				static const CVector4	X;
				static const CVector4	Y;
				static const CVector4	Z;
				static const CVector4	W;
				static const CVector4	NEGATIVE_X;
				static const CVector4	NEGATIVE_Y;
				static const CVector4	NEGATIVE_Z;
				static const CVector4	NEGATIVE_W;
				static const CVector4	INF; // infinity
				static const CVector4	NOT_A_NUMBER;

			public:

				xst_fi CVector4() {}
				xst_fi CVector4(const Vec4& _vec) : x( _vec.x ), y( _vec.y ), z( _vec.z ), w( _vec.w ) {}
				xst_fi CVector4(Vec4&& _vec) { x = _vec.x; y = _vec.y; z = _vec.z; w = _vec.w; }
				xst_fi CVector4(const CVector3& _vec) : x( _vec.x ), y( _vec.y ), z( _vec.z ), w( 0.0f ) {}
				xst_fi CVector4(const CVector2& _vec) : x( _vec.x ), y( _vec.y ), z( 0.0f ), w( 0.0f ) {}
				xst_fi CVector4(const CVector3& _vec, cf32& fW) : x( _vec.x ), y( _vec.y ), z( _vec.z ), w( fW ) {}
				xst_fi CVector4(const CVector2& _vec, cf32& fZ, cf32& fW) : x( _vec.x ), y( _vec.y ), z( fZ ), w( fW ) {}
				xst_fi CVector4(const f32& _fValue) : x(_fValue), y(_fValue), z(_fValue), w( _fValue ) {}
				xst_fi CVector4(const f32& _fX, const f32& _fY, const f32& _fZ, cf32& _fW) : x(_fX), y(_fY), z(_fZ), w(_fW) {}
				xst_fi CVector4(const f32 _afValues[4]) : x( _afValues[ 0 ] ), y( _afValues[ 1 ] ), z( _afValues[ 2 ] ), w( _afValues[ 3 ] ) {}
				xst_fi CVector4(const m128& vec) : v( vec ) {}

				~CVector4() {}

				xst_fi bool operator==(const Vec4& _vecRight) const
				{
					return x == _vecRight.x && y == _vecRight.y && z == _vecRight.z && w == _vecRight.w;
				}

				xst_fi bool operator>(const Vec4& _vecRight) const
				{
					return x > _vecRight.x && y > _vecRight.y && z > _vecRight.z && w > _vecRight.w;
				}

				xst_fi bool operator>=(const Vec4& _vecRight) const
				{
					return x >= _vecRight.x && y >= _vecRight.y && z >= _vecRight.z && w >= _vecRight.w;
				}

				xst_fi bool operator<(const Vec4& _vecRight) const
				{
					return x < _vecRight.x && y < _vecRight.y && z < _vecRight.z && w < _vecRight.w;
				}

				xst_fi bool operator<=(const Vec4& _vecRight) const
				{
					return x <= _vecRight.x && y <= _vecRight.y && z <= _vecRight.z && w <= _vecRight.w;
				}

				xst_fi bool operator!=(const Vec4& _vecRight) const
				{
					return x != _vecRight.x && y != _vecRight.y && z != _vecRight.z && w != _vecRight.w;
				}

				xst_fi Vec4 operator+(const Vec4& _vecRight) const
				{
					return Vec4( x + _vecRight.x, y + _vecRight.y, z + _vecRight.z, w + _vecRight.w );
				}

				xst_fi void operator+=(const Vec4& _vecRight)
				{
					x += _vecRight.x; y += _vecRight.y; z += _vecRight.z; w += _vecRight.w;
				}

				xst_fi Vec4 operator-(const Vec4& _vecRight) const
				{
					return Vec4( x - _vecRight.x, y - _vecRight.y, z - _vecRight.z, w - _vecRight.w );
				}

				xst_fi Vec4 operator-() const
				{
					return Vec4( -x, -y, -z, -w );
				}

				xst_fi void operator-=(const Vec4& _vecRight)
				{
					x -= _vecRight.x; y -= _vecRight.y; z -= _vecRight.z; w -= _vecRight.w;
				}

				xst_fi Vec4 operator*(const Vec4& _vecRight) const
				{
					return Vec4( x * _vecRight.x, y * _vecRight.y, z * _vecRight.z, w * _vecRight.w );
				}

				xst_fi Vec4 operator*(const f32& fValue) const
				{
					return Vec4( x * fValue, y * fValue, z * fValue, w * fValue );
				}

				xst_fi void operator*=(const Vec4& _vecRight)
				{
					x *= _vecRight.x; y *= _vecRight.y; z *= _vecRight.z; w *= _vecRight.w;
				}

				xst_fi Vec4 operator/(const Vec4& _vecRight) const
				{
					return Vec4( x / _vecRight.x, y / _vecRight.y, z / _vecRight.z, w / _vecRight.w );
				}

				xst_fi void operator/=(const Vec4& _vecRight)
				{
					x /= _vecRight.x; y /= _vecRight.y; z /= _vecRight.z; w /= _vecRight.w;
				}

				xst_fi bool Eq(const Vec4& _vecRight) const
				{
					return x == _vecRight.x && y == _vecRight.y && z == _vecRight.z && w == _vecRight.w;
				}

				xst_fi bool Gt(const Vec4& _vecRight) const
				{
					return x > _vecRight.x && y > _vecRight.y && z > _vecRight.z && w > _vecRight.w;
				}

				xst_fi bool Ge(const Vec4& _vecRight) const
				{
					return x >= _vecRight.x && y >= _vecRight.y && z >= _vecRight.z && w >= _vecRight.w;
				}

				xst_fi bool Lt(const Vec4& _vecRight) const
				{
					return x < _vecRight.x && y < _vecRight.y && z < _vecRight.z && w <= _vecRight.w;
				}

				xst_fi bool Le(const Vec4& _vecRight) const
				{
					return x <= _vecRight.x && y <= _vecRight.y && z <= _vecRight.z && w <= _vecRight.w;
				}

				xst_fi bool Ne(const Vec4& _vecRight) const
				{
					return x != _vecRight.x && y != _vecRight.y && z != _vecRight.z && w != _vecRight.w;
				}

				xst_fi const Vec4& operator=(const Vec4& _vecRight)
				{
					x = _vecRight.x; y = _vecRight.y; z = _vecRight.z; w = _vecRight.w;
					return (*this);
				}

				xst_fi const Vec4& operator=(const m128& vecRight)
				{
					v = vecRight;
					return (*this);
				}

				xst_fi f32 operator[](const cu32& uiID) const
				{
					return a[ uiID ];
				}

				xst_fi operator m128() const
				{
					m128 ret;
					ret.m128_f32[ 0 ] = v.m128_f32[ 0 ];
					ret.m128_f32[ 1 ] = v.m128_f32[ 1 ];
					ret.m128_f32[ 2 ] = v.m128_f32[ 2 ];
					ret.m128_f32[ 3 ] = v.m128_f32[ 3 ];
					return ret;
				}

				xst_fi operator Vec3() const
				{
					return Vec3( x, y, z );
				}

				xst_fi operator Vec2() const
				{
					return Vec2( x, y );
				}

				xst_fi void Set(const Vec4& _vecRight)
				{
					x = _vecRight.x; y = _vecRight.y; z = _vecRight.z; w = _vecRight.w;
				}

				xst_fi void Set(cf32& fValue)
				{
					x = fValue; y = fValue; z = fValue; w = fValue;
				}

				xst_fi void Set(const f32& _fX, const f32& _fY, const f32& _fZ, cf32& _fW)
				{
					x = _fX; y = _fY; z = _fZ; w = _fW;
				}

				xst_fi const u32 GetSize() const
				{
					return 4;
				}

				xst_fi Vec4 Add(const Vec4& _vecRight) const
				{
					return Vec4( x + _vecRight.x, y + _vecRight.y, z + _vecRight.z, w + _vecRight.w );
				}

				xst_fi void AddAssign(const Vec4& _vecRight)
				{
					x += _vecRight.x; y += _vecRight.y; z += _vecRight.z; w += _vecRight.w;
				}

				xst_fi Vec4 Sub(const Vec4& _vecRight) const
				{
					return Vec4( x - _vecRight.x, y - _vecRight.y, z - _vecRight.z, w - _vecRight.w );
				}

				xst_fi void SubAssign(const Vec4& _vecRight)
				{
					x -= _vecRight.x; y -= _vecRight.y; z -= _vecRight.z; w -= _vecRight.w;
				}

				xst_fi Vec4 Mul(const Vec4& _vecRight) const
				{
					return Vec4( x * _vecRight.x, y * _vecRight.y, z * _vecRight.z, w * _vecRight.w );
				}

				xst_fi void  MulAssign(const Vec4& _vecRight)
				{
					x *= _vecRight.x; y *= _vecRight.y; z *= _vecRight.z; w *= _vecRight.w;
				}

				xst_fi Vec4 Div(const Vec4& _vecRight) const
				{
					return Vec4( x / _vecRight.x, y / _vecRight.y, z / _vecRight.z, w / _vecRight.w );
				}

				xst_fi void DivAssign(const Vec4& _vecRight)
				{
					x /= _vecRight.x; y /= _vecRight.y; z /= _vecRight.z; w /= _vecRight.w;
				}

				xst_fi void AddAssign(const f32& _fScalar)
				{
					x += _fScalar; y += _fScalar; z += _fScalar; w += _fScalar;
				}

				xst_fi void SubAssign(const f32& _fScalar)
				{
					x -= _fScalar; y -= _fScalar; z -= _fScalar; w -= _fScalar;
				}

				xst_fi void MulAssign(const f32& _fScalar)
				{
					x *= _fScalar; y *= _fScalar; z *= _fScalar; w *= _fScalar;
				}

				xst_fi void DivAssign(const f32& _fScalar)
				{
					x /= _fScalar; y /= _fScalar; z /= _fScalar; w /= _fScalar;
				}

				xst_fi void Add(const Vec4& vecLeft, const Vec4& vecRight)
				{
					x = vecLeft.x + vecRight.x;
					y = vecLeft.y + vecRight.y;
					z = vecLeft.z + vecRight.z;
					w = vecLeft.w + vecRight.w;
				}

				xst_fi void Sub(const Vec4& vecLeft, const Vec4& vecRight)
				{
					x = vecLeft.x - vecRight.x;
					y = vecLeft.y - vecRight.y;
					z = vecLeft.z - vecRight.z;
					w = vecLeft.w - vecRight.w;
				}

				xst_fi void Div(const Vec4& vecLeft, const Vec4& vecRight)
				{
					x = vecLeft.x / vecRight.x;
					y = vecLeft.y / vecRight.y;
					z = vecLeft.z / vecRight.z;
					w = vecLeft.w / vecRight.w;
				}

				xst_fi void Mul(const Vec4& vecLeft, const Vec4& vecRight)
				{
					x = vecLeft.x * vecRight.x;
					y = vecLeft.y * vecRight.y;
					z = vecLeft.z * vecRight.z;
					w = vecLeft.w * vecRight.w;
				}

				xst_fi f32 Length() const
				{
					return Math::Sqrt( x * x + y * y + z * z + w * w );
				}

				xst_fi f32 SquaredLength() const
				{
					return x * x + y * y + z * z + w * w;
				}

				xst_fi f32 Distance(const Vec4& RightVec) const
				{
					return ( *this - RightVec ).Length();
				}

				xst_fi f32 SquareDistance(const Vec4& RightVec) const
				{
					return ( *this - RightVec ).SquaredLength();
				}

				xst_fi f32 Normalize()
				{
#if defined( XST_SSE2 )
					Vec4 vLengthSq = _mm_mul_ps(v,v);
					Vec4 vTemp = XST_SSE_PERMUTE_PS(vLengthSq,_MM_SHUFFLE(2,1,2,1));
					vLengthSq = _mm_add_ss(vLengthSq,vTemp);
					vTemp = XST_SSE_PERMUTE_PS(vTemp,_MM_SHUFFLE(1,1,1,1));
					vLengthSq = _mm_add_ss(vLengthSq,vTemp);
					vLengthSq = XST_SSE_PERMUTE_PS(vLengthSq,_MM_SHUFFLE(0,0,0,0));
					// Prepare for the division
					Vec4 vResult = _mm_sqrt_ps(vLengthSq);
					// Create zero with a single instruction
					Vec4 vZeroMask = _mm_setzero_ps();
					// Test for a divide by zero (Must be FP to detect -0.0)
					vZeroMask = _mm_cmpneq_ps(vZeroMask,vResult);
					// Failsafe on zero (Or epsilon) length planes
					// If the length is infinity, set the elements to zero
					vLengthSq = _mm_cmpneq_ps(vLengthSq, Vec4::INF );
					// Divide to perform the normalization
					vResult = _mm_div_ps(v,vResult);
					// Any that are infinity, set to zero
					vResult = _mm_and_ps(vResult,vZeroMask);
					// Select qnan or result based on infinite length
					Vec4 vTemp1 = _mm_andnot_ps(vLengthSq, Vec4::NOT_A_NUMBER );
					Vec4 vTemp2 = _mm_and_ps(vResult,vLengthSq);
					vResult = _mm_or_ps(vTemp1,vTemp2);
					v = vResult.v;
					return vLengthSq.x;
#else
					f32 fLen = Length();
					if( fLen > 1e-08 )
					{
						f32 fInvLen = 1.0f / fLen;
						x *= fInvLen;
						y *= fInvLen;
						z *= fInvLen;
						w *= fInvLen;
					}

					return fLen;
#endif
				}

				xst_fi f32 Dot(const Vec4& RightVec) const
				{
					return x * RightVec.x + y * RightVec.y + z * RightVec.z + w * RightVec.w;
				}

				xst_fi Vec4 MidPoint(const Vec4& VecRight) const
				{
					return Vec4(	( x + VecRight.x ) * 0.5f, 
									( y + VecRight.y ) * 0.5f,
									( z + VecRight.z ) * 0.5f,
									( w + VecRight.w ) * 0.5f);
				}

				/*xst_fi void Transform(const Mtx4& mtxProj)
				{
					VectorTransform( this, *this, mtxProj );
				}*/

				xst_fi void Reciprocal(const Vec4& vecOther)
				{
					VectorReciprocal( this, vecOther );
				}

				xst_fi static void VectorReciprocal(Vec4* pvecOut, const Vec4& vecOther)
				{
#if defined( XST_SSE2)
					pvecOut->v = _mm_div_ps( Vec4::UNIT.v, vecOther.v );
#else
					pvecOut->x = 1.f / vecOther.x;
					pvecOut->y = 1.f / vecOther.y;
					pvecOut->z = 1.f / vecOther.z;
					pvecOut->w = 1.f / vecOther.w;
#endif // XST_SSE2
				}

				xst_fi static Vec4 VectorReciprocalRet(const Vec4& vecOther)
				{
					Vec4 vecRet;
					VectorReciprocal( &vecRet, vecOther );
					return vecRet;
				}

				template<u8 _COMPONENT_>
				xst_fi static void VectorSplat(Vec4* pvecOut, const Vec4& vecOther)
				{
#if defined(XST_SSE2)
					pvecOut->v = XST_SSE_PERMUTE_PS( vecOther.v, _MM_SHUFFLE( _COMPONENT_, _COMPONENT_, _COMPONENT_, _COMPONENT_ ) );
#else
					pvecOut->Set( vecOther.x );
#endif // XST_SSE2
				}

				template<u8 _COMPONENT_>
				xst_fi static Vec4 VectorSplatRet(const Vec4& vecOther)
				{
					Vec4 vecRet;
					VectorSplat< _COMPONENT_ >( &vecRet, vecOther );
					return vecRet;
				}

				xst_fi static void VectorSplatX(Vec4* pvecOut, const Vec4& vecOther)
				{
#if defined(XST_SSE2)
					pvecOut->v = XST_SSE_PERMUTE_PS( vecOther.v, _MM_SHUFFLE( 0, 0, 0, 0 ) );
#else
					pvecOut->Set( vecOther.x );
#endif // XST_SSE2
				}

				xst_fi static void VectorSplatY(Vec4* pvecOut, const Vec4& vecOther)
				{
#if defined(XST_SSE2)
					pvecOut->v = XST_SSE_PERMUTE_PS( vecOther.v, _MM_SHUFFLE( 1, 1, 1, 1 ) );
#else
					pvecOut->Set( vecOther.y );
#endif // XST_SSE2
				}

				xst_fi static void VectorSplatZ(Vec4* pvecOut, const Vec4& vecOther)
				{
#if defined(XST_SSE2)
					pvecOut->v = XST_SSE_PERMUTE_PS( vecOther.v, _MM_SHUFFLE( 2, 2, 2, 2 ) );
#else
					pvecOut->Set( vecOther.z );
#endif // XST_SSE2
				}

				xst_fi static void VectorSplatW(Vec4* pvecOut, const Vec4& vecOther)
				{
#if defined(XST_SSE2)
					pvecOut->v = XST_SSE_PERMUTE_PS( vecOther.v, _MM_SHUFFLE( 3, 3, 3, 3 ) );
#else
					pvecOut->Set( vecOther.w );
#endif // XST_SSE2
				}

				xst_fi static Vec4 VectorSplatXRet(const Vec4& vecOther)
				{
					Vec4 vecRet;
					VectorSplatX( &vecRet, vecOther );
					return vecRet;
				}

				xst_fi static Vec4 VectorSplatYRet(const Vec4& vecOther)
				{
					Vec4 vecRet;
					VectorSplatY( &vecRet, vecOther );
					return vecRet;
				}

				xst_fi static Vec4 VectorSplatZRet(const Vec4& vecOther)
				{
					Vec4 vecRet;
					VectorSplatZ( &vecRet, vecOther );
					return vecRet;
				}

				xst_fi static Vec4 VectorSplatWRet(const Vec4& vecOther)
				{
					Vec4 vecRet;
					VectorSplatW( &vecRet, vecOther );
					return vecRet;
				}

				Vec4				Abs() const;
				void				Abs();
				static Vec4			Abs(const Vec4& vecOther);
				xst_fi static f32	Dot(const Vec4& vecLeft, const Vec4& vecRight);
				//static void Transform(Vec4* pvecOut, const Vec4& vecVector, const Mtx4& mtxProj);
			
		};

		static xst_fi xst_ostream& operator<<(xst_ostream& o, const Vec4& right)
		{
			o << "( " << right.x << ", " << right.y << ", " << right.z << ", " << right.w << " )";
			return o;
		}

#include "Impl/XSTCVector4.impl"

	}//math


}//xst

#endif