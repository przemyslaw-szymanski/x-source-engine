#ifndef XSE_CFILE_H
#define XSE_CFILE_H

#include "XSECommon.h"
#include "XST/XSTTCData.h"

namespace XSE
{
	namespace Resources
	{
		class XST_API CFile : public XST::IObject
		{
			friend class CResourceFileManager;
			public:

				typedef XST::TCData< u8 >	Data;
				typedef XST::TCObjectSmartPointer< CFile >	FilePtr;
				typedef XST::TCWeakPointer< CFile >			FileWeakPtr;

			public:

				xst_fi Data&		GetData()
									{ return m_Data; }

				xst_fi 
				const Data&			GetData() const
									{ return m_Data; }

				xst_fi ul32			GetInfoHandle() const
									{ return m_ulInfoHandle; }

			protected:

				Data		m_Data;
				ul32		m_ulHandle;
				ul32		m_ulGroupHandle;
				ul32		m_ulInfoHandle;
		};
	} // resources

	typedef Resources::CFile::FilePtr		ResFilePtr;
	typedef Resources::CFile::FileWeakPtr	ResFileWeakPtr;
} // xse

#endif // XSE_CFILE_H