#ifndef XST_CVECTOR3_H
#define XST_CVECTOR3_H

#include "XSTMath.h"
#include "XSTCVector2.h"

namespace XST
{
	namespace Math
	{
		class CVector3;
		typedef CVector3 Vec3;

		class XST_API CVector3
		{
			public:

				union
				{
					struct	{ f32 x, y, z; };
					f32		a[ 3 ];
				};

			public:

				static const CVector3	ZERO;
				static const CVector3	UNIT;
				static const CVector3	SIGN_MASK; //1 << 31
				static const CVector3	X;
				static const CVector3	Y;
				static const CVector3	Z;
				static const CVector3	NEGATIVE_X;
				static const CVector3	NEGATIVE_Y;
				static const CVector3	NEGATIVE_Z;

			public:

				xst_fi CVector3() {}
				xst_fi CVector3(const CVector3& _vec) : x( _vec.x ), y( _vec.y ), z( _vec.z ) {}
				xst_fi CVector3(const CVector2& _vec) : x( _vec.x ), y( _vec.y ), z( 0.0f ) {}
				xst_fi CVector3(const f32& _fValue) : x(_fValue), y(_fValue), z(_fValue) {}
				xst_fi CVector3(const f32& _fX, const f32& _fY, const f32& _fZ) : x(_fX), y(_fY), z(_fZ) {}
				xst_fi CVector3(const f32 _afValues[3]) : x( _afValues[ 0 ] ), y( _afValues[ 1 ] ), z( _afValues[ 2 ] ) {}

				~CVector3() {}

				xst_fi bool operator==(const Vec3& _vecRight) const
				{
					return x == _vecRight.x && y == _vecRight.y && z == _vecRight.z;
				}

				xst_fi bool operator>(const Vec3& _vecRight) const
				{
					return x > _vecRight.x && y > _vecRight.y && z > _vecRight.z;
				}

				xst_fi bool operator>=(const Vec3& _vecRight) const
				{
					return x >= _vecRight.x && y >= _vecRight.y && z >= _vecRight.z;
				}

				xst_fi bool operator<(const Vec3& _vecRight) const
				{
					return x < _vecRight.x && y < _vecRight.y && z < _vecRight.z;
				}

				xst_fi bool operator<=(const Vec3& _vecRight) const
				{
					return x <= _vecRight.x && y <= _vecRight.y && z <= _vecRight.z;
				}

				xst_fi bool operator!=(const Vec3& _vecRight) const
				{
					return x != _vecRight.x && y != _vecRight.y && z != _vecRight.z;
				}

				xst_fi bool operator==(cf32& fRight) const
				{
					return x == fRight && y == fRight && z == fRight;
				}

				xst_fi bool operator>(cf32& fRight) const
				{
					return x == fRight && y == fRight && z == fRight;
				}

				xst_fi bool operator>=(cf32& fRight) const
				{
					return x == fRight && y == fRight && z == fRight;
				}

				xst_fi bool operator<(cf32& fRight) const
				{
					return x == fRight && y == fRight && z == fRight;
				}

				xst_fi bool operator<=(cf32& fRight) const
				{
					return x == fRight && y == fRight && z == fRight;
				}

				xst_fi bool operator!=(cf32& fRight) const
				{
					return x == fRight && y == fRight && z == fRight;
				}

				xst_fi Vec3 operator+(const Vec3& _vecRight) const
				{
					return Vec3( x + _vecRight.x, y + _vecRight.y, z + _vecRight.z );
				}

				xst_fi void operator+=(const Vec3& _vecRight)
				{
					x += _vecRight.x; y += _vecRight.y; z += _vecRight.z;
				}

				xst_fi Vec3 operator+(cf32& fRight) const
				{
					return Vec3( x + fRight, y + fRight, z + fRight );
				}

				xst_fi void operator+=(cf32& fRight)
				{
					x += fRight; y += fRight; z += fRight;
				}

				xst_fi Vec3 operator-(const Vec3& _vecRight) const
				{
					return Vec3( x - _vecRight.x, y - _vecRight.y, z - _vecRight.z );
				}

				xst_fi void operator-=(const Vec3& _vecRight)
				{
					x -= _vecRight.x; y -= _vecRight.y; z -= _vecRight.z;
				}

				xst_fi Vec3 operator-(cf32& fRight) const
				{
					return Vec3( x - fRight, y - fRight, z - fRight );
				}

				xst_fi void operator-=(cf32& fRight)
				{
					x -= fRight; y -= fRight; z -= fRight;
				}

				xst_fi Vec3 operator-() const
				{
					return Vec3( -x, -y, -z );
				}

				xst_fi Vec3 operator*(const Vec3& _vecRight) const
				{
					return Vec3( x * _vecRight.x, y * _vecRight.y, z * _vecRight.z );
				}

				xst_fi Vec3 operator*(cf32& fRight) const
				{
					return Vec3( x * fRight, y * fRight, z * fRight );
				}

				xst_fi void operator*=(const Vec3& _vecRight)
				{
					x *= _vecRight.x; y *= _vecRight.y; z *= _vecRight.z;
				}

				xst_fi void operator*=(cf32& fRight )
				{
					x *= fRight; y *= fRight; z *= fRight;
				}

				xst_fi Vec3 operator/(const Vec3& _vecRight) const
				{
					return Vec3( x / _vecRight.x, y / _vecRight.y, z / _vecRight.z );
				}

				xst_fi void operator/=(const Vec3& _vecRight)
				{
					x /= _vecRight.x; y /= _vecRight.y; z /= _vecRight.z;
				}

				xst_fi Vec3 operator/(cf32& fRight) const
				{
					return Vec3( x / fRight, y / fRight, z / fRight );
				}

				xst_fi void operator/=(cf32& fRight)
				{
					x /= fRight; y /= fRight; z /= fRight;
				}

				xst_fi f32 operator[](const cu32& uiID) const
				{
					return a[ uiID ];
				}

				xst_fi bool Eq(const Vec3& _vecRight) const
				{
					return x == _vecRight.x && y == _vecRight.y && z == _vecRight.z;
				}

				xst_fi bool Gt(const Vec3& _vecRight) const
				{
					return x > _vecRight.x && y > _vecRight.y && z > _vecRight.z;
				}

				xst_fi bool Ge(const Vec3& _vecRight) const
				{
					return x >= _vecRight.x && y >= _vecRight.y && z >= _vecRight.z;
				}

				xst_fi bool Lt(const Vec3& _vecRight) const
				{
					return x < _vecRight.x && y < _vecRight.y && z < _vecRight.z;
				}

				xst_fi bool Le(const Vec3& _vecRight) const
				{
					return x <= _vecRight.x && y <= _vecRight.y && z <= _vecRight.z;
				}

				xst_fi bool Ne(const Vec3& _vecRight) const
				{
					return x != _vecRight.x && y != _vecRight.y && z != _vecRight.z;
				}

				xst_fi const Vec3& operator=(const Vec3& _vecRight)
				{
					x = _vecRight.x; y = _vecRight.y; z = _vecRight.z;
					return (*this);
				}

				xst_fi const Vec3& Set(const Vec3& _vecRight)
				{
					x = _vecRight.x; y = _vecRight.y; z = _vecRight.z;
					return (*this);
				}

				xst_fi const Vec3& Set(const f32& _fX, const f32& _fY, const f32& _fZ)
				{
					x = _fX; y = _fY; z = _fZ;
					return (*this);
				}

				xst_fi const u32 GetArrayElementCount() const
				{
					return 3;
				}

				xst_fi Vec3 Add(const Vec3& _vecRight) const
				{
					return Vec3( x + _vecRight.x, y + _vecRight.y, z + _vecRight.z );
				}

				xst_fi Vec3 Add(const f32& fScalar) const
				{
					return Vec3( x + fScalar, y + fScalar, z + fScalar );
				}

				xst_fi void AddAssign(const Vec3& _vecRight)
				{
					x += _vecRight.x; y += _vecRight.y; z += _vecRight.z;
				}

				xst_fi Vec3 Sub(const Vec3& _vecRight)
				{
					return Vec3( x - _vecRight.x, y - _vecRight.y, z - _vecRight.z );
				}

				xst_fi Vec3 Sub(const f32& fScalar) const
				{
					return Vec3( x - fScalar, y - fScalar, z - fScalar );
				}

				xst_fi void SubAssign(const Vec3& _vecRight)
				{
					x -= _vecRight.x; y -= _vecRight.y; z -= _vecRight.z;
				}

				xst_fi Vec3 Mul(const Vec3& _vecRight) const
				{
					return Vec3( x * _vecRight.x, y * _vecRight.y, z * _vecRight.z );
				}

				xst_fi Vec3 Mul(cf32& fScalar) const
				{
					return Vec3( x * fScalar, y * fScalar, z * fScalar );
				}

				xst_fi void  MulAssign(const Vec3& _vecRight)
				{
					x *= _vecRight.x; y *= _vecRight.y; z *= _vecRight.z;
				}

				xst_fi Vec3 Div(const Vec3& _vecRight)
				{
					return Vec3( x / _vecRight.x, y / _vecRight.y, z / _vecRight.z );
				}

				xst_fi Vec3 Div(const f32& fScalar) const 
				{
					return Vec3( x / fScalar, y / fScalar, z / fScalar );
				}

				xst_fi void DivAssign(const Vec3& _vecRight)
				{
					x /= _vecRight.x; y /= _vecRight.y; z /= _vecRight.z;
				}

				xst_fi void AddAssign(const f32& _fScalar)
				{
					x += _fScalar; y += _fScalar; z += _fScalar;
				}

				xst_fi void SubAssign(const f32& _fScalar)
				{
					x -= _fScalar; y -= _fScalar; z -= _fScalar;
				}

				xst_fi void MulAssign(const f32& _fScalar)
				{
					x *= _fScalar; y *= _fScalar; z *= _fScalar;
				}

				xst_fi void DivAssign(const f32& _fScalar)
				{
					x /= _fScalar; y /= _fScalar; z /= _fScalar;
				}

				xst_fi f32 Length() const
				{
					return Length( this );
				}

				xst_fi static f32 Length(const Vec3& vecRight)
				{
					return Length( &vecRight );
				}

				xst_fi static f32 Length(const Vec3* pvecRight)
				{
					return Math::Sqrt( pvecRight->x * pvecRight->x + pvecRight->y * pvecRight->y + pvecRight->z * pvecRight->z );
				}

				xst_fi f32 SquaredLength() const
				{
					return x * x + y * y + z * z;
				}

				xst_fi f32 Distance(const Vec3& RightVec) const
				{
					return ( *this - RightVec ).Length();
				}

				xst_fi f32 SquareDistance(const Vec3& RightVec) const
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
					}

					return fLen;
				}

				xst_fi static Vec3 Normalize(const Vec3& vecRight)
				{
					f32 fLen = Length( vecRight );
					Vec3 vecOut( vecRight );

					if( fLen > 1e-08 )
					{
						f32 fInvLen = 1.0f / fLen;
						vecOut.MulAssign( fInvLen );
					}

					return vecOut;
				}

				xst_fi f32 Dot(const Vec3& RightVec) const 
				{
					return x * RightVec.x + y * RightVec.y + z * RightVec.z;
				}

				xst_fi Vec3 Cross(const Vec3& RightVec) const
				{
					return Vec3(y * RightVec.z - z * RightVec.y, 
								z * RightVec.x - x * RightVec.z,
								x * RightVec.y - y * RightVec.x);
				}

				xst_fi Vec3 MidPoint(const Vec3& VecRight) const
				{
					return Vec3(	( x + VecRight.x ) * 0.5f, 
									( y + VecRight.y ) * 0.5f,
									( z + VecRight.z ) * 0.5f	);
				}

				Vec3		Abs() const;
				void		Abs();
				static Vec3 Abs(const Vec3& vecOther);
		};

		static xst_fi xst_ostream& operator<<(xst_ostream& o, const Vec3& right)
		{
			o << "( " << right.x << ", " << right.y << ", " << right.z << " )";
			return o;
		}
	
	}//math

}//xst

#endif