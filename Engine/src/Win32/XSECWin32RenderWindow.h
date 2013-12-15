#ifndef XSE_CWIN32_RENDER_WINDOW_H
#define XSE_CWIN32_RENDER_WINDOW_H

#include "XSEIRenderWindow.h"
#include "XSEIMouseListener.h"
#include "XSEIKeyboardListener.h"

#if defined (XST_WINDOWS)
namespace XSE
{
	namespace Win32
	{
		class CKeyboard;
		class CMouse;

		class CRenderWindow : public IRenderWindow
		{
			friend LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

			public:
										CRenderWindow(CEngine* pEngine);

				virtual					~CRenderWindow();

						i32				Create(u32 uiExternalHandle = 0);

						void			SetSize(u32 uiWidth, u32 uiHeight);

						void			SetPosition(u32 uiPosX, u32 uiPosY);

						void			Show(bool bShow);

						bool			IsVisible();

						i32				Update();

						void			SetTitle(xst_castring& strTitle);

						i32				ProcessMessages();

						i32				BeginRenderFrame();

						i32				EndRenderFrame();

						i32				RenderFrame();

						f32				GetFrameTime();

						void			StopRendering();

						void			Close();

						IKeyboard*		GetKeyboard() const
										{ return (IKeyboard*)m_pKeyboard; }

						IMouse*			GetMouse() const
										{ return (IMouse*)m_pMouse; }

			protected:

						void			_OnWMSize(cu32& uiWidth, cu32& uiHeight);		

			protected:

				HWND		m_hWnd;
				MSG			m_WndMsg;
				bool		m_bVisible;
				CEngine*	m_pEngine;
				XST::CTimer	m_Timer; //for each frame
				XST::CTimer	m_GlobalTimer; //from app start
				CKeyboard*	m_pKeyboard;
				CMouse*		m_pMouse;
				u32			m_auiKeyCodes[ KeyCodes::_ENUM_COUNT ]; //mapping engine to windows
				u32			m_auiWinKeyCodes[ 256 ]; //maping windows to engine
				f32			m_fFrameTime;
		};
	}//win32
}//xse
#endif //XST_WINDOWS

#endif