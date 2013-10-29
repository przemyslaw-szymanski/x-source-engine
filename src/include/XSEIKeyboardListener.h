#ifndef XSE_IKEYBOARD_LISTENER_H
#define XSE_IKEYBOARD_LISTENER_H

#include "XSECommon.h"

namespace XSE
{
	struct KeyState
	{
		enum KEY_STATE
		{
			UP = 0,
			DOWN,
			_ENUM_COUNT
		};
	};
	typedef KeyState::KEY_STATE	KEY_STATE;

	struct KeyModifiers
	{
		enum MODIFIER
		{
			NONE			= 0x00000000,
			SHIFT			= 0x80000000,
			ALT				= 0x40000000,
			CONTROL			= 0x20000000,
			LEFT_SHIFT		= 0x10000000,
			RIGHT_SHIFT		= 0x08000000,
			LEFT_CONTROL	= 0x04000000,
			RIGHT_CONTROL	= 0x02000000,
			LEFT_ALT		= 0x01000000,
			RIGHT_ALT		= 0x00800000,
			_ENUM_COUNT		= 10
		};
	};
	typedef KeyModifiers::MODIFIER		KEY_MODIFIER;

	struct KeyCodes
	{
		enum CODE
		{
			UNKNOWN = 0,
			Q,
			W,
			E,
			R,
			T,
			Y,
			U,
			I,
			O,
			P,
			A,
			S,
			D,
			F,
			G,
			H,
			J,
			K,
			L,
			Z,
			X,
			C,
			V,
			B,
			N,
			M,
			CAPITAL_Q,
			CAPITAL_W,
			CAPITAL_E,
			CAPITAL_R,
			CAPITAL_T,
			CAPITAL_Y,
			CAPITAL_U,
			CAPITAL_I,
			CAPITAL_O,
			CAPITAL_P,
			CAPITAL_A,
			CAPITAL_S,
			CAPITAL_D,
			CAPITAL_F,
			CAPITAL_G,
			CAPITAL_H,
			CAPITAL_J,
			CAPITAL_K,
			CAPITAL_L,
			CAPITAL_Z,
			CAPITAL_X,
			CAPITAL_C,
			CAPITAL_V,
			CAPITAL_B,
			CAPITAL_N,
			CAPITAL_M,
			KEYBOARD_1,
			KEYBOARD_2,
			KEYBOARD_3,
			KEYBOARD_4,
			KEYBOARD_5,
			KEYBOARD_6,
			KEYBOARD_7,
			KEYBOARD_8,
			KEYBOARD_9,
			KEYBOARD_0,
			NUM_1,
			NUM_2,
			NUM_3,
			NUM_4,
			NUM_5,
			NUM_6,
			NUM_7,
			NUM_8,
			NUM_9,
			NUM_0,
			BACK,
			TAB,
			CLEAR,
			RETURN,
			SHIFT,
			CONTROL,
			ALT,
			PAUSE,
			CAPS_LOCK,
			SCROLL_LOCK,
			SPACE,
			PAGE_UP,
			PAGE_DOWN,
			END,
			HOME,
			LEFT,
			UP,
			RIGHT,
			DOWN,
			SELECT,
			PRINT,
			EXECUTE,
			PRINT_SCREEN,
			INSERT,
			DEL,
			HELP,
			SLEEP,
			MULTIPLY,
			ADD,
			SEPARATOR,
			SUBTRACT,
			DECIMAL,
			DIVIDE,
			F1,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,
			NUMLOCK,
			SRCOLL,
			LEFT_SHIFT,
			RIGHT_SHIFT,
			LEFT_CONTROL,
			RIGHT_CONTROL,
			LEFT_ALT,
			RIGHT_ALT,
			ESCAPE,
			//SPECIAL
			KANA,
			HANGUL,
			JUNJA,
			FINAL,
			HANJA,
			KANJI,
			_ENUM_COUNT
		};
	};
	typedef KeyCodes::CODE		KEY_CODE;

	struct SKeyboardEvent
	{
		IRenderWindow*	pWnd;
		KEY_STATE		eKeyState;
		u32				uiModifier;
		ch16			aChars[2];
		KEY_CODE		eKeyCode;
	};


	class IKeyboardListener
	{
		public:

			virtual				~IKeyboardListener() {}
			
			virtual	void		OnKeyPressed(const SKeyboardEvent& Event) { return; }
			virtual	void		OnKeyDown(const SKeyboardEvent& Event) { return; }
			virtual	void		OnKeyUp(const SKeyboardEvent& Event) { return; }
			virtual void		OnKeyboardUpdate(const SKeyboardEvent& Event) { return; }
			virtual bool		IsKeyboardListenerAutoDestroy() { return false; }

		protected:
	};

}//xse

#endif