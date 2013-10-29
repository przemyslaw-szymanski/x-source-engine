#include "XSEIRenderableObject.h"
#include "XSECSceneNode.h"
#include "XSECSceneManager.h"
#include "XSECMaterialManager.h"
#include "XSEIRenderSystem.h"

namespace XSE
{
	void IRenderableObject::SetRenderableObject(const IRenderableObject* pOther)
	{
		this->SetObject( pOther );
		this->SetMaterial( pOther->GetMaterial() );
		this->SetTransformMatrix( pOther->GetTransformMatrix() );
		this->SetVisible( pOther->IsVisible() );
		this->m_pInputLayout = pOther->GetInputLayout();
		this->m_bManualRendering = pOther->IsManualRendering();
		this->m_bNeedUpdate = false;
	}

	void IRenderableObject::SetPosition(cf32 &fX, cf32 &fY, cf32 &fZ)
	{
		xst_assert( m_pSceneNode != xst_null, "(IRenderableObject::SetPosition) Object is not in scene node" ); 
		m_pSceneNode->SetObjectPosition( fX, fY, fZ );
	}

	void IRenderableObject::SetScale(cf32& fX, cf32& fY, cf32& fZ)
	{
		xst_assert( m_pSceneNode != xst_null, "(IRenderableObject::SetPosition) Object is not in scene node" ); 
		m_pSceneNode->SetObjectScale( fX, fY, fZ );
	}

	void IRenderableObject::SetOrientation(cf32& fAngle, cf32& fX, cf32& fY, cf32& fZ)
	{
		xst_assert( m_pSceneNode != xst_null, "(IRenderableObject::SetPosition) Object is not in scene node" ); 
		m_pSceneNode->SetObjectOrientation( fAngle, fX, fY, fZ );
	}

	const Vec3& IRenderableObject::GetPosition() const
	{ 
		xst_assert( m_pSceneNode != xst_null, "(IRenderableObject::SetPosition) Object is not in scene node" ); 
		return m_pSceneNode->GetObjectPosition(); 
	}

	const Vec3& IRenderableObject::GetScale() const
	{
		xst_assert( m_pSceneNode != xst_null, "(IRenderableObject::SetPosition) Object is not in scene node" ); 
		return m_pSceneNode->GetObjectScale();
	}

	const Quat& IRenderableObject::GetOrientation() const
	{
		xst_assert( m_pSceneNode != xst_null, "(IRenderableObject::SetPosition) Object is not in scene node" ); 
		return m_pSceneNode->GetObjectOrientation();
	}

	i32 IRenderableObject::SetMaterial(xst_castring& strMatName, xst_castring& strMatGroup /* = ALL_GROUPS */)
	{
		MaterialPtr pMat = m_pSceneNode->GetSceneManager()->GetMaterialManager()->GetMaterial( strMatName, strMatGroup );
		xst_assert( pMat != xst_null, "(IRenderableObject::SetMaterial) Material does not exists" );
		SetMaterial( pMat );
		return XST_OK;
	}

	void IRenderableObject::SetObjectPosition(cf32& fX, cf32& fY, cf32& fZ)
	{
		if( this->m_vecPosition.x != fX || this->m_vecPosition.y != fY || this->m_vecPosition.z != fZ )
		{
			this->m_vecPosition.x = fX;
			this->m_vecPosition.y = fY;
			this->m_vecPosition.z = fZ;
			m_bNeedUpdate = true;
		}
	}

	void IRenderableObject::SetObjectOrientation(cf32& fAngle, cf32& fX, cf32& fY, cf32& fZ)
	{
		if( this->m_quatOrientation.x != fX || this->m_quatOrientation.y != fY || this->m_quatOrientation.z != fZ || this->m_quatOrientation.w != fAngle )
		{
			this->m_quatOrientation.Set( XST::Math::AngleToRadian( fAngle ), fX, fY, fZ );
			m_bNeedUpdate = true;
		}
	}

	void IRenderableObject::SetObjectScale(cf32& fX, cf32& fY, cf32& fZ)
	{
		if( this->m_vecScale.x != fX || this->m_vecScale.y != fY || this->m_vecScale.z != fZ )
		{
			this->m_vecScale.x = fX;
			this->m_vecScale.y = fY;
			this->m_vecScale.z = fZ;
			m_bNeedUpdate = true;
		}
	}

	void IRenderableObject::Update()
	{
		if( !m_bNeedUpdate )
			return;
		m_bNeedUpdate = false;
		xst_assert( m_pSceneNode != xst_null, "(IRenderableObject::SetPosition) Object is not in scene node" ); 
		
		IRenderSystem* pRS = m_pSceneNode->GetSceneManager()->GetRenderSystem();

		m_mtxTransform.Identity();

		Mtx4 mtxTranslate, mtxScale, mtxRotation;
		//pRS->SetTranslation( this->m_vecPosition.x, this->m_vecPosition.y, this->m_vecPosition.z );
		//pRS->GetMatrix( MatrixTypes::WORLD, &mtxTransform );
		pRS->SetTranslation( &mtxTranslate, this->GetObjectPosition() );
		f32 fAngle;
		Vec3 vecAxis;
		this->GetObjectOrientation().ToAngleAxis( &fAngle, &vecAxis );
		pRS->SetRotation( &mtxRotation, fAngle, vecAxis );
		pRS->SetScale( &mtxScale, this->GetObjectScale() );
		
		m_mtxTransform = mtxScale * ( mtxTranslate * mtxRotation );
	}

}//xse