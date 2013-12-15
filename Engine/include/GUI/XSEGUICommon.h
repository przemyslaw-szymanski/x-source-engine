#ifndef XSE_GUI_COMMON_H
#define XSE_GUI_COMMON_H

#include "XSECommon.h"

namespace XSE
{
	namespace GUI
	{
		class CComponent;
		class CWindow;
		class CPanel;
		class CButton;
		
		typedef l32*	HANDLE;

		struct ComponentTypes
		{
			enum TYPE
			{
				UNKNOWN = 0,
				PANEL,
				BUTTON,
				WINDOW,
				COMBO_BOX,
				CHECK_BOX,
				RADIO_BUTTON,
				HORIZONTAL_SCROLL,
				VERTICAL_SCROLL,
				_ENUM_COUNT
			};
		};
		typedef ComponentTypes::TYPE	COMPONENT_TYPE;
		typedef ComponentTypes			CTs;
		typedef CTs::TYPE				CT;

		struct ComponentStyles
		{
			enum STYLE
			{
				NONE			= 0x00000000,
				CLOSE_BTN		= 0x80000000,
				MINIMIZE_BTN	= 0x40000000,
				MAXIMIZE_BTN	= 0x20000000,
				TITLE_BAR		= 0x10000000,
				_ENUM_COUNT		= 5
			};
		};
		typedef ComponentStyles::STYLE	COMPONENT_STYLE;
		typedef ComponentStyles			CSs;
		typedef CSs::STYLE				CS;

		struct ComponentMetricsTypes
		{
			enum TYPE
			{
				PIXEL = 0,
				PERCENT,
				_ENUM_COUNT
			};
		};
		typedef ComponentMetricsTypes::TYPE	COMPONENT_METRICS_TYPE;
		typedef ComponentMetricsTypes		CMTs;
		typedef CMTs::TYPE					CMT;

	}//gui
}//xse

#endif