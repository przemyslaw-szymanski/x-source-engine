
#ifndef XST_TCDATA_H
#define XST_TCDATA_H

#include "XSTCommon.h"

namespace XST
{
	template<typename _T_>
	class XST_API TCData
	{
		public:
		
			typedef _T_			DataType;
			typedef _T_*		DataPtr;

			enum
			{
				DATA_TYPE_SIZE = sizeof( _T_ )
			};

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

				bool		Create(ul32 _ulSize, bool _bNullTerminated = false)
							{
								xst_assert(_ulSize > 0, "Size should be greater than 0");
								Delete();
								m_pData = xst_new DataType[ _ulSize + (i32)_bNullTerminated ];
								m_ulSize = _ulSize;
								m_bNullTerminated = _bNullTerminated;
								if( m_bNullTerminated )
									m_pData[ _ulSize ] = 0;
								if(!m_pData)
									return false;

								return true;
							}

				bool		Create(const xst_unknown pSrcData, cul32& ulSrcDataSize, bool bNullTerminated = false)
							{
								if( !Create( ulSrcDataSize, bNullTerminated ) )
								{
									return false;
								}

								xst_memcpy( m_pData, ulSrcDataSize, pSrcData, ulSrcDataSize );
								Terminate();
								
								return true;
							}

				bool		Create(const _T_* pSrcData, cul32& ulSrcDataSize, bool bNullTerminated = false)
							{
								if( !Create( ulSrcDataSize, bNullTerminated ) )
								{
									return false;
								}

								xst_memcpy( m_pData, ulSrcDataSize, pSrcData, ulSrcDataSize );					
								Terminate();

								return true;
							}


				bool		Create(const xst_unknown pSrcData, cul32& ulSrcDataSize, cul32& ulBufferSize, bool bNullTerminated = false)
							{
								if( !Create( ulBufferSize, bNullTerminated ) )
								{
									return false;
								}

								xst_memcpy( m_pData, ulBufferSize * DATA_TYPE_SIZE, pSrcData, ulSrcDataSize );
								Terminate();

								return true;
							}

		xst_fi	void		Delete()
							{
								xst_deletea( m_pData );
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

								xst_memcpy(pData, m_ulSize, m_pData, m_ulSize);

								return pData;
							}

			xst_fi	ul32	GetSize() const
							{ return m_ulSize; }

			xst_fi	bool	IsLoaded() const
							{ return m_pData != 0; }

			xst_fi	bool	IsNullTerminated() const
							{ return m_bNullTerminated; }

			xst_fi	void	Terminate(const _T_& _tValue)
							{ if( m_bNullTerminated ) m_pData[ m_ulSize ] = _tValue; }

			xst_fi	void	Terminate()
							{ if( m_bNullTerminated ) m_pData[ m_ulSize ] = xst_null; }

		protected:

			DataPtr			m_pData;
			ul32			m_ulSize;
			bool			m_bNullTerminated;
	};

}//XST

#endif //XST_CDATA_H
