#include "../../include/xst/XSTCColor.h"
#include "XST/XSTCRandom.h"

#define XST_FROM_RGBA(_uiRGBA, _r, _g, _b, _a)  ( (_r) << (unsigned)_uiRGBA )
#define XST_TO_RGBA(_r, _g, _b, _a)

namespace XST
{
	CColor::CColor(cu32& uiRGBA)
	{

	}

	u32 CColor::ToRGBA(cu8& uR, cu8& uG, cu8& uB, cu8& uA)
	{
		return 0;
	}

	CColor CColor::FromRGBA(cu32& uiRGBA)
	{
		return 0;
	}

	u32 CColor::ToRGBA()
	{
		return 0;
	}

	CColor	CColor::FromFloats(cf32& fR, cf32& fG, cf32& fB, cf32& fA)
	{
		return CColor( (u8)( fR * 255.0f ), (u8)( fG * 255.0f ), (u8)( fB * 255.0f ), (u8)( fA * 255.0f ) );
	}

	CColor		CColor::Random()
	{
		return CColor(	XST::Random::GetNextIntInRange( 0, 255 ), XST::Random::GetNextIntInRange( 0, 255 ), 
						XST::Random::GetNextIntInRange( 0, 255 ), XST::Random::GetNextIntInRange( 0, 255 ) );
	}

}//xst