#ifndef XST_TCITERATOR_H
#define XST_TCITERATOR_H

#include "XSTCommonTypes.h"
#include "XSTCommonInclude.h"

#if defined (XST_VISUAL_STUDIO)
#pragma warning( push )
#endif
#pragma warning( disable: 4251 )

namespace XST
{
	template<class _CONTAINER_TYPE_>
	class XST_API TCIterator
	{
		public:

			typedef typename _CONTAINER_TYPE_::value_type	Type;

		public:

									TCIterator(_CONTAINER_TYPE_* pCont) : m_pContainer ( pCont ), m_Itr( pCont->begin() ) {}

			virtual					~TCIterator() {}

					bool			HasMoreElements()
									{ return m_Itr != m_pContainer->end(); }

					Type&			GetValue()
									{ return *m_Itr; }

					void			MoveNext()
									{ ++m_Itr; }

		protected:

			_CONTAINER_TYPE_*					m_pContainer;
			typename _CONTAINER_TYPE_::iterator	m_Itr;
	};

	template<class _CONTAINER_TYPE_>
	class XST_API TCConstIterator
	{
		public:

			typedef typename _CONTAINER_TYPE_::value_type	Type;

		public:

									TCConstIterator(const _CONTAINER_TYPE_* pCont) : m_pContainer ( pCont ), m_Itr( pCont->begin() ) {}

			virtual					~TCConstIterator() {}

					bool			HasMoreElements()
									{ return m_Itr != m_pContainer->end(); }

			const	Type&			GetValue()
									{ return *m_Itr; }

					void			MoveNext()
									{ ++m_Itr; }

		protected:

			const _CONTAINER_TYPE_*						m_pContainer;
			typename _CONTAINER_TYPE_::const_iterator	m_Itr;
	};

	template<class _CONTAINER_TYPE_>
	class XST_API TCPairIterator
	{
		public:

			typedef typename _CONTAINER_TYPE_::key_type					KeyType;
			typedef typename _CONTAINER_TYPE_::value_type::second_type	ValueType;

		public:

									TCPairIterator(_CONTAINER_TYPE_* pCont) : m_pContainer ( pCont ), m_Itr( pCont->begin() ) {}

			virtual					~TCPairIterator() {}

					bool			HasMoreElements()
									{ return m_Itr != m_pContainer->end(); }

					KeyType&		GetKey()
									{ return m_Itr->first; }

					ValueType&		GetValue()
									{ return m_Itr->second; }

					void			MoveNext()
									{ ++m_Itr; }

		protected:

			_CONTAINER_TYPE_*			m_pContainer;
			typename _CONTAINER_TYPE_::iterator	m_Itr;
	};

	template<class _CONTAINER_TYPE_>
	class XST_API TCConstPairIterator
	{
		public:

			typedef typename _CONTAINER_TYPE_::key_type					KeyType;
			typedef typename _CONTAINER_TYPE_::value_type::second_type	ValueType;

		public:

									TCConstPairIterator(_CONTAINER_TYPE_* pCont) : m_pContainer ( pCont ), m_Itr( pCont->begin() ) {}

			virtual					~TCConstPairIterator() {}

					bool			HasMoreElements()
									{ return m_Itr != m_pContainer->end(); }

			const	KeyType&		GetKey()
									{ return m_Itr->first; }

			const	ValueType&		GetValue()
									{ return m_Itr->second; }

					void			MoveNext()
									{ ++m_Itr; }

		protected:

			_CONTAINER_TYPE_*			m_pContainer;
			typename _CONTAINER_TYPE_::iterator	m_Itr;
	};

}//xst

#if defined(XST_VISUAL_STUDIO)
#pragma warning( pop )
#endif//XST_VISUAL_STUDIO

#endif