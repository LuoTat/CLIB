#include "ArrayList.h"
#include <limits.h>
#include "../Iterator/Iterator.h"
#include "../Predefined/Predefined.h"

#define DEFAULT_ARRAYLIST_CAPACITY 16
#define MAX_ARRAYLIST_CAPACITY     (INT_MAX - 8)

typedef struct _ArrayList
{
    void**          Array;       //数组,装的是元素的地址
    int             Capacity;    //数组容量
    int             Size;        //当前元素个数
    size_t          DataSize;
    Equals_Function Equals;      // 比较函数
    Iterator        Iterator;    // 迭代器
} _ArrayList;

void* LTT_ArrayList_GetCurrent(const Iterator* const IP) { return *(void**)IP->Current; }

inline static void LTT_ArrayList_BackToFirst(Iterator* const IP)
{
    ArrayList* ArrayList = IP->Collection;
    IP->Current          = ArrayList->Array;
}

inline static int LTT_ArrayList_HasNext(const Iterator* const IP)
{
    ArrayList* ArrayList = IP->Collection;
    return IP->Current != &ArrayList->Array[ArrayList->Size - 1];
}

static int LTT_ArrayList_MoveNext(Iterator* const IP)
{
    if (IP->Current == NULL)
    {
        LTT_ArrayList_BackToFirst(IP);    //迭代器自初始化成功刻起，必然有至少一个元素
        return 1;
    }
    if (LTT_ArrayList_HasNext(IP) == 0) return 0;
    IP->Current += sizeof(void*);
    return 1;
}

static void LTT_ArrayList_Reset(Iterator* const IP) { IP->Current = NULL; }

Iterator LTT_ArrayList_GetIterator(const ArrayList* const ArrayList)
{
    if (ArrayList->Size <= 0)
    {
        printf("数组为空\n");
        return Iterator_NULL;
    }
    return ArrayList->Iterator;
}

ArrayList* LTT_ArrayList_New(const size_t DataSize, Equals_Function Equals)
{
    ArrayList* ArrayListP = (ArrayList*)malloc(sizeof(ArrayList));
    if (ArrayListP == NULL)
    {
        printf("内存分配失败!\n");
        return NULL;
    }
    ArrayListP->Array    = NULL;
    ArrayListP->Size     = 0;
    ArrayListP->DataSize = DataSize;
    ArrayListP->Capacity = 0;
    //初始化迭代器
    InitIterator(&ArrayListP->Iterator, ArrayListP, LTT_ArrayList_MoveNext, LTT_ArrayList_HasNext, LTT_ArrayList_Reset, LTT_ArrayList_BackToFirst);
    //用户可自定义的函数
    ArrayListP->Equals = Equals ? Equals : LTT_DEFAULT_Equals;
    return ArrayListP;
}

static int LTT_ArrayList_newCapacity(ArrayList* const ArrayList, const int OldCapacity, const int MinGrowth, const int PrefGrowth)
{
    int PrefLength = OldCapacity + ((MinGrowth < PrefGrowth) ? PrefGrowth : MinGrowth);
    if (0 < PrefLength && PrefLength <= MAX_ARRAYLIST_CAPACITY) return PrefLength;
    else
    {
        int MinLength = OldCapacity + MinGrowth;
        if (MinLength < 0) return 0;    //溢出
        else if (MinLength <= MAX_ARRAYLIST_CAPACITY) return MAX_ARRAYLIST_CAPACITY;
        else return MinLength;
    }
}

static Status LTT_ArrayList_Resize(ArrayList* const ArrayList, const int MinCapacity)
{
    int OldCapacity = ArrayList->Capacity;
    if (OldCapacity > 0 || ArrayList->Array != NULL)
    {
        int NewCapacity = LTT_ArrayList_newCapacity(ArrayList, OldCapacity, MinCapacity - OldCapacity, OldCapacity >> 1);
        if (NewCapacity == 0) return ERROR;
        void** EA = NULL;
        if (!(EA = (void**)realloc(ArrayList->Array, NewCapacity * sizeof(void*))))
        {
            printf("数组扩容失败！\n");
            return ERROR;
        }
        ArrayList->Array    = EA;
        ArrayList->Capacity = NewCapacity;
    }
    else
    {
        if ((ArrayList->Array = (void**)calloc(DEFAULT_ARRAYLIST_CAPACITY, sizeof(void*))) == NULL)
        {
            printf("数组初始化失败！\n");
            return ERROR;
        }
        ArrayList->Capacity = DEFAULT_ARRAYLIST_CAPACITY;
    }
    return OK;
}

Status LTT_ArrayList_AddLast(ArrayList* const ArrayList, void* const Data)
{
    if (ArrayList->Size == ArrayList->Capacity)
    {
        if (LTT_ArrayList_Resize(ArrayList, ArrayList->Size + 1) == ERROR)
        {
            printf("数组插入失败！\n");
            return ERROR;
        }
    }
    ArrayList->Array[ArrayList->Size] = Data;
    ++ArrayList->Size;
    return OK;
}

static bool LTT_ArrayList_CheckIndex(const ArrayList* const ArrayList, const int Index)
{
    if (Index < 0 || Index >= ArrayList->Size)
    {
        printf("数组下标越界！\n");
        return false;
    }
    return true;
}

Status LTT_ArrayList_AddIndex(ArrayList* const ArrayList, const int Index, void* const Data)
{
    if (!LTT_ArrayList_CheckIndex(ArrayList, Index)) return ERROR;
    if (ArrayList->Size == ArrayList->Capacity)
    {
        if (LTT_ArrayList_Resize(ArrayList, ArrayList->Size + 1) == ERROR)
        {
            printf("数组插入失败！\n");
            return ERROR;
        }
    }
    //将Index后的元素向后移动一位
    memmove(ArrayList->Array + Index + 1, ArrayList->Array + Index, (ArrayList->Size - Index) * sizeof(void*));
    ArrayList->Array[Index] = Data;
    ++ArrayList->Size;
    return OK;
}

void* LTT_ArrayList_SetIndex(ArrayList* const ArrayList, const int Index, void* const Data)
{
    if (!LTT_ArrayList_CheckIndex(ArrayList, Index)) return NULL;
    void* OldData           = ArrayList->Array[Index];
    ArrayList->Array[Index] = Data;
    return OldData;
}

void* LTT_ArrayList_DeleteIndex(ArrayList* const ArrayList, const int Index)
{
    if (!LTT_ArrayList_CheckIndex(ArrayList, Index)) return NULL;
    void* OldData = ArrayList->Array[Index];
    //将Index后的元素向前移动一位
    memmove(ArrayList->Array + Index, ArrayList->Array + Index + 1, (ArrayList->Size - Index - 1) * sizeof(void*));
    --ArrayList->Size;
    return OldData;
}

bool LTT_ArrayList_DeleteData(ArrayList* const ArrayList, const void* const Data)
{
    void** EA   = ArrayList->Array;
    int    Size = ArrayList->Size;
    int    i    = 0;
    if (Data == NULL)
    {
        for (; i < Size; ++i)
        {
            if (EA[i] == NULL) goto A;
        }
    }
    else
    {
        for (; i < Size; ++i)
        {
            if (Data == EA[i] || ArrayList->Equals(Data, EA[i], ArrayList->DataSize)) goto A;
        }
    }
    return false;
A:
    memmove(EA + i, EA + i + 1, (Size - i - 1) * sizeof(void*));
    --ArrayList->Size;
    return true;
}

void* LTT_ArrayList_GetData(const ArrayList* const ArrayList, const int Index)
{
    if (!LTT_ArrayList_CheckIndex(ArrayList, Index)) return NULL;
    return ArrayList->Array[Index];
}

int LTT_ArrayList_IndexOf(const ArrayList* const ArrayList, const void* const Data)
{
    void** EA   = ArrayList->Array;
    int    Size = ArrayList->Size;
    int    i    = 0;
    if (Data == NULL)
    {
        for (; i < Size; ++i)
        {
            if (EA[i] == NULL) return i;
        }
    }
    else
    {
        for (; i < Size; ++i)
        {
            if (Data == EA[i] || ArrayList->Equals(Data, EA[i], ArrayList->DataSize)) return i;
        }
    }
    return -1;
}

int LTT_ArrayList_LastIndexOf(const ArrayList* const ArrayList, const void* const Data)
{
    void** EA = ArrayList->Array;
    int    i  = ArrayList->Size - 1;
    if (Data == NULL)
    {
        for (; i >= 0; --i)
        {
            if (EA[i] == NULL) return i;
        }
    }
    else
    {
        for (; i >= 0; --i)
        {
            if (Data == EA[i] || ArrayList->Equals(Data, EA[i], ArrayList->DataSize)) return i;
        }
    }
    return -1;
}

bool LTT_ArrayList_Contains(const ArrayList* const ArrayList, const void* const Data) { return LTT_ArrayList_IndexOf(ArrayList, Data) >= 0; }

bool LTT_ArrayList_IsEmpty(const ArrayList* const ArrayList) { return ArrayList->Size == 0; }

int LTT_ArrayList_GetSize(const ArrayList* const ArrayList) { return ArrayList->Size; }

void LTT_ArrayList_Clear(ArrayList* const ArrayList)
{
    void** EA = ArrayList->Array;
    for (int to = ArrayList->Size, i = ArrayList->Size = 0; i < to; ++i) { EA[i] = NULL; }
}

void LTT_ArrayList_Destroy(ArrayList** ArrayList)
{
    free((*ArrayList)->Array);
    free(*ArrayList);
    *ArrayList = NULL;
}