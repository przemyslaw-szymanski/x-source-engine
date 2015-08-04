#ifndef XST_CPOINT_H
#define XST_CPOINT_H

namespace XST
{
    namespace Math
    {
        template<typename _T_>
        class XST_API TCPoint
        {
            public:

                union
                {
                    struct
                    {
                        _T_ x, y;
                    };
                    _T_ a[ 2 ];
                };

                static const TCPoint    ZERO;
                static const TCPoint    X;
                static const TCPoint    Y;
                static const TCPoint    NEGATIVE_X;
                static const TCPoint    NEGATIVE_Y;

            public:

                xst_fi TCPoint() {}
                xst_fi TCPoint(const TCPoint& Point) : x( Point.x ), y( Point.y ) {}
                xst_fi explicit TCPoint(const _T_& tValue) : x(tValue), y(tValue) {}
                xst_fi TCPoint(const _T_& tX, const _T_& tY) : x(tX), y(tY) {}
                xst_fi explicit TCPoint(const _T_ aValues[2]) : x( aValues[ 0 ] ), y( aValues[ 1 ] ) {}
                template<typename _A_>
                xst_fi TCPoint(const _A_& tX, const _A_& tY) : x( tX ), y( tY ) {}
                template<typename _A_>
                xst_fi explicit TCPoint(const _A_& tValue) : x( tValue ), y( tValue ) {}

                ~TCPoint() {}

                xst_fi const bool operator==(const TCPoint& Right) const 
                {
                    return x == Right.x && y == Right.y;
                }

                xst_fi const bool operator>(const TCPoint& Right) const 
                {
                    return x > Right.x && y > Right.y;
                }

                xst_fi const  bool operator>=(const TCPoint& Right) const 
                {
                    return x >= Right.x && y >= Right.y;
                }

                xst_fi const bool operator<(const TCPoint& Right) const 
                {
                    return x < Right.x && y < Right.y;
                }

                xst_fi const bool operator<=(const TCPoint& Right) const 
                {
                    return x <= Right.x && y <= Right.y;
                }

                xst_fi const bool operator!=(const TCPoint& Right) const 
                {
                    return x != Right.x && y != Right.y;
                }

                xst_fi const bool operator==(const _T_& tValue) const 
                {
                    return x == tValue && y == tValue;
                }

                xst_fi const bool operator>(const _T_& tValue) const 
                {
                    return x > tValue && y > tValue;
                }

                xst_fi const  bool operator>=(const _T_& tValue) const 
                {
                    return x >= tValue && y >= tValue;
                }

                xst_fi const bool operator<(const _T_& tValue) const 
                {
                    return x < tValue && y < tValue;
                }

                xst_fi const bool operator<=(const _T_& tValue) const 
                {
                    return x <= tValue && y <= tValue;
                }

                xst_fi const bool operator!=(const _T_& tValue) const 
                {
                    return x != tValue && y != tValue;
                }

                xst_fi TCPoint operator-() const 
                {
                    return TCPoint( -x, -y );
                }

                xst_fi TCPoint operator+(const TCPoint& Right) const 
                {
                    return TCPoint( x + Right.x, y + Right.y );
                }

                xst_fi void operator+=(const TCPoint& Right) 
                {
                    x += Right.x; y += Right.y;;
                }

                xst_fi TCPoint operator-(const TCPoint& Right) const 
                {
                    return TCPoint( x - Right.x, y - Right.y);
                }

                xst_fi void operator-=(const TCPoint& Right)
                {
                    x -= Right.x; y -= Right.y;
                }

                xst_fi TCPoint operator*(const TCPoint& Right) const 
                {
                    return TCPoint( x * Right.x, y * Right.y );
                }

                xst_fi void operator*=(const TCPoint& Right) 
                {
                    x *= Right.x; y *= Right.y;
                }

                xst_fi TCPoint operator/(const TCPoint& Right) const 
                {
                    return TCPoint( x / Right.x, y / Right.y );
                }

                xst_fi void operator/=(const TCPoint& Right) 
                {
                    x /= Right.x; y /= Right.y;
                }

                xst_fi TCPoint operator+(const _T_& tValue) const 
                {
                    return TCPoint( x + Right.x, y + Right.y );
                }

                xst_fi void operator+=(const _T_& tValue) 
                {
                    x += tValue; y += tValue;
                }

                xst_fi TCPoint operator-(const _T_& tValue) const 
                {
                    return TCPoint( x - tValue, y - tValue);
                }

                xst_fi void operator-=(const _T_& tValue)
                {
                    x -= tValue; y -= tValue;
                }

                xst_fi TCPoint operator*(const _T_& tValue) const 
                {
                    return TCPoint( x * tValue, y * tValue );
                }

                xst_fi void operator*=(const _T_& tValue) 
                {
                    x *= tValue; y *= tValue;
                }

                xst_fi TCPoint operator/(const _T_& tValue) const 
                {
                    return TCPoint( x / tValue, y / tValue );
                }

                xst_fi void operator/=(const _T_& tValue) 
                {
                    x /= tValue; y /= tValue;
                }

                xst_fi const bool Eq(const TCPoint& Right) const 
                {
                    return x == Right.x && y == Right.y;
                }

                xst_fi const bool Gt(const TCPoint& Right) const 
                {
                    return x > Right.x && y > Right.y;
                }

                xst_fi bool Ge(const TCPoint& Right)
                {
                    return x >= Right.x && y >= Right.y;
                }

                xst_fi const bool Lt(const TCPoint& Right) const
                {
                    return x < Right.x && y < Right.y;
                }

                xst_fi const bool Le(const TCPoint& Right) const
                {
                    return x <= Right.x && y <= Right.y;
                }

                xst_fi const bool Ne(const TCPoint& Right) const
                {
                    return x != Right.x && y != Right.y;
                }

                xst_fi const TCPoint& operator=(const TCPoint& Right)
                {
                    x = Right.x; y = Right.y;
                    return (*this);
                }

                xst_fi const TCPoint& Set(const TCPoint& Right)
                {
                    x = Right.x; y = Right.y;
                    return (*this);
                }

                xst_fi const TCPoint& Set(const _T_& tX, const _T_& tY)
                {
                    x = tX; y = tY;
                    return (*this);
                }

                xst_fi TCPoint Add(const TCPoint& Right) const
                {
                    return TCPoint( x + Right.x, y + Right.y );
                }

                xst_fi void AddEq(const TCPoint& Right)
                {
                    x += Right.x; y += Right.y;
                }

                xst_fi TCPoint Sub(const TCPoint& Right) const
                {
                    return TCPoint( x - Right.x, y - Right.y);
                }

                xst_fi void SubEq(const TCPoint& Right)
                {
                    x -= Right.x; y -= Right.y;
                }

                xst_fi TCPoint Mul(const TCPoint& Right) const
                {
                    return TCPoint( x * Right.x, y * Right.y );
                }

                xst_fi void  MulEq(const TCPoint& Right)
                {
                    x *= Right.x; y *= Right.y;
                }

                xst_fi TCPoint Div(const TCPoint& Right) const
                {
                    return TCPoint( x / Right.x, y / Right.y );
                }

                xst_fi void DivEq(const TCPoint& Right)
                {
                    x /= Right.x; y /= Right.y;
                }

                xst_fi void AddEq(const _T_& tValue)
                {
                    x += iValue; y += tValue;
                }

                xst_fi void SubEq(const _T_& tValue)
                {
                    x -= tValue; y -= tValue;
                }

                xst_fi void MulEq(const _T_& tValue)
                {
                    x *= tValue; y *= tValue;
                }

                xst_fi void DivEq(const _T_& tValue)
                {
                    x /= tValue; y /= tValue;
                }

                xst_fi f32 Length() const
                {
                    return Math::Sqrt( (f32)(x * x + y * y) );
                }

                xst_fi _T_ SquaredLength() const
                {
                    return x * x + y * y ;
                }

                xst_fi f32 Distance(const TCPoint& Right) const
                {
                    return ( *this - Right ).Length();
                }

                xst_fi _T_ SquareDistance(const TCPoint& Right) const
                {
                    return ( *this - Right ).SquaredLength();
                }

                xst_fi TCPoint MidPoint(const TCPoint& VecRight) const
                {
                    return TCPoint(	(_T_)( ( x + VecRight.x ) * 0.5f ), 
                                    (_T_)( ( y + VecRight.y ) * 0.5f ) );
                }

                xst_fi _T_ ElementAdd() const
                {
                    return x + y;
                }

                xst_fi _T_ ElementMul() const
                {
                    return x * y;
                }

                xst_fi _T_ ElementSub() const
                {
                    return x - y;
                }

                xst_fi f32 ElementDiv() const
                {
                    return (f32)x / y;
                }
        };

        typedef TCPoint< i32 >  CPoint;
        typedef TCPoint< i32 >  PointI32;
        typedef PointI32        Point;
        typedef TCPoint< i16 >  PointI16;
        typedef TCPoint< f32 >  PointF32;
        typedef TCPoint< u32 >  PointU32;
        typedef TCPoint< u16 >  PointU16;
        typedef TCPoint< l32 >  PointL32;
        typedef TCPoint< ul32 > PointUL32;
        typedef TCPoint< i64 >  PointI64;
        typedef TCPoint< u64 >  PointU64;
        typedef TCPoint< f64 >  PointF64;

        XST_API class Point;
        XST_API class PointI16;
        XST_API class PointI32;
        XST_API class PointU16;
        XST_API class PointU32;
        XST_API class PointF32;
        XST_API class PointF64;
        XST_API class PointL32;
        XST_API class PointUL32;
        XST_API class PointI64;
        XST_API class PointU64;

        /*template<typename _T_> const TCPoint< _T_ > TCPoint< _T_ >::ZERO( 0, 0 );
        template<typename _T_> const TCPoint< _T_ > TCPoint< _T_ >::X( 1, 0 );
        template<typename _T_> const TCPoint< _T_ > TCPoint< _T_ >::Y( 0, 1 );
        template<typename _T_> const TCPoint< _T_ > TCPoint< _T_ >::NEGATIVE_X( -1, 0 );
        template<typename _T_> const TCPoint< _T_ > TCPoint< _T_ >::NEGATIVE_Y( 0, -1 );*/

    }//math
}//xst

#endif