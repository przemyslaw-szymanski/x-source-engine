#ifndef XSE_CFPP_CAMERA_H
#define XSE_CFPP_CAMERA_H

#include "../include/XSECCamera.h"

namespace XSE
{
	class CFPPCamera : public CCamera
	{
		public:

								CFPPCamera(CSceneManager* pSceneMgr, xst_castring& strName, ul32 ulHandle);
			virtual				~CFPPCamera();

					void		Update(cf32& fElapsedTime);

			//const	Vec3&		Move(cf32& fX, cf32& fY, cf32& fZ);

					void		SetProperty(cu32& uiProp, xst_unknown pProperty);

		protected:

			xst_fi	void		_SetMouseDelta(const CPoint& mouseDelta)
								{ m_mouseDelta = mouseDelta; }

					void		_Update();

		protected:

			CPoint		m_mouseDelta;
	};
}//xse

#endif