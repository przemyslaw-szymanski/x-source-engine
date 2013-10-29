#ifndef XSE_IMOUSE_H
#define XSE_IMOUSE_H

#include "XSEIMouseListener.h"

namespace XSE
{
	class IMouse
	{
		public:

			virtual					~IMouse() {}
			virtual void			AddListener(IMouseListener* pListener) = 0;
			virtual void			RemoveListener(IMouseListener* pListener, bool bDestroy = true) = 0;
			virtual void			RemoveListeners(bool bDestroy = true) = 0;
			virtual bool			IsCapturedOutsideTheWindow() const = 0;
			virtual void			IsCapturedOutsideTheWindow(bool bIs) = 0;
			virtual const CPoint&	GetPosition() const = 0;
			virtual const CPoint&	GetDelta() const = 0;
	};
}//xse

#endif