
#ifndef XST_CDATA_H
#define XST_CDATA_H

#include "XSTCommon.h"

namespace XST
{
	template<typename _T_>
	class XS_API TCData
	{

		typedef _T_			DataType;
		typedef _T_*		DataPtr;

		public:

							TCData() : m_pData(0), m_ulSize(0) 
							{}

							~TCData()
							{
								Delete();
							}

		xst_fi	DataPtr		GetData() const
							{ return m_pData; }

		xst_fi	void		SetData(DataPtr _pData, ul32 _ulSize)
							{ Delete(); m_pData = _pData; m_ulSize = _ulSize; }

				bool		Create(ul32 _ulSize)
							{
								xst_assert(_ulSize > 0, "Size should be greater than 0");
								Delete();
								m_pData = xst_new DataType[_ulSize];
								if(!m_pData)
									return false;

								return true;
							}

		xst_fi	void		Delete()
							{
								xst_delete [] m_pData;
								m_pData = 0;
								m_ulSize = 0;
							}

				bool		Resize(ul32 _ulNewSize)
							{
								DataPtr pData = Copy();
								ul32 ulSize = m_ulSize;

								Delete();
								if(!Create(_ulNewSize))
								{
									SetData(pData, ulSize);
									return false;
								}

								return true;
							}	

				DataPtr		Copy(ul32 xst_out _pulSize)
							{
								xst_out _pulSize = m_ulSize;
								return Copy();
							}

				DataPtr		Copy()
							{
								DataPtr pData = xst_new DataType[m_ulSize];
								if(!pData)
									return 0;
								
								xst_memcpy(pData, m_pData, m_ulSize);

								return pData;
							}

		protected:

			DataPtr			m_pData;
			ul32			m_ulSize;
	};

}//XST

#endif //XST_CDATA_H