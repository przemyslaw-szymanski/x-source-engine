#ifndef XSE_IMOUSE_LISTENER_H
#define XSE_IMOUSE_LISTENER_H

#include "XSECommon.h"

namespace XSE
{
	struct MouseButtons
	{
		enum BUTTON
		{
			LEFT		= 0x80000000,
			RIGHT		= 0x40000000,
			MIDDLE		= 0x20000000,
			WHEEL_UP	= 0x10000000,
			WHEEL_DOWN	= 0x08000000,
			_ENUM_COUNT	= 5
		};
	};
	typedef MouseButtons::BUTTON	MOUSE_BUTTON;

	struct SMouseEvent
	{
		CPoint	Position;
		CPoint	Delta;
		u32		uiButtonDown;
	};

	class IMouseListener
	{
		public:

			virtual				~IMouseListener() {}

			virtual	void		OnMouseMove(const SMouseEvent& Event) { return; }
			virtual void		OnMouseButtonDown(const SMouseEvent& Event) { return; }
			virtual void		OnMouseButtonUp(const SMouseEvent& Event) { return; }
			virtual bool		IsMouseListenerAutoDestroy() { return false; }
	};
}//xse

#endif