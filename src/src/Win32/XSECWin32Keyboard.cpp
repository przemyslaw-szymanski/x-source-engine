#include "XSECWin32Keyboard.h"
#include "XSECWin32RenderWindow.h"

namespace XSE
{
#if defined (XST_WINDOWS)
	namespace Win32
	{
		void SetKeyCodesWin32(u32* auiKeyCodes, u8* auiWinKeyCodes);

		#define XSE_WIN32_KE(_methodName) \
			xst_stl_foreach( m_ListenerItr, m_vListeners ) \
				(*m_ListenerItr)->_methodName( m_KeyEvent )

		#define XSE_WIN32_ADD_MODIFIER(_uiCode) m_KeyEvent.uiModifier |= ( _uiCode )
		#define XSE_WIN32_REM_MODIFIER(_uiCode) m_KeyEvent.uiModifier &= ~( _uiCode )

		CKeyboard::CKeyboard(CRenderWindow* pWnd)
		{
			//m_ausLocalizations[ KeyboardLanguages::SYSTEM_DEFAULT ] = LANG_SYSTEM_DEFAULT;
			//m_ausLocalizations[ KeyboardLanguages::USER_DEFAULT ] = LANG_USER_DEFAULT;

			xst_zero( &m_KeyEvent, sizeof( SKeyboardEvent ) );
			xst_zero( &m_auiKeyCodes, sizeof( KEY_CODE ) * KeyCodes::_ENUM_COUNT );
			xst_zero( &m_abyWinKeyCodes, sizeof( u8 ) * 256 );
			xst_zero( &m_abyKeyboardState, sizeof( u8 ) * 256 );

			//Add some default languages
			AddLanguage( "en_US", "00000409" );
			AddLanguage( "en_GB", "00000809" );
			AddLanguage( "pl_PL", "00000415" );
			AddLanguage( "fr_FR", "0000040C" );
			AddLanguage( "de_DE", "00000407" );

			m_pWnd = pWnd;
			m_KeyEvent.pWnd = (IRenderWindow*)pWnd;

			SetKeyCodesWin32( m_auiKeyCodes, m_abyWinKeyCodes );
		}

		CKeyboard::~CKeyboard()
		{
			RemoveListeners( true );
		}

		void CKeyboard::RemoveListeners(bool bDestroy)
		{
			xst_stl_foreach( m_ListenerItr, m_vListeners )
			{
				RemoveListener( (*m_ListenerItr), bDestroy );
			}

			m_vListeners.clear();
		}

		i32 CKeyboard::Init()
		{
			//Get default keyboard localization
			m_WinHKL = GetKeyboardLayout( 0 );

			return XST_OK;
		}

		i32 CKeyboard::SetLocalization(xst_castring& strLang)
		{
			//Find this locale
			LangMap::iterator Itr = m_mLangs.find( LangStr( strLang ) );
			if( Itr == m_mLangs.end() )
			{
				XST_LOG_ERR( "Could not find language: " << strLang << ". If you want to use this language add it first." );
				return XST_FAIL;
			}

			/*HKL aBuff[10];
			GetKeyboardLayoutList( 10, aBuff );
			for(u32 i = 0; i < 10; ++i)
			{
				XST::CDebug::PrintDebugLN( XST::StringUtil::ToString( aBuff[ i ] ) );
			}*/

			m_WinHKL = LoadKeyboardLayoutA( Itr->second.data(), KLF_ACTIVATE );
			if( m_WinHKL == xst_null )
			{
				XST_LOG_ERR( "Failed to set keybaord localization: " << GetLastError() );
				return XST_FAIL;
			}
			if( ActivateKeyboardLayout( m_WinHKL, KLF_RESET ) == 0 )
			{
				XST_LOG_ERR( "Failed to activate the keyboard localization: " << GetLastError() );
				return XST_FAIL;
			}

			m_strCurrLang = strLang;
			return XST_OK;
		}

		i32	CKeyboard::AddLanguage(xst_castring& strLang, xst_castring& strCode)
		{
			return XST::MapUtils::Insert( m_mLangs, LangStr( strLang ), XST::xst_astr8( strCode ) );
		}

		void CKeyboard::Update(cf32& fElapsedTime, cf32& fTimeSinceLastUpdate)
		{
			GetKeyboardState( m_abyKeyboardState );
			//SetWindowTextW( (HWND)this->m_pWnd->GetHandle(), m_KeyEvent.aChars );
		}

		bool CKeyboard::IsKeyPressed(KEY_CODE eCode) const
		{
			/*KEY_STATE e = m_KeyEvent.eKeyState;
			u32 u = m_auiKeyCodes[ eCode ];
			u16 i = GetAsyncKeyState( m_auiKeyCodes[ eCode ] );*/
			return GetAsyncKeyState( m_auiKeyCodes[ eCode ] ) & 0x8000;
		}

		void CKeyboard::_OnKeyDown(WPARAM wParam, LPARAM lParam)
		{
			m_KeyEvent.eKeyState = KeyState::DOWN;
			//Map key from windows to the engine
			m_KeyEvent.eKeyCode = (KEY_CODE)m_abyWinKeyCodes[ wParam ];

			//Check for modifiers

			if( m_abyKeyboardState[ VK_SHIFT ] )
			{
				XSE_WIN32_ADD_MODIFIER( KeyModifiers::SHIFT );
			}

			if( m_abyKeyboardState[ VK_LSHIFT ] )
			{
				XSE_WIN32_ADD_MODIFIER( KeyModifiers::LEFT_SHIFT );
			}

			if( m_abyKeyboardState[ VK_RSHIFT ] )
			{
				XSE_WIN32_ADD_MODIFIER( KeyModifiers::RIGHT_SHIFT );
			}

			if( m_abyKeyboardState[ VK_LCONTROL ] )
			{
				XSE_WIN32_ADD_MODIFIER( KeyModifiers::LEFT_CONTROL );
			}

			if( m_abyKeyboardState[ VK_RCONTROL ] )
			{
				XSE_WIN32_ADD_MODIFIER( KeyModifiers::RIGHT_CONTROL );
			}

			if( m_abyKeyboardState[ VK_CONTROL ] )
			{
				XSE_WIN32_ADD_MODIFIER( KeyModifiers::CONTROL );
			}

			//If not modifier
			if( m_KeyEvent.eKeyCode != KeyCodes::SHIFT && m_KeyEvent.eKeyCode != KeyCodes::LEFT_SHIFT &&
				m_KeyEvent.eKeyCode != KeyCodes::RIGHT_SHIFT && m_KeyEvent.eKeyCode != KeyCodes::CONTROL &&
				m_KeyEvent.eKeyCode != KeyCodes::LEFT_CONTROL && m_KeyEvent.eKeyCode != KeyCodes::RIGHT_CONTROL )
			{
				m_KeyEvent.aChars[ 0 ] = this->_TranslateChar( wParam, lParam, ( m_KeyEvent.uiModifier & KeyModifiers::ALT ) || ( m_KeyEvent.uiModifier & KeyModifiers::SHIFT ) );
			}

			XSE_WIN32_KE( OnKeyDown );
			
		}
		
		void CKeyboard::_OnKeyUp(WPARAM wParam, LPARAM lParam)
		{
			m_KeyEvent.eKeyState = KeyState::UP;
			//Check for modifiers
			if( m_abyKeyboardState[ VK_SHIFT ] )
			{
				XSE_WIN32_REM_MODIFIER( KeyModifiers::SHIFT );
			}

			if( m_abyKeyboardState[ VK_LSHIFT ] )
			{
				XSE_WIN32_REM_MODIFIER( KeyModifiers::LEFT_SHIFT );
			}

			if( m_abyKeyboardState[ VK_RSHIFT ] )
			{
				XSE_WIN32_REM_MODIFIER( KeyModifiers::RIGHT_SHIFT );
			}

			if( m_abyKeyboardState[ VK_LCONTROL ] )
			{
				XSE_WIN32_REM_MODIFIER( KeyModifiers::LEFT_CONTROL );
			}

			if( m_abyKeyboardState[ VK_RCONTROL ] )
			{
				XSE_WIN32_REM_MODIFIER( KeyModifiers::RIGHT_CONTROL );
			}

			if( m_abyKeyboardState[ VK_CONTROL ] )
			{
				XSE_WIN32_REM_MODIFIER( KeyModifiers::CONTROL );
			}


			//Map key from windows to the engine
			m_KeyEvent.eKeyCode = (KEY_CODE)m_abyWinKeyCodes[ wParam ];
			//Check for modifiers

			XSE_WIN32_KE( OnKeyUp );
		}
		
		void CKeyboard::_OnSysKeyDown(WPARAM wParam, LPARAM lParam)
		{
			m_KeyEvent.eKeyState = KeyState::DOWN;
			//Check for alt
			if( lParam & 0x20000000 )
			{
				XSE_WIN32_ADD_MODIFIER( KeyModifiers::ALT );
				XSE_WIN32_ADD_MODIFIER( KeyModifiers::LEFT_ALT );

				if( wParam != VK_MENU )
				{
					m_KeyEvent.aChars[ 0 ] = _TranslateChar( wParam, lParam, 0x00000001 );
					//OutputDebugStringW( m_KeyEvent.aChars );
				}
			}

			//Map key from windows to the engine
			m_KeyEvent.eKeyCode = (KEY_CODE)m_abyWinKeyCodes[ wParam ];
		
			XSE_WIN32_KE( OnKeyDown );
		}
		
		void CKeyboard::_OnSysKeyUp(WPARAM wParam, LPARAM lParam)
		{
			m_KeyEvent.eKeyState = KeyState::UP;
			//Check for alt
			if( lParam & 0x20000000 )
			{
				XSE_WIN32_REM_MODIFIER( KeyModifiers::ALT );
				XSE_WIN32_REM_MODIFIER( KeyModifiers::LEFT_ALT );
			}

			//Map key from windows to the engine
			m_KeyEvent.eKeyCode = (KEY_CODE)m_abyWinKeyCodes[ wParam ];
			XSE_WIN32_KE( OnKeyUp );
		}
		
		void CKeyboard::_OnChar(WPARAM wParam, LPARAM lParam)
		{
			m_KeyEvent.aChars[ 0 ] = (ch16)wParam;
			m_KeyEvent.aChars[ 1 ] = 0;
			//OutputDebugStringW( m_KeyEvent.aChars );
			XSE_WIN32_KE( OnKeyUp );
		}

		void CKeyboard::_OnUniChar(WPARAM wParam, LPARAM lParam)
		{
			XSE_WIN32_KE( OnKeyUp );
		}
		
		void CKeyboard::_OnSysChar(WPARAM wParam, LPARAM lParam)
		{
			m_KeyEvent.aChars[ 0 ] = (ch16)wParam;
			m_KeyEvent.aChars[ 1 ] = 0;
			//OutputDebugStringW( m_KeyEvent.aChars );
			XSE_WIN32_KE( OnKeyUp );
		}

		void CKeyboard::_OnInput(WPARAM wParam, LPARAM lParam)
		{
			if( GetKeyboardState( m_abyKeyboardState ) )
			{
				//i32 iResult = ToUnicodeEx( wParam, 
			}
		}

		u16 CKeyboard::_TranslateChar(WPARAM wParam, LPARAM lParam, u32 uiFlags)
		{
			u32 uiScanCode = MapVirtualKeyA( wParam, 2 );
			ch16 wchCode[] = { 0, 0 };
			i32 iResult = ToUnicodeEx( wParam, uiScanCode, m_abyKeyboardState, wchCode, sizeof( ch16 ), uiFlags, m_WinHKL );
			//iResult = ToUnicodeEx( wParam, uiScanCode, m_abyKeyboardState, wchCode, sizeof( ch16 ), uiFlags, m_WinHKL );
			if( iResult <= 0 )
			{
				//Set to default if there is no translation
				wchCode[ 0 ] = (ch16)wParam;
			}
			wchCode[ 1 ] = 0;
			return wchCode[ 0 ];
		}

		void SetKeyCodesWin32(u32* auiKeyCodes, u8* auiWinKeyCodes)
		{
			
			auiKeyCodes[ KeyCodes::Q ] = (u8)'q'; auiWinKeyCodes[ (u8)'q' ] = KeyCodes::Q;
			auiKeyCodes[ KeyCodes::W ] = (u8)'w'; auiWinKeyCodes[ (u8)'w' ] = KeyCodes::W;
			auiKeyCodes[ KeyCodes::E ] = (u8)'e'; auiWinKeyCodes[ (u8)'e' ] = KeyCodes::E;
			auiKeyCodes[ KeyCodes::R ] = (u8)'r'; auiWinKeyCodes[ (u8)'r' ] = KeyCodes::R;
			auiKeyCodes[ KeyCodes::T ] = (u8)'t'; auiWinKeyCodes[ (u8)'t' ] = KeyCodes::T;
			auiKeyCodes[ KeyCodes::Y ] = (u8)'y'; auiWinKeyCodes[ (u8)'y' ] = KeyCodes::Y;
			auiKeyCodes[ KeyCodes::U ] = (u8)'u'; auiWinKeyCodes[ (u8)'u' ] = KeyCodes::U;
			auiKeyCodes[ KeyCodes::I ] = (u8)'i'; auiWinKeyCodes[ (u8)'i' ] = KeyCodes::I;
			auiKeyCodes[ KeyCodes::O ] = (u8)'o'; auiWinKeyCodes[ (u8)'o' ] = KeyCodes::O;
			auiKeyCodes[ KeyCodes::P ] = (u8)'p'; auiWinKeyCodes[ (u8)'p' ] = KeyCodes::P;
			auiKeyCodes[ KeyCodes::A ] = (u8)'a'; auiWinKeyCodes[ (u8)'a' ] = KeyCodes::A;
			auiKeyCodes[ KeyCodes::S ] = (u8)'s'; auiWinKeyCodes[ (u8)'s' ] = KeyCodes::S;
			auiKeyCodes[ KeyCodes::D ] = (u8)'d'; auiWinKeyCodes[ (u8)'d' ] = KeyCodes::D;
			auiKeyCodes[ KeyCodes::F ] = (u8)'f'; auiWinKeyCodes[ (u8)'f' ] = KeyCodes::F;
			auiKeyCodes[ KeyCodes::G ] = (u8)'g'; auiWinKeyCodes[ (u8)'g' ] = KeyCodes::G;
			auiKeyCodes[ KeyCodes::H ] = (u8)'h'; auiWinKeyCodes[ (u8)'h' ] = KeyCodes::H;
			auiKeyCodes[ KeyCodes::J ] = (u8)'j'; auiWinKeyCodes[ (u8)'j' ] = KeyCodes::J;
			auiKeyCodes[ KeyCodes::K ] = (u8)'k'; auiWinKeyCodes[ (u8)'k' ] = KeyCodes::K;
			auiKeyCodes[ KeyCodes::L ] = (u8)'l'; auiWinKeyCodes[ (u8)'l' ] = KeyCodes::K;
			auiKeyCodes[ KeyCodes::Z ] = (u8)'z'; auiWinKeyCodes[ (u8)'z' ] = KeyCodes::Z;
			auiKeyCodes[ KeyCodes::X ] = (u8)'x'; auiWinKeyCodes[ (u8)'x' ] = KeyCodes::X;
			auiKeyCodes[ KeyCodes::C ] = (u8)'c'; auiWinKeyCodes[ (u8)'c' ] = KeyCodes::C;
			auiKeyCodes[ KeyCodes::V ] = (u8)'v'; auiWinKeyCodes[ (u8)'v' ] = KeyCodes::V;
			auiKeyCodes[ KeyCodes::B ] = (u8)'b'; auiWinKeyCodes[ (u8)'b' ] = KeyCodes::B;
			auiKeyCodes[ KeyCodes::N ] = (u8)'n'; auiWinKeyCodes[ (u8)'n' ] = KeyCodes::N;
			auiKeyCodes[ KeyCodes::M ] = (u8)'m'; auiWinKeyCodes[ (u8)'m' ] = KeyCodes::M;
			auiKeyCodes[ KeyCodes::CAPITAL_Q ] = (u8)'Q'; auiWinKeyCodes[ (u8)'Q' ] = KeyCodes::CAPITAL_Q;
			auiKeyCodes[ KeyCodes::CAPITAL_W ] = (u8)'W'; auiWinKeyCodes[ (u8)'W' ] = KeyCodes::CAPITAL_W;
			auiKeyCodes[ KeyCodes::CAPITAL_E ] = (u8)'E'; auiWinKeyCodes[ (u8)'E' ] = KeyCodes::CAPITAL_E;
			auiKeyCodes[ KeyCodes::CAPITAL_R ] = (u8)'R'; auiWinKeyCodes[ (u8)'R' ] = KeyCodes::CAPITAL_R;
			auiKeyCodes[ KeyCodes::CAPITAL_T ] = (u8)'T'; auiWinKeyCodes[ (u8)'T' ] = KeyCodes::CAPITAL_T;
			auiKeyCodes[ KeyCodes::CAPITAL_Y ] = (u8)'Y'; auiWinKeyCodes[ (u8)'Y' ] = KeyCodes::CAPITAL_Y;
			auiKeyCodes[ KeyCodes::CAPITAL_U ] = (u8)'U'; auiWinKeyCodes[ (u8)'U' ] = KeyCodes::CAPITAL_U;
			auiKeyCodes[ KeyCodes::CAPITAL_I ] = (u8)'I'; auiWinKeyCodes[ (u8)'I' ] = KeyCodes::CAPITAL_I;
			auiKeyCodes[ KeyCodes::CAPITAL_O ] = (u8)'O'; auiWinKeyCodes[ (u8)'O' ] = KeyCodes::CAPITAL_O;
			auiKeyCodes[ KeyCodes::CAPITAL_P ] = (u8)'P'; auiWinKeyCodes[ (u8)'P' ] = KeyCodes::CAPITAL_P;
			auiKeyCodes[ KeyCodes::CAPITAL_A ] = (u8)'A'; auiWinKeyCodes[ (u8)'A' ] = KeyCodes::CAPITAL_A;
			auiKeyCodes[ KeyCodes::CAPITAL_S ] = (u8)'S'; auiWinKeyCodes[ (u8)'S' ] = KeyCodes::CAPITAL_S;
			auiKeyCodes[ KeyCodes::CAPITAL_D ] = (u8)'D'; auiWinKeyCodes[ (u8)'D' ] = KeyCodes::CAPITAL_D;
			auiKeyCodes[ KeyCodes::CAPITAL_F ] = (u8)'F'; auiWinKeyCodes[ (u8)'F' ] = KeyCodes::CAPITAL_F;
			auiKeyCodes[ KeyCodes::CAPITAL_G ] = (u8)'G'; auiWinKeyCodes[ (u8)'G' ] = KeyCodes::CAPITAL_G;
			auiKeyCodes[ KeyCodes::CAPITAL_H ] = (u8)'H'; auiWinKeyCodes[ (u8)'H' ] = KeyCodes::CAPITAL_H;
			auiKeyCodes[ KeyCodes::CAPITAL_J ] = (u8)'J'; auiWinKeyCodes[ (u8)'J' ] = KeyCodes::CAPITAL_J;
			auiKeyCodes[ KeyCodes::CAPITAL_K ] = (u8)'K'; auiWinKeyCodes[ (u8)'K' ] = KeyCodes::CAPITAL_K;
			auiKeyCodes[ KeyCodes::CAPITAL_L ] = (u8)'L'; auiWinKeyCodes[ (u8)'L' ] = KeyCodes::CAPITAL_L;
			auiKeyCodes[ KeyCodes::CAPITAL_Z ] = (u8)'Z'; auiWinKeyCodes[ (u8)'Z' ] = KeyCodes::CAPITAL_Z;
			auiKeyCodes[ KeyCodes::CAPITAL_X ] = (u8)'X'; auiWinKeyCodes[ (u8)'X' ] = KeyCodes::CAPITAL_X;
			auiKeyCodes[ KeyCodes::CAPITAL_C ] = (u8)'C'; auiWinKeyCodes[ (u8)'C' ] = KeyCodes::CAPITAL_C;
			auiKeyCodes[ KeyCodes::CAPITAL_V ] = (u8)'V'; auiWinKeyCodes[ (u8)'V' ] = KeyCodes::CAPITAL_V;
			auiKeyCodes[ KeyCodes::CAPITAL_B ] = (u8)'B'; auiWinKeyCodes[ (u8)'B' ] = KeyCodes::CAPITAL_B;
			auiKeyCodes[ KeyCodes::CAPITAL_N ] = (u8)'N'; auiWinKeyCodes[ (u8)'N' ] = KeyCodes::CAPITAL_N;
			auiKeyCodes[ KeyCodes::CAPITAL_M ] = (u8)'M'; auiWinKeyCodes[ (u8)'M' ] = KeyCodes::CAPITAL_M;
			auiKeyCodes[ KeyCodes::KEYBOARD_1 ] = 0x31; auiWinKeyCodes[ 0x31 ] = KeyCodes::KEYBOARD_1; 
			auiKeyCodes[ KeyCodes::KEYBOARD_2 ] = 0x32; auiWinKeyCodes[ 0x32 ] = KeyCodes::KEYBOARD_2;
			auiKeyCodes[ KeyCodes::KEYBOARD_3 ] = 0x33; auiWinKeyCodes[ 0x33 ] = KeyCodes::KEYBOARD_3;
			auiKeyCodes[ KeyCodes::KEYBOARD_4 ] = 0x34; auiWinKeyCodes[ 0x34 ] = KeyCodes::KEYBOARD_4;
			auiKeyCodes[ KeyCodes::KEYBOARD_5 ] = 0x35; auiWinKeyCodes[ 0x35 ] = KeyCodes::KEYBOARD_5;
			auiKeyCodes[ KeyCodes::KEYBOARD_6 ] = 0x36; auiWinKeyCodes[ 0x36 ] = KeyCodes::KEYBOARD_6;
			auiKeyCodes[ KeyCodes::KEYBOARD_7 ] = 0x37; auiWinKeyCodes[ 0x37 ] = KeyCodes::KEYBOARD_7;
			auiKeyCodes[ KeyCodes::KEYBOARD_8 ] = 0x38; auiWinKeyCodes[ 0x38 ] = KeyCodes::KEYBOARD_8;
			auiKeyCodes[ KeyCodes::KEYBOARD_9 ] = 0x39; auiWinKeyCodes[ 0x39 ] = KeyCodes::KEYBOARD_9;
			auiKeyCodes[ KeyCodes::KEYBOARD_0 ] = 0x30; auiWinKeyCodes[ 0x30 ] = KeyCodes::KEYBOARD_0;
			auiKeyCodes[ KeyCodes::NUM_1 ] = 0x61; auiWinKeyCodes[ 0x61 ] = KeyCodes::NUM_1;
			auiKeyCodes[ KeyCodes::NUM_2 ] = 0x62; auiWinKeyCodes[ 0x62 ] = KeyCodes::NUM_2;
			auiKeyCodes[ KeyCodes::NUM_3 ] = 0x63; auiWinKeyCodes[ 0x63 ] = KeyCodes::NUM_3;
			auiKeyCodes[ KeyCodes::NUM_4 ] = 0x64; auiWinKeyCodes[ 0x64 ] = KeyCodes::NUM_4;
			auiKeyCodes[ KeyCodes::NUM_5 ] = 0x65; auiWinKeyCodes[ 0x65 ] = KeyCodes::NUM_5;
			auiKeyCodes[ KeyCodes::NUM_6 ] = 0x66; auiWinKeyCodes[ 0x66 ] = KeyCodes::NUM_6;
			auiKeyCodes[ KeyCodes::NUM_7 ] = 0x67; auiWinKeyCodes[ 0x67 ] = KeyCodes::NUM_7;
			auiKeyCodes[ KeyCodes::NUM_8 ] = 0x68; auiWinKeyCodes[ 0x68 ] = KeyCodes::NUM_8;
			auiKeyCodes[ KeyCodes::NUM_9 ] = 0x69; auiWinKeyCodes[ 0x69 ] = KeyCodes::NUM_9;
			auiKeyCodes[ KeyCodes::NUM_0 ] = 0x60; auiWinKeyCodes[ 0x60 ] = KeyCodes::NUM_0;
			auiKeyCodes[ KeyCodes::BACK ]			= VK_BACK;		auiWinKeyCodes[ VK_BACK ] = KeyCodes::BACK;
			auiKeyCodes[ KeyCodes::TAB ]			= VK_TAB;		auiWinKeyCodes[ VK_TAB ] = KeyCodes::TAB;
			auiKeyCodes[ KeyCodes::CLEAR ]			= VK_CLEAR;		auiWinKeyCodes[ VK_CLEAR ] = KeyCodes::CLEAR;
			auiKeyCodes[ KeyCodes::RETURN ]			= VK_RETURN;	auiWinKeyCodes[ VK_RETURN ] = KeyCodes::RETURN;
			auiKeyCodes[ KeyCodes::SHIFT ]			= VK_SHIFT;		auiWinKeyCodes[ VK_SHIFT ] = KeyCodes::SHIFT;
			auiKeyCodes[ KeyCodes::CONTROL ]		= VK_CONTROL;	auiWinKeyCodes[ VK_CONTROL ] = KeyCodes::CONTROL;
			auiKeyCodes[ KeyCodes::ALT ]			= VK_MENU;		auiWinKeyCodes[ VK_MENU ] = KeyCodes::ALT;
			auiKeyCodes[ KeyCodes::PAUSE ]			= VK_PAUSE;		auiWinKeyCodes[ VK_PAUSE ] = KeyCodes::PAUSE;
			auiKeyCodes[ KeyCodes::CAPS_LOCK ]		= VK_CAPITAL;	auiWinKeyCodes[ VK_CAPITAL ] = KeyCodes::CAPS_LOCK;
			auiKeyCodes[ KeyCodes::SPACE ]			= VK_SPACE;		auiWinKeyCodes[ VK_SPACE ] = KeyCodes::SPACE;
			auiKeyCodes[ KeyCodes::PAGE_UP ]		= VK_PRIOR;		auiWinKeyCodes[ VK_PRIOR ] = KeyCodes::PAGE_UP;
			auiKeyCodes[ KeyCodes::PAGE_DOWN ]		= VK_NEXT;		auiWinKeyCodes[ VK_NEXT ] = KeyCodes::PAGE_DOWN;
			auiKeyCodes[ KeyCodes::END ]			= VK_END;		auiWinKeyCodes[ VK_END ] = KeyCodes::END;
			auiKeyCodes[ KeyCodes::HOME ]			= VK_HOME;		auiWinKeyCodes[ VK_HOME ] = KeyCodes::HOME;
			auiKeyCodes[ KeyCodes::LEFT ]			= VK_LEFT;		auiWinKeyCodes[ VK_LEFT ] = KeyCodes::LEFT;
			auiKeyCodes[ KeyCodes::RIGHT ]			= VK_RIGHT;		auiWinKeyCodes[ VK_RIGHT ] = KeyCodes::RIGHT;
			auiKeyCodes[ KeyCodes::UP ]				= VK_UP;		auiWinKeyCodes[ VK_UP ] = KeyCodes::UP;
			auiKeyCodes[ KeyCodes::DOWN ]			= VK_DOWN;		auiWinKeyCodes[ VK_DOWN ] = KeyCodes::DOWN;
			auiKeyCodes[ KeyCodes::SELECT ]			= VK_SELECT;	auiWinKeyCodes[ VK_SELECT ] = KeyCodes::SELECT;
			auiKeyCodes[ KeyCodes::PRINT ]			= VK_PRINT;		auiWinKeyCodes[ VK_PRINT ] = KeyCodes::PRINT;
			auiKeyCodes[ KeyCodes::EXECUTE ]		= VK_EXECUTE;	auiWinKeyCodes[ VK_EXECUTE ] = KeyCodes::EXECUTE;
			auiKeyCodes[ KeyCodes::PRINT_SCREEN ]	= VK_SNAPSHOT;	auiWinKeyCodes[ VK_SNAPSHOT ] = KeyCodes::PRINT_SCREEN;
			auiKeyCodes[ KeyCodes::INSERT ]			= VK_INSERT;	auiWinKeyCodes[ VK_INSERT ] = KeyCodes::INSERT;
			auiKeyCodes[ KeyCodes::DEL ]			= VK_DELETE;	auiWinKeyCodes[ VK_DELETE ] = KeyCodes::DEL;
			auiKeyCodes[ KeyCodes::HELP ]			= VK_HELP;		auiWinKeyCodes[ VK_HELP ] = KeyCodes::HELP;
			auiKeyCodes[ KeyCodes::SLEEP ]			= VK_SLEEP;		auiWinKeyCodes[ VK_SLEEP ] = KeyCodes::SLEEP;
			auiKeyCodes[ KeyCodes::MULTIPLY ]		= VK_MULTIPLY;	auiWinKeyCodes[ VK_MULTIPLY ] = KeyCodes::MULTIPLY;
			auiKeyCodes[ KeyCodes::ADD ]			= VK_ADD;		auiWinKeyCodes[ VK_ADD ] = KeyCodes::ADD;
			auiKeyCodes[ KeyCodes::SEPARATOR ]		= VK_SEPARATOR;	auiWinKeyCodes[ VK_SEPARATOR ] = KeyCodes::SEPARATOR;
			auiKeyCodes[ KeyCodes::SUBTRACT ]		= VK_SUBTRACT;	auiWinKeyCodes[ VK_SUBTRACT ] = KeyCodes::SUBTRACT;
			auiKeyCodes[ KeyCodes::DECIMAL ]		= VK_DECIMAL;	auiWinKeyCodes[ VK_DECIMAL ] = KeyCodes::DECIMAL;
			auiKeyCodes[ KeyCodes::DIVIDE ]			= VK_DIVIDE;	auiWinKeyCodes[ VK_DIVIDE ] = KeyCodes::DIVIDE;
			auiKeyCodes[ KeyCodes::F1 ]				= VK_F1;		auiWinKeyCodes[ VK_F1 ] = KeyCodes::F1;
			auiKeyCodes[ KeyCodes::F2 ]				= VK_F2;		auiWinKeyCodes[ VK_F2 ] = KeyCodes::F2;
			auiKeyCodes[ KeyCodes::F3 ]				= VK_F3;		auiWinKeyCodes[ VK_F3 ] = KeyCodes::F3;
			auiKeyCodes[ KeyCodes::F4 ]				= VK_F4;		auiWinKeyCodes[ VK_F4 ] = KeyCodes::F4;
			auiKeyCodes[ KeyCodes::F5 ]				= VK_F5;		auiWinKeyCodes[ VK_F5 ] = KeyCodes::F5;
			auiKeyCodes[ KeyCodes::F6 ]				= VK_F6;		auiWinKeyCodes[ VK_F6 ] = KeyCodes::F6;
			auiKeyCodes[ KeyCodes::F7 ]				= VK_F7;		auiWinKeyCodes[ VK_F7 ] = KeyCodes::F7;
			auiKeyCodes[ KeyCodes::F8 ]				= VK_F8;		auiWinKeyCodes[ VK_F8 ] = KeyCodes::F8;
			auiKeyCodes[ KeyCodes::F9 ]				= VK_F9;		auiWinKeyCodes[ VK_F9 ] = KeyCodes::F9;
			auiKeyCodes[ KeyCodes::F10 ]			= VK_F10;		auiWinKeyCodes[ VK_F10 ] = KeyCodes::F10;
			auiKeyCodes[ KeyCodes::F11 ]			= VK_F11;		auiWinKeyCodes[ VK_F11 ] = KeyCodes::F11;
			auiKeyCodes[ KeyCodes::F12 ]			= VK_F12;		auiWinKeyCodes[ VK_F12 ] = KeyCodes::F12;
			auiKeyCodes[ KeyCodes::NUMLOCK ]		= VK_NUMLOCK;	auiWinKeyCodes[ VK_NUMLOCK ] = KeyCodes::NUMLOCK;
			auiKeyCodes[ KeyCodes::SCROLL_LOCK ]	= VK_SCROLL;	auiWinKeyCodes[ VK_SCROLL ] = KeyCodes::SCROLL_LOCK;
			auiKeyCodes[ KeyCodes::LEFT_SHIFT ]		= VK_LSHIFT;	auiWinKeyCodes[ VK_LSHIFT ] = KeyCodes::LEFT_SHIFT;
			auiKeyCodes[ KeyCodes::RIGHT_SHIFT ]	= VK_RSHIFT;	auiWinKeyCodes[ VK_RSHIFT ] = KeyCodes::RIGHT_SHIFT;
			auiKeyCodes[ KeyCodes::LEFT_ALT ]		= VK_LMENU;		auiWinKeyCodes[ VK_LMENU ] = KeyCodes::LEFT_ALT;
			auiKeyCodes[ KeyCodes::RIGHT_ALT ]		= VK_RMENU;		auiWinKeyCodes[ VK_RMENU ] = KeyCodes::RIGHT_ALT;
			auiKeyCodes[ KeyCodes::LEFT_CONTROL ]	= VK_LCONTROL;	auiWinKeyCodes[ VK_LCONTROL ] = KeyCodes::LEFT_CONTROL;
			auiKeyCodes[ KeyCodes::RIGHT_CONTROL ]	= VK_RCONTROL;	auiWinKeyCodes[ VK_RCONTROL ] = KeyCodes::RIGHT_CONTROL;
			auiKeyCodes[ KeyCodes::ESCAPE ]			= VK_ESCAPE;	auiWinKeyCodes[ VK_ESCAPE ] = KeyCodes::ESCAPE;
			//auiKeyCodes[ KeyCodes:: ] = VK_;
			//auiKeyCodes[ KeyCodes:: ] = 0x;
		}

	}//win32
#endif
}//xse