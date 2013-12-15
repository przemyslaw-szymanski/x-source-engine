#include "XSECRenderLayer.h"

namespace XSE
{
	CRenderLayer::CRenderLayer(xst_castring& strName, cu32 uiPriority) :
		IRenderLayer( strName, uiPriority )
	{

	}
									
	CRenderLayer::~CRenderLayer()
	{
		for(u32 i = 0; i < m_aLayers.size(); ++i)
		{
			xst_delete( m_aLayers[ i ] );
		}

		m_aLayers.clear();
		xst_vector_clear( m_vObjects, IRenderableObject* );
	}

			
	i32	CRenderLayer::AddObject(IRenderableObject* pObj)
	{
		m_vObjects.push_back( pObj );
		return XST_OK;
	}
	
#define xst_vector_erase_ex(_vector, _elemType, _itr) \
	{ \
		_elemType _el = _vector.back(); \
		(*_itr) = _el; _vector.pop_back(); \
	}

#define xst_vector_erase(_vector, _elemType, _elem) \
	{ \
		xst_vector< _elemType >::iterator _itr = xst_find_ex( _vector, _elem ); \
		if( _itr != _vector.end() ) { xst_vector_erase_ex( _vector, _elemType, _itr ); } \
	}

	void CRenderLayer::RemoveObject(IRenderableObject* pObj)
	{
		xst_vector_erase( m_vObjects, IRenderableObject*, pObj );
	}
			
	void CRenderLayer::Render()
	{
		if( !this->m_bEnabled )
			return;
	}
			
	void CRenderLayer::RenderChildLayer(xst_castring& strChildName)
	{
		IRenderLayer* pLayer = GetLayer( strChildName );
		if( pLayer )
		{
			pLayer->Render();
		}
	}

			
	void CRenderLayer::RenderChildLayer(cu32& uiPriority)
	{
		IRenderLayer* pLayer = GetLayer( uiPriority );
		if( pLayer )
		{
			pLayer->Render();
		}
	}

			
	IRenderLayer*	CRenderLayer::GetLayer(xst_castring& strName)
	{
		for(i32 i = m_aLayers.size(); i --> 0;)
		{
			if( m_aLayers[ i ]->GetName() == strName )
				return m_aLayers[ i ];
		}
		return xst_null;
	}

			
	IRenderLayer*	CRenderLayer::GetLayer(cu32& uiPriority)
	{
		for(i32 i = m_aLayers.size(); i --> 0;)
		{
			if( m_aLayers[ i ]->GetPriority() == uiPriority )
				return m_aLayers[ i ];
		}
		return xst_null;
	}

			
	IRenderLayer* CRenderLayer::CreateLayer(cu32& uiPriority, xst_castring& strName)
	{
		CRenderLayer* pLayer = xst_new CRenderLayer( strName, uiPriority );
		if( pLayer == xst_null )
		{
			XST_LOG_ERR( "Unable to create new layer: " << strName << ". Memory error" );
			return xst_null;
		}

		InsertLayer( pLayer );

		return pLayer;
	}

	void CRenderLayer::SetParent(IRenderLayer* pParent)
	{
		xst_assert( this->m_pParentLayer == xst_null, "(CRenderLayer::SetParent) This layer already has a parent" );
		xst_assert( pParent != xst_null, "(CRenderLayer::SetParent)" );

		this->m_pParentLayer = pParent;
	}
			
	i32 CRenderLayer::InsertLayer(IRenderLayer* pLayer)
	{
		if( pLayer->GetParent() != xst_null )
			return XST_FAIL; //this layer already exists in this or some other parent layer

		pLayer->SetParent( this );
		m_aLayers.push_back( pLayer );
		return XST_OK;
	}

}//xse