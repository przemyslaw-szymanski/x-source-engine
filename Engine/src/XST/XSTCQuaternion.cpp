#include "XST/XSTCQuaternion.h"

namespace XST
{
	namespace Math
	{
		const CQuaternion CQuaternion::ZERO( 0.0f, 0.0f, 0.0f, 0.0f );

		CQuaternion operator* (f32 fScalar, const CQuaternion& rkQ)
		{
			return CQuaternion(fScalar*rkQ.x,fScalar*rkQ.y, fScalar*rkQ.z, fScalar*rkQ.w);
		}

		void CQuaternion::ToMatrix(Mtx4* pMtxOut) const
		{
			f32 fTx  = x+x;
			f32 fTy  = y+y;
			f32 fTz  = z+z;
			f32 fTwx = fTx*w;
			f32 fTwy = fTy*w;
			f32 fTwz = fTz*w;
			f32 fTxx = fTx*x;
			f32 fTxy = fTy*x;
			f32 fTxz = fTz*x;
			f32 fTyy = fTy*y;
			f32 fTyz = fTz*y;
			f32 fTzz = fTz*z;

			pMtxOut->m[0][0] = 1.0f-(fTyy+fTzz);
			pMtxOut->m[0][1] = fTxy-fTwz;
			pMtxOut->m[0][2] = fTxz+fTwy;
			pMtxOut->m[1][0] = fTxy+fTwz;
			pMtxOut->m[1][1] = 1.0f-(fTxx+fTzz);
			pMtxOut->m[1][2] = fTyz-fTwx;
			pMtxOut->m[2][0] = fTxz-fTwy;
			pMtxOut->m[2][1] = fTyz+fTwx;
			pMtxOut->m[2][2] = 1.0f-(fTxx+fTyy);
		}

		void CQuaternion::ToMatrix(Mtx3* pMtxOut) const
		{
			f32 fTx  = x+x;
			f32 fTy  = y+y;
			f32 fTz  = z+z;
			f32 fTwx = fTx*w;
			f32 fTwy = fTy*w;
			f32 fTwz = fTz*w;
			f32 fTxx = fTx*x;
			f32 fTxy = fTy*x;
			f32 fTxz = fTz*x;
			f32 fTyy = fTy*y;
			f32 fTyz = fTz*y;
			f32 fTzz = fTz*z;

			pMtxOut->m[0][0] = 1.0f-(fTyy+fTzz);
			pMtxOut->m[0][1] = fTxy-fTwz;
			pMtxOut->m[0][2] = fTxz+fTwy;
			pMtxOut->m[1][0] = fTxy+fTwz;
			pMtxOut->m[1][1] = 1.0f-(fTxx+fTzz);
			pMtxOut->m[1][2] = fTyz-fTwx;
			pMtxOut->m[2][0] = fTxz-fTwy;
			pMtxOut->m[2][1] = fTyz+fTwx;
			pMtxOut->m[2][2] = 1.0f-(fTxx+fTyy);
		}

		void CQuaternion::ToAngleAxis(f32* pfRadianOut, Vec3* pvecAxisOut) const
		{
			f32 fSqrLength = x*x+y*y+z*z;
			if ( fSqrLength > 0.0 )
			{
				*pfRadianOut = 2.0*Math::ACos(w);
				f32 fInvLength = Math::InvSqrt(fSqrLength);
				pvecAxisOut->x = x*fInvLength;
				pvecAxisOut->y = y*fInvLength;
				pvecAxisOut->z = z*fInvLength;
			}
			else
			{
				// angle is 0 (mod 2*pi), so any axis will do
				*pfRadianOut = /*Radian*/f32(0.0);
				pvecAxisOut->x = 1.0;
				pvecAxisOut->y = 0.0;
				pvecAxisOut->z = 0.0;
			}
		}

		void CQuaternion::ToAxes(Vec3* pvecXAxis, Vec3* pvecYAxis, Vec3* pvecZAxis) const
		{
			Mtx3 kRot;

			ToMatrix(&kRot);

			pvecXAxis->x = kRot[0][0];
			pvecXAxis->y = kRot[1][0];
			pvecXAxis->z = kRot[2][0];

			pvecYAxis->x = kRot[0][1];
			pvecYAxis->y = kRot[1][1];
			pvecYAxis->z = kRot[2][1];

			pvecZAxis->x = kRot[0][2];
			pvecZAxis->y = kRot[1][2];
			pvecZAxis->z = kRot[2][2];
		}

		CQuaternion CQuaternion::Inverse () const
		{
			f32 fNorm = w*w+x*x+y*y+z*z;
			if ( fNorm > 0.0 )
			{
				f32 fInvNorm = 1.0f/fNorm;
				return CQuaternion(w*fInvNorm,-x*fInvNorm,-y*fInvNorm,-z*fInvNorm);
			}
			else
			{
				// return an invalid result to flag the error
				return ZERO;
			}
		}

		CQuaternion CQuaternion::Exp () const
		{
			// If q = A*(x*i+y*j+z*k) where (x,y,z) is unit length, then
			// exp(q) = cos(A)+sin(A)*(x*i+y*j+z*k).  If sin(A) is near zero,
			// use exp(q) = cos(A)+A*(x*i+y*j+z*k) since A/sin(A) has limit 1.

			CRadian fAngle ( Math::Sqrt(x*x+y*y+z*z) );
			f32 fSin = Math::Sin(fAngle.fValue);

			CQuaternion kResult;
			kResult.w = Math::Cos(fAngle.fValue);

			if ( Math::Abs(fSin) >= Math::EPSILON )
			{
				f32 fCoeff = fSin/(fAngle.valueRadians());
				kResult.x = fCoeff*x;
				kResult.y = fCoeff*y;
				kResult.z = fCoeff*z;
			}
			else
			{
				kResult.x = x;
				kResult.y = y;
				kResult.z = z;
			}

			return kResult;
		}
		//-----------------------------------------------------------------------
		CQuaternion CQuaternion::Log () const
		{
			// If q = cos(A)+sin(A)*(x*i+y*j+z*k) where (x,y,z) is unit length, then
			// log(q) = A*(x*i+y*j+z*k).  If sin(A) is near zero, use log(q) =
			// sin(A)*(x*i+y*j+z*k) since sin(A)/A has limit 1.

			CQuaternion kResult;
			kResult.w = 0.0;

			if ( Math::Abs(w) < 1.0 )
			{
				CRadian fAngle ( Math::ACos(w) );
				f32 fSin = Math::Sin(fAngle);
				if ( Math::Abs(fSin) >= Math::EPSILON )
				{
					f32 fCoeff = fAngle.valueRadians()/fSin;
					kResult.x = fCoeff*x;
					kResult.y = fCoeff*y;
					kResult.z = fCoeff*z;
					return kResult;
				}
			}

			kResult.x = x;
			kResult.y = y;
			kResult.z = z;

			return kResult;
		}
		//-----------------------------------------------------------------------
		Vec3 CQuaternion::operator* (const Vec3& v) const
		{
			// nVidia SDK implementation
			Vec3 uv, uuv;
			Vec3 qvec(x, y, z);
			uv = qvec.Cross(v);
			uuv = qvec.Cross(uv);
			uv *= (2.0f * w);
			uuv *= 2.0f;

			return v + uv + uuv;

		}
		//-----------------------------------------------------------------------
		//bool CQuaternion::Equals(const CQuaternion& rhs, const CRadian& tolerance) const
		//{
		//	f32 fCos = Dot(rhs);
		//	/*Radian*/f32 angle = Math::ACos(fCos);

		//	return (Math::Abs(angle.valueRadians()) <= tolerance.valueRadians())
		//		|| Math::RealEqual(angle.valueRadians(), Math::PI, tolerance.valueRadians());


		//}
		//-----------------------------------------------------------------------
		CQuaternion CQuaternion::Slerp (f32 fT, const CQuaternion& rkP,
			const CQuaternion& rkQ, bool shortestPath)
		{
			f32 fCos = rkP.Dot(rkQ);
			CQuaternion rkT;

			// Do we need to invert rotation?
			if (fCos < 0.0f && shortestPath)
			{
				fCos = -fCos;
				rkT = -rkQ;
			}
			else
			{
				rkT = rkQ;
			}

			if (Math::Abs(fCos) < 1 - Math::EPSILON)
			{
				// Standard case (slerp)
				f32 fSin = Math::Sqrt(1 - Math::Sqr(fCos));
				CRadian fAngle = Math::ATan2(fSin, fCos);
				f32 fInvSin = 1.0f / fSin;
				f32 fCoeff0 = Math::Sin((1.0f - fT) * fAngle) * fInvSin;
				f32 fCoeff1 = Math::Sin(fT * fAngle) * fInvSin;
				return fCoeff0 * rkP + fCoeff1 * rkT;
			}
			else
			{
				// There are two situations:
				// 1. "rkP" and "rkQ" are very close (fCos ~= +1), so we can do a linear
				//    interpolation safely.
				// 2. "rkP" and "rkQ" are almost inverse of each other (fCos ~= -1), there
				//    are an infinite number of possibilities interpolation. but we haven't
				//    have method to fix this case, so just use linear interpolation here.
				CQuaternion t = (1.0f - fT) * rkP + fT * rkT;
				// taking the complement requires renormalisation
				t.Normalize();
				return t;
			}
		}
		//-----------------------------------------------------------------------
		CQuaternion CQuaternion::SlerpExtraSpins (f32 fT,
			const CQuaternion& rkP, const CQuaternion& rkQ, int iExtraSpins)
		{
			f32 fCos = rkP.Dot(rkQ);
			CRadian fAngle ( Math::ACos(fCos) );

			if ( Math::Abs(fAngle.valueRadians()) < Math::EPSILON )
				return rkP;

			f32 fSin = Math::Sin(fAngle);
			/*Radian*/f32 fPhase ( Math::PI*iExtraSpins*fT );
			f32 fInvSin = 1.0f/fSin;
			f32 fCoeff0 = Math::Sin((1.0f-fT)*fAngle - fPhase)*fInvSin;
			f32 fCoeff1 = Math::Sin(fT*fAngle + fPhase)*fInvSin;
			return fCoeff0*rkP + fCoeff1*rkQ;
		}
		
		//void CQuaternion::Intermediate (const CQuaternion& rkQ0,
		//	const CQuaternion& rkQ1, const CQuaternion& rkQ2,
		//	CQuaternion& rkA, CQuaternion& rkB)
		//{
		//	// assert:  q0, q1, q2 are unit quaternions

		//	CQuaternion kQ0inv = rkQ0.UnitInverse();
		//	CQuaternion kQ1inv = rkQ1.UnitInverse();
		//	CQuaternion rkP0 = kQ0inv*rkQ1;
		//	CQuaternion rkP1 = kQ1inv*rkQ2;
		//	CQuaternion kArg = 0.25*(rkP0.Log()-rkP1.Log());
		//	CQuaternion kMinusArg = -kArg;

		//	rkA = rkQ1*kArg.Exp();
		//	rkB = rkQ1*kMinusArg.Exp();
		//}
	
		CQuaternion CQuaternion::Squad (f32 fT,
			const CQuaternion& rkP, const CQuaternion& rkA,
			const CQuaternion& rkB, const CQuaternion& rkQ, bool shortestPath)
		{
			f32 fSlerpT = 2.0f*fT*(1.0f-fT);
			CQuaternion kSlerpP = Slerp(fT, rkP, rkQ, shortestPath);
			CQuaternion kSlerpQ = Slerp(fT, rkA, rkB);
			return Slerp(fSlerpT, kSlerpP ,kSlerpQ);
		}
		
		f32 CQuaternion::Normalize(void)
		{
			f32 len = Norm();
			f32 factor = 1.0f / Math::Sqrt(len);
			Set( *this * factor );
			return len;
		}
	
		/*Radian*/f32 CQuaternion::GetRoll(bool reprojectAxis) const
		{
			if (reprojectAxis)
			{
				// roll = atan2(localx.y, localx.x)
				// pick parts of xAxis() implementation that we need
				//			f32 fTx  = 2.0*x;
				f32 fTy  = 2.0f*y;
				f32 fTz  = 2.0f*z;
				f32 fTwz = fTz*w;
				f32 fTxy = fTy*x;
				f32 fTyy = fTy*y;
				f32 fTzz = fTz*z;

				// Vec3(1.0-(fTyy+fTzz), fTxy+fTwz, fTxz-fTwy);

				return CRadian(Math::ATan2(fTxy+fTwz, 1.0f-(fTyy+fTzz)));

			}
			else
			{
				return CRadian(Math::ATan2(2*(x*y + w*z), w*w + x*x - y*y - z*z));
			}
		}
		//-----------------------------------------------------------------------
		/*Radian*/f32 CQuaternion::GetPitch(bool reprojectAxis) const
		{
			if (reprojectAxis)
			{
				// pitch = atan2(localy.z, localy.y)
				// pick parts of yAxis() implementation that we need
				f32 fTx  = 2.0f*x;
				//			f32 fTy  = 2.0f*y;
				f32 fTz  = 2.0f*z;
				f32 fTwx = fTx*w;
				f32 fTxx = fTx*x;
				f32 fTyz = fTz*y;
				f32 fTzz = fTz*z;

				// Vec3(fTxy-fTwz, 1.0-(fTxx+fTzz), fTyz+fTwx);
				return CRadian(Math::ATan2(fTyz+fTwx, 1.0f-(fTxx+fTzz)));
			}
			else
			{
				// internal version
				return CRadian(Math::ATan2(2*(y*z + w*x), w*w - x*x - y*y + z*z));
			}
		}
		//-----------------------------------------------------------------------
		/*Radian*/f32 CQuaternion::GetYaw(bool reprojectAxis) const
		{
			if (reprojectAxis)
			{
				// yaw = atan2(localz.x, localz.z)
				// pick parts of zAxis() implementation that we need
				f32 fTx  = 2.0f*x;
				f32 fTy  = 2.0f*y;
				f32 fTz  = 2.0f*z;
				f32 fTwy = fTy*w;
				f32 fTxx = fTx*x;
				f32 fTxz = fTz*x;
				f32 fTyy = fTy*y;

				// Vec3(fTxz+fTwy, fTyz-fTwx, 1.0-(fTxx+fTyy));

				return CRadian(Math::ATan2(fTxz+fTwy, 1.0f-(fTxx+fTyy)));

			}
			else
			{
				// internal version
				return CRadian(Math::ASin(-2*(x*z - w*y)));
			}
		}
		//-----------------------------------------------------------------------
		/*CQuaternion CQuaternion::Nlerp(f32 fT, const CQuaternion& rkP,
			const CQuaternion& rkQ, bool shortestPath)
		{
			CQuaternion result;
			f32 fCos = rkP.Dot(rkQ);
			if (fCos < 0.0f && shortestPath)
			{
				result = rkP + fT * ((-rkQ) - rkP);
			}
			else
			{
				result = rkP + fT * (rkQ - rkP);
			}
			result.Normalize();
			return result;
		}*/

		void CQuaternion::Set(const Vec3& vecXAxis, const Vec3& vecYAxis, const Vec3& vecZAxis)
		{
			Mtx3 kRot;

			kRot[0][0] = vecXAxis.x;
			kRot[1][0] = vecXAxis.y;
			kRot[2][0] = vecXAxis.z;

			kRot[0][1] = vecYAxis.x;
			kRot[1][1] = vecYAxis.y;
			kRot[2][1] = vecYAxis.z;

			kRot[0][2] = vecZAxis.x;
			kRot[1][2] = vecZAxis.y;
			kRot[2][2] = vecZAxis.z;

			Set(kRot);
		}

		void CQuaternion::Set(const CRadian& fAngle, cf32& fX, cf32& fY, cf32& fZ)
		{
			CRadian fHalfAngle ( 0.5*fAngle );
			f32 fSin = Math::Sin(fHalfAngle);
			w = Math::Cos(fHalfAngle);
			x = fSin*fX;
			y = fSin*fY;
			z = fSin*fZ;
		}

		void CQuaternion::Set(const Mtx3& mtx)
		{
			f32 fTrace = mtx.m[ 0 ][ 0 ] + mtx.m[ 1 ][ 1 ] + mtx.m[ 2 ][ 2 ];
			f32 fRoot;

			if( fTrace > 0.0f )
			{
				fRoot = Math::Sqrt( fTrace + 1.0f );
				w = 0.5f * fRoot;
				fRoot = 0.5f / fRoot;

				x = ( mtx.m[ 2 ][ 1 ] - mtx.m[ 1 ][ 2 ] ) / fRoot;
				y = ( mtx.m[ 0 ][ 2 ] - mtx.m[ 2 ][ 0 ] ) / fRoot;
				z = ( mtx.m[ 1 ][ 0 ] - mtx.m[ 0 ][ 1 ] ) / fRoot;
			}
			else
			{
				static size_t s_iNext[3] = { 1, 2, 0 };
				size_t i = 0;
				if ( mtx.m[1][1] > mtx.m[0][0] )
					i = 1;
				if ( mtx.m[2][2] > mtx.m[i][i] )
					i = 2;
				size_t j = s_iNext[i];
				size_t k = s_iNext[j];

				fRoot = Math::Sqrt(mtx.m[i][i]-mtx.m[j][j]-mtx.m[k][k] + 1.0f);
				f32* apkQuat[3] = { &x, &y, &z };
				*apkQuat[i] = 0.5f*fRoot;
				fRoot = 0.5f/fRoot;
				w = (mtx.m[k][j]-mtx.m[j][k])*fRoot;
				*apkQuat[j] = (mtx.m[j][i]+mtx.m[i][j])*fRoot;
				*apkQuat[k] = (mtx.m[k][i]+mtx.m[i][k])*fRoot;
			}
		}

		void CQuaternion::Set(const Mtx4& mtx)
		{
			f32 fTrace = mtx.m[ 0 ][ 0 ] + mtx.m[ 1 ][ 1 ] + mtx.m[ 2 ][ 2 ];
			f32 fRoot;

			if( fTrace > 0.0f )
			{
				fRoot = Math::Sqrt( fTrace + 1.0f );
				w = 0.5f * fRoot;
				fRoot = 0.5f / fRoot;

				x = ( mtx.m[ 2 ][ 1 ] - mtx.m[ 1 ][ 2 ] ) / fRoot;
				y = ( mtx.m[ 0 ][ 2 ] - mtx.m[ 2 ][ 0 ] ) / fRoot;
				z = ( mtx.m[ 1 ][ 0 ] - mtx.m[ 0 ][ 1 ] ) / fRoot;
			}
			else
			{
				static size_t s_iNext[3] = { 1, 2, 0 };
				size_t i = 0;
				if ( mtx.m[1][1] > mtx.m[0][0] )
					i = 1;
				if ( mtx.m[2][2] > mtx.m[i][i] )
					i = 2;
				size_t j = s_iNext[i];
				size_t k = s_iNext[j];

				fRoot = Math::Sqrt(mtx.m[i][i]-mtx.m[j][j]-mtx.m[k][k] + 1.0f);
				f32* apkQuat[3] = { &x, &y, &z };
				*apkQuat[i] = 0.5f*fRoot;
				fRoot = 0.5f/fRoot;
				w = (mtx.m[k][j]-mtx.m[j][k])*fRoot;
				*apkQuat[j] = (mtx.m[j][i]+mtx.m[i][j])*fRoot;
				*apkQuat[k] = (mtx.m[k][i]+mtx.m[i][k])*fRoot;
			}
		}
	}//math
}//xst