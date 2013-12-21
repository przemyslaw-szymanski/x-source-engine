#ifndef CSIMPLE_SAMPLE_H
#define CSIMPLE_SAMPLE_H

#include "../SampleMgr/ISample.h"

class CSimpleSample : public ISample
{
	public:

		virtual				~CSimpleSample() {}

		const char*			GetName() const override
							{ return "SimpleSample"; }

		const char*			GetGroupName() const override
							{ return "Default"; }

		i32					Init(XSE::CEngine* pEngine, XSE::IRenderWindow* pWnd);

		i32					Run();

		void				Destroy();

	protected:

		XSE::IRenderWindow*	m_pWnd;
		XSE::CEngine*		m_pEngine;
};

SAMPLE_EXPORT_FUNC( CSimpleSample );

#endif //CSIMPLE_SAMPLE_H