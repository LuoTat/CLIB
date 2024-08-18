#pragma once
/*
 **************************************************************************
 *                                                                        *
 *          General Purpose Hash Function Algorithms Library              *
 *                                                                        *
 * Author: Arash Partow - 2002                                            *
 * URL: http://www.partow.net                                             *
 * URL: http://www.partow.net/programming/hashfunctions/index.html        *
 *                                                                        *
 * Copyright notice:                                                      *
 * Free use of the General Purpose Hash Function Algorithms Library is    *
 * permitted under the guidelines and in accordance with the MIT License. *
 * http://www.opensource.org/licenses/MIT                                 *
 *                                                                        *
 **************************************************************************
*/

#include "Generic_tool.h"
#include <stddef.h>

static LTT_inline LTT_unused unsigned int RSHash(const char* str, size_t len)
{
    unsigned int b    = 378551;
    unsigned int a    = 63689;
    unsigned int hash = 0;

    for (size_t i = 0; i < len; ++str, ++i)
    {
        hash = hash * a + (*str);
        a    = a * b;
    }

    return hash;
}

/* End Of RS Hash Function */


static LTT_inline LTT_unused unsigned int JSHash(const char* str, size_t len)
{
    unsigned int hash = 1315423911;

    for (size_t i = 0; i < len; str++, i++) { hash ^= ((hash << 5) + (*str) + (hash >> 2)); }

    return hash;
}

/* End Of JS Hash Function */


static LTT_inline LTT_unused unsigned int PJWHash(const char* str, size_t len)
{
    const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
    const unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt * 3) / 4);
    const unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
    const unsigned int HighBits          = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
    unsigned int       hash              = 0;
    unsigned int       test              = 0;

    for (size_t i = 0; i < len; ++str, ++i)
    {
        hash = (hash << OneEighth) + (*str);

        if ((test = hash & HighBits) != 0) { hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits)); }
    }

    return hash;
}

/* End Of  P. J. Weinberger Hash Function */


static LTT_inline LTT_unused unsigned int ELFHash(const char* str, size_t len)
{
    unsigned int hash = 0;
    unsigned int x    = 0;

    for (size_t i = 0; i < len; ++str, ++i)
    {
        hash = (hash << 4) + (*str);
        if ((x = hash & 0xF0000000L) != 0) { hash ^= (x >> 24); }
        hash &= ~x;
    }

    return hash;
}

/* End Of ELF Hash Function */


static LTT_inline LTT_unused unsigned int BKDRHash(const char* str, size_t len)
{
    unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
    unsigned int hash = 0;

    for (size_t i = 0; i < len; ++str, ++i) { hash = (hash * seed) + (*str); }

    return hash;
}

/* End Of BKDR Hash Function */


static LTT_inline LTT_unused unsigned int SDBMHash(const char* str, size_t len)
{
    unsigned int hash = 0;

    for (size_t i = 0; i < len; ++str, ++i) { hash = (*str) + (hash << 6) + (hash << 16) - hash; }

    return hash;
}

/* End Of SDBM Hash Function */


static LTT_inline LTT_unused unsigned int DJBHash(const char* str, size_t len)
{
    unsigned int hash = 5381;

    for (size_t i = 0; i < len; ++str, ++i) { hash = ((hash << 5) + hash) + (*str); }

    return hash;
}

/* End Of DJB Hash Function */


static LTT_inline LTT_unused unsigned int DEKHash(const char* str, size_t len)
{
    unsigned int hash = len;

    for (size_t i = 0; i < len; ++str, ++i) { hash = ((hash << 5) ^ (hash >> 27)) ^ (*str); }
    return hash;
}

/* End Of DEK Hash Function */


static LTT_inline LTT_unused unsigned int BPHash(const char* str, size_t len)
{
    unsigned int hash = 0;

    for (size_t i = 0; i < len; ++str, ++i) { hash = hash << 7 ^ (*str); }

    return hash;
}

/* End Of BP Hash Function */


static LTT_inline LTT_unused unsigned int FNV_1aHash(const char* str, size_t len)
{
    const unsigned int FNV32_PRIME = 0x1000193;
    unsigned int       hash        = 0x811C9DC5;

    for (size_t i = 0; i < len; ++str, ++i)
    {
        hash ^= (*str);
        hash *= FNV32_PRIME;
    }

    return hash;
}

/* End Of FNV_1a Hash Function */


static LTT_inline LTT_unused unsigned int APHash(const char* str, size_t len)
{
    unsigned int hash = 0xAAAAAAAA;

    for (size_t i = 0; i < len; ++str, ++i) { hash ^= ((i & 1) == 0) ? ((hash << 7) ^ (*str) * (hash >> 3)) : (~((hash << 11) + ((*str) ^ (hash >> 5)))); }

    return hash;
}

/* End Of AP Hash Function */
