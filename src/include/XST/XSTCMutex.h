#ifndef XST_MUTEX_H
#define XST_MUTEX_H

#include "XSTCommonInclude.h"
#include "XSTCommonTypes.h"

namespace XST
{
	class CMutex
	{
		friend class CLockMutex;

		public:

						CMutex() 
						{
							#if defined (XST_WINDOWS)
								InitializeCriticalSection( &m_CritSection );
							#else

							#endif
						}

						~CMutex()
						{
							#if defined (XST_WINDOWS)
								DeleteCriticalSection( &m_CritSection );
							#else

							#endif
						}

		private:

			xst_fi	void	Enter()
			{
				#if defined (XST_WINDOWS)
					EnterCriticalSection( &m_CritSection );
				#else

				#endif
			}

			xst_fi	void	Leave()
			{
				#if defined (XST_WINDOWS)
					LeaveCriticalSection( &m_CritSection );
				#else

				#endif
			}

		private:

			#if defined (XST_WINDOWS)
				CRITICAL_SECTION	m_CritSection;
			#else

			#endif
	};

}//xst

#endif