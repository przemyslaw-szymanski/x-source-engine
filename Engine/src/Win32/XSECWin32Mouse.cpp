#include "XSECWin32Mouse.h"
#include "XSEIMouseListener.h"
#include "XSECWin32RenderWindow.h"

namespace XSE
{
	namespace Win32
	{
		#define XSE_WIN32_ME(_methodName) \
			xst_stl_foreach( m_ListenerItr, m_vListeners ) (*m_ListenerItr)->_methodName( m_Event )


		CMouse::CMouse(CRenderWindow* pWnd) :
			m_pWnd( pWnd ),
			_CaptureMouse( &CMouse::_CaptureInWindow )
		{
			xst_zero( &m_Event, sizeof( SMouseEvent ) );
		}

		CMouse::~CMouse()
		{
			RemoveListeners( true );
		}

		i32 CMouse::Init()
		{
			return XST_OK;
		}

		void CMouse::RemoveListeners(bool bDestroy)
		{
			xst_stl_foreach( m_ListenerItr, m_vListeners )
			{
				RemoveListener( (*m_ListenerItr), bDestroy );
			}

			m_vListeners.clear();
		}

		void CMouse::IsCapturedOutsideTheWindow(bool bIs)
		{
			if( bIs )
			{	
				_CaptureMouse = &CMouse::_CaptureOtsideTheWindow;
			}
			else
			{
				_CaptureMouse = &CMouse::_CaptureInWindow;
			}
		}

		void CMouse::_OnMouseMove(const WPARAM& wParam, const LPARAM& lParam)
		{
			//Capture mouse
			XST_CALL_MEMBER_FN_PTR( this, _CaptureMouse )();

			GetCursorPos( &m_WinPoint );
			m_Event.Delta.x = m_WinPoint.x - m_Event.Position.x;
			m_Event.Delta.y = m_WinPoint.y - m_Event.Position.y;
			m_Event.Position.x = m_WinPoint.x;
			m_Event.Position.y = m_WinPoint.y;

			XSE_WIN32_ME( OnMouseMove );
		}

		void CMouse::_CaptureOtsideTheWindow()
		{
			//SetCapture( (HWND)m_pWnd->GetHandle() );
		}

		void CMouse::_CaptureInWindow()
		{
			//ReleaseCapture();
		}

		void CMouse::_OnMouseLButtonDown(const WPARAM& wParam, const LPARAM& lParam)
		{
			m_Event.uiButtonDown |= MouseButtons::LEFT;
			XSE_WIN32_ME( OnMouseButtonDown );
		}

		void CMouse::_OnMouseLButtonUp(const WPARAM& wParam, const LPARAM& lParam)
		{
			m_Event.uiButtonDown &= ~MouseButtons::LEFT;
			XSE_WIN32_ME( OnMouseButtonUp );
		}

		void CMouse::_OnMouseRButtonDown(const WPARAM& wParam, const LPARAM& lParam)
		{
			m_Event.uiButtonDown |= MouseButtons::RIGHT;
			XSE_WIN32_ME( OnMouseButtonDown );
		}

		void CMouse::_OnMouseRButtonUp(const WPARAM& wParam, const LPARAM& lParam)
		{
			m_Event.uiButtonDown &= ~MouseButtons::RIGHT;
			XSE_WIN32_ME( OnMouseButtonUp );
		}

		void CMouse::_OnMouseMButtonDown(const WPARAM& wParam, const LPARAM& lParam)
		{
			m_Event.uiButtonDown |= MouseButtons::MIDDLE;
			XSE_WIN32_ME( OnMouseButtonDown );
		}

		void CMouse::_OnMouseMButtonUp(const WPARAM& wParam, const LPARAM& lParam)
		{
			m_Event.uiButtonDown &= ~MouseButtons::MIDDLE;
			XSE_WIN32_ME( OnMouseButtonUp );
		}

		void CMouse::_OnMouseWheelDown(const WPARAM& wParam, const LPARAM& lParam)
		{
		}

		void CMouse::_OnMouseWheelUp(const WPARAM& wParam, const LPARAM& lParam)
		{
		}

		void CMouse::_OnMouseLButtonDblClick(const WPARAM &wParam, const LPARAM &lParam)
		{
		}

		void CMouse::_OnMouseRButtonDblClick(const WPARAM &wParam, const LPARAM &lParam)
		{
		}

		void CMouse::_OnMouseMButtonDblClick(const WPARAM &wParam, const LPARAM &lParam)
		{
		}

	}//win32
}//xse