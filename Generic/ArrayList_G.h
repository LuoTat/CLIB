#pragma once
#include <limits.h>
#include <stdlib.h>
#include "Generic_tool.h"


#define DEFAULT_ARRAYLIST_CAPACITY  (16)

#define SOFT_MAX_ARRAYLIST_CAPACITY (INT_MAX - 8)


#define _ARRAYLIST_TYPE(NAME, TYPE) \
    typedef struct ArrayList_##NAME \
    {                               \
        TYPE* Array;                \
        int   Capacity;             \
        int   Size;                 \
    } ArrayList_##NAME;

#define _ARRAYLIST_PROTOTYPES(NAME, TYPE)                                                                                       \
    extern void ArrayList_##NAME##_Init(ArrayList_##NAME* const ArrayList);                                                     \
    extern CODE ArrayList_##NAME##_AddFirst(ArrayList_##NAME* const ArrayList, const TYPE Data);                                \
    extern CODE ArrayList_##NAME##_AddLast(ArrayList_##NAME* const ArrayList, const TYPE Data);                                 \
    extern CODE ArrayList_##NAME##_AddIndex_Safe(ArrayList_##NAME* const ArrayList, const TYPE Data, const int Index);          \
    extern CODE ArrayList_##NAME##_DeleteFirst(ArrayList_##NAME* const ArrayList, TYPE* const Result);                          \
    extern CODE ArrayList_##NAME##_DeleteLast(ArrayList_##NAME* const ArrayList, TYPE* const Result);                           \
    extern CODE ArrayList_##NAME##_DeleteIndex(ArrayList_##NAME* const ArrayList, const int Index, TYPE* const Result);         \
    extern CODE ArrayList_##NAME##_SetIndex(ArrayList_##NAME* const ArrayList, TYPE Data, const int Index, TYPE* const Result); \
    extern CODE ArrayList_##NAME##_GetFirst(ArrayList_##NAME* const ArrayList, TYPE* const Result);                             \
    extern void ArrayList_##NAME##_GetLast(ArrayList_##NAME* const ArrayList, TYPE* const Result);                              \
    extern CODE ArrayList_##NAME##_GetIndex(ArrayList_##NAME* const ArrayList, const int Index, TYPE* const Result);            \
    extern int  ArrayList_##NAME##_IndexOf(const ArrayList_##NAME* const ArrayList, TYPE Data);                                 \
    extern int  ArrayList_##NAME##_LastIndexOf(const ArrayList_##NAME* const ArrayList, TYPE Data);                             \
    extern int  ArrayList_##NAME##_GetSize(const ArrayList_##NAME* const ArrayList);                                            \
    extern bool ArrayList_##NAME##_isEmpty(const ArrayList_##NAME* const ArrayList);                                            \
    extern bool ArrayList_##NAME##_Contains(const ArrayList_##NAME* const ArrayList, const TYPE Data);                          \
    extern void ArrayList_##NAME##_Clear(ArrayList_##NAME* const ArrayList);                                                    \
    extern void ArrayList_##NAME##_Destroy(ArrayList_##NAME* const ArrayList);


#define _ARRAYLIST_IMPL(NAME, TYPE, SCOPE, Equals_Function)                                                                                                       \
    SCOPE bool ArrayList_##NAME##_CheckIndex(const ArrayList_##NAME* const ArrayList, const int Index)                                                            \
    {                                                                                                                                                             \
        if (Index < 0 || Index > ArrayList->Size) return false;                                                                                                   \
        else return true;                                                                                                                                         \
    }                                                                                                                                                             \
    SCOPE void ArrayList_##NAME##_Init(ArrayList_##NAME* const ArrayList)                                                                                         \
    {                                                                                                                                                             \
        ArrayList->Array    = NULL;                                                                                                                               \
        ArrayList->Size     = 0;                                                                                                                                  \
        ArrayList->Capacity = 0;                                                                                                                                  \
    }                                                                                                                                                             \
    SCOPE CODE ArrayList_##NAME##_newCapacity(const int OldCapacity, const int MinGrowth, const int PrefGrowth, int* const Result)                                \
    {                                                                                                                                                             \
        int PrefCapacity = OldCapacity + ((MinGrowth > PrefGrowth) ? MinGrowth : PrefGrowth);                                                                     \
        if (PrefCapacity > 0 && PrefCapacity <= SOFT_MAX_ARRAYLIST_CAPACITY)                                                                                      \
        {                                                                                                                                                         \
            *Result = PrefCapacity;                                                                                                                               \
            return Success;                                                                                                                                       \
        }                                                                                                                                                         \
        else                                                                                                                                                      \
        {                                                                                                                                                         \
            int MinCapacity = OldCapacity + MinGrowth;                                                                                                            \
            if (MinCapacity < 0) return MemoryOverflow;                                                                                                           \
            else if (MinCapacity <= SOFT_MAX_ARRAYLIST_CAPACITY)                                                                                                  \
            {                                                                                                                                                     \
                *Result = SOFT_MAX_ARRAYLIST_CAPACITY;                                                                                                            \
                return Success;                                                                                                                                   \
            }                                                                                                                                                     \
            else                                                                                                                                                  \
            {                                                                                                                                                     \
                *Result = MinCapacity;                                                                                                                            \
                return Success;                                                                                                                                   \
            }                                                                                                                                                     \
        }                                                                                                                                                         \
    }                                                                                                                                                             \
    SCOPE CODE ArrayList_##NAME##_Resize(ArrayList_##NAME* const ArrayList, const int MinCapacity)                                                                \
    {                                                                                                                                                             \
        int OldCapacity = ArrayList->Capacity;                                                                                                                    \
        int NewCapacity;                                                                                                                                          \
        if (OldCapacity > 0 || ArrayList->Array != NULL)                                                                                                          \
        {                                                                                                                                                         \
            if (Success == ArrayList_##NAME##_newCapacity(OldCapacity, MinCapacity - OldCapacity, OldCapacity >> 1, &NewCapacity))                                \
            {                                                                                                                                                     \
                TYPE* EA = (TYPE*)realloc(ArrayList->Array, NewCapacity * sizeof(TYPE));                                                                          \
                if (EA == NULL) return MemoryAllocationError;                                                                                                     \
                ArrayList->Array    = EA;                                                                                                                         \
                ArrayList->Capacity = NewCapacity;                                                                                                                \
                return Success;                                                                                                                                   \
            }                                                                                                                                                     \
            else return MemoryOverflow;                                                                                                                           \
        }                                                                                                                                                         \
        else                                                                                                                                                      \
        {                                                                                                                                                         \
            NewCapacity = (MinCapacity > DEFAULT_ARRAYLIST_CAPACITY) ? MinCapacity : DEFAULT_ARRAYLIST_CAPACITY;                                                  \
            TYPE* EA    = (TYPE*)malloc(NewCapacity * sizeof(TYPE));                                                                                              \
            if (EA == NULL) return MemoryAllocationError;                                                                                                         \
            ArrayList->Array    = EA;                                                                                                                             \
            ArrayList->Capacity = NewCapacity;                                                                                                                    \
            return Success;                                                                                                                                       \
        }                                                                                                                                                         \
    }                                                                                                                                                             \
    SCOPE CODE ArrayList_##NAME##_AddIndex_UnSafe(ArrayList_##NAME* const ArrayList, const TYPE Data, const int Index)                                            \
    {                                                                                                                                                             \
        if (unlikely(ArrayList->Size == ArrayList->Capacity))                                                                                                     \
        {                                                                                                                                                         \
            if (Success != ArrayList_##NAME##_Resize(ArrayList, ArrayList->Size + 1)) return MemoryAllocationError;                                               \
        }                                                                                                                                                         \
        memmove(ArrayList->Array + Index + 1, ArrayList->Array + Index, (ArrayList->Size - Index) * sizeof(TYPE));                                                \
        ArrayList->Array[Index] = Data;                                                                                                                           \
        ++ArrayList->Size;                                                                                                                                        \
        return Success;                                                                                                                                           \
    }                                                                                                                                                             \
    SCOPE CODE ArrayList_##NAME##_AddFirst(ArrayList_##NAME* const ArrayList, const TYPE Data) { return ArrayList_##NAME##_AddIndex_UnSafe(ArrayList, Data, 0); } \
    SCOPE CODE ArrayList_##NAME##_AddLast(ArrayList_##NAME* const ArrayList, const TYPE Data)                                                                     \
    {                                                                                                                                                             \
        if (unlikely(ArrayList->Size == ArrayList->Capacity))                                                                                                     \
        {                                                                                                                                                         \
            if (Success != ArrayList_##NAME##_Resize(ArrayList, ArrayList->Size + 1)) return MemoryAllocationError;                                               \
        }                                                                                                                                                         \
        ArrayList->Array[ArrayList->Size++] = Data;                                                                                                               \
        return Success;                                                                                                                                           \
    }                                                                                                                                                             \
    SCOPE CODE ArrayList_##NAME##_AddIndex_Safe(ArrayList_##NAME* const ArrayList, const TYPE Data, const int Index)                                              \
    {                                                                                                                                                             \
        if (!ArrayList_##NAME##_CheckIndex(ArrayList, Index)) return ArrayIndexOutOfRange;                                                                        \
        return ArrayList_##NAME##_AddIndex_UnSafe(ArrayList, Data, Index);                                                                                        \
    }                                                                                                                                                             \
    SCOPE void ArrayList_##NAME##_FastDeleteIndex(ArrayList_##NAME* const ArrayList, const int Index)                                                             \
    {                                                                                                                                                             \
        int NewSize = ArrayList->Size - 1;                                                                                                                        \
        if (NewSize > Index) memmove(ArrayList->Array + Index, ArrayList->Array + Index + 1, (NewSize - Index) * sizeof(TYPE));                                   \
        ArrayList->Size = NewSize;                                                                                                                                \
    }                                                                                                                                                             \
    SCOPE CODE ArrayList_##NAME##_DeleteFirst(ArrayList_##NAME* const ArrayList, TYPE* const Result)                                                              \
    {                                                                                                                                                             \
        if (unlikely(ArrayList->Size == 0)) return ArrayIndexOutOfRange;                                                                                          \
        *Result = ArrayList->Array[0];                                                                                                                            \
        ArrayList_##NAME##_FastDeleteIndex(ArrayList, 0);                                                                                                         \
        return Success;                                                                                                                                           \
    }                                                                                                                                                             \
    SCOPE CODE ArrayList_##NAME##_DeleteLast(ArrayList_##NAME* const ArrayList, TYPE* const Result)                                                               \
    {                                                                                                                                                             \
        if (unlikely(ArrayList->Size == 0)) return ArrayIndexOutOfRange;                                                                                          \
        *Result = ArrayList->Array[ArrayList->Size - 1];                                                                                                          \
        ArrayList_##NAME##_FastDeleteIndex(ArrayList, ArrayList->Size - 1);                                                                                       \
        return Success;                                                                                                                                           \
    }                                                                                                                                                             \
    SCOPE CODE ArrayList_##NAME##_DeleteIndex(ArrayList_##NAME* const ArrayList, const int Index, TYPE* const Result)                                             \
    {                                                                                                                                                             \
        if (!ArrayList_##NAME##_CheckIndex(ArrayList, Index)) return ArrayIndexOutOfRange;                                                                        \
        *Result = ArrayList->Array[Index];                                                                                                                        \
        ArrayList_##NAME##_FastDeleteIndex(ArrayList, Index);                                                                                                     \
        return Success;                                                                                                                                           \
    }                                                                                                                                                             \
    SCOPE CODE ArrayList_##NAME##_SetIndex(ArrayList_##NAME* const ArrayList, TYPE Data, const int Index, TYPE* const Result)                                     \
    {                                                                                                                                                             \
        if (!ArrayList_##NAME##_CheckIndex(ArrayList, Index)) return ArrayIndexOutOfRange;                                                                        \
        *Result                 = ArrayList->Array[Index];                                                                                                        \
        ArrayList->Array[Index] = Data;                                                                                                                           \
        return Success;                                                                                                                                           \
    }                                                                                                                                                             \
    SCOPE CODE ArrayList_##NAME##_GetFirst(ArrayList_##NAME* const ArrayList, TYPE* const Result)                                                                 \
    {                                                                                                                                                             \
        if (unlikely(ArrayList->Size == 0)) return ArrayIndexOutOfRange;                                                                                          \
        *Result = ArrayList->Array[0];                                                                                                                            \
        return Success;                                                                                                                                           \
    }                                                                                                                                                             \
    SCOPE CODE ArrayList_##NAME##_GetLast(ArrayList_##NAME* const ArrayList, TYPE* const Result)                                                                  \
    {                                                                                                                                                             \
        if (unlikely(ArrayList->Size == 0)) return ArrayIndexOutOfRange;                                                                                          \
        *Result = ArrayList->Array[ArrayList->Size - 1];                                                                                                          \
        return Success;                                                                                                                                           \
    }                                                                                                                                                             \
    SCOPE CODE ArrayList_##NAME##_GetIndex(ArrayList_##NAME* const ArrayList, const int Index, TYPE* const Result)                                                \
    {                                                                                                                                                             \
        if (!ArrayList_##NAME##_CheckIndex(ArrayList, Index)) return ArrayIndexOutOfRange;                                                                        \
        *Result = ArrayList->Array[Index];                                                                                                                        \
        return Success;                                                                                                                                           \
    }                                                                                                                                                             \
    SCOPE int ArrayList_##NAME##_IndexOf(const ArrayList_##NAME* const ArrayList, TYPE Data)                                                                      \
    {                                                                                                                                                             \
        for (int i = 0; i < ArrayList->Size; ++i)                                                                                                                 \
        {                                                                                                                                                         \
            if (Equals_Function(Data, ArrayList->Array[i])) return i;                                                                                             \
        }                                                                                                                                                         \
        return -1;                                                                                                                                                \
    }                                                                                                                                                             \
    SCOPE int ArrayList_##NAME##_LastIndexOf(const ArrayList_##NAME* const ArrayList, TYPE Data)                                                                  \
    {                                                                                                                                                             \
        for (int i = ArrayList->Size - 1; i >= 0; --i)                                                                                                            \
        {                                                                                                                                                         \
            if (Equals_Function(Data, ArrayList->Array[i])) return i;                                                                                             \
        }                                                                                                                                                         \
        return -1;                                                                                                                                                \
    }                                                                                                                                                             \
    SCOPE int  ArrayList_##NAME##_GetSize(const ArrayList_##NAME* const ArrayList) { return ArrayList->Size; }                                                    \
    SCOPE bool ArrayList_##NAME##_isEmpty(const ArrayList_##NAME* const ArrayList) { return ArrayList->Size == 0; }                                               \
    SCOPE bool ArrayList_##NAME##_Contains(const ArrayList_##NAME* const ArrayList, const TYPE Data) { return ArrayList_##NAME##_IndexOf(ArrayList, Data) >= 0; } \
    SCOPE void ArrayList_##NAME##_Clear(ArrayList_##NAME* const ArrayList) { ArrayList->Size = 0; }                                                               \
    SCOPE void ArrayList_##NAME##_Destroy(ArrayList_##NAME* const ArrayList)                                                                                      \
    {                                                                                                                                                             \
        free(ArrayList->Array);                                                                                                                                   \
        ArrayList->Array    = NULL;                                                                                                                               \
        ArrayList->Size     = 0;                                                                                                                                  \
        ArrayList->Capacity = 0;                                                                                                                                  \
    }

#define _ARRAYLIST_DECLARE(NAME, TYPE) \
    _ARRAYLIST_TYPE(NAME, TYPE)        \
    _ARRAYLIST_PROTOTYPES(NAME, TYPE)

#define _ARRAYLIST_INIT(NAME, TYPE, SCOPE, Equals_Function) \
    _ARRAYLIST_TYPE(NAME, TYPE)                             \
    _ARRAYLIST_IMPL(NAME, TYPE, SCOPE, Equals_Function)


#define ArrayList(NAME)                                          ArrayList_##NAME
#define ArrayList_Init(NAME, ArrayList)                          ArrayList_##NAME##_Init((ArrayList))
#define ArrayList_AddFirst(NAME, ArrayList, Data)                ArrayList_##NAME##_AddFirst((ArrayList), (Data))
#define ArrayList_AddLast(NAME, ArrayList, Data)                 ArrayList_##NAME##_AddLast((ArrayList), (Data))
#define ArrayList_AddIndex(NAME, ArrayList, Data, Index)         ArrayList_##NAME##_AddIndex_Safe((ArrayList), (Data), (Index))
#define ArrayList_DeleteFirst(NAME, ArrayList, Result)           ArrayList_##NAME##_DeleteFirst((ArrayList), (Result))
#define ArrayList_DeleteLast(NAME, ArrayList, Result)            ArrayList_##NAME##_DeleteLast((ArrayList), (Result))
#define ArrayList_DeleteIndex(NAME, ArrayList, Index, Result)    ArrayList_##NAME##_DeleteIndex((ArrayList), (Index), (Result))
#define ArrayList_SetIndex(NAME, ArrayList, Data, Index, Result) ArrayList_##NAME##_SetIndex((ArrayList), (Data), (Index), (Result))
#define ArrayList_GetFirst(NAME, ArrayList, Result)              ArrayList_##NAME##_GetFirst((ArrayList), (Result))
#define ArrayList_GetLast(NAME, ArrayList, Result)               ArrayList_##NAME##_GetLast((ArrayList), (Result))
#define ArrayList_GetIndex(NAME, ArrayList, Index, Result)       ArrayList_##NAME##_GetIndex((ArrayList), (Index), (Result))
#define ArrayList_IndexOf(NAME, ArrayList, Data)                 ArrayList_##NAME##_IndexOf((ArrayList), (Data))
#define ArrayList_LastIndexOf(NAME, ArrayList, Data)             ArrayList_##NAME##_LastIndexOf((ArrayList), (Data))
#define ArrayList_GetSize(ArrayList)                             ((ArrayList)->Size)
#define ArrayList_isEmpty(ArrayList)                             ((ArrayList)->Size == 0)
#define ArrayList_Contains(NAME, ArrayList, Data)                (ArrayList_##NAME##_IndexOf((ArrayList), (Data)) >= 0)
#define ArrayList_Clear(ArrayList)                               ((ArrayList)->Size = 0)
#define ArrayList_Destroy(NAME, ArrayList)                       ArrayList_##NAME##_Destroy((ArrayList))

// 函数实现
#define LTT_ARRAYLIST_INIT(NAME, TYPE, Equals_Function)          _ARRAYLIST_INIT(NAME, TYPE, static LTT_inline LTT_unused, Equals_Function)

// 函数声明
#define LTT_ARRAYLIST_DECLARE(NAME, TYPE)                        _ARRAYLIST_DECLARE(NAME, TYPE)