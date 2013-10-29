
#ifndef CCRITICALSECTION_H
#define CCRITICALSECTION_H

#include "XSTCommonInclude.h"
#include "XSTCommonTypes.h"

namespace XST
{
		using namespace XST::Types;

		class XST_API CCriticalSection2
		{
			friend class CLockSection;

			public:

						CCriticalSection2();

				void	Enter();

				void	Leave();

				bool	Try();

			private:

				bool	SetLock(const l32 _lThreadID);

			private:

				l32		m_lLockCount;
				l32		m_lThreadID;
		};

		class XST_API CCriticalSection
		{
			public:

						CCriticalSection();
						~CCriticalSection();

				void	Enter();

				void	Leave();

			private:

			#if defined (XST_WINDOWS)
				::CRITICAL_SECTION	m_CritSection;
			#else

			#endif
		};

}//XST

#endif //CCRITICALSECTION_H
