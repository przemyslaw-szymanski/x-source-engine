#ifndef XSE_IKEYBOARD_H
#define XSE_IKEYBOARD_H

#include "XSEIKeyboardListener.h"

namespace XSE
{
	class IKeyboard
	{
		public:

		public:

			virtual						~IKeyboard() {}

			virtual i32					Init() = 0;
			virtual void				Update(cf32& fElapsedTime, cf32& fTimeSinceLastUpdate) = 0;
			virtual i32					SetLocalization(xst_castring& strLang) = 0;
			virtual i32					AddLanguage(xst_castring& strLang, xst_castring& strCode) = 0;
			virtual xst_castring&		GetLocalization() const = 0;
			virtual KEY_CODE			GetPressedKey() const = 0;
			virtual u32					GetPressedModifiers() const = 0;
			virtual bool				IsKeyPressed(KEY_CODE eCode) const = 0;
			virtual bool				IsKeyToggled(KEY_CODE eCode) const = 0;
			virtual void				AddListener(IKeyboardListener* pListener) = 0;
			virtual void				RemoveListener(IKeyboardListener* pListener, bool bDestroy = true) = 0;
			virtual void				RemoveListeners(bool bDestroy = true) = 0;
	};
}//xse

#endif