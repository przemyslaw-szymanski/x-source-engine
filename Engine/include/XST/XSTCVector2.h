#ifndef XST_CVECTOR2_H
#define XST_CVECTOR2_H

#include "XSTMath.h"

namespace XST
{
	namespace Math
	{
		class XST_ALIGN(16) CVector2SSE;
		typedef XST_ALIGN(16) CVector2SSE Vec2SSE;

		class XST_API CVector2;
		typedef CVector2 Vec2;

		typedef Vec2 (*Vector2Func1)(const Vec2& _vec);
		typedef Vec2 (*Vector2Func1a)(const Vec2* _pvec);
		typedef Vec2 (*Vector2Func2)(const Vec2& _vecLeft, const Vec2& _vecRight);
		typedef Vec2 (*Vector2Func2a)(const Vec2* _pvecLeft, const Vec2& _vecRight);
		typedef Vec2 (*Vector2Func3)(const Vec2& _vecLeft, const f32& _fScalar);
		typedef Vec2 (*Vector2Func3a)(const Vec2* _vecLeft, const f32& _fScalar);
		typedef f32 (*Vector2Func4)(const Vec2& _vecLeft, const Vec2& _vecRight);
		typedef f32 (*Vector2Func4c)(const Vec2* _vecLeft, const Vec2& _vecRight);
		typedef void (*Vector2Func5c)(Vec2* _vecLeft, const Vec2& _vecRight);
		typedef void (*Vector2Func5ca)(Vec2* _vecLeft, const f32& _fScalar);
		typedef f32 (*Vector2Func6)(const Vec2& _vecLeft);
		typedef f32 (*Vector2Func6c)(const Vec2* _vecLeft);

		//static xst_i f32 Length(const Vec2& _vec);
//		static xst_i f32 Dot(const Vec2& _vecLeft, const Vec2& _vecRight);
//		static xst_i Vec2 Normalize(const Vec2& _vec);
//		static xst_i Vec2 Normalize(const Vec2& _vec, const f32& _fVecLen);
//		xst_fi Vec2 Mul(const Vec2& _vecLeft, const Vec2& _vecRight);
//
//		//Software
//		xst_fi Vec2 MulRef(const Vec2& _vecLeft, const Vec2& _vecRight);
//		xst_fi Vec2	MulRef(const Vec2& _vecLeft, const f32& _fScalar);
//		xst_fi Vec2 AddRef(const Vec2& _vecLeft, const Vec2& _vecRight);
//		xst_fi Vec2	AddRef(const Vec2& _vecLeft, const f32& _fScalar);
//		xst_fi Vec2 SubRef(const Vec2& _vecLeft, const Vec2& _vecRight);
//		xst_fi Vec2	SubRef(const Vec2& _vecLeft, const f32& _fScalar);
//		xst_fi Vec2 DivRef(const Vec2& _vecLeft, const Vec2& _vecRight);
//		xst_fi Vec2	DivRef(const Vec2& _vecLeft, const f32& _fScalar);
//
//		xst_fi Vec2	SqrtRef(const Vec2& _vecLeft);
//		xst_fi f32	DotRef(const Vec2& _vecLeft, const Vec2& _vecRight);
//		xst_fi f32	LengthRef(const Vec2& _vecLeft);
//
//		//SOFTWARE (for class use)
//		xst_fi Vec2 AddRef(const Vec2* _vecLeft, const Vec2& _vecRight);
//		xst_fi Vec2	AddRef(const Vec2* _vecLeft, const f32& _fScalar);
//		xst_fi void AddEqRef(Vec2* _vecLeft, const Vec2& _vecRight);
//		xst_fi void	AddEqRef(Vec2* _vecLeft, const f32& _fScalar);
//		xst_fi Vec2 SubRef(const Vec2* _vecLeft, const Vec2& _vecRight);
//		xst_fi Vec2	SubRef(const Vec2* _vecLeft, const f32& _fScalar);
//		xst_fi void SubEqRef(Vec2* _vecLeft, const Vec2& _vecRight);
//		xst_fi void	SubEqRef(Vec2* _vecLeft, const f32& _fScalar);
//		xst_fi Vec2 MulRef(const Vec2* _vecLeft, const Vec2& _vecRight);
//		xst_fi Vec2	MulRef(const Vec2* _vecLeft, const f32& _fScalar);
//		xst_fi void MulEqRef(Vec2* _vecLeft, const Vec2& _vecRight);
//		xst_fi void	MulEqRef(Vec2* _vecLeft, const f32& _fScalar);
//		xst_fi Vec2 DivRef(const Vec2* _vecLeft, const Vec2& _vecRight);
//		xst_fi Vec2	DivRef(const Vec2* _vecLeft, const f32& _fScalar);
//		xst_fi void DivEqRef(Vec2* _vecLeft, const Vec2& _vecRight);
//		xst_fi void	DivEqRef(Vec2* _vecLeft, const f32& _fScalar);
//
//		xst_fi Vec2	SqrtRef(const Vec2* _vecLeft);
//		xst_fi f32	DotRef(const Vec2* _vecLeft, const Vec2& _vecRight);
//		xst_fi f32	LengthRef(const Vec2* _vecLeft);
//
//
//		//SSE2
////#if defined (XST_SSE)
//		xst_fi Vec2SSE MulSSE2(const Vec2SSE& _vecLeft, const Vec2SSE& _vecRight);
//		xst_fi Vec2SSE AddSSE2(const Vec2SSE& _vecLeft, const Vec2SSE& _vecRight);
//		xst_fi Vec2SSE SubSSE2(const Vec2SSE& _vecLeft, const Vec2SSE& _vecRight);
//		xst_fi Vec2SSE DivSSE2(const Vec2SSE& _vecLeft, const Vec2SSE& _vecRight);
//		xst_fi Vec2SSE	SqrtSSE2(const Vec2SSE& _vecLeft);
//		xst_i f32	DotSSE2(const Vec2SSE& _vecLeft, const Vec2SSE& _vecRight);
//		xst_fi f32	LengthSSE2(const Vec2SSE& _vecLeft);
//
//		//SSE2 Scalar
//		xst_fi Vec2SSE AddSSE2(const Vec2SSE& _vecLeft, const f32& _fScalar);
//		xst_fi Vec2SSE SubSSE2(const Vec2SSE& _vecLeft, const f32& _fScalar);
//		xst_fi Vec2SSE MulSSE2(const Vec2SSE& _vecLeft, const f32& _fScalar);
//		xst_fi Vec2SSE DivSSE2(const Vec2SSE& _vecLeft, const f32& _fScalar);
//
//		//SSE2 (for class use)
//		xst_fi Vec2SSE MulSSE2(const Vec2SSE* _vecLeft, const Vec2SSE& _vecRight);
//		xst_fi Vec2SSE AddSSE2(const Vec2SSE* _vecLeft, const Vec2SSE& _vecRight);
//		xst_fi Vec2SSE SubSSE2(const Vec2SSE* _vecLeft, const Vec2SSE& _vecRight);
//		xst_fi Vec2SSE DivSSE2(const Vec2SSE* _vecLeft, const Vec2SSE& _vecRight);
//		xst_fi void MulEqSSE2(Vec2SSE* _vecLeft, const Vec2SSE& _vecRight);
//		xst_fi void AddEqSSE2(Vec2SSE* _vecLeft, const Vec2SSE& _vecRight);
//		xst_fi void SubEqSSE2(Vec2SSE* _vecLeft, const Vec2SSE& _vecRight);
//		xst_fi void DivEqSSE2(Vec2SSE* _vecLeft, const Vec2SSE& _vecRight);
//
//		xst_fi Vec2SSE	SqrtSSE2(const Vec2SSE& _vecLeft);
//		xst_fi f32	DotSSE2(const Vec2SSE* _vecLeft, const Vec2SSE& _vecRight);
//		xst_fi f32	LengthSSE2(const Vec2SSE* _vecLeft);
//
//		//SSE2 Scalar
//		xst_fi Vec2SSE AddSSE2(const Vec2SSE* _vecLeft, const f32& _fScalar);
//		xst_fi Vec2SSE SubSSE2(const Vec2SSE* _vecLeft, const f32& _fScalar);
//		xst_fi Vec2SSE MulSSE2(const Vec2SSE* _vecLeft, const f32& _fScalar);
//		xst_fi Vec2SSE DivSSE2(const Vec2SSE* _vecLeft, const f32& _fScalar);
//		xst_fi void MulEqSSE2(Vec2SSE* _vecLeft, const f32& _fScalar);
//		xst_fi void AddEqSSE2(Vec2SSE* _vecLeft, const f32& _fScalar);
//		xst_fi void SubEqSSE2(Vec2SSE* _vecLeft, const f32& _fScalar);
//		xst_fi void DivEqSSE2(Vec2SSE* _vecLeft, const f32& _fScalar);
//
//		//SSE4
//		xst_fi f32	DotSSE4(const Vec2SSE& _vecLeft, const Vec2SSE& _vecRight);
//
//		//SSE4 (class)
//		xst_fi f32	DotSSE4(const Vec2SSE* _vecLeft, const Vec2SSE& _vecRight);
////#endif //XST_SSE
//
//		//Function pointers
//		static Vector2Func2 Vec2Mul	= &MulRef;
//		static Vector2Func2 Vec2Add	= &AddRef;
//		static Vector2Func2 Vec2Sub	= &SubRef;
//		static Vector2Func2 Vec2Div	= &DivRef;
//
//		static Vector2Func3 Vec2MulS	= &MulRef;
//		static Vector2Func3 Vec2AddS	= &AddRef;
//		static Vector2Func3 Vec2SubS	= &SubRef;
//		static Vector2Func3 Vec2DivS	= &DivRef;
//
//		static Vector2Func2a Vec2cAdd	= &AddRef;
//		static Vector2Func2a Vec2cSub	= &SubRef;
//		static Vector2Func2a Vec2cMul	= &MulRef;
//		static Vector2Func2a Vec2cDiv	= &DivRef;
//
//		static Vector2Func3a Vec2cAddS	= &AddRef;
//		static Vector2Func3a Vec2cSubS	= &SubRef;
//		static Vector2Func3a Vec2cMulS	= &MulRef;
//		static Vector2Func3a Vec2cDivS	= &DivRef;
//
//		static Vector2Func1		Vec2Sqrt	= &SqrtRef;
//		static Vector2Func1a	Vec2cSqrt	= &SqrtRef;
//
//		static Vector2Func4		Vec2Dot		= &DotRef;
//		static Vector2Func4c	Vec2cDot	= &DotRef;
//
//		static Vector2Func5c	Vec2AddEq	= &AddEqRef;
//		static Vector2Func5c	Vec2SubEq	= &SubEqRef;
//		static Vector2Func5c	Vec2MulEq	= &MulEqRef;
//		static Vector2Func5c	Vec2DivEq	= &DivEqRef;
//
//		static Vector2Func5ca	Vec2AddEqS	= &AddEqRef;
//		static Vector2Func5ca	Vec2SubEqS	= &SubEqRef;
//		static Vector2Func5ca	Vec2MulEqS	= &MulEqRef;
//		static Vector2Func5ca	Vec2DivEqS	= &DivEqRef;
//
//		static Vector2Func6		Vec2Length	= &LengthRef;
//		static Vector2Func6c	Vec2cLength	= &LengthRef;
//
//
//		//Macros
//		#define XSTVec2Mul(_vecLeft, _vec3Right)	Vec2Mul(_vecLeft, _vec3Right)
//		#define XSTVec2Add(_vecLeft, _vec3Right)	Vec2Add(_vecLeft, _vec3Right)
//		#define XSTVec2Div(_vecLeft, _vec3Right)	Vec2Div(_vecLeft, _vec3Right)
//		#define XSTVec2Sub(_vecLeft, _vec3Right)	Vec2Sub(_vecLeft, _vec3Right)
//		#define XSTVec2MulS(_vecLeft, _fScalar)	Vec2MulS(_vecLeft, _vec3Right)
//		#define XSTVec2AddS(_vecLeft, _fScalar)	Vec2AddS(_vecLeft, _vec3Right)
//		#define XSTVec2DivS(_vecLeft, _fScalar)	Vec2DivS(_vecLeft, _vec3Right)
//		#define XSTVec2SubS(_vecLeft, _fScalar)	Vec2SubS(_vecLeft, _vec3Right)
//		#define XSTVec2Sqrt(_vec3)					Vec2Sqrt(_vec3)
//		#define XSTVec2Dot(_vecLeft, _vec3Right)	Vec2Dot(_vecLeft, _vec3Right)
//		#define XSTVec2Length(_vecLeft)			Vec2Length(_vecLeft)

		class XST_API CVector2
		{
			public:

				union
				{
					struct	{ f32 x, y; };
					f32		a[ 2 ];
				};

			public:

				static const CVector2	ZERO;
				static const CVector2	UNIT;
				static const CVector2	SIGN_MASK; //1 << 31
				static const CVector2	X;
				static const CVector2	Y;
				static const CVector2	NEGATIVE_X;
				static const CVector2	NEGATIVE_Y;

			public:

				xst_fi CVector2() {}
				xst_fi CVector2(const Vec2& _vec) : x( _vec.x ), y( _vec.y ) {}
				xst_fi CVector2(const f32& _fValue) : x(_fValue), y(_fValue) {}
				xst_fi CVector2(const f32& _fX, const f32& _fY) : x(_fX), y(_fY) {}
				xst_fi CVector2(const f32 _afValues[2]) : x( _afValues[ 0 ] ), y( _afValues[ 1 ] ) {}

				~CVector2() {}

				xst_fi const bool operator==(const Vec2& _vecRight) const 
				{
					return x == _vecRight.x && y == _vecRight.y;
				}

				xst_fi const bool operator>(const Vec2& _vecRight) const 
				{
					return x > _vecRight.x && y > _vecRight.y;
				}

				xst_fi const  bool operator>=(const Vec2& _vecRight) const 
				{
					return x >= _vecRight.x && y >= _vecRight.y;
				}

				xst_fi const bool operator<(const Vec2& _vecRight) const 
				{
					return x < _vecRight.x && y < _vecRight.y;
				}

				xst_fi const bool operator<=(const Vec2& _vecRight) const 
				{
					return x <= _vecRight.x && y <= _vecRight.y;
				}

				xst_fi const bool operator!=(const Vec2& _vecRight) const 
				{
					return x != _vecRight.x && y != _vecRight.y;
				}

				xst_fi Vec2 operator+(const Vec2& _vecRight) const 
				{
					return Vec2( x + _vecRight.x, y + _vecRight.y );
				}

				xst_fi void operator+=(const Vec2& _vecRight) 
				{
					x += _vecRight.x; y += _vecRight.y;;
				}

				xst_fi Vec2 operator-(const Vec2& _vecRight) const 
				{
					return Vec2( x - _vecRight.x, y - _vecRight.y);
				}

				xst_fi Vec2 operator-() const 
				{
					return Vec2( -x, -y );
				}

				xst_fi void operator-=(const Vec2& _vecRight)
				{
					x -= _vecRight.x; y -= _vecRight.y;
				}

				xst_fi Vec2 operator*(const Vec2& _vecRight) const 
				{
					return Vec2( x * _vecRight.x, y * _vecRight.y );
				}

				xst_fi void operator*=(const Vec2& _vecRight) 
				{
					x *= _vecRight.x; y *= _vecRight.y;
				}

				xst_fi Vec2 operator/(const Vec2& _vecRight) const 
				{
					return Vec2( x / _vecRight.x, y / _vecRight.y );
				}

				xst_fi void operator/=(const Vec2& _vecRight) 
				{
					x /= _vecRight.x; y /= _vecRight.y;
				}

				xst_fi f32 operator[](const cu32& uiID) const
				{
					return a[ uiID ];
				}

				xst_fi const bool Eq(const Vec2& _vecRight) const 
				{
					return x == _vecRight.x && y == _vecRight.y;
				}

				xst_fi const bool Gt(const Vec2& _vecRight) const 
				{
					return x > _vecRight.x && y > _vecRight.y;
				}

				xst_fi bool Ge(const Vec2& _vecRight)
				{
					return x >= _vecRight.x && y >= _vecRight.y;
				}

				xst_fi const bool Lt(const Vec2& _vecRight) const
				{
					return x < _vecRight.x && y < _vecRight.y;
				}

				xst_fi const bool Le(const Vec2& _vecRight) const
				{
					return x <= _vecRight.x && y <= _vecRight.y;
				}

				xst_fi const bool Ne(const Vec2& _vecRight) const
				{
					return x != _vecRight.x && y != _vecRight.y;
				}

				xst_fi const Vec2& operator=(const Vec2& _vecRight)
				{
					x = _vecRight.x; y = _vecRight.y;
					return (*this);
				}

				xst_fi const Vec2& Set(const Vec2& _vecRight)
				{
					x = _vecRight.x; y = _vecRight.y;
					return (*this);
				}

				xst_fi const Vec2& Set(const f32& _fX, const f32& _fY)
				{
					x = _fX; y = _fY;
					return (*this);
				}

				xst_fi const u32 GetArraySize() const
				{
					return 2;
				}

				xst_fi Vec2 Add(const Vec2& _vecRight) const
				{
					return Vec2( x + _vecRight.x, y + _vecRight.y );
				}

				xst_fi void AddAssign(const Vec2& _vecRight)
				{
					x += _vecRight.x; y += _vecRight.y;
				}

				xst_fi Vec2 Sub(const Vec2& _vecRight) const
				{
					return Vec2( x - _vecRight.x, y - _vecRight.y);
				}

				xst_fi void SubAssign(const Vec2& _vecRight)
				{
					x -= _vecRight.x; y -= _vecRight.y;
				}

				xst_fi Vec2 Mul(const Vec2& _vecRight) const
				{
					return Vec2( x * _vecRight.x, y * _vecRight.y );
				}

				xst_fi void  MulAssign(const Vec2& _vecRight)
				{
					x *= _vecRight.x; y *= _vecRight.y;
				}

				xst_fi Vec2 Div(const Vec2& _vecRight) const
				{
					return Vec2( x / _vecRight.x, y / _vecRight.y );
				}

				xst_fi void DivAssign(const Vec2& _vecRight)
				{
					x /= _vecRight.x; y /= _vecRight.y;
				}

				xst_fi void AddAssign(const f32& _fScalar)
				{
					x += _fScalar; y += _fScalar;
				}

				xst_fi void SubAssign(const f32& _fScalar)
				{
					x -= _fScalar; y -= _fScalar;
				}

				xst_fi void MulAssign(const f32& _fScalar)
				{
					x *= _fScalar; y *= _fScalar;
				}

				xst_fi void DivAssign(const f32& _fScalar)
				{
					x /= _fScalar; y /= _fScalar;
				}

				xst_fi void Add(const Vec2& vecLeft, const Vec2& vecRight)
				{
					x = vecLeft.x + vecRight.x;
					y = vecLeft.y + vecRight.y;
				}

				xst_fi void Sub(const Vec2& vecLeft, const Vec2& vecRight)
				{
					x = vecLeft.x - vecRight.x;
					y = vecLeft.y - vecRight.y;
				}

				xst_fi void Mul(const Vec2& vecLeft, const Vec2& vecRight)
				{
					x = vecLeft.x * vecRight.x;
					y = vecLeft.y * vecRight.y;
				}

				xst_fi void Div(const Vec2& vecLeft, const Vec2& vecRight)
				{
					x = vecLeft.x / vecRight.x;
					y = vecLeft.y / vecRight.y;
				}

				xst_fi f32 Length() const
				{
					return Math::Sqrt( x * x + y * y );
				}

				xst_fi f32 SquaredLength() const
				{
					return x * x + y * y ;
				}

				xst_fi f32 Distance(const Vec2& RightVec) const
				{
					return ( *this - RightVec ).Length();
				}

				xst_fi f32 SquareDistance(const Vec2& RightVec) const
				{
					return ( *this - RightVec ).SquaredLength();
				}

				xst_fi f32 Dot(const Vec2& RightVec)
				{
					f32 fLen = Length();
					if( fLen > 1e-08 )
					{
						f32 fInvLen = 1.0f / fLen;
						x *= fInvLen;
						y *= fInvLen;
					}

					return fLen;
				}

				xst_fi Vec2 MidPoint(const Vec2& VecRight) const
				{
					return Vec2(	( x + VecRight.x ) * 0.5f, 
									( y + VecRight.y ) * 0.5f);
				}

				Vec2		Abs() const;
				void		Abs();
				static Vec2 Abs(const Vec2& vecOther);
		};

		static xst_fi xst_ostream& operator<<(xst_ostream& o, const Vec2& right)
		{
			o << right.x << ", " << right.y;
			return o;
		}

//		//Vector2 class
//		class XST_ALIGN(16) CVector2SSE
//		{
//
//			public:
//
//				CVector2SSE() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
//				CVector2SSE(const Vec2SSE& _vec) : xyzw(_vec.xyzw), w(0.0f) {}
//				CVector2SSE(const f32& _fValue) : x(_fValue), y(_fValue), z(_fValue), w(0.0f) {}
//				CVector2SSE(const f32& _fX, const f32& _fY) : x(_fX), y(_fY) {}
//				CVector2SSE(const m128& _xyzw) : xyzw(_xyzw), w(0.0f) {}
//				CVector2SSE(const f32 _afValues[3]) : xyzw(*(m128*)_afValues), w(0.0f) {}
//
//				~CVector2SSE() {}
//
//				xst_fi bool operator==(const Vec2SSE& _vecRight)
//				{
//					return x == _vecRight.x && y == _vecRight.y;
//				}
//
//				xst_fi bool operator>(const Vec2SSE& _vecRight)
//				{
//					return x > _vecRight.x && y > _vecRight.y;
//				}
//
//				xst_fi bool operator>=(const Vec2SSE& _vecRight)
//				{
//					return x >= _vecRight.x && y >= _vecRight.y;
//				}
//
//				xst_fi bool operator<(const Vec2SSE& _vecRight)
//				{
//					return x < _vecRight.x && y < _vecRight.y;
//				}
//
//				xst_fi bool operator<=(const Vec2SSE& _vecRight)
//				{
//					return x <= _vecRight.x && y <= _vecRight.y;
//				}
//
//				xst_fi bool operator!=(const Vec2SSE& _vecRight)
//				{
//					return x != _vecRight.x && y != _vecRight.y;
//				}
//
//				xst_fi Vec2SSE operator+(const Vec2SSE& _vecRight)
//				{
//					//return Vec2cAdd( this, _vecRight );
//					return AddSSE2( this, _vecRight );
//				}
//
//				xst_fi void operator+=(const Vec2SSE& _vecRight)
//				{
//					//x += _vecRight.x; y += _vecRight.y; z += _vecRight.z;
//					//Vec2AddEq( this, _vecRight );
//					AddEqSSE2( this, _vecRight );
//				}
//
//				xst_fi Vec2SSE operator-(const Vec2SSE& _vecRight)
//				{
//					//return Vec2cSub( this, _vecRight );
//					return SubSSE2( this, _vecRight );
//				}
//
//				xst_fi void operator-=(const Vec2SSE& _vecRight)
//				{
//					//x -= _vecRight.x; y -= _vecRight.y; z -= _vecRight.z;
//					//Vec2SubEq( this, _vecRight );
//					SubEqSSE2( this, _vecRight );
//				}
//
//				xst_fi Vec2SSE operator*(const Vec2SSE& _vecRight)
//				{
//					//return Vec2cMul( this, _vecRight );
//					return MulSSE2( this, _vecRight );
//				}
//
//				xst_fi void operator*=(const Vec2SSE& _vecRight)
//				{
//					//x *= _vecRight.x; y *= _vecRight.y; z *= _vecRight.z;
//					//Vec2MulEq( this, _vecRight );
//					MulEqSSE2( this, _vecRight );
//				}
//
//				xst_fi Vec2SSE operator/(const Vec2SSE& _vecRight)
//				{
//					return DivSSE2( (*this), _vecRight );
//				}
//
//				xst_fi void operator/=(const Vec2SSE& _vecRight)
//				{
//					//x /= _vecRight.x; y /= _vecRight.y; z /= _vecRight.z;
//					//Vec2DivEq( this, _vecRight );
//					DivEqSSE2( this, _vecRight );
//				}
//
//				xst_fi bool Eq(const Vec2SSE& _vecRight)
//				{
//					return x == _vecRight.x && y == _vecRight.y;
//				}
//
//				xst_fi bool Gt(const Vec2SSE& _vecRight)
//				{
//					return x > _vecRight.x && y > _vecRight.y;
//				}
//
//				xst_fi bool Ge(const Vec2SSE& _vecRight)
//				{
//					return x >= _vecRight.x && y >= _vecRight.y;
//				}
//
//				xst_fi bool Lt(const Vec2SSE& _vecRight)
//				{
//					return x < _vecRight.x && y < _vecRight.y;
//				}
//
//				xst_fi bool Le(const Vec2SSE& _vecRight)
//				{
//					return x <= _vecRight.x && y <= _vecRight.y;
//				}
//
//				xst_fi bool Ne(const Vec2SSE& _vecRight)
//				{
//					return x != _vecRight.x && y != _vecRight.y;
//				}
//
//				xst_fi const Vec2SSE& operator=(const Vec2SSE& _vecRight)
//				{
//					x = _vecRight.x; y = _vecRight.y;
//					return (*this);
//				}
//
//				xst_fi const Vec2SSE& Set(const Vec2SSE& _vecRight)
//				{
//					x = _vecRight.x; y = _vecRight.y; 
//					return (*this);
//				}
//
//				xst_fi const Vec2SSE& Set(const f32& _fX, const f32& _fY)
//				{
//					x = _fX; y = _fY; 
//					return (*this);
//				}
//
//				xst_fi const f32* ToArray() const
//				{
//					static f32 afArray[] = { x, y };
//					return afArray;
//				}
//
//				xst_fi const u32 GetSize() const
//				{
//					return 3;
//				}
//
//				xst_fi Vec2SSE Add(const Vec2SSE& _vecRight)
//				{
//					//return Vec2cAdd( this, _vecRight );
//					return AddSSE2( this, _vecRight );
//				}
//
//				xst_fi void AddEq(const Vec2SSE& _vecRight)
//				{
//					x += _vecRight.x; y += _vecRight.y;
//				}
//
//				xst_fi Vec2SSE Sub(const Vec2SSE& _vecRight)
//				{
//					//return Vec2cSub( this, _vecRight );
//					return SubSSE2( this, _vecRight );
//				}
//
//				xst_fi void SubEq(const Vec2SSE& _vecRight)
//				{
//					//x -= _vecRight.x; y -= _vecRight.y; z -= _vecRight.z;
//					//Vec2SubEq( this, _vecRight );
//					SubEqSSE2( this, _vecRight );
//				}
//
//				xst_fi Vec2SSE Mul(const Vec2SSE& _vecRight)
//				{
//					//return Vec2SSE(x * _vecRight.x, y * _vecRight.y, z * _vecRight.z);
//					//return Vec2cMul( this, _vecRight );
//					return MulSSE2( this, _vecRight );
//				}
//
//				xst_fi void  MulEq(const Vec2SSE& _vecRight)
//				{
//					//x *= _vecRight.x; y *= _vecRight.y; z *= _vecRight.z;
//					//Vec2MulEq( this, _vecRight );
//					MulEqSSE2( this, _vecRight );
//				}
//
//				xst_fi Vec2SSE Div(const Vec2SSE& _vecRight)
//				{
//					//return Vec2cDiv( this, _vecRight );
//					return DivSSE2( this, _vecRight );
//				}
//
//				xst_fi void DivEq(const Vec2SSE& _vecRight)
//				{
//					//x /= _vecRight.x; y /= _vecRight.y; z /= _vecRight.z;
//					//Vec2DivEq( this, _vecRight );
//					DivEqSSE2( this, _vecRight );
//				}
//
//				xst_fi void AddEq(const f32& _fScalar)
//				{
//					//Vec2AddEqS( this, _fScalar);
//					AddEqSSE2( this, _fScalar );
//				}
//
//				xst_fi void SubEq(const f32& _fScalar)
//				{
//					//Vec2SubEqS( this, _fScalar);
//					SubEqSSE2( this, _fScalar );
//				}
//
//				xst_fi void MulEq(const f32& _fScalar)
//				{
//					//Vec2MulEqS( this, _fScalar);
//					MulEqSSE2( this, _fScalar );
//				}
//
//				xst_fi void DivEq(const f32& _fScalar)
//				{
//					//Vec2DivEqS( this, _fScalar);
//					DivEqSSE2( this, _fScalar );
//				}
//
//				xst_fi f32 Length()
//				{
//					//return Math::Vec2cLength(this);
//					return LengthSSE2( this );
//				}
//
//				xst_fi f32 Dot(const Vec2SSE& _vecRight)
//				{
//					//return Vec2cDot( this, _vecRight );
//					return DotSSE2( this, _vecRight );
//				}
//
//				xst_fi const Vec2SSE& Normalize()
//				{
//					const f32 fLen = Length();
//					x /= fLen; y /= fLen; 
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
//		static xst_i f32 Length(const Vec2& _vec)
//		{
//			return sqrtf(_vec.x * _vec.x + _vec.y * _vec.y );
//		}
//
//		static xst_i f32 Dot(const Vec2& _vecLeft, const Vec2& _vecRight)
//		{
//			return ( _vecLeft.x * _vecRight.x + _vecLeft.y * _vecRight.y  );
//		}
//
//		static xst_i Vec2 Normalize(const Vec2& _vec)
//		{
//			const f32 fLen = Length(_vec);
//			return Vec2(_vec.x / fLen, _vec.y / fLen);
//		}
//
//		static xst_i Vec2 Normalize(const Vec2& _vec, const f32& _fVecLen)
//		{
//			xst_assert(_fVecLen != 0, "(XST::Math::Normalize) _fVecLen is 0.");
//			return Vec2(_vec.x / _fVecLen, _vec.y / _fVecLen);
//		}
//
//		xst_fi Vec2 Mul(const Vec2& _vecLeft, const Vec2& _vecRight)
//		{
//			//return *(Vec2*)&_mm_mul_ps(*(__m128*)&_vecLeft, *(__m128*)&_vecRight);
//			//return Vec2( _mm_mul_ps(_vecLeft.xyzw, _vecRight.xyzw) );
//			return Vec2( _vecLeft.x * _vecRight.x, _vecLeft.y * _vecRight.y );
//		}
//
//		//SOFTWARE
//		xst_fi Vec2 MulRef(const Vec2& _vecLeft, const Vec2& _vecRight)
//		{
//			return Vec2(_vecLeft.x * _vecRight.x, _vecLeft.y * _vecRight.y);
//		}
//
//		xst_fi Vec2 AddRef(const Vec2& _vecLeft, const Vec2& _vecRight)
//		{
//			return Vec2(_vecLeft.x + _vecRight.x, _vecLeft.y + _vecRight.y);
//		}
//
//		xst_fi Vec2 SubRef(const Vec2& _vecLeft, const Vec2& _vecRight)
//		{
//			return Vec2(_vecLeft.x - _vecRight.x, _vecLeft.y - _vecRight.y);
//		}
//
//		xst_fi Vec2 DivRef(const Vec2& _vecLeft, const Vec2& _vecRight)
//		{
//			return Vec2(_vecLeft.x / _vecRight.x, _vecLeft.y / _vecRight.y);
//		}
//
//		xst_fi Vec2 MulRef(const Vec2& _vecLeft, const f32& _fScalar)
//		{
//			return Vec2(_vecLeft.x * _fScalar, _vecLeft.y * _fScalar);
//		}
//
//		xst_fi Vec2 AddRef(const Vec2& _vecLeft, const f32& _fScalar)
//		{
//			return Vec2(_vecLeft.x + _fScalar, _vecLeft.y + _fScalar);
//		}
//
//		xst_fi Vec2 SubRef(const Vec2& _vecLeft, const f32& _fScalar)
//		{
//			return Vec2(_vecLeft.x - _fScalar, _vecLeft.y - _fScalar);
//		}
//
//		xst_fi Vec2 DivRef(const Vec2& _vecLeft, const f32& _fScalar)
//		{
//			return Vec2(_vecLeft.x / _fScalar, _vecLeft.y / _fScalar);
//		}
//
//		xst_fi Vec2 NormalizeRef(const Vec2& _vecLeft)
//		{
//			const f32 fLen = LengthRef( _vecLeft );
//			return Vec2( _vecLeft.x / fLen, _vecLeft.y / fLen );
//		}
//
//		//SOFTWARE (for class use)
//
//		xst_fi Vec2 MulRef(const Vec2* _vecLeft, const Vec2& _vecRight)
//		{
//			return Vec2(_vecLeft->x * _vecRight.x, _vecLeft->y * _vecRight.y);
//		}
//
//		xst_fi Vec2 AddRef(const Vec2* _vecLeft, const Vec2& _vecRight)
//		{
//			return Vec2(_vecLeft->x + _vecRight.x, _vecLeft->y + _vecRight.y);
//		}
//
//		xst_fi Vec2 SubRef(const Vec2* _vecLeft, const Vec2& _vecRight)
//		{
//			return Vec2(_vecLeft->x - _vecRight.x, _vecLeft->y - _vecRight.y);
//		}
//
//		xst_fi Vec2 DivRef(const Vec2* _vecLeft, const Vec2& _vecRight)
//		{
//			return Vec2(_vecLeft->x / _vecRight.x, _vecLeft->y / _vecRight.y);
//		}
//
//		xst_fi Vec2 MulRef(const Vec2* _vecLeft, const f32& _fScalar)
//		{
//			return Vec2(_vecLeft->x * _fScalar, _vecLeft->y * _fScalar);
//		}
//
//		xst_fi Vec2 AddRef(const Vec2* _vecLeft, const f32& _fScalar)
//		{
//			return Vec2(_vecLeft->x + _fScalar, _vecLeft->y + _fScalar);
//		}
//
//		xst_fi Vec2 SubRef(const Vec2* _vecLeft, const f32& _fScalar)
//		{
//			return Vec2(_vecLeft->x - _fScalar, _vecLeft->y - _fScalar);
//		}
//
//		xst_fi Vec2 DivRef(const Vec2* _vecLeft, const f32& _fScalar)
//		{
//			return Vec2(_vecLeft->x / _fScalar, _vecLeft->y / _fScalar);
//		}
//
//
//		xst_fi Vec2 SqrtRef(const Vec2& _vecLeft)
//		{
//			return Vec2( sqrtf(_vecLeft.x), sqrtf(_vecLeft.y) );
//		}
//
//		xst_fi Vec2 SqrtRef(const Vec2* _vecLeft)
//		{
//			return Vec2( sqrtf(_vecLeft->x), sqrtf(_vecLeft->y) );
//		}
//		xst_fi f32 DotRef(const Vec2& _vecLeft, const Vec2& _vecRight)
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
//			return _vecLeft.x * _vecRight.x + _vecLeft.y * _vecRight.y;
//		}
//
//		xst_fi f32 DotRef(const Vec2* _vecLeft, const Vec2& _vecRight)
//		{
//			return _vecLeft->x * _vecRight.x + _vecLeft->y * _vecRight.y;
//		}
//
//		xst_fi void AddEqRef(Vec2* _vecLeft, const Vec2& _vecRight)
//		{
//			_vecLeft->x += _vecRight.x; _vecLeft->y += _vecRight.y;
//		}
//
//		xst_fi void SubEqRef(Vec2* _vecLeft, const Vec2& _vecRight)
//		{
//			_vecLeft->x -= _vecRight.x; _vecLeft->y -= _vecRight.y;
//		}
//
//		xst_fi void MulEqRef(Vec2* _vecLeft, const Vec2& _vecRight)
//		{
//			_vecLeft->x *= _vecRight.x; _vecLeft->y *= _vecRight.y; 
//		}
//
//		xst_fi void DivEqRef(Vec2* _vecLeft, const Vec2& _vecRight)
//		{
//			_vecLeft->x /= _vecRight.x; _vecLeft->y /= _vecRight.y;
//		}
//
//		xst_fi void AddEqRef(Vec2* _vecLeft, const f32& _fScalar)
//		{
//			_vecLeft->x += _fScalar; _vecLeft->y += _fScalar; 
//		}
//
//		xst_fi void SubEqRef(Vec2* _vecLeft, const f32& _fScalar)
//		{
//			_vecLeft->x -= _fScalar; _vecLeft->y -= _fScalar;
//		}
//
//		xst_fi void MulEqRef(Vec2* _vecLeft, const f32& _fScalar)
//		{
//			_vecLeft->x *= _fScalar; _vecLeft->y *= _fScalar; 
//		}
//
//		xst_fi void DivEqRef(Vec2* _vecLeft, const f32& _fScalar)
//		{
//			_vecLeft->x /= _fScalar; _vecLeft->y /= _fScalar;
//		}
//
//		xst_fi f32 LengthRef(const Vec2& _vecLeft)
//		{
//			return sqrtf( _vecLeft.x * _vecLeft.x + _vecLeft.y * _vecLeft.y );
//		}
//
//		xst_fi f32 LengthRef(const Vec2* _vecLeft)
//		{
//			return sqrtf( _vecLeft->x * _vecLeft->x + _vecLeft->y * _vecLeft->y  );
//		}
//
//		xst_fi Vec2 NormalizeRef(const Vec2* _vecLeft)
//		{
//			const f32 fLen = LengthRef( _vecLeft );
//			return Vec2( _vecLeft->x / fLen, _vecLeft->y / fLen );
//		}
//
//
////#if defined (XST_SSE)
//		//SSE2
//		xst_fi Vec2SSE MulSSE2(const Vec2SSE& _vecLeft, const Vec2SSE& _vecRight)
//		{
//			return _mm_mul_ps(_vecLeft.xyzw, _vecRight.xyzw);
//		}
//
//		xst_fi Vec2SSE AddSSE2(const Vec2SSE& _vecLeft, const Vec2SSE& _vecRight)
//		{
//			return _mm_add_ps(_vecLeft.xyzw, _vecRight.xyzw);
//		}
//
//		xst_fi Vec2SSE SubSSE2(const Vec2SSE& _vecLeft, const Vec2SSE& _vecRight)
//		{
//			return _mm_sub_ps(_vecLeft.xyzw, _vecRight.xyzw);
//		}
//
//		xst_fi Vec2SSE DivSSE2(const Vec2SSE& _vecLeft, const Vec2SSE& _vecRight)
//		{
//			return _mm_div_ps(_vecLeft.xyzw, _vecRight.xyzw);
//		}
//
//		xst_fi Vec2SSE MulSSE2(const Vec2SSE& _vecLeft, const f32& _fScalar)
//		{
//			return _mm_mul_ps( _vecLeft.xyzw, _mm_load1_ps(&_fScalar) );
//		}
//
//		xst_fi Vec2SSE AddSSE2(const Vec2SSE& _vecLeft, const f32& _fScalar)
//		{
//			return _mm_add_ps( _vecLeft.xyzw, _mm_load1_ps(&_fScalar) );
//		}
//
//		xst_fi Vec2SSE SubSSE2(const Vec2SSE& _vecLeft, const f32& _fScalar)
//		{
//			return _mm_sub_ps( _vecLeft.xyzw, _mm_load1_ps(&_fScalar) );
//		}
//
//		xst_fi Vec2SSE DivSSE2(const Vec2SSE& _vecLeft, const f32& _fScalar)
//		{
//			return _mm_div_ps( _vecLeft.xyzw, _mm_load1_ps(&_fScalar) );
//		}
//
//		xst_fi Vec2SSE SqrtSSE2(const Vec2SSE& _vec)
//		{
//			return _mm_sqrt_ps( _vec.xyzw );
//		}
//
//		xst_i f32 DotSSE2(const Vec2SSE& _vecLeft, const Vec2SSE& _vecRight)
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
//		xst_fi f32 LengthSSE2(const Vec2SSE& _vecLeft)
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
//		xst_fi Vec2SSE NormalizeSSE2(const Vec2SSE& _vecLeft)
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
//		xst_fi Vec2SSE MulSSE2(const Vec2SSE* _vecLeft, const Vec2SSE& _vecRight)
//		{
//			return _mm_mul_ps(_vecLeft->xyzw, _vecRight.xyzw);
//		}
//
//		xst_fi Vec2SSE AddSSE2(const Vec2SSE* _vecLeft, const Vec2SSE& _vecRight)
//		{
//			return _mm_add_ps(_vecLeft->xyzw, _vecRight.xyzw);
//		}
//
//		xst_fi Vec2SSE SubSSE2(const Vec2SSE* _vecLeft, const Vec2SSE& _vecRight)
//		{
//			return _mm_sub_ps(_vecLeft->xyzw, _vecRight.xyzw);
//		}
//
//		xst_fi Vec2SSE DivSSE2(const Vec2SSE* _vecLeft, const Vec2SSE& _vecRight)
//		{
//			return _mm_div_ps(_vecLeft->xyzw, _vecRight.xyzw);
//		}
//
//		xst_fi Vec2SSE MulSSE2(const Vec2SSE* _vecLeft, const f32& _fScalar)
//		{
//			return _mm_mul_ps( _vecLeft->xyzw, _mm_load1_ps(&_fScalar) );
//		}
//
//		xst_fi Vec2SSE AddSSE2(const Vec2SSE* _vecLeft, const f32& _fScalar)
//		{
//			return _mm_add_ps( _vecLeft->xyzw, _mm_load1_ps(&_fScalar) );
//		}
//
//		xst_fi Vec2SSE SubSSE2(const Vec2SSE* _vecLeft, const f32& _fScalar)
//		{
//			return _mm_sub_ps( _vecLeft->xyzw, _mm_load1_ps(&_fScalar) );
//		}
//
//		xst_fi Vec2SSE DivSSE2(const Vec2SSE* _vecLeft, const f32& _fScalar)
//		{
//			return _mm_div_ps( _vecLeft->xyzw, _mm_load1_ps(&_fScalar) );
//		}
//
//		xst_fi Vec2SSE SqrtSSE2(const Vec2SSE* _vec)
//		{
//			return _mm_sqrt_ps( _vec->xyzw );
//		}
//
//		xst_fi f32 DotSSE2(const Vec2SSE* _vecLeft, const Vec2SSE& _vecRight)
//		{
//			m128 r1 = _mm_mul_ps( _vecLeft->xyzw , _vecRight.xyzw );
//			r1 = _mm_add_ps( r1, _mm_movehl_ps(r1, r1));
//			r1 = _mm_add_ps( r1, _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(3, 2, 1, 1)));
//			f32 fResult;
//			_mm_store_ss( &fResult, r1 ); // zapisz 1 float
//			return fResult;
//		}
//
//		xst_fi void AddEqSSE2(Vec2SSE* _vecLeft, const Vec2SSE& _vecRight)
//		{
//			_vecLeft->xyzw = _mm_add_ps( _vecLeft->xyzw, _vecRight.xyzw );
//		}
//
//		xst_fi void SubEqSSE2(Vec2SSE* _vecLeft, const Vec2SSE& _vecRight)
//		{
//			_vecLeft->xyzw = _mm_sub_ps( _vecLeft->xyzw, _vecRight.xyzw );
//		}
//
//		xst_fi void MulEqSSE2(Vec2SSE* _vecLeft, const Vec2SSE& _vecRight)
//		{
//			_vecLeft->xyzw = _mm_mul_ps( _vecLeft->xyzw, _vecRight.xyzw );
//		}
//
//		xst_fi void DivEqSSE2(Vec2SSE* _vecLeft, const Vec2SSE& _vecRight)
//		{
//			_vecLeft->xyzw = _mm_div_ps( _vecLeft->xyzw, _vecRight.xyzw );
//		}
//
//		xst_fi void AddEqSSE2(Vec2SSE* _vecLeft, const f32& _fScalar)
//		{
//			_vecLeft->xyzw = _mm_add_ps( _vecLeft->xyzw, _mm_load1_ps( &_fScalar ) );
//		}
//
//		xst_fi void SubEqSSE2(Vec2SSE* _vecLeft, const f32& _fScalar)
//		{
//			_vecLeft->xyzw = _mm_sub_ps( _vecLeft->xyzw, _mm_load1_ps( &_fScalar ) );
//		}
//
//		xst_fi void MulEqSSE2(Vec2SSE* _vecLeft, const f32& _fScalar)
//		{
//			_vecLeft->xyzw = _mm_mul_ps( _vecLeft->xyzw, _mm_load1_ps( &_fScalar ) );
//		}
//
//		xst_fi void DivEqSSE2(Vec2SSE* _vecLeft, const f32& _fScalar)
//		{
//			_vecLeft->xyzw = _mm_div_ps( _vecLeft->xyzw, _mm_load1_ps( &_fScalar ) );
//		}
//
//		xst_fi f32 LengthSSE2(const Vec2SSE* _vecLeft)
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
//		xst_fi Vec2SSE NormalizeSSE2(const Vec2SSE* _vecLeft)
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
//		xst_fi f32	DotSSE4(const Vec2SSE& _vecLeft, const Vec2SSE& _vecRight)
//		{
//			return xst_m128( _mm_dp_ps( _vecLeft.xyzw, _vecRight.xyzw, 0x55 ) ).m128_f32[0];
//		}
//
//		//SSE4 (class)
//		xst_fi f32	DotSSE4(const Vec2SSE* _vecLeft, const Vec2SSE& _vecRight)
//		{
//			return xst_m128( _mm_dp_ps( _vecLeft->xyzw, _vecRight.xyzw, 0x55 ) ).m128_f32[0];
//		}
	}//math
}//xst

#endif