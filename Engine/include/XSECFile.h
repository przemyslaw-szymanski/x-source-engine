#ifndef XSE_CFILE_H
#define XSE_CFILE_H

#include "XSECommon.h"
#include "XSTTCData.h"

namespace XSE
{
	namespace Resources
	{
		class XST_API CFile : public XST::IObject
		{
			friend class CResourceFileManager;
			public:

				typedef XST::TCData< u8 >	Data;

			public:

			protected:

				Data		m_Data;
				ul32		m_ulHandle;
				ul32		m_ulGroupHandle;
				ul32		m_ulInfoHandle;
		};

		//typedef XST::TCObjectSmartPointer< CFile >	FilePtr;
		//typedef XST::TCWeakPointer< CFile >			FileWeakPtr;
	}
}

#endif // XSE_CFILE_H