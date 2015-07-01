
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

							TCData()
							{}

				virtual		~TCData()
							{
								Delete();
							}

		xst_fi	DataPtr		GetPointer()
							{ return m_pData; }

		xst_fi	
		const	DataPtr		GetPointer() const
							{ return m_pData; }

		xst_fi	void		SetSharedData(const DataPtr pData, ul32 uSize, bool bNullTerminated)
		{ 
			Delete();
			m_pData = pData;
			m_ulSize = uSize;
			m_bNullTerminated = bNullTerminated;
		}

		xst_fi	bool		Copy(const DataPtr pSrc, ul32 ulSize, bool bIsNullTerminated)
							{ 
								if( Create( pSrc, ulSize, bIsNullTerminated ) )
									return true;
                                return false;
							}

		xst_fi	void		Move(DataPtr* ppData, ul32 uSize, bool bIsNullTerminated)
							{
								Delete();
								m_pData = (*ppData);
								m_ulSize = uSize;
								m_bNullTerminated = bIsNullTerminated;
								Terminate();
								(*ppData) = xst_null;
							}

				bool		Create(ul32 _ulSize, bool _bNullTerminated = false)
							{
								xst_assert(_ulSize > 0, "Size should be greater than 0");
								Delete();
								m_pData = xst_new DataType[ _ulSize + (i32)_bNullTerminated ];
								if(!m_pData)
									return false;
								m_ulSize = _ulSize;
								m_bNullTerminated = _bNullTerminated;
								Terminate();
								return true;
							}

				bool		Create(const xst_unknown pSrcData, cul32& ulSrcDataSize, bool bNullTerminated = false)
							{
								if( !Create( ulSrcDataSize, bNullTerminated ) )
									return false;
								xst_memcpy( m_pData, ulSrcDataSize, pSrcData, ulSrcDataSize );
								Terminate();
								return true;
							}

				bool		Create(const _T_* pSrcData, cul32& ulSrcDataSize, bool bNullTerminated = false)
							{
								if( !Create( ulSrcDataSize, bNullTerminated ) )
									return false;
								xst_memcpy( m_pData, ulSrcDataSize, pSrcData, ulSrcDataSize );					
								Terminate();
								return true;
							}


				bool		Create(const xst_unknown pSrcData, cul32& ulSrcDataSize, cul32& ulBufferSize, bool bNullTerminated = false)
							{
								if( !Create( ulBufferSize, bNullTerminated ) )
									return false;
								xst_memcpy( m_pData, ulBufferSize * DATA_TYPE_SIZE, pSrcData, ulSrcDataSize );
								Terminate();
								return true;
							}

		xst_fi	void		Delete()
							{
								if( !m_bShared )
									xst_deletea( m_pData );
								m_ulSize = 0;
								m_bNullTerminated = false;
								m_pData = xst_null;
								m_bShared = false;
							}

				bool		Resize(ul32 ulNewSize)
							{
								DataPtr pData = Copy();
								ul32 ulSize = m_ulSize;
								Delete();
								if( !Create( ulNewSize ) )
								{
									Move( &pData, ulSize, m_bNullTerminated );
									return false;
								}
								m_ulSize = ulNewSize;
								return true;
							}

				DataPtr		Copy(ul32* pulSize)
							{
								*pulSize = m_ulSize;
								return Copy();
							}

				DataPtr		Copy()
							{
								DataPtr pData = xst_new DataType[ m_ulSize + m_bNullTerminated ];
								if( !pData )
									return xst_null;
								xst_memcpy( pData, m_ulSize, m_pData, m_ulSize );
								if( m_bNullTerminated )
									pData[ m_ulSize ] = (DataType)0;
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
							{ if( m_bNullTerminated ) m_pData[ m_ulSize ] = (DataType)0; }

		protected:

			DataPtr			m_pData = xst_null;
			ul32			m_ulSize = 0;
			bool			m_bNullTerminated = false;
			bool			m_bShared = false;
            u8 m_padding[ 2 ];
	};

}//XST

#endif //XST_CDATA_H
