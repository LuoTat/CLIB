/*
    这个ArrayStack采用懒加载的方式，只有在需要的时候才会分配内存，这样可以节省内存空间
*/

#pragma once
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Generic_tool.h"

#define DEFAULT_ARRAYSTACK_CAPACITY  (16)

#define SOFT_MAX_ARRAYSTACK_CAPACITY (INT_MAX - 8)


#define _ARRRAYSTACK_TYPE(NAME, TYPE) \
    typedef struct ArrayStack_##NAME  \
    {                                 \
        TYPE* Array;                  \
        int   Size;                   \
        int   Capacity;               \
    } ArrayStack_##NAME;

#define _ARRRAYSTACK_PROTOTYPES(NAME, TYPE)                                                                             \
    extern ArrayStack_##NAME* ArrayStack_##NAME##_New();                                                                \
    extern int                ArrayStack_##NAME##_Push(ArrayStack_##NAME* const ArrayStack, const TYPE Data);           \
    extern TYPE               ArrayStack_##NAME##_Pop(ArrayStack_##NAME* const ArrayStack);                             \
    extern TYPE               ArrayStack_##NAME##_Peek(ArrayStack_##NAME* const ArrayStack);                            \
    extern bool               ArrayStack_##NAME##_Contains(const ArrayStack_##NAME* const ArrayStack, const TYPE Data); \
    extern void               ArrayStack_##NAME##_Clear(ArrayStack_##NAME* const ArrayStack);                           \
    extern void               ArrayStack_##NAME##_Destroy(ArrayStack_##NAME** ArrayStack);

#define _ARRAYSTACK_IMPL(NAME, TYPE, SCOPE, Equals_Function)                                                                     \
    SCOPE ArrayStack_##NAME* ArrayStack_##NAME##_New()                                                                           \
    {                                                                                                                            \
        ArrayStack_##NAME* ArrayStackP = (ArrayStack_##NAME*)malloc(sizeof(ArrayStack_##NAME));                                  \
        if (ArrayStackP == NULL) return NULL;                                                                                    \
        ArrayStackP->Array    = NULL;                                                                                            \
        ArrayStackP->Size     = 0;                                                                                               \
        ArrayStackP->Capacity = 0;                                                                                               \
        return ArrayStackP;                                                                                                      \
    }                                                                                                                            \
    SCOPE int ArrayStack_##NAME##_newCapacity(const int OldCapacity, const int MinGrowth, const int PrefGrowth)                  \
    {                                                                                                                            \
        int PrefCapacity = OldCapacity + ((MinGrowth > PrefGrowth) ? MinGrowth : PrefGrowth);                                    \
        if (PrefCapacity > 0 && PrefCapacity <= SOFT_MAX_ARRAYSTACK_CAPACITY) return PrefCapacity;                               \
        else                                                                                                                     \
        {                                                                                                                        \
            int MinCapacity = OldCapacity + MinGrowth;                                                                           \
            if (MinCapacity < 0) return 0;                                                                                       \
            else if (MinCapacity <= SOFT_MAX_ARRAYSTACK_CAPACITY) return SOFT_MAX_ARRAYSTACK_CAPACITY;                           \
            else return MinCapacity;                                                                                             \
        }                                                                                                                        \
    }                                                                                                                            \
    SCOPE int ArrayStack_##NAME##_Resize(ArrayStack_##NAME* const ArrayStack, const int MinCapacity)                             \
    {                                                                                                                            \
        int OldCapacity = ArrayStack->Capacity;                                                                                  \
        int NewCapacity;                                                                                                         \
        if (OldCapacity > 0 || ArrayStack->Array != NULL)                                                                        \
        {                                                                                                                        \
            NewCapacity = ArrayStack_##NAME##_newCapacity(OldCapacity, MinCapacity - OldCapacity, OldCapacity >> 1);             \
            if (NewCapacity == 0) return 0;                                                                                      \
            TYPE* EA = (TYPE*)realloc(ArrayStack->Array, NewCapacity * sizeof(TYPE));                                            \
            if (EA == NULL) return 0;                                                                                            \
            ArrayStack->Array    = EA;                                                                                           \
            ArrayStack->Capacity = NewCapacity;                                                                                  \
            return 1;                                                                                                            \
        }                                                                                                                        \
        else                                                                                                                     \
        {                                                                                                                        \
            NewCapacity = (MinCapacity > DEFAULT_ARRAYSTACK_CAPACITY) ? MinCapacity : DEFAULT_ARRAYSTACK_CAPACITY;               \
            TYPE* EA    = (TYPE*)malloc(NewCapacity * sizeof(TYPE));                                                             \
            if (EA == NULL) return 0;                                                                                            \
            ArrayStack->Array    = EA;                                                                                           \
            ArrayStack->Capacity = NewCapacity;                                                                                  \
            return 1;                                                                                                            \
        }                                                                                                                        \
    }                                                                                                                            \
    SCOPE int ArrayStack_##NAME##_Push(ArrayStack_##NAME* const ArrayStack, const TYPE Data)                                     \
    {                                                                                                                            \
        if (unlikely(ArrayStack->Size == ArrayStack->Capacity))                                                                  \
        {                                                                                                                        \
            if (ArrayStack_##NAME##_Resize(ArrayStack, ArrayStack->Size + 1) == 0) return 0;                                     \
        }                                                                                                                        \
        ArrayStack->Array[ArrayStack->Size++] = Data;                                                                            \
        return 1;                                                                                                                \
    }                                                                                                                            \
    SCOPE TYPE ArrayStack_##NAME##_Pop(ArrayStack_##NAME* const ArrayStack) { return ArrayStack->Array[--ArrayStack->Size]; }    \
    SCOPE TYPE ArrayStack_##NAME##_Peek(ArrayStack_##NAME* const ArrayStack) { return ArrayStack->Array[ArrayStack->Size - 1]; } \
    SCOPE bool ArrayStack_##NAME##_Contains(const ArrayStack_##NAME* const ArrayStack, const TYPE Data)                          \
    {                                                                                                                            \
        TYPE* EA = ArrayStack->Array;                                                                                            \
        for (int i = 0; i < ArrayStack->Size; ++i)                                                                               \
        {                                                                                                                        \
            if (Equals_Function(Data, EA[i])) return true;                                                                       \
        }                                                                                                                        \
        return false;                                                                                                            \
    }                                                                                                                            \
    SCOPE void ArrayStack_##NAME##_Clear(ArrayStack_##NAME* const ArrayStack) { ArrayStack->Size = 0; }                          \
    SCOPE void ArrayStack_##NAME##_Destroy(ArrayStack_##NAME** ArrayStack)                                                       \
    {                                                                                                                            \
        free((*ArrayStack)->Array);                                                                                              \
        free(*ArrayStack);                                                                                                       \
        *ArrayStack = NULL;                                                                                                      \
    }

#define _ARRAYSTACK_DECLARE(NAME, TYPE) \
    _ARRRAYSTACK_TYPE(NAME, TYPE)       \
    _ARRRAYSTACK_PROTOTYPES(NAME, TYPE)

#define _ARRAYSTACK_INIT(NAME, TYPE, SCOPE, Equals_Function) \
    _ARRRAYSTACK_TYPE(NAME, TYPE)                            \
    _ARRAYSTACK_IMPL(NAME, TYPE, SCOPE, Equals_Function)


#define ArrayStack(NAME)                                 ArrayStack_##NAME
#define ArrayStack_New(NAME)                             ArrayStack_##NAME##_New()
#define ArrayStack_Push(NAME, ArrayStack, Data)          ArrayStack_##NAME##_Push(ArrayStack, Data)
#define ArrayStack_Pop(ArrayStack)                       (ArrayStack->Array[--ArrayStack->Size])
#define ArrayStack_Peek(ArrayStack)                      (ArrayStack->Array[ArrayStack->Size - 1])
#define ArrayStack_Contains(NAME, ArrayStack, Data)      ArrayStack_##NAME##_Contains(ArrayStack, Data)
#define ArrayStack_IsEmpty(ArrayStack)                   (ArrayStack->Size == 0)
#define ArrayStack_GetSize(ArrayStack)                   (ArrayStack->Size)
#define ArrayStack_Clear(ArrayStack)                     (ArrayStack->Size = 0)
#define ArrayStack_Destroy(NAME, ArrayStack)             ArrayStack_##NAME##_Destroy(&ArrayStack)

#define LTT_ARRAYSTACK_INIT(NAME, TYPE, Equals_Function) _ARRAYSTACK_INIT(NAME, TYPE, static LTT_inline LTT_unused, Equals_Function)
#define LTT_ARRAYSTACK_DECLARE(NAME, TYPE)               _ARRAYSTACK_DECLARE(NAME, TYPE)