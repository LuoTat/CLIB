/*
    这个ArrayStack采用懒加载的方式，只有在需要的时候才会分配内存，这样可以节省内存空间
*/
#pragma once
#include <limits.h>
#include <stdlib.h>
#include "Generic_tool.h"

// ----------------------------------------------------------------------------------------
// Benchmark                                              Time             CPU   Iterations
// ----------------------------------------------------------------------------------------
// ArrayStack_G_Push_Test/100000                      53271 ns        53263 ns        10000
// stack_Push_Test/100000                            147762 ns       147759 ns         4510
// ArrayStack_G_Pop_Test/100000/iterations:21474        176 ns          146 ns        21474
// stack_Pop_Test/100000/iterations:21474             39987 ns        39972 ns        21474

#define DEFAULT_ARRAYSTACK_CAPACITY  (16)
#define SOFT_MAX_ARRAYSTACK_CAPACITY (INT_MAX - 8)


#define ARRAYSTACK_TYPE(NAME, TYPE)                 \
    typedef struct ArrayStack_##NAME                \
    {                                               \
        /*Array of the elements*/                   \
        TYPE* Array;                                \
        /*The capacity of the Array*/               \
        int Capacity;                               \
        /*The number of the elements in the array*/ \
        int Size;                                   \
    } ArrayStack_##NAME;


#define ARRAYSTACK_PROTOTYPES(NAME, TYPE)                                                                 \
    extern void ArrayStack_##NAME##_Init(ArrayStack_##NAME* const ArrayStack);                            \
    extern CODE ArrayStack_##NAME##_Push(ArrayStack_##NAME* const ArrayStack, const TYPE Data);           \
    extern CODE ArrayStack_##NAME##_Pop(ArrayStack_##NAME* const ArrayStack, TYPE* const Result);         \
    extern CODE ArrayStack_##NAME##_Peek(ArrayStack_##NAME* const ArrayStack, TYPE* const Result);        \
    extern int  ArrayStack_##NAME##_GetSize(const ArrayStack_##NAME* const ArrayStack);                   \
    extern bool ArrayStack_##NAME##_IsEmpty(const ArrayStack_##NAME* const ArrayStack);                   \
    extern bool ArrayStack_##NAME##_Contains(const ArrayStack_##NAME* const ArrayStack, const TYPE Data); \
    extern void ArrayStack_##NAME##_Clear(ArrayStack_##NAME* const ArrayStack);                           \
    extern void ArrayStack_##NAME##_Destroy(ArrayStack_##NAME* const ArrayStack);


#define ARRAYSTACK_IMPL(NAME, TYPE, SCOPE, Equals_Function)                                                                         \
    SCOPE void ArrayStack_##NAME##_Init(ArrayStack_##NAME* const ArrayStack)                                                        \
    {                                                                                                                               \
        ArrayStack->Array    = NULL;                                                                                                \
        ArrayStack->Size     = 0;                                                                                                   \
        ArrayStack->Capacity = 0;                                                                                                   \
    }                                                                                                                               \
    SCOPE CODE ArrayStack_##NAME##_newCapacity(const int OldCapacity, const int MinGrowth, const int PrefGrowth, int* const Result) \
    {                                                                                                                               \
        int PrefCapacity = OldCapacity + ((MinGrowth > PrefGrowth) ? MinGrowth : PrefGrowth);                                       \
        if (PrefCapacity > 0 && PrefCapacity <= SOFT_MAX_ARRAYSTACK_CAPACITY)                                                       \
        {                                                                                                                           \
            *Result = PrefCapacity;                                                                                                 \
            return Success;                                                                                                         \
        }                                                                                                                           \
        else                                                                                                                        \
        {                                                                                                                           \
            int MinCapacity = OldCapacity + MinGrowth;                                                                              \
            if (MinCapacity < 0) return MemoryOverflow;                                                                             \
            else if (MinCapacity <= SOFT_MAX_ARRAYSTACK_CAPACITY)                                                                   \
            {                                                                                                                       \
                *Result = SOFT_MAX_ARRAYSTACK_CAPACITY;                                                                             \
                return Success;                                                                                                     \
            }                                                                                                                       \
            else                                                                                                                    \
            {                                                                                                                       \
                *Result = MinCapacity;                                                                                              \
                return Success;                                                                                                     \
            }                                                                                                                       \
        }                                                                                                                           \
    }                                                                                                                               \
    SCOPE CODE ArrayStack_##NAME##_Resize(ArrayStack_##NAME* const ArrayStack, const int MinCapacity)                               \
    {                                                                                                                               \
        int OldCapacity = ArrayStack->Capacity;                                                                                     \
        int NewCapacity;                                                                                                            \
        if (OldCapacity > 0 || ArrayStack->Array != NULL)                                                                           \
        {                                                                                                                           \
            if (Success == ArrayStack_##NAME##_newCapacity(OldCapacity, MinCapacity - OldCapacity, OldCapacity >> 1, &NewCapacity)) \
            {                                                                                                                       \
                TYPE* EA = (TYPE*)realloc(ArrayStack->Array, NewCapacity * sizeof(TYPE));                                           \
                if (unlikely(EA == NULL)) return MemoryAllocationError;                                                             \
                ArrayStack->Array    = EA;                                                                                          \
                ArrayStack->Capacity = NewCapacity;                                                                                 \
                return Success;                                                                                                     \
            }                                                                                                                       \
            else return MemoryOverflow;                                                                                             \
        }                                                                                                                           \
        else                                                                                                                        \
        {                                                                                                                           \
            NewCapacity = (MinCapacity > DEFAULT_ARRAYSTACK_CAPACITY) ? MinCapacity : DEFAULT_ARRAYSTACK_CAPACITY;                  \
            TYPE* EA    = (TYPE*)malloc(NewCapacity * sizeof(TYPE));                                                                \
            if (unlikely(EA == NULL)) return MemoryAllocationError;                                                                 \
            ArrayStack->Array    = EA;                                                                                              \
            ArrayStack->Capacity = NewCapacity;                                                                                     \
            return Success;                                                                                                         \
        }                                                                                                                           \
    }                                                                                                                               \
    SCOPE CODE ArrayStack_##NAME##_Push(ArrayStack_##NAME* const ArrayStack, const TYPE Data)                                       \
    {                                                                                                                               \
        if (unlikely(ArrayStack->Size == ArrayStack->Capacity))                                                                     \
        {                                                                                                                           \
            if (Success != ArrayStack_##NAME##_Resize(ArrayStack, ArrayStack->Size + 1)) return MemoryAllocationError;              \
        }                                                                                                                           \
        ArrayStack->Array[ArrayStack->Size++] = Data;                                                                               \
        return Success;                                                                                                             \
    }                                                                                                                               \
    SCOPE CODE ArrayStack_##NAME##_Pop(ArrayStack_##NAME* const ArrayStack, TYPE* const Result)                                     \
    {                                                                                                                               \
        if (unlikely(ArrayStack->Size == 0)) return ArrayIndexOutOfRange;                                                           \
        *Result = ArrayStack->Array[--ArrayStack->Size];                                                                            \
        return Success;                                                                                                             \
    }                                                                                                                               \
    SCOPE CODE ArrayStack_##NAME##_Peek(ArrayStack_##NAME* const ArrayStack, TYPE* const Result)                                    \
    {                                                                                                                               \
        if (unlikely(ArrayStack->Size == 0)) return ArrayIndexOutOfRange;                                                           \
        *Result = ArrayStack->Array[ArrayStack->Size - 1];                                                                          \
        return Success;                                                                                                             \
    }                                                                                                                               \
    SCOPE int  ArrayStack_##NAME##_GetSize(const ArrayStack_##NAME* const ArrayStack) { return ArrayStack->Size; }                  \
    SCOPE bool ArrayStack_##NAME##_IsEmpty(const ArrayStack_##NAME* const ArrayStack) { return ArrayStack->Size == 0; }             \
    SCOPE bool ArrayStack_##NAME##_Contains(const ArrayStack_##NAME* const ArrayStack, const TYPE Data)                             \
    {                                                                                                                               \
        for (int i = 0; i < ArrayStack->Size; ++i)                                                                                  \
        {                                                                                                                           \
            if (Equals_Function(Data, ArrayStack->Array[i])) return true;                                                           \
        }                                                                                                                           \
        return false;                                                                                                               \
    }                                                                                                                               \
    SCOPE void ArrayStack_##NAME##_Clear(ArrayStack_##NAME* const ArrayStack) { ArrayStack->Size = 0; }                             \
    SCOPE void ArrayStack_##NAME##_Destroy(ArrayStack_##NAME* const ArrayStack)                                                     \
    {                                                                                                                               \
        free(ArrayStack->Array);                                                                                                    \
        ArrayStack->Array    = NULL;                                                                                                \
        ArrayStack->Size     = 0;                                                                                                   \
        ArrayStack->Capacity = 0;                                                                                                   \
    }


#define ARRAYSTACK_DECLARE(NAME, TYPE) \
    ARRAYSTACK_TYPE(NAME, TYPE)        \
    ARRAYSTACK_PROTOTYPES(NAME, TYPE)


#define ARRAYSTACK_INIT(NAME, TYPE, SCOPE, Equals_Function) \
    ARRAYSTACK_TYPE(NAME, TYPE)                             \
    ARRAYSTACK_IMPL(NAME, TYPE, SCOPE, Equals_Function)

/**
 * @brief Get the ArrayStack_##NAME struct
 * @param NAME
 */
#define ArrayStack(NAME)                                 ArrayStack_##NAME

/**
 * @brief Initialize the ArrayStack_##NAME struct
 * @param NAME The name of the function
 * @param ArrayStack The ArrayStack_##NAME struct
 * @return CODE The result of the function
 */
#define ArrayStack_Init(NAME, ArrayStack)                ArrayStack_##NAME##_Init((ArrayStack))

/**
 * @brief Push the data to the ArrayStack_##NAME
 * @param NAME The name of the function
 * @param ArrayStack The ArrayStack_##NAME struct
 * @param Data The data to push
 * @return CODE The result of the function
 */
#define ArrayStack_Push(NAME, ArrayStack, Data)          ArrayStack_##NAME##_Push((ArrayStack), Data)

/**
 * @brief Pop the data from the ArrayStack_##NAME
 * @param NAME The name of the function
 * @param ArrayStack The ArrayStack_##NAME struct
 * @param Result To store the deleted element
 * @return CODE The result of the function
 */
#define ArrayStack_Pop(NAME, ArrayStack, Result)         ArrayStack_##NAME##_Pop((ArrayStack), (Result))

/**
 * @brief Peek the data from the ArrayStack_##NAME
 * @param NAME The name of the function
 * @param ArrayStack The ArrayStack_##NAME struct
 * @param Result To store the element
 * @return CODE The result of the function
 */
#define ArrayStack_Peek(NAME, ArrayStack, Result)        ArrayStack_##NAME##_Peek((ArrayStack), (Result))

/**
 * @brief Check if the ArrayStack_##NAME contains the data
 * @param NAME The name of the function
 * @param ArrayStack The ArrayStack_##NAME struct
 * @param Data The data to check
 * @retval true The ArrayStack_##NAME contains the data
 * @retval false The ArrayStack_##NAME does not contain the data
 */
#define ArrayStack_Contains(NAME, ArrayStack, Data)      ArrayStack_##NAME##_Contains((ArrayStack), Data)

/**
 * @brief Get the size of the ArrayStack_##NAME
 * @param NAME The name of the function
 * @param ArrayStack The ArrayStack_##NAME struct
 * @return int The size of the ArrayStack_##NAME
 */
#define ArrayStack_GetSize(ArrayStack)                   ((ArrayStack)->Size)

/**
 * @brief Check if the ArrayStack_##NAME is empty
 * @param NAME The name of the function
 * @param ArrayStack The ArrayStack_##NAME struct
 * @retval true The ArrayStack_##NAME is empty
 * @retval false The ArrayStack_##NAME is not empty
 */
#define ArrayStack_IsEmpty(ArrayStack)                   ((ArrayStack)->Size == 0)

/**
 * @brief Clear the ArrayStack_##NAME, it will not free the memory
 * @param NAME The name of the function
 * @param ArrayStack The ArrayStack_##NAME struct
 */
#define ArrayStack_Clear(ArrayStack)                     ((ArrayStack)->Size = 0)

/**
 * @brief Destroy the ArrayStack_##NAME, it will free the memory
 * @param NAME The name of the function
 * @param ArrayStack The ArrayStack_##NAME struct
 */
#define ArrayStack_Destroy(NAME, ArrayStack)             ArrayStack_##NAME##_Destroy((ArrayStack))

// Function implementation
#define LTT_ARRAYSTACK_INIT(NAME, TYPE, Equals_Function) ARRAYSTACK_INIT(NAME, TYPE, static LTT_inline LTT_unused, Equals_Function)

// Function declaration
#define LTT_ARRAYSTACK_DECLARE(NAME, TYPE)               ARRAYSTACK_DECLARE(NAME, TYPE)