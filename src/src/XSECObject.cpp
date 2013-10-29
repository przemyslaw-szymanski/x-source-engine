#include "XSECObject.h"
#include "XSECMesh.h"

namespace XSE
{
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

	void CObject::SetObjectBoundingVolume(const CBoundingVolume& Volume)
	{
		m_ObjBoundingVolume = Volume; 
	}

	void CObject::RotateObject(cf32& fAngle, cf32& fX, cf32& fY, cf32& fZ)
	{
		m_quatOrientation.Set( XST::Math::AngleToRadian( fAngle ), fX, fY, fZ );
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

}//xse