#ifndef XST_CQUATERNION_H
#define XST_CQUATERNION_H

#include "XSTMath.h"
#include "XSTCMatrix4.h"

namespace XST
{
	namespace Math
	{
		class XST_API XST_ALIGN( 16 ) CQuaternion;
		typedef XST_API XST_ALIGN( 16 ) CQuaternion CQuaternion;

		class XST_API XST_ALIGN( 16 ) CQuaternion
		{			
		public:

			union
			{
				struct	{ f32 x, y, z, w; };
				m128	v;
				f32		a[ 4 ];
			};

			static const CQuaternion	ZERO;
			static const CQuaternion	IDENTITY;
			static const CQuaternion	UNIT;
			static const CQuaternion	NEGATIVE_UNIT;

		public:

			xst_fi				CQuaternion(const CQuaternion& Quat) : x( Quat.x ), y( Quat.y ), z( Quat.z ), w( Quat.w ) {}
			xst_fi				CQuaternion(f32 fX = 0.0f, f32 fY = 0.0f, f32 fZ = 0.0f, f32 fW = 0.0f) : x( fX ), y( fY ), z( fZ ), w( fW ) {}
			xst_fi				CQuaternion(const Vec4& vec) : v( vec.v ) {}
			xst_fi				CQuaternion(const Vec3& vecXAxis, const Vec3& vecYAxis, const Vec3& vecZAxis)
								{ this->Set( vecXAxis, vecYAxis, vecZAxis ); }
			xst_fi				CQuaternion(const Mtx4& mtx) 
								{ this->Set( mtx ); }
			xst_fi				CQuaternion(const Mtx3& mtx)
								{ this->Set( mtx ); }

			xst_fi				operator m128() const
								{ return v;}

			xst_fi	f32			operator[](cu32& uiID) 
								{ return this->a[ uiID ]; }

			xst_fi	cf32		operator[](cu32& uiID) const
								{ return this->a[ uiID ]; }

			xst_fi				operator Mtx3() const
								{
									Mtx3 mtx;
									ToMatrix( &mtx );
									return mtx;
								}

			xst_fi				operator Mtx4() const
								{
									Mtx4 mtx;
									ToMatrix( &mtx );
									return mtx;
								}

			xst_fi	CQuaternion	operator+(const CQuaternion& quatRight) const
								{ return CQuaternion(w+quatRight.w,x+quatRight.x,y+quatRight.y,z+quatRight.z); }

			xst_fi CQuaternion	operator-(const CQuaternion& quatRight) const
								{ return CQuaternion(w-quatRight.w,x-quatRight.x,y-quatRight.y,z-quatRight.z); }

			xst_fi CQuaternion	operator*(const CQuaternion& quatRight) const
			{
				/*return CQuaternion
					(
					w * quatRight.x + x * quatRight.w + y * quatRight.z - z * quatRight.y,
					w * quatRight.y + y * quatRight.w + z * quatRight.x - x * quatRight.z,
					w * quatRight.z + z * quatRight.w + x * quatRight.y - y * quatRight.x,
					w * quatRight.w - x * quatRight.x - y * quatRight.y - z * quatRight.z
					);*/
				return MulRet( *this, quatRight );
			}

			xst_fi	CQuaternion	operator*(f32 fScalar) const
								{ return CQuaternion(fScalar*w,fScalar*x,fScalar*y,fScalar*z); }

			XST_API friend CQuaternion			operator* (f32 fScalar, const CQuaternion& rkQ);

			xst_fi	CQuaternion operator-() const
								{ return CQuaternion( -x, -y, -z, -w ); }

			xst_fi bool			operator== (const CQuaternion& rhs) const
								{ return (rhs.x == x) && (rhs.y == y) && (rhs.z == z) && (rhs.w == w); }

			xst_fi bool			operator!= (const CQuaternion& rhs) const
								{ return !operator==(rhs); }

			xst_fi CQuaternion& operator= (const CQuaternion& rkQ)
								{ w = rkQ.w; x = rkQ.x; y = rkQ.y; z = rkQ.z; return *this; }

			xst_fi	f32			Dot (const CQuaternion& quatRight) const
								{ return w*quatRight.w+x*quatRight.x+y*quatRight.y+z*quatRight.z; }

			xst_fi f32			Norm() const
								{ return w*w+x*x+y*y+z*z; }

			f32					Normalize();

			CQuaternion	Inverse () const;

			xst_fi	CQuaternion	UnitInverse() const
								{ return CQuaternion( -x, -y, -z, -w ); }

			CQuaternion	Exp () const;

			CQuaternion	Log () const;

			Vec3				operator* (const Vec3& v) const;

			CQuaternion			Slerp (f32 fT, const CQuaternion& rkP, const CQuaternion& rkQ, bool shortestPath = false);

			CQuaternion			SlerpExtraSpins (f32 fT, const CQuaternion& rkP, const CQuaternion& rkQ, int iExtraSpins);

			CQuaternion			Squad (f32 fT, const CQuaternion& rkP, const CQuaternion& rkA, const CQuaternion& rkB, const CQuaternion& rkQ, bool shortestPath);

					f32			GetRoll(bool reprojectAxis) const;

					f32			GetPitch(bool reprojectAxis) const;

					f32			GetYaw(bool reprojectAxis) const;

					void		Set(const Mtx3& mtx);
					void		Set(const Mtx4& mtx);
			xst_fi	void		Set(const Vec4& vecAxisRadian)
								{ Set( CRadian( vecAxisRadian.w ), vecAxisRadian.x, vecAxisRadian.y, vecAxisRadian.z ); }
					void		Set(const Vec3& vecXAxis, const Vec3& vecYAxis, const Vec3& vecZAxis);
			xst_fi	void		Set(const CRadian& fRadian, const Vec3& vecAxis)
								{ Set( fRadian, vecAxis.x, vecAxis.y, vecAxis.z ); }
					void		Set(const CRadian& fAngle, cf32& fXAxis, cf32& fYAxis, cf32& fZAxis);
			xst_fi	void		Set(const CQuaternion& quat)
								{ x = quat.x; y = quat.y; z = quat.z; w = quat.w; }	

					void		ToMatrix(Mtx3* pMtxOut) const;
					void		ToMatrix(Mtx4* pMtxOut) const;
					void		ToAngleAxis(f32* pfRadianOut, Vec3* pvecAxisOut) const;
					void		ToAxes(Vec3* pvecXAxis, Vec3* pvecYAxis, Vec3* pVecZAxis) const;
					void		FromAngleAxis(cf32& fRadianAngle, const Vec3& vecAxis);

			xst_fi	void		Conjugate();

			xst_fi	static void	Conjugate(CQuaternion* pquatOut);
			xst_fi	static void	Conjugate(CQuaternion* pquatOut, const CQuaternion& quatQuaternion);
			xst_fi	static
					CQuaternion	ConjugateRet(const CQuaternion& quatQuaternion);

			xst_fi	static void	Mul(CQuaternion* pquatOut, const CQuaternion& quatLeft, const CQuaternion& quatRight);
			xst_fi	static
					CQuaternion	MulRet(const CQuaternion& quatLeft, const CQuaternion& quatRight);
		};

		#include "Impl/XSTCQuaternion.impl"

	}//math

}//xst

#endif //XST_CQUATERNION_H