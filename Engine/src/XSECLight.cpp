#include "XSECLight.h"

namespace XSE
{
	CLight::CLight(xst_castring& strName, ul32 uHandle, CObject* pParent) :
		XST::IObject( strName )
		, CObject( ObjectTypes::LIGHT, uHandle, strName.c_str(), pParent )
	{

	}

	CLight::~CLight()
	{}

} // xse