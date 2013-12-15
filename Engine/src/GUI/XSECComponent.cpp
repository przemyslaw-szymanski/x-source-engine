#include "GUI/XSECComponent.h"
#include "XSECMaterialManager.h"
#include "GUI/XSEIGUIRenderer.h"

namespace XSE
{
	namespace GUI
	{
		CComponent::CComponent(ul32 ulStyles, COMPONENT_TYPE eType, IGUIRenderer* pRenderer, CMaterialManager* pMatMgr, CComponent* pParent) : 
			m_lHandle( -1 ),
			m_pParent( pParent ),
			m_eCompType( eType ),
			m_eMetricsType( CMTs::PIXEL ),
			m_vecPosition( Vec3::ZERO ),
			m_vecSize( 100, 100 ),
			m_ulStyles( ulStyles ),
			m_pMaterial( pMatMgr->GetDefaultMaterial() ),
			m_pGUIRenderer( pRenderer )
		{
		}

		CComponent::~CComponent()
		{
			DestroyChildren();
		}

		void CComponent::DestroyChildren()
		{
			CompVector::iterator Itr;
			xst_stl_foreach( Itr, m_vChildren )
			{
				xst_delete( (*Itr) );
			}

			m_vChildren.clear();
		}

		void CComponent::Update()
		{
			CompVector::iterator Itr;
			xst_stl_foreach( Itr, m_vChildren )
			{
				(*Itr)->Update();
			}
		}

		CComponent* CComponent::CreateComponent()
		{
			CComponent* pCmp = xst_new CComponent( ComponentStyles::NONE, ComponentTypes::UNKNOWN, m_pGUIRenderer, (CMaterialManager*)m_pMaterial->GetResourceCreator(), this );
			if( pCmp == xst_null )
			{
				return xst_null;
			}

			if( XST_FAILED( AddChild( pCmp ) ) )
			{
				xst_delete( pCmp );
				return xst_null;
			}

			return pCmp;
		}

		void CComponent::SetPosition(cu32& uiX, cu32& uiY)
		{
			m_vecPosition.x = (f32)uiX;
			m_vecPosition.y = -(f32)uiY; //Y axis is inverse
			if( m_pParent )
			{
				m_vecPosition.z = m_pParent->GetPosition().z + 0.001f;
			}
			m_vecPosition.z = 1.0f;
			m_pGUIRenderer->SetComponentPosition( this );
		}

		void CComponent::SetSize(cu32& uiWidth, cu32& uiHeight)
		{
			m_vecSize.x = (f32)uiWidth;
			m_vecSize.y = (f32)uiHeight;
		}

		i32 CComponent::AddChild(XSE::GUI::CComponent *pComp)
		{
			xst_assert( pComp, "(CComponent::AddChild)" );

			if( pComp->GetParent() != xst_null && pComp->GetParent() != this )
			{
				XST_LOG_ERR( "Component: " << pComp->GetHandle() << " already has a parent" );
				return XST_FAIL;
			}

			pComp->m_lHandle = m_vChildren.size();
			//Add component
			m_vChildren.push_back( pComp );
			//Set position
			pComp->m_pParent = this;
			pComp->SetPosition( (u32)(this->m_vecPosition.x + pComp->m_vecPosition.x), (u32)(this->m_vecPosition.y + pComp->m_vecPosition.y) );

			return XST_OK;
		}

		void CComponent::UpdateSize()
		{
			CompVector::iterator Itr;
			xst_stl_foreach( Itr, m_vChildren )
			{
				//(*Itr)->SetMetricsType( m_eMetricsType );
				(*Itr)->UpdateSize();
			}
		}

		void CComponent::UpdatePosition()
		{
			CompVector::iterator Itr;
			xst_stl_foreach( Itr, m_vChildren )
			{
				//(*Itr)->SetMetricsType( m_eMetricsType );
				(*Itr)->UpdatePosition();
			}
		}

		CComponent::ComponentIterator CComponent::GetChildIterator()
		{
			return ComponentIterator( &m_vChildren );
		}

		void CComponent::SetMetricsType(XSE::GUI::COMPONENT_METRICS_TYPE eType)
		{
			m_eMetricsType = eType;
			UpdateSize();
		}

		void CComponent::UpdateChild()
		{
		}

		void CComponent::UpdateChildren()
		{
		}

		CComponent* GetTopParent(CComponent* pComp);

		CComponent*	CComponent::GetTopParent() 
		{
			return GUI::GetTopParent( this );
		}


		CComponent* GetTopParent(CComponent* pComp)
		{
			if( pComp )
			{
				if( pComp->GetParent() )
				{
					return GetTopParent( pComp->GetParent() );
				}
				else
				{
					return pComp;
				}
			}

			return xst_null;
		}

	}//gui
}//xse