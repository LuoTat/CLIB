#include "ArrayStack.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "Predefined.h"


#define DEFAULT_ARRAYSTACK_CAPACITY  (16)

#define SOFT_MAX_ARRAYSTACK_CAPACITY (INT_MAX - 8)

typedef struct _ArrayStack
{
    void**          Array;       // 数组,装的是元素的地址
    int             Size;        // 当前元素个数
    int             Capacity;    // 数组容量
    size_t          DataSize;
    Iterator        Iterator;    // 迭代器
    Equals_Function Equals;      // 比较函数
} _ArrayStack;

void* LTT_ArrayStack_GetCurrent(const Iterator* const IP) { return *(void**)IP->Current; }

inline static void LTT_ArrayStack_BackToFirst(Iterator* const IP)
{
    ArrayStack* ArrayStack = IP->Collection;
    IP->Current            = ArrayStack->Array;
}

inline static int LTT_ArrayStack_HasNext(const Iterator* const IP)
{
    ArrayStack* ArrayStack = IP->Collection;
    return IP->Current != &ArrayStack->Array[ArrayStack->Size - 1];
}

static int LTT_ArrayStack_MoveNext(Iterator* const IP)
{
    if (IP->Current == NULL)
    {
        LTT_ArrayStack_BackToFirst(IP);    // 迭代器自初始化成功刻起，必然有至少一个元素
        return 1;
    }
    if (LTT_ArrayStack_HasNext(IP) == 0) return 0;
    IP->Current += sizeof(void*);
    return 1;
}

static void LTT_ArrayStack_Reset(Iterator* const IP) { IP->Current = NULL; }

Iterator LTT_ArrayStack_GetIterator(const ArrayStack* const ArrayStack)
{
    if (ArrayStack->Size <= 0)
    {
        printf("栈为空\n");
        return Iterator_NULL;
    }
    return ArrayStack->Iterator;
}

ArrayStack* LTT_ArrayStack_New(const size_t DataSize, Equals_Function Equals)
{
    // 初始化队列
    ArrayStack* ArrayStackP = (ArrayStack*)malloc(sizeof(ArrayStack));
    if (ArrayStackP == NULL || (ArrayStackP->Array = (void**)calloc(DEFAULT_ARRAYSTACK_CAPACITY, sizeof(void*))) == NULL)
    {
        printf("栈初始化失败\n");
        return NULL;
    }
    ArrayStackP->Size     = 0;
    ArrayStackP->DataSize = DataSize;
    ArrayStackP->Capacity = DEFAULT_ARRAYSTACK_CAPACITY;
    // 初始化迭代器
    InitIterator(&ArrayStackP->Iterator, ArrayStackP, LTT_ArrayStack_MoveNext, LTT_ArrayStack_HasNext, LTT_ArrayStack_Reset, LTT_ArrayStack_BackToFirst);
    // 用户可自定义的函数
    ArrayStackP->Equals = Equals ? Equals : LTT_DEFAULT_Equals;
    return ArrayStackP;
}

inline static int LTT_ArrayStack_newCapacity(const int OldCapacity, const int MinGrowth, const int PrefGrowth)
{
    int PrefCapacity = OldCapacity + ((MinGrowth > PrefGrowth) ? MinGrowth : PrefGrowth);
    if (PrefCapacity > 0 && PrefCapacity <= SOFT_MAX_ARRAYSTACK_CAPACITY) return PrefCapacity;
    else
    {
        int MinCapacity = OldCapacity + MinGrowth;
        if (MinCapacity < 0) return 0;
        else if (MinCapacity <= SOFT_MAX_ARRAYSTACK_CAPACITY) return SOFT_MAX_ARRAYSTACK_CAPACITY;
        else return MinCapacity;
    }
}

Status LTT_ArrayStack_Resize(ArrayStack* const ArrayStack, const int MinCapacity)
{
    int OldCapacity = ArrayStack->Capacity;
    int NewCapacity;
    if (OldCapacity > 0 || ArrayStack->Array != NULL)
    {
        NewCapacity = LTT_ArrayStack_newCapacity(OldCapacity, MinCapacity - OldCapacity, OldCapacity >> 1);
        if (NewCapacity == 0) return ERROR;
        void** EA = (void**)realloc(ArrayStack->Array, NewCapacity * sizeof(void*));
        if (EA == NULL) return ERROR;
        ArrayStack->Array    = EA;
        ArrayStack->Capacity = NewCapacity;
        return OK;
    }
    else
    {
        NewCapacity = (MinCapacity > DEFAULT_ARRAYSTACK_CAPACITY) ? MinCapacity : DEFAULT_ARRAYSTACK_CAPACITY;
        void** EA   = (void**)malloc(NewCapacity * sizeof(void*));
        if (EA == NULL) return ERROR;
        ArrayStack->Array    = EA;
        ArrayStack->Capacity = NewCapacity;
        return OK;
    }
}

Status LTT_ArrayStack_Push(ArrayStack* const ArrayStack, void* const Data)
{
    if (Data == NULL)
    {
        printf("数据不能为空\n");
        return ERROR;
    }
    if (unlikely(ArrayStack->Size == ArrayStack->Capacity))
    {
        if (LTT_ArrayStack_Resize(ArrayStack, ArrayStack->Size + 1) == 0) return ERROR;
    }
    ArrayStack->Array[ArrayStack->Size++] = Data;
    return OK;
}

void* LTT_ArrayStack_Pop(ArrayStack* const ArrayStack) { return ArrayStack->Array[--ArrayStack->Size]; }

void* LTT_ArrayStack_Peek(const ArrayStack* const ArrayStack) { return ArrayStack->Array[ArrayStack->Size - 1]; }

bool LTT_ArrayStack_Contains(const ArrayStack* const ArrayStack, void* const Data)
{
    if (Data != NULL)
    {
        void** EA = ArrayStack->Array;
        for (int i = 0; i < ArrayStack->Size; ++i)
        {
            if (Data == EA[i] || ArrayStack->Equals(Data, EA[i], ArrayStack->DataSize)) return true;
        }
    }
    return false;
}

bool LTT_ArrayStack_IsEmpty(const ArrayStack* const ArrayStack) { return ArrayStack->Size == 0; }

int LTT_ArrayStack_GetSize(const ArrayStack* const ArrayStack) { return ArrayStack->Size; }

void LTT_ArrayStack_Clear(ArrayStack* const ArrayStack)
{
    void** EA = ArrayStack->Array;
    for (int i = 0; i < ArrayStack->Size; ++i) { EA[i] = NULL; }
    ArrayStack->Size = 0;
}

void LTT_ArrayStack_Destroy(ArrayStack** ArrayStack)
{
    free((*ArrayStack)->Array);
    free(*ArrayStack);
    *ArrayStack = NULL;
}