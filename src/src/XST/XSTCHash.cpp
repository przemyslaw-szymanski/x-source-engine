#include "../../include/XST/XSTCHash.h"

namespace XST
{
    ul32	CHash::GetCRC(lpcastr _lpszString, u32 _uiStringLength)
    {
        ul32 hash = _uiStringLength, tmp;
        int rem;

            if (_uiStringLength <= 0 || _lpszString == NULL) return 0;

            rem = _uiStringLength & 3;
            _uiStringLength >>= 2;

            for (;_uiStringLength > 0; _uiStringLength--) {
                hash  += xst_get16bits (_lpszString);
                tmp    = (xst_get16bits (_lpszString+2) << 11) ^ hash;
                hash   = (hash << 16) ^ tmp;
                _lpszString  += 2*sizeof (u16);
                hash  += hash >> 11;
            }

            switch (rem) {
                case 3: hash += xst_get16bits (_lpszString);
                        hash ^= hash << 16;
                        hash ^= _lpszString[sizeof (u16)] << 18;
                        hash += hash >> 11;
                        break;
                case 2: hash += xst_get16bits (_lpszString);
                        hash ^= hash << 11;
                        hash += hash >> 17;
                        break;
                case 1: hash += *_lpszString;
                        hash ^= hash << 10;
                        hash += hash >> 1;
            }

            hash ^= hash << 3;
            hash += hash >> 5;
            hash ^= hash << 4;
            hash += hash >> 17;
            hash ^= hash << 25;
            hash += hash >> 6;

            return hash;
    }


    ul32	CHash::GetCRC(lpcwstr _lpszString, u32 _uiStringLength)
    {
        ul32 hash = _uiStringLength, tmp;
        int rem;

            if (_uiStringLength <= 0 || _lpszString == NULL) return 0;

            rem = _uiStringLength & 3;
            _uiStringLength >>= 2;

            /* Main loop */
            for (;_uiStringLength > 0; _uiStringLength--) {
                hash  += xst_get16bits (_lpszString);
                tmp    = (xst_get16bits (_lpszString+2) << 11) ^ hash;
                hash   = (hash << 16) ^ tmp;
                _lpszString  += 2*sizeof (u16);
                hash  += hash >> 11;
            }

            /* Handle end cases */
            switch (rem) {
                case 3: hash += xst_get16bits (_lpszString);
                        hash ^= hash << 16;
                        hash ^= _lpszString[sizeof (u16)] << 18;
                        hash += hash >> 11;
                        break;
                case 2: hash += xst_get16bits (_lpszString);
                        hash ^= hash << 11;
                        hash += hash >> 17;
                        break;
                case 1: hash += *_lpszString;
                        hash ^= hash << 10;
                        hash += hash >> 1;
            }

            /* Force "avalanching" of final 127 bits */
            hash ^= hash << 3;
            hash += hash >> 5;
            hash ^= hash << 4;
            hash += hash >> 17;
            hash ^= hash << 25;
            hash += hash >> 6;

            return hash;
    }

}//xst
