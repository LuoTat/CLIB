#pragma once
#include <stdbool.h>
#include <string.h>

#define likely(x)                   __builtin_expect(!!(x), 1)
#define unlikely(x)                 __builtin_expect(!!(x), 0)

/*
	EQUALS_AND_GETHASHCODE_MODE为1时，使用指针地址作为哈希值，使用指针地址作为比较值
	EQUALS_AND_GETHASHCODE_MODE为2时，使用memcmp作为比较值，使用哈希算法作为哈希值
*/
#define EQUALS_AND_GETHASHCODE_MODE 2


#define OK                          1
#define ERROR                       0
#define INFEASIBLE                  -1
#define OVERFLOW                    -2


typedef int Status;
typedef Status (*VisitFunction)(void* const);
typedef int (*ValueFunction)(const void* const);
typedef int (*CompareFunction)(const void* const, const void* const);
typedef unsigned int (*GetHashCode_Function)(const void* const, const size_t);
typedef bool (*Equals_Function)(const void* const, const void* const, const size_t);

inline static bool LTT_DEFAULT_Compare(const void* const a, const void* const b, const size_t DataSize) { return memcmp(a, b, DataSize); }

inline static bool LTT_DEFAULT_Equals(const void* const a, const void* const b, const size_t DataSize)
{
#if EQUALS_AND_GETHASHCODE_MODE == 1
    return a == b;
#elif EQUALS_AND_GETHASHCODE_MODE == 2
    return memcmp(a, b, DataSize) == 0;
#endif
}

inline static unsigned int LTT_DEFAULT_GetHashCode(const void* const Data, const size_t DataSize)
{
#if EQUALS_AND_GETHASHCODE_MODE == 1
    return (unsigned long int)Data;
#elif EQUALS_AND_GETHASHCODE_MODE == 2
    unsigned int seed = 131;    // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;
    char*        data = (char*)Data;
    for (size_t i = 0; i < DataSize; ++i) { hash = hash * seed + data[i]; }
    return (hash & 0x7FFFFFFF);
#endif
}
