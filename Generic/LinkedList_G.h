#pragma once
#include <stdlib.h>
#include "Generic_tool.h"

// -------------------------------------------------------------------------------
// Benchmark                                     Time             CPU   Iterations
// -------------------------------------------------------------------------------
// LinkedList_G_AddFirst_Test/100000       1164698 ns      1164540 ns          689
// list_AddFirst_Test/100000               1421341 ns      1421333 ns          506
// LinkedList_G_AddLast_Test/100000        1262992 ns      1262679 ns          529
// list_AddLast_Test/100000                1614373 ns      1614376 ns          445
// LinkedList_G_DeleteFirst_Test/100000     620178 ns       620170 ns         1137
// list_DeleteFirst_Test/100000             990247 ns       990188 ns          690
// LinkedList_G_DeleteLast_Test/100000      621517 ns       621492 ns         1118
// list_DeleteLast_Test/100000              975274 ns       975269 ns          694

#define LINKEDLIST_TYPE(NAME, TYPE)         \
    typedef struct LinkedListNode_##NAME    \
    {                                       \
        struct LinkedListNode_##NAME* Prev; \
        struct LinkedListNode_##NAME* Next; \
        TYPE                          Data; \
    } LinkedListNode_##NAME;                \
    typedef struct LinkedList_##NAME        \
    {                                       \
        LinkedListNode_##NAME* EndNode;     \
        int                    Size;        \
    } LinkedList_##NAME;


#define LINKEDLIST_PROTOTYPES(NAME, TYPE)                                                                                                      \
    extern void LinkedList_##NAME##_Init(LinkedList_##NAME* const LinkedList);                                                                 \
    extern CODE LinkedList_##NAME##_AddFirst(LinkedList_##NAME* const LinkedList, const TYPE Data);                                            \
    extern CODE LinkedList_##NAME##_AddLast(LinkedList_##NAME* const LinkedList, const TYPE Data);                                             \
    extern CODE LinkedList_##NAME##_AddIndex(LinkedList_##NAME* const LinkedList, const TYPE Data, const int Index);                           \
    extern CODE LinkedList_##NAME##_DeleteFirst(LinkedList_##NAME* const LinkedList, TYPE* const Result);                                      \
    extern CODE LinkedList_##NAME##_DeleteLast(LinkedList_##NAME* const LinkedList, TYPE* const Result);                                       \
    extern CODE LinkedList_##NAME##_DeleteIndex(LinkedList_##NAME* const LinkedList, const int Index, TYPE* const Result);                     \
    extern CODE LinkedList_##NAME##_SetFirst(const LinkedList_##NAME* const LinkedList, const TYPE Data, TYPE* const Result);                  \
    extern CODE LinkedList_##NAME##_SetLast(const LinkedList_##NAME* const LinkedList, const TYPE Data, TYPE* const Result);                   \
    extern CODE LinkedList_##NAME##_SetIndex(const LinkedList_##NAME* const LinkedList, const TYPE Data, const int Index, TYPE* const Result); \
    extern CODE LinkedList_##NAME##_GetFirst(const LinkedList_##NAME* const LinkedList, TYPE* const Result);                                   \
    extern CODE LinkedList_##NAME##_GetLast(const LinkedList_##NAME* const LinkedList, TYPE* const Result);                                    \
    extern CODE LinkedList_##NAME##_GetIndex(const LinkedList_##NAME* const LinkedList, const int Index, TYPE* const Result);                  \
    extern int  LinkedList_##NAME##_IndexOf(const LinkedList_##NAME* const LinkedList, const TYPE Data);                                       \
    extern int  LinkedList_##NAME##_LastIndexOf(const LinkedList_##NAME* const LinkedList, const TYPE Data);                                   \
    extern bool LinkedList_##NAME##_Contains(const LinkedList_##NAME* const LinkedList, const TYPE Data);                                      \
    extern bool LinkedList_##NAME##_IsEmpty(const LinkedList_##NAME* const LinkedList);                                                        \
    extern int  LinkedList_##NAME##_GetSize(const LinkedList_##NAME* const LinkedList);                                                        \
    extern void LinkedList_##NAME##_Destroy(LinkedList_##NAME* const LinkedList);


#define LINKEDLIST_IMPL(NAME, TYPE, SCOPE, Equals_Function)                                                                                                                                                      \
    SCOPE void LinkedList_##NAME##_Init(LinkedList_##NAME* const LinkedList)                                                                                                                                     \
    {                                                                                                                                                                                                            \
        LinkedList->EndNode       = (LinkedListNode_##NAME*)malloc(sizeof(LinkedListNode_##NAME));                                                                                                               \
        LinkedList->EndNode->Prev = LinkedList->EndNode;                                                                                                                                                         \
        LinkedList->EndNode->Next = LinkedList->EndNode;                                                                                                                                                         \
        LinkedList->Size          = 0;                                                                                                                                                                           \
    }                                                                                                                                                                                                            \
    SCOPE void LinkedList_##NAME##_UnhookNode(LinkedListNode_##NAME* const Position)                                                                                                                             \
    {                                                                                                                                                                                                            \
        LinkedListNode_##NAME* NextNode = Position->Next;                                                                                                                                                        \
        LinkedListNode_##NAME* PrevNode = Position->Prev;                                                                                                                                                        \
        PrevNode->Next                  = NextNode;                                                                                                                                                              \
        NextNode->Prev                  = PrevNode;                                                                                                                                                              \
    }                                                                                                                                                                                                            \
    SCOPE void LinkedList_##NAME##_EraseNode(LinkedList_##NAME* const LinkedList, LinkedListNode_##NAME* const Position)                                                                                         \
    {                                                                                                                                                                                                            \
        --LinkedList->Size;                                                                                                                                                                                      \
        LinkedList_##NAME##_UnhookNode(Position);                                                                                                                                                                \
        free(Position);                                                                                                                                                                                          \
    }                                                                                                                                                                                                            \
    SCOPE bool LinkedList_##NAME##_CheckIndex(const LinkedList_##NAME* const LinkedList, const int Index)                                                                                                        \
    {                                                                                                                                                                                                            \
        if (unlikely(Index < 0 || Index >= LinkedList->Size)) return false;                                                                                                                                       \
        else return true;                                                                                                                                                                                        \
    }                                                                                                                                                                                                            \
    SCOPE LinkedListNode_##NAME* LinkedList_##NAME##_GetEndNode(const LinkedList_##NAME* const LinkedList) { return LinkedList->EndNode; }                                                                       \
    SCOPE LinkedListNode_##NAME* LinkedList_##NAME##_GetNode(const LinkedList_##NAME* const LinkedList, const int Index)                                                                                         \
    {                                                                                                                                                                                                            \
        LinkedListNode_##NAME* x = LinkedList_##NAME##_GetEndNode(LinkedList);                                                                                                                                   \
        int                    i = 0;                                                                                                                                                                            \
        if (Index < (LinkedList->Size >> 1))                                                                                                                                                                     \
        {                                                                                                                                                                                                        \
            do {                                                                                                                                                                                                 \
                x = x->Next;                                                                                                                                                                                     \
            }                                                                                                                                                                                                    \
            while (i++ < Index);                                                                                                                                                                                 \
        }                                                                                                                                                                                                        \
        else                                                                                                                                                                                                     \
        {                                                                                                                                                                                                        \
            do {                                                                                                                                                                                                 \
                x = x->Prev;                                                                                                                                                                                     \
            }                                                                                                                                                                                                    \
            while (++i < LinkedList->Size - Index);                                                                                                                                                              \
        }                                                                                                                                                                                                        \
        return x;                                                                                                                                                                                                \
    }                                                                                                                                                                                                            \
    SCOPE LinkedListNode_##NAME* LinkedList_##NAME##_GetFirstNode(const LinkedList_##NAME* const LinkedList) { return LinkedList->EndNode->Next; }                                                               \
    SCOPE void                   LinkedList_##NAME##_HookNode(LinkedListNode_##NAME* const PrevNode, LinkedListNode_##NAME* const Position)                                                                      \
    {                                                                                                                                                                                                            \
        PrevNode->Next       = Position;                                                                                                                                                                         \
        PrevNode->Prev       = Position->Prev;                                                                                                                                                                   \
        Position->Prev->Next = PrevNode;                                                                                                                                                                         \
        Position->Prev       = PrevNode;                                                                                                                                                                         \
    }                                                                                                                                                                                                            \
    SCOPE CODE LinkedList_##NAME##_MakeNode(const TYPE Data, LinkedListNode_##NAME** const Result)                                                                                                               \
    {                                                                                                                                                                                                            \
        *Result = (LinkedListNode_##NAME*)malloc(sizeof(LinkedListNode_##NAME));                                                                                                                                 \
        if (unlikely(*Result == NULL)) return MemoryAllocationError;                                                                                                                                             \
        (*Result)->Data = Data;                                                                                                                                                                                  \
        return Success;                                                                                                                                                                                          \
    }                                                                                                                                                                                                            \
    SCOPE CODE LinkedList_##NAME##_InsertPrevNode(LinkedList_##NAME* const LinkedList, LinkedListNode_##NAME* const Position, const TYPE Data)                                                                   \
    {                                                                                                                                                                                                            \
        LinkedListNode_##NAME* NewNode;                                                                                                                                                                          \
        if (Success != LinkedList_##NAME##_MakeNode(Data, &NewNode)) return MemoryAllocationError;                                                                                                               \
        LinkedList_##NAME##_HookNode(NewNode, Position);                                                                                                                                                         \
        ++LinkedList->Size;                                                                                                                                                                                      \
        return Success;                                                                                                                                                                                          \
    }                                                                                                                                                                                                            \
    SCOPE CODE LinkedList_##NAME##_AddFirst(LinkedList_##NAME* const LinkedList, const TYPE Data) { return LinkedList_##NAME##_InsertPrevNode(LinkedList, LinkedList_##NAME##_GetFirstNode(LinkedList), Data); } \
    SCOPE CODE LinkedList_##NAME##_AddLast(LinkedList_##NAME* const LinkedList, const TYPE Data) { return LinkedList_##NAME##_InsertPrevNode(LinkedList, LinkedList_##NAME##_GetEndNode(LinkedList), Data); }    \
    SCOPE CODE LinkedList_##NAME##_AddIndex(LinkedList_##NAME* const LinkedList, const TYPE Data, const int Index)                                                                                               \
    {                                                                                                                                                                                                            \
        if (unlikely(!LinkedList_##NAME##_CheckIndex(LinkedList, Index))) return ArrayIndexOutOfRange;                                                                                                           \
        if (Index == 0) return LinkedList_##NAME##_AddFirst(LinkedList, Data);                                                                                                                                   \
        if (Index == LinkedList->Size) return LinkedList_##NAME##_AddLast(LinkedList, Data);                                                                                                                     \
        LinkedListNode_##NAME* Position = LinkedList_##NAME##_GetNode(LinkedList, Index);                                                                                                                        \
        return LinkedList_##NAME##_InsertPrevNode(LinkedList, Position, Data);                                                                                                                                   \
    }                                                                                                                                                                                                            \
    SCOPE bool LinkedList_##NAME##_IsEmpty(const LinkedList_##NAME* const LinkedList) { return LinkedList->Size == 0; }                                                                                          \
    SCOPE CODE LinkedList_##NAME##_DeleteFirst(LinkedList_##NAME* const LinkedList, TYPE* const Result)                                                                                                          \
    {                                                                                                                                                                                                            \
        if (unlikely(LinkedList_##NAME##_IsEmpty(LinkedList))) return NullPointerAccess;                                                                                                                         \
        LinkedListNode_##NAME* FirstNode = LinkedList_##NAME##_GetFirstNode(LinkedList);                                                                                                                         \
        *Result                          = FirstNode->Data;                                                                                                                                                      \
        LinkedList_##NAME##_EraseNode(LinkedList, FirstNode);                                                                                                                                                    \
        return Success;                                                                                                                                                                                          \
    }                                                                                                                                                                                                            \
    SCOPE CODE LinkedList_##NAME##_DeleteLast(LinkedList_##NAME* const LinkedList, TYPE* const Result)                                                                                                           \
    {                                                                                                                                                                                                            \
        if (unlikely(LinkedList_##NAME##_IsEmpty(LinkedList))) return NullPointerAccess;                                                                                                                         \
        LinkedListNode_##NAME* LastNode = LinkedList->EndNode->Prev;                                                                                                                                             \
        *Result                         = LastNode->Data;                                                                                                                                                        \
        LinkedList_##NAME##_EraseNode(LinkedList, LastNode);                                                                                                                                                     \
        return Success;                                                                                                                                                                                          \
    }                                                                                                                                                                                                            \
    SCOPE CODE LinkedList_##NAME##_DeleteIndex(LinkedList_##NAME* const LinkedList, const int Index, TYPE* const Result)                                                                                         \
    {                                                                                                                                                                                                            \
        if (unlikely(!LinkedList_##NAME##_CheckIndex(LinkedList, Index))) return ArrayIndexOutOfRange;                                                                                                           \
        if (Index == 0) return LinkedList_##NAME##_DeleteFirst(LinkedList, Result);                                                                                                                              \
        if (Index == LinkedList->Size) return LinkedList_##NAME##_DeleteLast(LinkedList, Result);                                                                                                                \
        LinkedListNode_##NAME* Position = LinkedList_##NAME##_GetNode(LinkedList, Index);                                                                                                                        \
        *Result                         = Position->Data;                                                                                                                                                        \
        LinkedList_##NAME##_EraseNode(LinkedList, Position);                                                                                                                                                     \
        return Success;                                                                                                                                                                                          \
    }                                                                                                                                                                                                            \
    SCOPE CODE LinkedList_##NAME##_SetFirst(const LinkedList_##NAME* const LinkedList, const TYPE Data, TYPE* const Result)                                                                                      \
    {                                                                                                                                                                                                            \
        if (unlikely(LinkedList_##NAME##_IsEmpty(LinkedList))) return NullPointerAccess;                                                                                                                         \
        LinkedListNode_##NAME* FirstNode = LinkedList_##NAME##_GetFirstNode(LinkedList);                                                                                                                         \
        *Result                          = FirstNode->Data;                                                                                                                                                      \
        FirstNode->Data                  = Data;                                                                                                                                                                 \
        return Success;                                                                                                                                                                                          \
    }                                                                                                                                                                                                            \
    SCOPE CODE LinkedList_##NAME##_SetLast(const LinkedList_##NAME* const LinkedList, const TYPE Data, TYPE* const Result)                                                                                       \
    {                                                                                                                                                                                                            \
        if (unlikely(LinkedList_##NAME##_IsEmpty(LinkedList))) return NullPointerAccess;                                                                                                                         \
        LinkedListNode_##NAME* LastNode = LinkedList->EndNode->Prev;                                                                                                                                             \
        *Result                         = LastNode->Data;                                                                                                                                                        \
        LastNode->Data                  = Data;                                                                                                                                                                  \
        return Success;                                                                                                                                                                                          \
    }                                                                                                                                                                                                            \
    SCOPE CODE LinkedList_##NAME##_SetIndex(const LinkedList_##NAME* const LinkedList, const TYPE Data, const int Index, TYPE* const Result)                                                                     \
    {                                                                                                                                                                                                            \
        if (unlikely(!LinkedList_##NAME##_CheckIndex(LinkedList, Index))) return ArrayIndexOutOfRange;                                                                                                           \
        LinkedListNode_##NAME* Position = LinkedList_##NAME##_GetNode(LinkedList, Index);                                                                                                                        \
        *Result                         = Position->Data;                                                                                                                                                        \
        Position->Data                  = Data;                                                                                                                                                                  \
        return Success;                                                                                                                                                                                          \
    }                                                                                                                                                                                                            \
    SCOPE CODE LinkedList_##NAME##_GetFirst(const LinkedList_##NAME* const LinkedList, TYPE* const Result)                                                                                                       \
    {                                                                                                                                                                                                            \
        if (unlikely(LinkedList_##NAME##_IsEmpty(LinkedList))) return NullPointerAccess;                                                                                                                         \
        LinkedListNode_##NAME* FirstNode = LinkedList_##NAME##_GetFirstNode(LinkedList);                                                                                                                         \
        *Result                          = FirstNode->Data;                                                                                                                                                      \
        return Success;                                                                                                                                                                                          \
    }                                                                                                                                                                                                            \
    SCOPE CODE LinkedList_##NAME##_GetLast(const LinkedList_##NAME* const LinkedList, TYPE* const Result)                                                                                                        \
    {                                                                                                                                                                                                            \
        if (unlikely(LinkedList_##NAME##_IsEmpty(LinkedList))) return NullPointerAccess;                                                                                                                         \
        LinkedListNode_##NAME* LastNode = LinkedList->EndNode->Prev;                                                                                                                                             \
        *Result                         = LastNode->Data;                                                                                                                                                        \
        return Success;                                                                                                                                                                                          \
    }                                                                                                                                                                                                            \
    SCOPE CODE LinkedList_##NAME##_GetIndex(const LinkedList_##NAME* const LinkedList, const int Index, TYPE* const Result)                                                                                      \
    {                                                                                                                                                                                                            \
        if (unlikely(!LinkedList_##NAME##_CheckIndex(LinkedList, Index))) return ArrayIndexOutOfRange;                                                                                                           \
        LinkedListNode_##NAME* Position = LinkedList_##NAME##_GetNode(LinkedList, Index);                                                                                                                        \
        *Result                         = Position->Data;                                                                                                                                                        \
        return Success;                                                                                                                                                                                          \
    }                                                                                                                                                                                                            \
    SCOPE int LinkedList_##NAME##_IndexOf(const LinkedList_##NAME* const LinkedList, const TYPE Data)                                                                                                            \
    {                                                                                                                                                                                                            \
        int                    index = 0;                                                                                                                                                                        \
        LinkedListNode_##NAME* x     = LinkedList_##NAME##_GetFirstNode(LinkedList);                                                                                                                             \
        LinkedListNode_##NAME* End   = LinkedList_##NAME##_GetEndNode(LinkedList);                                                                                                                               \
        while (x != End)                                                                                                                                                                                         \
        {                                                                                                                                                                                                        \
            if (Equals_Function(Data, x->Data)) return index;                                                                                                                                                    \
            x = x->Next;                                                                                                                                                                                         \
            ++index;                                                                                                                                                                                             \
        }                                                                                                                                                                                                        \
        return -1;                                                                                                                                                                                               \
    }                                                                                                                                                                                                            \
    SCOPE int LinkedList_##NAME##_LastIndexOf(const LinkedList_##NAME* const LinkedList, const TYPE Data)                                                                                                        \
    {                                                                                                                                                                                                            \
        int                    index = LinkedList->Size - 1;                                                                                                                                                     \
        LinkedListNode_##NAME* x     = LinkedList->EndNode->Prev;                                                                                                                                                \
        LinkedListNode_##NAME* End   = LinkedList_##NAME##_GetEndNode(LinkedList);                                                                                                                               \
        while (x != End)                                                                                                                                                                                         \
        {                                                                                                                                                                                                        \
            if (Equals_Function(Data, x->Data)) return index;                                                                                                                                                    \
            x = x->Prev;                                                                                                                                                                                         \
            --index;                                                                                                                                                                                             \
        }                                                                                                                                                                                                        \
        return -1;                                                                                                                                                                                               \
    }                                                                                                                                                                                                            \
    SCOPE bool LinkedList_##NAME##_Contains(const LinkedList_##NAME* const LinkedList, const TYPE Data) { return LinkedList_##NAME##_IndexOf(LinkedList, Data) >= 0; }                                           \
    SCOPE int  LinkedList_##NAME##_GetSize(const LinkedList_##NAME* const LinkedList) { return LinkedList->Size; }                                                                                               \
    SCOPE void LinkedList_##NAME##_Destroy(LinkedList_##NAME* const LinkedList)                                                                                                                                  \
    {                                                                                                                                                                                                            \
        LinkedListNode_##NAME* end   = LinkedList_##NAME##_GetEndNode(LinkedList);                                                                                                                               \
        LinkedListNode_##NAME* begin = end->Next;                                                                                                                                                                \
        while (begin != end)                                                                                                                                                                                     \
        {                                                                                                                                                                                                        \
            LinkedListNode_##NAME* Next = begin->Next;                                                                                                                                                           \
            free(begin);                                                                                                                                                                                         \
            begin = Next;                                                                                                                                                                                        \
        }                                                                                                                                                                                                        \
        free(LinkedList->EndNode);                                                                                                                                                                               \
        LinkedList->EndNode = NULL;                                                                                                                                                                              \
        LinkedList->Size    = 0;                                                                                                                                                                                 \
    }


#define LINKEDLIST_DECLARE(NAME, TYPE) \
    LINKEDLIST_TYPE(NAME, TYPE)        \
    LINKEDLIST_PROTOTYPES(NAME, TYPE)


#define LINKEDLIST_INIT(NAME, TYPE, SCOPE, Equals_Function) \
    LINKEDLIST_TYPE(NAME, TYPE)                             \
    LINKEDLIST_IMPL(NAME, TYPE, SCOPE, Equals_Function)


#define LinkedList(NAME)                                           LinkedList_##NAME
#define LinkedList_Init(NAME, LinkedList)                          LinkedList_##NAME##_Init((LinkedList))
#define LinkedList_AddFirst(NAME, LinkedList, Data)                LinkedList_##NAME##_AddFirst((LinkedList), (Data))
#define LinkedList_AddLast(NAME, LinkedList, Data)                 LinkedList_##NAME##_AddLast((LinkedList), (Data))
#define LinkedList_AddIndex(NAME, LinkedList, Data, Index)         LinkedList_##NAME##_AddIndex((LinkedList), (Data), (Index))
#define LinkedList_DeleteFirst(NAME, LinkedList, Result)           LinkedList_##NAME##_DeleteFirst((LinkedList), (Result))
#define LinkedList_DeleteLast(NAME, LinkedList, Result)            LinkedList_##NAME##_DeleteLast((LinkedList), (Result))
#define LinkedList_DeleteIndex(NAME, LinkedList, Index, Result)    LinkedList_##NAME##_DeleteIndex((LinkedList), (Index), (Result))
#define LinkedList_SetFirst(NAME, LinkedList, Data, Result)        LinkedList_##NAME##_SetFirst((LinkedList), (Data), (Result))
#define LinkedList_SetLast(NAME, LinkedList, Data, Result)         LinkedList_##NAME##_SetLast((LinkedList), (Data), (Result))
#define LinkedList_SetIndex(NAME, LinkedList, Data, Index, Result) LinkedList_##NAME##_SetIndex((LinkedList), (Data), (Index), (Result))
#define LinkedList_GetFirst(NAME, LinkedList, Result)              LinkedList_##NAME##_GetFirst((LinkedList), (Result))
#define LinkedList_GetLast(NAME, LinkedList, Result)               LinkedList_##NAME##_GetLast((LinkedList), (Result))
#define LinkedList_GetIndex(NAME, LinkedList, Index, Result)       LinkedList_##NAME##_GetIndex((LinkedList), (Index), (Result))
#define LinkedList_IndexOf(NAME, LinkedList, Data)                 LinkedList_##NAME##_IndexOf((LinkedList), (Data))
#define LinkedList_LastIndexOf(NAME, LinkedList, Data)             LinkedList_##NAME##_LastIndexOf((LinkedList), (Data))
#define LinkedList_Contains(NAME, LinkedList, Data)                (LinkedList_##NAME##_IndexOf((LinkedList), (Data)) >= 0)
#define LinkedList_IsEmpty(LinkedList)                             ((LinkedList)->Size == 0)
#define LinkedList_GetSize(LinkedList)                             ((LinkedList)->Size)
#define LinkedList_Destroy(NAME, LinkedList)                       LinkedList_##NAME##_Destroy((LinkedList))

// 函数实现
#define LTT_LINKEDLIST_INIT(NAME, TYPE, Equals_Function)           LINKEDLIST_INIT(NAME, TYPE, static LTT_inline LTT_unused, Equals_Function)

// 函数声明
#define LTT_LINKEDLIST_DECLARE(NAME, TYPE)                         LINKEDLIST_DECLARE(NAME, TYPE)