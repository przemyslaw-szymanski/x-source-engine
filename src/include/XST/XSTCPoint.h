#ifndef XST_CPOINT_H
#define XST_CPOINT_H

namespace XST
{
	namespace Math
	{
		class CPoint
		{
			public:

				i32 x, y;

				static const CPoint ZERO;
				static const CPoint	X;
				static const CPoint Y;
				static const CPoint NEGATIVE_X;
				static const CPoint NEGATIVE_Y;

			public:

				xst_fi CPoint() {}
				xst_fi CPoint(const CPoint& Point) : x( Point.x ), y( Point.y ) {}
				xst_fi CPoint(const i32& _fValue) : x(_fValue), y(_fValue) {}
				xst_fi CPoint(const i32& _fX, const i32& _fY) : x(_fX), y(_fY) {}
				xst_fi CPoint(const i32 _afValues[2]) : x( _afValues[ 0 ] ), y( _afValues[ 1 ] ) {}

				~CPoint() {}

				xst_fi const bool operator==(const CPoint& Right) const 
				{
					return x == Right.x && y == Right.y;
				}

				xst_fi const bool operator>(const CPoint& Right) const 
				{
					return x > Right.x && y > Right.y;
				}

				xst_fi const  bool operator>=(const CPoint& Right) const 
				{
					return x >= Right.x && y >= Right.y;
				}

				xst_fi const bool operator<(const CPoint& Right) const 
				{
					return x < Right.x && y < Right.y;
				}

				xst_fi const bool operator<=(const CPoint& Right) const 
				{
					return x <= Right.x && y <= Right.y;
				}

				xst_fi const bool operator!=(const CPoint& Right) const 
				{
					return x != Right.x && y != Right.y;
				}

				xst_fi CPoint operator+(const CPoint& Right) const 
				{
					return CPoint( x + Right.x, y + Right.y );
				}

				xst_fi void operator+=(const CPoint& Right) 
				{
					x += Right.x; y += Right.y;;
				}

				xst_fi CPoint operator-(const CPoint& Right) const 
				{
					return CPoint( x - Right.x, y - Right.y);
				}

				xst_fi CPoint operator-() const 
				{
					return CPoint( -x, -y );
				}

				xst_fi void operator-=(const CPoint& Right)
				{
					x -= Right.x; y -= Right.y;
				}

				xst_fi CPoint operator*(const CPoint& Right) const 
				{
					return CPoint( x * Right.x, y * Right.y );
				}

				xst_fi void operator*=(const CPoint& Right) 
				{
					x *= Right.x; y *= Right.y;
				}

				xst_fi CPoint operator/(const CPoint& Right) const 
				{
					return CPoint( x / Right.x, y / Right.y );
				}

				xst_fi void operator/=(const CPoint& Right) 
				{
					x /= Right.x; y /= Right.y;
				}

				xst_fi const bool Eq(const CPoint& Right) const 
				{
					return x == Right.x && y == Right.y;
				}

				xst_fi const bool Gt(const CPoint& Right) const 
				{
					return x > Right.x && y > Right.y;
				}

				xst_fi bool Ge(const CPoint& Right)
				{
					return x >= Right.x && y >= Right.y;
				}

				xst_fi const bool Lt(const CPoint& Right) const
				{
					return x < Right.x && y < Right.y;
				}

				xst_fi const bool Le(const CPoint& Right) const
				{
					return x <= Right.x && y <= Right.y;
				}

				xst_fi const bool Ne(const CPoint& Right) const
				{
					return x != Right.x && y != Right.y;
				}

				xst_fi const CPoint& operator=(const CPoint& Right)
				{
					x = Right.x; y = Right.y;
					return (*this);
				}

				xst_fi const CPoint& Set(const CPoint& Right)
				{
					x = Right.x; y = Right.y;
					return (*this);
				}

				xst_fi const CPoint& Set(const i32& _fX, const i32& _fY)
				{
					x = _fX; y = _fY;
					return (*this);
				}

				xst_fi const i32* ToArray() const
				{
					static i32 afArray[] = { x, y };
					return afArray;
				}

				xst_fi const u32 GetArraySize() const
				{
					return 2;
				}

				xst_fi CPoint Add(const CPoint& Right) const
				{
					return CPoint( x + Right.x, y + Right.y );
				}

				xst_fi void AddEq(const CPoint& Right)
				{
					x += Right.x; y += Right.y;
				}

				xst_fi CPoint Sub(const CPoint& Right) const
				{
					return CPoint( x - Right.x, y - Right.y);
				}

				xst_fi void SubEq(const CPoint& Right)
				{
					x -= Right.x; y -= Right.y;
				}

				xst_fi CPoint Mul(const CPoint& Right) const
				{
					return CPoint( x * Right.x, y * Right.y );
				}

				xst_fi void  MulEq(const CPoint& Right)
				{
					x *= Right.x; y *= Right.y;
				}

				xst_fi CPoint Div(const CPoint& Right) const
				{
					return CPoint( x / Right.x, y / Right.y );
				}

				xst_fi void DivEq(const CPoint& Right)
				{
					x /= Right.x; y /= Right.y;
				}

				xst_fi void AddEq(const i32& iValue)
				{
					x += iValue; y += iValue;
				}

				xst_fi void SubEq(const i32& iValue)
				{
					x -= iValue; y -= iValue;
				}

				xst_fi void MulEq(const i32& iValue)
				{
					x *= iValue; y *= iValue;
				}

				xst_fi void DivEq(const i32& iValue)
				{
					x /= iValue; y /= iValue;
				}

				xst_fi f32 Length() const
				{
					return Math::Sqrt( (f32)(x * x + y * y) );
				}

				xst_fi i32 SquaredLength() const
				{
					return x * x + y * y ;
				}

				xst_fi f32 Distance(const CPoint& Right) const
				{
					return ( *this - Right ).Length();
				}

				xst_fi i32 SquareDistance(const CPoint& Right) const
				{
					return ( *this - Right ).SquaredLength();
				}

				xst_fi CPoint MidPoint(const CPoint& VecRight) const
				{
					return CPoint(	(i32)( ( x + VecRight.x ) * 0.5f ), 
									(i32)( ( y + VecRight.y ) * 0.5f ) );
				}
		};

	}//math
}//xst

#endif