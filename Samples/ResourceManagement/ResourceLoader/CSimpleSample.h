#ifndef CSIMPLE_SAMPLE_H
#define CSIMPLE_SAMPLE_H

#include "../SampleMgr/ISample.h"

class CSimpleSample : public ISample
{
	public:

		virtual				~CSimpleSample() {}

		const char*			GetName() const
							{ return "SimpleSample"; }

		i32					Init(XSE::CEngine* pEngine, XSE::IRenderWindow* pWnd);

		i32					Run();

		void				Destroy();

	protected:

		XSE::IRenderWindow*	m_pWnd;
		XSE::CEngine*		m_pEngine;
};

#endif //CSIMPLE_SAMPLE_H