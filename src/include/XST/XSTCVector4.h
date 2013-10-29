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
				static const CVector4	SIGN_MASK; //1 << 31
				static const CVector4	X;
				static const CVector4	Y;
				static const CVector4	Z;
				static const CVector4	W;
				static const CVector4	NEGATIVE_X;
				static const CVector4	NEGATIVE_Y;
				static const CVector4	NEGATIVE_Z;
				static const CVector4	NEGATIVE_W;

			public:

				xst_fi CVector4() {}
				xst_fi CVector4(const Vec4& _vec) : x( _vec.x ), y( _vec.y ), z( _vec.z ), w( _vec.w ) {}
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

				xst_fi const Vec4& Set(const Vec4& _vecRight)
				{
					x = _vecRight.x; y = _vecRight.y; z = _vecRight.z; w = _vecRight.w;
					return (*this);
				}

				xst_fi const Vec4& Set(const f32& _fX, const f32& _fY, const f32& _fZ, cf32& _fW)
				{
					x = _fX; y = _fY; z = _fZ; w = _fW;
					return (*this);
				}

				xst_fi const u32 GetSize() const
				{
					return 4;
				}

				xst_fi Vec4 Add(const Vec4& _vecRight) const
				{
					return Vec4( x + _vecRight.x, y + _vecRight.y, z + _vecRight.z, w + _vecRight.w );
				}

				xst_fi void AddEq(const Vec4& _vecRight)
				{
					x += _vecRight.x; y += _vecRight.y; z += _vecRight.z; w += _vecRight.w;
				}

				xst_fi Vec4 Sub(const Vec4& _vecRight) const
				{
					return Vec4( x - _vecRight.x, y - _vecRight.y, z - _vecRight.z, w - _vecRight.w );
				}

				xst_fi void SubEq(const Vec4& _vecRight)
				{
					x -= _vecRight.x; y -= _vecRight.y; z -= _vecRight.z; w -= _vecRight.w;
				}

				xst_fi Vec4 Mul(const Vec4& _vecRight) const
				{
					return Vec4( x * _vecRight.x, y * _vecRight.y, z * _vecRight.z, w * _vecRight.w );
				}

				xst_fi void  MulEq(const Vec4& _vecRight)
				{
					x *= _vecRight.x; y *= _vecRight.y; z *= _vecRight.z; w *= _vecRight.w;
				}

				xst_fi Vec4 Div(const Vec4& _vecRight) const
				{
					return Vec4( x / _vecRight.x, y / _vecRight.y, z / _vecRight.z, w / _vecRight.w );
				}

				xst_fi void DivEq(const Vec4& _vecRight)
				{
					x /= _vecRight.x; y /= _vecRight.y; z /= _vecRight.z; w /= _vecRight.w;
				}

				xst_fi void AddEq(const f32& _fScalar)
				{
					x += _fScalar; y += _fScalar; z += _fScalar; w += _fScalar;
				}

				xst_fi void SubEq(const f32& _fScalar)
				{
					x -= _fScalar; y -= _fScalar; z -= _fScalar; w -= _fScalar;
				}

				xst_fi void MulEq(const f32& _fScalar)
				{
					x *= _fScalar; y *= _fScalar; z *= _fScalar; w *= _fScalar;
				}

				xst_fi void DivEq(const f32& _fScalar)
				{
					x /= _fScalar; y /= _fScalar; z /= _fScalar; w /= _fScalar;
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

				Vec4		Abs() const;
				void		Abs();
				static Vec4 Abs(const Vec4& vecOther);
			
		};

		static xst_fi xst_ostream& operator<<(xst_ostream& o, const Vec4& right)
		{
			o << "( " << right.x << ", " << right.y << ", " << right.z << ", " << right.w << " )";
			return o;
		}

	}//math

//		//Vector4 class
//		class XST_ALIGN(16) CVector4SSE
//		{
//
//			public:
//
//				CVector4SSE() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
//				CVector4SSE(const Vec4SSE& _vec) : xyzw(_vec.xyzw), w(0.0f) {}
//				CVector4SSE(const f32& _fValue) : x(_fValue), y(_fValue), z(_fValue), w(0.0f) {}
//				CVector4SSE(const f32& _fX, const f32& _fY, const f32& _fZ) : x(_fX), y(_fY), z(_fZ), w(0.0f) {}
//				CVector4SSE(const m128& _xyzw) : xyzw(_xyzw), w(0.0f) {}
//				CVector4SSE(const f32 _afValues[3]) : xyzw(*(m128*)_afValues), w(0.0f) {}
//
//				~CVector4SSE() {}
//
//				xst_fi bool operator==(const Vec4SSE& _vecRight)
//				{
//					return x == _vecRight.x && y == _vecRight.y && z == _vecRight.z;
//				}
//
//				xst_fi bool operator>(const Vec4SSE& _vecRight)
//				{
//					return x > _vecRight.x && y > _vecRight.y && z > _vecRight.z;
//				}
//
//				xst_fi bool operator>=(const Vec4SSE& _vecRight)
//				{
//					return x >= _vecRight.x && y >= _vecRight.y && z >= _vecRight.z;
//				}
//
//				xst_fi bool operator<(const Vec4SSE& _vecRight)
//				{
//					return x < _vecRight.x && y < _vecRight.y && z < _vecRight.z;
//				}
//
//				xst_fi bool operator<=(const Vec4SSE& _vecRight)
//				{
//					return x <= _vecRight.x && y <= _vecRight.y && z <= _vecRight.z;
//				}
//
//				xst_fi bool operator!=(const Vec4SSE& _vecRight)
//				{
//					return x != _vecRight.x && y != _vecRight.y && z != _vecRight.z;
//				}
//
//				xst_fi Vec4SSE operator+(const Vec4SSE& _vecRight)
//				{
//					//return Vec4cAdd( this, _vecRight );
//					return AddSSE2( this, _vecRight );
//				}
//
//				xst_fi void operator+=(const Vec4SSE& _vecRight)
//				{
//					//x += _vecRight.x; y += _vecRight.y; z += _vecRight.z;
//					//Vec4AddEq( this, _vecRight );
//					AddEqSSE2( this, _vecRight );
//				}
//
//				xst_fi Vec4SSE operator-(const Vec4SSE& _vecRight)
//				{
//					//return Vec4cSub( this, _vecRight );
//					return SubSSE2( this, _vecRight );
//				}
//
//				xst_fi void operator-=(const Vec4SSE& _vecRight)
//				{
//					//x -= _vecRight.x; y -= _vecRight.y; z -= _vecRight.z;
//					//Vec4SubEq( this, _vecRight );
//					SubEqSSE2( this, _vecRight );
//				}
//
//				xst_fi Vec4SSE operator*(const Vec4SSE& _vecRight)
//				{
//					//return Vec4cMul( this, _vecRight );
//					return MulSSE2( this, _vecRight );
//				}
//
//				xst_fi void operator*=(const Vec4SSE& _vecRight)
//				{
//					//x *= _vecRight.x; y *= _vecRight.y; z *= _vecRight.z;
//					//Vec4MulEq( this, _vecRight );
//					MulEqSSE2( this, _vecRight );
//				}
//
//				xst_fi Vec4SSE operator/(const Vec4SSE& _vecRight)
//				{
//					return DivSSE2( (*this), _vecRight );
//				}
//
//				xst_fi void operator/=(const Vec4SSE& _vecRight)
//				{
//					//x /= _vecRight.x; y /= _vecRight.y; z /= _vecRight.z;
//					//Vec4DivEq( this, _vecRight );
//					DivEqSSE2( this, _vecRight );
//				}
//
//				xst_fi bool Eq(const Vec4SSE& _vecRight)
//				{
//					return x == _vecRight.x && y == _vecRight.y && z == _vecRight.z;
//				}
//
//				xst_fi bool Gt(const Vec4SSE& _vecRight)
//				{
//					return x > _vecRight.x && y > _vecRight.y && z > _vecRight.z;
//				}
//
//				xst_fi bool Ge(const Vec4SSE& _vecRight)
//				{
//					return x >= _vecRight.x && y >= _vecRight.y && z >= _vecRight.z;
//				}
//
//				xst_fi bool Lt(const Vec4SSE& _vecRight)
//				{
//					return x < _vecRight.x && y < _vecRight.y && z < _vecRight.z;
//				}
//
//				xst_fi bool Le(const Vec4SSE& _vecRight)
//				{
//					return x <= _vecRight.x && y <= _vecRight.y && z <= _vecRight.z;
//				}
//
//				xst_fi bool Ne(const Vec4SSE& _vecRight)
//				{
//					return x != _vecRight.x && y != _vecRight.y && z != _vecRight.z;
//				}
//
//				xst_fi const Vec4SSE& operator=(const Vec4SSE& _vecRight)
//				{
//					x = _vecRight.x; y = _vecRight.y; z = _vecRight.z;
//					return (*this);
//				}
//
//				xst_fi const Vec4SSE& Set(const Vec4SSE& _vecRight)
//				{
//					x = _vecRight.x; y = _vecRight.y; z = _vecRight.z;
//					return (*this);
//				}
//
//				xst_fi const Vec4SSE& Set(const f32& _fX, const f32& _fY, const f32& _fZ)
//				{
//					x = _fX; y = _fY; z = _fZ;
//					return (*this);
//				}
//
//				xst_fi const f32* ToArray() const
//				{
//					static f32 afArray[] = { x, y, z, w };
//					return afArray;
//				}
//
//				xst_fi const u32 GetSize() const
//				{
//					return 3;
//				}
//
//				xst_fi Vec4SSE Add(const Vec4SSE& _vecRight)
//				{
//					//return Vec4cAdd( this, _vecRight );
//					return AddSSE2( this, _vecRight );
//				}
//
//				xst_fi void AddEq(const Vec4SSE& _vecRight)
//				{
//					x += _vecRight.x; y += _vecRight.y; z += _vecRight.z;
//				}
//
//				xst_fi Vec4SSE Sub(const Vec4SSE& _vecRight)
//				{
//					//return Vec4cSub( this, _vecRight );
//					return SubSSE2( this, _vecRight );
//				}
//
//				xst_fi void SubEq(const Vec4SSE& _vecRight)
//				{
//					//x -= _vecRight.x; y -= _vecRight.y; z -= _vecRight.z;
//					//Vec4SubEq( this, _vecRight );
//					SubEqSSE2( this, _vecRight );
//				}
//
//				xst_fi Vec4SSE Mul(const Vec4SSE& _vecRight)
//				{
//					//return Vec4SSE(x * _vecRight.x, y * _vecRight.y, z * _vecRight.z);
//					//return Vec4cMul( this, _vecRight );
//					return MulSSE2( this, _vecRight );
//				}
//
//				xst_fi void  MulEq(const Vec4SSE& _vecRight)
//				{
//					//x *= _vecRight.x; y *= _vecRight.y; z *= _vecRight.z;
//					//Vec4MulEq( this, _vecRight );
//					MulEqSSE2( this, _vecRight );
//				}
//
//				xst_fi Vec4SSE Div(const Vec4SSE& _vecRight)
//				{
//					//return Vec4cDiv( this, _vecRight );
//					return DivSSE2( this, _vecRight );
//				}
//
//				xst_fi void DivEq(const Vec4SSE& _vecRight)
//				{
//					//x /= _vecRight.x; y /= _vecRight.y; z /= _vecRight.z;
//					//Vec4DivEq( this, _vecRight );
//					DivEqSSE2( this, _vecRight );
//				}
//
//				xst_fi void AddEq(const f32& _fScalar)
//				{
//					//Vec4AddEqS( this, _fScalar);
//					AddEqSSE2( this, _fScalar );
//				}
//
//				xst_fi void SubEq(const f32& _fScalar)
//				{
//					//Vec4SubEqS( this, _fScalar);
//					SubEqSSE2( this, _fScalar );
//				}
//
//				xst_fi void MulEq(const f32& _fScalar)
//				{
//					//Vec4MulEqS( this, _fScalar);
//					MulEqSSE2( this, _fScalar );
//				}
//
//				xst_fi void DivEq(const f32& _fScalar)
//				{
//					//Vec4DivEqS( this, _fScalar);
//					DivEqSSE2( this, _fScalar );
//				}
//
//				xst_fi f32 Length()
//				{
//					//return Math::Vec4cLength(this);
//					return LengthSSE2( this );
//				}
//
//				xst_fi f32 Dot(const Vec4SSE& _vecRight)
//				{
//					//return Vec4cDot( this, _vecRight );
//					return DotSSE2( this, _vecRight );
//				}
//
//				xst_fi const Vec4SSE& Normalize()
//				{
//					const f32 fLen = Length();
//					x /= fLen; y /= fLen; z /= fLen;
//					return (*this);
//				}
//
//			public:
//
//				union
//				{
//					XST_ALIGN(16) struct
//					{
//						f32 x;
//						f32 y;
//						f32 z;
//						f32 w;
//					};
//
//					XST_ALIGN(16) m128 xyzw;
//				};
//		};
//
//		//FUNCTION DEFINITIONS
//
//		static xst_i f32 Length(const Vec4& _vec)
//		{
//			return sqrtf(_vec.x * _vec.x + _vec.y * _vec.y + _vec.z * _vec.z);
//		}
//
//		static xst_i f32 Dot(const Vec4& _vecLeft, const Vec4& _vecRight)
//		{
//			return ( _vecLeft.x * _vecRight.x + _vecLeft.y * _vecRight.y + _vecLeft.z * _vecRight.z );
//		}
//
//		static xst_i Vec4 Normalize(const Vec4& _vec)
//		{
//			const f32 fLen = Length(_vec);
//			return Vec4(_vec.x / fLen, _vec.y / fLen, _vec.z / fLen, _vec.w / fLen);
//		}
//
//		static xst_i Vec4 Normalize(const Vec4& _vec, const f32& _fVecLen)
//		{
//			xst_assert(_fVecLen != 0, "(XST::Math::Normalize) _fVecLen is 0.");
//			return Vec4(_vec.x / _fVecLen, _vec.y / _fVecLen, _vec.z / _fVecLen, _vec.w / _fVecLen);
//		}
//
//		xst_fi Vec4 Mul(const Vec4& _vecLeft, const Vec4& _vecRight)
//		{
//			//return *(Vec4*)&_mm_mul_ps(*(__m128*)&_vecLeft, *(__m128*)&_vecRight);
//			//return Vec4( _mm_mul_ps(_vecLeft.xyzw, _vecRight.xyzw) );
//			return Vec4( _vecLeft.x * _vecRight.x, _vecLeft.y * _vecRight.y, _vecLeft.z * _vecRight.z, _vecLeft.w * _vecRight.w );
//		}
//
//		//SOFTWARE
//		xst_fi Vec4 MulRef(const Vec4& _vecLeft, const Vec4& _vecRight)
//		{
//			return Vec4(_vecLeft.x * _vecRight.x, _vecLeft.y * _vecRight.y, _vecLeft.z * _vecRight.z, _vecLeft.w * _vecRight.w);
//		}
//
//		xst_fi Vec4 AddRef(const Vec4& _vecLeft, const Vec4& _vecRight)
//		{
//			return Vec4(_vecLeft.x + _vecRight.x, _vecLeft.y + _vecRight.y, _vecLeft.z + _vecRight.z, _vecLeft.w + _vecRight.w);
//		}
//
//		xst_fi Vec4 SubRef(const Vec4& _vecLeft, const Vec4& _vecRight)
//		{
//			return Vec4(_vecLeft.x - _vecRight.x, _vecLeft.y - _vecRight.y, _vecLeft.z - _vecRight.z, _vecLeft.w - _vecRight.w);
//		}
//
//		xst_fi Vec4 DivRef(const Vec4& _vecLeft, const Vec4& _vecRight)
//		{
//			return Vec4(_vecLeft.x / _vecRight.x, _vecLeft.y / _vecRight.y, _vecLeft.z / _vecRight.z, _vecLeft.w / _vecRight.w);
//		}
//
//		xst_fi Vec4 MulRef(const Vec4& _vecLeft, const f32& _fScalar)
//		{
//			return Vec4(_vecLeft.x * _fScalar, _vecLeft.y * _fScalar, _vecLeft.z * _fScalar, _vecLeft.w * _fScalar);
//		}
//
//		xst_fi Vec4 AddRef(const Vec4& _vecLeft, const f32& _fScalar)
//		{
//			return Vec4(_vecLeft.x + _fScalar, _vecLeft.y + _fScalar, _vecLeft.z + _fScalar, _vecLeft.w + _fScalar);
//		}
//
//		xst_fi Vec4 SubRef(const Vec4& _vecLeft, const f32& _fScalar)
//		{
//			return Vec4(_vecLeft.x - _fScalar, _vecLeft.y - _fScalar, _vecLeft.z - _fScalar, _vecLeft.w - _fScalar);
//		}
//
//		xst_fi Vec4 DivRef(const Vec4& _vecLeft, const f32& _fScalar)
//		{
//			return Vec4(_vecLeft.x / _fScalar, _vecLeft.y / _fScalar, _vecLeft.z / _fScalar, _vecLeft.w / _fScalar);
//		}
//
//		xst_fi Vec4 NormalizeRef(const Vec4& _vecLeft)
//		{
//			const f32 fLen = LengthRef( _vecLeft );
//			return Vec4( _vecLeft.x / fLen, _vecLeft.y / fLen, _vecLeft.z / fLen, _vecLeft.w / fLen );
//		}
//
//		//SOFTWARE (for class use)
//
//		xst_fi Vec4 MulRef(const Vec4* _vecLeft, const Vec4& _vecRight)
//		{
//			return Vec4(_vecLeft->x * _vecRight.x, _vecLeft->y * _vecRight.y, _vecLeft->z * _vecRight.z, _vecLeft->w * _vecRight.w);
//		}
//
//		xst_fi Vec4 AddRef(const Vec4* _vecLeft, const Vec4& _vecRight)
//		{
//			return Vec4(_vecLeft->x + _vecRight.x, _vecLeft->y + _vecRight.y, _vecLeft->z + _vecRight.z, _vecLeft->w + _vecRight.w);
//		}
//
//		xst_fi Vec4 SubRef(const Vec4* _vecLeft, const Vec4& _vecRight)
//		{
//			return Vec4(_vecLeft->x - _vecRight.x, _vecLeft->y - _vecRight.y, _vecLeft->z - _vecRight.z, _vecLeft->w - _vecRight.w);
//		}
//
//		xst_fi Vec4 DivRef(const Vec4* _vecLeft, const Vec4& _vecRight)
//		{
//			return Vec4(_vecLeft->x / _vecRight.x, _vecLeft->y / _vecRight.y, _vecLeft->z / _vecRight.z, _vecLeft->w / _vecRight.w);
//		}
//
//		xst_fi Vec4 MulRef(const Vec4* _vecLeft, const f32& _fScalar)
//		{
//			return Vec4(_vecLeft->x * _fScalar, _vecLeft->y * _fScalar, _vecLeft->z * _fScalar);
//		}
//
//		xst_fi Vec4 AddRef(const Vec4* _vecLeft, const f32& _fScalar)
//		{
//			return Vec4(_vecLeft->x + _fScalar, _vecLeft->y + _fScalar, _vecLeft->z + _fScalar);
//		}
//
//		xst_fi Vec4 SubRef(const Vec4* _vecLeft, const f32& _fScalar)
//		{
//			return Vec4(_vecLeft->x - _fScalar, _vecLeft->y - _fScalar, _vecLeft->z - _fScalar);
//		}
//
//		xst_fi Vec4 DivRef(const Vec4* _vecLeft, const f32& _fScalar)
//		{
//			return Vec4(_vecLeft->x / _fScalar, _vecLeft->y / _fScalar, _vecLeft->z / _fScalar);
//		}
//
//
//		xst_fi Vec4 SqrtRef(const Vec4& _vecLeft)
//		{
//			return Vec4( sqrtf(_vecLeft.x), sqrtf(_vecLeft.y), sqrtf(_vecLeft.z) );
//		}
//
//		xst_fi Vec4 SqrtRef(const Vec4* _vecLeft)
//		{
//			return Vec4( sqrtf(_vecLeft->x), sqrtf(_vecLeft->y), sqrtf(_vecLeft->z) );
//		}
//		xst_fi f32 DotRef(const Vec4& _vecLeft, const Vec4& _vecRight)
//		{
//			/*
//			0135FB2E  mov         eax,dword ptr [_vecLeft]
//			0135FB31  fld         dword ptr [eax]
//			0135FB33  mov         ecx,dword ptr [_vecRight]
//			0135FB36  fmul        dword ptr [ecx]
//			0135FB38  mov         edx,dword ptr [_vecLeft]
//			0135FB3B  fld         dword ptr [edx+4]
//			0135FB3E  mov         eax,dword ptr [_vecRight]
//			0135FB41  fmul        dword ptr [eax+4]
//			0135FB44  faddp       st(1),st
//			0135FB46  mov         ecx,dword ptr [_vecLeft]
//			0135FB49  fld         dword ptr [ecx+8]
//			0135FB4C  mov         edx,dword ptr [_vecRight]
//			0135FB4F  fmul        dword ptr [edx+8]
//			0135FB52  faddp       st(1),st
//			0135FB54  fstp        dword ptr [ebp-0C4h]
//			0135FB5A  fld         dword ptr [ebp-0C4h]
//			*/
//			return _vecLeft.x * _vecRight.x + _vecLeft.y * _vecRight.y + _vecLeft.z * _vecRight.z;
//		}
//
//		xst_fi f32 DotRef(const Vec4* _vecLeft, const Vec4& _vecRight)
//		{
//			return _vecLeft->x * _vecRight.x + _vecLeft->y * _vecRight.y + _vecLeft->z * _vecRight.z;
//		}
//
//		xst_fi void AddEqRef(Vec4* _vecLeft, const Vec4& _vecRight)
//		{
//			_vecLeft->x += _vecRight.x; _vecLeft->y += _vecRight.y; _vecLeft->z += _vecRight.z;
//		}
//
//		xst_fi void SubEqRef(Vec4* _vecLeft, const Vec4& _vecRight)
//		{
//			_vecLeft->x -= _vecRight.x; _vecLeft->y -= _vecRight.y; _vecLeft->z -= _vecRight.z;
//		}
//
//		xst_fi void MulEqRef(Vec4* _vecLeft, const Vec4& _vecRight)
//		{
//			_vecLeft->x *= _vecRight.x; _vecLeft->y *= _vecRight.y; _vecLeft->z *= _vecRight.z;
//		}
//
//		xst_fi void DivEqRef(Vec4* _vecLeft, const Vec4& _vecRight)
//		{
//			_vecLeft->x /= _vecRight.x; _vecLeft->y /= _vecRight.y; _vecLeft->z /= _vecRight.z;
//		}
//
//		xst_fi void AddEqRef(Vec4* _vecLeft, const f32& _fScalar)
//		{
//			_vecLeft->x += _fScalar; _vecLeft->y += _fScalar; _vecLeft->z += _fScalar;
//		}
//
//		xst_fi void SubEqRef(Vec4* _vecLeft, const f32& _fScalar)
//		{
//			_vecLeft->x -= _fScalar; _vecLeft->y -= _fScalar; _vecLeft->z -= _fScalar;
//		}
//
//		xst_fi void MulEqRef(Vec4* _vecLeft, const f32& _fScalar)
//		{
//			_vecLeft->x *= _fScalar; _vecLeft->y *= _fScalar; _vecLeft->z *= _fScalar;
//		}
//
//		xst_fi void DivEqRef(Vec4* _vecLeft, const f32& _fScalar)
//		{
//			_vecLeft->x /= _fScalar; _vecLeft->y /= _fScalar; _vecLeft->z /= _fScalar;
//		}
//
//		xst_fi f32 LengthRef(const Vec4& _vecLeft)
//		{
//			return sqrtf( _vecLeft.x * _vecLeft.x + _vecLeft.y * _vecLeft.y + _vecLeft.z * _vecLeft.z );
//		}
//
//		xst_fi f32 LengthRef(const Vec4* _vecLeft)
//		{
//			return sqrtf( _vecLeft->x * _vecLeft->x + _vecLeft->y * _vecLeft->y + _vecLeft->z * _vecLeft->z );
//		}
//
//		xst_fi Vec4 NormalizeRef(const Vec4* _vecLeft)
//		{
//			const f32 fLen = LengthRef( _vecLeft );
//			return Vec4( _vecLeft->x / fLen, _vecLeft->y / fLen, _vecLeft->z / fLen );
//		}
//
//
////#if defined (XST_SSE)
//		//SSE2
//		xst_fi Vec4SSE MulSSE2(const Vec4SSE& _vecLeft, const Vec4SSE& _vecRight)
//		{
//			return _mm_mul_ps(_vecLeft.xyzw, _vecRight.xyzw);
//		}
//
//		xst_fi Vec4SSE AddSSE2(const Vec4SSE& _vecLeft, const Vec4SSE& _vecRight)
//		{
//			return _mm_add_ps(_vecLeft.xyzw, _vecRight.xyzw);
//		}
//
//		xst_fi Vec4SSE SubSSE2(const Vec4SSE& _vecLeft, const Vec4SSE& _vecRight)
//		{
//			return _mm_sub_ps(_vecLeft.xyzw, _vecRight.xyzw);
//		}
//
//		xst_fi Vec4SSE DivSSE2(const Vec4SSE& _vecLeft, const Vec4SSE& _vecRight)
//		{
//			return _mm_div_ps(_vecLeft.xyzw, _vecRight.xyzw);
//		}
//
//		xst_fi Vec4SSE MulSSE2(const Vec4SSE& _vecLeft, const f32& _fScalar)
//		{
//			return _mm_mul_ps( _vecLeft.xyzw, _mm_load1_ps(&_fScalar) );
//		}
//
//		xst_fi Vec4SSE AddSSE2(const Vec4SSE& _vecLeft, const f32& _fScalar)
//		{
//			return _mm_add_ps( _vecLeft.xyzw, _mm_load1_ps(&_fScalar) );
//		}
//
//		xst_fi Vec4SSE SubSSE2(const Vec4SSE& _vecLeft, const f32& _fScalar)
//		{
//			return _mm_sub_ps( _vecLeft.xyzw, _mm_load1_ps(&_fScalar) );
//		}
//
//		xst_fi Vec4SSE DivSSE2(const Vec4SSE& _vecLeft, const f32& _fScalar)
//		{
//			return _mm_div_ps( _vecLeft.xyzw, _mm_load1_ps(&_fScalar) );
//		}
//
//		xst_fi Vec4SSE SqrtSSE2(const Vec4SSE& _vec)
//		{
//			return _mm_sqrt_ps( _vec.xyzw );
//		}
//
//		xst_i f32 DotSSE2(const Vec4SSE& _vecLeft, const Vec4SSE& _vecRight)
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
//		xst_fi f32 LengthSSE2(const Vec4SSE& _vecLeft)
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
//		xst_fi Vec4SSE NormalizeSSE2(const Vec4SSE& _vecLeft)
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
//		xst_fi Vec4SSE MulSSE2(const Vec4SSE* _vecLeft, const Vec4SSE& _vecRight)
//		{
//			return _mm_mul_ps(_vecLeft->xyzw, _vecRight.xyzw);
//		}
//
//		xst_fi Vec4SSE AddSSE2(const Vec4SSE* _vecLeft, const Vec4SSE& _vecRight)
//		{
//			return _mm_add_ps(_vecLeft->xyzw, _vecRight.xyzw);
//		}
//
//		xst_fi Vec4SSE SubSSE2(const Vec4SSE* _vecLeft, const Vec4SSE& _vecRight)
//		{
//			return _mm_sub_ps(_vecLeft->xyzw, _vecRight.xyzw);
//		}
//
//		xst_fi Vec4SSE DivSSE2(const Vec4SSE* _vecLeft, const Vec4SSE& _vecRight)
//		{
//			return _mm_div_ps(_vecLeft->xyzw, _vecRight.xyzw);
//		}
//
//		xst_fi Vec4SSE MulSSE2(const Vec4SSE* _vecLeft, const f32& _fScalar)
//		{
//			return _mm_mul_ps( _vecLeft->xyzw, _mm_load1_ps(&_fScalar) );
//		}
//
//		xst_fi Vec4SSE AddSSE2(const Vec4SSE* _vecLeft, const f32& _fScalar)
//		{
//			return _mm_add_ps( _vecLeft->xyzw, _mm_load1_ps(&_fScalar) );
//		}
//
//		xst_fi Vec4SSE SubSSE2(const Vec4SSE* _vecLeft, const f32& _fScalar)
//		{
//			return _mm_sub_ps( _vecLeft->xyzw, _mm_load1_ps(&_fScalar) );
//		}
//
//		xst_fi Vec4SSE DivSSE2(const Vec4SSE* _vecLeft, const f32& _fScalar)
//		{
//			return _mm_div_ps( _vecLeft->xyzw, _mm_load1_ps(&_fScalar) );
//		}
//
//		xst_fi Vec4SSE SqrtSSE2(const Vec4SSE* _vec)
//		{
//			return _mm_sqrt_ps( _vec->xyzw );
//		}
//
//		xst_fi f32 DotSSE2(const Vec4SSE* _vecLeft, const Vec4SSE& _vecRight)
//		{
//			m128 r1 = _mm_mul_ps( _vecLeft->xyzw , _vecRight.xyzw );
//			r1 = _mm_add_ps( r1, _mm_movehl_ps(r1, r1));
//			r1 = _mm_add_ps( r1, _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(3, 2, 1, 1)));
//			f32 fResult;
//			_mm_store_ss( &fResult, r1 ); // zapisz 1 float
//			return fResult;
//		}
//
//		xst_fi void AddEqSSE2(Vec4SSE* _vecLeft, const Vec4SSE& _vecRight)
//		{
//			_vecLeft->xyzw = _mm_add_ps( _vecLeft->xyzw, _vecRight.xyzw );
//		}
//
//		xst_fi void SubEqSSE2(Vec4SSE* _vecLeft, const Vec4SSE& _vecRight)
//		{
//			_vecLeft->xyzw = _mm_sub_ps( _vecLeft->xyzw, _vecRight.xyzw );
//		}
//
//		xst_fi void MulEqSSE2(Vec4SSE* _vecLeft, const Vec4SSE& _vecRight)
//		{
//			_vecLeft->xyzw = _mm_mul_ps( _vecLeft->xyzw, _vecRight.xyzw );
//		}
//
//		xst_fi void DivEqSSE2(Vec4SSE* _vecLeft, const Vec4SSE& _vecRight)
//		{
//			_vecLeft->xyzw = _mm_div_ps( _vecLeft->xyzw, _vecRight.xyzw );
//		}
//
//		xst_fi void AddEqSSE2(Vec4SSE* _vecLeft, const f32& _fScalar)
//		{
//			_vecLeft->xyzw = _mm_add_ps( _vecLeft->xyzw, _mm_load1_ps( &_fScalar ) );
//		}
//
//		xst_fi void SubEqSSE2(Vec4SSE* _vecLeft, const f32& _fScalar)
//		{
//			_vecLeft->xyzw = _mm_sub_ps( _vecLeft->xyzw, _mm_load1_ps( &_fScalar ) );
//		}
//
//		xst_fi void MulEqSSE2(Vec4SSE* _vecLeft, const f32& _fScalar)
//		{
//			_vecLeft->xyzw = _mm_mul_ps( _vecLeft->xyzw, _mm_load1_ps( &_fScalar ) );
//		}
//
//		xst_fi void DivEqSSE2(Vec4SSE* _vecLeft, const f32& _fScalar)
//		{
//			_vecLeft->xyzw = _mm_div_ps( _vecLeft->xyzw, _mm_load1_ps( &_fScalar ) );
//		}
//
//		xst_fi f32 LengthSSE2(const Vec4SSE* _vecLeft)
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
//		xst_fi Vec4SSE NormalizeSSE2(const Vec4SSE* _vecLeft)
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
//		xst_fi f32	DotSSE4(const Vec4SSE& _vecLeft, const Vec4SSE& _vecRight)
//		{
//			return xst_m128( _mm_dp_ps( _vecLeft.xyzw, _vecRight.xyzw, 0x55 ) ).m128_f32[0];
//		}
//
//		//SSE4 (class)
//		xst_fi f32	DotSSE4(const Vec4SSE* _vecLeft, const Vec4SSE& _vecRight)
//		{
//			return xst_m128( _mm_dp_ps( _vecLeft->xyzw, _vecRight.xyzw, 0x55 ) ).m128_f32[0];
//		}


	//}//math
}//xst

#endif