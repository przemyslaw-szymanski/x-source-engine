#ifndef XST_CMATRIX4_H
#define XST_CMATRIX4_H

#include "XSTMath.h"
#include "XSTCMatrix3.h"
#include "XSTCVector4.h"

namespace XST
{
#define XST_MATH_MTX_MUL(_m128Result, _m128Row, _m128Col, _afRow, _afCol, _iColNum, _afTmpCol) \
	xst_memcpy( &_m128Row, 16, _afRow, 16 ); \
	_afTmpCol[0] = _afCol[0][_iColNum]; _afTmpCol[1] = _afCol[1][_iColNum]; _afTmpCol[2] = _afCol[2][_iColNum]; _afTmpCol[3] = _afCol[3][_iColNum]; \
	xst_memcpy( &_m128Col, 16, _afTmpCol, 16 ); \
	_m128Result = _mm_mul_ps( _m128Row, _m128Col ); \
	_m128Result = _mm_add_ps( _m128Result, _mm_movehl_ps( _m128Result, _m128Result ) ); \
	_m128Result = _mm_add_ps( _m128Result, _mm_shuffle_ps( _m128Result, _m128Result, _MM_SHUFFLE( 3, 2, 1, 1 ) ) );


	namespace Math
	{
		class CMatrix4;
		typedef XST_ALIGN( 64 ) CMatrix4 Mtx4;

		class XST_API XST_ALIGN( 64 ) CMatrix4
		{
			public:

				union
				{
					f32		m[4][4];
					f32		mm[16];
					m128	r[4];
				};

			public:

				static const CMatrix4	IDENTITY;
				static const m128		IDENTITY_R0;
				static const m128		IDENTITY_R1;
				static const m128		IDENTITY_R2;
				static const m128		IDENTITY_R3;

			public:

				xst_fi			CMatrix4() {}
				
				xst_fi			CMatrix4(cf32* aR1, cf32* aR2, cf32* aR3, cf32* aR4)
								{
									SetRow( 0, aR1 );
									SetRow( 1, aR2 );
									SetRow( 2, aR3 );
									SetRow( 3, aR4 );
								}

				xst_fi			CMatrix4(const Vec4& vecR1, const Vec4& vecR2, const Vec4& vecR3, const Vec4& vecR4)
								{
									SetRow( 0, vecR1.v );
									SetRow( 1, vecR2.v );
									SetRow( 2, vecR3.v );
									SetRow( 3, vecR4.v );
								}

				xst_fi			CMatrix4(	cf32& f00, cf32& f01, cf32& f02, cf32& f03, 
											cf32& f10, cf32& f11, cf32& f12, cf32& f13, 
											cf32& f20, cf32& f21, cf32& f22, cf32& f23,
											cf32& f30, cf32& f31, cf32& f32, cf32& f33)
								{
									m[0][0] = f00;
									m[0][1] = f01;
									m[0][2] = f02;
									m[0][3] = f03;
									m[1][0] = f10;
									m[1][1] = f11;
									m[1][2] = f12;
									m[1][3] = f13;
									m[2][0] = f20;
									m[2][1] = f21;
									m[2][2] = f22;
									m[2][3] = f23;
									m[3][0] = f30;
									m[3][1] = f31;
									m[3][2] = f32;
									m[3][3] = f33;
								}

				xst_fi			CMatrix4(cf32** afMtx)
								{
									SetRow( 0, afMtx[ 0 ] );
									SetRow( 1, afMtx[ 1 ] );
									SetRow( 2, afMtx[ 2 ] );
									SetRow( 3, afMtx[ 3 ] );
								}

				xst_fi			CMatrix4(const Mtx4& mtx)
								{
									Set( mtx );
								}

				xst_fi explicit	CMatrix4(const CMatrix3& Mat) 
								{
									Set( Mat );
								}

				virtual			~CMatrix4() {}

				xst_fi	void	Swap(CMatrix4* pMat)
				{
					f32 fTmp;
					XST_SWAP( m[0][0], pMat->m[0][0], fTmp );
					XST_SWAP( m[0][1], pMat->m[0][1], fTmp );
					XST_SWAP( m[0][2], pMat->m[0][2], fTmp );
					XST_SWAP( m[0][3], pMat->m[0][3], fTmp );
					XST_SWAP( m[1][0], pMat->m[1][0], fTmp );
					XST_SWAP( m[1][1], pMat->m[1][1], fTmp );
					XST_SWAP( m[1][2], pMat->m[1][2], fTmp );
					XST_SWAP( m[1][3], pMat->m[1][3], fTmp );
					XST_SWAP( m[2][0], pMat->m[2][0], fTmp );
					XST_SWAP( m[2][1], pMat->m[2][1], fTmp );
					XST_SWAP( m[2][2], pMat->m[2][2], fTmp );
					XST_SWAP( m[2][3], pMat->m[2][3], fTmp );
					XST_SWAP( m[3][0], pMat->m[3][0], fTmp );
					XST_SWAP( m[3][1], pMat->m[3][1], fTmp );
					XST_SWAP( m[3][2], pMat->m[3][2], fTmp );
					XST_SWAP( m[3][3], pMat->m[3][3], fTmp );
				}

				xst_fi	f32*		operator[](cu32& uiRow)
				{
					assert( uiRow < 4 );
					return m[ uiRow ];
				}

				xst_fi	cf32*		operator[](cu32& uiRow) const
				{
					assert( uiRow < 4 );
					return m[ uiRow ];
				}

				xst_fi Mtx4&	operator=(const Mtx4& mtxRight)
				{
					Set( mtxRight );
					return *this;
				}

				xst_fi Mtx4	operator*(const Mtx4& mtxRight) const
				{
					return Mul( *this, mtxRight );
				}

				xst_fi Mtx4&	operator*=(const Mtx4& mtxRight)
				{
					Set( Mul( *this, mtxRight ) );
					return *this;
				}

				xst_fi	void	SetRow(cu32& uiRowId, cf32* afRow)
				{
					//#if defined (XST_SSE2)
					//	r[ uiRowId ] = _mm_loadu_ps( afRow );
					//#else
						m[ uiRowId ][ 0 ] = afRow[ 0 ];
						m[ uiRowId ][ 1 ] = afRow[ 1 ];
						m[ uiRowId ][ 2 ] = afRow[ 2 ];
						m[ uiRowId ][ 3 ] = afRow[ 3 ];
					//#endif
				}

				xst_fi	void	SetRow(cu32& uiRowId, const m128& mRow)
				{
					//#if defined (XST_SSE2)
						//r[ uiRowId ] = mRow;
					//#else
						m[ uiRowId ][ 0 ] = mRow.m128_f32[ 0 ];
						m[ uiRowId ][ 1 ] = mRow.m128_f32[ 1 ];
						m[ uiRowId ][ 2 ] = mRow.m128_f32[ 2 ];
						m[ uiRowId ][ 3 ] = mRow.m128_f32[ 3 ];
					//#endif
				}

				xst_fi	void	SetRow(cu32& uiRowId, const Vec4& vecRow)
				{
					//#if defined (XST_SSE2)
					//	r[ uiRowId ] = vecRow.v;	
					//#else
						m[ uiRowId ][ 0 ] = vecRow.x;
						m[ uiRowId ][ 1 ] = vecRow.y;
						m[ uiRowId ][ 2 ] = vecRow.z;
						m[ uiRowId ][ 3 ] = vecRow.w;
					//#endif
				}

				xst_fi	void	SetRow(cu32& uiRowId, cf32& fX, cf32& fY, cf32& fZ, cf32& fW)
				{
					//#if defined (XST_SSE2)
					//	r[ uiRowId ] = _mm_set_ps( fX, fY, fZ, fW );	
					//#else
						m[ uiRowId ][ 0 ] = fX;
						m[ uiRowId ][ 1 ] = fY;
						m[ uiRowId ][ 2 ] = fZ;
						m[ uiRowId ][ 3 ] = fW;
					//#endif
				}

				xst_fi void Translate(cf32& fOffsetX, cf32& fOffsetY, cf32& fOffsetZ)
				{
					SetRow( 0, IDENTITY_R0 );
					SetRow( 1, IDENTITY_R1);
					SetRow( 2, IDENTITY_R2 );
					SetRow( 3, fOffsetX, fOffsetY, fOffsetZ, 1.0f );
				}

				xst_fi void	Identity()
				{
					SetRow( 0, IDENTITY_R0 );
					SetRow( 1, IDENTITY_R1 );
					SetRow( 2, IDENTITY_R2 );
					SetRow( 3, IDENTITY_R3 );
				}

				xst_i void	RotateX(cf32& fAngle)
				{
					cf32 fSinAngle = sinf( fAngle );
					cf32 fCosAngle = cosf( fAngle );

					#if defined (XST_SSE2)
						Vec4 vecSin( _mm_set_ss( fSinAngle ) );
						Vec4 vecCos( _mm_set_ss( fCosAngle ) );
						vecCos.v = _mm_shuffle_ps( vecCos.v, vecSin.v, _MM_SHUFFLE( 3, 0, 0, 3 ) );
						r[ 0 ] = IDENTITY_R0;
						r[ 1 ] = vecCos.v;
						vecCos.v = _mm_shuffle_ps( vecCos.v, vecCos.v, _MM_SHUFFLE( 3, 1, 2, 0 ) );
						vecCos.v = _mm_mul_ps( vecCos.v, Vec4::NEGATIVE_Y.v );
						r[ 2 ] = vecCos.v;
						r[ 3 ] = IDENTITY_R3;
					#else
						m[0][0] = 1.0f;
						m[0][1] = 0.0f;
						m[0][2] = 0.0f;
						m[0][3] = 0.0f;

						m[1][0] = 0.0f;
						m[1][1] = fCosAngle;
						m[1][2] = fSinAngle;
						m[1][3] = 0.0f;

						m[2][0] = 0.0f;
						m[2][1] = -fSinAngle;
						m[2][2] = fCosAngle;
						m[2][3] = 0.0f;

						m[3][0] = 0.0f;
						m[3][1] = 0.0f;
						m[3][2] = 0.0f;
						m[3][3] = 1.0f;
					#endif
				}

				xst_i void	RotateY(cf32& fAngle)
				{
					cf32 fSinAngle = sinf( fAngle );
					cf32 fCosAngle = cosf( fAngle );

					#if defined (XST_SSE2)
						Vec4 vecSin( _mm_set_ss( fSinAngle ) );
						Vec4 vecCos( _mm_set_ss( fCosAngle ) );
						vecCos.v = _mm_shuffle_ps( vecCos.v, vecSin.v, _MM_SHUFFLE( 3, 0, 3, 0 ) );
						r[ 0 ] = vecSin.v;
						r[ 1 ] = IDENTITY_R1;
						vecCos.v = _mm_shuffle_ps( vecSin.v, vecSin.v, _MM_SHUFFLE( 3, 0, 2, 1 ) );
						vecCos.v = _mm_mul_ps( vecSin.v, Vec4::NEGATIVE_Z.v );
						r[ 2 ] = vecSin.v;
						r[ 3 ] = IDENTITY_R3;
					#else
						m[0][0] = fCosAngle;
						m[0][1] = 0.0f;
						m[0][2] = -fSinAngle;
						m[0][3] = 0.0f;

						m[1][0] = 0.0f;
						m[1][1] = 1.0f;
						m[1][2] = 0.0f;
						m[1][3] = 0.0f;

						m[2][0] = fSinAngle;
						m[2][1] = 0.0f;
						m[2][2] = fCosAngle;
						m[2][3] = 0.0f;

						m[3][0] = 0.0f;
						m[3][1] = 0.0f;
						m[3][2] = 0.0f;
						m[3][3] = 1.0f;
					#endif
				}

				xst_i void	RotateZ(cf32& fAngle)
				{
					cf32 fSinAngle = sinf( fAngle );
					cf32 fCosAngle = cosf( fAngle );

					#if defined (XST_SSE2)
						Vec4 vecSin( _mm_set_ss( fSinAngle ) );
						Vec4 vecCos( _mm_set_ss( fCosAngle ) );
						vecCos.v = _mm_unpacklo_ps( vecCos.v, vecSin.v );
						r[ 0 ] = vecCos.v;
						vecCos.v = _mm_shuffle_ps( vecCos.v, vecCos.v, _MM_SHUFFLE( 3, 2, 0, 1 ) );
						vecCos.v = _mm_mul_ps( vecCos.v, Vec4::NEGATIVE_X.v );
						r[ 1 ] = vecCos.v;
						r[ 2 ] = IDENTITY_R2;
						r[ 3 ] = IDENTITY_R3;
					#else
						m[0][0] = fCosAngle;
						m[0][1] = fSinAngle;
						m[0][2] = 0.0f;
						m[0][3] = 0.0f;

						m[1][0] = -fSinAngle;
						m[1][1] = fCosAngle;
						m[1][2] = 0.0f;
						m[1][3] = 0.0f;

						m[2][0] = 0.0f;
						m[2][1] = 0.0f;
						m[2][2] = 1.0f;
						m[2][3] = 0.0f;

						m[3][0] = 0.0f;
						m[3][1] = 0.0f;
						m[3][2] = 0.0f;
						m[3][3] = 1.0f;
					#endif
				}

				xst_fi void Scale(const Vec3& vecScale)
				{
					SetRow( 0, 0, 0, 0, vecScale.x );
					SetRow( 1, 0, 0, vecScale.y, 0 );
					SetRow( 2, 0, vecScale.z, 0, 0 );
					SetRow( 3, IDENTITY_R3 );

					/*#if defined (XST_SSE2)

						r[0] = _mm_set_ps( 0, 0, 0, ScaleX );
						r[1] = _mm_set_ps( 0, 0, ScaleY, 0 );
						r[2] = _mm_set_ps( 0, ScaleZ, 0, 0 );
						r[3] = g_XMIdentityR3;	

					#else
			
						r[0] = XMVectorSet(ScaleX, 0.0f, 0.0f, 0.0f);
						r[1] = XMVectorSet(0.0f, ScaleY, 0.0f, 0.0f);
						r[2] = XMVectorSet(0.0f, 0.0f, ScaleZ, 0.0f);
						r[3] = g_XMIdentityR3.v;

					#endif*/
				}

				xst_fi	void	GetRow(cu32& uiRowID, CVector3* pVecOut)
				{
					assert( uiRowID < 4 && "CMatrix4::GetRow" );
					pVecOut->x = m[ uiRowID ][ 0 ];
					pVecOut->y = m[ uiRowID ][ 1 ];
					pVecOut->z = m[ uiRowID ][ 2 ];
				}

				xst_fi	CVector3	GetRow3(cu32& uiRowID)
				{
					Vec3 vecTmp;
					GetRow( uiRowID, &vecTmp );
					return vecTmp;
				}

				xst_fi	void	GetRow(cu32& uiRowID, CVector4* pVecOut)
				{
					assert( uiRowID < 4 && "CMatrix4::GetRow" );
					pVecOut->x = m[ uiRowID ][ 0 ];
					pVecOut->y = m[ uiRowID ][ 1 ];
					pVecOut->z = m[ uiRowID ][ 2 ];
					pVecOut->w = m[ uiRowID ][ 4 ];
				}

				xst_fi	CVector4	GetRow4(cu32& uiRowID)
				{
					CVector4 vecTmp;
					GetRow( uiRowID, &vecTmp );
					return vecTmp;
				}

				xst_fi	void	GetCol(cu32& uiColID, CVector3* pVecOut)
				{
					assert( uiColID < 4 && "CMatrix4::GetRow" );
					pVecOut->x = m[ 0 ][ uiColID ];
					pVecOut->y = m[ 1 ][ uiColID ];
					pVecOut->z = m[ 2 ][ uiColID ];
				}

				xst_fi	CVector3	GetCol3(cu32& uiColID)
				{
					Vec3 vecTmp;
					GetCol( uiColID, &vecTmp );
					return vecTmp;
				}

				xst_fi	void	GetCol(cu32& uiColID, CVector4* pVecOut)
				{
					assert( uiColID < 4 && "CMatrix4::GetRow" );
					pVecOut->x = m[ 0 ][ uiColID ];
					pVecOut->y = m[ 1 ][ uiColID ];
					pVecOut->z = m[ 2 ][ uiColID ];
					pVecOut->z = m[ 3 ][ uiColID ];
				}

				xst_fi	CVector4	GetCol4(cu32& uiColID)
				{
					CVector4 vecTmp;
					GetCol( uiColID, &vecTmp );
					return vecTmp;
				}

				void	Rotate(cf32& fAngle, const Vec3& vecAxis);

				void Mul(const CMatrix4& mtxRight);

				void Inverse();

				void Transpose();

				void MulTranspose(const CMatrix4& mtxRight);

				xst_fi void CMatrix4::Set(const Mtx4& mtx)
				{
					xst_memcpy( this->mm, sizeof( this->mm ), mtx.mm, sizeof( mtx.mm ) );
				}

				xst_fi void CMatrix4::Set(const Mtx3& mtx)
				{
					xst_memcpy( this->mm, sizeof( this->mm ), mtx.mm, sizeof( mtx.mm ) );
					this->r[ 0 ].m128_f32[ 3 ] = 0.0f;
					this->r[ 1 ].m128_f32[ 3 ] = 0.0f;
					this->r[ 2 ].m128_f32[ 3 ] = 0.0f;
					this->r[ 3 ].m128_f32[ 0 ] = 0.0f;
					this->r[ 3 ].m128_f32[ 1 ] = 0.0f;
					this->r[ 3 ].m128_f32[ 2 ] = 0.0f;
					this->r[ 3 ].m128_f32[ 3 ] = 1.0f;
				}

				xst_fi void Set(cf32** aafValues)
				{
					xst_memcpy( this->m, sizeof( this->m ), aafValues, sizeof( this->m ) );
				}

				xst_fi void Set(cf32* afValues)
				{
					xst_memcpy( this->mm, sizeof( this->mm ), afValues, sizeof( this->mm ) );
				}

				xst_fi void Set(const Vec4* avecRows)
				{
					xst_memcpy( this->mm, sizeof( this->mm ), avecRows, sizeof( this->mm ) );
				}

				static xst_fi CMatrix4 Inverse(const CMatrix4& mtxMatrix)
				{
					CMatrix4 mtxRet;
					Inverse( mtxMatrix, &mtxRet );
					return mtxRet;
				}
				
				static void Inverse(const CMatrix4& mtxSrc, CMatrix4* pMtxOut);

				static void Mul(const CMatrix4& mtxLeft, const CMatrix4& mtxRight, CMatrix4* pMtxOut);

				static xst_fi CMatrix4 Mul(const CMatrix4& mtxLeft, const CMatrix4& mtxRight)
				{
					CMatrix4 mtxRet;
					Mul( mtxLeft, mtxRight, &mtxRet );
					return mtxRet;
				}

				xst_i void VectorTransform(Vec4* pvecOut, const Vec4& vecVector) const
				{
#if defined(XST_SSE2)
					// Splat x,y,z and w
					Vec4 vTempX = XST_SSE_PERMUTE_PS( vecVector.v,_MM_SHUFFLE(0,0,0,0) );
					Vec4 vTempY = XST_SSE_PERMUTE_PS( vecVector.v,_MM_SHUFFLE(1,1,1,1) );
					Vec4 vTempZ = XST_SSE_PERMUTE_PS( vecVector.v,_MM_SHUFFLE(2,2,2,2) );
					Vec4 vTempW = XST_SSE_PERMUTE_PS( vecVector.v,_MM_SHUFFLE(3,3,3,3) );
					// Mul by the matrix
					vTempX = _mm_mul_ps(vTempX, r[0]);
					vTempY = _mm_mul_ps(vTempY, r[1]);
					vTempZ = _mm_mul_ps(vTempZ, r[2]);
					vTempW = _mm_mul_ps(vTempW, r[3]);
					// Add them all together
					vTempX = _mm_add_ps(vTempX,vTempY);
					vTempZ = _mm_add_ps(vTempZ,vTempW);
					vTempX = _mm_add_ps(vTempX,vTempZ);
					pvecOut->v = vTempX.v;
#else // XST_SSE2
					pvecOut->x = ( m[0][0]*pvecOut->m128_f32[0])+( m[1][0]*pvecOut->m128_f32[1])+( m[2][0]*pvecOut->m128_f32[2])+( m[3][0]*pvecOut->m128_f32[3]);
					pvecOut->y = ( m[0][1]*pvecOut->m128_f32[0])+( m[1][1]*pvecOut->m128_f32[1])+( m[2][1]*pvecOut->m128_f32[2])+( m[3][1]*pvecOut->m128_f32[3]);
					pvecOut->z = ( m[0][2]*pvecOut->m128_f32[0])+( m[1][2]*pvecOut->m128_f32[1])+( m[2][2]*pvecOut->m128_f32[2])+( m[3][2]*pvecOut->m128_f32[3]);
					pvecOut->w = ( m[0][3]*pvecOut->m128_f32[0])+( m[1][3]*pvecOut->m128_f32[1])+( m[2][3]*pvecOut->m128_f32[2])+( m[3][3]*pvecOut->m128_f32[3]);			
#endif
				}

				xst_fi Vec4 VectorTransformRet(const Vec4& vecVector) const
				{
					Vec4 vecRet;
					VectorTransform( &vecRet, vecVector );
					return vecRet;
				}
			
		};

	}//math

}//xst

#endif //XST_CMATRIX4_H