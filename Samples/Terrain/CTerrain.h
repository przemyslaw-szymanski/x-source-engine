#ifndef CTERRAIN_H
#define CTERRAIN_H

#include "../SampleMgr/ISample.h"

class CTerrain : public ISample
{
	public:

		virtual				~CTerrain() {}

		const char*			GetName() const
							{ return "Terrain"; }

		i32					Init(XSE::CEngine* pEngine, XSE::IRenderWindow* pWnd);

		i32					Run();

		void				Destroy();

		void				OnUpdate();

		void				OnKeyPressEvent( int iKey );

	protected:

		XSE::IRenderWindow*	m_pWnd;
		XSE::CEngine*		m_pEngine;
};

SAMPLE_EXPORT_FUNC( CTerrain );

#endif //CSIMPLE_SAMPLE_H