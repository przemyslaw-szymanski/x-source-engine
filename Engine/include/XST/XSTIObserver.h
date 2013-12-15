#ifndef XST_IOBSERVER_H
#define XST_IOBSERVER_H

#include "XSTCommon.h"

namespace XST
{
	class IObserver 
	{
		public:

			virtual			~IObserver() {}

			virtual void	OnEnter() {}
			virtual void	OnLeave() {}

	};

}//XST

#endif//XST_OBSERVER_H