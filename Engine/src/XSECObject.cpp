#include "XSECObject.h"
#include "XSECMesh.h"

namespace XSE
{
	class CEmptyObjectListener : public IObjectListener
	{

	};

	static CEmptyObjectListener g_EmptyListener;

	CObject::CObject(ul32 ulType, lpcastr strDbgName) : 
#if defined( XST_OBJ_DEBUG )
		XST::IObject( strDbgName )
#endif
		,m_ulObjType( ulType )
		,m_pObjListener( &g_EmptyListener )
	{
		//XST_SET_DBG_NAME( this, strDbgName );
	}

	CObject::CObject(ul32 ulType, ul32 ulHandle, lpcastr strDbgName) : CObject( ulType, strDbgName )
	{
	}

	CObject::~CObject() 
	{
	}

	const Vec3&	CObject::MoveObject(cf32& fDistance, const Vec3& vecDirection)
	{
		m_vecPosition.AddAssign( vecDirection.Mul( fDistance ) );
		return this->m_vecPosition;
	}
	
	const Vec3&	CObject::MoveObject(const Vec3& vecMove)
	{
		m_vecPosition.AddAssign( vecMove );
		return this->m_vecPosition;
	}

	void CObject::SetBoundingVolume(const CBoundingVolume& Volume)
	{
		m_ObjBoundingVolume = Volume; 
	}

	void CObject::Rotate(cf32& fAngle, cf32& fX, cf32& fY, cf32& fZ)
	{
		m_quatOrientation.Set( XST::Math::AngleToRadian( fAngle ), fX, fY, fZ );
		IsDirty( true );
	}

	void CObject::VisibleAABB(bool bVisible)
	{
#if defined( XSE_SCENE_DEBUG )
		if( m_pAABBMesh )
		{
			m_pAABBMesh->SetVisible( bVisible );
		}
#endif
	}

	bool CObject::IsAABBVisibled() const
	{
#if defined( XSE_SCENE_DEBUG )
		if( m_pAABBMesh )
			return m_pAABBMesh->IsVisible();
#endif
		return false;
	}

	void CObject::SetDistanceToCamera(cf32& fDist)
	{
		xst_assert( m_pObjListener, "(CObject::SetDistanceToCamera) Listener is null" );
		m_fObjDistToCamera = fDist;
		m_pObjListener->OnObjectSetDistanceToCamera( this );
	}

	void CObject::SetObjectListener(IObjectListener* pListener)
	{
		xst_assert( pListener, "(CObject::SetObjectListener) Listener pointer is null" );
		if( m_pObjListener == &g_EmptyListener )
			m_pObjListener = xst_null;
		xst_assert( !m_pObjListener, "(CObject::SetObjectListener) Current listener is not removed" );
		m_pObjListener = pListener;
	}

	void CObject::RemoveObjectListener()
	{
		m_pObjListener = xst_null;
	}

}//xse