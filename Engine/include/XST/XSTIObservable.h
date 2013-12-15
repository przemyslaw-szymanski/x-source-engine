#ifndef XST_IOBSERVABLE_H
#define XST_IOBSERVABLE_H

#include "XSTIObserver.h"

namespace XST
{
	class IObservable
	{
		public:

			typedef xst_map<xst_string, IObserver*>		ObserverMap;
			typedef xst_pair<xst_string, IObserver*>	ObserverPair;
			typedef xst_vector<IObserver*>				ObserverVector;
		
		public:

			virtual	~IObservable() { XSTVectorClear(m_vObservers, IObserver*); }

			xst_fi void AddObserver(IObserver* _pObserver)
			{
				m_vObservers.push_back(_pObserver);
				_pObserver->OnEnter();
			}

			void RemoveObserver()
			{
			}

		protected:

			xst_fi ObserverVector	_GetObservers() const
			{ 
				return m_vObservers; 
			}


		protected:

			ObserverMap		m_mObservers;
			ObserverVector	m_vObservers;
	};

}//XST

#endif//XST_IOBSERVABLE_H