#include "XSECWin32RenderWindow.h"
#include "XSECEngine.h"
#include "XSEIRenderWindowListener.h"
#include "XSEIKeyboardListener.h"
#include "XSEIMouseListener.h"
#include "XSECSceneManager.h"
#include "XSECWin32Keyboard.h"
#include "XSECWin32Mouse.h"

#if defined (XST_WINDOWS)
namespace XSE
{
	namespace Win32
	{
		SMouseEvent g_MouseEvent;
		SKeyboardEvent g_KeyboardEvent;

		LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			CRenderWindow* pWnd = (CRenderWindow*)XSE::CEngine::GetSingletonPtr()->GetRenderWindow( (ul32)hWnd );
			if( pWnd == xst_null )
			{
				XST_LOG_ERR( "Window is not created" );
				return 1;
			}

			CKeyboard* pKeyboard = pWnd->m_pKeyboard;
			CMouse* pMouse = pWnd->m_pMouse;

			HDC hDC;
			PAINTSTRUCT PS;

			g_KeyboardEvent.pWnd = pWnd;

			switch(uMsg)
			{
				case WM_PAINT:
				{
					hDC = BeginPaint( hWnd, &PS );
					EndPaint( hWnd, &PS );
				}
				break;
				// If the user wants to close the application
				case WM_DESTROY:
				{
					XSE::SWindowEvent Event = { pWnd };
					XSE_RW_EVENT_E( pWnd, OnClose, Event ); 

					// then close it
					PostQuitMessage(WM_QUIT);
				}
				break;

				case WM_SIZE:
				{
					pWnd->_OnWMSize( LOWORD( lParam ), HIWORD( lParam ) );
				}
				break;

				case WM_CHAR:
				{
					pKeyboard->_OnChar( wParam, lParam );
				}
				break;

				case WM_SYSKEYDOWN:
				{
					pKeyboard->_OnSysKeyDown( wParam, lParam );
				}
				break;

				case WM_SYSKEYUP:
				{
					pKeyboard->_OnSysKeyUp( wParam, lParam );
				}
				break;

				case WM_KEYDOWN:
				{	
					pKeyboard->_OnKeyDown( wParam, lParam );
				}
				break;

				case WM_KEYUP:
				{
					pKeyboard->_OnKeyUp( wParam, lParam );
				}
				break;

				case WM_MOVE:
				{
					u32 uiPosX = pWnd->GetPositionX();
					u32 uiPosY = pWnd->GetPositionY();
					XSE::SWindowEvent WndEvt = { pWnd };
					XSE_RW_EVENT_E3( pWnd, OnMove, WndEvt, LOWORD( wParam ), HIWORD( wParam ) ); 
					if( pWnd->m_iListenersResult > 0 )
					{
						SetWindowPos( pWnd->m_hWnd, HWND_TOP, uiPosX, uiPosY, pWnd->m_uiWndWidth, pWnd->m_uiWndHeight, SWP_NOSIZE | SWP_NOZORDER );
					}
				}
				break;

				case WM_INPUT:
				{
					pKeyboard->_OnInput( wParam, lParam );
				}
				break;

				//MOUSE
				case WM_MOUSEMOVE:
				{
					//POINT Point;
					//GetCursorPos( &Point );
					////ScreenToClient( pWnd->m_hWnd, &Point );
					//g_MouseEvent.iDeltaX = Point.x - g_MouseEvent.uiPositionX;
					//g_MouseEvent.iDeltaY = Point.y - g_MouseEvent.uiPositionY;
					//g_MouseEvent.uiPositionX = Point.x;
					//g_MouseEvent.uiPositionY = Point.y;
				
					//XSE_RW_MOUSE_EVENT_E( pWnd, OnMouseMove, g_MouseEvent );
					pMouse->_OnMouseMove( wParam, lParam );
				}
				break;

				case WM_LBUTTONDOWN:
				{
					pMouse->_OnMouseLButtonDown( wParam, lParam );
				}
				break;

				case WM_LBUTTONUP:
				{
					pMouse->_OnMouseLButtonUp( wParam, lParam );
				}
				break;

				case WM_RBUTTONDOWN:
				{
					pMouse->_OnMouseRButtonDown( wParam, lParam );
				}
				break;

				case WM_RBUTTONUP:
				{
					pMouse->_OnMouseRButtonUp( wParam, lParam );
				}
				break;

				case WM_MBUTTONDOWN:
				{
					pMouse->_OnMouseMButtonDown( wParam, lParam );
				}
				break;

				case WM_MBUTTONUP:
				{
					pMouse->_OnMouseMButtonUp( wParam, lParam );
				}
				break;

				case WM_LBUTTONDBLCLK:
				{
					pMouse->_OnMouseLButtonDblClick( wParam, lParam );
				}
				break;

				case WM_RBUTTONDBLCLK:
				{
					pMouse->_OnMouseRButtonDblClick( wParam, lParam );
				}
				break;

				case WM_MBUTTONDBLCLK:
				{
					pMouse->_OnMouseMButtonDblClick( wParam, lParam );
				}
				break;

				case WM_SETFOCUS:
				{
					XSE::SWindowEvent Event = { pWnd };
					XSE_RW_EVENT_E( pWnd, OnLostFocus, Event ); 
				}
				break;

				case WM_KILLFOCUS:
				{
					XSE::SWindowEvent Event = { pWnd };
					XSE_RW_EVENT_E( pWnd, OnGetFocus, Event ); 
				}
				break;

				default:
					// Process the left-over messages
					return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
			// If something was not done, let it go
			return DefWindowProc( hWnd, uMsg, wParam, lParam );

		}

		HWND GetConsoleHwnd(void)
		{
			#define MY_BUFSIZE 1024 // Buffer size for console window titles.
			HWND hwndFound;         // This is what is returned to the caller.
			char pszNewWindowTitle[MY_BUFSIZE]; // Contains fabricated
											   // WindowTitle.
			char pszOldWindowTitle[MY_BUFSIZE]; // Contains original
											   // WindowTitle.

			// Fetch current window title.

			GetConsoleTitleA(pszOldWindowTitle, MY_BUFSIZE);

			// Format a "unique" NewWindowTitle.

			wsprintfA(pszNewWindowTitle,"%d/%d",
					   GetTickCount(),
					   GetCurrentProcessId());

			// Change current window title.

			SetConsoleTitleA(pszNewWindowTitle);

			// Ensure window title has been updated.

			Sleep(40);

			// Look for NewWindowTitle.

			hwndFound=FindWindowA(NULL, pszNewWindowTitle);

			// Restore original window title.

			SetConsoleTitleA(pszOldWindowTitle);

			return(hwndFound);
		}

		void SetKeyCodesWin32(u32* auiKeyCodes, u32* auiWinKeyCodes);

		CRenderWindow::CRenderWindow(CEngine* pEngine)
		{
			m_bVisible = false;
			m_hWnd = xst_null;
			m_pEngine = pEngine;
			m_fFrameTime = 0.0f;

			xst_zero( &g_MouseEvent, sizeof( SMouseEvent ) );
			xst_zero( &g_KeyboardEvent, sizeof( SKeyboardEvent ) );
			xst_zero( &m_auiKeyCodes, sizeof( KEY_CODE ) * KeyCodes::_ENUM_COUNT );
			xst_zero( &m_auiWinKeyCodes, sizeof( u32 ) * 256 );

			//Set the key codes
			SetKeyCodesWin32( m_auiKeyCodes, m_auiWinKeyCodes );

			m_pKeyboard = xst_new CKeyboard( this );
			m_pMouse = xst_new CMouse( this );
		}

		CRenderWindow::~CRenderWindow()
		{
			xst_delete( m_pKeyboard );
			xst_delete( m_pMouse );
		}

		i32 CRenderWindow::Create(u32 uiExternalHandle)
		{

			HINSTANCE hInstance = GetModuleHandleA( 0 );

			char title[500];  // to hold title

			// get title of console window
			GetConsoleTitleA( title, 500 );

			// get HWND of console, based on its title
			HWND hwndConsole = FindWindowA( NULL, title );

			// get HINSTANCE of console, based on HWND
			HINSTANCE hInstance2 = (HINSTANCE)GetWindowLong(hwndConsole, GWL_HINSTANCE);


			WNDCLASSEXA wcex;
			ZeroMemory( &wcex, sizeof(WNDCLASSEXA) );
			wcex.cbSize			= sizeof(WNDCLASSEXA);
			wcex.style          = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc    = WndProc;
			wcex.cbClsExtra     = 0;
			wcex.cbWndExtra     = 0;
			wcex.hInstance      = hInstance;
			//wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
			wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
			wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
			wcex.lpszMenuName   = NULL;
			wcex.lpszClassName  = "XSourceEngine";
			//wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

			if (!RegisterClassExA(&wcex))
			{
				return RESULT::FAILED;
			}

			m_hWnd = (HWND)uiExternalHandle;

			if( m_hWnd == (HWND)0 )
			{
				// Create the window object if not external window
				m_hWnd = CreateWindowA("XSourceEngine", "XSourceEngine",
									  WS_OVERLAPPEDWINDOW,
									  CW_USEDEFAULT, CW_USEDEFAULT,
									  //CW_USEDEFAULT, CW_USEDEFAULT,
									  800, 600,
									  NULL, NULL, hInstance, NULL);
			}

			if( !m_hWnd )
			{
				XST_LOG_ERR( "Create render window failed: " << ::GetLastError() );
				return RESULT::FAILED;
			}

			m_uiWndHandle = ( u32 )m_hWnd;

			UpdateWindow( m_hWnd );
			RECT Rect;
			GetWindowRect( m_hWnd, &Rect );
			SetSize( Rect.right - Rect.left, Rect.bottom - Rect.top );

			if( XST_FAILED( m_pKeyboard->Init() ) )
			{
				return XST_FAIL;
			}

			if( XST_FAILED( m_pMouse->Init() ) )
			{
				return XST_FAIL;
			}

			//Register keyboard and mouse
			//RAWINPUTDEVICE RawInputDevices[2]; //keyboard, mouse
			//RawInputDevices[ 0 ].dwFlags
			//RegisterRawInputDevices( RawInputDevices, 2, sizeof( RAWINPUTDEVICE ) * 2 );

			//Start global timer
			m_GlobalTimer.StartQPerf();

			return RESULT::OK;
		}

		void CRenderWindow::Show(bool bShow)
		{
			if( bShow )
			{
				ShowWindow( m_hWnd, SW_SHOWNORMAL );
				//UpdateWindow( m_hWnd );
			}
			else
			{
				ShowWindow( m_hWnd, SW_HIDE );
			}

			m_bVisible = bShow;
		}

		i32 CRenderWindow::Update()
		{
			if( m_WndMsg.message == WM_QUIT )
			{
				this->m_bIsWndClosing = true;
				return XST_FAIL;
			}

			if( ProcessMessages() == RESULT::OK )
			{
				SWindowEvent Event = { this };
				XSE_RW_EVENT( OnUpdate, Event );
				if( _GetKeyboardListenersResult() > 0 )
				{
					return RESULT::FAILED;
				}

				m_pKeyboard->Update( (cf32)m_GlobalTimer.GetQPerfElapsedTime(), GetFrameTime() );

				//::UpdateWindow( m_hWnd );
				return RESULT::OK;
			}

			return RESULT::FAILED;
		}

		void CRenderWindow::StopRendering()
		{
		}

		void CRenderWindow::Close()
		{
			this->m_bIsWndClosing = true;
		}

		bool CRenderWindow::IsVisible()
		{
			return m_bVisible;
		}

		void CRenderWindow::SetPosition(u32 uiPosX, u32 uiPosY)
		{
			SWindowEvent Event = { this };
			XSE_RW_EVENT3( OnMove, Event, uiPosX, uiPosY );
			if( _GetKeyboardListenersResult() > 0 )
			{
				return;
			}

			BOOL bResult = SetWindowPos( m_hWnd, HWND_TOP, uiPosX, uiPosY, m_uiWndWidth, m_uiWndHeight, SWP_NOSIZE | SWP_NOZORDER );
		}

		void CRenderWindow::SetSize(u32 uiWidth, u32 uiHeight)
		{
			//Do not anything
			if( uiWidth == this->m_uiWndWidth && uiHeight == this->m_uiWndHeight )
				return;

			SWindowEvent Event = { this };
			XSE_RW_EVENT3( OnResize, Event, uiWidth, uiHeight );
			if( _GetKeyboardListenersResult() > 0 )
			{
				return;
			}

			this->m_uiWndWidth = uiWidth;
			this->m_uiWndHeight = uiHeight;
			SetWindowPos( m_hWnd, HWND_TOP, 0, 0, uiWidth, uiHeight, SWP_NOMOVE | SWP_NOZORDER );
			if( m_pRenderSystem )
				this->m_pRenderSystem->ResizeBuffers( uiWidth - 1, uiHeight - 1 );
			UpdateWindow( m_hWnd );
		}

		void CRenderWindow::SetTitle(xst_castring& strTitle)
		{
			SetWindowTextA( m_hWnd, strTitle.data() );
		}

		i32 CRenderWindow::ProcessMessages()
		{  
			if( PeekMessageA( &m_WndMsg, NULL, 0, 0, PM_REMOVE ) )
			{
				TranslateMessage( &m_WndMsg );
				DispatchMessage( &m_WndMsg );
				return XST_FAIL;
			}

			return XST_OK;
		}

		i32 CRenderWindow::BeginRenderFrame()
		{
			SWindowEvent Event = { this };
			XSE_RW_EVENT( OnBeginRenderFrame, Event );
			if( _GetKeyboardListenersResult() > 0 )
			{
				return RESULT::FAILED;
			}

			m_Timer.StartQPerf();

			if( XST_FAILED( m_pRenderSystem->BeginRender() ) )
			{
				return XST_FAIL;
			}

			xst_assert( m_pEngine->GetSceneManager(), "" );
			m_pEngine->Update( m_fFrameTime );

			return XST_OK;
		}

		i32 CRenderWindow::EndRenderFrame()
		{
			SWindowEvent Event = { this };
			XSE_RW_EVENT( OnEndRenderFrame, Event );
			if( _GetKeyboardListenersResult() > 0 )
			{
				return RESULT::FAILED;
			}

			//m_Timer.StopQPerf();
			i32 iResult = m_pRenderSystem->EndRender();
			m_fFrameTime = m_Timer.GetQPerfElapsedTime();
			return iResult;
		}

		i32 CRenderWindow::RenderFrame()
		{
			if( Update() != RESULT::OK )
			{
				return RESULT::FAILED;
			}

			if( BeginRenderFrame() != RESULT::OK )
			{
				return RESULT::FAILED;
			}

			if( EndRenderFrame() != RESULT::OK )
			{
				return RESULT::FAILED;
			}

			return RESULT::OK;
		}

		f32 CRenderWindow::GetFrameTime()
		{
			//return ( f32 )m_Timer.GetQPerfElapsedTime();
			return m_fFrameTime;
		}

		//PROTECTED//////////////////////////////////////////////

		void CRenderWindow::_OnWMSize(cu32& uiWidth, cu32& uiHeight)
		{
			SWindowEvent Event = { this };
			XSE_RW_EVENT3( OnResize, Event, uiWidth, uiHeight );
			if( _GetKeyboardListenersResult() > 0 )
			{
				return;
			}

			this->m_uiWndWidth = uiWidth;
			this->m_uiWndHeight = uiHeight;
			if( this->m_pRenderSystem )
			{
				this->m_pRenderSystem->ResizeBuffers( uiWidth, uiHeight );
			}
		}

		void SetKeyCodesWin32(u32* auiKeyCodes, u32* auiWinKeyCodes)
		{
			
			auiKeyCodes[ KeyCodes::Q ] = (u32)'q'; auiWinKeyCodes[ (u32)'q' ] = KeyCodes::Q;
			auiKeyCodes[ KeyCodes::W ] = (u32)'w'; auiWinKeyCodes[ (u32)'w' ] = KeyCodes::W;
			auiKeyCodes[ KeyCodes::E ] = (u32)'e'; auiWinKeyCodes[ (u32)'e' ] = KeyCodes::E;
			auiKeyCodes[ KeyCodes::R ] = (u32)'r'; auiWinKeyCodes[ (u32)'r' ] = KeyCodes::R;
			auiKeyCodes[ KeyCodes::T ] = (u32)'t'; auiWinKeyCodes[ (u32)'t' ] = KeyCodes::T;
			auiKeyCodes[ KeyCodes::Y ] = (u32)'y'; auiWinKeyCodes[ (u32)'y' ] = KeyCodes::Y;
			auiKeyCodes[ KeyCodes::U ] = (u32)'u'; auiWinKeyCodes[ (u32)'u' ] = KeyCodes::U;
			auiKeyCodes[ KeyCodes::I ] = (u32)'i'; auiWinKeyCodes[ (u32)'i' ] = KeyCodes::I;
			auiKeyCodes[ KeyCodes::O ] = (u32)'o'; auiWinKeyCodes[ (u32)'o' ] = KeyCodes::O;
			auiKeyCodes[ KeyCodes::P ] = (u32)'p'; auiWinKeyCodes[ (u32)'p' ] = KeyCodes::P;
			auiKeyCodes[ KeyCodes::A ] = (u32)'a'; auiWinKeyCodes[ (u32)'a' ] = KeyCodes::A;
			auiKeyCodes[ KeyCodes::S ] = (u32)'s'; auiWinKeyCodes[ (u32)'s' ] = KeyCodes::S;
			auiKeyCodes[ KeyCodes::D ] = (u32)'d'; auiWinKeyCodes[ (u32)'d' ] = KeyCodes::D;
			auiKeyCodes[ KeyCodes::F ] = (u32)'f'; auiWinKeyCodes[ (u32)'f' ] = KeyCodes::F;
			auiKeyCodes[ KeyCodes::G ] = (u32)'g'; auiWinKeyCodes[ (u32)'g' ] = KeyCodes::G;
			auiKeyCodes[ KeyCodes::H ] = (u32)'h'; auiWinKeyCodes[ (u32)'h' ] = KeyCodes::H;
			auiKeyCodes[ KeyCodes::J ] = (u32)'j'; auiWinKeyCodes[ (u32)'j' ] = KeyCodes::J;
			auiKeyCodes[ KeyCodes::K ] = (u32)'k'; auiWinKeyCodes[ (u32)'k' ] = KeyCodes::K;
			auiKeyCodes[ KeyCodes::L ] = (u32)'l'; auiWinKeyCodes[ (u32)'l' ] = KeyCodes::K;
			auiKeyCodes[ KeyCodes::Z ] = (u32)'z'; auiWinKeyCodes[ (u32)'z' ] = KeyCodes::Z;
			auiKeyCodes[ KeyCodes::X ] = (u32)'x'; auiWinKeyCodes[ (u32)'x' ] = KeyCodes::X;
			auiKeyCodes[ KeyCodes::C ] = (u32)'c'; auiWinKeyCodes[ (u32)'c' ] = KeyCodes::C;
			auiKeyCodes[ KeyCodes::V ] = (u32)'v'; auiWinKeyCodes[ (u32)'v' ] = KeyCodes::V;
			auiKeyCodes[ KeyCodes::B ] = (u32)'b'; auiWinKeyCodes[ (u32)'b' ] = KeyCodes::B;
			auiKeyCodes[ KeyCodes::N ] = (u32)'n'; auiWinKeyCodes[ (u32)'n' ] = KeyCodes::N;
			auiKeyCodes[ KeyCodes::M ] = (u32)'m'; auiWinKeyCodes[ (u32)'m' ] = KeyCodes::M;
			auiKeyCodes[ KeyCodes::CAPITAL_Q] = (u32)'Q';
			auiKeyCodes[ KeyCodes::CAPITAL_W ] = (u32)'W';
			auiKeyCodes[ KeyCodes::CAPITAL_E ] = (u32)'E';
			auiKeyCodes[ KeyCodes::CAPITAL_R ] = (u32)'R';
			auiKeyCodes[ KeyCodes::CAPITAL_T ] = (u32)'T';
			auiKeyCodes[ KeyCodes::CAPITAL_Y ] = (u32)'Y';
			auiKeyCodes[ KeyCodes::CAPITAL_U ] = (u32)'U';
			auiKeyCodes[ KeyCodes::CAPITAL_I ] = (u32)'I';
			auiKeyCodes[ KeyCodes::CAPITAL_O ] = (u32)'O';
			auiKeyCodes[ KeyCodes::CAPITAL_P ] = (u32)'P';
			auiKeyCodes[ KeyCodes::CAPITAL_A ] = (u32)'A';
			auiKeyCodes[ KeyCodes::CAPITAL_S ] = (u32)'S';
			auiKeyCodes[ KeyCodes::CAPITAL_D ] = (u32)'D';
			auiKeyCodes[ KeyCodes::CAPITAL_F ] = (u32)'F';
			auiKeyCodes[ KeyCodes::CAPITAL_G ] = (u32)'G';
			auiKeyCodes[ KeyCodes::CAPITAL_H ] = (u32)'H';
			auiKeyCodes[ KeyCodes::CAPITAL_J ] = (u32)'J';
			auiKeyCodes[ KeyCodes::CAPITAL_K ] = (u32)'K';
			auiKeyCodes[ KeyCodes::CAPITAL_L ] = (u32)'L';
			auiKeyCodes[ KeyCodes::CAPITAL_Z ] = (u32)'Z';
			auiKeyCodes[ KeyCodes::CAPITAL_X ] = (u32)'X';
			auiKeyCodes[ KeyCodes::CAPITAL_C ] = (u32)'C';
			auiKeyCodes[ KeyCodes::CAPITAL_V ] = (u32)'V';
			auiKeyCodes[ KeyCodes::CAPITAL_B ] = (u32)'B';
			auiKeyCodes[ KeyCodes::CAPITAL_N ] = (u32)'N';
			auiKeyCodes[ KeyCodes::CAPITAL_M ] = (u32)'M';
			auiKeyCodes[ KeyCodes::KEYBOARD_1 ] = 0x31;
			auiKeyCodes[ KeyCodes::KEYBOARD_2 ] = 0x32;
			auiKeyCodes[ KeyCodes::KEYBOARD_3 ] = 0x33;
			auiKeyCodes[ KeyCodes::KEYBOARD_4 ] = 0x34;
			auiKeyCodes[ KeyCodes::KEYBOARD_5 ] = 0x35;
			auiKeyCodes[ KeyCodes::KEYBOARD_6 ] = 0x36;
			auiKeyCodes[ KeyCodes::KEYBOARD_7 ] = 0x37;
			auiKeyCodes[ KeyCodes::KEYBOARD_8 ] = 0x38;
			auiKeyCodes[ KeyCodes::KEYBOARD_9 ] = 0x39;
			auiKeyCodes[ KeyCodes::KEYBOARD_0 ] = 0x30;
			auiKeyCodes[ KeyCodes::NUM_1 ] = 0x61;
			auiKeyCodes[ KeyCodes::NUM_2 ] = 0x62;
			auiKeyCodes[ KeyCodes::NUM_3 ] = 0x63;
			auiKeyCodes[ KeyCodes::NUM_4 ] = 0x64;
			auiKeyCodes[ KeyCodes::NUM_5 ] = 0x65;
			auiKeyCodes[ KeyCodes::NUM_6 ] = 0x66;
			auiKeyCodes[ KeyCodes::NUM_7 ] = 0x67;
			auiKeyCodes[ KeyCodes::NUM_8 ] = 0x68;
			auiKeyCodes[ KeyCodes::NUM_9 ] = 0x69;
			auiKeyCodes[ KeyCodes::NUM_0 ] = 0x60;
			auiKeyCodes[ KeyCodes::BACK ] = VK_BACK;
			auiKeyCodes[ KeyCodes::TAB ] = VK_TAB;
			auiKeyCodes[ KeyCodes::CLEAR ] = VK_CLEAR;
			auiKeyCodes[ KeyCodes::RETURN ] = VK_RETURN;
			auiKeyCodes[ KeyCodes::SHIFT ] = VK_SHIFT;
			auiKeyCodes[ KeyCodes::CONTROL ] = VK_CONTROL;
			auiKeyCodes[ KeyCodes::ALT ] = VK_MENU;
			auiKeyCodes[ KeyCodes::PAUSE ] = VK_PAUSE;
			auiKeyCodes[ KeyCodes::CAPS_LOCK ] = VK_CAPITAL;
			auiKeyCodes[ KeyCodes::SPACE ] = VK_SPACE;
			auiKeyCodes[ KeyCodes::PAGE_UP ] = VK_PRIOR;
			auiKeyCodes[ KeyCodes::PAGE_DOWN ] = VK_NEXT;
			auiKeyCodes[ KeyCodes::END ] = VK_END;
			auiKeyCodes[ KeyCodes::HOME ] = VK_HOME;
			auiKeyCodes[ KeyCodes::LEFT ] = VK_LEFT;
			auiKeyCodes[ KeyCodes::RIGHT ] = VK_RIGHT;
			auiKeyCodes[ KeyCodes::UP ] = VK_UP;
			auiKeyCodes[ KeyCodes::DOWN ] = VK_DOWN;
			auiKeyCodes[ KeyCodes::SELECT ] = VK_SELECT;
			auiKeyCodes[ KeyCodes::PRINT ] = VK_PRINT;
			auiKeyCodes[ KeyCodes::EXECUTE ] = VK_EXECUTE;
			auiKeyCodes[ KeyCodes::PRINT_SCREEN ] = VK_SNAPSHOT;
			auiKeyCodes[ KeyCodes::INSERT ] = VK_INSERT;
			auiKeyCodes[ KeyCodes::DEL ] = VK_DELETE;
			auiKeyCodes[ KeyCodes::HELP ] = VK_HELP;
			auiKeyCodes[ KeyCodes::SLEEP ] = VK_SLEEP;
			auiKeyCodes[ KeyCodes::MULTIPLY ] = VK_MULTIPLY;
			auiKeyCodes[ KeyCodes::ADD ] = VK_ADD;
			auiKeyCodes[ KeyCodes::SEPARATOR ] = VK_SEPARATOR;
			auiKeyCodes[ KeyCodes::SUBTRACT ] = VK_SUBTRACT;
			auiKeyCodes[ KeyCodes::DECIMAL ] = VK_DECIMAL;
			auiKeyCodes[ KeyCodes::DIVIDE ] = VK_DIVIDE;
			auiKeyCodes[ KeyCodes::F1 ] = VK_F1;
			auiKeyCodes[ KeyCodes::F2 ] = VK_F2;
			auiKeyCodes[ KeyCodes::F3 ] = VK_F3;
			auiKeyCodes[ KeyCodes::F4 ] = VK_F4;
			auiKeyCodes[ KeyCodes::F5 ] = VK_F5;
			auiKeyCodes[ KeyCodes::F6 ] = VK_F6;
			auiKeyCodes[ KeyCodes::F7 ] = VK_F7;
			auiKeyCodes[ KeyCodes::F8 ] = VK_F8;
			auiKeyCodes[ KeyCodes::F9 ] = VK_F9;
			auiKeyCodes[ KeyCodes::F10 ] = VK_F10;
			auiKeyCodes[ KeyCodes::F11 ] = VK_F11;
			auiKeyCodes[ KeyCodes::F12 ] = VK_F12;
			auiKeyCodes[ KeyCodes::NUMLOCK ] = VK_NUMLOCK;
			auiKeyCodes[ KeyCodes::SCROLL_LOCK ] = VK_SCROLL;
			auiKeyCodes[ KeyCodes::LEFT_SHIFT ] = VK_LSHIFT;
			auiKeyCodes[ KeyCodes::RIGHT_SHIFT ] = VK_RSHIFT;
			auiKeyCodes[ KeyCodes::LEFT_ALT ] = VK_LMENU;
			auiKeyCodes[ KeyCodes::RIGHT_ALT ] = VK_RMENU;
			auiKeyCodes[ KeyCodes::LEFT_CONTROL ] = VK_LCONTROL;
			auiKeyCodes[ KeyCodes::RIGHT_CONTROL ] = VK_RCONTROL;
			//auiKeyCodes[ KeyCodes:: ] = VK_;
			//auiKeyCodes[ KeyCodes:: ] = 0x;
		}
	}//win32
}//xse
#endif //XST_WINDOWS