#ifndef XSE_CWIN32_MOUSE_H
#define XSE_CWIN32_MOUSE_H

#include "XSEIMouse.h"

namespace XSE
{
	namespace Win32
	{
		class CRenderWindow;

		class CMouse : public IMouse
		{
			friend class CRenderWindow;
			friend LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

			typedef xst_vector< IMouseListener* >	ListenerVector;
			typedef ListenerVector::iterator		ListenerItr;
			typedef void (CMouse::*pfnCapture)();

			public:

										CMouse(CRenderWindow* pWnd);
				virtual					~CMouse();

						i32				Init();

						void			AddListener(IMouseListener* pListener)
										{ m_vListeners.push_back( pListener ); }

				xst_fi	void			RemoveListener(IMouseListener* pListener, bool bDestroy = true)
										{ 
											xst_assert2( pListener != xst_null );
											if( m_vListeners.size() > 0 )
											{
												m_vListeners.erase( std::find( m_vListeners.begin(), m_vListeners.end(), pListener ) ); 
												if( bDestroy && pListener->IsMouseListenerAutoDestroy() )
												{
													delete pListener;
												}
											}
										}

						void			RemoveListeners(bool bDestroy);

						bool			IsCapturedOutsideTheWindow() const
										{ return _CaptureMouse == &CMouse::_CaptureOtsideTheWindow; }

						void			IsCapturedOutsideTheWindow(bool bIs);

				const	CPoint&			GetPosition() const
										{ return m_Event.Position; }

				const	CPoint&			GetDelta() const
										{ return m_Event.Delta; }

			protected:

						void			_OnMouseMove(const WPARAM& wParam, const LPARAM& lParam);
						void			_OnMouseLButtonDown(const WPARAM& wParam, const LPARAM& lParam);
						void			_OnMouseLButtonUp(const WPARAM& wParam, const LPARAM& lParam);
						void			_OnMouseRButtonDown(const WPARAM& wParam, const LPARAM& lParam);
						void			_OnMouseRButtonUp(const WPARAM& wParam, const LPARAM& lParam);
						void			_OnMouseMButtonDown(const WPARAM& wParam, const LPARAM& lParam);
						void			_OnMouseMButtonUp(const WPARAM& wParam, const LPARAM& lParam);
						void			_OnMouseWheelDown(const WPARAM& wParam, const LPARAM& lParam);
						void			_OnMouseWheelUp(const WPARAM& wParam, const LPARAM& lParam);
						void			_OnMouseLButtonDblClick(const WPARAM& wParam, const LPARAM& lParam);
						void			_OnMouseRButtonDblClick(const WPARAM& wParam, const LPARAM& lParam);
						void			_OnMouseMButtonDblClick(const WPARAM& wParam, const LPARAM& lParam);

						void			_CaptureOtsideTheWindow();
						void			_CaptureInWindow();

						pfnCapture		_CaptureMouse;

			protected:

				ListenerVector	m_vListeners;
				ListenerItr		m_ListenerItr;
				bool			m_bIsCaptured;
				SMouseEvent		m_Event;
				CRenderWindow*	m_pWnd;
				POINT			m_WinPoint;
		};
	}//win32
}//xse

#endif