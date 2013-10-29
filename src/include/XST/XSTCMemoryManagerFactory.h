
#ifndef XST_CMEMORYMANAGERFACTORY_H
#define XST_CMEMORYMANAGERFACTORY_H

#include "XSTCommon.h"
#include "XSTTCSingleton.h"
#include "XSTIMemoryManager.h"

namespace XST
{

	class CMemoryManagerFactory : public XST::TCSingleton<CMemoryManagerFactory>
	{

		typedef xst_vector<IMemoryManager*>	MemMgrVector;

		public:

											CMemoryManagerFactory()
											{
											}

											~CMemoryManagerFactory()
											{

											}



		private:

			MemMgrVector		m_vManagers;
	};

}//XST

#endif //#ifndef XST_CMEMORYMANAGERFACTORY_H
