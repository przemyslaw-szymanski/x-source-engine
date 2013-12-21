#ifndef CSAMPLE_MGR_H
#define CSAMPLE_MGR_H

#include <string>
#include <vector>
#include <map>

#include "ISample.h"

class CSampleMgr
{
	public:

		typedef ISample* (*pfnCreateSample)();
		typedef void (*pfnDestroySample)(ISample*);

		struct SSample
		{
			ISample*			pSample;
			pfnCreateSample		CreateSample;
			pfnDestroySample	DestroySample;
		};

		typedef std::map< std::string, SSample >	SampleMap;

	public:

							CSampleMgr();
		virtual				~CSampleMgr();

		i32					InitEngine(u32 uiWindowHandle);

		void				RegisterSamples();

		XSE::CEngine*		GetEngine() const
							{ return m_pEngine; }

		ISample*			GetCurrentSample()
							{ return m_pCurrSample; }

		XSE::IRenderWindow*	GetRenderWindow() const
							{ return this->m_pRenderWnd; }

		void				DestroyEngine();

		void				RenderSample();

		bool				RegisterSample(pfnCreateSample Create, pfnDestroySample Destroy);

		bool				RunSample(xst_castring& strName);

		void				UnregisterSamples();

		void				UnregisterSample(ISample* pSample);

		ISample*			GetSample(xst_castring& strName);

		const SampleMap&	GetSamples() const
							{ return m_mSamples; }

		void				ResetCamera();
		
		void				RenderWireframe();

		void				RenderSolid();

	protected:

		XSE::CEngine*		m_pEngine = xst_null;
		SampleMap			m_mSamples;
		ISample*			m_pCurrSample = xst_null;
		XSE::IRenderWindow*	m_pRenderWnd = xst_null;
};

#endif //CSAMPLE_MGR_H