#include "LinkedList.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct LinkedListNode
{
    struct LinkedListNode* Prev;    // 上一个元素
    struct LinkedListNode* Next;    // 下一个元素
    void*                  Data;    // 数据
} LinkedListNode;

typedef struct _LinkedList
{
    LinkedListNode* First;          // 头结点
    LinkedListNode* Last;           // 尾结点
    int             Size;           // 当前元素个数
    size_t          DataSize;       // 数据大小
    Iterator        Iterator;       // 迭代器
    Equals_Function Equals;         // 比较函数
} _LinkedList;

void* LTT_LinkedList_GetCurrent(const Iterator* const IP) { return ((LinkedListNode*)IP->Current)->Data; }

inline static void LTT_LinkedList_BackToFirst(Iterator* const IP)
{
    LinkedList* LinkedList = IP->Collection;
    IP->Current            = LinkedList->First;
}

inline static int LTT_LinkedList_HasNext(const Iterator* const IP)
{
    LinkedList* LinkedList = IP->Collection;
    return IP->Current != LinkedList->Last;
}

static int LTT_LinkedList_MoveNext(Iterator* const IP)
{
    if (IP->Current == NULL)
    {
        LTT_LinkedList_BackToFirst(IP);    //迭代器自初始化成功刻起，必然有至少一个元素
        return 1;
    }
    if (LTT_LinkedList_HasNext(IP) == 0) return 0;
    IP->Current = ((LinkedListNode*)IP->Current)->Next;
    return 1;
}

static void LTT_LinkedList_Reset(Iterator* const IP) { IP->Current = NULL; }

Iterator LTT_LinkedList_GetIterator(const LinkedList* const LinkedList)
{
    if (LinkedList->Size <= 0) { printf("链表为空\n"); }
    return LinkedList->Iterator;
}

LinkedList* LTT_LinkedList_New(const size_t DataSize, Equals_Function Equals)
{
    LinkedList* LinkedListP = (LinkedList*)malloc(sizeof(LinkedList));
    if (LinkedListP == NULL)
    {
        printf("内存分配失败!\n");
        return NULL;
    }
    LinkedListP->First    = NULL;
    LinkedListP->Last     = NULL;
    LinkedListP->Size     = 0;
    LinkedListP->DataSize = DataSize;
    //初始化迭代器
    InitIterator(&LinkedListP->Iterator, LinkedListP, LTT_LinkedList_MoveNext, LTT_LinkedList_HasNext, LTT_LinkedList_Reset, LTT_LinkedList_BackToFirst);
    //用户可自定义的函数
    LinkedListP->Equals = Equals ? Equals : LTT_DEFAULT_Equals;
    return LinkedListP;
}

static LinkedListNode* LTT_LinkedList_MakeNode(LinkedListNode* const Prev, void* const Data, LinkedListNode* const Next)
{
    LinkedListNode* NewNode = (LinkedListNode*)malloc(sizeof(LinkedListNode));
    if (NewNode == NULL)
    {
        printf("节点分配失败!\n");
        return NULL;
    }
    NewNode->Data = Data;
    NewNode->Prev = Prev;
    NewNode->Next = Next;
    return NewNode;
}

Status LTT_LinkedList_AddFirst(LinkedList* const LinkedList, void* const Data)
{
    LinkedListNode* FirstNode = LinkedList->First;
    LinkedListNode* NewNode   = LTT_LinkedList_MakeNode(NULL, Data, FirstNode);
    LinkedList->First         = NewNode;
    if (FirstNode == NULL) { LinkedList->Last = NewNode; }
    else { FirstNode->Prev = NewNode; }
    ++LinkedList->Size;
    return OK;
}

void* LTT_LinkedList_DeleteFirst(LinkedList* const LinkedList)
{
    LinkedListNode* FirstNode = LinkedList->First;
    if (FirstNode == NULL) { return NULL; }
    LinkedListNode* Next    = FirstNode->Next;
    void*           OldData = FirstNode->Data;
    LinkedList->First       = Next;
    if (Next == NULL) { LinkedList->Last = NULL; }
    else { Next->Prev = NULL; }
    --LinkedList->Size;
    free(FirstNode);
    return OldData;
}

Status LTT_LinkedList_AddLast(LinkedList* const LinkedList, void* const Data)
{
    LinkedListNode* LastNode = LinkedList->Last;
    LinkedListNode* NewNode  = LTT_LinkedList_MakeNode(LastNode, Data, NULL);
    LinkedList->Last         = NewNode;
    if (LastNode == NULL) { LinkedList->First = NewNode; }
    else { LastNode->Next = NewNode; }
    ++LinkedList->Size;
    return OK;
}

void* LTT_LinkedList_DeleteLast(LinkedList* const LinkedList)
{
    LinkedListNode* LastNode = LinkedList->Last;
    if (LastNode == NULL) { return NULL; }
    LinkedListNode* Prev    = LastNode->Prev;
    void*           OldData = LastNode->Data;
    LinkedList->Last        = Prev;
    if (Prev == NULL) { LinkedList->First = NULL; }
    else { Prev->Next = NULL; }
    --LinkedList->Size;
    free(LastNode);
    return OldData;
}

void* LTT_LinkedList_GetFirst(const LinkedList* const LinkedList)
{
    LinkedListNode* FirstNode = LinkedList->First;
    if (FirstNode == NULL) { return NULL; }
    return FirstNode->Data;
}

void* LTT_LinkedList_GetLast(const LinkedList* const LinkedList)
{
    LinkedListNode* LastNode = LinkedList->Last;
    if (LastNode == NULL) { return NULL; }
    return LastNode->Data;
}

static bool LTT_LinkedList_CheckIndex(const LinkedList* const LinkedList, const int Index)
{
    if (Index < 0 || Index >= LinkedList->Size)
    {
        printf("数组下标越界！\n");
        return false;
    }
    return true;
}

void* LTT_LinkedList_GetData(const LinkedList* const LinkedList, const int Index)
{
    if (!LTT_LinkedList_CheckIndex(LinkedList, Index)) return NULL;
    LinkedListNode* x;
    if (Index < (LinkedList->Size >> 1))    //从前往后找
    {
        x = LinkedList->First;
        for (int i = 0; i < Index; i++) x = x->Next;
        return x->Data;
    }
    else                                    //从后往前找
    {
        x = LinkedList->Last;
        for (int i = LinkedList->Size - 1; i > Index; i--) x = x->Prev;
        return x->Data;
    }
}

int LTT_LinkedList_IndexOf(const LinkedList* const LinkedList, const void* const Data)
{
    int             index = 0;
    LinkedListNode* x;
    if (Data == NULL)
    {
        for (x = LinkedList->First; x != NULL; x = x->Next)
        {
            if (x->Data == NULL) return index;
            ++index;
        }
    }
    else
    {
        for (x = LinkedList->First; x != NULL; x = x->Next)
        {
            if (Data == x->Data || LinkedList->Equals(Data, x->Data, LinkedList->DataSize)) return index;
            ++index;
        }
    }
    return -1;
}

int LTT_LinkedList_LastIndexOf(const LinkedList* const LinkedList, const void* const Data)
{
    int             index = LinkedList->Size;
    LinkedListNode* x;
    if (Data == NULL)
    {
        for (x = LinkedList->Last; x != NULL; x = x->Prev)
        {
            --index;
            if (x->Data == NULL) return index;
        }
    }
    else
    {
        for (x = LinkedList->Last; x != NULL; x = x->Prev)
        {
            --index;
            if (Data == x->Data || LinkedList->Equals(Data, x->Data, LinkedList->DataSize)) return index;
        }
    }
    return -1;
}

bool LTT_LinkedList_Contains(const LinkedList* const LinkedList, void* const Data) { return LTT_LinkedList_IndexOf(LinkedList, Data) >= 0; }

bool LTT_LinkedList_IsEmpty(const LinkedList* const LinkedList) { return LinkedList->Size == 0; }

int LTT_LinkedList_GetSize(const LinkedList* const LinkedList) { return LinkedList->Size; }

void LTT_LinkedList_Clear(LinkedList* const LinkedList)
{
    for (LinkedListNode* x = LinkedList->First; x != NULL;)
    {
        LinkedListNode* Next = x->Next;
        x->Data              = NULL;
        x->Next              = NULL;
        x->Prev              = NULL;
        x                    = Next;
    }
    LinkedList->First = LinkedList->Last = NULL;
    LinkedList->Size                     = 0;
}

void LTT_LinkedList_Destroy(LinkedList** LinkedList)
{
    for (LinkedListNode* x = (*LinkedList)->First; x != NULL;)
    {
        LinkedListNode* Next = x->Next;
        free(x);
        x = Next;
    }
    free(*LinkedList);
    *LinkedList = NULL;
}
