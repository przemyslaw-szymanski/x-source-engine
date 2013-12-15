#include "../../include/XST/XST.h"

namespace XST
{
	xst_castring StringUtil::EmptyAString = "";
	xst_cwstring StringUtil::EmptyUString = L"";

	xst_astring CLastError::m_strError = "OK";

	const CColor CColor::BLACK( 0, 0, 0, 0 );
	const CColor CColor::WHITE( 255, 255, 255, 255 );
	const CColor CColor::BLUE( 0, 0, 255, 0);
	const CColor CColor::RED( 255, 0, 0, 0 );
	const CColor CColor::GREEN( 0, 255, 0, 0 );

}//xst
