#include "../include/XSECGeometryBatch.h"
#include "../include/XSEIRenderSystem.h"

namespace XSE
{
	CGeometryBatch::CGeometryBatch(IRenderSystem* pRS, CSceneManager* pMgr, lpcastr strName) : 
		IRenderableObject( OT::GEOMETRY_BATCH, xst_null, strName ), 
		m_pSceneMgr( pMgr ),
		m_RenderMethod( &CGeometryBatch::_RenderNormal )
	{

	}

	CGeometryBatch::~CGeometryBatch()
	{

	}

	void CGeometryBatch::Disable(cu32& uiReason)
	{
		this->m_uiObjDisableReason = uiReason;

		if( !this->IsDisabled() )
		{
			this->SetVisible( this->IsVisible() );
		}
		else
		{
			m_RenderMethod = &CGeometryBatch::_RenderEmpty;
		}
	}

	void CGeometryBatch::SetVisible(bool bVisible)
	{
		this->m_bVisible = bVisible;

		if( bVisible && !this->IsDisabled() )
		{
			m_RenderMethod = &CGeometryBatch::_RenderNormal;
		}
		else
		{
			m_RenderMethod = &CGeometryBatch::_RenderEmpty;
		}
	}

	i32 CGeometryBatch::_RenderNormal()
	{
		return XST_OK;
	}

}//xse