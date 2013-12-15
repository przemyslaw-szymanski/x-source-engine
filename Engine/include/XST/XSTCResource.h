
#ifndef XST_CRESOURCE_H
#define XST_CRESOURCE_H

#include "XSTTCData.h"
#include "XSTIObject.h"

namespace XST
{

	class CResource : public virtual IObject
	{
		public:

								CResource() : m_ulID(0) {}
								CResource(ul32 _ulID) : m_ulID(_ulID) {}
								CResource(const xst_string &_strName) : m_ulID(0), m_strName(_strName) {}
								CResource(ul32 _ulID, const xst_string &_strName) : m_ulID(_ulID), m_strName(_strName) {}

		virtual					~CResource() {}

		xst_fi ul32				GetHandle() const
								{ return m_ulID; }

		xst_fi xst_string		&GetName()
								{ return m_strName; }

		protected:

		xst_fi	void			SetName(const xst_string &_strName)
								{ m_strName = _strName; }

		xst_fi void				SetID(ul32 _ulID)
								{ m_ulID = _ulID; }

		virtual	bool			OnRelease() = 0;

		protected:

			xst_string			m_strName;
			ul32				m_ulID;


	};

}//XST

#endif //XST_CRESOURCE_H
