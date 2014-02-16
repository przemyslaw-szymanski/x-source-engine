#include "XSECBoundingSphere.h"

namespace XSE
{
	//f32 CBoundingSphere::CalcDistance(const CBoundingSphere& Sphere) const
	//{
	//	#if defined (XST_SSE2_not_used)

	//		m128 vSub = _mm_sub_ps( this->vecCenter, Sphere.vecCenter );
	//		// Perform the dot product on x,y,z and w
	//		m128 vLengthSq = _mm_mul_ps( vSub, vSub );
	//		// vTemp has z and w
	//		m128 vTemp = _mm_shuffle_ps( vLengthSq, vLengthSq, _MM_SHUFFLE(3,2,3,2) );
	//		// x+z, y+w
	//		vLengthSq = _mm_add_ps(vLengthSq,vTemp);
	//		// x+z,x+z,x+z,y+w
	//		vLengthSq = _mm_shuffle_ps( vLengthSq, vLengthSq, _MM_SHUFFLE(1,0,0,0) );
	//		// ??,??,y+w,y+w;
	//		vTemp = _mm_shuffle_ps( vTemp, vLengthSq, _MM_SHUFFLE(3,3,0,0) );
	//		// ??,??,x+z+y+w,??
	//		vLengthSq = _mm_add_ps( vLengthSq, vTemp );
	//		// Splat the length
	//		vLengthSq = _mm_shuffle_ps( vLengthSq, vLengthSq, _MM_SHUFFLE(2,2,2,2) );
	//		// Prepare for the division
	//		vLengthSq = _mm_sqrt_ps( vLengthSq );
	//		return vLengthSq.m128_f32[ 0 ];

	//	#else

	//		return this->vecCenter.Distance( Sphere.vecCenter );

	//	#endif
	//}

}//xse