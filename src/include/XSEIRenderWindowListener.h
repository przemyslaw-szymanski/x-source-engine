#ifndef XSE_IWINDOW_LISTENER_H
#define XSE_IWINDOW_LISTENER_H

#include "XSECommon.h"

namespace XSE
{
	class IRenderWindowListener
	{
		public:

			virtual				~IRenderWindowListener() {}

			virtual	i32			OnClosing(const SWindowEvent& Event) { return RESULT::OK; }
			virtual	i32			OnClose(const SWindowEvent& Event) { return RESULT::OK; }
			virtual	i32			OnResize(const SWindowEvent& Event, cu32& uiNewWidth, cu32& uiNewHeight) { return RESULT::OK; }
			virtual	i32			OnMinimize(const SWindowEvent& Event) { return RESULT::OK; }
			virtual i32			OnMaximize(const SWindowEvent& Event) { return RESULT::OK; }
			virtual i32			OnMove(const SWindowEvent& Event, cu32& uiNewXPos, cu32& uiNewYPos) { return RESULT::OK; }
			virtual i32			OnShow(const SWindowEvent& Event) { return RESULT::OK; }
			virtual i32			OnUpdate(const SWindowEvent& Event) { return RESULT::OK; }
			virtual i32			OnBeginRenderFrame(const SWindowEvent& Event) { return RESULT::OK; }
			virtual i32			OnEndRenderFrame(const SWindowEvent& Event) { return RESULT::OK; }
			virtual i32			OnLostFocus(const SWindowEvent& Event) { return XST_OK; }
			virtual i32			OnGetFocus(const SWindowEvent& Event) { return XST_OK; }

		protected:
	};

}//xse

#endif //XSE_IWINDOW_LISTENER_H