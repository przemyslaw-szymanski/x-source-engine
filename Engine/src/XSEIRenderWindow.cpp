#include "../include/XSEIRenderWindow.h"
#include "../include/XSEIKeyboardListener.h"
#include "../include/XSEIMouseListener.h"

namespace XSE
{
	void IRenderWindow::AddMouseListener(IMouseListener* pListener)
	{
		xst_assert( pListener, "(IRenderWindow::AddMouseListener) Listener is null" );
		m_vMouseListeners.push_back( pListener );
	}

	void IRenderWindow::AddListener(IRenderWindowListener* pListener)
	{
		xst_assert( pListener, "(IRenderWindow::AddListener) Listener is null" );
		m_vListeners.push_back( pListener );
	}			
					
	void IRenderWindow::RemoveMouseListeners()
	{
		for(u32 i = m_vMouseListeners.size(); i --> 0;)
		{
			//xst_delete( m_vMouseListeners[ i ] );
		}

		m_vMouseListeners.clear();
	}

	void IRenderWindow::RemoveListeners()
	{
		for(u32 i = m_vListeners.size(); i --> 0;)
		{
			//xst_delete( m_vListeners[ i ] );
		}

		m_vListeners.clear();
	}

}//xse