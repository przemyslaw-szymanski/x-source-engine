#include "XSECTechnique.h"
#include "XSECPass.h"

namespace XSE
{
	CTechnique::CTechnique(xst_castring& strName) : ITechnique( strName )
	{
	}

	CTechnique::~CTechnique()
	{
		
	}

	IPass* CTechnique::_CreatePass(xst_castring &strName)
	{
		return xst_new CPass( strName );
	}

	void CTechnique::_DestroyPass(IPass* pPass)
	{
		CPass* pPass2 = (CPass*)pPass;
		xst_delete( pPass2 );
	}

}//xse