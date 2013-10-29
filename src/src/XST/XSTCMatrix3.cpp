#include "XST/XSTCMatrix3.h"

namespace XST
{
	namespace Math
	{
		void CMatrix3::Rotate(cf32& fAngle, const Vec3& vecAxis)
		{
			cf32 fSinAngle = sinf(fAngle);
			cf32 fCosAngle = cosf(fAngle);

			//#if defined (XST_SSE2)
			//	Vec4               N0, N1;
			//	Vec4               V0, V1, V2;
			//	Vec4               R0, R1, R2;
			//	Vec4               C0, C1, C2;
			//	Vec4               M;
			//	Vec4 NormalAxis( vecAxis );

			//	C2.v = _mm_set_ps1(1.0f - fCosAngle);
			//	C1.v = _mm_set_ps1(fCosAngle);
			//	C0.v = _mm_set_ps1(fSinAngle);

			//	N0.v = _mm_shuffle_ps(NormalAxis.v,NormalAxis.v,_MM_SHUFFLE(3,0,2,1));
			////    N0 = XMVectorPermute(NormalAxis, NormalAxis, SwizzleYZXW);
			//	N1.v = _mm_shuffle_ps(NormalAxis.v,NormalAxis.v,_MM_SHUFFLE(3,1,0,2));
			////    N1 = XMVectorPermute(NormalAxis, NormalAxis, SwizzleZXYW);

			//	V0.v = _mm_mul_ps(C2.v, N0.v);
			//	V0.v = _mm_mul_ps(V0.v, N1.v);

			//	R0.v = _mm_mul_ps(C2.v, NormalAxis.v);
			//	R0.v = _mm_mul_ps(R0.v, NormalAxis.v);
			//	R0.v = _mm_add_ps(R0.v, C1.v);

			//	R1.v = _mm_mul_ps(C0.v, NormalAxis.v);
			//	R1.v = _mm_add_ps(R1.v, V0.v);
			//	R2.v = _mm_mul_ps(C0.v, NormalAxis.v);
			//	R2.v = _mm_sub_ps(V0.v,R2.v);

			//	V0.v = _mm_and_ps(R0.v,g_XMMask3);
			////    V0 = XMVectorSelect(A, R0, g_XMSelect1110);
			//	V1.v = _mm_shuffle_ps(R1.v,R2.v,_MM_SHUFFLE(2,1,2,0));
			//	V1.v = _mm_shuffle_ps(V1.v,V1.v,_MM_SHUFFLE(0,3,2,1));
			////    V1 = XMVectorPermute(R1, R2, Permute0Z1Y1Z0X);
			//	V2.v = _mm_shuffle_ps(R1.v,R2.v,_MM_SHUFFLE(0,0,1,1));
			//	V2.v = _mm_shuffle_ps(V2.v,V2.v,_MM_SHUFFLE(2,0,2,0));
			////    V2 = XMVectorPermute(R1, R2, Permute0Y1X0Y1X);

			//	R2.v = _mm_shuffle_ps(V0.v,V1.v,_MM_SHUFFLE(1,0,3,0));
			//	R2.v = _mm_shuffle_ps(R2.v,R2.v,_MM_SHUFFLE(1,3,2,0));
			//	M.r[0] = R2;
			////    M.r[0] = XMVectorPermute(V0, V1, Permute0X1X1Y0W);
			//	R2.v = _mm_shuffle_ps(V0.v,V1.v,_MM_SHUFFLE(3,2,3,1));
			//	R2.v = _mm_shuffle_ps(R2.v,R2.v,_MM_SHUFFLE(1,3,0,2));
			//	M.r[1] = R2;
			////    M.r[1] = XMVectorPermute(V0, V1, Permute1Z0Y1W0W);
			//	V2.v = _mm_shuffle_ps(V2.v,V0.v,_MM_SHUFFLE(3,2,1,0));
			////    R2 = _mm_shuffle_ps(R2,R2,_MM_SHUFFLE(3,2,1,0));
			//	M.r[2] = V2.v;
			////    M.r[2] = XMVectorPermute(V0, V2, Permute1X1Y0Z0W);
			//	M.r[3] = g_XMIdentityR3;
			//#else

			//#endif
		}

		void CMatrix3::Mul(const CMatrix3& mtxLeft, const CMatrix3& mtxRight, CMatrix3* pMtxOut)
		{
			f32 fSum;

			for(u32 i = 0; i < 4; ++i)
			{
				for(u32 j = 0; j < 4; ++j)
				{
					fSum = 0.0f;
					for(u32 k = 0; k < 4; ++k)
					{
						fSum += mtxLeft.m[ i ][ k ] * mtxRight.m[ k ][ j ];
					}

					pMtxOut->m[ i ][ j ] = fSum;
				}
			}
		}

		void CMatrix3::Mul(const CMatrix3& mtxRight)
		{
			Mul( *this, mtxRight, this );
		//	CMatrix3 mResult;
		//	// Cache the invariants in registers
		//	float x = M1.m[0][0];
		//	float y = M1.m[0][1];
		//	float z = M1.m[0][2];
		//	float w = M1.m[0][3];
		//	// Perform the operation on the first row
		//	mResult.m[0][0] = (M2.m[0][0]*x)+(M2.m[1][0]*y)+(M2.m[2][0]*z)+(M2.m[3][0]*w);
		//	mResult.m[0][1] = (M2.m[0][1]*x)+(M2.m[1][1]*y)+(M2.m[2][1]*z)+(M2.m[3][1]*w);
		//	mResult.m[0][2] = (M2.m[0][2]*x)+(M2.m[1][2]*y)+(M2.m[2][2]*z)+(M2.m[3][2]*w);
		//	mResult.m[0][3] = (M2.m[0][3]*x)+(M2.m[1][3]*y)+(M2.m[2][3]*z)+(M2.m[3][3]*w);
		//	// Repeat for all the other rows
		//	x = M1.m[1][0];
		//	y = M1.m[1][1];
		//	z = M1.m[1][2];
		//	w = M1.m[1][3];
		//	mResult.m[1][0] = (M2.m[0][0]*x)+(M2.m[1][0]*y)+(M2.m[2][0]*z)+(M2.m[3][0]*w);
		//	mResult.m[1][1] = (M2.m[0][1]*x)+(M2.m[1][1]*y)+(M2.m[2][1]*z)+(M2.m[3][1]*w);
		//	mResult.m[1][2] = (M2.m[0][2]*x)+(M2.m[1][2]*y)+(M2.m[2][2]*z)+(M2.m[3][2]*w);
		//	mResult.m[1][3] = (M2.m[0][3]*x)+(M2.m[1][3]*y)+(M2.m[2][3]*z)+(M2.m[3][3]*w);
		//	x = M1.m[2][0];
		//	y = M1.m[2][1];
		//	z = M1.m[2][2];
		//	w = M1.m[2][3];
		//	mResult.m[2][0] = (M2.m[0][0]*x)+(M2.m[1][0]*y)+(M2.m[2][0]*z)+(M2.m[3][0]*w);
		//	mResult.m[2][1] = (M2.m[0][1]*x)+(M2.m[1][1]*y)+(M2.m[2][1]*z)+(M2.m[3][1]*w);
		//	mResult.m[2][2] = (M2.m[0][2]*x)+(M2.m[1][2]*y)+(M2.m[2][2]*z)+(M2.m[3][2]*w);
		//	mResult.m[2][3] = (M2.m[0][3]*x)+(M2.m[1][3]*y)+(M2.m[2][3]*z)+(M2.m[3][3]*w);
		//	x = M1.m[3][0];
		//	y = M1.m[3][1];
		//	z = M1.m[3][2];
		//	w = M1.m[3][3];
		//	mResult.m[3][0] = (M2.m[0][0]*x)+(M2.m[1][0]*y)+(M2.m[2][0]*z)+(M2.m[3][0]*w);
		//	mResult.m[3][1] = (M2.m[0][1]*x)+(M2.m[1][1]*y)+(M2.m[2][1]*z)+(M2.m[3][1]*w);
		//	mResult.m[3][2] = (M2.m[0][2]*x)+(M2.m[1][2]*y)+(M2.m[2][2]*z)+(M2.m[3][2]*w);
		//	mResult.m[3][3] = (M2.m[0][3]*x)+(M2.m[1][3]*y)+(M2.m[2][3]*z)+(M2.m[3][3]*w);
		//	return mResult;
		//#elif defined(_XM_SSE_INTRINSICS_)
		//	CMatrix3 mResult;
		//	// Use vW to hold the original row
		//	Vec4 vW = M1.r[0];
		//	// Splat the component X,Y,Z then W
		//	Vec4 vX = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(0,0,0,0));
		//	Vec4 vY = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(1,1,1,1));
		//	Vec4 vZ = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(2,2,2,2));
		//	vW = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(3,3,3,3));
		//	// Perform the opertion on the first row
		//	vX = _mm_mul_ps(vX,M2.r[0]);
		//	vY = _mm_mul_ps(vY,M2.r[1]);
		//	vZ = _mm_mul_ps(vZ,M2.r[2]);
		//	vW = _mm_mul_ps(vW,M2.r[3]);
		//	// Perform a binary add to reduce cumulative errors
		//	vX = _mm_add_ps(vX,vZ);
		//	vY = _mm_add_ps(vY,vW);
		//	vX = _mm_add_ps(vX,vY);
		//	mResult.r[0] = vX;
		//	// Repeat for the other 3 rows
		//	vW = M1.r[1];
		//	vX = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(0,0,0,0));
		//	vY = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(1,1,1,1));
		//	vZ = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(2,2,2,2));
		//	vW = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(3,3,3,3));
		//	vX = _mm_mul_ps(vX,M2.r[0]);
		//	vY = _mm_mul_ps(vY,M2.r[1]);
		//	vZ = _mm_mul_ps(vZ,M2.r[2]);
		//	vW = _mm_mul_ps(vW,M2.r[3]);
		//	vX = _mm_add_ps(vX,vZ);
		//	vY = _mm_add_ps(vY,vW);
		//	vX = _mm_add_ps(vX,vY);
		//	mResult.r[1] = vX;
		//	vW = M1.r[2];
		//	vX = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(0,0,0,0));
		//	vY = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(1,1,1,1));
		//	vZ = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(2,2,2,2));
		//	vW = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(3,3,3,3));
		//	vX = _mm_mul_ps(vX,M2.r[0]);
		//	vY = _mm_mul_ps(vY,M2.r[1]);
		//	vZ = _mm_mul_ps(vZ,M2.r[2]);
		//	vW = _mm_mul_ps(vW,M2.r[3]);
		//	vX = _mm_add_ps(vX,vZ);
		//	vY = _mm_add_ps(vY,vW);
		//	vX = _mm_add_ps(vX,vY);
		//	mResult.r[2] = vX;
		//	vW = M1.r[3];
		//	vX = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(0,0,0,0));
		//	vY = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(1,1,1,1));
		//	vZ = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(2,2,2,2));
		//	vW = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(3,3,3,3));
		//	vX = _mm_mul_ps(vX,M2.r[0]);
		//	vY = _mm_mul_ps(vY,M2.r[1]);
		//	vZ = _mm_mul_ps(vZ,M2.r[2]);
		//	vW = _mm_mul_ps(vW,M2.r[3]);
		//	vX = _mm_add_ps(vX,vZ);
		//	vY = _mm_add_ps(vY,vW);
		//	vX = _mm_add_ps(vX,vY);
		//	mResult.r[3] = vX;
		//	return mResult;
		//#endif
		}

		void CMatrix3::MulTranspose(const CMatrix3& mtxRight)
		{
			/*#if defined (XST_SSE2)
				
				CMatrix3 Product;
				Product = Mul( mtxRight );
				this->Set( Transpose( Product ) );

			#else
				CMatrix3 mResult;
				// Cache the invariants in registers
				float x = mtxRight.m[0][0];
				float y = mtxRight.m[1][0];
				float z = mtxRight.m[2][0];
				float w = mtxRight.m[3][0];
				// Perform the operation on the first row
				mResult.m[0][0] = (this->m[0][0]*x)+(this->m[0][1]*y)+(this->m[0][2]*z)+(this->m[0][3]*w);
				mResult.m[0][1] = (this->m[1][0]*x)+(this->m[1][1]*y)+(this->m[1][2]*z)+(this->m[1][3]*w);
				mResult.m[0][2] = (this->m[2][0]*x)+(this->m[2][1]*y)+(this->m[2][2]*z)+(this->m[2][3]*w);
				mResult.m[0][3] = (this->m[3][0]*x)+(this->m[3][1]*y)+(this->m[3][2]*z)+(this->m[3][3]*w);
				// Repeat for all the other rows
				x = mtxRight.m[0][1];
				y = mtxRight.m[1][1];
				z = mtxRight.m[2][1];
				w = mtxRight.m[3][1];
				mResult.m[1][0] = (this->m[0][0]*x)+(this->m[0][1]*y)+(this->m[0][2]*z)+(this->m[0][3]*w);
				mResult.m[1][1] = (this->m[1][0]*x)+(this->m[1][1]*y)+(this->m[1][2]*z)+(this->m[1][3]*w);
				mResult.m[1][2] = (this->m[2][0]*x)+(this->m[2][1]*y)+(this->m[2][2]*z)+(this->m[2][3]*w);
				mResult.m[1][3] = (this->m[3][0]*x)+(this->m[3][1]*y)+(this->m[3][2]*z)+(this->m[3][3]*w);
				x = mtxRight.m[0][2];
				y = mtxRight.m[1][2];
				z = mtxRight.m[2][2];
				w = mtxRight.m[3][2];
				mResult.m[2][0] = (this->m[0][0]*x)+(this->m[0][1]*y)+(this->m[0][2]*z)+(this->m[0][3]*w);
				mResult.m[2][1] = (this->m[1][0]*x)+(this->m[1][1]*y)+(this->m[1][2]*z)+(this->m[1][3]*w);
				mResult.m[2][2] = (this->m[2][0]*x)+(this->m[2][1]*y)+(this->m[2][2]*z)+(this->m[2][3]*w);
				mResult.m[2][3] = (this->m[3][0]*x)+(this->m[3][1]*y)+(this->m[3][2]*z)+(this->m[3][3]*w);
				x = mtxRight.m[0][3];
				y = mtxRight.m[1][3];
				z = mtxRight.m[2][3];
				w = mtxRight.m[3][3];
				mResult.m[3][0] = (this->m[0][0]*x)+(this->m[0][1]*y)+(this->m[0][2]*z)+(this->m[0][3]*w);
				mResult.m[3][1] = (this->m[1][0]*x)+(this->m[1][1]*y)+(this->m[1][2]*z)+(this->m[1][3]*w);
				mResult.m[3][2] = (this->m[2][0]*x)+(this->m[2][1]*y)+(this->m[2][2]*z)+(this->m[2][3]*w);
				mResult.m[3][3] = (this->m[3][0]*x)+(this->m[3][1]*y)+(this->m[3][2]*z)+(this->m[3][3]*w);
		
			#endif*/
		}

				void CMatrix3::Transpose()
				{
		//			CMatrix3 P;
		//	CMatrix3 MT;

		//	// Original matrix:
		//	//
		//	//     m00m01m02m03
		//	//     m10m11m12m13
		//	//     m20m21m22m23
		//	//     m30m31m32m33

		//	P.r[0] = XMVectorMergeXY(M.r[0], M.r[2]); // m00m20m01m21
		//	P.r[1] = XMVectorMergeXY(M.r[1], M.r[3]); // m10m30m11m31
		//	P.r[2] = XMVectorMergeZW(M.r[0], M.r[2]); // m02m22m03m23
		//	P.r[3] = XMVectorMergeZW(M.r[1], M.r[3]); // m12m32m13m33

		//	MT.r[0] = XMVectorMergeXY(P.r[0], P.r[1]); // m00m10m20m30
		//	MT.r[1] = XMVectorMergeZW(P.r[0], P.r[1]); // m01m11m21m31
		//	MT.r[2] = XMVectorMergeXY(P.r[2], P.r[3]); // m02m12m22m32
		//	MT.r[3] = XMVectorMergeZW(P.r[2], P.r[3]); // m03m13m23m33

		//	return MT;

		//#elif defined(_XM_SSE_INTRINSICS_)
		//	// x.x,x.y,y.x,y.y
		//	Vec4 vTemp1 = _mm_shuffle_ps(M.r[0],M.r[1],_MM_SHUFFLE(1,0,1,0));
		//	// x.z,x.w,y.z,y.w
		//	Vec4 vTemp3 = _mm_shuffle_ps(M.r[0],M.r[1],_MM_SHUFFLE(3,2,3,2));
		//	// z.x,z.y,w.x,w.y
		//	Vec4 vTemp2 = _mm_shuffle_ps(M.r[2],M.r[3],_MM_SHUFFLE(1,0,1,0));
		//	// z.z,z.w,w.z,w.w
		//	Vec4 vTemp4 = _mm_shuffle_ps(M.r[2],M.r[3],_MM_SHUFFLE(3,2,3,2));
		//	CMatrix3 mResult;

		//	// x.x,y.x,z.x,w.x
		//	mResult.r[0] = _mm_shuffle_ps(vTemp1, vTemp2,_MM_SHUFFLE(2,0,2,0));
		//	// x.y,y.y,z.y,w.y
		//	mResult.r[1] = _mm_shuffle_ps(vTemp1, vTemp2,_MM_SHUFFLE(3,1,3,1));
		//	// x.z,y.z,z.z,w.z
		//	mResult.r[2] = _mm_shuffle_ps(vTemp3, vTemp4,_MM_SHUFFLE(2,0,2,0));
		//	// x.w,y.w,z.w,w.w
		//	mResult.r[3] = _mm_shuffle_ps(vTemp3, vTemp4,_MM_SHUFFLE(3,1,3,1));
		//	return mResult;
		//#endif
		}

		//CMatrix3	CMatrix3::Mul(const CMatrix3& mtxLeft, const CMatrix3& mtxRight)
		//{
		//	CMatrix3 mResult;
		//	//#if defined (XST_SSE2)
		//	//	// Use vW to hold the original row
		//	//	Vec4 vW = mtxLeft.r[0];
		//	//	// Splat the component X,Y,Z then W
		//	//	Vec4 vX = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(0,0,0,0));
		//	//	Vec4 vY = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(1,1,1,1));
		//	//	Vec4 vZ = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(2,2,2,2));
		//	//	vW = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(3,3,3,3));
		//	//	// Perform the opertion on the first row
		//	//	vX = _mm_mul_ps(vX,mtxRight.r[0]);
		//	//	vY = _mm_mul_ps(vY,mtxRight.r[1]);
		//	//	vZ = _mm_mul_ps(vZ,mtxRight.r[2]);
		//	//	vW = _mm_mul_ps(vW,mtxRight.r[3]);
		//	//	// Perform a binary add to reduce cumulative errors
		//	//	vX = _mm_add_ps(vX,vZ);
		//	//	vY = _mm_add_ps(vY,vW);
		//	//	vX = _mm_add_ps(vX,vY);
		//	//	mResult.r[0] = vX;
		//	//	// Repeat for the other 3 rows
		//	//	vW = mtxLeft.r[1];
		//	//	vX = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(0,0,0,0));
		//	//	vY = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(1,1,1,1));
		//	//	vZ = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(2,2,2,2));
		//	//	vW = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(3,3,3,3));
		//	//	vX = _mm_mul_ps(vX,mtxRight.r[0]);
		//	//	vY = _mm_mul_ps(vY,mtxRight.r[1]);
		//	//	vZ = _mm_mul_ps(vZ,mtxRight.r[2]);
		//	//	vW = _mm_mul_ps(vW,mtxRight.r[3]);
		//	//	vX = _mm_add_ps(vX,vZ);
		//	//	vY = _mm_add_ps(vY,vW);
		//	//	vX = _mm_add_ps(vX,vY);
		//	//	mResult.r[1] = vX;
		//	//	vW = mtxLeft.r[2];
		//	//	vX = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(0,0,0,0));
		//	//	vY = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(1,1,1,1));
		//	//	vZ = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(2,2,2,2));
		//	//	vW = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(3,3,3,3));
		//	//	vX = _mm_mul_ps(vX,mtxRight.r[0]);
		//	//	vY = _mm_mul_ps(vY,mtxRight.r[1]);
		//	//	vZ = _mm_mul_ps(vZ,mtxRight.r[2]);
		//	//	vW = _mm_mul_ps(vW,mtxRight.r[3]);
		//	//	vX = _mm_add_ps(vX,vZ);
		//	//	vY = _mm_add_ps(vY,vW);
		//	//	vX = _mm_add_ps(vX,vY);
		//	//	mResult.r[2] = vX;
		//	//	vW = mtxLeft.r[3];
		//	//	vX = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(0,0,0,0));
		//	//	vY = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(1,1,1,1));
		//	//	vZ = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(2,2,2,2));
		//	//	vW = _mm_shuffle_ps(vW,vW,_MM_SHUFFLE(3,3,3,3));
		//	//	vX = _mm_mul_ps(vX,mtxRight.r[0]);
		//	//	vY = _mm_mul_ps(vY,mtxRight.r[1]);
		//	//	vZ = _mm_mul_ps(vZ,mtxRight.r[2]);
		//	//	vW = _mm_mul_ps(vW,mtxRight.r[3]);
		//	//	vX = _mm_add_ps(vX,vZ);
		//	//	vY = _mm_add_ps(vY,vW);
		//	//	vX = _mm_add_ps(vX,vY);
		//	//	mResult.r[3] = vX;
		//	//#elif defined(_XM_SSE_INTRINSICS_)
		//	//	// Cache the invariants in registers
		//	//	float x = mtxLeft.m[0][0];
		//	//	float y = mtxLeft.m[0][1];
		//	//	float z = mtxLeft.m[0][2];
		//	//	float w = mtxLeft.m[0][3];
		//	//	// Perform the operation on the first row
		//	//	mResult.m[0][0] = (mtxRight.m[0][0]*x)+(mtxRight.m[1][0]*y)+(mtxRight.m[2][0]*z)+(mtxRight.m[3][0]*w);
		//	//	mResult.m[0][1] = (mtxRight.m[0][1]*x)+(mtxRight.m[1][1]*y)+(mtxRight.m[2][1]*z)+(mtxRight.m[3][1]*w);
		//	//	mResult.m[0][2] = (mtxRight.m[0][2]*x)+(mtxRight.m[1][2]*y)+(mtxRight.m[2][2]*z)+(mtxRight.m[3][2]*w);
		//	//	mResult.m[0][3] = (mtxRight.m[0][3]*x)+(mtxRight.m[1][3]*y)+(mtxRight.m[2][3]*z)+(mtxRight.m[3][3]*w);
		//	//	// Repeat for all the other rows
		//	//	x = mtxLeft.m[1][0];
		//	//	y = mtxLeft.m[1][1];
		//	//	z = mtxLeft.m[1][2];
		//	//	w = mtxLeft.m[1][3];
		//	//	mResult.m[1][0] = (mtxRight.m[0][0]*x)+(mtxRight.m[1][0]*y)+(mtxRight.m[2][0]*z)+(mtxRight.m[3][0]*w);
		//	//	mResult.m[1][1] = (mtxRight.m[0][1]*x)+(mtxRight.m[1][1]*y)+(mtxRight.m[2][1]*z)+(mtxRight.m[3][1]*w);
		//	//	mResult.m[1][2] = (mtxRight.m[0][2]*x)+(mtxRight.m[1][2]*y)+(mtxRight.m[2][2]*z)+(mtxRight.m[3][2]*w);
		//	//	mResult.m[1][3] = (mtxRight.m[0][3]*x)+(mtxRight.m[1][3]*y)+(mtxRight.m[2][3]*z)+(mtxRight.m[3][3]*w);
		//	//	x = mtxLeft.m[2][0];
		//	//	y = mtxLeft.m[2][1];
		//	//	z = mtxLeft.m[2][2];
		//	//	w = mtxLeft.m[2][3];
		//	//	mResult.m[2][0] = (mtxRight.m[0][0]*x)+(mtxRight.m[1][0]*y)+(mtxRight.m[2][0]*z)+(mtxRight.m[3][0]*w);
		//	//	mResult.m[2][1] = (mtxRight.m[0][1]*x)+(mtxRight.m[1][1]*y)+(mtxRight.m[2][1]*z)+(mtxRight.m[3][1]*w);
		//	//	mResult.m[2][2] = (mtxRight.m[0][2]*x)+(mtxRight.m[1][2]*y)+(mtxRight.m[2][2]*z)+(mtxRight.m[3][2]*w);
		//	//	mResult.m[2][3] = (mtxRight.m[0][3]*x)+(mtxRight.m[1][3]*y)+(mtxRight.m[2][3]*z)+(mtxRight.m[3][3]*w);
		//	//	x = mtxLeft.m[3][0];
		//	//	y = mtxLeft.m[3][1];
		//	//	z = mtxLeft.m[3][2];
		//	//	w = mtxLeft.m[3][3];
		//	//	mResult.m[3][0] = (mtxRight.m[0][0]*x)+(mtxRight.m[1][0]*y)+(mtxRight.m[2][0]*z)+(mtxRight.m[3][0]*w);
		//	//	mResult.m[3][1] = (mtxRight.m[0][1]*x)+(mtxRight.m[1][1]*y)+(mtxRight.m[2][1]*z)+(mtxRight.m[3][1]*w);
		//	//	mResult.m[3][2] = (mtxRight.m[0][2]*x)+(mtxRight.m[1][2]*y)+(mtxRight.m[2][2]*z)+(mtxRight.m[3][2]*w);
		//	//	mResult.m[3][3] = (mtxRight.m[0][3]*x)+(mtxRight.m[1][3]*y)+(mtxRight.m[2][3]*z)+(mtxRight.m[3][3]*w);
		//	//	return mResult;
		//	//#endif

		//	return mResult;
		//}

		void	CMatrix3::Inverse(const CMatrix3& mtx, CMatrix3* pMtxOut)
		{

			f32 inv[16], det;
			int i;

			inv[0] = mtx.mm[5]  * mtx.mm[10] * mtx.mm[15] - 
					 mtx.mm[5]  * mtx.mm[11] * mtx.mm[14] - 
					 mtx.mm[9]  * mtx.mm[6]  * mtx.mm[15] + 
					 mtx.mm[9]  * mtx.mm[7]  * mtx.mm[14] +
					 mtx.mm[13] * mtx.mm[6]  * mtx.mm[11] - 
					 mtx.mm[13] * mtx.mm[7]  * mtx.mm[10];

			inv[4] = -mtx.mm[4]  * mtx.mm[10] * mtx.mm[15] + 
					  mtx.mm[4]  * mtx.mm[11] * mtx.mm[14] + 
					  mtx.mm[8]  * mtx.mm[6]  * mtx.mm[15] - 
					  mtx.mm[8]  * mtx.mm[7]  * mtx.mm[14] - 
					  mtx.mm[12] * mtx.mm[6]  * mtx.mm[11] + 
					  mtx.mm[12] * mtx.mm[7]  * mtx.mm[10];

			inv[8] = mtx.mm[4]  * mtx.mm[9] * mtx.mm[15] - 
					 mtx.mm[4]  * mtx.mm[11] * mtx.mm[13] - 
					 mtx.mm[8]  * mtx.mm[5] * mtx.mm[15] + 
					 mtx.mm[8]  * mtx.mm[7] * mtx.mm[13] + 
					 mtx.mm[12] * mtx.mm[5] * mtx.mm[11] - 
					 mtx.mm[12] * mtx.mm[7] * mtx.mm[9];

			inv[12] = -mtx.mm[4]  * mtx.mm[9] * mtx.mm[14] + 
					   mtx.mm[4]  * mtx.mm[10] * mtx.mm[13] +
					   mtx.mm[8]  * mtx.mm[5] * mtx.mm[14] - 
					   mtx.mm[8]  * mtx.mm[6] * mtx.mm[13] - 
					   mtx.mm[12] * mtx.mm[5] * mtx.mm[10] + 
					   mtx.mm[12] * mtx.mm[6] * mtx.mm[9];

			inv[1] = -mtx.mm[1]  * mtx.mm[10] * mtx.mm[15] + 
					  mtx.mm[1]  * mtx.mm[11] * mtx.mm[14] + 
					  mtx.mm[9]  * mtx.mm[2] * mtx.mm[15] - 
					  mtx.mm[9]  * mtx.mm[3] * mtx.mm[14] - 
					  mtx.mm[13] * mtx.mm[2] * mtx.mm[11] + 
					  mtx.mm[13] * mtx.mm[3] * mtx.mm[10];

			inv[5] = mtx.mm[0]  * mtx.mm[10] * mtx.mm[15] - 
					 mtx.mm[0]  * mtx.mm[11] * mtx.mm[14] - 
					 mtx.mm[8]  * mtx.mm[2] * mtx.mm[15] + 
					 mtx.mm[8]  * mtx.mm[3] * mtx.mm[14] + 
					 mtx.mm[12] * mtx.mm[2] * mtx.mm[11] - 
					 mtx.mm[12] * mtx.mm[3] * mtx.mm[10];

			inv[9] = -mtx.mm[0]  * mtx.mm[9] * mtx.mm[15] + 
					  mtx.mm[0]  * mtx.mm[11] * mtx.mm[13] + 
					  mtx.mm[8]  * mtx.mm[1] * mtx.mm[15] - 
					  mtx.mm[8]  * mtx.mm[3] * mtx.mm[13] - 
					  mtx.mm[12] * mtx.mm[1] * mtx.mm[11] + 
					  mtx.mm[12] * mtx.mm[3] * mtx.mm[9];

			inv[13] = mtx.mm[0]  * mtx.mm[9] * mtx.mm[14] - 
					  mtx.mm[0]  * mtx.mm[10] * mtx.mm[13] - 
					  mtx.mm[8]  * mtx.mm[1] * mtx.mm[14] + 
					  mtx.mm[8]  * mtx.mm[2] * mtx.mm[13] + 
					  mtx.mm[12] * mtx.mm[1] * mtx.mm[10] - 
					  mtx.mm[12] * mtx.mm[2] * mtx.mm[9];

			inv[2] = mtx.mm[1]  * mtx.mm[6] * mtx.mm[15] - 
					 mtx.mm[1]  * mtx.mm[7] * mtx.mm[14] - 
					 mtx.mm[5]  * mtx.mm[2] * mtx.mm[15] + 
					 mtx.mm[5]  * mtx.mm[3] * mtx.mm[14] + 
					 mtx.mm[13] * mtx.mm[2] * mtx.mm[7] - 
					 mtx.mm[13] * mtx.mm[3] * mtx.mm[6];

			inv[6] = -mtx.mm[0]  * mtx.mm[6] * mtx.mm[15] + 
					  mtx.mm[0]  * mtx.mm[7] * mtx.mm[14] + 
					  mtx.mm[4]  * mtx.mm[2] * mtx.mm[15] - 
					  mtx.mm[4]  * mtx.mm[3] * mtx.mm[14] - 
					  mtx.mm[12] * mtx.mm[2] * mtx.mm[7] + 
					  mtx.mm[12] * mtx.mm[3] * mtx.mm[6];

			inv[10] = mtx.mm[0]  * mtx.mm[5] * mtx.mm[15] - 
					  mtx.mm[0]  * mtx.mm[7] * mtx.mm[13] - 
					  mtx.mm[4]  * mtx.mm[1] * mtx.mm[15] + 
					  mtx.mm[4]  * mtx.mm[3] * mtx.mm[13] + 
					  mtx.mm[12] * mtx.mm[1] * mtx.mm[7] - 
					  mtx.mm[12] * mtx.mm[3] * mtx.mm[5];

			inv[14] = -mtx.mm[0]  * mtx.mm[5] * mtx.mm[14] + 
					   mtx.mm[0]  * mtx.mm[6] * mtx.mm[13] + 
					   mtx.mm[4]  * mtx.mm[1] * mtx.mm[14] - 
					   mtx.mm[4]  * mtx.mm[2] * mtx.mm[13] - 
					   mtx.mm[12] * mtx.mm[1] * mtx.mm[6] + 
					   mtx.mm[12] * mtx.mm[2] * mtx.mm[5];

			inv[3] = -mtx.mm[1] * mtx.mm[6] * mtx.mm[11] + 
					  mtx.mm[1] * mtx.mm[7] * mtx.mm[10] + 
					  mtx.mm[5] * mtx.mm[2] * mtx.mm[11] - 
					  mtx.mm[5] * mtx.mm[3] * mtx.mm[10] - 
					  mtx.mm[9] * mtx.mm[2] * mtx.mm[7] + 
					  mtx.mm[9] * mtx.mm[3] * mtx.mm[6];

			inv[7] = mtx.mm[0] * mtx.mm[6] * mtx.mm[11] - 
					 mtx.mm[0] * mtx.mm[7] * mtx.mm[10] - 
					 mtx.mm[4] * mtx.mm[2] * mtx.mm[11] + 
					 mtx.mm[4] * mtx.mm[3] * mtx.mm[10] + 
					 mtx.mm[8] * mtx.mm[2] * mtx.mm[7] - 
					 mtx.mm[8] * mtx.mm[3] * mtx.mm[6];

			inv[11] = -mtx.mm[0] * mtx.mm[5] * mtx.mm[11] + 
					   mtx.mm[0] * mtx.mm[7] * mtx.mm[9] + 
					   mtx.mm[4] * mtx.mm[1] * mtx.mm[11] - 
					   mtx.mm[4] * mtx.mm[3] * mtx.mm[9] - 
					   mtx.mm[8] * mtx.mm[1] * mtx.mm[7] + 
					   mtx.mm[8] * mtx.mm[3] * mtx.mm[5];

			inv[15] = mtx.mm[0] * mtx.mm[5] * mtx.mm[10] - 
					  mtx.mm[0] * mtx.mm[6] * mtx.mm[9] - 
					  mtx.mm[4] * mtx.mm[1] * mtx.mm[10] + 
					  mtx.mm[4] * mtx.mm[2] * mtx.mm[9] + 
					  mtx.mm[8] * mtx.mm[1] * mtx.mm[6] - 
					  mtx.mm[8] * mtx.mm[2] * mtx.mm[5];

			det = mtx.mm[0] * inv[0] + mtx.mm[1] * inv[4] + mtx.mm[2] * inv[8] + mtx.mm[3] * inv[12];

			if (det == 0)
				return;

			det = 1.0 / det;

			for (i = 0; i < 16; i++)
				pMtxOut->mm[i] = inv[i] * det;
		}

		void CMatrix3::Inverse()
		{
			//CMatrix3 mtxInv = Inverse( *this );
			//xst_memcpy( this->m, sizeof( this->m ), mtxInv.m, sizeof( mtxInv.m ) );
			Inverse( *this, this );
			 /*CMatrix3               R;
				CMatrix3               MT;
				Vec4               D0, D1, D2;
				Vec4               C0, C1, C2, C3, C4, C5, C6, C7;
				Vec4               V0[4], V1[4];
				Vec4               Determinant;
				Vec4               Reciprocal;
				CMatrix3               Result;
				static CONST XMVECTORU32 SwizzleXXYY = {XM_PERMUTE_0X, XM_PERMUTE_0X, XM_PERMUTE_0Y, XM_PERMUTE_0Y};
				static CONST XMVECTORU32 SwizzleZWZW = {XM_PERMUTE_0Z, XM_PERMUTE_0W, XM_PERMUTE_0Z, XM_PERMUTE_0W};
				static CONST XMVECTORU32 SwizzleYZXY = {XM_PERMUTE_0Y, XM_PERMUTE_0Z, XM_PERMUTE_0X, XM_PERMUTE_0Y};
				static CONST XMVECTORU32 SwizzleZWYZ = {XM_PERMUTE_0Z, XM_PERMUTE_0W, XM_PERMUTE_0Y, XM_PERMUTE_0Z};
				static CONST XMVECTORU32 SwizzleWXWX = {XM_PERMUTE_0W, XM_PERMUTE_0X, XM_PERMUTE_0W, XM_PERMUTE_0X};
				static CONST XMVECTORU32 SwizzleZXYX = {XM_PERMUTE_0Z, XM_PERMUTE_0X, XM_PERMUTE_0Y, XM_PERMUTE_0X};
				static CONST XMVECTORU32 SwizzleYWXZ = {XM_PERMUTE_0Y, XM_PERMUTE_0W, XM_PERMUTE_0X, XM_PERMUTE_0Z};
				static CONST XMVECTORU32 SwizzleWZWY = {XM_PERMUTE_0W, XM_PERMUTE_0Z, XM_PERMUTE_0W, XM_PERMUTE_0Y};
				static CONST XMVECTORU32 Permute0X0Z1X1Z = {XM_PERMUTE_0X, XM_PERMUTE_0Z, XM_PERMUTE_1X, XM_PERMUTE_1Z};
				static CONST XMVECTORU32 Permute0Y0W1Y1W = {XM_PERMUTE_0Y, XM_PERMUTE_0W, XM_PERMUTE_1Y, XM_PERMUTE_1W};
				static CONST XMVECTORU32 Permute1Y0Y0W0X = {XM_PERMUTE_1Y, XM_PERMUTE_0Y, XM_PERMUTE_0W, XM_PERMUTE_0X};
				static CONST XMVECTORU32 Permute0W0X0Y1X = {XM_PERMUTE_0W, XM_PERMUTE_0X, XM_PERMUTE_0Y, XM_PERMUTE_1X};
				static CONST XMVECTORU32 Permute0Z1Y1X0Z = {XM_PERMUTE_0Z, XM_PERMUTE_1Y, XM_PERMUTE_1X, XM_PERMUTE_0Z};
				static CONST XMVECTORU32 Permute0W1Y0Y0Z = {XM_PERMUTE_0W, XM_PERMUTE_1Y, XM_PERMUTE_0Y, XM_PERMUTE_0Z};
				static CONST XMVECTORU32 Permute0Z0Y1X0X = {XM_PERMUTE_0Z, XM_PERMUTE_0Y, XM_PERMUTE_1X, XM_PERMUTE_0X};
				static CONST XMVECTORU32 Permute1Y0X0W1X = {XM_PERMUTE_1Y, XM_PERMUTE_0X, XM_PERMUTE_0W, XM_PERMUTE_1X};
				static CONST XMVECTORU32 Permute1W0Y0W0X = {XM_PERMUTE_1W, XM_PERMUTE_0Y, XM_PERMUTE_0W, XM_PERMUTE_0X};
				static CONST XMVECTORU32 Permute0W0X0Y1Z = {XM_PERMUTE_0W, XM_PERMUTE_0X, XM_PERMUTE_0Y, XM_PERMUTE_1Z};
				static CONST XMVECTORU32 Permute0Z1W1Z0Z = {XM_PERMUTE_0Z, XM_PERMUTE_1W, XM_PERMUTE_1Z, XM_PERMUTE_0Z};
				static CONST XMVECTORU32 Permute0W1W0Y0Z = {XM_PERMUTE_0W, XM_PERMUTE_1W, XM_PERMUTE_0Y, XM_PERMUTE_0Z};
				static CONST XMVECTORU32 Permute0Z0Y1Z0X = {XM_PERMUTE_0Z, XM_PERMUTE_0Y, XM_PERMUTE_1Z, XM_PERMUTE_0X};
				static CONST XMVECTORU32 Permute1W0X0W1Z = {XM_PERMUTE_1W, XM_PERMUTE_0X, XM_PERMUTE_0W, XM_PERMUTE_1Z};

				XMASSERT(pDeterminant);

				MT = XMMatrixTranspose(M);

				V0[0] = XMVectorPermute(MT.r[2], MT.r[2], SwizzleXXYY.v);
				V1[0] = XMVectorPermute(MT.r[3], MT.r[3], SwizzleZWZW.v);
				V0[1] = XMVectorPermute(MT.r[0], MT.r[0], SwizzleXXYY.v);
				V1[1] = XMVectorPermute(MT.r[1], MT.r[1], SwizzleZWZW.v);
				V0[2] = XMVectorPermute(MT.r[2], MT.r[0], Permute0X0Z1X1Z.v);
				V1[2] = XMVectorPermute(MT.r[3], MT.r[1], Permute0Y0W1Y1W.v);

				D0 = XMVectorMultiply(V0[0], V1[0]);
				D1 = XMVectorMultiply(V0[1], V1[1]);
				D2 = XMVectorMultiply(V0[2], V1[2]);

				V0[0] = XMVectorPermute(MT.r[2], MT.r[2], SwizzleZWZW.v);
				V1[0] = XMVectorPermute(MT.r[3], MT.r[3], SwizzleXXYY.v);
				V0[1] = XMVectorPermute(MT.r[0], MT.r[0], SwizzleZWZW.v);
				V1[1] = XMVectorPermute(MT.r[1], MT.r[1], SwizzleXXYY.v);
				V0[2] = XMVectorPermute(MT.r[2], MT.r[0], Permute0Y0W1Y1W.v);
				V1[2] = XMVectorPermute(MT.r[3], MT.r[1], Permute0X0Z1X1Z.v);

				D0 = XMVectorNegativeMultiplySubtract(V0[0], V1[0], D0);
				D1 = XMVectorNegativeMultiplySubtract(V0[1], V1[1], D1);
				D2 = XMVectorNegativeMultiplySubtract(V0[2], V1[2], D2);

				V0[0] = XMVectorPermute(MT.r[1], MT.r[1], SwizzleYZXY.v);
				V1[0] = XMVectorPermute(D0, D2, Permute1Y0Y0W0X.v);
				V0[1] = XMVectorPermute(MT.r[0], MT.r[0], SwizzleZXYX.v);
				V1[1] = XMVectorPermute(D0, D2, Permute0W1Y0Y0Z.v);
				V0[2] = XMVectorPermute(MT.r[3], MT.r[3], SwizzleYZXY.v);
				V1[2] = XMVectorPermute(D1, D2, Permute1W0Y0W0X.v);
				V0[3] = XMVectorPermute(MT.r[2], MT.r[2], SwizzleZXYX.v);
				V1[3] = XMVectorPermute(D1, D2, Permute0W1W0Y0Z.v);

				C0 = XMVectorMultiply(V0[0], V1[0]);
				C2 = XMVectorMultiply(V0[1], V1[1]);
				C4 = XMVectorMultiply(V0[2], V1[2]);
				C6 = XMVectorMultiply(V0[3], V1[3]);

				V0[0] = XMVectorPermute(MT.r[1], MT.r[1], SwizzleZWYZ.v);
				V1[0] = XMVectorPermute(D0, D2, Permute0W0X0Y1X.v);
				V0[1] = XMVectorPermute(MT.r[0], MT.r[0], SwizzleWZWY.v);
				V1[1] = XMVectorPermute(D0, D2, Permute0Z0Y1X0X.v);
				V0[2] = XMVectorPermute(MT.r[3], MT.r[3], SwizzleZWYZ.v);
				V1[2] = XMVectorPermute(D1, D2, Permute0W0X0Y1Z.v);
				V0[3] = XMVectorPermute(MT.r[2], MT.r[2], SwizzleWZWY.v);
				V1[3] = XMVectorPermute(D1, D2, Permute0Z0Y1Z0X.v);

				C0 = XMVectorNegativeMultiplySubtract(V0[0], V1[0], C0);
				C2 = XMVectorNegativeMultiplySubtract(V0[1], V1[1], C2);
				C4 = XMVectorNegativeMultiplySubtract(V0[2], V1[2], C4);
				C6 = XMVectorNegativeMultiplySubtract(V0[3], V1[3], C6);

				V0[0] = XMVectorPermute(MT.r[1], MT.r[1], SwizzleWXWX.v);
				V1[0] = XMVectorPermute(D0, D2, Permute0Z1Y1X0Z.v);
				V0[1] = XMVectorPermute(MT.r[0], MT.r[0], SwizzleYWXZ.v);
				V1[1] = XMVectorPermute(D0, D2, Permute1Y0X0W1X.v);
				V0[2] = XMVectorPermute(MT.r[3], MT.r[3], SwizzleWXWX.v);
				V1[2] = XMVectorPermute(D1, D2, Permute0Z1W1Z0Z.v);
				V0[3] = XMVectorPermute(MT.r[2], MT.r[2], SwizzleYWXZ.v);
				V1[3] = XMVectorPermute(D1, D2, Permute1W0X0W1Z.v);

				C1 = XMVectorNegativeMultiplySubtract(V0[0], V1[0], C0);
				C0 = XMVectorMultiplyAdd(V0[0], V1[0], C0);
				C3 = XMVectorMultiplyAdd(V0[1], V1[1], C2);
				C2 = XMVectorNegativeMultiplySubtract(V0[1], V1[1], C2);
				C5 = XMVectorNegativeMultiplySubtract(V0[2], V1[2], C4);
				C4 = XMVectorMultiplyAdd(V0[2], V1[2], C4);
				C7 = XMVectorMultiplyAdd(V0[3], V1[3], C6);
				C6 = XMVectorNegativeMultiplySubtract(V0[3], V1[3], C6);

				R.r[0] = XMVectorSelect(C0, C1, g_XMSelect0101.v);
				R.r[1] = XMVectorSelect(C2, C3, g_XMSelect0101.v);
				R.r[2] = XMVectorSelect(C4, C5, g_XMSelect0101.v);
				R.r[3] = XMVectorSelect(C6, C7, g_XMSelect0101.v);

				Determinant = XMVector4Dot(R.r[0], MT.r[0]);

				*pDeterminant = Determinant;

				Reciprocal = XMVectorReciprocal(Determinant);

				Result.r[0] = XMVectorMultiply(R.r[0], Reciprocal);
				Result.r[1] = XMVectorMultiply(R.r[1], Reciprocal);
				Result.r[2] = XMVectorMultiply(R.r[2], Reciprocal);
				Result.r[3] = XMVectorMultiply(R.r[3], Reciprocal);

				return Result;

			#elif defined(_XM_SSE_INTRINSICS_)
				XMASSERT(pDeterminant);
				CMatrix3 MT = XMMatrixTranspose(M);
				Vec4 V00 = _mm_shuffle_ps(MT.r[2], MT.r[2],_MM_SHUFFLE(1,1,0,0));
				Vec4 V10 = _mm_shuffle_ps(MT.r[3], MT.r[3],_MM_SHUFFLE(3,2,3,2));
				Vec4 V01 = _mm_shuffle_ps(MT.r[0], MT.r[0],_MM_SHUFFLE(1,1,0,0));
				Vec4 V11 = _mm_shuffle_ps(MT.r[1], MT.r[1],_MM_SHUFFLE(3,2,3,2));
				Vec4 V02 = _mm_shuffle_ps(MT.r[2], MT.r[0],_MM_SHUFFLE(2,0,2,0));
				Vec4 V12 = _mm_shuffle_ps(MT.r[3], MT.r[1],_MM_SHUFFLE(3,1,3,1));

				Vec4 D0 = _mm_mul_ps(V00,V10);
				Vec4 D1 = _mm_mul_ps(V01,V11);
				Vec4 D2 = _mm_mul_ps(V02,V12);

				V00 = _mm_shuffle_ps(MT.r[2],MT.r[2],_MM_SHUFFLE(3,2,3,2));
				V10 = _mm_shuffle_ps(MT.r[3],MT.r[3],_MM_SHUFFLE(1,1,0,0));
				V01 = _mm_shuffle_ps(MT.r[0],MT.r[0],_MM_SHUFFLE(3,2,3,2));
				V11 = _mm_shuffle_ps(MT.r[1],MT.r[1],_MM_SHUFFLE(1,1,0,0));
				V02 = _mm_shuffle_ps(MT.r[2],MT.r[0],_MM_SHUFFLE(3,1,3,1));
				V12 = _mm_shuffle_ps(MT.r[3],MT.r[1],_MM_SHUFFLE(2,0,2,0));

				V00 = _mm_mul_ps(V00,V10);
				V01 = _mm_mul_ps(V01,V11);
				V02 = _mm_mul_ps(V02,V12);
				D0 = _mm_sub_ps(D0,V00);
				D1 = _mm_sub_ps(D1,V01);
				D2 = _mm_sub_ps(D2,V02);
				// V11 = D0Y,D0W,D2Y,D2Y
				V11 = _mm_shuffle_ps(D0,D2,_MM_SHUFFLE(1,1,3,1));
				V00 = _mm_shuffle_ps(MT.r[1], MT.r[1],_MM_SHUFFLE(1,0,2,1));
				V10 = _mm_shuffle_ps(V11,D0,_MM_SHUFFLE(0,3,0,2));
				V01 = _mm_shuffle_ps(MT.r[0], MT.r[0],_MM_SHUFFLE(0,1,0,2));
				V11 = _mm_shuffle_ps(V11,D0,_MM_SHUFFLE(2,1,2,1));
				// V13 = D1Y,D1W,D2W,D2W
				Vec4 V13 = _mm_shuffle_ps(D1,D2,_MM_SHUFFLE(3,3,3,1));
				V02 = _mm_shuffle_ps(MT.r[3], MT.r[3],_MM_SHUFFLE(1,0,2,1));
				V12 = _mm_shuffle_ps(V13,D1,_MM_SHUFFLE(0,3,0,2));
				Vec4 V03 = _mm_shuffle_ps(MT.r[2], MT.r[2],_MM_SHUFFLE(0,1,0,2));
				V13 = _mm_shuffle_ps(V13,D1,_MM_SHUFFLE(2,1,2,1));

				Vec4 C0 = _mm_mul_ps(V00,V10);
				Vec4 C2 = _mm_mul_ps(V01,V11);
				Vec4 C4 = _mm_mul_ps(V02,V12);
				Vec4 C6 = _mm_mul_ps(V03,V13);

				// V11 = D0X,D0Y,D2X,D2X
				V11 = _mm_shuffle_ps(D0,D2,_MM_SHUFFLE(0,0,1,0));
				V00 = _mm_shuffle_ps(MT.r[1], MT.r[1],_MM_SHUFFLE(2,1,3,2));
				V10 = _mm_shuffle_ps(D0,V11,_MM_SHUFFLE(2,1,0,3));
				V01 = _mm_shuffle_ps(MT.r[0], MT.r[0],_MM_SHUFFLE(1,3,2,3));
				V11 = _mm_shuffle_ps(D0,V11,_MM_SHUFFLE(0,2,1,2));
				// V13 = D1X,D1Y,D2Z,D2Z
				V13 = _mm_shuffle_ps(D1,D2,_MM_SHUFFLE(2,2,1,0));
				V02 = _mm_shuffle_ps(MT.r[3], MT.r[3],_MM_SHUFFLE(2,1,3,2));
				V12 = _mm_shuffle_ps(D1,V13,_MM_SHUFFLE(2,1,0,3));
				V03 = _mm_shuffle_ps(MT.r[2], MT.r[2],_MM_SHUFFLE(1,3,2,3));
				V13 = _mm_shuffle_ps(D1,V13,_MM_SHUFFLE(0,2,1,2));

				V00 = _mm_mul_ps(V00,V10);
				V01 = _mm_mul_ps(V01,V11);
				V02 = _mm_mul_ps(V02,V12);
				V03 = _mm_mul_ps(V03,V13);
				C0 = _mm_sub_ps(C0,V00);
				C2 = _mm_sub_ps(C2,V01);
				C4 = _mm_sub_ps(C4,V02);
				C6 = _mm_sub_ps(C6,V03);

				V00 = _mm_shuffle_ps(MT.r[1],MT.r[1],_MM_SHUFFLE(0,3,0,3));
				// V10 = D0Z,D0Z,D2X,D2Y
				V10 = _mm_shuffle_ps(D0,D2,_MM_SHUFFLE(1,0,2,2));
				V10 = _mm_shuffle_ps(V10,V10,_MM_SHUFFLE(0,2,3,0));
				V01 = _mm_shuffle_ps(MT.r[0],MT.r[0],_MM_SHUFFLE(2,0,3,1));
				// V11 = D0X,D0W,D2X,D2Y
				V11 = _mm_shuffle_ps(D0,D2,_MM_SHUFFLE(1,0,3,0));
				V11 = _mm_shuffle_ps(V11,V11,_MM_SHUFFLE(2,1,0,3));
				V02 = _mm_shuffle_ps(MT.r[3],MT.r[3],_MM_SHUFFLE(0,3,0,3));
				// V12 = D1Z,D1Z,D2Z,D2W
				V12 = _mm_shuffle_ps(D1,D2,_MM_SHUFFLE(3,2,2,2));
				V12 = _mm_shuffle_ps(V12,V12,_MM_SHUFFLE(0,2,3,0));
				V03 = _mm_shuffle_ps(MT.r[2],MT.r[2],_MM_SHUFFLE(2,0,3,1));
				// V13 = D1X,D1W,D2Z,D2W
				V13 = _mm_shuffle_ps(D1,D2,_MM_SHUFFLE(3,2,3,0));
				V13 = _mm_shuffle_ps(V13,V13,_MM_SHUFFLE(2,1,0,3));

				V00 = _mm_mul_ps(V00,V10);
				V01 = _mm_mul_ps(V01,V11);
				V02 = _mm_mul_ps(V02,V12);
				V03 = _mm_mul_ps(V03,V13);
				Vec4 C1 = _mm_sub_ps(C0,V00);
				C0 = _mm_add_ps(C0,V00);
				Vec4 C3 = _mm_add_ps(C2,V01);
				C2 = _mm_sub_ps(C2,V01);
				Vec4 C5 = _mm_sub_ps(C4,V02);
				C4 = _mm_add_ps(C4,V02);
				Vec4 C7 = _mm_add_ps(C6,V03);
				C6 = _mm_sub_ps(C6,V03);

				C0 = _mm_shuffle_ps(C0,C1,_MM_SHUFFLE(3,1,2,0));
				C2 = _mm_shuffle_ps(C2,C3,_MM_SHUFFLE(3,1,2,0));
				C4 = _mm_shuffle_ps(C4,C5,_MM_SHUFFLE(3,1,2,0));
				C6 = _mm_shuffle_ps(C6,C7,_MM_SHUFFLE(3,1,2,0));
				C0 = _mm_shuffle_ps(C0,C0,_MM_SHUFFLE(3,1,2,0));
				C2 = _mm_shuffle_ps(C2,C2,_MM_SHUFFLE(3,1,2,0));
				C4 = _mm_shuffle_ps(C4,C4,_MM_SHUFFLE(3,1,2,0));
				C6 = _mm_shuffle_ps(C6,C6,_MM_SHUFFLE(3,1,2,0));
				// Get the determinate
				Vec4 vTemp = XMVector4Dot(C0,MT.r[0]);
				*pDeterminant = vTemp;
				vTemp = _mm_div_ps(g_XMOne,vTemp);
				CMatrix3 mResult;
				mResult.r[0] = _mm_mul_ps(C0,vTemp);
				mResult.r[1] = _mm_mul_ps(C2,vTemp);
				mResult.r[2] = _mm_mul_ps(C4,vTemp);
				mResult.r[3] = _mm_mul_ps(C6,vTemp);
			#endif*/
		}
	}
}//xst