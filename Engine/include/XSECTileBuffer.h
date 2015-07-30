#ifndef XSE_CTILEBUFFER_H
#define XSE_CTILEBUFFER_H

#include "XSECommon.h"

namespace XSE  
{
    class CTileBuffer
    {
        public:

            struct Tile
            {
                u32 uiStartOffset;
                u32 uiEndOffset;
                u16 usId : 8;
                u16 usVertexBufferId : 8;
            };

        public:

                    CTileBuffer();
            virtual ~CTileBuffer();

            i32 Create(u32 uiWidth, u32 uiHeight, IInputLayout* pIL)
    };
} // xse

#endif // XSE_CTILEBUFFER_H