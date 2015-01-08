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
	}

	i32 IRenderableObject::SetMaterial(xst_castring& strMatName, xst_castring& strMatGroup /* = ALL_GROUPS */)
	{
		MaterialPtr pMat = m_pSceneNode->GetSceneManager()->GetMaterialManager()->GetMaterial( strMatName, strMatGroup );
		xst_assert( pMat != xst_null, "(IRenderableObject::SetMaterial) Material does not exists" );
		SetMaterial( pMat );
		return XST_OK;
	}

    void IRenderableObject::SetVisible(bool bIs)
    {
        m_bVisible = bIs;
		CObject::Disable( bIs ? ODR::NOT_DISABLED : RODR::NOT_VISIBLE );
    }


	/*void IRenderableObject::SetObjectOrientation(cf32& fAngle, cf32& fX, cf32& fY, cf32& fZ)
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
	}*/

	void IRenderableObject::CalcWorldPosition(Vec3* pVecOut) const
	{
		xst_assert( pVecOut != xst_null, "(IRenderableObject::CalcWorldPosition)" );
		xst_assert( m_pSceneNode != xst_null, "(IRenderableObject::CalcWorldPosition) Scene node is not set" );
		pVecOut->Set( m_pSceneNode->GetPosition() );
		pVecOut->AddAssign( this->m_vecPosition );
	}

	void IRenderableObject::CalcWorldScale(Vec3* pVecOut)
	{
		xst_assert( pVecOut != xst_null, "(IRenderableObject::CalcWorldPosition)" );
		xst_assert( m_pSceneNode != xst_null, "(IRenderableObject::CalcWorldPosition) Scene node is not set" );
		pVecOut->Set( m_pSceneNode->GetScale() );
		pVecOut->MulAssign( this->m_vecScale );
	}

	void IRenderableObject::CalcWorldRotation(Vec4* pVecOut)
	{
		xst_assert( pVecOut != xst_null, "(IRenderableObject::CalcWorldPosition)" );
		xst_assert( m_pSceneNode != xst_null, "(IRenderableObject::CalcWorldPosition) Scene node is not set" );
		//pVecOut->Set( m_pSceneNode->Getdi );
		//pVecOut->AddAssign( this->m_vecPosition );
		// TODO: implement
		pVecOut->Set( 0.0f );
	}

	void IRenderableObject::Update(cf32& fTime)
	{
		if( !this->IsDirty() )
			return;
		CObject::Update( fTime );
		//this->m_bDbgObject = false;
		xst_assert( m_pSceneNode != xst_null, "(IRenderableObject::SetPosition) Object is not in scene node" ); 
		
		IRenderSystem* pRS = m_pSceneNode->GetSceneManager()->GetRenderSystem();

		m_mtxTransform.Identity();

		Mtx4 mtxTranslate( Mtx4::IDENTITY ), mtxScale ( Mtx4::IDENTITY ), mtxRotation( Mtx4::IDENTITY );
		//pRS->SetTranslation( this->m_vecPosition.x, this->m_vecPosition.y, this->m_vecPosition.z );
		//pRS->GetMatrix( MatrixTypes::WORLD, &mtxTransform );
		CalcWorldPosition( &m_vecWorldPosition );
		mtxTranslate.Translate( m_vecWorldPosition.x, m_vecWorldPosition.y, m_vecWorldPosition.z );
		//pRS->SetTranslation( &mtxTranslate, m_vecWorldPosition );
		Vec3 vecScale, vecRotate;
		// TODO: implmenet scale and rotation
		f32 fAngle;
		Vec3 vecAxis;
		this->GetOrientation().ToAngleAxis( &fAngle, &vecAxis );
		//pRS->SetRotation( &mtxRotation, fAngle, vecAxis );
		//pRS->SetScale( &mtxScale, this->GetScale() );
		
		m_mtxTransform = mtxScale * ( mtxTranslate * mtxRotation );
	}

}//xse