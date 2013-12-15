#ifndef XST_OBSERVABLE_H
#define XST_OBSERVABLE_H

#include "XSTIObserver.h"

namespace XST
{
	template<class  _T_>
	class TIObservable
	{
		public:

			typedef xst_map<xst_string, _T_*>	ObserverMap;
			typedef xst_pair<xst_string, _T_*>	ObserverPair;
			typedef xst_vector<_T_*>			ObserverVector;
		
		public:

			virtual	~TIObservable<_T_>() { XSTVectorClear(m_vObservers, _T_*); }

			xst_fi void AddObserver(_T_* _pObserver)
			{
				m_vObservers.push_back(_pObserver);
				_pObserver->OnEnter();
			}

			void RemoveObserver()
			{
			}

		protected:

			xst_fi ObserverVector&	_GetObservers() const
			{ 
				return m_vObservers; 
			}


		protected:

			ObserverMap		m_mObservers;
			ObserverVector	m_vObservers;
	};

}//XST

#endif//XST_OBSERVABLE_H