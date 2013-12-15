#include "GUI/XSECGUIManager.h"
#include "GUI/XSECComponent.h"
#include "GUI/XSEIGUIRenderer.h"
#include "XSECMaterialManager.h"
#include "XSECMeshManager.h"

namespace XSE
{
	namespace GUI
	{
		void AddComponents(IGUIRenderer* pRenderer, CComponent* pCmp);

		CGUIManager::CGUIManager() :
			m_pMainComp( xst_null ),
			m_pRenderer( xst_null )
		{
		}

		CGUIManager::~CGUIManager()
		{
			xst_delete( m_pMainComp );
			xst_delete( m_pRenderer );
		}


		i32 CGUIManager::Init(IRenderSystem* pRS, CSceneManager* pSceneMgr, CMeshManager* pMeshMgr)
		{
			m_pRenderer = xst_new IGUIRenderer( pRS, pSceneMgr, pMeshMgr );
			m_pMainComp = xst_new CComponent( ComponentStyles::NONE, ComponentTypes::UNKNOWN, m_pRenderer, pMeshMgr->GetMaterialManager(), xst_null );
			if( m_pMainComp == xst_null )
			{
				XST_LOG_ERR( "Failed to create the gui main component. Memory error." );
				return XST_FAIL;
			}

			return XST_OK;
		}

		i32 CGUIManager::Build(IInputLayout* pIL, xst_castring& strGroup)
		{
			CComponent::ComponentIterator Itr = m_pMainComp->GetChildIterator();
			while( Itr.HasMoreElements() )
			{
				AddComponents( m_pRenderer, Itr.GetValue() );
				Itr.MoveNext();
			}

			return m_pRenderer->BuildGUI( pIL, strGroup );
		}

		void AddComponents(IGUIRenderer* pRenderer, CComponent* pCmp)
		{
			if( pCmp )
			{
				pRenderer->AddComponent( pCmp );
			}
		}

	}//gui
}//xse