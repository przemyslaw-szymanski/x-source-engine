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

	void CalcFrustumVertices(Vec3 avecVertices[ 8 ], const CCamera* pCamera);

	CCamera::CCamera( CSceneManager* pSceneMgr, xst_castring& strName, ul32 ulHandle ) :
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
		m_fFar( 1000.0f ),
		m_fAspectRatio( 1.33333f ),
		m_fTime( 1.0f ),
		m_fViewDistance( 1000.0f ),
		m_fMoveSpeed( 200.0f ), m_fHorizontalSpeed( 10.0f ), m_fVerticalSpeed( 10.0f ),
		m_bRightCalc( false ), m_bUpCalc( false ),
		m_bCompute( true )
	{
		m_quatOrientation = Quaternion::IDENTITY;
	}

	CCamera::~CCamera()
	{
	}

	i32 CCamera::Init()
	{
		// Just update the perspective matrix. FOV angle, near, far, etc should be set earlier
		m_pRS->SetPerspectiveFOV( m_fFOV, m_fAspectRatio, m_fNear, m_fFar );
		m_pRS->GetMatrix( MatrixTypes::PROJECTION, &m_mtxProjection );
		return XST_OK;
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

	void CCamera::Rotate(const Quaternion& quatRotation)
	{
		Quaternion quatNorm = quatRotation;
		quatNorm.Normalize();
		m_quatOrientation = quatNorm * m_quatOrientation;
	}

	void CCamera::Rotate(const Vec3& vecAxis, cf32& fAngle)
	{
		Quaternion Q;
		Q.FromAngleAxis( fAngle, vecAxis );
		Rotate( Q );
	}

	void CCamera::SetAngleY(cf32& fAngle) // yaw
	{
		m_vecAngles.y = std::min( std::max( -XST_HALF_PI + 1e-3f, fAngle ), XST_HALF_PI - 1e-3f );
		//m_vecRotationAxis.y = 1;
		Vec3 vecYAxis = m_quatOrientation * Vec3::Y;
		Rotate( vecYAxis, fAngle );
	}

	void CCamera::SetAngleX(cf32& fAngle) // pitch
	{
		m_vecAngles.x = _NormalizeAngle( fAngle );
		//m_vecRotationAxis = 1;
		Vec3 vecXAxis = m_quatOrientation * Vec3::X;
		Rotate( vecXAxis, fAngle );
	}

#include <DirectXCollision.h>
	DirectX::BoundingFrustum g_Frust;

	void CCamera::Update(cf32& fElapsedTime)
	{
		xst_assert( m_pRS, "" );

		m_fTime = fElapsedTime;

		//m_pRS->SetPerspectiveFOV( m_fFOV, m_fAspectRatio, m_fNear, m_fFar );

		SphericalToCartesian( &m_vecLookAt, m_vecAngles.x - XST_HALF_PI, -m_vecAngles.y, 1.0f );
		
		m_vecCamDirection = Vec3::Normalize( m_vecLookAt );
		
		m_pRS->SetLookAt( this->m_vecCamPosition, m_vecLookAt + this->m_vecCamPosition, this->m_vecUpAxis );

		m_pRS->GetMatrix( MatrixTypes::VIEW_PROJ, &m_mtxViewProj );
		m_pRS->GetMatrix( MatrixTypes::VIEW, &m_mtxView );
		
		CalcRight();
		CalcUp();

		_CreateViewFrustum( m_mtxViewProj );
		_UpdateVolumeMesh();
		//m_vecRotationAxis = Vec3::ZERO;
	}

	void CCamera::SetFOV(cf32& fAngle)
	{
		SetFOV( fAngle, m_fNear, m_fFar );
	}

	void CCamera::SetFOV(cf32& fAngle, cu32& uiScreenWidth, cu32& uiScreenHeight, cf32& fNear, cf32& fFar)
	{
		m_fFOV = fAngle;
		m_fNear = fNear;
		m_fFar = fFar;
		m_fAspectRatio = (f32) uiScreenWidth / (f32) uiScreenHeight;
	}

	void CCamera::SetFOV(cf32& fAngle, cf32& fNear, cf32& fFar)
	{
		SetFOV( fAngle, m_pRS->GetOptions().uiResolutionWidth, m_pRS->GetOptions().uiResolutionHeight, fNear, fFar );
	}

	void CCamera::SetAspectRatio(cu32& uiScreenWidth, cu32& uiScreenHeight)
	{
		SetFOV( m_fFOV, uiScreenWidth, uiScreenHeight, m_fNear, m_fFar );
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
		if( !m_bCompute )
			return;

		m_Frustum.CreateFromMatrix( m_mtxProjection );
		Mtx4 mtxIV;
		Mtx4::Inverse( m_mtxView, &mtxIV );
		m_Frustum.Transform( mtxIV );
		
		/*DirectX::XMMATRIX mtxProj, mtxInvView, mtxView;
		xst_memcpy( mtxProj.r, sizeof( DirectX::XMMATRIX ), &m_mtxProjection.r, sizeof( DirectX::XMMATRIX ) );
		xst_memcpy( mtxView.r, sizeof( DirectX::XMMATRIX ), &m_mtxView.r, sizeof( DirectX::XMMATRIX ) );
		DirectX::BoundingFrustum::CreateFromMatrix( g_Frust, mtxProj );
		DirectX::XMVECTOR Det;
		mtxInvView = DirectX::XMMatrixInverse( &Det, mtxView );
		g_Frust.Transform( g_Frust, mtxInvView );*/
	}

	bool CCamera::IsSphereInFrustum(const Vec4& vecSpherePosition, cf32& fSphereRadius) const
	{
		if( !m_bCompute )
			return true;
		// Do a simple sphere-sphere test
		f32 fDist = m_Frustum.m_BoundingSphere.vecCenter.Distance( vecSpherePosition );
		if( fDist >= m_Frustum.m_BoundingSphere.fRadius + fSphereRadius )
			return false;
		return m_Frustum.SphereTest( vecSpherePosition, fSphereRadius );
		/*DirectX::BoundingSphere Sphere;
		Sphere.Center.x = vecSpherePosition.x;
		Sphere.Center.y = vecSpherePosition.y;
		Sphere.Center.z = vecSpherePosition.z;
		Sphere.Radius = fSphereRadius;
		return g_Frust.Intersects( Sphere );*/
		//return true;
	}

	bool CCamera::IsAABBInFrustum(const XST::CAABB& AABB) const
	{
		if( !m_bCompute )
			return true;
		return m_Frustum.AABBTest( AABB );
		/*DirectX::BoundingBox Box;
		Vec3 vecCenter = AABB.CalcCenter();
		Vec3 vecSize = AABB.CalcSize() * 0.5f;
		Box.Center.x = vecCenter.x;
		Box.Center.y = vecCenter.y;
		Box.Center.z = vecCenter.z;
		Box.Extents.x = vecSize.x;
		Box.Extents.y = vecSize.y;
		Box.Extents.z = vecSize.z;
		return g_Frust.Intersects( Box );
		return true;*/
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

	void Test( RenderableObjectPtr p )
	{}

	i32 CCamera::_CreateFrustumMesh()
	{
		CMeshManager* pMeshMgr = m_pSceneMgr->GetModelManager()->GetMeshManager();
		IInputLayout* pIL = m_pRS->GetInputLayout( InputLayoutElements::POSITION );
		MeshPtr pMesh = pMeshMgr->CreateMesh( this->m_strName, pIL, m_pSceneMgr->GetName() );
		Test( pMesh );
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

		Vec3 avecVertices[ 8 ];
		CalcFrustumVertices( avecVertices, this );

		VData.SetPosition( LEFT_UP_FAR, avecVertices[ LEFT_UP_FAR ] );
		VData.SetPosition( RIGHT_UP_FAR, avecVertices[ RIGHT_UP_FAR ] );
		VData.SetPosition( LEFT_DOWN_FAR, avecVertices[ LEFT_DOWN_FAR ] );
		VData.SetPosition( RIGHT_DOWN_FAR, avecVertices[ RIGHT_DOWN_FAR ] );

		VData.SetPosition( LEFT_UP_NEAR, avecVertices[ LEFT_UP_NEAR ] );
		VData.SetPosition( RIGHT_UP_NEAR, avecVertices[ RIGHT_UP_NEAR ] );
		VData.SetPosition( LEFT_DOWN_NEAR, avecVertices[ LEFT_DOWN_NEAR ] );
		VData.SetPosition( RIGHT_DOWN_NEAR, avecVertices[ RIGHT_DOWN_NEAR ] );

		m_pVolumeMesh->GetVertexBuffer()->Update();

		return XST_OK;
	}

	void CalcFrustumVertices(Vec3 avecVertices[ 8 ], const CCamera* pCamera)
	{
		const Vec3& Position = pCamera->GetPosition();
		const Vec3& Forward = pCamera->GetDirection();
		const Vec3& Right = pCamera->GetRight();
		const Vec3& Up = pCamera->GetUp();

		cf32 fTanPrecalc = 2.0f * tanf( pCamera->GetFov() * 0.5f );
		cf32 fHnear = fTanPrecalc * pCamera->GetNear();
		cf32 fWnear = fHnear * pCamera->GetAspectRatio();
		cf32 fHfar = fTanPrecalc * pCamera->GetFar();
		cf32 fWfar = fHfar * pCamera->GetAspectRatio( );
		const Vec3 fc = Position + Forward * pCamera->GetFar( );
		const Vec3 UpHFarPrecalc( Up * fHfar * 0.5f );
		const Vec3 RightWFarPrecalc( Right * fWfar * 0.5f );
		const Vec3 ftl = fc + (UpHFarPrecalc) -( RightWFarPrecalc );
		const Vec3 ftr = fc + (UpHFarPrecalc) +( RightWFarPrecalc );
		const Vec3 fbl = fc - (UpHFarPrecalc) -( RightWFarPrecalc );
		const Vec3 fbr = fc - (UpHFarPrecalc) +( RightWFarPrecalc );

		const Vec3 nc = Position + Forward * pCamera->GetNear();
		const Vec3 UpHNearPrecalc( Up * fHnear / 2.0f );
		const Vec3 RightWNearPrecalc( Right * fWnear / 2.0f );
		const Vec3 ntl = nc + (UpHNearPrecalc) -( RightWNearPrecalc );
		const Vec3 ntr = nc + (UpHNearPrecalc) +( RightWNearPrecalc );
		const Vec3 nbl = nc - (UpHNearPrecalc) -( RightWNearPrecalc );
		const Vec3 nbr = nc - (UpHNearPrecalc) +( RightWNearPrecalc );

		avecVertices[ LEFT_UP_FAR ] = (ftl);
		avecVertices[ RIGHT_UP_FAR ] = (ftr);
		avecVertices[ LEFT_DOWN_FAR ] = ( fbl );
		avecVertices[ RIGHT_DOWN_FAR ] = ( fbr );

		avecVertices[ LEFT_UP_NEAR ] = ( ntl );
		avecVertices[ RIGHT_UP_NEAR ] = ( ntr );
		avecVertices[ LEFT_DOWN_NEAR ] = ( nbl );
		avecVertices[ RIGHT_DOWN_NEAR ] = ( nbr );
	}

}//xse