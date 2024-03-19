#include "ArrayQueue.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Iterator/Iterator.h"
#include "../Predefined/Predefined.h"

#define DEFAULT_ARRAYQUEUE_CAPACITY (16 + 1)
#define MAX_ARRAYQUEUE_CAPACITY     (INT_MAX - 8)
#define INC(Head, Capacity)         ((Head + 1LL) % Capacity)                 // 同余类+1
#define DEC(Head, Capacity)         ((Head + (Capacity - 1LL)) % Capacity)    // 同余类-1

typedef struct _ArrayQueue
{
    void**          Array;                                                    // 数组,装的是元素的地址
    int             Head;                                                     // 头部元素的下标
    int             Tail;                                                     // 尾部元素的下标
    int             Size;                                                     // 当前元素个数
    int             Capacity;                                                 // 数组容量
    size_t          DataSize;
    Iterator        Iterator;                                                 // 迭代器
    Equals_Function Equals;                                                   // 比较函数
} _ArrayQueue;

void* LTT_ArrayQueue_GetCurrent(const Iterator* const IP) { return *(void**)IP->Current; }

inline static void LTT_ArrayQueue_BackToFirst(Iterator* const IP)
{
    ArrayQueue* ArrayQueue = IP->Collection;
    IP->Current            = &ArrayQueue->Array[ArrayQueue->Head];
}

inline static int LTT_ArrayQueue_HasNext(const Iterator* const IP)
{
    ArrayQueue* ArrayQueue = IP->Collection;
    return IP->Current != &ArrayQueue->Array[ArrayQueue->Tail - 1];
}

static int LTT_ArrayQueue_MoveNext(Iterator* const IP)
{
    ArrayQueue* ArrayQueue = IP->Collection;
    void**      Current    = IP->Current;
    if (Current == NULL)
    {
        LTT_ArrayQueue_BackToFirst(IP);    //迭代器自初始化成功刻起，必然有至少一个元素
        return 1;
    }
    if (LTT_ArrayQueue_HasNext(IP) == 0) return 0;
    int CurrentIndex = Current - ArrayQueue->Array;
    IP->Current      = &ArrayQueue->Array[INC(CurrentIndex, ArrayQueue->Capacity)];
    return 1;
}

static void LTT_ArrayQueue_Reset(Iterator* const IP) { IP->Current = NULL; }

Iterator LTT_ArrayQueue_GetIterator(const ArrayQueue* const ArrayQueue)
{
    if (ArrayQueue->Size <= 0)
    {
        printf("队列为空\n");
        return Iterator_NULL;
    }
    return ArrayQueue->Iterator;
}

ArrayQueue* LTT_ArrayQueue_New(const size_t DataSize, Equals_Function Equals)
{
    // 初始化队列
    ArrayQueue* ArrayQueueP = (ArrayQueue*)malloc(sizeof(ArrayQueue));
    if (ArrayQueueP == NULL || (ArrayQueueP->Array = (void**)calloc(DEFAULT_ARRAYQUEUE_CAPACITY, sizeof(void*))) == NULL)
    {
        printf("队列初始化失败\n");
        return NULL;
    }
    ArrayQueueP->Head     = 0;
    ArrayQueueP->Tail     = 0;
    ArrayQueueP->Size     = 0;
    ArrayQueueP->DataSize = DataSize;
    ArrayQueueP->Capacity = DEFAULT_ARRAYQUEUE_CAPACITY;
    //初始化迭代器
    InitIterator(&ArrayQueueP->Iterator, ArrayQueueP, LTT_ArrayQueue_MoveNext, LTT_ArrayQueue_HasNext, LTT_ArrayQueue_Reset, LTT_ArrayQueue_BackToFirst);
    //用户可自定义的函数
    ArrayQueueP->Equals = Equals ? Equals : LTT_DEFAULT_Equals;
    return ArrayQueueP;
}

static int LTT_ArrayQueue_newCapacity(const int OldCapacity, const int Needed, const int jump)
{
    int MinCapacity;
    if ((MinCapacity = OldCapacity + Needed) - MAX_ARRAYQUEUE_CAPACITY > 0)
    {
        if (MinCapacity < 0)    // 溢出
        {
            printf("队列容量过大\n");
            return ERROR;
        }
        return INT_MAX;
    }
    if (Needed > jump) return MinCapacity;
    return (OldCapacity + jump - MAX_ARRAYQUEUE_CAPACITY < 0) ? OldCapacity + jump : MAX_ARRAYQUEUE_CAPACITY;
}

static Status LTT_ArrayQueue_Resize(ArrayQueue* const ArrayQueue, const int Needed)
{
    int OldCapacity = ArrayQueue->Capacity;
    int NewCapacity;
    int jump = (OldCapacity < 64) ? (OldCapacity + 2) : (OldCapacity >> 1);    // jump为增长量
    if (jump < Needed || (NewCapacity = OldCapacity + jump) - MAX_ARRAYQUEUE_CAPACITY > 0)
    {
        if ((NewCapacity = LTT_ArrayQueue_newCapacity(OldCapacity, Needed, jump)) == ERROR)
        {
            printf("队列扩容失败\n");
            return ERROR;
        }
    }
    void** EA = (void**)realloc(ArrayQueue->Array, NewCapacity * sizeof(void*));
    if (EA == NULL)
    {
        printf("队列扩容失败\n");
        return ERROR;
    }
    ArrayQueue->Array    = EA;
    ArrayQueue->Capacity = NewCapacity;
    int Head             = ArrayQueue->Head;
    int Tail             = ArrayQueue->Tail;
    if (Tail < Head || (Tail == Head && EA[Head] != NULL))
    {
        int NewSpace = NewCapacity - OldCapacity;
        memmove(&EA[Head + NewSpace], &EA[Head], (OldCapacity - Head) * sizeof(void*));
        for (int i = Head, to = (ArrayQueue->Head += NewSpace); i < to; i++) { EA[i] = NULL; }
    }
    return OK;
}

Status LTT_ArrayQueue_Push(ArrayQueue* const ArrayQueue, void* const Data)
{
    if (Data == NULL)
    {
        printf("数据不能为空\n");
        return ERROR;
    }
    void** EA       = ArrayQueue->Array;
    int    Tail     = ArrayQueue->Tail;
    int    Capacity = ArrayQueue->Capacity;
    EA[Tail]        = Data;
    ++ArrayQueue->Size;
    if (ArrayQueue->Head == (ArrayQueue->Tail = INC(Tail, Capacity))) LTT_ArrayQueue_Resize(ArrayQueue, 1);
    return OK;
}

void* LTT_ArrayQueue_Pop(ArrayQueue* const ArrayQueue)
{
    int   Head     = ArrayQueue->Head;
    int   Capacity = ArrayQueue->Capacity;
    void* Data     = ArrayQueue->Array[Head];
    if (Data != NULL)
    {
        ArrayQueue->Array[Head] = NULL;
        ArrayQueue->Head        = INC(Head, Capacity);
        --ArrayQueue->Size;
    }
    return Data;
}

void* LTT_ArrayQueue_Peek(const ArrayQueue* const ArrayQueue)
{
    void* Data = ArrayQueue->Array[DEC(ArrayQueue->Tail, ArrayQueue->Capacity)];
    if (Data == NULL)
    {
        printf("队列为空\n");
        return NULL;
    }
    return Data;
}

bool LTT_ArrayQueue_Contains(const ArrayQueue* const ArrayQueue, void* const Data)
{
    if (Data != NULL)
    {
        void** EA = ArrayQueue->Array;
        for (int i = ArrayQueue->Head, end = ArrayQueue->Tail, to = (i <= end) ? end : ArrayQueue->Capacity;; i = 0, to = end)
        {
            for (; i < to; i++)
            {
                if (Data == EA[i] || ArrayQueue->Equals(Data, EA[i], ArrayQueue->DataSize)) return true;
            }
            if (to == end) break;
        }
    }
    return false;
}

bool LTT_ArrayQueue_IsEmpty(const ArrayQueue* const ArrayQueue) { return ArrayQueue->Size == 0; }

int LTT_ArrayQueue_GetSize(const ArrayQueue* const ArrayQueue) { return ArrayQueue->Size; }

void LTT_ArrayQueue_Clear(ArrayQueue* const ArrayQueue)
{
    void** EA = ArrayQueue->Array;
    for (int i = ArrayQueue->Head, end = ArrayQueue->Tail, to = (i <= end) ? end : ArrayQueue->Capacity;; i = 0, to = end)
    {
        for (; i < to; i++) EA[i] = NULL;
        if (to == end) break;
    }
    ArrayQueue->Head = ArrayQueue->Tail = 0;
}

void LTT_ArrayQueue_Destroy(ArrayQueue* ArrayQueue)
{
    free(ArrayQueue->Array);
    free(ArrayQueue);
    ArrayQueue = NULL;
}