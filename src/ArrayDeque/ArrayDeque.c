#include "ArrayDeque.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_ARRAYDEQUE_CAPACITY (16 + 1)
#define MAX_ARRAYDEQUE_CAPACITY     (INT_MAX - 8)
#define INC(Head, Capacity)         (((Head) + 1LL) % (Capacity))                 // 同余类+1
#define DEC(Head, Capacity)         (((Head) + ((Capacity) - 1LL)) % (Capacity))    // 同余类-1

typedef struct _ArrayDeque
{
    void**          Array;                                                    // 数组,装的是元素的地址
    int             Head;                                                     // 头部元素的下标
    int             Tail;                                                     // 尾部元素的下标
    int             Size;                                                     // 当前元素个数
    int             Capacity;                                                 // 数组容量
    size_t          DataSize;
    Iterator        Iterator;                                                 // 迭代器
    Equals_Function Equals;                                                   // 比较函数
} _ArrayDeque;

void* LTT_ArrayDeque_GetCurrent(const Iterator* const IP) { return *(void**)IP->Current; }

inline static void LTT_ArrayDeque_BackToFirst(Iterator* const IP)
{
    ArrayDeque* ArrayDeque = IP->Collection;
    IP->Current            = &ArrayDeque->Array[ArrayDeque->Head];
}

inline static int LTT_ArrayDeque_HasNext(const Iterator* const IP)
{
    ArrayDeque* ArrayDeque = IP->Collection;
    return IP->Current != &ArrayDeque->Array[DEC(ArrayDeque->Tail, ArrayDeque->Capacity)];
}

static int LTT_ArrayDeque_MoveNext(Iterator* const IP)
{
    ArrayDeque* ArrayDeque = IP->Collection;
    void**      Current    = IP->Current;
    if (Current == NULL)
    {
        LTT_ArrayDeque_BackToFirst(IP);    //迭代器自初始化成功刻起，必然有至少一个元素
        return 1;
    }
    if (LTT_ArrayDeque_HasNext(IP) == 0) return 0;
    int CurrentIndex = (int)(Current - ArrayDeque->Array);
    IP->Current      = &ArrayDeque->Array[INC(CurrentIndex, ArrayDeque->Capacity)];
    return 1;
}

static void LTT_ArrayDeque_Reset(Iterator* const IP) { IP->Current = NULL; }

Iterator LTT_ArrayDeque_GetIterator(const ArrayDeque* const ArrayDeque)
{
    if (ArrayDeque->Size <= 0)
    {
        printf("队列为空\n");
        return Iterator_NULL;
    }
    return ArrayDeque->Iterator;
}

ArrayDeque* LTT_ArrayDeque_New(const size_t DataSize, Equals_Function Equals)
{
    // 初始化队列
    ArrayDeque* ArrayDequeP = (ArrayDeque*)malloc(sizeof(ArrayDeque));
    if (ArrayDequeP == NULL || (ArrayDequeP->Array = (void**)calloc(DEFAULT_ARRAYDEQUE_CAPACITY, sizeof(void*))) == NULL)
    {
        printf("队列初始化失败\n");
        return NULL;
    }
    ArrayDequeP->Head     = 0;
    ArrayDequeP->Tail     = 0;
    ArrayDequeP->Size     = 0;
    ArrayDequeP->DataSize = DataSize;
    ArrayDequeP->Capacity = DEFAULT_ARRAYDEQUE_CAPACITY;
    //初始化迭代器
    InitIterator(&ArrayDequeP->Iterator, ArrayDequeP, LTT_ArrayDeque_MoveNext, LTT_ArrayDeque_HasNext, LTT_ArrayDeque_Reset, LTT_ArrayDeque_BackToFirst);
    //用户可自定义的函数
    ArrayDequeP->Equals = Equals ? Equals : LTT_DEFAULT_Equals;
    return ArrayDequeP;
}

static int LTT_ArrayDeque_newCapacity(const int OldCapacity, const int Needed, const int jump)
{
    int MinCapacity;
    if ((MinCapacity = OldCapacity + Needed) - MAX_ARRAYDEQUE_CAPACITY > 0)
    {
        if (MinCapacity < 0)    // 溢出
        {
            printf("队列容量过大\n");
            return ERROR;
        }
        return INT_MAX;
    }
    if (Needed > jump) return MinCapacity;
    return (OldCapacity + jump - MAX_ARRAYDEQUE_CAPACITY < 0) ? OldCapacity + jump : MAX_ARRAYDEQUE_CAPACITY;
}

static Status LTT_ArrayDeque_Resize(ArrayDeque* const ArrayDeque, const int Needed)
{
    int OldCapacity = ArrayDeque->Capacity;
    int NewCapacity;
    int jump = (OldCapacity < 64) ? (OldCapacity + 2) : (OldCapacity >> 1);    // jump为增长量
    if (jump < Needed || (NewCapacity = OldCapacity + jump) - MAX_ARRAYDEQUE_CAPACITY > 0)
    {
        if ((NewCapacity = LTT_ArrayDeque_newCapacity(OldCapacity, Needed, jump)) == ERROR)
        {
            printf("队列扩容失败\n");
            return ERROR;
        }
    }
    void** EA = (void**)realloc(ArrayDeque->Array, NewCapacity * sizeof(void*));
    if (EA == NULL)
    {
        printf("队列扩容失败\n");
        return ERROR;
    }
    ArrayDeque->Array    = EA;
    ArrayDeque->Capacity = NewCapacity;
    int Head             = ArrayDeque->Head;
    int Tail             = ArrayDeque->Tail;
    if (Tail < Head || (Tail == Head && EA[Head] != NULL))
    {
        int NewSpace = NewCapacity - OldCapacity;
        memmove(&EA[Head + NewSpace], &EA[Head], (OldCapacity - Head) * sizeof(void*));
        for (int i = Head, to = (ArrayDeque->Head += NewSpace); i < to; i++) { EA[i] = NULL; }
    }
    return OK;
}

Status LTT_ArrayDeque_AddFirst(ArrayDeque* const ArrayDeque, void* const Data)
{
    if (Data == NULL)
    {
        printf("数据不能为空\n");
        return ERROR;
    }
    void** EA                                  = ArrayDeque->Array;
    int    Head                                = ArrayDeque->Head;
    int    Capacity                            = ArrayDeque->Capacity;
    EA[ArrayDeque->Head = DEC(Head, Capacity)] = Data;
    ++ArrayDeque->Size;
    if (ArrayDeque->Head == ArrayDeque->Tail)
    {
        if (!LTT_ArrayDeque_Resize(ArrayDeque, 1))
        {
            printf("队列扩容失败\n");
            return ERROR;
        }
    }
    return OK;
}

void* LTT_ArrayDeque_DeleteFirst(ArrayDeque* const ArrayDeque)
{
    int   Head     = ArrayDeque->Head;
    int   Capacity = ArrayDeque->Capacity;
    void* Data     = ArrayDeque->Array[Head];
    if (Data != NULL)
    {
        ArrayDeque->Array[Head] = NULL;
        ArrayDeque->Head        = INC(Head, Capacity);
        --ArrayDeque->Size;
    }
    return Data;
}

Status LTT_ArrayDeque_AddLast(ArrayDeque* const ArrayDeque, void* const Data)
{
    if (Data == NULL)
    {
        printf("数据不能为空\n");
        return ERROR;
    }
    void** EA       = ArrayDeque->Array;
    int    Tail     = ArrayDeque->Tail;
    int    Capacity = ArrayDeque->Capacity;
    EA[Tail]        = Data;
    ++ArrayDeque->Size;
    if (ArrayDeque->Head == (ArrayDeque->Tail = INC(Tail, Capacity))) LTT_ArrayDeque_Resize(ArrayDeque, 1);
    return OK;
}

void* LTT_ArrayDeque_DeleteLast(ArrayDeque* const ArrayDeque)
{
    int   Tail     = ArrayDeque->Tail;
    int   Capacity = ArrayDeque->Capacity;
    void* Data     = ArrayDeque->Array[Tail = DEC(Tail, Capacity)];
    if (Data != NULL)
    {
        ArrayDeque->Array[ArrayDeque->Tail = Tail] = NULL;
        --ArrayDeque->Size;
    }
    return Data;
}

void* LTT_ArrayDeque_GetFirst(const ArrayDeque* const ArrayDeque)
{
    void* Data = ArrayDeque->Array[ArrayDeque->Head];
    if (Data == NULL)
    {
        printf("队列为空\n");
        return NULL;
    }
    return Data;
}

void* LTT_ArrayDeque_GetLast(const ArrayDeque* const ArrayDeque)
{
    void* Data = ArrayDeque->Array[DEC(ArrayDeque->Tail, ArrayDeque->Capacity)];
    if (Data == NULL)
    {
        printf("队列为空\n");
        return NULL;
    }
    return Data;
}

bool LTT_ArrayDeque_Contains(const ArrayDeque* const ArrayDeque, void* const Data)
{
    if (Data != NULL)
    {
        void** EA = ArrayDeque->Array;
        for (int i = ArrayDeque->Head, end = ArrayDeque->Tail, to = (i <= end) ? end : ArrayDeque->Capacity;; i = 0, to = end)
        {
            for (; i < to; i++)
            {
                if (Data == EA[i] || ArrayDeque->Equals(Data, EA[i], ArrayDeque->DataSize)) return true;
            }
            if (to == end) break;
        }
    }
    return false;
}

bool LTT_ArrayDeque_IsEmpty(const ArrayDeque* const ArrayDeque) { return ArrayDeque->Head == ArrayDeque->Tail; }

int LTT_ArrayDeque_GetSize(const ArrayDeque* const ArrayDeque) { return ArrayDeque->Size; }

void LTT_ArrayDeque_Clear(ArrayDeque* const ArrayDeque)
{
    void** EA = ArrayDeque->Array;
    for (int i = ArrayDeque->Head, end = ArrayDeque->Tail, to = (i <= end) ? end : ArrayDeque->Capacity;; i = 0, to = end)
    {
        for (; i < to; i++) EA[i] = NULL;
        if (to == end) break;
    }
    ArrayDeque->Head = ArrayDeque->Tail = 0;
}

void LTT_ArrayDeque_Destroy(ArrayDeque** ArrayDeque)
{
    free((*ArrayDeque)->Array);
    free(*ArrayDeque);
    *ArrayDeque = NULL;
}