/*
    这个ArrayStack采用懒加载的方式，只有在需要的时候才会分配内存，这样可以节省内存空间
*/
#pragma once
#include <limits.h>
#include <stdlib.h>
#include "Generic_tool.h"


#define DEFAULT_ARRAYSTACK_CAPACITY  (16)
#define SOFT_MAX_ARRAYSTACK_CAPACITY (INT_MAX - 8)


#define _ARRAYSTACK_TYPE(NAME, TYPE) \
    typedef struct ArrayStack_##NAME \
    {                                \
        TYPE* Array;                 \
        int   Capacity;              \
        int   Size;                  \
    } ArrayStack_##NAME;


#define _ARRAYSTACK_PROTOTYPES(NAME, TYPE)                                                                \
    extern void ArrayStack_##NAME##_Init(ArrayStack_##NAME* const ArrayStack);                            \
    extern CODE ArrayStack_##NAME##_Push(ArrayStack_##NAME* const ArrayStack, const TYPE Data);           \
    extern CODE ArrayStack_##NAME##_Pop(ArrayStack_##NAME* const ArrayStack, TYPE* const Result);         \
    extern CODE ArrayStack_##NAME##_Peek(ArrayStack_##NAME* const ArrayStack, TYPE* const Result);        \
    extern int  ArrayStack_##NAME##_GetSize(const ArrayStack_##NAME* const ArrayStack);                   \
    extern bool ArrayStack_##NAME##_IsEmpty(const ArrayStack_##NAME* const ArrayStack);                   \
    extern bool ArrayStack_##NAME##_Contains(const ArrayStack_##NAME* const ArrayStack, const TYPE Data); \
    extern void ArrayStack_##NAME##_Clear(ArrayStack_##NAME* const ArrayStack);                           \
    extern void ArrayStack_##NAME##_Destroy(ArrayStack_##NAME* const ArrayStack);


#define _ARRAYSTACK_IMPL(NAME, TYPE, SCOPE, Equals_Function)                                                                        \
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
                if (EA == NULL) return MemoryAllocationError;                                                                       \
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
            if (EA == NULL) return MemoryAllocationError;                                                                           \
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


#define _ARRAYSTACK_DECLARE(NAME, TYPE) \
    _ARRAYSTACK_TYPE(NAME, TYPE)        \
    _ARRAYSTACK_PROTOTYPES(NAME, TYPE)


#define _ARRAYSTACK_INIT(NAME, TYPE, SCOPE, Equals_Function) \
    _ARRAYSTACK_TYPE(NAME, TYPE)                             \
    _ARRAYSTACK_IMPL(NAME, TYPE, SCOPE, Equals_Function)


#define ArrayStack(NAME)                                 ArrayStack_##NAME
#define ArrayStack_Init(NAME, ArrayStack)                ArrayStack_##NAME##_Init((ArrayStack))
#define ArrayStack_Push(NAME, ArrayStack, Data)          ArrayStack_##NAME##_Push((ArrayStack), Data)
#define ArrayStack_Pop(NAME, ArrayStack, Result)         ArrayStack_##NAME##_Pop((ArrayStack), (Result))
#define ArrayStack_Peek(NAME, ArrayStack, Result)        ArrayStack_##NAME##_Peek((ArrayStack), (Result))
#define ArrayStack_Contains(NAME, ArrayStack, Data)      ArrayStack_##NAME##_Contains((ArrayStack), Data)
#define ArrayStack_GetSize(ArrayStack)                   ((ArrayStack)->Size)
#define ArrayStack_IsEmpty(ArrayStack)                   ((ArrayStack)->Size == 0)
#define ArrayStack_Clear(ArrayStack)                     ((ArrayStack)->Size = 0)
#define ArrayStack_Destroy(NAME, ArrayStack)             ArrayStack_##NAME##_Destroy((ArrayStack))

// 函数实现
#define LTT_ARRAYSTACK_INIT(NAME, TYPE, Equals_Function) _ARRAYSTACK_INIT(NAME, TYPE, static LTT_inline LTT_unused, Equals_Function)

// 函数声明
#define LTT_ARRAYSTACK_DECLARE(NAME, TYPE)               _ARRAYSTACK_DECLARE(NAME, TYPE)