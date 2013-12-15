#include "../include/XSECRenderSystemFactory.h"

namespace XSE
{

	i32 CRenderSystemFactory::RegisterRenderSystem(xst_castring& strName, XST_PFNCREATERENDERSYSTEM pfnCreateRenderSystem)
	{
		RenderSystemMap::iterator Itr = m_mRenderSystems.find( strName );
		if( Itr != m_mRenderSystems.end() )
		{
			return RESULT::FAILED;
		}

		m_mRenderSystems.insert( RenderSystemMap::value_type( strName, pfnCreateRenderSystem ) );

		return RESULT::OK;
	}

	IRenderSystem* CRenderSystemFactory::CreateRenderSystem(xst_castring& strName)
	{
		RenderSystemMap::iterator Itr = m_mRenderSystems.find( strName );
		if( Itr == m_mRenderSystems.end() )
		{
			return xst_null;
		}

		XST_PFNCREATERENDERSYSTEM pfnCreateRenderSystem = Itr->second;
		return pfnCreateRenderSystem();
	}

}//xse

