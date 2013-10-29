
#ifndef XST_IMEMORYMANAGER_H
#define XST_IMEMORYMANAGER_H

#include "XSTCommon.h"
#include "XSTIObject.h"

namespace XST
{

	XST_INTERFACE(IMemoryManager)
	{
		public:

			virtual				~IMemoryManager() {}

		protected:

			XST_DECLPV(bool, OnRelease());

	};

}//XST

#endif //#ifndef XST_IMEMORYMANAGER_H
