#ifndef XSE_IRENDER_WINDOW_H
#define XSE_IRENDER_WINDOW_H

#include "XSECommon.h"

namespace XSE
{
	struct WindowStates
	{
		enum WINDOW_STATE
		{
			NORMAL = 1,
			MINIMIZED,
			MAXIMIZED,
			VISIBLE,
			HIDDEN
		};
	};

	struct SWindowEvent
	{
		IRenderWindow*	pWindow;
	};

	#define XSE_RW_KEYBOARD_EVENT(_name, _params)  this->m_iKeyboardListenersResult = 0; for(u32 i = 0; i < this->m_vKeyboardListeners.size(); ++i) this->m_iKeyboardListenersResult += this->m_vKeyboardListeners[ i ]->_name( _params )
	#define XSE_RW_MOUSE_EVENT(_name, _params) this->m_iMouseListenersResult = 0; for(u32 i = 0; i < this->m_vMouseListeners.size(); ++i) this->m_iMouseListenersResult += this->m_vMouseListeners[ i ]->_name( _params ) 
	#define XSE_RW_KEYBOARD_EVENT_E(_pWnd, _name, _params)  (_pWnd)->m_iKeyboardListenersResult = 0; for(u32 i = 0; i < (_pWnd)->m_vKeyboardListeners.size(); ++i) (_pWnd)->m_iKeyboardListenersResult += (_pWnd)->m_vKeyboardListeners[ i ]->_name( _params )
	#define XSE_RW_MOUSE_EVENT_E(_pWnd, _name, _params) (_pWnd)->m_iMouseListenersResult = 0; for(u32 i = 0; i < (_pWnd)->m_vMouseListeners.size(); ++i) (_pWnd)->m_iMouseListenersResult += (_pWnd)->m_vMouseListeners[ i ]->_name( _params ) 
	#define XSE_RW_EVENT(_name, _params) this->m_iListenersResult = 0; for(u32 i = 0; i < this->m_vListeners.size(); ++i) this->m_iListenersResult += this->m_vListeners[ i ]->_name( _params )
	#define XSE_RW_EVENT_E(_pWnd, _name, _params) (_pWnd)->m_iListenersResult = 0; for(u32 i = 0; i < (_pWnd)->m_vListeners.size(); ++i) (_pWnd)->m_iListenersResult += (_pWnd)->m_vListeners[ i ]->_name( _params )

	#define XSE_RW_KEYBOARD_EVENT2(_name, _param1, _param2)  this->m_iKeyboardListenersResult = 0; for(u32 i = 0; i < this->m_vKeyboardListeners.size(); ++i) this->m_iKeyboardListenersResult += this->m_vKeyboardListeners[ i ]->_name( (_param1), (_param2) )
	#define XSE_RW_MOUSE_EVENT2(_name, _param1, _param2) this->m_iMouseListenersResult = 0; for(u32 i = 0; i < this->m_vMouseListeners.size(); ++i) this->m_iMouseListenersResult += this->m_vMouseListeners[ i ]->_name( (_param1), (_param2) ) 
	#define XSE_RW_KEYBOARD_EVENT_E2(_pWnd, _name, _param1, _param2)  (_pWnd)->m_iKeyboardListenersResult = 0; for(u32 i = 0; i < (_pWnd)->m_vKeyboardListeners.size(); ++i) (_pWnd)->m_iKeyboardListenersResult += (_pWnd)->m_vKeyboardListeners[ i ]->_name( (_param1), (_param2) )
	#define XSE_RW_MOUSE_EVENT_E2(_pWnd, _name, _param1, _param2) (_pWnd)->m_iMouseListenersResult = 0; for(u32 i = 0; i < (_pWnd)->m_vMouseListeners.size(); ++i) (_pWnd)->m_iMouseListenersResult += (_pWnd)->m_vMouseListeners[ i ]->_name( (_param1), (_param2) ) 
	#define XSE_RW_EVENT2(_name, _param1, _param2) this->m_iListenersResult = 0; for(u32 i = 0; i < this->m_vListeners.size(); ++i) this->m_iListenersResult += this->m_vListeners[ i ]->_name( (_param1), (_param2) )
	#define XSE_RW_EVENT_E2(_pWnd, _name, _param1, _param2) (_pWnd)->m_iListenersResult = 0; for(u32 i = 0; i < (_pWnd)->m_vListeners.size(); ++i) (_pWnd)->m_iListenersResult += (_pWnd)->m_vListeners[ i ]->_name( (_param1), (_param2) )

	#define XSE_RW_KEYBOARD_EVENT3(_name, _param1, _param2, _param3)  this->m_iKeyboardListenersResult = 0; for(u32 i = 0; i < this->m_vKeyboardListeners.size(); ++i) this->m_iKeyboardListenersResult += this->m_vKeyboardListeners[ i ]->_name( (_param1), (_param2), (_param3) )
	#define XSE_RW_MOUSE_EVENT3(_name, _param1, _param2, _param3) this->m_iMouseListenersResult = 0; for(u32 i = 0; i < this->m_vMouseListeners.size(); ++i) this->m_iMouseListenersResult += this->m_vMouseListeners[ i ]->_name( (_param1), (_param2), (_param3) ) 
	#define XSE_RW_KEYBOARD_EVENT_E3(_pWnd, _name, _param1, _param2, _param3)  (_pWnd)->m_iKeyboardListenersResult = 0; for(u32 i = 0; i < (_pWnd)->m_vKeyboardListeners.size(); ++i) (_pWnd)->m_iKeyboardListenersResult += (_pWnd)->m_vKeyboardListeners[ i ]->_name( (_param1), (_param2), (_param3) )
	#define XSE_RW_MOUSE_EVENT_E3(_pWnd, _name, _param1, _param2, _param3) (_pWnd)->m_iMouseListenersResult = 0; for(u32 i = 0; i < (_pWnd)->m_vMouseListeners.size(); ++i) (_pWnd)->m_iMouseListenersResult += (_pWnd)->m_vMouseListeners[ i ]->_name( (_param1), (_param2), (_param3) ) 
	#define XSE_RW_EVENT3(_name, _param1, _param2, _param3) this->m_iListenersResult = 0; for(u32 i = 0; i < this->m_vListeners.size(); ++i) this->m_iListenersResult += this->m_vListeners[ i ]->_name( (_param1), (_param2), (_param3) )
	#define XSE_RW_EVENT_E3(_pWnd, _name, _param1, _param2, _param3) (_pWnd)->m_iListenersResult = 0; for(u32 i = 0; i < (_pWnd)->m_vListeners.size(); ++i) (_pWnd)->m_iListenersResult += (_pWnd)->m_vListeners[ i ]->_name( (_param1), (_param2), (_param3) )

	class XST_API IRenderWindow
	{
		
		protected:

			friend class CEngine;

			xst_tvectorx( IKeyboardListener* )		KeyboardListenerVector;
			xst_tvectorx( IMouseListener* )			MouseListenerVector;
			xst_tvectorx( IRenderWindowListener* )	ListenerVector;

		public:

									IRenderWindow() :	m_uiWndHandle( 0 ), m_bIsWndClosing( false ), m_uiWndWidth( 800 ),
														m_uiWndHeight( 600 ), m_uiWndPosX( 0 ), m_uiWndPosY( 0 ),
														m_iKeyboardListenersResult( 0 ), m_iMouseListenersResult( 0 ),
														m_iListenersResult( 0 ), m_pRenderSystem( xst_null ){}
			virtual					~IRenderWindow() 
									{
										RemoveMouseListeners();
										RemoveListeners();
									}

			virtual	i32				Create(u32 uiExternalHandle = 0) = 0;

			virtual void			Close() = 0;

			virtual	void			SetSize(u32 uiWidth, u32 uiHeight) = 0;

			virtual void			SetPosition(u32 uiPosX, u32 uiPosY) = 0;

			virtual	void			Show(bool bShow) = 0;

			virtual	bool			IsVisible() = 0;

			virtual	i32				Update() = 0;

			virtual void			SetTitle(xst_castring& strTitle) = 0;

			virtual i32				ProcessMessages() = 0;

			virtual	i32				BeginRenderFrame() = 0;

			virtual i32				EndRenderFrame() = 0;

			virtual i32				RenderFrame() = 0;

			virtual f32				GetFrameTime() = 0;

			virtual void			StopRendering() = 0;

			virtual IKeyboard*		GetKeyboard() const = 0;

			virtual IMouse*			GetMouse() const = 0;

			xst_fi	u32				GetWidth()
									{ return m_uiWndWidth; }

			xst_fi	u32				GetHeight()
									{ return m_uiWndHeight; }

			xst_fi	u32				GetHandle()
									{ return m_uiWndHandle; }

			xst_fi	u32				GetPositionX()
									{ return m_uiWndPosX; }

			xst_fi	u32				GetPositionY()
									{ return m_uiWndPosY; }

			xst_fi	xst_castring&	GetTitle()
									{ return m_strWndTitle; }

			xst_fi	bool			IsClosing()
									{ return m_bIsWndClosing; }

					void			AddMouseListener(IMouseListener* pListener);

					void			AddListener(IRenderWindowListener* pListener);
					
					void			RemoveMouseListeners();

					void			RemoveListeners();

					IRenderSystem*	GetRenderSystem() const
									{ return m_pRenderSystem; }


		protected:

			xst_fi	i32				_GetKeyboardListenersResult()
									{ return m_iKeyboardListenersResult; }

			xst_fi	i32				_GetMouseListenersResult()
									{ return m_iMouseListenersResult; }

			xst_fi	i32				_GetListenersResult()
									{ return m_iListenersResult; }

			xst_fi	void			_SetRenderSystem(IRenderSystem* pSystem)
									{ xst_assert( pSystem, "(IRenderWindow::_SetRenderSystem) Render system is null" ); m_pRenderSystem = pSystem; }

		protected:

			u32				m_uiWndHandle;
			xst_astring		m_strWndTitle;
			u32				m_uiWndWidth;
			u32				m_uiWndHeight;
			u32				m_uiWndPosX;
			u32				m_uiWndPosY;
			bool			m_bIsWndClosing;
			i32				m_iMouseListenersResult;
			i32				m_iKeyboardListenersResult;
			i32				m_iListenersResult;
			IRenderSystem*	m_pRenderSystem;

			KeyboardListenerVector	m_vKeyboardListeners;
			MouseListenerVector		m_vMouseListeners;
			ListenerVector			m_vListeners;
	};

}//xse

#endif