#ifndef XST_MATH_H
#define XST_MATH_H


#include "XSTCommonTypes.h"
#include <cmath>

namespace XST
{
	namespace Math
	{
		using namespace XST::Types;
		//#define  xst_min( _v1, _v2 ) ( ( ( _v1 ) < ( _v2 ) )? ( _v1 ) : ( _v2 ) )
		//#define  xst_max( _v1, _v2 ) ( ( ( _v1 ) > ( _v2 ) )? ( _v1 ) : ( _v2 ) )

		template<typename _T_>
		xst_fi _T_ xst_min(const _T_& _t1, const _T_& _t2) { return ( _t1 < _t2 )? _t1 : _t2; }

		template<typename _T_>
		xst_fi _T_ xst_max(const _T_& _t1, const _T_& _t2) { return ( _t1 < _t2 )? _t2 : _t1; }

		template<typename _T_>
		xst_fi _T_ Clamp(const _T_& x, const _T_& a, const _T_& b) { return x < a ? a : (x > b ? b : x); }

		static xst_fi f32 Abs(f32 _fValue)
		{
			union { float a; unsigned int b; } f;
			f.a = _fValue; f.b &= 0x7FFFFFFFU;
			return f.a;
		}

		class XST_ALIGN(16) CVector3;
		typedef XST_ALIGN(16) CVector3 Vec3;

		typedef Vec3 (*Vector3Func1)(const Vec3& _vec);
		typedef Vec3 (*Vector3Func1a)(const Vec3* _pvec);
		typedef Vec3 (*Vector3Func2)(const Vec3& _vecLeft, const Vec3& _vecRight);
		typedef Vec3 (*Vector3Func2a)(const Vec3* _pvecLeft, const Vec3& _vecRight);
		typedef Vec3 (*Vector3Func3)(const Vec3& _vecLeft, const f32& _fScalar);
		typedef Vec3 (*Vector3Func3a)(const Vec3* _vecLeft, const f32& _fScalar);
		typedef f32 (*Vector3Func4)(const Vec3& _vecLeft, const Vec3& _vecRight);
		typedef f32 (*Vector3Func4c)(const Vec3* _vecLeft, const Vec3& _vecRight);
		typedef void (*Vector3Func5c)(Vec3* _vecLeft, const Vec3& _vecRight);
		typedef void (*Vector3Func5ca)(Vec3* _vecLeft, const f32& _fScalar);
		typedef f32 (*Vector3Func6)(const Vec3& _vecLeft);
		typedef f32 (*Vector3Func6c)(const Vec3* _vecLeft);
		typedef f32 (*FloatFunc1)(const f32& _fValue);

		//Math functions

		//SOFTWARE
		xst_fi f32 SqrtRef(const f32& _fValue)
		{
			return sqrtf( _fValue );
		}

		//SSE2
		xst_fi f32 SqrtSSE2(const f32& _fValue)
		{
			/*f32 fValue;
			_mm_store_ss( &fValue, _mm_sqrt_ss( _mm_load_ss( &_fValue ) ) );
			return fValue;*/
            xst_m128 m;
            m = _mm_sqrt_ss( _mm_load_ss( &_fValue ) );
            return m.m128_f32[0];
			//return _mm_sqrt_ss( _mm_load_ss( &_fValue ) ).m128_f32[0];
		}

		//static xst_i f32 Length(const Vec3& _vec);
		static xst_i f32 Dot(const Vec3& _vecLeft, const Vec3& _vecRight);
		static xst_i Vec3 Normalize(const Vec3& _vec);
		static xst_i Vec3 Normalize(const Vec3& _vec, const f32& _fVecLen);
		xst_fi Vec3 Mul(const Vec3& _vecLeft, const Vec3& _vecRight);

		//Software
		xst_fi Vec3 MulRef(const Vec3& _vecLeft, const Vec3& _vecRight);
		xst_fi Vec3	MulRef(const Vec3& _vecLeft, const f32& _fScalar);
		xst_fi Vec3 AddRef(const Vec3& _vecLeft, const Vec3& _vecRight);
		xst_fi Vec3	AddRef(const Vec3& _vecLeft, const f32& _fScalar);
		xst_fi Vec3 SubRef(const Vec3& _vecLeft, const Vec3& _vecRight);
		xst_fi Vec3	SubRef(const Vec3& _vecLeft, const f32& _fScalar);
		xst_fi Vec3 DivRef(const Vec3& _vecLeft, const Vec3& _vecRight);
		xst_fi Vec3	DivRef(const Vec3& _vecLeft, const f32& _fScalar);

		xst_fi Vec3	SqrtRef(const Vec3& _vecLeft);
		xst_fi f32	DotRef(const Vec3& _vecLeft, const Vec3& _vecRight);
		xst_fi f32	LengthRef(const Vec3& _vecLeft);

		//SOFTWARE (for class use)
		xst_fi Vec3 AddRef(const Vec3* _vecLeft, const Vec3& _vecRight);
		xst_fi Vec3	AddRef(const Vec3* _vecLeft, const f32& _fScalar);
		xst_fi void AddEqRef(Vec3* _vecLeft, const Vec3& _vecRight);
		xst_fi void	AddEqRef(Vec3* _vecLeft, const f32& _fScalar);
		xst_fi Vec3 SubRef(const Vec3* _vecLeft, const Vec3& _vecRight);
		xst_fi Vec3	SubRef(const Vec3* _vecLeft, const f32& _fScalar);
		xst_fi void SubEqRef(Vec3* _vecLeft, const Vec3& _vecRight);
		xst_fi void	SubEqRef(Vec3* _vecLeft, const f32& _fScalar);
		xst_fi Vec3 MulRef(const Vec3* _vecLeft, const Vec3& _vecRight);
		xst_fi Vec3	MulRef(const Vec3* _vecLeft, const f32& _fScalar);
		xst_fi void MulEqRef(Vec3* _vecLeft, const Vec3& _vecRight);
		xst_fi void	MulEqRef(Vec3* _vecLeft, const f32& _fScalar);
		xst_fi Vec3 DivRef(const Vec3* _vecLeft, const Vec3& _vecRight);
		xst_fi Vec3	DivRef(const Vec3* _vecLeft, const f32& _fScalar);
		xst_fi void DivEqRef(Vec3* _vecLeft, const Vec3& _vecRight);
		xst_fi void	DivEqRef(Vec3* _vecLeft, const f32& _fScalar);

		xst_fi Vec3	SqrtRef(const Vec3* _vecLeft);
		xst_fi f32	DotRef(const Vec3* _vecLeft, const Vec3& _vecRight);
		xst_fi f32	LengthRef(const Vec3* _vecLeft);


		//SSE2
		xst_fi Vec3 MulSSE2(const Vec3& _vecLeft, const Vec3& _vecRight);
		xst_fi Vec3 AddSSE2(const Vec3& _vecLeft, const Vec3& _vecRight);
		xst_fi Vec3 SubSSE2(const Vec3& _vecLeft, const Vec3& _vecRight);
		xst_fi Vec3 DivSSE2(const Vec3& _vecLeft, const Vec3& _vecRight);
		xst_fi Vec3	SqrtSSE2(const Vec3& _vecLeft);
		xst_i f32	DotSSE2(const Vec3& _vecLeft, const Vec3& _vecRight);
		xst_fi f32	LengthSSE2(const Vec3& _vecLeft);

		//SSE2 Scalar
		xst_fi Vec3 AddSSE2(const Vec3& _vecLeft, const f32& _fScalar);
		xst_fi Vec3 SubSSE2(const Vec3& _vecLeft, const f32& _fScalar);
		xst_fi Vec3 MulSSE2(const Vec3& _vecLeft, const f32& _fScalar);
		xst_fi Vec3 DivSSE2(const Vec3& _vecLeft, const f32& _fScalar);

		//SSE2 (for class use)
		xst_fi Vec3 MulSSE2(const Vec3* _vecLeft, const Vec3& _vecRight);
		xst_fi Vec3 AddSSE2(const Vec3* _vecLeft, const Vec3& _vecRight);
		xst_fi Vec3 SubSSE2(const Vec3* _vecLeft, const Vec3& _vecRight);
		xst_fi Vec3 DivSSE2(const Vec3* _vecLeft, const Vec3& _vecRight);
		xst_fi void MulEqSSE2(Vec3* _vecLeft, const Vec3& _vecRight);
		xst_fi void AddEqSSE2(Vec3* _vecLeft, const Vec3& _vecRight);
		xst_fi void SubEqSSE2(Vec3* _vecLeft, const Vec3& _vecRight);
		xst_fi void DivEqSSE2(Vec3* _vecLeft, const Vec3& _vecRight);

		xst_fi Vec3	SqrtSSE2(const Vec3& _vecLeft);
		xst_fi f32	DotSSE2(const Vec3* _vecLeft, const Vec3& _vecRight);
		xst_fi f32	LengthSSE2(const Vec3* _vecLeft);

		//SSE2 Scalar
		xst_fi Vec3 AddSSE2(const Vec3* _vecLeft, const f32& _fScalar);
		xst_fi Vec3 SubSSE2(const Vec3* _vecLeft, const f32& _fScalar);
		xst_fi Vec3 MulSSE2(const Vec3* _vecLeft, const f32& _fScalar);
		xst_fi Vec3 DivSSE2(const Vec3* _vecLeft, const f32& _fScalar);
		xst_fi void MulEqSSE2(Vec3* _vecLeft, const f32& _fScalar);
		xst_fi void AddEqSSE2(Vec3* _vecLeft, const f32& _fScalar);
		xst_fi void SubEqSSE2(Vec3* _vecLeft, const f32& _fScalar);
		xst_fi void DivEqSSE2(Vec3* _vecLeft, const f32& _fScalar);

		//SSE4
		xst_fi f32	DotSSE4(const Vec3& _vecLeft, const Vec3& _vecRight);

		//SSE4 (class)
		xst_fi f32	DotSSE4(const Vec3* _vecLeft, const Vec3& _vecRight);


		//Function pointers
		static Vector3Func2 Vec3Mul	= &MulRef;
		static Vector3Func2 Vec3Add	= &AddRef;
		static Vector3Func2 Vec3Sub	= &SubRef;
		static Vector3Func2 Vec3Div	= &DivRef;

		static Vector3Func3 Vec3MulS	= &MulRef;
		static Vector3Func3 Vec3AddS	= &AddRef;
		static Vector3Func3 Vec3SubS	= &SubRef;
		static Vector3Func3 Vec3DivS	= &DivRef;

		static Vector3Func2a Vec3cAdd	= &AddRef;
		static Vector3Func2a Vec3cSub	= &SubRef;
		static Vector3Func2a Vec3cMul	= &MulRef;
		static Vector3Func2a Vec3cDiv	= &DivRef;

		static Vector3Func3a Vec3cAddS	= &AddRef;
		static Vector3Func3a Vec3cSubS	= &SubRef;
		static Vector3Func3a Vec3cMulS	= &MulRef;
		static Vector3Func3a Vec3cDivS	= &DivRef;

		static Vector3Func1		Vec3Sqrt	= &SqrtRef;
		static Vector3Func1a	Vec3cSqrt	= &SqrtRef;

		static Vector3Func4		Vec3Dot		= &DotRef;
		static Vector3Func4c	Vec3cDot	= &DotRef;

		static Vector3Func5c	Vec3AddEq	= &AddEqRef;
		static Vector3Func5c	Vec3SubEq	= &SubEqRef;
		static Vector3Func5c	Vec3MulEq	= &MulEqRef;
		static Vector3Func5c	Vec3DivEq	= &DivEqRef;

		static Vector3Func5ca	Vec3AddEqS	= &AddEqRef;
		static Vector3Func5ca	Vec3SubEqS	= &SubEqRef;
		static Vector3Func5ca	Vec3MulEqS	= &MulEqRef;
		static Vector3Func5ca	Vec3DivEqS	= &DivEqRef;

		static Vector3Func6		Vec3Length	= &LengthRef;
		static Vector3Func6c	Vec3cLength	= &LengthRef;

		static FloatFunc1 Sqrt = &SqrtRef;

		

		//Macros
		#define XSTVec3Mul(_vec3Left, _vec3Right)	Vec3Mul(_vec3Left, _vec3Right)
		#define XSTVec3Add(_vec3Left, _vec3Right)	Vec3Add(_vec3Left, _vec3Right)
		#define XSTVec3Div(_vec3Left, _vec3Right)	Vec3Div(_vec3Left, _vec3Right)
		#define XSTVec3Sub(_vec3Left, _vec3Right)	Vec3Sub(_vec3Left, _vec3Right)
		#define XSTVec3MulS(_vec3Left, _fScalar)	Vec3MulS(_vec3Left, _vec3Right)
		#define XSTVec3AddS(_vec3Left, _fScalar)	Vec3AddS(_vec3Left, _vec3Right)
		#define XSTVec3DivS(_vec3Left, _fScalar)	Vec3DivS(_vec3Left, _vec3Right)
		#define XSTVec3SubS(_vec3Left, _fScalar)	Vec3SubS(_vec3Left, _vec3Right)
		#define XSTVec3Sqrt(_vec3)					Vec3Sqrt(_vec3)
		#define XSTVec3Dot(_vec3Left, _vec3Right)	Vec3Dot(_vec3Left, _vec3Right)
		#define XSTVec3Length(_vec3Left)			Vec3Length(_vec3Left)

		//Vector3 class
		class XST_ALIGN(16) CVector3
		{

			public:

				CVector3() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
				CVector3(const Vec3& _vec) : xyzw(_vec.xyzw), w(0.0f) {}
				CVector3(const f32& _fValue) : x(_fValue), y(_fValue), z(_fValue), w(0.0f) {}
				CVector3(const f32& _fX, const f32& _fY, const f32& _fZ) : x(_fX), y(_fY), z(_fZ), w(0.0f) {}
				CVector3(const m128& _xyzw) : xyzw(_xyzw), w(0.0f) {}
				CVector3(const f32 _afValues[3]) : xyzw(*(m128*)_afValues), w(0.0f) {}

				~CVector3() {}

				xst_fi bool operator==(const Vec3& _vecRight)
				{
					return x == _vecRight.x && y == _vecRight.y && z == _vecRight.z;
				}

				xst_fi bool operator>(const Vec3& _vecRight)
				{
					return x > _vecRight.x && y > _vecRight.y && z > _vecRight.z;
				}

				xst_fi bool operator>=(const Vec3& _vecRight)
				{
					return x >= _vecRight.x && y >= _vecRight.y && z >= _vecRight.z;
				}

				xst_fi bool operator<(const Vec3& _vecRight)
				{
					return x < _vecRight.x && y < _vecRight.y && z < _vecRight.z;
				}

				xst_fi bool operator<=(const Vec3& _vecRight)
				{
					return x <= _vecRight.x && y <= _vecRight.y && z <= _vecRight.z;
				}

				xst_fi bool operator!=(const Vec3& _vecRight)
				{
					return x != _vecRight.x && y != _vecRight.y && z != _vecRight.z;
				}

				xst_fi Vec3 operator+(const Vec3& _vecRight)
				{
					return Vec3cAdd( this, _vecRight );
				}

				xst_fi void operator+=(const Vec3& _vecRight)
				{
					//x += _vecRight.x; y += _vecRight.y; z += _vecRight.z;
					Vec3AddEq( this, _vecRight );
				}

				xst_fi Vec3 operator-(const Vec3& _vecRight)
				{
					return Vec3cSub( this, _vecRight );
				}

				xst_fi void operator-=(const Vec3& _vecRight)
				{
					//x -= _vecRight.x; y -= _vecRight.y; z -= _vecRight.z;
					Vec3SubEq( this, _vecRight );
				}

				xst_fi Vec3 operator*(const Vec3& _vecRight)
				{
					return Vec3cMul( this, _vecRight );
				}

				xst_fi void operator*=(const Vec3& _vecRight)
				{
					//x *= _vecRight.x; y *= _vecRight.y; z *= _vecRight.z;
					Vec3MulEq( this, _vecRight );
				}

				xst_fi Vec3 operator/(const Vec3& _vecRight)
				{
					return XSTVec3Div( (*this), _vecRight );
				}

				xst_fi void operator/=(const Vec3& _vecRight)
				{
					//x /= _vecRight.x; y /= _vecRight.y; z /= _vecRight.z;
					Vec3DivEq( this, _vecRight );
				}

				xst_fi bool Eq(const Vec3& _vecRight)
				{
					return x == _vecRight.x && y == _vecRight.y && z == _vecRight.z;
				}

				xst_fi bool Gt(const Vec3& _vecRight)
				{
					return x > _vecRight.x && y > _vecRight.y && z > _vecRight.z;
				}

				xst_fi bool Ge(const Vec3& _vecRight)
				{
					return x >= _vecRight.x && y >= _vecRight.y && z >= _vecRight.z;
				}

				xst_fi bool Lt(const Vec3& _vecRight)
				{
					return x < _vecRight.x && y < _vecRight.y && z < _vecRight.z;
				}

				xst_fi bool Le(const Vec3& _vecRight)
				{
					return x <= _vecRight.x && y <= _vecRight.y && z <= _vecRight.z;
				}

				xst_fi bool Ne(const Vec3& _vecRight)
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

				xst_fi const f32* ToArray() const
				{
					static f32 afArray[] = { x, y, z, w };
					return afArray;
				}

				xst_fi const u32 GetSize() const
				{
					return 3;
				}

				xst_fi Vec3 Add(const Vec3& _vecRight)
				{
					return Vec3cAdd( this, _vecRight );
				}

				xst_fi void AddEq(const Vec3& _vecRight)
				{
					x += _vecRight.x; y += _vecRight.y; z += _vecRight.z;
				}

				xst_fi Vec3 Sub(const Vec3& _vecRight)
				{
					return Vec3cSub( this, _vecRight );
				}

				xst_fi void SubEq(const Vec3& _vecRight)
				{
					//x -= _vecRight.x; y -= _vecRight.y; z -= _vecRight.z;
					Vec3SubEq( this, _vecRight );
				}

				xst_fi Vec3 Mul(const Vec3& _vecRight)
				{
					//return Vec3(x * _vecRight.x, y * _vecRight.y, z * _vecRight.z);
					return Vec3cMul( this, _vecRight );
				}

				xst_fi void  MulEq(const Vec3& _vecRight)
				{
					//x *= _vecRight.x; y *= _vecRight.y; z *= _vecRight.z;
					Vec3MulEq( this, _vecRight );
				}

				xst_fi Vec3 Div(const Vec3& _vecRight)
				{
					return Vec3cDiv( this, _vecRight );
				}

				xst_fi void DivEq(const Vec3& _vecRight)
				{
					//x /= _vecRight.x; y /= _vecRight.y; z /= _vecRight.z;
					Vec3DivEq( this, _vecRight );
				}

				xst_fi void AddEq(const f32& _fScalar)
				{
					Vec3AddEqS( this, _fScalar);
				}

				xst_fi void SubEq(const f32& _fScalar)
				{
					Vec3SubEqS( this, _fScalar);
				}

				xst_fi void MulEq(const f32& _fScalar)
				{
					Vec3MulEqS( this, _fScalar);
				}

				xst_fi void DivEq(const f32& _fScalar)
				{
					Vec3DivEqS( this, _fScalar);
				}

				xst_fi f32 Length()
				{
					return Math::Vec3cLength(this);
				}

				xst_fi f32 Dot(const Vec3& _vecRight)
				{
					return Vec3cDot( this, _vecRight );
				}

				xst_fi const Vec3& Normalize()
				{
					const f32 fLen = Length();
					x /= fLen; y /= fLen; z /= fLen;
					return (*this);
				}

				xst_fi const Vec3& Sqrt()
				{
					xyzw = *(m128*)&Vec3cSqrt( this );
					return (*this);
				}

			public:

				union
				{
					XST_ALIGN(16) struct
					{
						f32 x;
						f32 y;
						f32 z;
						f32 w;
					};

					XST_ALIGN(16) m128 xyzw;
				};
		};

		//FUNCTION DEFINITIONS

		static xst_i f32 Length(const Vec3& _vec)
		{
			return sqrtf(_vec.x * _vec.x + _vec.y * _vec.y + _vec.z * _vec.z);
		}

		static xst_i f32 Dot(const Vec3& _vecLeft, const Vec3& _vecRight)
		{
			return ( _vecLeft.x * _vecRight.x + _vecLeft.y * _vecRight.y + _vecLeft.z * _vecRight.z );
		}

		static xst_i Vec3 Normalize(const Vec3& _vec)
		{
			const f32 fLen = Length(_vec);
			return Vec3(_vec.x / fLen, _vec.y / fLen, _vec.z / fLen);
		}

		static xst_i Vec3 Normalize(const Vec3& _vec, const f32& _fVecLen)
		{
			xst_assert(_fVecLen != 0, "(XST::Math::Normalize) _fVecLen is 0.");
			return Vec3(_vec.x / _fVecLen, _vec.y / _fVecLen, _vec.z / _fVecLen);
		}

		xst_fi Vec3 Mul(const Vec3& _vecLeft, const Vec3& _vecRight)
		{
			//return *(Vec3*)&_mm_mul_ps(*(__m128*)&_vecLeft, *(__m128*)&_vecRight);
			return Vec3( _mm_mul_ps(_vecLeft.xyzw, _vecRight.xyzw) );
		}

		//SOFTWARE
		xst_fi Vec3 MulRef(const Vec3& _vecLeft, const Vec3& _vecRight)
		{
			return Vec3(_vecLeft.x * _vecRight.x, _vecLeft.y * _vecRight.y, _vecLeft.z * _vecRight.z);
		}

		xst_fi Vec3 AddRef(const Vec3& _vecLeft, const Vec3& _vecRight)
		{
			return Vec3(_vecLeft.x + _vecRight.x, _vecLeft.y + _vecRight.y, _vecLeft.z + _vecRight.z);
		}

		xst_fi Vec3 SubRef(const Vec3& _vecLeft, const Vec3& _vecRight)
		{
			return Vec3(_vecLeft.x - _vecRight.x, _vecLeft.y - _vecRight.y, _vecLeft.z - _vecRight.z);
		}

		xst_fi Vec3 DivRef(const Vec3& _vecLeft, const Vec3& _vecRight)
		{
			return Vec3(_vecLeft.x / _vecRight.x, _vecLeft.y / _vecRight.y, _vecLeft.z / _vecRight.z);
		}

		xst_fi Vec3 MulRef(const Vec3& _vecLeft, const f32& _fScalar)
		{
			return Vec3(_vecLeft.x * _fScalar, _vecLeft.y * _fScalar, _vecLeft.z * _fScalar);
		}

		xst_fi Vec3 AddRef(const Vec3& _vecLeft, const f32& _fScalar)
		{
			return Vec3(_vecLeft.x + _fScalar, _vecLeft.y + _fScalar, _vecLeft.z + _fScalar);
		}

		xst_fi Vec3 SubRef(const Vec3& _vecLeft, const f32& _fScalar)
		{
			return Vec3(_vecLeft.x - _fScalar, _vecLeft.y - _fScalar, _vecLeft.z - _fScalar);
		}

		xst_fi Vec3 DivRef(const Vec3& _vecLeft, const f32& _fScalar)
		{
			return Vec3(_vecLeft.x / _fScalar, _vecLeft.y / _fScalar, _vecLeft.z / _fScalar);
		}

		xst_fi Vec3 NormalizeRef(const Vec3& _vecLeft)
		{
			const f32 fLen = LengthRef( _vecLeft );
			return Vec3( _vecLeft.x / fLen, _vecLeft.y / fLen, _vecLeft.z / fLen );
		}

		//SOFTWARE (for class use)

		xst_fi Vec3 MulRef(const Vec3* _vecLeft, const Vec3& _vecRight)
		{
			return Vec3(_vecLeft->x * _vecRight.x, _vecLeft->y * _vecRight.y, _vecLeft->z * _vecRight.z);
		}

		xst_fi Vec3 AddRef(const Vec3* _vecLeft, const Vec3& _vecRight)
		{
			return Vec3(_vecLeft->x + _vecRight.x, _vecLeft->y + _vecRight.y, _vecLeft->z + _vecRight.z);
		}

		xst_fi Vec3 SubRef(const Vec3* _vecLeft, const Vec3& _vecRight)
		{
			return Vec3(_vecLeft->x - _vecRight.x, _vecLeft->y - _vecRight.y, _vecLeft->z - _vecRight.z);
		}

		xst_fi Vec3 DivRef(const Vec3* _vecLeft, const Vec3& _vecRight)
		{
			return Vec3(_vecLeft->x / _vecRight.x, _vecLeft->y / _vecRight.y, _vecLeft->z / _vecRight.z);
		}

		xst_fi Vec3 MulRef(const Vec3* _vecLeft, const f32& _fScalar)
		{
			return Vec3(_vecLeft->x * _fScalar, _vecLeft->y * _fScalar, _vecLeft->z * _fScalar);
		}

		xst_fi Vec3 AddRef(const Vec3* _vecLeft, const f32& _fScalar)
		{
			return Vec3(_vecLeft->x + _fScalar, _vecLeft->y + _fScalar, _vecLeft->z + _fScalar);
		}

		xst_fi Vec3 SubRef(const Vec3* _vecLeft, const f32& _fScalar)
		{
			return Vec3(_vecLeft->x - _fScalar, _vecLeft->y - _fScalar, _vecLeft->z - _fScalar);
		}

		xst_fi Vec3 DivRef(const Vec3* _vecLeft, const f32& _fScalar)
		{
			return Vec3(_vecLeft->x / _fScalar, _vecLeft->y / _fScalar, _vecLeft->z / _fScalar);
		}


		xst_fi Vec3 SqrtRef(const Vec3& _vecLeft)
		{
			return Vec3( sqrtf(_vecLeft.x), sqrtf(_vecLeft.y), sqrtf(_vecLeft.z) );
		}

		xst_fi Vec3 SqrtRef(const Vec3* _vecLeft)
		{
			return Vec3( sqrtf(_vecLeft->x), sqrtf(_vecLeft->y), sqrtf(_vecLeft->z) );
		}
		xst_fi f32 DotRef(const Vec3& _vecLeft, const Vec3& _vecRight)
		{
			/*
			0135FB2E  mov         eax,dword ptr [_vecLeft]
			0135FB31  fld         dword ptr [eax]
			0135FB33  mov         ecx,dword ptr [_vecRight]
			0135FB36  fmul        dword ptr [ecx]
			0135FB38  mov         edx,dword ptr [_vecLeft]
			0135FB3B  fld         dword ptr [edx+4]
			0135FB3E  mov         eax,dword ptr [_vecRight]
			0135FB41  fmul        dword ptr [eax+4]
			0135FB44  faddp       st(1),st
			0135FB46  mov         ecx,dword ptr [_vecLeft]
			0135FB49  fld         dword ptr [ecx+8]
			0135FB4C  mov         edx,dword ptr [_vecRight]
			0135FB4F  fmul        dword ptr [edx+8]
			0135FB52  faddp       st(1),st
			0135FB54  fstp        dword ptr [ebp-0C4h]
			0135FB5A  fld         dword ptr [ebp-0C4h]
			*/
			return _vecLeft.x * _vecRight.x + _vecLeft.y * _vecRight.y + _vecLeft.z * _vecRight.z;
		}

		xst_fi f32 DotRef(const Vec3* _vecLeft, const Vec3& _vecRight)
		{
			return _vecLeft->x * _vecRight.x + _vecLeft->y * _vecRight.y + _vecLeft->z * _vecRight.z;
		}

		xst_fi void AddEqRef(Vec3* _vecLeft, const Vec3& _vecRight)
		{
			_vecLeft->x += _vecRight.x; _vecLeft->y += _vecRight.y; _vecLeft->z += _vecRight.z;
		}

		xst_fi void SubEqRef(Vec3* _vecLeft, const Vec3& _vecRight)
		{
			_vecLeft->x -= _vecRight.x; _vecLeft->y -= _vecRight.y; _vecLeft->z -= _vecRight.z;
		}

		xst_fi void MulEqRef(Vec3* _vecLeft, const Vec3& _vecRight)
		{
			_vecLeft->x *= _vecRight.x; _vecLeft->y *= _vecRight.y; _vecLeft->z *= _vecRight.z;
		}

		xst_fi void DivEqRef(Vec3* _vecLeft, const Vec3& _vecRight)
		{
			_vecLeft->x /= _vecRight.x; _vecLeft->y /= _vecRight.y; _vecLeft->z /= _vecRight.z;
		}

		xst_fi void AddEqRef(Vec3* _vecLeft, const f32& _fScalar)
		{
			_vecLeft->x += _fScalar; _vecLeft->y += _fScalar; _vecLeft->z += _fScalar;
		}

		xst_fi void SubEqRef(Vec3* _vecLeft, const f32& _fScalar)
		{
			_vecLeft->x -= _fScalar; _vecLeft->y -= _fScalar; _vecLeft->z -= _fScalar;
		}

		xst_fi void MulEqRef(Vec3* _vecLeft, const f32& _fScalar)
		{
			_vecLeft->x *= _fScalar; _vecLeft->y *= _fScalar; _vecLeft->z *= _fScalar;
		}

		xst_fi void DivEqRef(Vec3* _vecLeft, const f32& _fScalar)
		{
			_vecLeft->x /= _fScalar; _vecLeft->y /= _fScalar; _vecLeft->z /= _fScalar;
		}

		xst_fi f32 LengthRef(const Vec3& _vecLeft)
		{
			return sqrtf( _vecLeft.x * _vecLeft.x + _vecLeft.y * _vecLeft.y + _vecLeft.z * _vecLeft.z );
		}

		xst_fi f32 LengthRef(const Vec3* _vecLeft)
		{
			return sqrtf( _vecLeft->x * _vecLeft->x + _vecLeft->y * _vecLeft->y + _vecLeft->z * _vecLeft->z );
		}

		xst_fi Vec3 NormalizeRef(const Vec3* _vecLeft)
		{
			const f32 fLen = LengthRef( _vecLeft );
			return Vec3( _vecLeft->x / fLen, _vecLeft->y / fLen, _vecLeft->z / fLen );
		}


		//SSE2
		xst_fi Vec3 MulSSE2(const Vec3& _vecLeft, const Vec3& _vecRight)
		{
			return _mm_mul_ps(_vecLeft.xyzw, _vecRight.xyzw);
		}

		xst_fi Vec3 AddSSE2(const Vec3& _vecLeft, const Vec3& _vecRight)
		{
			return _mm_add_ps(_vecLeft.xyzw, _vecRight.xyzw);
		}

		xst_fi Vec3 SubSSE2(const Vec3& _vecLeft, const Vec3& _vecRight)
		{
			return _mm_sub_ps(_vecLeft.xyzw, _vecRight.xyzw);
		}

		xst_fi Vec3 DivSSE2(const Vec3& _vecLeft, const Vec3& _vecRight)
		{
			return _mm_div_ps(_vecLeft.xyzw, _vecRight.xyzw);
		}

		xst_fi Vec3 MulSSE2(const Vec3& _vecLeft, const f32& _fScalar)
		{
			return _mm_mul_ps( _vecLeft.xyzw, _mm_load1_ps(&_fScalar) );
		}

		xst_fi Vec3 AddSSE2(const Vec3& _vecLeft, const f32& _fScalar)
		{
			return _mm_add_ps( _vecLeft.xyzw, _mm_load1_ps(&_fScalar) );
		}

		xst_fi Vec3 SubSSE2(const Vec3& _vecLeft, const f32& _fScalar)
		{
			return _mm_sub_ps( _vecLeft.xyzw, _mm_load1_ps(&_fScalar) );
		}

		xst_fi Vec3 DivSSE2(const Vec3& _vecLeft, const f32& _fScalar)
		{
			return _mm_div_ps( _vecLeft.xyzw, _mm_load1_ps(&_fScalar) );
		}

		xst_fi Vec3 SqrtSSE2(const Vec3& _vec)
		{
			return _mm_sqrt_ps( _vec.xyzw );
		}

		xst_i f32 DotSSE2(const Vec3& _vecLeft, const Vec3& _vecRight)
		{
			/*
			013673AA  mov         eax,dword ptr [ebx+0Ch]
			013673AD  movaps      xmm0,xmmword ptr [eax]
			013673B0  mov         ecx,dword ptr [ebx+8]
			013673B3  movaps      xmm1,xmmword ptr [ecx]
			013673B6  mulps       xmm1,xmm0
			013673B9  movaps      xmmword ptr [ebp-180h],xmm1
			013673C0  movaps      xmm0,xmmword ptr [ebp-180h]
			013673C7  movaps      xmmword ptr [ebp-20h],xmm0
			*/
			xst_m128 r1 = _mm_mul_ps( _vecLeft.xyzw , _vecRight.xyzw );
			/*
			013673CB  movaps      xmm0,xmmword ptr [ebp-20h]
			013673CF  movaps      xmm1,xmmword ptr [ebp-20h]
			013673D3  movhlps     xmm1,xmm0
			013673D6  movaps      xmmword ptr [ebp-160h],xmm1
			013673DD  movaps      xmm0,xmmword ptr [ebp-160h]
			013673E4  movaps      xmm1,xmmword ptr [ebp-20h]
			013673E8  addps       xmm1,xmm0
			013673EB  movaps      xmmword ptr [ebp-140h],xmm1
			013673F2  movaps      xmm0,xmmword ptr [ebp-140h]
			013673F9  movaps      xmmword ptr [ebp-20h],xmm0
			*/
			r1 = _mm_add_ps( r1, _mm_movehl_ps(r1, r1));
			/*
			013673FD  movaps      xmm0,xmmword ptr [ebp-20h]
			01367401  movaps      xmm1,xmmword ptr [ebp-20h]
			01367405  shufps      xmm1,xmm0,0E5h
			01367409  movaps      xmmword ptr [ebp-120h],xmm1
			01367410  movaps      xmm0,xmmword ptr [ebp-120h]
			01367417  movaps      xmm1,xmmword ptr [ebp-20h]
			0136741B  addps       xmm1,xmm0
			0136741E  movaps      xmmword ptr [ebp-100h],xmm1
			01367425  movaps      xmm0,xmmword ptr [ebp-100h]
			0136742C  movaps      xmmword ptr [ebp-20h],xmm0
			*/
			r1 = _mm_add_ps( r1, _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(3, 2, 1, 1)));
			//f32 fResult;
			//_mm_store_ss( &fResult, r1 ); // zapisz 1 float
			//return fResult;
			return r1.m128_f32[0];
		}

		xst_fi f32 LengthSSE2(const Vec3& _vecLeft)
		{
			xst_m128 r1 = _mm_mul_ps( _vecLeft.xyzw , _vecLeft.xyzw );
			r1 = _mm_add_ps( r1, _mm_movehl_ps(r1, r1));
			r1 = _mm_add_ps( r1, _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(3, 2, 1, 1)));
			r1 = _mm_sqrt_ps( r1 );
			//f32 fResult;
			//_mm_store_ss( &fResult, r1 ); // zapisz 1 float
			//return fResult;
			return r1.m128_f32[0];
		}

		xst_fi Vec3 NormalizeSSE2(const Vec3& _vecLeft)
		{
			//_vecLeft->xyzw / LengthSSE2(_vecLeft)

			//Calc vector length
			m128 l = _mm_mul_ps( _vecLeft.xyzw , _vecLeft.xyzw );
			l = _mm_add_ps( l, _mm_movehl_ps(l, l));
			l = _mm_add_ps( l, _mm_shuffle_ps(l, l, _MM_SHUFFLE(3, 2, 1, 1)));
			l = _mm_sqrt_ps( l );

			return _mm_div_ps( _vecLeft.xyzw, l );
		}

		//SSE2 (for class use)
		xst_fi Vec3 MulSSE2(const Vec3* _vecLeft, const Vec3& _vecRight)
		{
			return _mm_mul_ps(_vecLeft->xyzw, _vecRight.xyzw);
		}

		xst_fi Vec3 AddSSE2(const Vec3* _vecLeft, const Vec3& _vecRight)
		{
			return _mm_add_ps(_vecLeft->xyzw, _vecRight.xyzw);
		}

		xst_fi Vec3 SubSSE2(const Vec3* _vecLeft, const Vec3& _vecRight)
		{
			return _mm_sub_ps(_vecLeft->xyzw, _vecRight.xyzw);
		}

		xst_fi Vec3 DivSSE2(const Vec3* _vecLeft, const Vec3& _vecRight)
		{
			return _mm_div_ps(_vecLeft->xyzw, _vecRight.xyzw);
		}

		xst_fi Vec3 MulSSE2(const Vec3* _vecLeft, const f32& _fScalar)
		{
			return _mm_mul_ps( _vecLeft->xyzw, _mm_load1_ps(&_fScalar) );
		}

		xst_fi Vec3 AddSSE2(const Vec3* _vecLeft, const f32& _fScalar)
		{
			return _mm_add_ps( _vecLeft->xyzw, _mm_load1_ps(&_fScalar) );
		}

		xst_fi Vec3 SubSSE2(const Vec3* _vecLeft, const f32& _fScalar)
		{
			return _mm_sub_ps( _vecLeft->xyzw, _mm_load1_ps(&_fScalar) );
		}

		xst_fi Vec3 DivSSE2(const Vec3* _vecLeft, const f32& _fScalar)
		{
			return _mm_div_ps( _vecLeft->xyzw, _mm_load1_ps(&_fScalar) );
		}

		xst_fi Vec3 SqrtSSE2(const Vec3* _vec)
		{
			return _mm_sqrt_ps( _vec->xyzw );
		}

		xst_fi f32 DotSSE2(const Vec3* _vecLeft, const Vec3& _vecRight)
		{
			m128 r1 = _mm_mul_ps( _vecLeft->xyzw , _vecRight.xyzw );
			r1 = _mm_add_ps( r1, _mm_movehl_ps(r1, r1));
			r1 = _mm_add_ps( r1, _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(3, 2, 1, 1)));
			f32 fResult;
			_mm_store_ss( &fResult, r1 ); // zapisz 1 float
			return fResult;
		}

		xst_fi void AddEqSSE2(Vec3* _vecLeft, const Vec3& _vecRight)
		{
			_vecLeft->xyzw = _mm_add_ps( _vecLeft->xyzw, _vecRight.xyzw );
		}

		xst_fi void SubEqSSE2(Vec3* _vecLeft, const Vec3& _vecRight)
		{
			_vecLeft->xyzw = _mm_sub_ps( _vecLeft->xyzw, _vecRight.xyzw );
		}

		xst_fi void MulEqSSE2(Vec3* _vecLeft, const Vec3& _vecRight)
		{
			_vecLeft->xyzw = _mm_mul_ps( _vecLeft->xyzw, _vecRight.xyzw );
		}

		xst_fi void DivEqSSE2(Vec3* _vecLeft, const Vec3& _vecRight)
		{
			_vecLeft->xyzw = _mm_div_ps( _vecLeft->xyzw, _vecRight.xyzw );
		}

		xst_fi void AddEqSSE2(Vec3* _vecLeft, const f32& _fScalar)
		{
			_vecLeft->xyzw = _mm_add_ps( _vecLeft->xyzw, _mm_load1_ps( &_fScalar ) );
		}

		xst_fi void SubEqSSE2(Vec3* _vecLeft, const f32& _fScalar)
		{
			_vecLeft->xyzw = _mm_sub_ps( _vecLeft->xyzw, _mm_load1_ps( &_fScalar ) );
		}

		xst_fi void MulEqSSE2(Vec3* _vecLeft, const f32& _fScalar)
		{
			_vecLeft->xyzw = _mm_mul_ps( _vecLeft->xyzw, _mm_load1_ps( &_fScalar ) );
		}

		xst_fi void DivEqSSE2(Vec3* _vecLeft, const f32& _fScalar)
		{
			_vecLeft->xyzw = _mm_div_ps( _vecLeft->xyzw, _mm_load1_ps( &_fScalar ) );
		}

		xst_fi f32 LengthSSE2(const Vec3* _vecLeft)
		{
			//return sqrtf( _vecLeft->x * _vecLeft->x + _vecLeft->y * _vecLeft->y + _vecLeft->z * _vecLeft->z );
			m128 r1 = _mm_mul_ps( _vecLeft->xyzw , _vecLeft->xyzw );
			//r1 = _mm_add_ps( r1, _mm_movehl_ps(r1, r1) );
			//r1 = _mm_add_ps( r1, _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(3, 2, 1, 1)) );
			r1 = _mm_sqrt_ps( r1 );
			f32 fResult;
			_mm_store_ss( &fResult, r1 ); // zapisz 1 float
			return fResult;
		}

		xst_fi Vec3 NormalizeSSE2(const Vec3* _vecLeft)
		{
			//_vecLeft->xyzw / LengthSSE2(_vecLeft)

			//Calc vector length
			m128 l = _mm_mul_ps( _vecLeft->xyzw , _vecLeft->xyzw );
			l = _mm_add_ps( l, _mm_movehl_ps(l, l));
			l = _mm_add_ps( l, _mm_shuffle_ps(l, l, _MM_SHUFFLE(3, 2, 1, 1)));
			l = _mm_sqrt_ps( l );

			return _mm_div_ps( _vecLeft->xyzw, l );
		}

		//SSE4
		xst_fi f32	DotSSE4(const Vec3& _vecLeft, const Vec3& _vecRight)
		{
			return xst_m128( _mm_dp_ps( _vecLeft.xyzw, _vecRight.xyzw, 0x55 ) ).m128_f32[0];
		}

		//SSE4 (class)
		xst_fi f32	DotSSE4(const Vec3* _vecLeft, const Vec3& _vecRight)
		{
			return xst_m128( _mm_dp_ps( _vecLeft->xyzw, _vecRight.xyzw, 0x55 ) ).m128_f32[0];
		}

	}//Math


	class CMath
	{

		public:

			static void	EnableFullSSE2()
			{
				//Vector - Vector
				Math::Vec3Mul	= &Math::MulSSE2;
				Math::Vec3Add	= &Math::AddSSE2;
				Math::Vec3Sub	= &Math::SubSSE2;
				Math::Vec3Div	= &Math::DivSSE2;
				Math::Vec3Dot	= &Math::DotSSE2;

				//Vector - scalar
				Math::Vec3AddS = &Math::AddSSE2;
				Math::Vec3SubS = &Math::SubSSE2;
				Math::Vec3MulS = &Math::MulSSE2;
				Math::Vec3DivS = &Math::DivSSE2;
				Math::Vec3Sqrt = &Math::SqrtSSE2;

				//Vector ptr - Vector
				Math::Vec3cAdd	= &Math::AddSSE2;
				Math::Vec3cSub	= &Math::SubSSE2;
				Math::Vec3cMul	= &Math::MulSSE2;
				Math::Vec3cDiv	= &Math::DivSSE2;
				Math::Vec3cSqrt	= &Math::SqrtSSE2;
				Math::Vec3cDot	= &Math::DotSSE2;
				Math::Vec3AddEq	= &Math::AddEqSSE2;
				Math::Vec3SubEq	= &Math::SubEqSSE2;
				Math::Vec3MulEq	= &Math::MulEqSSE2;
				Math::Vec3DivEq	= &Math::DivEqSSE2;

				//Vector ptr - scalar
				Math::Vec3cAddS		= &Math::AddSSE2;
				Math::Vec3cSubS		= &Math::SubSSE2;
				Math::Vec3cMulS		= &Math::MulSSE2;
				Math::Vec3cDivS		= &Math::DivSSE2;
				Math::Vec3AddEqS	= &Math::AddEqSSE2;
				Math::Vec3SubEqS	= &Math::SubEqSSE2;
				Math::Vec3MulEqS	= &Math::MulEqSSE2;
				Math::Vec3DivEqS	= &Math::DivEqSSE2;

				//Vector
				Math::Vec3Length	= &Math::LengthSSE2;

				//Vector ptr
				Math::Vec3cLength	= &Math::LengthSSE2;

				//Math
				Math::Sqrt = &Math::SqrtSSE2;
			}

			static void	EnableOptimalSSE2()
			{
				//For vector3 only sqrtf is optimal with sse2
				//For basic operations compiler does more optimal assembler code

				//Vector - Vector
				Math::Vec3Mul = &Math::MulRef;
				Math::Vec3Add = &Math::AddRef;
				Math::Vec3Sub = &Math::SubRef;
				Math::Vec3Div = &Math::DivRef;
				Math::Vec3Dot = &Math::DotRef;

				//Vector - scalar
				Math::Vec3AddS = &Math::AddRef;
				Math::Vec3SubS = &Math::SubRef;
				Math::Vec3MulS = &Math::MulRef;
				Math::Vec3DivS = &Math::DivRef;
				Math::Vec3Sqrt = &Math::SqrtSSE2;

				//Vector ptr - Vector
				Math::Vec3cAdd	= &Math::AddRef;
				Math::Vec3cSub	= &Math::SubRef;
				Math::Vec3cMul	= &Math::MulRef;
				Math::Vec3cDiv	= &Math::DivRef;
				Math::Vec3cSqrt	= &Math::SqrtSSE2;
				Math::Vec3cDot	= &Math::DotRef;
				Math::Vec3AddEq	= &Math::AddEqRef;
				Math::Vec3SubEq	= &Math::SubEqRef;
				Math::Vec3MulEq	= &Math::MulEqRef;
				Math::Vec3DivEq	= &Math::DivEqRef;

				//Vector ptr - scalar
				Math::Vec3cAddS		= &Math::AddRef;
				Math::Vec3cSubS		= &Math::SubRef;
				Math::Vec3cMulS		= &Math::MulRef;
				Math::Vec3cDivS		= &Math::DivRef;
				Math::Vec3AddEqS	= &Math::AddEqRef;
				Math::Vec3SubEqS	= &Math::SubEqRef;
				Math::Vec3MulEqS	= &Math::MulEqRef;
				Math::Vec3DivEqS	= &Math::DivEqRef;

				//Vector
				Math::Vec3Length	= &Math::LengthSSE2;

				//Vector ptr
				Math::Vec3cLength	= &Math::LengthSSE2;

				//Math
				Math::Sqrt = &Math::SqrtSSE2;
			}
	};


}//XST

#endif
