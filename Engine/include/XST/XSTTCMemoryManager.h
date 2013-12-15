#ifndef XST_TCMEMORY_MANAGER_H
#define XST_TCMEMORY_MANAGER_H

#include "XSTCommon.h"
#include "XSTIAllocator.h"

namespace XST
{

	typedef struct _XSTSMemoryBlock
	{
		//u8* pNextFree;
		u32 uiNextFree;
		bool bFree;
	}SMemoryBlock;

	template<class  _T_>
	class TCMemoryManager
	{
		public:

			typedef xst_stack<SMemoryBlock>	BlockStack;

		public:

			TCMemoryManager()
			{
				m_uiMemBlocks	= 0;
				m_uiBlockSize	= 0;
				m_uiBufferSize	= 0;
				m_pMemoryBuffer	= 0;
			}

			~TCMemoryManager()
			{
				xst_deletea(m_pMemoryBuffer);
			}

			i32		Init(u32 _uiElementCount)
			{
				m_uiMemBlocks	= _uiElementCount;
				m_uiBlockSize	= sizeof(_T_);// + sizeof(SMemoryBlock);
				m_uiBufferSize	= m_uiMemBlocks * m_uiBlockSize;
				m_pMemoryBuffer = xst_new u8[m_uiBufferSize];
				if(!m_pMemoryBuffer)
				{
					return RESULT::FAILED;
				}

				//Set first free memory chunk
				SMemoryBlock MemBlock;
				MemBlock.bFree = true;
				MemBlock.uiNextFree = 0; //m_pMemoryBuffer;// + sizeof(SMemoryBlock); 
				m_sFreeMemBlocks.push(MemBlock);

				return RESULT::OK;
			}

			_T_*	Allocate()
			{
				//Get free chunk
				if(m_sFreeMemBlocks.empty())
					return 0;

				SMemoryBlock MemBlock = m_sFreeMemBlocks.top();
				m_sFreeMemBlocks.pop();
				
				u8* pMemBlock = m_pMemoryBuffer + MemBlock.uiNextFree;

				MemBlock.uiNextFree += m_uiBlockSize;
				if( (m_pMemoryBuffer + MemBlock.uiNextFree) < (m_pMemoryBuffer + m_uiBufferSize))
				{
					m_sFreeMemBlocks.push(MemBlock);
				}

				return (_T_*)pMemBlock;
			}

		private:

			BlockStack	m_sFreeMemBlocks;
			u8*			m_pMemoryBuffer;
			u32			m_uiBufferSize;
			u32			m_uiMemBlocks;
			u32			m_uiBlockSize;
	};

	template<class _T_>
	class TCFreeListMemoryManager : public IAllocator, public TCSingleton< TCFreeListMemoryManager< _T_ > >
	{
		public:

			struct SFreeBlock
			{
				SFreeBlock* pNext;
			};

			typedef u8			DataType;
			typedef DataType*	MemoryPool;	
			
			enum
			{
				ELEMENT_SIZE = sizeof( _T_ )
			};

			TCFreeListMemoryManager(cu32& uiElementCount)
			{
				m_bMemoryPoolCreated = true;

				m_uiElementCount = uiElementCount;
				m_uiMemorySize = uiElementCount * ELEMENT_SIZE;
				m_pMemoryPool = xst_new DataType[ m_uiMemorySize ];
				if( m_pMemoryPool == xst_null )
				{
					m_bMemoryPoolCreated = false;
					return;
				}

				//Initialize all block with free blocks
				MemoryPool pMem = m_pMemoryPool;
				SFreeBlock* pLastBlock = xst_null, *pCurrBlock = xst_null;

				for(u32 i = 0; i < m_uiElementCount; ++i)
				{
					pCurrBlock = (SFreeBlock*)pMem;
					pCurrBlock->pNext = pLastBlock;
					pLastBlock = pCurrBlock;
					pMem += ELEMENT_SIZE;
				}

				m_pFirstFreeBlock = pLastBlock;
				m_uiFreeBlockCount = m_uiElementCount;
			}

			virtual ~TCFreeListMemoryManager()
			{
				xst_deletea( m_pMemoryPool );
			}

			virtual void*			Allocate() 
			{ 
				return Alloc(); 
			}

			virtual void*			Allocate(u32 uiSize) 
			{ 
				return Alloc(); 
			}

			virtual void*			Allocate(void* pData) 
			{ 
				return Alloc(); 
			}

			virtual void*			Allocate(void* pData, u32 uiDataSize) 
			{ 
				return Alloc(); 
			}

			virtual void*			Allocate(u32 _uiElementCount, u32 _uiElementSize) 
			{ 
				return Alloc(); 
			}

			virtual void			Deallocate(void* pPtr) 
			{ 
				Delete( (_T_*)pPtr );
			}

			virtual void			Deallocate(void** pPtr) 
			{ 
				Delete( (_T_*)*pPtr );
			}

			virtual void			Deallocate(void* pPtr, u32 uiSize) 
			{
				Delete( (_T_*)pPtr );
			}

			virtual void			Deallocate(void* pPtr, u32 uiElementCount, u32 uiElementSize) 
			{ 
				Delete( (_T_*)pPtr );
			}

			virtual xst_fi	_T_*	Alloc()
			{
				if( m_pFirstFreeBlock == xst_null )
				{
					return xst_null;
				}

				_T_* pPtr = (_T_*)m_pFirstFreeBlock;
				m_pFirstFreeBlock = m_pFirstFreeBlock->pNext;
				--m_uiFreeBlockCount;
				return pPtr;
			}

			virtual xst_fi	void	Delete(_T_* pPtr)
			{
				SFreeBlock* pFreeBlock = (SFreeBlock*)pPtr;
				pFreeBlock->pNext = m_pFirstFreeBlock;
				m_pFirstFreeBlock = pFreeBlock;
				++m_uiFreeBlockCount;
			}

		protected:

			MemoryPool	m_pMemoryPool;
			bool		m_bMemoryPoolCreated;
			u32			m_uiMemorySize;
			u32			m_uiElementCount;
			SFreeBlock*	m_pFirstFreeBlock;
			u32			m_uiFreeBlockCount;
	};

}//XST

#endif