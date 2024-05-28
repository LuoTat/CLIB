#pragma once
#include <stdlib.h>
#include "Generic_tool.h"

// TODO:优化AddFirst和AddLast函数

#define _LINKEDLIST_TYPE(NAME, TYPE)        \
    typedef struct LinkedListNode_##NAME    \
    {                                       \
        struct LinkedListNode_##NAME* Prev; \
        struct LinkedListNode_##NAME* Next; \
        TYPE                          Data; \
    } LinkedListNode_##NAME;                \
    typedef struct LinkedList_##NAME        \
    {                                       \
        LinkedListNode_##NAME* First;       \
        LinkedListNode_##NAME* Last;        \
        int                    Size;        \
    } LinkedList_##NAME;

#define _LINKEDLIST_PROTOTYPES(NAME, TYPE)                                                                                    \
    extern void LinkedList_##NAME##_Init(LinkedList_##NAME* const LinkedList);                                                \
    extern CODE LinkedList_##NAME##_AddFirst(LinkedList_##NAME* const LinkedList, TYPE Data);                                 \
    extern CODE LinkedList_##NAME##_AddLast(LinkedList_##NAME* const LinkedList, TYPE Data);                                  \
    extern CODE LinkedList_##NAME##_AddIndex(LinkedList_##NAME* const LinkedList, TYPE Data, const int Index);                \
    extern CODE LinkedList_##NAME##_DeleteFirst(LinkedList_##NAME* const LinkedList, TYPE* const Result);                     \
    extern CODE LinkedList_##NAME##_DeleteLast(LinkedList_##NAME* const LinkedList, TYPE* const Result);                      \
    extern CODE LinkedList_##NAME##_DeleteIndex(LinkedList_##NAME* const LinkedList, const int Index, TYPE* const Result);    \
    extern CODE LinkedList_##NAME##_SetFirst(const LinkedList_##NAME* const LinkedList, TYPE Data);                           \
    extern CODE LinkedList_##NAME##_SetLast(const LinkedList_##NAME* const LinkedList, TYPE Data);                            \
    extern CODE LinkedList_##NAME##_SetIndex(const LinkedList_##NAME* const LinkedList, TYPE Data, const int Index);          \
    extern CODE LinkedList_##NAME##_GetFirst(const LinkedList_##NAME* const LinkedList, TYPE* const Result);                  \
    extern CODE LinkedList_##NAME##_GetLast(const LinkedList_##NAME* const LinkedList, TYPE* const Result);                   \
    extern CODE LinkedList_##NAME##_GetIndex(const LinkedList_##NAME* const LinkedList, const int Index, TYPE* const Result); \
    extern int  LinkedList_##NAME##_IndexOf(const LinkedList_##NAME* const LinkedList, const TYPE Data);                      \
    extern int  LinkedList_##NAME##_LastIndexOf(const LinkedList_##NAME* const LinkedList, const TYPE Data);                  \
    extern bool LinkedList_##NAME##_Contains(const LinkedList_##NAME* const LinkedList, const TYPE Data);                     \
    extern bool LinkedList_##NAME##_IsEmpty(const LinkedList_##NAME* const LinkedList);                                       \
    extern int  LinkedList_##NAME##_GetSize(const LinkedList_##NAME* const LinkedList);                                       \
    extern void LinkedList_##NAME##_Destroy(LinkedList_##NAME* const LinkedList);


#define _LINKEDLIST_IMPL(NAME, TYPE, SCOPE, Equals_Function)                                                                                                           \
    SCOPE bool LinkedList_##NAME##_CheckIndex(const LinkedList_##NAME* const LinkedList, const int Index)                                                              \
    {                                                                                                                                                                  \
        if (Index < 0 || Index > LinkedList->Size) return false;                                                                                                       \
        else return true;                                                                                                                                              \
    }                                                                                                                                                                  \
    SCOPE LinkedListNode_##NAME* LinkedList_##NAME##_GetNode(const LinkedList_##NAME* const LinkedList, const int Index)                                               \
    {                                                                                                                                                                  \
        LinkedListNode_##NAME* x;                                                                                                                                      \
        if (Index < (LinkedList->Size >> 1))                                                                                                                           \
        {                                                                                                                                                              \
            x = LinkedList->First;                                                                                                                                     \
            for (int i = 0; i < Index; i++) x = x->Next;                                                                                                               \
            return x;                                                                                                                                                  \
        }                                                                                                                                                              \
        else                                                                                                                                                           \
        {                                                                                                                                                              \
            x = LinkedList->Last;                                                                                                                                      \
            for (int i = LinkedList->Size - 1; i > Index; i--) x = x->Prev;                                                                                            \
            return x;                                                                                                                                                  \
        }                                                                                                                                                              \
    }                                                                                                                                                                  \
    SCOPE CODE LinkedList_##NAME##_MakeNode(LinkedListNode_##NAME* const Prev, TYPE Data, LinkedListNode_##NAME* const Next, LinkedListNode_##NAME** const Result)     \
    {                                                                                                                                                                  \
        *Result = (LinkedListNode_##NAME*)malloc(sizeof(LinkedListNode_##NAME));                                                                                       \
        if (*Result == NULL) return MemoryAllocationError;                                                                                                             \
        (*Result)->Data = Data;                                                                                                                                        \
        (*Result)->Prev = Prev;                                                                                                                                        \
        (*Result)->Next = Next;                                                                                                                                        \
        return Success;                                                                                                                                                \
    }                                                                                                                                                                  \
    SCOPE void LinkedList_##NAME##_Init(LinkedList_##NAME* const LinkedList)                                                                                           \
    {                                                                                                                                                                  \
        LinkedList->First = NULL;                                                                                                                                      \
        LinkedList->Last  = NULL;                                                                                                                                      \
        LinkedList->Size  = 0;                                                                                                                                         \
    }                                                                                                                                                                  \
    SCOPE CODE LinkedList_##NAME##_AddFirst(LinkedList_##NAME* const LinkedList, TYPE Data)                                                                            \
    {                                                                                                                                                                  \
        LinkedListNode_##NAME* FirstNode = LinkedList->First;                                                                                                          \
        LinkedListNode_##NAME* NewNode;                                                                                                                                \
        if (Success != LinkedList_##NAME##_MakeNode(NULL, Data, FirstNode, &NewNode)) return MemoryAllocationError;                                                    \
        LinkedList->First = NewNode;                                                                                                                                   \
        if (FirstNode == NULL) LinkedList->Last = NewNode;                                                                                                             \
        else FirstNode->Prev = NewNode;                                                                                                                                \
        ++LinkedList->Size;                                                                                                                                            \
        return Success;                                                                                                                                                \
    }                                                                                                                                                                  \
    SCOPE CODE LinkedList_##NAME##_AddLast(LinkedList_##NAME* const LinkedList, TYPE Data)                                                                             \
    {                                                                                                                                                                  \
        LinkedListNode_##NAME* LastNode = LinkedList->Last;                                                                                                            \
        LinkedListNode_##NAME* NewNode;                                                                                                                                \
        if (Success != LinkedList_##NAME##_MakeNode(LastNode, Data, NULL, &NewNode)) return MemoryAllocationError;                                                     \
        LinkedList->Last = NewNode;                                                                                                                                    \
        if (LastNode == NULL) LinkedList->First = NewNode;                                                                                                             \
        else LastNode->Next = NewNode;                                                                                                                                 \
        ++LinkedList->Size;                                                                                                                                            \
        return Success;                                                                                                                                                \
    }                                                                                                                                                                  \
    SCOPE CODE LinkedList_##NAME##_AddIndex(LinkedList_##NAME* const LinkedList, TYPE Data, const int Index)                                                           \
    {                                                                                                                                                                  \
        if (!LinkedList_##NAME##_CheckIndex(LinkedList, Index)) return ArrayIndexOutOfRange;                                                                           \
        if (Index == 0) return LinkedList_##NAME##_AddFirst(LinkedList, Data);                                                                                         \
        if (Index == LinkedList->Size) return LinkedList_##NAME##_AddLast(LinkedList, Data);                                                                           \
        LinkedListNode_##NAME* x    = LinkedList_##NAME##_GetNode(LinkedList, Index);                                                                                  \
        LinkedListNode_##NAME* Prev = x->Prev;                                                                                                                         \
        LinkedListNode_##NAME* NewNode;                                                                                                                                \
        if (Success != LinkedList_##NAME##_MakeNode(Prev, Data, x, &NewNode)) return MemoryAllocationError;                                                            \
        x->Prev = NewNode;                                                                                                                                             \
        if (Prev == NULL) LinkedList->First = NewNode;                                                                                                                 \
        else Prev->Next = NewNode;                                                                                                                                     \
        ++LinkedList->Size;                                                                                                                                            \
        return Success;                                                                                                                                                \
    }                                                                                                                                                                  \
    SCOPE CODE LinkedList_##NAME##_DeleteFirst(LinkedList_##NAME* const LinkedList, TYPE* const Result)                                                                \
    {                                                                                                                                                                  \
        LinkedListNode_##NAME* FirstNode = LinkedList->First;                                                                                                          \
        if (FirstNode == NULL) return NullPointerAccess;                                                                                                               \
        *Result                     = FirstNode->Data;                                                                                                                 \
        LinkedListNode_##NAME* Next = FirstNode->Next;                                                                                                                 \
        LinkedList->First           = Next;                                                                                                                            \
        if (Next == NULL) LinkedList->Last = NULL;                                                                                                                     \
        else Next->Prev = NULL;                                                                                                                                        \
        --LinkedList->Size;                                                                                                                                            \
        free(FirstNode);                                                                                                                                               \
        return Success;                                                                                                                                                \
    }                                                                                                                                                                  \
    SCOPE CODE LinkedList_##NAME##_DeleteLast(LinkedList_##NAME* const LinkedList, TYPE* const Result)                                                                 \
    {                                                                                                                                                                  \
        LinkedListNode_##NAME* LastNode = LinkedList->Last;                                                                                                            \
        if (LastNode == NULL) return NullPointerAccess;                                                                                                                \
        *Result                     = LastNode->Data;                                                                                                                  \
        LinkedListNode_##NAME* Prev = LastNode->Prev;                                                                                                                  \
        LinkedList->Last            = Prev;                                                                                                                            \
        if (Prev == NULL) LinkedList->First = NULL;                                                                                                                    \
        else Prev->Next = NULL;                                                                                                                                        \
        --LinkedList->Size;                                                                                                                                            \
        free(LastNode);                                                                                                                                                \
        return Success;                                                                                                                                                \
    }                                                                                                                                                                  \
    SCOPE CODE LinkedList_##NAME##_DeleteIndex(LinkedList_##NAME* const LinkedList, const int Index, TYPE* const Result)                                               \
    {                                                                                                                                                                  \
        if (!LinkedList_##NAME##_CheckIndex(LinkedList, Index)) return ArrayIndexOutOfRange;                                                                           \
        if (Index == 0) return LinkedList_##NAME##_DeleteFirst(LinkedList, Result);                                                                                    \
        if (Index == LinkedList->Size) return LinkedList_##NAME##_DeleteLast(LinkedList, Result);                                                                      \
        LinkedListNode_##NAME* x    = LinkedList_##NAME##_GetNode(LinkedList, Index);                                                                                  \
        *Result                     = x->Data;                                                                                                                         \
        LinkedListNode_##NAME* Prev = x->Prev;                                                                                                                         \
        LinkedListNode_##NAME* Next = x->Next;                                                                                                                         \
        if (Prev == NULL) LinkedList->First = Next;                                                                                                                    \
        else                                                                                                                                                           \
        {                                                                                                                                                              \
            Prev->Next = Next;                                                                                                                                         \
            x->Prev    = NULL;                                                                                                                                         \
        }                                                                                                                                                              \
        if (Next == NULL) LinkedList->Last = Prev;                                                                                                                     \
        else                                                                                                                                                           \
        {                                                                                                                                                              \
            Next->Prev = Prev;                                                                                                                                         \
            x->Next    = NULL;                                                                                                                                         \
        }                                                                                                                                                              \
        free(x);                                                                                                                                                       \
        --LinkedList->Size;                                                                                                                                            \
        return Success;                                                                                                                                                \
    }                                                                                                                                                                  \
    SCOPE CODE LinkedList_##NAME##_SetFirst(const LinkedList_##NAME* const LinkedList, TYPE Data)                                                                      \
    {                                                                                                                                                                  \
        LinkedListNode_##NAME* FirstNode = LinkedList->First;                                                                                                          \
        if (FirstNode == NULL) return NullPointerAccess;                                                                                                               \
        else                                                                                                                                                           \
        {                                                                                                                                                              \
            FirstNode->Data = Data;                                                                                                                                    \
            return Success;                                                                                                                                            \
        }                                                                                                                                                              \
    }                                                                                                                                                                  \
    SCOPE CODE LinkedList_##NAME##_SetLast(const LinkedList_##NAME* const LinkedList, TYPE Data)                                                                       \
    {                                                                                                                                                                  \
        LinkedListNode_##NAME* LastNode = LinkedList->Last;                                                                                                            \
        if (LastNode == NULL) return NullPointerAccess;                                                                                                                \
        else                                                                                                                                                           \
        {                                                                                                                                                              \
            LastNode->Data = Data;                                                                                                                                     \
            return Success;                                                                                                                                            \
        }                                                                                                                                                              \
    }                                                                                                                                                                  \
    SCOPE CODE LinkedList_##NAME##_SetIndex(const LinkedList_##NAME* const LinkedList, TYPE Data, const int Index)                                                     \
    {                                                                                                                                                                  \
        if (!LinkedList_##NAME##_CheckIndex(LinkedList, Index)) return ArrayIndexOutOfRange;                                                                           \
        LinkedListNode_##NAME* x = LinkedList_##NAME##_GetNode(LinkedList, Index);                                                                                     \
        if (x == NULL) return NullPointerAccess;                                                                                                                       \
        else                                                                                                                                                           \
        {                                                                                                                                                              \
            x->Data = Data;                                                                                                                                            \
            return Success;                                                                                                                                            \
        }                                                                                                                                                              \
    }                                                                                                                                                                  \
    SCOPE CODE LinkedList_##NAME##_GetFirst(const LinkedList_##NAME* const LinkedList, TYPE* const Result)                                                             \
    {                                                                                                                                                                  \
        LinkedListNode_##NAME* FirstNode = LinkedList->First;                                                                                                          \
        if (FirstNode == NULL) return NullPointerAccess;                                                                                                               \
        *Result = FirstNode->Data;                                                                                                                                     \
        return Success;                                                                                                                                                \
    }                                                                                                                                                                  \
    SCOPE CODE LinkedList_##NAME##_GetLast(const LinkedList_##NAME* const LinkedList, TYPE* const Result)                                                              \
    {                                                                                                                                                                  \
        LinkedListNode_##NAME* LastNode = LinkedList->Last;                                                                                                            \
        if (LastNode == NULL) return NullPointerAccess;                                                                                                                \
        *Result = LastNode->Data;                                                                                                                                      \
        return Success;                                                                                                                                                \
    }                                                                                                                                                                  \
    SCOPE CODE LinkedList_##NAME##_GetIndex(const LinkedList_##NAME* const LinkedList, const int Index, TYPE* const Result)                                            \
    {                                                                                                                                                                  \
        if (!LinkedList_##NAME##_CheckIndex(LinkedList, Index)) return ArrayIndexOutOfRange;                                                                           \
        LinkedListNode_##NAME* x = LinkedList_##NAME##_GetNode(LinkedList, Index);                                                                                     \
        if (x == NULL) return NullPointerAccess;                                                                                                                       \
        *Result = x->Data;                                                                                                                                             \
        return Success;                                                                                                                                                \
    }                                                                                                                                                                  \
    SCOPE int LinkedList_##NAME##_IndexOf(const LinkedList_##NAME* const LinkedList, const TYPE Data)                                                                  \
    {                                                                                                                                                                  \
        int                    index = 0;                                                                                                                              \
        LinkedListNode_##NAME* x;                                                                                                                                      \
        for (x = LinkedList->First; x != NULL; x = x->Next)                                                                                                            \
        {                                                                                                                                                              \
            if (Equals_Function(Data, x->Data)) { return index; }                                                                                                      \
            ++index;                                                                                                                                                   \
        }                                                                                                                                                              \
        return -1;                                                                                                                                                     \
    }                                                                                                                                                                  \
    SCOPE int LinkedList_##NAME##_LastIndexOf(const LinkedList_##NAME* const LinkedList, const TYPE Data)                                                              \
    {                                                                                                                                                                  \
        int                    index = LinkedList->Size;                                                                                                               \
        LinkedListNode_##NAME* x;                                                                                                                                      \
        for (x = LinkedList->Last; x != NULL; x = x->Prev)                                                                                                             \
        {                                                                                                                                                              \
            --index;                                                                                                                                                   \
            if (Equals_Function(Data, x->Data)) { return index; }                                                                                                      \
        }                                                                                                                                                              \
        return -1;                                                                                                                                                     \
    }                                                                                                                                                                  \
    SCOPE bool LinkedList_##NAME##_Contains(const LinkedList_##NAME* const LinkedList, const TYPE Data) { return LinkedList_##NAME##_IndexOf(LinkedList, Data) >= 0; } \
    SCOPE bool LinkedList_##NAME##_IsEmpty(const LinkedList_##NAME* const LinkedList) { return LinkedList->Size == 0; }                                                \
    SCOPE int  LinkedList_##NAME##_GetSize(const LinkedList_##NAME* const LinkedList) { return LinkedList->Size; }                                                     \
    SCOPE void LinkedList_##NAME##_Destroy(LinkedList_##NAME* const LinkedList)                                                                                        \
    {                                                                                                                                                                  \
        for (LinkedListNode_##NAME* x = LinkedList->First; x != NULL;)                                                                                                 \
        {                                                                                                                                                              \
            LinkedListNode_##NAME* Next = x->Next;                                                                                                                     \
            free(x);                                                                                                                                                   \
            x = Next;                                                                                                                                                  \
        }                                                                                                                                                              \
        LinkedList->First = LinkedList->Last = NULL;                                                                                                                   \
        LinkedList->Size                     = 0;                                                                                                                      \
    }

#define _LINKEDLIST_DECLARE(NAME, TYPE) \
    _LINKEDLIST_TYPE(NAME, TYPE)        \
    _LINKEDLIST_PROTOTYPES(NAME, TYPE)

#define _LINKEDLIST_INIT(NAME, TYPE, SCOPE, Equals_Function) \
    _LINKEDLIST_TYPE(NAME, TYPE)                             \
    _LINKEDLIST_IMPL(NAME, TYPE, SCOPE, Equals_Function)

#define LinkedList(NAME)                                        LinkedList_##NAME
#define LinkedList_Init(NAME, LinkedList)                       LinkedList_##NAME##_Init((LinkedList))
#define LinkedList_AddFirst(NAME, LinkedList, Data)             LinkedList_##NAME##_AddFirst((LinkedList), (Data))
#define LinkedList_AddLast(NAME, LinkedList, Data)              LinkedList_##NAME##_AddLast((LinkedList), (Data))
#define LinkedList_AddIndex(NAME, LinkedList, Data, Index)      LinkedList_##NAME##_AddIndex((LinkedList), (Data), (Index))
#define LinkedList_DeleteFirst(NAME, LinkedList, Result)        LinkedList_##NAME##_DeleteFirst((LinkedList), (Result))
#define LinkedList_DeleteLast(NAME, LinkedList, Result)         LinkedList_##NAME##_DeleteLast((LinkedList), (Result))
#define LinkedList_DeleteIndex(NAME, LinkedList, Index, Result) LinkedList_##NAME##_DeleteIndex((LinkedList), (Index), (Result))
#define LinkedList_SetFirst(NAME, LinkedList, Data)             LinkedList_##NAME##_SetFirst((LinkedList), (Data))
#define LinkedList_SetLast(NAME, LinkedList, Data)              LinkedList_##NAME##_SetLast((LinkedList), (Data))
#define LinkedList_SetIndex(NAME, LinkedList, Data, Index)      LinkedList_##NAME##_SetIndex((LinkedList), (Data), (Index))
#define LinkedList_GetFirst(NAME, LinkedList, Result)           LinkedList_##NAME##_GetFirst((LinkedList), (Result))
#define LinkedList_GetLast(NAME, LinkedList, Result)            LinkedList_##NAME##_GetLast((LinkedList), (Result))
#define LinkedList_GetIndex(NAME, LinkedList, Index, Result)    LinkedList_##NAME##_GetIndex((LinkedList), (Index), (Result))
#define LinkedList_IndexOf(NAME, LinkedList, Data)              LinkedList_##NAME##_IndexOf((LinkedList), (Data))
#define LinkedList_LastIndexOf(NAME, LinkedList, Data)          LinkedList_##NAME##_LastIndexOf((LinkedList), (Data))
#define LinkedList_Contains(NAME, LinkedList, Data)             (LinkedList_##NAME##_GetIndex((LinkedList), (Data)) >= 0)
#define LinkedList_IsEmpty(LinkedList)                          ((LinkedList)->Size == 0)
#define LinkedList_GetSize(LinkedList)                          ((LinkedList)->Size)
#define LinkedList_Destroy(NAME, LinkedList)                    LinkedList_##NAME##_Destroy((LinkedList))




// 函数实现
#define LTT_LINKEDLIST_INIT(NAME, TYPE, Equals_Function)        _LINKEDLIST_INIT(NAME, TYPE, static LTT_inline LTT_unused, Equals_Function)

// 函数声明
#define LTT_LINKEDLIST_DECLARE(NAME, TYPE)                      _LINKEDLIST_DECLARE(NAME, TYPE)