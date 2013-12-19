#ifndef CSAMPLE_TOOL_BOX_H
#define CSAMPLE_TOOL_BOX_H

#include <QtGui\qtoolbox.h>
#include <QtGui\qmessagebox.h>

class CSampleMgr;

class CSampleToolBox : public QToolBox
{
	public:

		CSampleToolBox(QWidget* pParent = NULL) : QToolBox( pParent ) {}
		~CSampleToolBox() {}


};

#endif