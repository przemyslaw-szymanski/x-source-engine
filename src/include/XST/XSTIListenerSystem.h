#ifndef XST_ILISTENER_SYSTEM_H
#define XST_ILISTENER_SYSTEM_H

#include "XSTCommonTypes.h"
#include "XSTCommonInclude.h"

namespace XST
{

	template<class _T_>
	class IListenerSystem
	{
		public:

			typedef xst_vector< _T_* >	ListenerVector;

		public:

			virtual					~IListenerSystem() {}



		protected:

			ListenerVector	m_vListeners;
	};

}//xst

#endif