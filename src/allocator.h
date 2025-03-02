#pragma once
#include "precomp.h"
#include <stdlib.h>

inline static LTT_UNUSED void* LTT_alloc_allocate(size_t n)
{
    return malloc(n);
}

inline static LTT_UNUSED void LTT_alloc_deallocate(void* p, LTT_UNUSED size_t n)
{
    free(p);
}

#define ALLOCATOR_INIT(NAME, TYPE, Alloc, SCOPE)                \
    SCOPE TYPE* Allocator_##NAME##_allocate(size_t n)           \
    {                                                           \
        return (TYPE*)Alloc##_allocate(n * sizeof(TYPE));       \
    }                                                           \
    SCOPE void Allocator_##NAME##_deallocate(TYPE* p, size_t n) \
    {                                                           \
        Alloc##_deallocate(p, n);                               \
    }

/**
 * @brief malloc Num * sizeof(TYPE) and return the pointer
 * @param NAME The name of the function
 * @param Num The number of elements
 * @return TYPE* The pointer to the allocated memory
 */
#define Allocator_allocate(NAME, Num)             Allocator_##NAME##_allocate((Num))

/**
 * @brief free the memory
 * @param NAME The name of the function
 * @param Pointer The pointer to the memory
 * @param Size The size of the memory
 */
#define Allocator_deallocate(NAME, Pointer, Size) Allocator_##NAME##_deallocate((Pointer), (Size))

// Function implementation
#define LTT_ALLOCATOR_INIT(NAME, TYPE, Alloc)     ALLOCATOR_INIT(NAME, TYPE, Alloc, inline static LTT_UNUSED)