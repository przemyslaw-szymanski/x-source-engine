#ifndef XST_LAST_ERROR_H
#define XST_LAST_ERROR_H

#include "XSTCommon.h"

namespace XST
{
	class XST_API CLastError
	{
		public:

			static	void			Set(xst_castring& _strError)
									{ m_strError = _strError; }

			static	xst_castring&	Get()
									{ return m_strError; }

		private:

			static xst_astring	m_strError;
	};
}//XST

#endif
