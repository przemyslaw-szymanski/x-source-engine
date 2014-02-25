#ifndef XST_TCPOOL_H
#define XST_TCPOOL_H

#include "XSTCommon.h"

namespace XST
{
    template< class _T_ >
    class TCPool
    {
        public:

            typedef xst_vector< _T_ >   ObjectVec;
            typedef xst_stack< u32 >    IdStack;

            class CIterator
            {
                public:

                                            CIterator(TCPool& Pool) : m_Pool( Pool )
                                            {}

                        virtual             ~CIterator()
                                            {}

                        _T_&                MoveNext();

                        _T_&                Get();

                        bool                HasNext() const;

                protected:

                    TCPool< _T_ >&  m_Pool;
            };

        public:
            
                                    TCPool();
            virtual                 ~TCPool();

                    i32             Init(u32 uiPoolSize);

                    void            Clear();

            const   ObjectVec&      GetObjects() const
                                    { return m_vObjects; }

        protected:

            ObjectVec       m_vObjects;
            IdStack         m_sFreeIds;
    };
} // XST

#endif // XST_TCPOOL_H