#ifndef XST_CSTACK_H
#define XST_CSTACK_H

#include "XSTCommonInclude.h"
#include "XSTCommonTypes.h"

namespace XST
{
    using namespace Types;

	template<class  _T_>
	class TCStack
	{
		public:

						TCStack() : m_uiSize(0)
						{
						}

			explicit	TCStack(u32 _uiSize) : m_uiSize(0)
			{
				m_vStack.reserve(_uiSize);
			}

						~TCStack()
						{
							m_vStack.clear();
							std::vector<_T_>().swap(m_vStack);
						}

			void push(const _T_& _tValue)
			{
				if(m_uiSize < m_vStack.size())
				{
					m_vStack[m_uiSize] = _tValue;
				}
				else
				{
					m_vStack.push_back(_tValue);
				}

				++m_uiSize;
			}

			xst_fi void	pop()
			{
				assert(m_uiSize > 0);
				--m_uiSize;
			}

			xst_fi const _T_& top() const
			{
				return m_vStack[m_uiSize - 1];
			}

			xst_fi const _T_& top_pop()
			{
				const _T_& t = top();
				pop();
				return t;
			}

			xst_fi u32	size() const
			{
				return m_uiSize;
			}

			xst_fi _T_	operator[](u32 _uiIndex)
			{
				assert(_uiIndex < m_uiSize);
				return m_vStack[_uiIndex];
			}

			xst_fi bool	empty() const
			{
				return m_uiSize == 0;
			}

			xst_fi void	clear()
			{
				m_vStack.clear();
			}

		private:

			//_T_*		m_atStack;
			#if defined (XST_STL)
			std::vector<_T_>	m_vStack;
			#else

			#endif
			u32					m_uiSize;
	};

	#define  xst_stack  XST::TCStack

}//XST

#endif
