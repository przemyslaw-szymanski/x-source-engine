#ifndef XSE_CWIN32_KEYBOARD_H
#define XSE_CWIN32_KEYBOARD_H

#include "XSEIKeyboard.h"

namespace XSE
{
#if defined (XST_WINDOWS)
	namespace Win32
	{
		class CRenderWindow;

		class CKeyboard : public IKeyboard
		{
			friend class CWin32RenderWindow;
			friend LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

			typedef XST::CConstLengthAsciiString< 5 >	LangStr;
			typedef xst_map< LangStr, XST::xst_astr8 >	LangMap;
			typedef xst_vector< IKeyboardListener* >	ListenerVector;

			public:

											CKeyboard(CRenderWindow* pWnd);
				virtual						~CKeyboard();

						i32					Init();

						void				Update(cf32& fElapsedTime, cf32& fTimeSinceLastUpdate);

						i32					SetLocalization(xst_castring& strLang);

						i32					AddLanguage(xst_castring& strLang, xst_castring& strCode);
						
						xst_castring&		GetLocalization() const
											{ return m_strCurrLang; }
						
						KEY_CODE			GetPressedKey() const
											{ return m_KeyEvent.eKeyCode; }

						u32					GetPressedModifiers() const
											{ return m_KeyEvent.uiModifier; }

						bool				IsKeyPressed(KEY_CODE eCode) const;

						bool				IsKeyToggled(KEY_CODE eCode) const
											{ return GetAsyncKeyState( m_abyWinKeyCodes[ eCode ] ) & 0x8000; }

						void				AddListener(IKeyboardListener* pListener)
											{ m_vListeners.push_back( pListener ); }


						void				RemoveListeners(bool bDestroy);

				xst_fi	void				RemoveListener(IKeyboardListener* pListener, bool bDestroy)
											{ 
												if( m_vListeners.size() > 0 )
												{
													m_vListeners.erase( std::find( m_vListeners.begin(), m_vListeners.end(), pListener ) ); 
													if( bDestroy && pListener->IsKeyboardListenerAutoDestroy() )
													{
														delete pListener;
													}
												}
											}

			protected:

						void				_OnKeyDown(WPARAM usParam, LPARAM lParam);
						void				_OnKeyUp(WPARAM usParam, LPARAM lParam);
						void				_OnSysKeyDown(WPARAM usParam, LPARAM lParam);
						void				_OnSysKeyUp(WPARAM usParam, LPARAM lParam);
						void				_OnChar(WPARAM usParam, LPARAM lParam);
						void				_OnUniChar(WPARAM wParam, LPARAM lParam);
						void				_OnSysChar(WPARAM usParam, LPARAM lParam);
						void				_OnInput(WPARAM wParam, LPARAM lParam);
						u16					_TranslateChar(WPARAM wParam, LPARAM lParam, u32 uiFlags);

			protected:

				SKeyboardEvent	m_KeyEvent;
				u32				m_auiKeyCodes[ KeyCodes::_ENUM_COUNT ]; //mapping engine to windows
				u8				m_abyWinKeyCodes[ 256 ]; //maping windows to engine
				u8				m_abyKeyboardState[ 256 ];
				RAWKEYBOARD		m_WinRawKeyboard;
				HKL				m_WinHKL;
				LangMap			m_mLangs;
				xst_astring		m_strCurrLang;
				ListenerVector	m_vListeners;
				CRenderWindow*	m_pWnd;
				ListenerVector::iterator	m_ListenerItr;
		};

	}//win32
#endif
}//xse

#endif