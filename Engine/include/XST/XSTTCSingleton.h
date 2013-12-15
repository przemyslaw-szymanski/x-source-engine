
#ifndef XST_TCSINGLETON_H
#define XST_TCSINGLETON_H

#include <cassert>
#pragma warning(disable: 4312) //Warning	110	warning C4312: 'type cast' : conversion from 'XST::i32' to 'Core::Input::CInputManager *' of greater size
#pragma warning(disable: 4311) //Warning	115	warning C4311: 'type cast' : pointer truncation from 'Game::GUI::CGUIManager *' to 'XST::i32'


namespace XST
{
		template<class  _T_>
		class TCSingleton 
		{
			public:

				TCSingleton()
				{
					assert(!m_pSingleton && "TCSingleton(): Singleton already created");
					i32 iOffset = (i32)((i32)(_T_*)1 - (i32)(TCSingleton<_T_>*)(_T_*)1);
					m_pSingleton = (_T_*)((i32)this + iOffset);
				}

				virtual ~TCSingleton()
				{
					assert(m_pSingleton && "~TCSingleton(): Singleton must be created");
					//delete m_pSingleton;
					m_pSingleton = 0;
					//m_pSingleton = 0;
					/*
					if(m_pSingleton)
					{
						delete m_pSingleton;
						m_pSingleton = 0;
					}
					*/
				}

				
				static inline _T_		*GetSingletonPtr()
				{
					assert(m_pSingleton != 0 && "GetSingletonPtr(): Singleton must be created");
					return m_pSingleton;
				}

				static inline _T_&		GetSingleton()
				{
					assert(m_pSingleton != 0 && "GetSingletonPtr(): Singleton must be created");
					return (*m_pSingleton);
				}

				static inline bool		IsSingletonCreated()
				{
					return m_pSingleton != 0;
				}
				

			private:

				static _T_	*m_pSingleton;
		};

		template<class  _T_> _T_ *TCSingleton<_T_>::m_pSingleton = 0;

}//XST



#endif //TCSINGLETON