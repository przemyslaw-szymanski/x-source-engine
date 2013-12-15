#ifndef XST_CTO_STRING_H
#define XST_CTO_STRING_H

#include "XSTCommonInclude.h"

namespace XST
{
	class CToString
	{
		public:

			//static xst_castring SEPARATOR;
			static const ch8 SEPARATOR = ' ';

		public:

			CToString() {}

			template<class _T_>
			CToString(const _T_* pArray, cu32& uiElemCount)
			{
				Array( pArray, uiElemCount );
			}

			virtual				~CToString() {}

			template<class _T_>
			xst_fi	CToString&	operator<<(const _T_& right)
			{
				m_ssStr << right << SEPARATOR;
				return *this;
			}

			xst_fi xst_astring	operator<<(const CToString& ToStr)
			{
				return ToStr.BuildString();
			}

			xst_fi	void		Clear()
			{ m_ssStr.str( xst_astring( "" ) ); }

			xst_fi	xst_astring	BuildString() const
			{ return m_ssStr.str(); }

			xst_fi	operator	xst_astring() const
			{ return m_ssStr.str(); }	

			template<class _T_>
			xst_fi	CToString&	operator()(const _T_* pArray, cu32& uiElemCount)
			{
				return Array( pArray, uiElemCount );
			}

			template<class _T_>
			xst_fi	CToString&	Array(const _T_* pArray, u32 uiElemCount)
			{
				m_ssStr << "( ";
				for(int i = 0; i < uiElemCount; ++i)
				{
					m_ssStr << pArray[ i ] << SEPARATOR;
				}
				m_ssStr << ")";
				return *this;
			}


		protected:

			xst_stringstream	m_ssStr;
	};

	static xst_fi CToString ToStr()
	{
		return CToString();
	}

}//xst
#endif //XST_CTO_STRING_H