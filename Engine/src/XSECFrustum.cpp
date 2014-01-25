#include "XSECFrustum.h"

namespace XSE
{
	//DirectX::XMVector3Normalize
	xst_fi Vec4 Vector3Normalize
	(
		const Vec4& V
	)
	{
	#if defined(_XM_NO_INTRINSICS_)
		float fLength;
		Vec4 vResult;

		vResult = XMVector3Length( V );
		fLength = vResult.m128_f32[0];

		// Prevent divide by zero
		if (fLength > 0) {
			fLength = 1.0f/fLength;
		}
    
		vResult.m128_f32[0] = V.m128_f32[0]*fLength;
		vResult.m128_f32[1] = V.m128_f32[1]*fLength;
		vResult.m128_f32[2] = V.m128_f32[2]*fLength;
		vResult.m128_f32[3] = V.m128_f32[3]*fLength;
		return vResult;
	#elif defined(XST_SSE2)
		// Perform the dot product on x,y and z only
		m128 v = V;
		m128 vLengthSq = _mm_mul_ps(v,v);
		m128 vTemp = XST_SSE_PERMUTE_PS(vLengthSq,_MM_SHUFFLE(2,1,2,1));
		vLengthSq = _mm_add_ss(vLengthSq,vTemp);
		vTemp = XST_SSE_PERMUTE_PS(vTemp,_MM_SHUFFLE(1,1,1,1));
		vLengthSq = _mm_add_ss(vLengthSq,vTemp);
		vLengthSq = XST_SSE_PERMUTE_PS(vLengthSq,_MM_SHUFFLE(0,0,0,0));
		// Prepare for the division
		m128 vResult = _mm_sqrt_ps(vLengthSq);
		// Create zero with a single instruction
		m128 vZeroMask = _mm_setzero_ps();
		// Test for a divide by zero (Must be FP to detect -0.0)
		vZeroMask = _mm_cmpneq_ps(vZeroMask,vResult);
		// Failsafe on zero (Or epsilon) length planes
		// If the length is infinity, set the elements to zero
		vLengthSq = _mm_cmpneq_ps(vLengthSq,Vec4::INF);
		// Divide to perform the normalization
		vResult = _mm_div_ps(V,vResult);
		// Any that are infinity, set to zero
		vResult = _mm_and_ps(vResult,vZeroMask);
		// Select qnan or result based on infinite length
		m128 vTemp1 = _mm_andnot_ps(vLengthSq,Vec4::NOT_A_NUMBER);
		m128 vTemp2 = _mm_and_ps(vResult,vLengthSq);
		vResult = _mm_or_ps(vTemp1,vTemp2);
		return vResult;
	#else // _XM_VMX128_INTRINSICS_
	#endif // _XM_VMX128_INTRINSICS_
	}

	xst_fi Vec4 VectorReplicatePtr
	(
		const float *pValue
	)
	{
	#if defined(_XM_NO_INTRINSICS_) || defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
		float Value = pValue[0];
		XMVECTORF32 vResult = {Value,Value,Value,Value};
		return vResult;
	#elif defined(XST_SSE2)
		return _mm_load_ps1( pValue );
	#else // _XM_VMX128_INTRINSICS_
	#endif // _XM_VMX128_INTRINSICS_
	}

	void CFrustum::CreateFromMatrix(const Mtx4& mtxProj)
	{
		// Corners of the projection frustum in homogenous space.
		static Vec4 HomogenousPoints[6] =
		{
			{  1.0f,  0.0f, 1.0f, 1.0f },   // right (at far plane)
			{ -1.0f,  0.0f, 1.0f, 1.0f },   // left
			{  0.0f,  1.0f, 1.0f, 1.0f },   // top
			{  0.0f, -1.0f, 1.0f, 1.0f },   // bottom

			{ 0.0f, 0.0f, 0.0f, 1.0f },     // near
			{ 0.0f, 0.0f, 1.0f, 1.0f }      // far
		};

		Mtx4 matInverse;
		Mtx4::Inverse( mtxProj, &matInverse );

		// Compute the frustum corners in world space.
		Vec4 Points[6];

		for( size_t i = 0; i < 6; ++i )
		{
			// Transform point.
			matInverse.VectorTransform( &Points[ i ], HomogenousPoints[i] );
		}

		m_vecPosition = Vec4::ZERO;
		m_quatOrientation = Quaternion::IDENTITY;

		// Compute the slopes.
		Points[0] = Points[0] * Vec4::VectorReciprocalRet( Vec4::VectorSplatRet< 2 >( Points[0] ) ); //splat-z
		Points[1] = Points[1] * Vec4::VectorReciprocalRet( Vec4::VectorSplatZRet( Points[1] ) ); //splat-z
		Points[2] = Points[2] * Vec4::VectorReciprocalRet( Vec4::VectorSplatZRet( Points[2] ) ); //splat-z
		Points[3] = Points[3] * Vec4::VectorReciprocalRet( Vec4::VectorSplatZRet( Points[3] ) ); //splat-z
		/*
		Out.RightSlope = DirectX::XMVectorGetX( Points[0] );
		Out.LeftSlope = DirectX::XMVectorGetX( Points[1] );
		Out.TopSlope = DirectX::XMVectorGetY( Points[2] );
		Out.BottomSlope = DirectX::XMVectorGetY( Points[3] );
		*/

		// Compute near and far.
		Points[4] = Points[4] * Vec4::VectorReciprocalRet( Vec4::VectorSplatWRet( Points[4] ) );
		Points[5] = Points[5] * Vec4::VectorReciprocalRet( Vec4::VectorSplatWRet( Points[5] ) );

		m_fRightSlope	= Points[ 0 ].x;
		m_fLeftSlope	= Points[ 1 ].x;
		m_fTopSlope		= Points[ 2 ].y;
		m_fBottomSlope	= Points[ 3 ].y;
		m_fNear			= Points[ 4 ].z;
		m_fFar			= Points[ 5 ].z;


		// Build the frustum planes.
		aPlanes[0].vecPlane.Set( 0.0f, 0.0f, -1.0f, m_fNear );
		aPlanes[1].vecPlane.Set( 0.0f, 0.0f, 1.0f, -m_fFar );
		aPlanes[2].vecPlane.Set( 1.0f, 0.0f, -m_fRightSlope, 0.0f );
		aPlanes[3].vecPlane.Set( -1.0f, 0.0f, m_fLeftSlope, 0.0f );
		aPlanes[4].vecPlane.Set( 0.0f, 1.0f, -m_fTopSlope, 0.0f );
		aPlanes[5].vecPlane.Set( 0.0f, -1.0f, m_fBottomSlope, 0.0f );

		// Normalize the planes so we can compare to the sphere radius.
		aPlanes[ 2 ].vecPlane = Vector3Normalize( aPlanes[ 2 ].vecPlane );
		aPlanes[ 3 ].vecPlane = Vector3Normalize( aPlanes[ 3 ].vecPlane );
		aPlanes[ 4 ].vecPlane = Vector3Normalize( aPlanes[ 4 ].vecPlane );
		aPlanes[ 5 ].vecPlane = Vector3Normalize( aPlanes[ 5 ].vecPlane );

		// Build furstum corners
		Vec4 vRightTop = Vec4( m_fRightSlope, m_fTopSlope, 1.0f, 0.0f );
		Vec4 vRightBottom = Vec4( m_fRightSlope, m_fBottomSlope, 1.0f, 0.0f );
		Vec4 vLeftTop = Vec4( m_fLeftSlope, m_fTopSlope, 1.0f, 0.0f );
		Vec4 vLeftBottom = Vec4( m_fLeftSlope, m_fBottomSlope, 1.0f, 0.0f );
		Vec4 vNear = VectorReplicatePtr( &m_fNear );
		Vec4 vFar = VectorReplicatePtr( &m_fFar );

		Vec4 Corners[8];
		m_aCorners[0] = vRightTop * vNear;
		m_aCorners[1] = vRightBottom * vNear;
		m_aCorners[2] = vLeftTop * vNear;
		m_aCorners[3] = vLeftBottom * vNear;
		m_aCorners[4] = vRightTop * vFar;
		m_aCorners[5] = vRightBottom * vFar;
		m_aCorners[6] = vLeftTop * vFar;
		m_aCorners[7] = vLeftBottom * vFar;
	}

xst_fi Vec4  VectorSwizzle
(
   const Vec4& V,
    u32 E0,
    u32 E1,
    u32 E2,
    u32 E3
)
{
    assert( (E0 < 4) && (E1 < 4) && (E2 < 4) && (E3 < 4) );
    return Vec4( V.a[ E0 ], V.a[ E1 ], V.a[ E2 ], V.a[ E3 ] );
}

xst_fi Vec4 VectorSelect(const Vec4& V1,const Vec4& V2, const Vec4& Control)
{
#if defined(_XM_NO_INTRINSICS_)

    Vec4 Result;
    Result.m128_u32[0] = (V1.m128_u32[0] & ~Control.m128_u32[0]) | (V2.m128_u32[0] & Control.m128_u32[0]);
    Result.m128_u32[1] = (V1.m128_u32[1] & ~Control.m128_u32[1]) | (V2.m128_u32[1] & Control.m128_u32[1]);
    Result.m128_u32[2] = (V1.m128_u32[2] & ~Control.m128_u32[2]) | (V2.m128_u32[2] & Control.m128_u32[2]);
    Result.m128_u32[3] = (V1.m128_u32[3] & ~Control.m128_u32[3]) | (V2.m128_u32[3] & Control.m128_u32[3]);
    return Result;

#elif defined( XST_SSE2 )
    m128 vTemp1 = _mm_andnot_ps(Control,V1);
    m128 vTemp2 = _mm_and_ps(V2,Control);
    return _mm_or_ps(vTemp1,vTemp2);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

template<class _VEC4_OR_QUAT_>
xst_fi void VectorSelect(_VEC4_OR_QUAT_* pOut, const Vec4& V1,const Vec4& V2, const Vec4& Control)
{
#if defined(_XM_NO_INTRINSICS_)

    Vec4 Result;
    Result.m128_u32[0] = (V1.m128_u32[0] & ~Control.m128_u32[0]) | (V2.m128_u32[0] & Control.m128_u32[0]);
    Result.m128_u32[1] = (V1.m128_u32[1] & ~Control.m128_u32[1]) | (V2.m128_u32[1] & Control.m128_u32[1]);
    Result.m128_u32[2] = (V1.m128_u32[2] & ~Control.m128_u32[2]) | (V2.m128_u32[2] & Control.m128_u32[2]);
    Result.m128_u32[3] = (V1.m128_u32[3] & ~Control.m128_u32[3]) | (V2.m128_u32[3] & Control.m128_u32[3]);
    pVecOut->v = Result.v;

#elif defined( XST_SSE2 )
    m128 vTemp1 = _mm_andnot_ps(Control.v,V1.v);
    m128 vTemp2 = _mm_and_ps(V2.v,Control.v);
    pOut->v = _mm_or_ps(vTemp1,vTemp2);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

const u32 XSE_SELECT_0          = 0x00000000;
const u32 XSE_SELECT_1          = 0xFFFFFFFF;

XST_ALIGN(16) struct VectorI32
{
    union
    {
        i32 i[4];
        m128 v;
    };

    xst_fi operator Vec4() const { return v; }
#if defined( XST_SSE2 )
    xst_fi operator __m128i() const { return _mm_castps_si128(v); }
    xst_fi operator __m128d() const { return _mm_castps_pd(v); }
#endif
};

static const VectorI32 SEL_1110 = { XSE_SELECT_1, XSE_SELECT_1, XSE_SELECT_1, XSE_SELECT_0 };
static const Vec4 SELECT_1110( SEL_1110 );

xst_fi Quaternion   Vector3InverseRotate(const Vec4& V, const Quaternion& RotationQuaternion)
{
	Quaternion A = VectorSelect( SELECT_1110, V, SELECT_1110 );
    Quaternion Result = RotationQuaternion * A;
    Quaternion Q = Quaternion::ConjugateRet( RotationQuaternion );
    return Result * Q;
}

template<class _VEC4_OR_QUAT_>
xst_fi void QuaternionMultiply(_VEC4_OR_QUAT_* pOut, const _VEC4_OR_QUAT_& Q1, const _VEC4_OR_QUAT_& Q2);

xst_fi void Vector3InverseRotate(Vec4* pVecOut, const Vec4& V, const Quaternion& RotationQuaternion)
{
	Quaternion A, Result, Q;
	VectorSelect( &A, SELECT_1110, V, SELECT_1110 );
	QuaternionMultiply( &Result, RotationQuaternion, A );
	Quaternion::Conjugate( &Q, RotationQuaternion );
	QuaternionMultiply( &A, Result, Q );
	pVecOut->v = A.v;
}

template<u32 SwizzleX, u32 SwizzleY, u32 SwizzleZ, u32 SwizzleW>
xst_fi Vec4       VectorSwizzle(const Vec4& V)
{
    static_assert(SwizzleX <= 3, "SwizzleX template parameter out of range");
    static_assert(SwizzleY <= 3, "SwizzleY template parameter out of range");
    static_assert(SwizzleZ <= 3, "SwizzleZ template parameter out of range");
    static_assert(SwizzleW <= 3, "SwizzleW template parameter out of range");

#if defined( XST_SSE2 )
    return XST_SSE_PERMUTE_PS( V.v, _MM_SHUFFLE( SwizzleW, SwizzleZ, SwizzleY, SwizzleX ) );
#else
    return VectorSwizzle( V, SwizzleX, SwizzleY, SwizzleZ, SwizzleW );
#endif
}

template<u32 SwizzleX, u32 SwizzleY, u32 SwizzleZ, u32 SwizzleW>
xst_fi void VectorSwizzle(Vec4* pVecOut, const Vec4& V)
{
    static_assert(SwizzleX <= 3, "SwizzleX template parameter out of range");
    static_assert(SwizzleY <= 3, "SwizzleY template parameter out of range");
    static_assert(SwizzleZ <= 3, "SwizzleZ template parameter out of range");
    static_assert(SwizzleW <= 3, "SwizzleW template parameter out of range");

#if defined( XST_SSE2 )
    pVecOut->v = XST_SSE_PERMUTE_PS( V.v, _MM_SHUFFLE( SwizzleW, SwizzleZ, SwizzleY, SwizzleX ) );
#else
    return VectorSwizzle( V, SwizzleX, SwizzleY, SwizzleZ, SwizzleW );
#endif
}



xst_fi Vec4 VectorSelectControl(u32 VectorIndex0, u32 VectorIndex1, u32 VectorIndex2, u32 VectorIndex3)
{
#if defined( XST_SSE2 )
    // x=Index0,y=Index1,z=Index2,w=Index3
    __m128i vTemp = _mm_set_epi32(VectorIndex3,VectorIndex2,VectorIndex1,VectorIndex0);
    // Any non-zero entries become 0xFFFFFFFF else 0
    vTemp = _mm_cmpgt_epi32(vTemp,_mm_castps_si128(Vec4::ZERO.v));
    return _mm_castsi128_ps(vTemp);
#else
    Vec4    ControlVector;
    const u32  ControlElement[] =
                {
                    XSE_SELECT_0,
                    XSE_SELECT_1
                };

    assert(VectorIndex0 < 2);
    assert(VectorIndex1 < 2);
    assert(VectorIndex2 < 2);
    assert(VectorIndex3 < 2);

    ControlVector.x = ControlElement[VectorIndex0];
    ControlVector.y = ControlElement[VectorIndex1];
    ControlVector.z = ControlElement[VectorIndex2];
    ControlVector.w = ControlElement[VectorIndex3];

    return ControlVector;

#endif
}

	template<u32 Elements>
    xst_fi Vec4      VectorRotateLeft(const Vec4& V)
{
    static_assert( Elements < 4, "Elements template parameter out of range" );
    return VectorSwizzle<Elements & 3, (Elements + 1) & 3, (Elements + 2) & 3, (Elements + 3) & 3>(V);
}

template<u32 Elements>
    xst_fi Vec4      VectorRotateRight(const Vec4& V)
{
    static_assert( Elements < 4, "Elements template parameter out of range" );
    return VectorSwizzle<(4 - Elements) & 3, (5 - Elements) & 3, (6 - Elements) & 3, (7 - Elements) & 3>(V);
}

template<u32 VSLeftRotateElements, u32 Select0, u32 Select1, u32 Select2, u32 Select3> 
xst_fi Vec4    VectorInsert(const Vec4& VD, const Vec4& VS)
{
    Vec4 Control = VectorSelectControl(Select0&1, Select1&1, Select2&1, Select3&1);
    return VectorSelect( VD, VectorRotateLeft<VSLeftRotateElements>(VS), Control );
}

xst_fi Vec4  Vector4Dot(const Vec4& V1, const Vec4& V2)
{
#if defined(_XM_NO_INTRINSICS_)

    Vec4 Result;
    Result.m128_f32[0] =
    Result.m128_f32[1] =
    Result.m128_f32[2] =
    Result.m128_f32[3] = V1.m128_f32[0] * V2.m128_f32[0] + V1.m128_f32[1] * V2.m128_f32[1] + V1.m128_f32[2] * V2.m128_f32[2] + V1.m128_f32[3] * V2.m128_f32[3];
    return Result;
#elif defined(XST_SSE2)
    m128 vTemp2 = V2.v;
    m128 vTemp = _mm_mul_ps(V1.v,vTemp2);
    vTemp2 = _mm_shuffle_ps(vTemp2,vTemp,_MM_SHUFFLE(1,0,0,0)); // Copy X to the Z position and Y to the W position
    vTemp2 = _mm_add_ps(vTemp2,vTemp);          // Add Z = X+Z; W = Y+W;
    vTemp = _mm_shuffle_ps(vTemp,vTemp2,_MM_SHUFFLE(0,3,0,0));  // Copy W to the Z position
    vTemp = _mm_add_ps(vTemp,vTemp2);           // Add Z and W together
    return XST_SSE_PERMUTE_PS(vTemp,_MM_SHUFFLE(2,2,2,2));    // Splat Z and return
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}
xst_fi Vec4  VectorOrInt(const Vec4& V1,const Vec4& V2)
	{
#if defined( XST_SSE2 )
		__m128i V = _mm_or_si128( _mm_castps_si128( V1.v ), _mm_castps_si128( V2.v ) );
		return _mm_castsi128_ps( V );
#else
Vec4 Result;
Result.m128_u32[ 0 ] = V1.m128_u32[ 0 ] | V2.m128_u32[ 0 ];
Result.m128_u32[ 1 ] = V1.m128_u32[ 1 ] | V2.m128_u32[ 1 ];
Result.m128_u32[ 2 ] = V1.m128_u32[ 2 ] | V2.m128_u32[ 2 ];
Result.m128_u32[ 3 ] = V1.m128_u32[ 3 ] | V2.m128_u32[ 3 ];
return Result;

#endif // _XM_VMX128_INTRINSICS_
	}

xst_fi Vec4  VectorAndInt(const Vec4& V1,const Vec4& V2)
{
#if defined(_XM_NO_INTRINSICS_)

    Vec4 Result;
    Result.m128_u32[0] = V1.m128_u32[0] & V2.m128_u32[0];
    Result.m128_u32[1] = V1.m128_u32[1] & V2.m128_u32[1];
    Result.m128_u32[2] = V1.m128_u32[2] & V2.m128_u32[2];
    Result.m128_u32[3] = V1.m128_u32[3] & V2.m128_u32[3];
    return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
    return vandq_u32(V1,V2);
#elif defined(XST_SSE2)
    return _mm_and_ps(V1.v,V2.v);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

xst_fi Vec4  VectorGreater(const Vec4& V1, const Vec4& V2)
{
#if defined(_XM_NO_INTRINSICS_)

    Vec4 Control;
    Control.m128_u32[0] = (V1.m128_f32[0] > V2.m128_f32[0]) ? 0xFFFFFFFF : 0;
    Control.m128_u32[1] = (V1.m128_f32[1] > V2.m128_f32[1]) ? 0xFFFFFFFF : 0;
    Control.m128_u32[2] = (V1.m128_f32[2] > V2.m128_f32[2]) ? 0xFFFFFFFF : 0;
    Control.m128_u32[3] = (V1.m128_f32[3] > V2.m128_f32[3]) ? 0xFFFFFFFF : 0;
    return Control;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
    return vcgtq_f32( V1, V2 );
#elif defined(XST_SSE2)
    return _mm_cmpgt_ps( V1.v, V2.v );
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

xst_fi Vec4  VectorLessOrEqual(const Vec4& V1, const Vec4& V2)
{
#if defined(_XM_NO_INTRINSICS_)

    Vec4 Control;
    Control.m128_u32[0] = (V1.m128_f32[0] <= V2.m128_f32[0]) ? 0xFFFFFFFF : 0;
    Control.m128_u32[1] = (V1.m128_f32[1] <= V2.m128_f32[1]) ? 0xFFFFFFFF : 0;
    Control.m128_u32[2] = (V1.m128_f32[2] <= V2.m128_f32[2]) ? 0xFFFFFFFF : 0;
    Control.m128_u32[3] = (V1.m128_f32[3] <= V2.m128_f32[3]) ? 0xFFFFFFFF : 0;
    return Control;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
    return vcleq_f32( V1, V2 );
#elif defined(XST_SSE2)
    return _mm_cmple_ps( V1.v, V2.v );
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

xst_fi Vec4  VectorTrueInt()
{
#if defined(_XM_NO_INTRINSICS_)
    XMVECTORU32 vResult = {0xFFFFFFFFU,0xFFFFFFFFU,0xFFFFFFFFU,0xFFFFFFFFU};
    return vResult;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
    return vdupq_n_s32(-1);
#elif defined(XST_SSE2)
    __m128i V = _mm_set1_epi32(-1);
    return _mm_castsi128_ps(V);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

const u32 XSE_SSE_CRMASK_CR6TRUE    = 0x00000080;
const u32 XSE_SSE_CRMASK_CR6FALSE   = 0x00000020;
xst_fi bool ComparisonAllTrue(u32 CR) { return (((CR) & XSE_SSE_CRMASK_CR6TRUE) == XSE_SSE_CRMASK_CR6TRUE); }
static const Vec4 TRUE_INT = VectorTrueInt();

xst_fi u32  Vector4EqualIntR(const Vec4& V1, const Vec4& V2)
{
#if defined(_XM_NO_INTRINSICS_)
    u32 CR = 0;
    if (V1.m128_u32[0] == V2.m128_u32[0] && 
        V1.m128_u32[1] == V2.m128_u32[1] &&
        V1.m128_u32[2] == V2.m128_u32[2] &&
        V1.m128_u32[3] == V2.m128_u32[3])
    {
        CR = XM_CRMASK_CR6TRUE;
    }
    else if (V1.m128_u32[0] != V2.m128_u32[0] && 
        V1.m128_u32[1] != V2.m128_u32[1] &&
        V1.m128_u32[2] != V2.m128_u32[2] &&
        V1.m128_u32[3] != V2.m128_u32[3])
    {
        CR = XM_CRMASK_CR6FALSE;
    }
    return CR;
#elif defined(XST_SSE2)
    __m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1.v),_mm_castps_si128(V2.v));
    int iTest = _mm_movemask_ps(_mm_castsi128_ps(vTemp));
    u32 CR = 0;
    if (iTest==0xf)     // All equal?
    {
        CR = XSE_SSE_CRMASK_CR6TRUE;
    }
    else if (iTest==0)  // All not equal?
    {
        CR = XSE_SSE_CRMASK_CR6FALSE;
    }
    return CR;
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

xst_fi bool  Vector4EqualInt(const Vec4& V1, const Vec4& V2)
{
#if defined(_XM_NO_INTRINSICS_)
    return (((V1.m128_u32[0] == V2.m128_u32[0]) && (V1.m128_u32[1] == V2.m128_u32[1]) && (V1.m128_u32[2] == V2.m128_u32[2]) && (V1.m128_u32[3] == V2.m128_u32[3])) != 0);
#elif defined(XST_SSE2)
    __m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1.v),_mm_castps_si128(V2.v));
    return ((_mm_movemask_ps(_mm_castsi128_ps(vTemp))==0xf) != 0);
#else
    return ComparisonAllTrue(Vector4EqualIntR(V1, V2));
#endif
}

xst_fi Vec4 VectorFalseInt()
{
#if defined(_XM_NO_INTRINSICS_)
    Vec4 vResult = {0.0f,0.0f,0.0f,0.0f};
    return vResult;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
    return vdupq_n_u32(0);
#elif defined(XST_SSE2)
    return _mm_setzero_ps();
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

static const Vec4 FALSE_INT = VectorFalseInt();

xst_fi m128 VectorFalseInt2()
{
#if defined(_XM_NO_INTRINSICS_)
    m128 vResult = {0.0f,0.0f,0.0f,0.0f};
    return vResult;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
    return vdupq_n_u32(0);
#elif defined(XST_SSE2)
    return _mm_setzero_ps();
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}



xst_fi m128 VectorReplicatePtr2(const float *pValue)
{
#if defined(_XM_NO_INTRINSICS_) || defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
    float Value = pValue[0];
    XMVECTORF32 vResult = {Value,Value,Value,Value};
    return vResult;
#elif defined(XST_SSE2)
    return _mm_load_ps1( pValue );
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

static const Vec4 MASK3( 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 );

xst_fi Vec4  Vector3Dot(const Vec4& V1, const Vec4& V2)
{
#if defined(_XM_NO_INTRINSICS_)
    float fValue = V1.m128_f32[0] * V2.m128_f32[0] + V1.m128_f32[1] * V2.m128_f32[1] + V1.m128_f32[2] * V2.m128_f32[2];
    Vec4 vResult = {
        fValue,
        fValue,
        fValue,
        fValue
    };            
    return vResult;
#elif defined(XST_SSE2)
    // Perform the dot product
    m128 vDot = _mm_mul_ps(V1.v,V2.v);
    // x=Dot.m128_f32[1], y=Dot.m128_f32[2]
    m128 vTemp = XST_SSE_PERMUTE_PS(vDot,_MM_SHUFFLE(2,1,2,1));
    // Result.m128_f32[0] = x+y
    vDot = _mm_add_ss(vDot,vTemp);
    // x=Dot.m128_f32[2]
    vTemp = XST_SSE_PERMUTE_PS(vTemp,_MM_SHUFFLE(1,1,1,1));
    // Result.m128_f32[0] = (x+y)+z
    vDot = _mm_add_ss(vDot,vTemp);
    // Splat x
    return XST_SSE_PERMUTE_PS(vDot,_MM_SHUFFLE(0,0,0,0));
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

xst_fi void  Vector3Dot(Vec4* pVecOut, const Vec4& V1, const Vec4& V2)
{
#if defined(_XM_NO_INTRINSICS_)
    float fValue = V1.m128_f32[0] * V2.m128_f32[0] + V1.m128_f32[1] * V2.m128_f32[1] + V1.m128_f32[2] * V2.m128_f32[2];
	pVecOut->Set( fValue );
#elif defined(XST_SSE2)
    // Perform the dot product
    m128 vDot = _mm_mul_ps(V1.v,V2.v);
    // x=Dot.m128_f32[1], y=Dot.m128_f32[2]
    m128 vTemp = XST_SSE_PERMUTE_PS(vDot,_MM_SHUFFLE(2,1,2,1));
    // Result.m128_f32[0] = x+y
    vDot = _mm_add_ss(vDot,vTemp);
    // x=Dot.m128_f32[2]
    vTemp = XST_SSE_PERMUTE_PS(vTemp,_MM_SHUFFLE(1,1,1,1));
    // Result.m128_f32[0] = (x+y)+z
    vDot = _mm_add_ss(vDot,vTemp);
    // Splat x
    pVecOut->v = XST_SSE_PERMUTE_PS(vDot,_MM_SHUFFLE(0,0,0,0));
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

xst_fi Vec4  Vector3Cross(const Vec4& V1, const Vec4& V2)
{
    // [ V1.y*V2.z - V1.z*V2.y, V1.z*V2.x - V1.x*V2.z, V1.x*V2.y - V1.y*V2.x ]

#if defined(_XM_NO_INTRINSICS_)
    Vec4 vResult = {
        (V1.m128_f32[1] * V2.m128_f32[2]) - (V1.m128_f32[2] * V2.m128_f32[1]),
        (V1.m128_f32[2] * V2.m128_f32[0]) - (V1.m128_f32[0] * V2.m128_f32[2]),
        (V1.m128_f32[0] * V2.m128_f32[1]) - (V1.m128_f32[1] * V2.m128_f32[0]),
        0.0f
    };
    return vResult;
#elif defined(XST_SSE2)
    // y1,z1,x1,w1
    m128 vTemp1 = XST_SSE_PERMUTE_PS(V1.v,_MM_SHUFFLE(3,0,2,1));
    // z2,x2,y2,w2
    m128 vTemp2 = XST_SSE_PERMUTE_PS(V2.v,_MM_SHUFFLE(3,1,0,2));
    // Perform the left operation
    m128 vResult = _mm_mul_ps(vTemp1,vTemp2);
    // z1,x1,y1,w1
    vTemp1 = XST_SSE_PERMUTE_PS(vTemp1,_MM_SHUFFLE(3,0,2,1));
    // y2,z2,x2,w2
    vTemp2 = XST_SSE_PERMUTE_PS(vTemp2,_MM_SHUFFLE(3,1,0,2));
    // Perform the right operation
    vTemp1 = _mm_mul_ps(vTemp1,vTemp2);
    // Subract the right from left, and return answer
    vResult = _mm_sub_ps(vResult,vTemp1);
    // Set w to zero
    return _mm_and_ps(vResult,MASK3);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

xst_fi Vec4  VectorLess(const Vec4& V1, const Vec4& V2)
{
#if defined(_XM_NO_INTRINSICS_)

    Vec4 Control;
    Control.m128_u32[0] = (V1.m128_f32[0] < V2.m128_f32[0]) ? 0xFFFFFFFF : 0;
    Control.m128_u32[1] = (V1.m128_f32[1] < V2.m128_f32[1]) ? 0xFFFFFFFF : 0;
    Control.m128_u32[2] = (V1.m128_f32[2] < V2.m128_f32[2]) ? 0xFFFFFFFF : 0;
    Control.m128_u32[3] = (V1.m128_f32[3] < V2.m128_f32[3]) ? 0xFFFFFFFF : 0;
    return Control;
#elif defined(XST_SSE2)
    return _mm_cmplt_ps( V1.v, V2.v );
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}


xst_fi void PointOnLineSegmentNearestPoint(Vec4* pVecOut, const Vec4& S1, const Vec4& S2, const Vec4& P)
{
    Vec4 Dir; 
	Dir.Sub( S2, S1 );
    Vec4 Projection = ( Vector3Dot( P, Dir ) - Vector3Dot( S1, Dir ) );
    Vec4 LengthSq = Vector3Dot( Dir, Dir );

    Vec4 t = Projection * Vec4::VectorReciprocalRet( LengthSq );
    Vec4 Point = S1 + t * Dir;

    // t < 0
    Vec4 SelectS1 = VectorLess( Projection, Vec4::ZERO );
    Point = VectorSelect( Point, S1, SelectS1 );

    // t > 1
    Vec4 SelectS2 = VectorGreater( Projection, LengthSq );
    VectorSelect( pVecOut, Point, S2, SelectS2 );
}


xst_fi Vec4  PointOnLineSegmentNearestPoint(const Vec4& S1, const Vec4& S2, const Vec4& P)
{
    Vec4 Dir = S2 - S1;
    Vec4 Projection = ( Vector3Dot( P, Dir ) - Vector3Dot( S1, Dir ) );
    Vec4 LengthSq = Vector3Dot( Dir, Dir );

    Vec4 t = Projection * Vec4::VectorReciprocalRet( LengthSq );
    Vec4 Point = S1 + t * Dir;

    // t < 0
    Vec4 SelectS1 = VectorLess( Projection, Vec4::ZERO );
    Point = VectorSelect( Point, S1, SelectS1 );

    // t > 1
    Vec4 SelectS2 = VectorGreater( Projection, LengthSq );
    Point = VectorSelect( Point, S2, SelectS2 );

    return Point;
}

	bool CFrustum::SphereTest(const Vec4& vecCenter, cf32& fRadius) const
	{

		//assert( DirectX::Internal::XMQuaternionIsUnit( vOrientation ) );

		// Load the sphere.
		Vec4 vCenter = vecCenter;
		Vec4 vRadius = fRadius;

		// Transform the center of the sphere into the local space of frustum.
		Vec4 vecTmpSub;
		vecTmpSub.Sub( vCenter, m_vecPosition );
		Vector3InverseRotate( &vCenter, vecTmpSub, m_quatOrientation );
		//vCenter = Vector3InverseRotate( vCenter - m_vecPosition, m_quatOrientation );

		// Set w of the center to one so we can dot4 with the plane.
		vCenter = VectorInsert<0, 0, 0, 0, 1>( vCenter, Vec4::UNIT );

		// Check against each plane of the frustum.
		Vec4 Outside = VectorFalseInt();
		Vec4 InsideAll = VectorTrueInt();
		Vec4 CenterInsideAll = VectorTrueInt();

		Vec4 Dist[6];

		for( size_t i = 0; i < 6; ++i )
		{
			Dist[i] = Vec4::Dot( vCenter, aPlanes[i].vecPlane );

			// Outside the plane?
			Outside = VectorOrInt( Outside, VectorGreater( Dist[i], vRadius ) );

			// Fully inside the plane?
			InsideAll = VectorAndInt( InsideAll, VectorLessOrEqual( Dist[i], -vRadius ) );

			// Check if the center is inside the plane.
			CenterInsideAll = VectorAndInt( CenterInsideAll, VectorLessOrEqual( Dist[i], Vec4::ZERO ) );
		}

		// If the sphere is outside any of the planes it is outside. 
		if ( Vector4EqualInt( Outside, VectorTrueInt() ) )
			return false;

		// If the sphere is inside all planes it is fully inside.
		if ( Vector4EqualInt( InsideAll, VectorTrueInt() ) )
			return true;

		// If the center of the sphere is inside all planes and the sphere intersects 
		// one or more planes then it must intersect.
		if ( Vector4EqualInt( CenterInsideAll, VectorTrueInt() ) )
			return true;

		// The sphere may be outside the frustum or intersecting the frustum.
		// Find the nearest feature (face, edge, or corner) on the frustum 
		// to the sphere.

		// The faces adjacent to each face are:
		static const size_t adjacent_faces[6][4] =
		{
			{ 2, 3, 4, 5 },    // 0
			{ 2, 3, 4, 5 },    // 1
			{ 0, 1, 4, 5 },    // 2
			{ 0, 1, 4, 5 },    // 3
			{ 0, 1, 2, 3 },    // 4
			{ 0, 1, 2, 3 }
		};  // 5

		Vec4 Intersects = VectorFalseInt();

		// Check to see if the nearest feature is one of the planes.
		for( size_t i = 0; i < 6; ++i )
		{
			// Find the nearest point on the plane to the center of the sphere.
			Vec4 Point = vCenter - (aPlanes[i].vecPlane * Dist[i]);

			// Set w of the point to one.
			Point = VectorInsert<0, 0, 0, 0, 1>( Point, Vec4::UNIT );
        
			// If the point is inside the face (inside the adjacent planes) then
			// this plane is the nearest feature.
			Vec4 InsideFace = VectorTrueInt();
        
			for ( size_t j = 0; j < 4; j++ )
			{
				size_t plane_index = adjacent_faces[i][j];

				InsideFace = VectorAndInt( InsideFace,
							   VectorLessOrEqual( Vec4::Dot( Point, aPlanes[plane_index].vecPlane ), Vec4::ZERO ) );
			}
     
			// Since we have already checked distance from the plane we know that the
			// sphere must intersect if this plane is the nearest feature.
			Intersects = VectorOrInt( Intersects, 
										VectorAndInt( VectorGreater( Dist[i], Vec4::ZERO ), InsideFace ) );
		}

		if ( Vector4EqualInt( Intersects, VectorTrueInt() ) )
			return true;

		// The Edges are:
		static const size_t edges[12][2] =
		{
			{ 0, 1 }, { 2, 3 }, { 0, 2 }, { 1, 3 },    // Near plane
			{ 4, 5 }, { 6, 7 }, { 4, 6 }, { 5, 7 },    // Far plane
			{ 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 },
		}; // Near to far

		Vec4 RadiusSq = vRadius * vRadius;

		// Check to see if the nearest feature is one of the edges (or corners).
		for( size_t i = 0; i < 12; ++i )
		{
			size_t ei0 = edges[i][0];
			size_t ei1 = edges[i][1];

			// Find the nearest point on the edge to the center of the sphere.
			// The corners of the frustum are included as the endpoints of the edges.
			Vec4 Point = PointOnLineSegmentNearestPoint( m_aCorners[ei0], m_aCorners[ei1], vCenter );

			Vec4 Delta = vCenter - Point;

			Vec4 DistSq = Vec4::Dot( Delta, Delta );

			// If the distance to the center of the sphere to the point is less than 
			// the radius of the sphere then it must intersect.
			Intersects = VectorOrInt( Intersects, VectorLessOrEqual( DistSq, RadiusSq ) );
		}

		if ( Vector4EqualInt( Intersects, VectorTrueInt() ) )
			return true;

		// The sphere must be outside the frustum.
		return false;
	}

	bool CFrustum::PointTest(const Vec3& vecPoint) const
	{
		return false;
	}

	bool CFrustum::PointTest(const Vec4& vecPoint) const
	{
		return false;
	}

	bool CFrustum::AABBTest(const XST::CAABB& AABB) const
	{
		return false;
	}

	static const Vec4 IDENTITY_R3( 0.0f, 0.0f, 0.0f, 1.0f );

xst_fi Vec4 Vector4Length(const Vec4& V)
{
#if defined(_XM_NO_INTRINSICS_) 

    Vec4 Result;

    Result = XMVector4LengthSq(V);
    Result = XMVectorSqrt(Result);

    return Result;
#elif defined(XST_SSE2)
    // Perform the dot product on x,y,z and w
    Vec4 vLengthSq = _mm_mul_ps(V,V);
    // vTemp has z and w
    Vec4 vTemp = XST_SSE_PERMUTE_PS(vLengthSq,_MM_SHUFFLE(3,2,3,2));
    // x+z, y+w
    vLengthSq = _mm_add_ps(vLengthSq,vTemp);
    // x+z,x+z,x+z,y+w
    vLengthSq = XST_SSE_PERMUTE_PS(vLengthSq,_MM_SHUFFLE(1,0,0,0));
    // ??,??,y+w,y+w
    vTemp = _mm_shuffle_ps(vTemp,vLengthSq,_MM_SHUFFLE(3,3,0,0));
    // ??,??,x+z+y+w,??
    vLengthSq = _mm_add_ps(vLengthSq,vTemp);
    // Splat the length
    vLengthSq = XST_SSE_PERMUTE_PS(vLengthSq,_MM_SHUFFLE(2,2,2,2));
    // Prepare for the division
    vLengthSq = _mm_sqrt_ps(vLengthSq);
    return vLengthSq;
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

xst_fi Vec4 QuaternionRotationMatrix(const Mtx4& M)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTORF32 q;
    float r22 = M.m[2][2];
    if (r22 <= 0.f)  // x^2 + y^2 >= z^2 + w^2
    {
        float dif10 = M.m[1][1] - M.m[0][0];
        float omr22 = 1.f - r22;
        if (dif10 <= 0.f)  // x^2 >= y^2
        {
            float fourXSqr = omr22 - dif10;
            float inv4x = 0.5f / sqrtf(fourXSqr);
            q.f[0] = fourXSqr*inv4x;
            q.f[1] = (M.m[0][1] + M.m[1][0])*inv4x;
            q.f[2] = (M.m[0][2] + M.m[2][0])*inv4x;
            q.f[3] = (M.m[1][2] - M.m[2][1])*inv4x;
        }
        else  // y^2 >= x^2
        {
            float fourYSqr = omr22 + dif10;
            float inv4y = 0.5f / sqrtf(fourYSqr);
            q.f[0] = (M.m[0][1] + M.m[1][0])*inv4y;
            q.f[1] = fourYSqr*inv4y;
            q.f[2] = (M.m[1][2] + M.m[2][1])*inv4y;
            q.f[3] = (M.m[2][0] - M.m[0][2])*inv4y;
        }
    }
    else  // z^2 + w^2 >= x^2 + y^2
    {
        float sum10 = M.m[1][1] + M.m[0][0];
        float opr22 = 1.f + r22;
        if (sum10 <= 0.f)  // z^2 >= w^2
        {
            float fourZSqr = opr22 - sum10;
            float inv4z = 0.5f / sqrtf(fourZSqr);
            q.f[0] = (M.m[0][2] + M.m[2][0])*inv4z;
            q.f[1] = (M.m[1][2] + M.m[2][1])*inv4z;
            q.f[2] = fourZSqr*inv4z;
            q.f[3] = (M.m[0][1] - M.m[1][0])*inv4z;
        }
        else  // w^2 >= z^2
        {
            float fourWSqr = opr22 + sum10;
            float inv4w = 0.5f / sqrtf(fourWSqr);
            q.f[0] = (M.m[1][2] - M.m[2][1])*inv4w;
            q.f[1] = (M.m[2][0] - M.m[0][2])*inv4w;
            q.f[2] = (M.m[0][1] - M.m[1][0])*inv4w;
            q.f[3] = fourWSqr*inv4w;
        }
    }
    return q;
#elif defined(XST_SSE2)
    static const m128 XMPMMP = {+1.0f, -1.0f, -1.0f, +1.0f};
    static const m128 XMMPMP = {-1.0f, +1.0f, -1.0f, +1.0f};
    static const m128 XMMMPP = {-1.0f, -1.0f, +1.0f, +1.0f}; 

    const m128 r0 = M.r[0];  // (r00, r01, r02, 0)
    const m128 r1 = M.r[1];  // (r10, r11, r12, 0)
    const m128 r2 = M.r[2];  // (r20, r21, r22, 0)

    // (r00, r00, r00, r00)
    m128 r00 = XST_SSE_PERMUTE_PS(r0, _MM_SHUFFLE(0,0,0,0));
    // (r11, r11, r11, r11)
    m128 r11 = XST_SSE_PERMUTE_PS(r1, _MM_SHUFFLE(1,1,1,1));
    // (r22, r22, r22, r22)
    m128 r22 = XST_SSE_PERMUTE_PS(r2, _MM_SHUFFLE(2,2,2,2));

    // x^2 >= y^2 equivalent to r11 - r00 <= 0
    // (r11 - r00, r11 - r00, r11 - r00, r11 - r00)
    m128 r11mr00 = _mm_sub_ps(r11, r00);
    m128 x2gey2 = _mm_cmple_ps(r11mr00, Vec4::ZERO.v);

    // z^2 >= w^2 equivalent to r11 + r00 <= 0
    // (r11 + r00, r11 + r00, r11 + r00, r11 + r00)
    m128 r11pr00 = _mm_add_ps(r11, r00);
    m128 z2gew2 = _mm_cmple_ps(r11pr00, Vec4::ZERO.v);

    // x^2 + y^2 >= z^2 + w^2 equivalent to r22 <= 0
    m128 x2py2gez2pw2 = _mm_cmple_ps(r22, Vec4::ZERO.v);

    // (+r00, -r00, -r00, +r00)
    m128 t0 = _mm_mul_ps(XMPMMP, r00);

    // (-r11, +r11, -r11, +r11)
    m128 t1 = _mm_mul_ps(XMMPMP, r11);

    // (-r22, -r22, +r22, +r22)
    m128 t2 = _mm_mul_ps(XMMMPP, r22);

    // (4*x^2, 4*y^2, 4*z^2, 4*w^2)
    m128 x2y2z2w2 = _mm_add_ps(t0, t1);
    x2y2z2w2 = _mm_add_ps(t2, x2y2z2w2);
    x2y2z2w2 = _mm_add_ps(x2y2z2w2, Vec4::UNIT.v);

    // (r01, r02, r12, r11)
    t0 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(1,2,2,1));
    // (r10, r10, r20, r21)
    t1 = _mm_shuffle_ps(r1, r2, _MM_SHUFFLE(1,0,0,0));
    // (r10, r20, r21, r10)
    t1 = XST_SSE_PERMUTE_PS(t1, _MM_SHUFFLE(1,3,2,0));
    // (4*x*y, 4*x*z, 4*y*z, unused)
    Vec4 xyxzyz = _mm_add_ps(t0, t1);

    // (r21, r20, r10, r10)
    t0 = _mm_shuffle_ps(r2, r1, _MM_SHUFFLE(0,0,0,1));
    // (r12, r12, r02, r01)
    t1 = _mm_shuffle_ps(r1, r0, _MM_SHUFFLE(1,2,2,2));
    // (r12, r02, r01, r12)
    t1 = XST_SSE_PERMUTE_PS(t1, _MM_SHUFFLE(1,3,2,0));
    // (4*x*w, 4*y*w, 4*z*w, unused)
    m128 xwywzw = _mm_sub_ps(t0, t1);
    xwywzw = _mm_mul_ps(XMMPMP, xwywzw);

    // (4*x^2, 4*y^2, 4*x*y, unused)
    t0 = _mm_shuffle_ps(x2y2z2w2, xyxzyz, _MM_SHUFFLE(0,0,1,0));
    // (4*z^2, 4*w^2, 4*z*w, unused)
    t1 = _mm_shuffle_ps(x2y2z2w2, xwywzw, _MM_SHUFFLE(0,2,3,2));
    // (4*x*z, 4*y*z, 4*x*w, 4*y*w)
    t2 = _mm_shuffle_ps(xyxzyz, xwywzw, _MM_SHUFFLE(1,0,2,1));

    // (4*x*x, 4*x*y, 4*x*z, 4*x*w)
    m128 tensor0 = _mm_shuffle_ps(t0, t2, _MM_SHUFFLE(2,0,2,0));
    // (4*y*x, 4*y*y, 4*y*z, 4*y*w)
    m128 tensor1 = _mm_shuffle_ps(t0, t2, _MM_SHUFFLE(3,1,1,2));
    // (4*z*x, 4*z*y, 4*z*z, 4*z*w)
    m128 tensor2 = _mm_shuffle_ps(t2, t1, _MM_SHUFFLE(2,0,1,0));
    // (4*w*x, 4*w*y, 4*w*z, 4*w*w)
    m128 tensor3 = _mm_shuffle_ps(t2, t1, _MM_SHUFFLE(1,2,3,2));

    // Select the row of the tensor-product matrix that has the largest
    // magnitude.
    t0 = _mm_and_ps(x2gey2, tensor0);
    t1 = _mm_andnot_ps(x2gey2, tensor1);
    t0 = _mm_or_ps(t0, t1);
    t1 = _mm_and_ps(z2gew2, tensor2);
    t2 = _mm_andnot_ps(z2gew2, tensor3);
    t1 = _mm_or_ps(t1, t2);
    t0 = _mm_and_ps(x2py2gez2pw2, t0);
    t1 = _mm_andnot_ps(x2py2gez2pw2, t1);
    t2 = _mm_or_ps(t0, t1);

    // Normalize the row.  No division by zero is possible because the
    // quaternion is unit-length (and the row is a nonzero multiple of
    // the quaternion).
    t0 = Vector4Length(t2);
    return _mm_div_ps(t2, t0);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}



template<class _VEC4_OR_QUAT_>
xst_fi void QuaternionMultiply(_VEC4_OR_QUAT_* pOut, const _VEC4_OR_QUAT_& Q1, const _VEC4_OR_QUAT_& Q2)
{
#if defined(_XM_NO_INTRINSICS_)
    Quaternion Result = {
        (Q2.m128_f32[3] * Q1.m128_f32[0]) + (Q2.m128_f32[0] * Q1.m128_f32[3]) + (Q2.m128_f32[1] * Q1.m128_f32[2]) - (Q2.m128_f32[2] * Q1.m128_f32[1]),
        (Q2.m128_f32[3] * Q1.m128_f32[1]) - (Q2.m128_f32[0] * Q1.m128_f32[2]) + (Q2.m128_f32[1] * Q1.m128_f32[3]) + (Q2.m128_f32[2] * Q1.m128_f32[0]),
        (Q2.m128_f32[3] * Q1.m128_f32[2]) + (Q2.m128_f32[0] * Q1.m128_f32[1]) - (Q2.m128_f32[1] * Q1.m128_f32[0]) + (Q2.m128_f32[2] * Q1.m128_f32[3]),
        (Q2.m128_f32[3] * Q1.m128_f32[3]) - (Q2.m128_f32[0] * Q1.m128_f32[0]) - (Q2.m128_f32[1] * Q1.m128_f32[1]) - (Q2.m128_f32[2] * Q1.m128_f32[2]) };
    pQuatOut->v = Result->v;
#elif defined(XST_SSE2)
    static const m128 ControlWZYX = { 1.0f,-1.0f, 1.0f,-1.0f};
    static const m128 ControlZWXY = { 1.0f, 1.0f,-1.0f,-1.0f};
    static const m128 ControlYXWZ = {-1.0f, 1.0f, 1.0f,-1.0f};
    // Copy to SSE registers and use as few as possible for x86
    m128 Q2X = Q2.v;
    m128 Q2Y = Q2.v;
    m128 Q2Z = Q2.v;
    m128 vResult = Q2.v;
	m128 Q1Shuffle = Q1.v;
    // Splat with one instruction
    vResult = XST_SSE_PERMUTE_PS(vResult,_MM_SHUFFLE(3,3,3,3));
    Q2X = XST_SSE_PERMUTE_PS(Q2X,_MM_SHUFFLE(0,0,0,0));
    Q2Y = XST_SSE_PERMUTE_PS(Q2Y,_MM_SHUFFLE(1,1,1,1));
    Q2Z = XST_SSE_PERMUTE_PS(Q2Z,_MM_SHUFFLE(2,2,2,2));
    // Retire Q1 and perform Q1*Q2W
    vResult = _mm_mul_ps(vResult,Q1Shuffle);
    
    // Shuffle the copies of Q1
    Q1Shuffle = XST_SSE_PERMUTE_PS(Q1Shuffle,_MM_SHUFFLE(0,1,2,3));
    // Mul by Q1WZYX
    Q2X = _mm_mul_ps(Q2X,Q1Shuffle);
    Q1Shuffle = XST_SSE_PERMUTE_PS(Q1Shuffle,_MM_SHUFFLE(2,3,0,1));
    // Flip the signs on y and z
    Q2X = _mm_mul_ps(Q2X,ControlWZYX);
    // Mul by Q1ZWXY
    Q2Y = _mm_mul_ps(Q2Y,Q1Shuffle);
    Q1Shuffle = XST_SSE_PERMUTE_PS(Q1Shuffle,_MM_SHUFFLE(0,1,2,3));
    // Flip the signs on z and w
    Q2Y = _mm_mul_ps(Q2Y,ControlZWXY);
    // Mul by Q1YXWZ
    Q2Z = _mm_mul_ps(Q2Z,Q1Shuffle);
    vResult = _mm_add_ps(vResult,Q2X);
    // Flip the signs on x and w
    Q2Z = _mm_mul_ps(Q2Z,ControlYXWZ);
    Q2Y = _mm_add_ps(Q2Y,Q2Z);
    pOut->v = _mm_add_ps(vResult,Q2Y);
    //pOut->v = vResult;
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

template<class _VEC4_OR_QUAT_>
xst_fi _VEC4_OR_QUAT_ QuaternionMultiply(const _VEC4_OR_QUAT_& Q1, const _VEC4_OR_QUAT_& Q2)
{
    _VEC4_OR_QUAT_ ret;
	QuaternionMultiply( &ret, Q1, Q2 );
	return ret;
}

xst_fi Vec4 Vector3Transform
(
    const Vec4& V, 
    const Mtx4& M
)
{
#if defined(_XM_NO_INTRINSICS_)

    Vec4 Z = XMVectorSplatZ(V);
    Vec4 Y = XMVectorSplatY(V);
    Vec4 X = XMVectorSplatX(V);

    Vec4 Result = XMVectorMultiplyAdd(Z, M.r[2], M.r[3]);
    Result = XMVectorMultiplyAdd(Y, M.r[1], Result);
    Result = XMVectorMultiplyAdd(X, M.r[0], Result);

    return Result;
#elif defined(XST_SSE2)
    m128 vResult = XST_SSE_PERMUTE_PS(V.v,_MM_SHUFFLE(0,0,0,0));
    vResult = _mm_mul_ps(vResult,M.r[0]);
    m128 vTemp = XST_SSE_PERMUTE_PS(V.v,_MM_SHUFFLE(1,1,1,1));
    vTemp = _mm_mul_ps(vTemp,M.r[1]);
    vResult = _mm_add_ps(vResult,vTemp);
    vTemp = XST_SSE_PERMUTE_PS(V.v,_MM_SHUFFLE(2,2,2,2));
    vTemp = _mm_mul_ps(vTemp,M.r[2]);
    vResult = _mm_add_ps(vResult,vTemp);
    vResult = _mm_add_ps(vResult,M.r[3]);
    return vResult;
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

xst_fi void  StoreFloat3
(
    Vec3* pDestination, 
    const Vec4& V
)
{
    assert(pDestination);
#if defined(_XM_NO_INTRINSICS_)
    pDestination->x = V.m128_f32[0];
    pDestination->y = V.m128_f32[1];
    pDestination->z = V.m128_f32[2];
#elif defined(XST_SSE2)
    const m128 T1 = XST_SSE_PERMUTE_PS(V,_MM_SHUFFLE(1,1,1,1));
    const m128 T2 = XST_SSE_PERMUTE_PS(V,_MM_SHUFFLE(2,2,2,2));
    _mm_store_ss( &pDestination->x, V.v );
    _mm_store_ss( &pDestination->y, T1 );
    _mm_store_ss( &pDestination->z, T2 );
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

xst_fi void StoreFloat4
(
    Vec4* pDestination, 
    const Vec4&  V
)
{
    assert(pDestination);
#if defined(_XM_NO_INTRINSICS_)
    pDestination->x = V.m128_f32[0];
    pDestination->y = V.m128_f32[1];
    pDestination->z = V.m128_f32[2];
    pDestination->w = V.m128_f32[3];
#elif defined(XST_SSE2)
    _mm_storeu_ps( &pDestination->x, V.v );
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

xst_fi void VectorMax(Vec4* pVecOut, const Vec4& V1, const Vec4& V2)
{
#if defined(_XM_NO_INTRINSICS_)

    Vec4 Result;
    Result.m128_f32[0] = (V1.m128_f32[0] > V2.m128_f32[0]) ? V1.m128_f32[0] : V2.m128_f32[0];
    Result.m128_f32[1] = (V1.m128_f32[1] > V2.m128_f32[1]) ? V1.m128_f32[1] : V2.m128_f32[1];
    Result.m128_f32[2] = (V1.m128_f32[2] > V2.m128_f32[2]) ? V1.m128_f32[2] : V2.m128_f32[2];
    Result.m128_f32[3] = (V1.m128_f32[3] > V2.m128_f32[3]) ? V1.m128_f32[3] : V2.m128_f32[3];
    return Result;
#elif defined(XST_SSE2)
    pVecOut->v = _mm_max_ps( V1.v, V2.v );
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

xst_fi Vec4 VectorMax(const Vec4& V1, const Vec4& V2)
{
	Vec4 vecRet;
	VectorMax( &vecRet, V1, V2 );
	return vecRet;
}

	void  CFrustum::Transform(const Mtx4& M)
	{
		// Load the frustum.
		Vec4 vOrigin = m_vecPosition;
		Vec4 vOrientation = m_quatOrientation;

		//assert( DirectX::Internal::XMQuaternionIsUnit( vOrientation ) );

		// Composite the frustum rotation and the transform rotation
		Mtx4 nM;
		nM.r[0] = Vector3Normalize( M.r[0] );
		nM.r[1] = Vector3Normalize( M.r[1] );
		nM.r[2] = Vector3Normalize( M.r[2] );
		nM.r[3] = IDENTITY_R3;
		Vec4 Rotation = QuaternionRotationMatrix( nM );
		vOrientation = QuaternionMultiply( vOrientation, Rotation );

		// Transform the center.
		vOrigin = Vector3Transform( vOrigin, M );

		// Store the frustum.
		Vec3 vecPos;
		Vec4 vecOrient;
		StoreFloat3( &vecPos, vOrigin );
		m_vecPosition.Set( vecPos.x, vecPos.y, vecPos.z, 0.0f );
		StoreFloat4( &vecOrient, vOrientation );
		m_quatOrientation = vecOrient;

		// Scale the near and far distances (the slopes remain the same).
		/*Vec4 dX = Vector3Dot( M.r[0], M.r[0] );
		Vec4 dY = Vector3Dot( M.r[1], M.r[1] );
		Vec4 dZ = Vector3Dot( M.r[2], M.r[2] );*/
		Vec4 dX, dY, dZ;
		Vector3Dot( &dX, M.r[ 0 ], M.r[ 0 ] );
		Vector3Dot( &dY, M.r[ 1 ], M.r[ 1 ] );
		Vector3Dot( &dZ, M.r[ 2 ], M.r[ 2 ] );

		Vec4 max1, max2;
		VectorMax( &max1, dY, dZ );
		VectorMax( &max2, dX, max1 );
		float Scale = XST::Math::Sqrt( max2.x );

		m_fNear *= Scale;
		m_fFar *= Scale;
	}

} // XSE