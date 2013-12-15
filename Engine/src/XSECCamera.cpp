#include "XSECCamera.h"
#include "XSECSceneManager.h"
#include "XSECModelManager.h"
#include "XSECMeshManager.h"
#include "XSEIRenderSystem.h"
#include "XSECSceneManager.h"
#include "XSECSceneNode.h"

namespace XSE
{
	void SphericalToCartesian(Vec3* pvecOut, f32 fYaw, f32 fPitch, f32 fR)
	{
		f32 fSy = sinf( fYaw ), fCy = cosf( fYaw );
		f32 fSp = sinf( fPitch ), fCp = cosf( fPitch );

		pvecOut->x = fR * fCp * fCy;
		pvecOut->y = fR * fSp;
		pvecOut->z = -fR * fCp * fSy;
	}

	CCamera::CCamera(CSceneManager* pSceneMgr, xst_castring& strName, ul32 ulHandle) :
		CObject( OT::CAMERA, strName.c_str() ), 
		m_pSceneMgr( pSceneMgr ),
		m_pRS( pSceneMgr->GetRenderSystem() ), 
		m_vecLookAt( 0.0f, 0.0f, 0.1f ), 
		m_vecAngles( Vec2::ZERO ),
		m_vecCameraDistance( Vec3::ZERO ),
		m_vecRight( Vec3::X ),
		m_vecUp( Vec3::Y ),
		m_vecUpAxis( Vec3::Y ), 
		m_strName( strName ),
		m_ulObjHandle( ulHandle ),
		m_vecCamDirection( 1 ),
		m_vecCamPosition( 1 ),
		m_fFOV( XST::Math::DegreesToRadians( 45.0f ) ),
		m_fNear( 0.1f ),
		m_fFar( 100.0f ),
		m_fAspectRatio( 1.33333f ),
		m_fTime( 1.0f ),
		m_fMoveSpeed( 200.0f ), m_fHorizontalSpeed( 10.0f ), m_fVerticalSpeed( 10.0f ),
		m_bRightCalc( false ), m_bUpCalc( false ),
		m_bCompute( true )
	{
	
	}

	CCamera::~CCamera()
	{
	}


	const Vec3& CCamera::Move(cf32& fX, cf32& fY, cf32& fZ)
	{
		cf32 fConst = m_fMoveSpeed * m_fTime;
		this->m_vecCamPosition.x += fX * fConst;
		this->m_vecCamPosition.y += fY * fConst;
		this->m_vecCamPosition.z += fZ * fConst;
		return this->m_vecCamPosition;
	}

	const Vec3& CCamera::CalcRight()
	{
		m_mtxView.GetCol( 0, &m_vecRight );
		return m_vecRight;
	}

	const Vec3& CCamera::CalcUp()
	{
		//m_vecUp = m_vecCamDirection.Cross( m_vecRight ).Normalize();
		m_mtxView.GetCol( 1, &m_vecUp );
		return m_vecUp;
	}

	void CCamera::GetMatrix(const MATRIX_TYPE& eType, Mtx4* pMtxOut)
	{
		m_pRS->GetMatrix( eType, pMtxOut );
	}

	const Mtx4& CCamera::GetViewMatrix()
	{
		m_pRS->GetMatrix( MatrixTypes::VIEW, &m_mtxView );
		return m_mtxView;
	}

	const Mtx4& CCamera::GetViewProjMatrix()
	{
		m_pRS->GetMatrix( MatrixTypes::VIEW_PROJ, &m_mtxViewProj );
		return m_mtxViewProj;
	}

	void CCamera::Update(cf32& fElapsedTime)
	{
		xst_assert( m_pRS, "" );

		m_fTime = fElapsedTime;

		SetFOV( m_fFOV, m_fNear, m_fFar );

		SphericalToCartesian( &m_vecLookAt, m_vecAngles.x - XST_HALF_PI, -m_vecAngles.y, 1.0f );
		
		m_vecCamDirection = Vec3::Normalize( m_vecLookAt );
		
		m_pRS->SetLookAt( this->m_vecCamPosition, m_vecLookAt + this->m_vecCamPosition, this->m_vecUpAxis );

		m_pRS->GetMatrix( MatrixTypes::VIEW_PROJ, &m_mtxViewProj );
		m_pRS->GetMatrix( MatrixTypes::VIEW, &m_mtxView );
		
		CalcRight();
		CalcUp();

		_CreateViewFrustum( m_mtxViewProj );
		_UpdateVolumeMesh();
	}

	void CCamera::SetFOV(cf32& fAngle)
	{
		SetFOV( fAngle, m_fNear, m_fFar );
	}

	void CCamera::SetFOV(cf32& fAngle, cf32& fNear, cf32& fFar)
	{
		m_fFOV = fAngle;
		m_fNear = fNear;
		m_fFar = fFar;
		m_fAspectRatio = (f32)m_pRS->GetOptions().uiResolutionWidth / (f32)m_pRS->GetOptions().uiResolutionHeight;
		m_pRS->SetPerspectiveFOV( m_fFOV, m_fAspectRatio, m_fNear, m_fFar );
	}

	void CCamera::SetNear(cf32& fNear)
	{
		SetFOV( m_fFOV, fNear, m_fFar );
	}

	void CCamera::SetFar(cf32& fFar)
	{
		SetFOV( m_fFOV, m_fNear, fFar );
	}

	void CCamera::SetClippingPlane(cf32& fNear, cf32& fFar)
	{
		SetFOV( m_fFOV, fNear, fFar );
	}

	void CCamera::_SetCompute(bool bCompute)
	{
		m_bCompute = bCompute;
	}

	void CCamera::ShowFrustumMesh(bool bShow)
	{
		if( m_pVolumeMesh != xst_null )
		{
			m_pVolumeMesh->SetVisible( bShow );
		}
	}

	bool CCamera::IsFrustumMeshVisible() const
	{
		if( m_pVolumeMesh == xst_null )
			return false;
		return m_pVolumeMesh->IsVisible();
	}

	void CCamera::_CreateViewFrustum(const Mtx4& mtxViewProj)
	{
		m_Frustum.LeftPlane.vecPlane.x = mtxViewProj.m[ 0 ][ 3 ] + mtxViewProj.m[ 0 ][ 0 ];
		m_Frustum.LeftPlane.vecPlane.y = mtxViewProj.m[ 1 ][ 3 ] + mtxViewProj.m[ 1 ][ 0 ];
		m_Frustum.LeftPlane.vecPlane.z = mtxViewProj.m[ 2 ][ 3 ] + mtxViewProj.m[ 2 ][ 0 ];
		m_Frustum.LeftPlane.vecPlane.w = mtxViewProj.m[ 3 ][ 3 ] + mtxViewProj.m[ 3 ][ 0 ];
		m_Frustum.LeftPlane.Normalize();

		m_Frustum.RightPlane.vecPlane.x = mtxViewProj.m[ 0 ][ 3 ] - mtxViewProj.m[ 0 ][ 0 ];
		m_Frustum.RightPlane.vecPlane.y = mtxViewProj.m[ 1 ][ 3 ] - mtxViewProj.m[ 1 ][ 0 ];
		m_Frustum.RightPlane.vecPlane.z = mtxViewProj.m[ 2 ][ 3 ] - mtxViewProj.m[ 2 ][ 0 ];
		m_Frustum.RightPlane.vecPlane.w = mtxViewProj.m[ 3 ][ 3 ] - mtxViewProj.m[ 3 ][ 0 ];
		m_Frustum.RightPlane.Normalize();

		m_Frustum.TopPlane.vecPlane.x = mtxViewProj.m[ 0 ][ 3 ] - mtxViewProj.m[ 0 ][ 1 ];
		m_Frustum.TopPlane.vecPlane.y = mtxViewProj.m[ 1 ][ 3 ] - mtxViewProj.m[ 1 ][ 1 ];
		m_Frustum.TopPlane.vecPlane.z = mtxViewProj.m[ 2 ][ 3 ] - mtxViewProj.m[ 2 ][ 1 ];
		m_Frustum.TopPlane.vecPlane.w = mtxViewProj.m[ 3 ][ 3 ] - mtxViewProj.m[ 3 ][ 1 ];
		m_Frustum.TopPlane.Normalize();

		m_Frustum.BottomPlane.vecPlane.x = mtxViewProj.m[ 0 ][ 3 ] + mtxViewProj.m[ 0 ][ 1 ];
		m_Frustum.BottomPlane.vecPlane.y = mtxViewProj.m[ 1 ][ 3 ] + mtxViewProj.m[ 1 ][ 1 ];
		m_Frustum.BottomPlane.vecPlane.z = mtxViewProj.m[ 2 ][ 3 ] + mtxViewProj.m[ 2 ][ 1 ];
		m_Frustum.BottomPlane.vecPlane.w = mtxViewProj.m[ 3 ][ 3 ] + mtxViewProj.m[ 3 ][ 1 ];
		m_Frustum.BottomPlane.Normalize();

		m_Frustum.NearPlane.vecPlane.x = mtxViewProj.m[ 0 ][ 2 ];
		m_Frustum.NearPlane.vecPlane.y = mtxViewProj.m[ 1 ][ 2 ];
		m_Frustum.NearPlane.vecPlane.z = mtxViewProj.m[ 2 ][ 2 ];
		m_Frustum.NearPlane.vecPlane.w = mtxViewProj.m[ 3 ][ 2 ];
		m_Frustum.NearPlane.Normalize();

		m_Frustum.FarPlane.vecPlane.x = mtxViewProj.m[ 0 ][ 3 ] - mtxViewProj.m[ 0 ][ 2 ];
		m_Frustum.FarPlane.vecPlane.y = mtxViewProj.m[ 1 ][ 3 ] - mtxViewProj.m[ 1 ][ 2 ];
		m_Frustum.FarPlane.vecPlane.z = mtxViewProj.m[ 2 ][ 3 ] - mtxViewProj.m[ 2 ][ 2 ];
		m_Frustum.FarPlane.vecPlane.w = mtxViewProj.m[ 3 ][ 3 ] - mtxViewProj.m[ 3 ][ 2 ];
		m_Frustum.FarPlane.Normalize();
	}

	bool CCamera::IsSphereInFrustum(const Vec4& vecSpherePosition, cf32& fSphereRadius) const
	{
		for(u32 i = 6; i --> 0;)
		{
			f32 f = m_Frustum.aPlanes[ i ].DotCoord( vecSpherePosition );
			if( f + fSphereRadius < 0.0f )
			{
				return false;
			}
		}

		/*f32 f = m_Frustum.FarPlane.DotCoord( vecSpherePosition );
		if( f + fSphereRadius < 0.0f )
		{
			return false;
		}

		f = m_Frustum.NearPlane.DotCoord( vecSpherePosition );
		if( f + fSphereRadius < 0.0f )
		{
			return false;
		}

		f = m_Frustum.LeftPlane.DotCoord( vecSpherePosition );
		if( f + fSphereRadius < 0.0f )
		{
			return false;
		}

		f = m_Frustum.RightPlane.DotCoord( vecSpherePosition );
		if( f + fSphereRadius < 0.0f )
		{
			return false;
		}

		f = m_Frustum.TopPlane.DotCoord( vecSpherePosition );
		if( f + fSphereRadius < 0.0f )
		{
			return false;
		}

		f = m_Frustum.BottomPlane.DotCoord( vecSpherePosition );
		if( f + fSphereRadius < 0.0f )
		{
			return false;
		}*/

		return true;
	}

	bool CCamera::IsAABBInFrustum(const XST::CAABB& AABB) const
	{
		//implement
		return false;
	}

	/*f32 CCamera::_NormalizeAngle(f32 fAngle)
	{
		fAngle /= XST_2PI;
		fAngle = XST::Math::Frac( fAngle );

		if( fAngle < 0.0f )
		{
			fAngle += 0.1f;
		}
		else if( fAngle >= 1.0f )
		{
			fAngle -= 1.0f;
		}

		return fAngle * XST_2PI;
	}*/

#define LEFT_DOWN_NEAR		0
#define LEFT_UP_NEAR		1
#define RIGHT_UP_NEAR		2
#define RIGHT_DOWN_NEAR		3
#define LEFT_DOWN_FAR		4
#define LEFT_UP_FAR			5
#define RIGHT_UP_FAR		6
#define RIGHT_DOWN_FAR		7

	i32 CCamera::_CreateFrustumMesh()
	{
		CMeshManager* pMeshMgr = m_pSceneMgr->GetModelManager()->GetMeshManager();
		IInputLayout* pIL = m_pRS->GetInputLayout( InputLayoutElements::POSITION );
		MeshPtr pMesh = pMeshMgr->CreateMesh( this->m_strName, pIL, m_pSceneMgr->GetName() );

		VertexBufferPtr pVB = pMesh->CreateVertexBuffer();
		IndexBufferPtr pIB = pMesh->CreateIndexBuffer();

		pVB->SetInputLayout( pIL );
		pVB->SetTopologyType( TopologyTypes::LINE_LIST );
		pVB->SetUsage( BufferUsages::DEFAULT );
		pVB->SetVertexCount( 8 );

		if( XST_FAILED( pVB->Lock() ) )
		{
			return XST_FAIL;
		}

		if( XST_FAILED( pVB->Unlock() ) )
		{
			return XST_FAIL;
		}

		pIB->SetIndexCount( 8 * 4 );
		pIB->SetUsage( BufferUsages::STATIC );
		
		if( XST_FAILED( pIB->Lock() ) )
		{
			return XST_FAIL;
		}

		CIndexData& IData = pIB->GetIndexData();
		u32 uiCurrIndex = 0;

		//Front Face
		//Left edge
		IData.SetIndex( uiCurrIndex++, LEFT_DOWN_NEAR ); //left, down, back
		IData.SetIndex( uiCurrIndex++, LEFT_UP_NEAR ); //left, up, back
		//Up edge
		IData.SetIndex( uiCurrIndex++, LEFT_UP_NEAR ); //left, up, back
		IData.SetIndex( uiCurrIndex++, RIGHT_UP_NEAR ); //right, up, back
		//Right edge
		IData.SetIndex( uiCurrIndex++, RIGHT_UP_NEAR ); //
		IData.SetIndex( uiCurrIndex++, RIGHT_DOWN_NEAR ); //
		//Down edge
		IData.SetIndex( uiCurrIndex++, RIGHT_DOWN_NEAR ); //
		IData.SetIndex( uiCurrIndex++, LEFT_DOWN_NEAR ); //

		//Back Face
		//Left edge
		IData.SetIndex( uiCurrIndex++, LEFT_DOWN_FAR );
		IData.SetIndex( uiCurrIndex++, LEFT_UP_FAR );
		//Up edge
		IData.SetIndex( uiCurrIndex++, LEFT_UP_FAR ); 
		IData.SetIndex( uiCurrIndex++, RIGHT_UP_FAR ); 
		//Right edge
		IData.SetIndex( uiCurrIndex++, RIGHT_UP_FAR ); //
		IData.SetIndex( uiCurrIndex++, RIGHT_DOWN_FAR ); //
		//Down edge
		IData.SetIndex( uiCurrIndex++, RIGHT_DOWN_FAR ); //
		IData.SetIndex( uiCurrIndex++, LEFT_DOWN_FAR ); //

		//Left Face
		//Left edge
		IData.SetIndex( uiCurrIndex++, LEFT_DOWN_NEAR ); 
		IData.SetIndex( uiCurrIndex++, LEFT_UP_NEAR ); 
		//Up edge
		IData.SetIndex( uiCurrIndex++, LEFT_UP_NEAR ); 
		IData.SetIndex( uiCurrIndex++, LEFT_UP_FAR ); 
		//Right edge
		IData.SetIndex( uiCurrIndex++, LEFT_UP_FAR ); //
		IData.SetIndex( uiCurrIndex++, LEFT_DOWN_FAR ); //
		//Down edge
		IData.SetIndex( uiCurrIndex++, LEFT_DOWN_FAR ); //
		IData.SetIndex( uiCurrIndex++, LEFT_DOWN_NEAR ); //

		//Right Face
		//Left edge
		IData.SetIndex( uiCurrIndex++, RIGHT_DOWN_NEAR ); 
		IData.SetIndex( uiCurrIndex++, RIGHT_UP_NEAR ); 
		//Up edge
		IData.SetIndex( uiCurrIndex++, RIGHT_UP_NEAR ); 
		IData.SetIndex( uiCurrIndex++, RIGHT_UP_FAR ); 
		//Right edge
		IData.SetIndex( uiCurrIndex++, RIGHT_UP_FAR ); //
		IData.SetIndex( uiCurrIndex++, RIGHT_DOWN_FAR ); //
		//Down edge
		IData.SetIndex( uiCurrIndex++, RIGHT_DOWN_FAR ); //
		IData.SetIndex( uiCurrIndex++, RIGHT_DOWN_NEAR ); //

		if( XST_FAILED( pIB->Unlock() ) )
		{
			return XST_FAIL;
		}

		m_pVolumeMesh = pMesh.GetPointer();
		m_pVolumeMesh->SetVisible( false ); //by default do not show it

		m_pSceneMgr->CreateNode( this->m_strName + "/volume_dbg" )->AddObject( pMesh );

		return _UpdateVolumeMesh();
	}

	Vec4 Mul(const Mtx4& mtx, const Vec4& vec)
	{
		return Vec4(mtx.m[ 0 ][ 0 ] * vec.x + mtx.m[ 1 ][ 0 ] * vec.y + mtx.m[ 2 ][ 0 ] * vec.z + mtx.m[ 3 ][ 0 ] * vec.w,
					mtx.m[ 0 ][ 1 ] * vec.x + mtx.m[ 1 ][ 1 ] * vec.y + mtx.m[ 2 ][ 1 ] * vec.z + mtx.m[ 3 ][ 1 ] * vec.w,
					mtx.m[ 0 ][ 2 ] * vec.x + mtx.m[ 1 ][ 2 ] * vec.y + mtx.m[ 2 ][ 2 ] * vec.z + mtx.m[ 3 ][ 2 ] * vec.w,
					mtx.m[ 0 ][ 3 ] * vec.x + mtx.m[ 1 ][ 3 ] * vec.y + mtx.m[ 2 ][ 3 ] * vec.z + mtx.m[ 3 ][ 3 ] * vec.w );
	}

	i32 CCamera::_UpdateVolumeMesh()
	{
		if( !m_bCompute || m_pVolumeMesh == xst_null || !m_pVolumeMesh->IsVisible() )
			return XST_OK; //ok it is only for observing

		CVertexData& VData = m_pVolumeMesh->GetVertexBuffer()->GetVertexData();
		CPlane FarPlane = m_Frustum.FarPlane;
		CPlane NearPlane = m_Frustum.NearPlane;

		const Vec3& Position = this->m_vecCamPosition;
		const Vec3& Forward = this->m_vecCamDirection;
		const Vec3& Right = this->m_vecRight;
		const Vec3& Up = this->m_vecUp;
		Vec4 aVecs[ 8 ];

		/*f32 fFar = this->m_fFar;
		f32 fNear = this->m_fNear;
		f32 yFac = tanf(this->m_fFOV * XST_PI / 360.0f);
		f32 xFac = yFac*this->m_fAspectRatio;

		Vec3 vecFarLeftTop = Position + Forward*fFar - Right*fFar*xFac*fFar + Up*yFac*fFar;
        Vec3 vecFarRightTop = Position + Forward*fFar + Right*fFar*xFac*fFar + Up*yFac*fFar;
        Vec3 vecFarLeftBottom = Position + Forward*fFar - Right*fFar*xFac*fFar - Up*yFac*fFar;
        Vec3 vecFarRightBottom = Position + Forward*fFar + Right*fFar*xFac*fFar - Up*yFac*fFar;
		
		Vec3 vecNearLeftTop = Position + Forward*fNear - Right*fNear*xFac*fNear + Up*yFac*fNear;
		Vec3 vecNearRightTop = Position + Forward*fNear + Right*fNear*xFac*fNear + Up*yFac*fNear;
		Vec3 vecNearLeftBottom = Position + Forward*fNear - Right*fNear*xFac*fNear - Up*yFac*fNear;		
		Vec3 vecNearRightBottom = Position + Forward*fNear + Right*fNear*xFac*fNear - Up*yFac*fNear;*/

		/*vecNearLeftBottom = Vec3( -1, -1, -1 );
		vecNearRightBottom = Vec3( 1, -1, -1 );
		vecNearLeftTop = Vec3( -1, 1, -1 );
		vecNearRightTop = Vec3( 1, 1, -1 );

		vecFarLeftBottom = Vec3( -1, -1, 1 );
		vecFarRightBottom = Vec3( 1, -1, 1 );
		vecFarLeftTop = Vec3( -1, 1, 1 );
		vecFarRightTop = Vec3( 1, 1, 1 );*/

		//Mtx4 mtxInvViewProj, mtxInvViewProj2;
		//Mtx4::Inverse( m_mtxViewProj, &mtxInvViewProj2 );
		//Mtx4 mtxProj;
		//GetMatrix( MatrixTypes::INVERTED_VIEW_PROJ, &mtxInvViewProj );
		GetMatrix( MatrixTypes::PROJECTION, &m_mtxProjection );
		Mtx4 mtxWorld, mtxWorldViewProj;
		GetMatrix( MatrixTypes::WORLD, &mtxWorld );
		GetMatrix( MatrixTypes::VIEW, &m_mtxView );
		GetMatrix( MatrixTypes::PROJECTION, &m_mtxProjection );
		Mtx4 mtxWorldView = Mtx4::Mul( mtxWorld, m_mtxView );
		Mtx4 mtxProjWorldView = Mtx4::Mul( m_mtxProjection, mtxWorldView );
		mtxWorldViewProj = Mtx4::Mul( Mtx4::Mul( mtxWorld, m_mtxView ), m_mtxProjection );
		Mtx4 mtxInvProjWorldView = /*Mtx4::Inverse*/( mtxWorldViewProj ); 
		//Mtx4 mtxInvProjWorldView = Mtx4::Inverse( mtxProjWorldView );

		//vecNearLeftBottom = Mul( mtxInvViewProj,	Vec4( -1, -1, -1, 1 ) );
		//vecNearRightBottom = Mul( mtxInvViewProj,	Vec4( -1, -1, -1, 1 ) );
		//vecNearLeftTop = Mul( mtxInvViewProj,		Vec4( -1, -1, -1, 1 ) );
		//vecNearRightTop = Mul( mtxInvViewProj,		Vec4( -1, -1, -1, 1 ) );

		//vecFarLeftBottom = Mul( mtxInvViewProj,		Vec4( -1, -1, 1, 1 ) );
		//vecFarRightBottom = Mul( mtxInvViewProj,	Vec4( -1, -1, 1, 1 ) );
		//vecFarLeftTop = Mul( mtxInvViewProj,		Vec4( -1, -1, 1, 1 ) );
		//vecFarRightTop = Mul( mtxInvViewProj,		Vec4( -1, -1, 1, 1 ) );

		f32 yNear = 2* tanf( this->m_fFOV / 2 ) * this->m_fNear; //(tan (this->m_fFOV / 2 * 0.017453292519943295769236907684886f)) * this->m_fNear;
		f32 xNear = yNear * this->m_fAspectRatio;
		f32 yFar = ( yNear / this->m_fNear ) * this->m_fFar;
		f32 xFar = yFar * this->m_fAspectRatio;
		f32 fNear = this->m_fNear;
		f32 fFar = this->m_fFar;

		//Near
		aVecs[ RIGHT_UP_NEAR ] = Vec4( xNear, yNear, fNear, 1 );
		aVecs[ RIGHT_DOWN_NEAR ] = Vec4( xNear, -yNear, fNear, 1 );
		aVecs[ LEFT_DOWN_NEAR ] = Vec4( -xNear, -yNear, fNear, 1);
		aVecs[ LEFT_UP_NEAR ] = Vec4( -xNear, yNear, fNear, 1 );
		//Far
		aVecs[ RIGHT_UP_FAR ] = Vec4( xFar, yFar, fFar, 1 );
		aVecs[ RIGHT_DOWN_FAR ] = Vec4( xFar, -yFar, fFar, 1 );
		aVecs[ LEFT_DOWN_FAR ] = Vec4( -xFar, -yFar, fFar, 1 );
		aVecs[ LEFT_UP_FAR ] = Vec4( -xFar, yFar, fFar, 1 );

		//Near
		//aVecs[ RIGHT_UP_NEAR ] = Vec4( 1, 1, 0, 1 );
		//aVecs[ RIGHT_DOWN_NEAR ] = Vec4( 1, -1, 0, 1 );
		//aVecs[ LEFT_DOWN_NEAR ] = Vec4( -1, -1, 0, 1 );
		//aVecs[ LEFT_UP_NEAR ] = Vec4( -1, 1, 0, 1 );
		////Far
		//aVecs[ RIGHT_UP_FAR ] = Vec4( 1, 1, 1, 1 );
		//aVecs[ RIGHT_DOWN_FAR ] = Vec4( 1, -1, 1, 1 );
		//aVecs[ LEFT_DOWN_FAR ] = Vec4( -1, -1, 1, 1 );
		//aVecs[ LEFT_UP_FAR ] = Vec4( -1, 1, 1, 1 );

		//Near
		//aVecs[ RIGHT_UP_NEAR ] = Mul( mtxInvProjWorldView, aVecs[ RIGHT_UP_NEAR ] );
		//aVecs[ RIGHT_DOWN_NEAR ] = Mul( mtxInvProjWorldView, aVecs[ RIGHT_DOWN_NEAR ] );
		//aVecs[ LEFT_DOWN_NEAR ] = Mul( mtxInvProjWorldView, aVecs[ LEFT_DOWN_NEAR ] );
		//aVecs[ LEFT_UP_NEAR ] = Mul( mtxInvProjWorldView, aVecs[ LEFT_UP_NEAR ] );
		////Far
		//aVecs[ RIGHT_UP_FAR ] = Mul( mtxInvProjWorldView, aVecs[ RIGHT_UP_FAR ] );
		//aVecs[ RIGHT_DOWN_FAR ] = Mul( mtxInvProjWorldView, aVecs[ RIGHT_DOWN_FAR ] );
		//aVecs[ LEFT_DOWN_FAR ] = Mul( mtxInvProjWorldView, aVecs[ LEFT_DOWN_FAR ] );
		//aVecs[ LEFT_UP_FAR ] = Mul( mtxInvProjWorldView, aVecs[ LEFT_UP_FAR ] );

		for(u32 i = 0; i < 8; ++i)
		{
			//aVecs[ i ] = Mul( mtxInvProjWorldView, aVecs[ i ] );
			f32 fScale = 1.0f / aVecs[ i ].w;
			//aVecs[ i ] *= 10;
		}

		//Back
		VData.SetPosition( LEFT_DOWN_NEAR, aVecs[ LEFT_DOWN_NEAR ] );
		VData.SetPosition( LEFT_UP_NEAR, aVecs[ LEFT_UP_NEAR ] );
		VData.SetPosition( RIGHT_UP_NEAR, aVecs[ RIGHT_UP_NEAR ] );
		VData.SetPosition( RIGHT_DOWN_NEAR, aVecs[ RIGHT_DOWN_NEAR ] );

		//Front
		VData.SetPosition( LEFT_DOWN_FAR, aVecs[ LEFT_DOWN_FAR ] );
		VData.SetPosition( LEFT_UP_FAR, aVecs[ LEFT_UP_FAR ] );
		VData.SetPosition( RIGHT_UP_FAR, aVecs[ RIGHT_UP_FAR ] );
		VData.SetPosition( RIGHT_DOWN_FAR, aVecs[ RIGHT_DOWN_FAR ] );

		m_pVolumeMesh->GetVertexBuffer()->Update();
		//m_pVolumeMesh->SetPosition( this->GetPosition() );
		
		for(u32 i = 0; i < m_pVolumeMesh->GetIndexBuffer()->GetIndexCount() / 2; i += 2)
		{
			xst_stringstream ss;
			Vec3 vecPos1, vecPos2;
			u32 uiID;

			uiID = m_pVolumeMesh->GetIndexBuffer()->GetIndexData().GetIndex( i );
			VData.GetPosition( uiID, &vecPos1 );

			uiID = m_pVolumeMesh->GetIndexBuffer()->GetIndexData().GetIndex( i + 1 );
			VData.GetPosition( uiID, &vecPos2 );

			ss << vecPos1 << "; " << vecPos2;
			//XST::CDebug::PrintDebugLN( ss.str() );
		}

		return XST_OK;
	}

}//xse