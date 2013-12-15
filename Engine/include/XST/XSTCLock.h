
#ifndef XST_CLOCK_H
#define XST_CLOCK_H

#include "XSTCCriticalSection.h"
#include "XSTCMutex.h"

#pragma warning( disable : 4413 ) //warning C4413: '' : reference member is initialized to a temporary that doesn't persist after the constructor exits

namespace XST
{

		class CLockSection
		{
			public:
									//CLockSection() : m_CritSection( CCriticalSection() ) {}
									CLockSection() : m_pCritSection( xst_null ) {}
									CLockSection(CCriticalSection& _CritSection) : m_pCritSection( &_CritSection )
									{
									    xst_assert( m_pCritSection, "(CLockSection::CLockSection) Section not initialized" );
										m_pCritSection->Enter();
									}

									~CLockSection()
									{
									    xst_assert( m_pCritSection, "(CLockSection::CLockSection) Section not initialized" );
										m_pCritSection->Leave();
									}

			private:

				CCriticalSection* m_pCritSection;
		};


		class CLockMutex
		{
			public:

									//CLockMutex() : m_Mutex( CMutex() ) {}
									CLockMutex() : m_pMutex( xst_null ) {}
									CLockMutex(CMutex& _Mutex) : m_pMutex( &_Mutex )
									{
									    xst_assert( m_pMutex, "(CLockMutex::CLockMutex) Mutex not initialized" );
										m_pMutex->Enter();
									}

									~CLockMutex()
									{
									    xst_assert( m_pMutex, "(CLockMutex::CLockMutex) Mutex not initialized" );
										m_pMutex->Leave();
									}

			private:

				CMutex* m_pMutex;
		};

}//core

#pragma warning( default : 4413 )

#endif //XST_CLOCK_H
