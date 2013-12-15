#include "XST/XSTCPlane.h"

namespace XST
{

	/*void CPlane::Normalize()
	{
		vecPlane = Math::CVector3::Normalize( vecPlane );
	}*/

	//f32	CPlane::Dot(const Math::CVector4& vecVector)
	//{
	//	#if defined (XST_SSE2)
	//		Math::CVector4 vTemp2 = _mm_and_ps( vecVector, Math::CVector4( 0xffffffff, 0xffffffff, 0xffffffff, 0 ) );
	//		vTemp2 = _mm_or_ps( vTemp2, Math::CVector4::W );
	//		Math::CVector4 vTemp = _mm_mul_ps( this->vecPlane, vTemp2 );
	//		vTemp2 = _mm_shuffle_ps( vTemp2, vTemp,_MM_SHUFFLE(1,0,0,0) ); // Copy X to the Z position and Y to the W position
	//		vTemp2 = _mm_add_ps( vTemp2, vTemp );          // Add Z = X+Z; W = Y+W;
	//		vTemp = _mm_shuffle_ps( vTemp, vTemp2, _MM_SHUFFLE(0,3,0,0) );  // Copy W to the Z position
	//		vTemp = _mm_add_ps( vTemp, vTemp2 );           // Add Z and W together
	//		return _mm_shuffle_ps( vTemp, vTemp, _MM_SHUFFLE(2,2,2,2) ).m_f32[ 0 ];    // Splat Z and return
	//	#else
	//		return vecPlane.x * vecVector.x + vecPlane.y * vecVector.y + vecPlane.z * vecVector.z + vecPlane.w * 1.0f;
	//	#endif
	//}


}//xst