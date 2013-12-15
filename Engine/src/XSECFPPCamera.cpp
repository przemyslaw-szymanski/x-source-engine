#include "XSECFPPCamera.h"

namespace XSE
{
	CFPPCamera::CFPPCamera(CSceneManager* pSceneMgr, xst_castring& strName, ul32 ulHandle) : 
		CCamera( pSceneMgr, strName, ulHandle ),
		m_mouseDelta( 0, 0 )
	{
		m_vecDirection = Vec3( 0, 0, 1 );
	}

	CFPPCamera::~CFPPCamera()
	{

	}

	//const Vec3& CFPPCamera::Move(cf32& fX, cf32& fY, cf32& fZ)
	//{
	//	cf32 fConst = this->m_fMoveSpeed * this->m_fTime;
	//	/*this->m_vecCamPosition.x += ( ( fX * this->m_vecRight.x ) + this->m_vecCamDirection.x ) * fConst;
	//	this->m_vecCamPosition.y += ( ( fY * this->m_vecRight.y ) + this->m_vecCamDirection.y ) * fConst;
	//	this->m_vecCamPosition.z += ( ( fZ * this->m_vecRight.z ) + this->m_vecCamDirection.z ) * fConst;*/
	//	const Vec3 vecRight( this->m_vecRight * fX +  );
	//	this->m_vecCamPosition += ( this->m_vecRight * fX + this->m_vecUp * fY + this->m_vecCamDirection * fZ ) * fConst;
	//	return this->m_vecCamPosition;
	//}

	void CFPPCamera::Update(cf32& fElapsedTime)
	{
		_Update();
		CCamera::Update( fElapsedTime );
	}

	void CFPPCamera::SetProperty(cu32& uiProperty, xst_unknown pProp)
	{
		switch( uiProperty )
		{
			case FPPCameraProperties::MOUSE_DELTA:
			{
				_SetMouseDelta( *(CPoint*)pProp );
			}
			break;
		}
	}

	void CFPPCamera::_Update()
	{
		//CPoint delta( m_mouseDelta - m_lastMousePos );
		
		//m_lastMousePos = m_mouseDelta;
		this->RotateX( -m_mouseDelta.x /** this->m_fHorizontalSpeed*//*(f32)delta.x /** this->m_fHorizontalSpeed*/ );
		this->RotateY( -m_mouseDelta.y /** this->m_fHorizontalSpeed*//*(f32)delta.y /** this->m_fVerticalSpeed*/ );
		m_mouseDelta.x = m_mouseDelta.y = 0;
	}

}//xse