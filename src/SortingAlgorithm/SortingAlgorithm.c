#include "SortingAlgorithm.h"
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Predefined/Predefined.h"

//用来作为许多排序算法的临时存储空间
static char  Temp[MAX_SIZEOFELEMENTS];
static char* Temp_Merge;

#if SWAPMODE == 1
    #define SWAP(a, b, DataSize)            \
        do {                                \
            size_t __DataSize = (DataSize); \
            char * __a = (a), *__b = (b);   \
            do {                            \
                char __tmp = *__a;          \
                *__a++     = *__b;          \
                *__b++     = __tmp;         \
            }                               \
            while (--__DataSize > 0);       \
        }                                   \
        while (0)
#elif SWAPMODE == 2
    #define SWAP(a, b, DataSize)                       \
        do {                                           \
            size_t __DataSize = (DataSize);            \
            size_t n          = __DataSize / 4;        \
            short  m_1        = __DataSize % 4;        \
            short  m_2 = m_1, m_3 = m_1;               \
            int *  __a = (int*)(a), *__b = (int*)(b);  \
            int    Temp_int;                           \
            while (n > 0)                              \
            {                                          \
                Temp_int = *__a;                       \
                *__a++   = *__b;                       \
                *__b++   = Temp_int;                   \
                --n;                                   \
            }                                          \
            char* a_char      = (char*)__a;            \
            char* b_char_Last = ((char*)__b) + m_1;    \
            char  Temp[3];                             \
            switch (m_1)                               \
            {                                          \
                case 3 : Temp[--m_1] = *a_char++;      \
                case 2 : Temp[--m_1] = *a_char++;      \
                case 1 : Temp[--m_1] = *a_char++;      \
                case 0 :;                              \
            }                                          \
            switch (m_2)                               \
            {                                          \
                case 3 : *a_char-- = *b_char_Last--;   \
                case 2 : *a_char-- = *b_char_Last--;   \
                case 1 : *a_char-- = *b_char_Last--;   \
                case 0 :;                              \
            }                                          \
            switch (m_3)                               \
            {                                          \
                case 3 : *b_char_Last++ = Temp[--m_3]; \
                case 2 : *b_char_Last++ = Temp[--m_3]; \
                case 1 : *b_char_Last++ = Temp[--m_3]; \
                case 0 :;                              \
            }                                          \
        }                                              \
        while (0)
#endif


#define MAX_THRESH  4
#define S_Threshold 16
#define Bucket_NUM  8

typedef struct
{
    char* Low;
    char* High;
} stack_node;

#define STACK_SIZE          (CHAR_BIT * sizeof(size_t))
#define PUSH(LeftP, RightP) ((void)((Top->Low = (LeftP)), (Top->High = (RightP)), ++Top))
#define POP(LeftP, RightP)  ((void)(--Top, (LeftP = Top->Low), (RightP = Top->High)))
#define STACK_NOT_EMPTY     (Stack < Top)

static void MoveMidToFirst(char* Result, char* a, char* b, char* c, size_t SizeOfElements, CompareFunction Comparator)
{
    if (Comparator(a, b) < 0)
    {
        if (Comparator(b, c) < 0) SWAP(Result, b, SizeOfElements);
        else if (Comparator(a, c) < 0) SWAP(Result, c, SizeOfElements);
        else SWAP(Result, a, SizeOfElements);
    }
    else if (Comparator(a, c) < 0) SWAP(Result, a, SizeOfElements);
    else if (Comparator(b, c) < 0) SWAP(Result, c, SizeOfElements);
    else SWAP(Result, b, SizeOfElements);
}

inline static unsigned char lg2(size_t n)
{
    unsigned char log_val = 0;
    while (n >>= 1) { ++log_val; }
    return log_val;
}

/*
	此插入排序是一个通过每次都将比第一个元素小的元素移动到第一个的位置
	从而避免边界检查(这是一个常规的插入排序)
*/

void BinaryInsertionSort(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator)
{
    if (NumOfElements <= 0) return;
    //void* Temp = (void*)malloc(SizeOfElements);
    for (int i = 1; i < NumOfElements; ++i)
    {
        memcpy(Temp, Base + i * SizeOfElements, SizeOfElements);    //记录当前的值
        int Position;
        int Low  = 0;
        int High = i - 1;
        while (Low <= High)
        {
            int Mid   = (Low + High) >> 1;
            int Delta = Comparator(Temp, Base + Mid * SizeOfElements);
            if (Delta == 0)
            {
                Position = Mid + 1;
                goto A;
            }
            else if (Delta < 0) High = Mid - 1;
            else Low = Mid + 1;
        }
        Position = High + 1;
    A:
        for (int j = i - 1; j >= Position; --j) memcpy(Base + (j + 1) * SizeOfElements, Base + j * SizeOfElements, SizeOfElements);
        memcpy(Base + Position * SizeOfElements, Temp, SizeOfElements);
    }
    //free(Temp);
}

static int HibbardStepArray[31] = {2147483647, 1073741823, 536870911, 268435455, 134217727, 67108863, 33554431, 16777215, 8388607, 4194303, 2097151, 1048575, 524287, 262143, 131071, 65535, 32767, 16383, 8191, 4095, 2047, 1023, 511, 255, 127, 63, 31, 15, 7, 3, 1};

static int* GetHibbardStepArray(int Length)
{
    for (int i = 0;; ++i)
    {
        if (HibbardStepArray[i] <= Length) { return &HibbardStepArray[i]; }
    }
}

void ShellInsertionSort_Hibbard(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator)
{
    int* GapArray = GetHibbardStepArray(NumOfElements);
    //void* Temp     = (void*)malloc(SizeOfElements);
    for (int i = 0;; ++i)
    {
        int Gap = GapArray[i];
        for (int j = Gap; j < NumOfElements; ++j)
        {
            memcpy(Temp, Base + j * SizeOfElements, SizeOfElements);
            int Position = j - Gap;
            while (Position >= 0 && Comparator(Base + Position * SizeOfElements, Temp) > 0)    //向前遇到更大的就继续向前移
            {
                memcpy(Base + (Position + Gap) * SizeOfElements, Base + Position * SizeOfElements, SizeOfElements);
                Position -= Gap;
            }
            memcpy(Base + (Position + Gap) * SizeOfElements, Temp, SizeOfElements);
        }
        if (Gap == 1) break;
    }
    //free(Temp);
}

static int SedgewickStepArray[14] = {603906049, 150958081, 37730305, 9427969, 2354689, 587521, 146305, 36289, 8929, 2161, 505, 109, 19, 1};

static int* GetSedgewickStepArray(int Length)
{
    for (int i = 0;; ++i)
    {
        if (SedgewickStepArray[i] <= Length) { return &HibbardStepArray[i]; }
    }
}

void ShellInsertionSort_Sedgewick(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator)
{
    int* GapArray = GetSedgewickStepArray(NumOfElements);
    //void* Temp     = (void*)malloc(SizeOfElements);
    for (int i = 0;; ++i)
    {
        int Gap = GapArray[i];
        for (int j = Gap; j < NumOfElements; ++j)
        {
            memcpy(Temp, Base + j * SizeOfElements, SizeOfElements);
            int Position = j - Gap;
            while (Position >= 0 && Comparator(Base + Position * SizeOfElements, Temp) > 0)    //向前遇到更大的就继续向前移
            {
                memcpy(Base + (Position + Gap) * SizeOfElements, Base + Position * SizeOfElements, SizeOfElements);
                Position -= Gap;
            }
            memcpy(Base + (Position + Gap) * SizeOfElements, Temp, SizeOfElements);
        }
        if (Gap == 1) break;
    }
    //free(Temp);
}

void BubbleSort(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator)
{
    for (int i = 0; i < NumOfElements - 1; ++i)            //一共要排序NumOfElements-1次
    {
        bool Ordered = true;
        for (int j = 0; j < NumOfElements - 1 - i; j++)    //选出该趟排序的最大值往后移动
        {
            if (Comparator(Base + j * SizeOfElements, Base + (j + 1) * SizeOfElements) > 0)
            {
                SWAP(Base + j * SizeOfElements, Base + (j + 1) * SizeOfElements, SizeOfElements);
                Ordered = false;                           //只要有发生了交换，Ordered就置为false
            }
        }
        if (Ordered) break;
    }
}

void BubbleSort_Fast(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator)
{
    bool Ordered;                                  //记录当前数组是否已经有序
    int  HighPos = NumOfElements - 1;              //用来记录大数最后一次交换的位置
    int  LowPos  = 0;                              //用来记录小数最后一次交换的位置
    for (int i = 0; i < NumOfElements - 1; ++i)    //确定排序趟数
    {
        Ordered         = true;
        //正向寻找最大值
        int TempHighPos = HighPos;
        for (int j = LowPos; j < HighPos; ++j)
        {
            if (Comparator(Base + j * SizeOfElements, Base + (j + 1) * SizeOfElements) > 0)
            {
                SWAP(Base + j * SizeOfElements, Base + (j + 1) * SizeOfElements, SizeOfElements);    //交换
                Ordered     = false;                                                                 //只要有发生了交换，Ordered就置为false
                TempHighPos = j;                                                                     //记录最后一次交换的位置
            }
        }
        if (Ordered) break;
        HighPos        = TempHighPos;
        //反向寻找最小值
        int TempLowPos = LowPos;
        for (int j = HighPos; j > LowPos; --j)
        {
            if (Comparator(Base + j * SizeOfElements, Base + (j - 1) * SizeOfElements) < 0)
            {
                SWAP(Base + j * SizeOfElements, Base + (j - 1) * SizeOfElements, SizeOfElements);    //交换
                Ordered    = false;                                                                  //只要有发生了交换，Ordered就置为false
                TempLowPos = j;
            }
        }
        if (Ordered) break;
        LowPos = TempLowPos;
    }
}

/*
    这个AdjustHeap用来将以RootIndex为索引为根节点的子树的大顶堆化
*/
static void AdjustHeap(char* Base, int RootIndex, int Length, void* TempValue, size_t SizeOfElements, CompareFunction Comparator)
{
    int SecondChild = RootIndex;                                                                                              //记录子树的根的子节点
    while (SecondChild < (Length - 1) / 2)                                                                                    //判断是否有右子节点
    {
        SecondChild = 2 * (SecondChild + 1);                                                                                  //更新SecondChild为当前节点的右子节点
        if (Comparator(Base + (SecondChild - 1) * SizeOfElements, Base + SecondChild * SizeOfElements) > 0) --SecondChild;    //如果左子节点大于右子节点，更新SecondChild为左子节点,以确保选择较大的子节点
        if (Comparator(TempValue, Base + SecondChild * SizeOfElements) > 0) { goto A; }                                       //如果其父节点大于其子节点，则直接返回
        memcpy(Base + RootIndex * SizeOfElements, Base + SecondChild * SizeOfElements, SizeOfElements);                       //将更大的子节点放到父节点的位置
        RootIndex = SecondChild;                                                                                              //更新根节点的索引
    }
    if ((Length & 1) == 0 && SecondChild == (Length - 2) / 2)                                                                 //如果范围长度是偶数且SecondChild刚好为最后一个非叶子节点的索引
    {
        SecondChild = 2 * (SecondChild + 1) - 1;                                                                              //更新SecondChild为最后一个节点的索引
        if (Comparator(TempValue, Base + SecondChild * SizeOfElements) > 0) { goto A; };                                      //如果其父节点大于其子节点，则直接返回
        memcpy(Base + RootIndex * SizeOfElements, Base + SecondChild * SizeOfElements, SizeOfElements);
        RootIndex = SecondChild;
        //将更大的子节点放到父节点的位置
    }
A:
    memcpy(Base + RootIndex * SizeOfElements, TempValue, SizeOfElements);
    return;
}

inline static void PopHeap(char* First, char* Last, char* Result, size_t SizeOfElements, CompareFunction Comparator)
{
    //此函数的作用是将Result与Last,然后将其大顶堆化
    memcpy(Temp, Result, SizeOfElements);    //将Result存储起来
    memcpy(Result, First, SizeOfElements);
    //从将First节点，开始将整个树大顶堆化
    AdjustHeap(First, 0, (Last - First) / SizeOfElements, Temp, SizeOfElements, Comparator);
}

static void SortHeap(char* First, char* Last, size_t SizeOfElements, CompareFunction Comparator)
{
    while (Last - First > SizeOfElements)
    {
        Last -= SizeOfElements;
        PopHeap(First, Last, Last, SizeOfElements, Comparator);
    }
}

static void MakeHeap(char* First, char* Last, size_t SizeOfElements, CompareFunction Comparator)
{
    int Length = (Last - First) / SizeOfElements;    //元素的个数
    if (Length < 2) return;
    int LastParent = (Length - 2) / 2;               //最后一个父节点的索引
    //void* Temp       = (void*)malloc(SizeOfElements);
    while (true)
    {
        memcpy(Temp, First + LastParent * SizeOfElements, SizeOfElements);
        AdjustHeap(First, LastParent, Length, Temp, SizeOfElements, Comparator);    //将以LastParent顶点为根节点的子树大顶堆化
        if (LastParent == 0) return;
        LastParent--;
    }
    //free(Temp);
}

static void HeapSelect(char* First, char* Middle, char* Last, size_t SizeOfElements, CompareFunction Comparator)
{
    //先将现有的前(First-Middle)项构建为一个大顶堆
    MakeHeap(First, Middle, SizeOfElements, Comparator);
    //然后历遍后面所有的节点，遇到比最大的First节点大的，就弹出First节点
    for (char* i = Middle; i < Last; i += SizeOfElements)
    {
        if (Comparator(i, First) > 0) PopHeap(First, Middle, i, SizeOfElements, Comparator);
    }
}

//此函数是将数列的最大的前(First-Middle)项排序，并且放在前面
void _PartialSort(char* First, char* Middle, char* Last, size_t SizeOfElements, CompareFunction Comparator)
{
    HeapSelect(First, Middle, Last, SizeOfElements, Comparator);    //构建一个大顶堆
    SortHeap(First, Middle, SizeOfElements, Comparator);            //将大顶堆排序
}

void HeapSort(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator) { _PartialSort(Base, Base + NumOfElements * SizeOfElements, Base + NumOfElements * SizeOfElements, SizeOfElements, Comparator); }

void PartialSort(void* Base, size_t k, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator)
{
    if (k > 0 && k <= NumOfElements) _PartialSort(Base, Base + k * SizeOfElements, Base + NumOfElements * SizeOfElements, SizeOfElements, Comparator);
}

static void _Unguarded_LinearInsert(char* Last, size_t SizeOfElements, CompareFunction Comparator)
{
    memcpy(Temp, Last, SizeOfElements);
    char* Next  = Last;
    Next       -= SizeOfElements;
    while (Comparator(Temp, Next) < 0)
    {
        memcpy(Last, Next, SizeOfElements);
        Last  = Next;
        Next -= SizeOfElements;
    }
    memcpy(Last, Temp, SizeOfElements);
}

/*
	此插入排序是专门用于一些递归排序算法中用于给元素个数小于S_Threshold的小数组排序用的
    此插入排序是一个通过每次都将比第一个元素小的元素移动到第一个的位置
	从而避免边界检查
*/
void _InsertionSort_Small(char* Low, char* High, size_t SizeOfElements, CompareFunction Comparator)
{
    if (Low == High) return;
    for (char* i = Low + SizeOfElements; i != High; i += SizeOfElements)
    {
        if (Comparator(i, Low) < 0)
        {
            memcpy(Temp, i, SizeOfElements);                //记录当前的值
            memmove(Low + SizeOfElements, Low, i - Low);    //将前面的所有元素向后移一位
            memcpy(Low, Temp, SizeOfElements);              //将i放到第一位
        }
        else { _Unguarded_LinearInsert(i, SizeOfElements, Comparator); }
    }
}

void InsertionSort(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator) { _InsertionSort_Small(Base, Base + NumOfElements * SizeOfElements, SizeOfElements, Comparator); }

/*
	此插入排序是无边界检查的插入排序
*/
inline static void _Unguarded_InsertionSort(char* Low, char* High, size_t SizeOfElements, CompareFunction Comparator)
{
    for (char* i = Low; i != High; i += SizeOfElements) _Unguarded_LinearInsert(i, SizeOfElements, Comparator);
}

/*
	此插入排序是用来给一个长数组,且已经以S_Threshold长度为单位局部有序
	现在先将前S_Threshold个元素插入排序
	后面的元素就可以使用无边界检查的插入排序来增加性能
*/
static void _Final_InsertionSort(char* Low, char* High, size_t SizeOfElements, CompareFunction Comparator)
{
    if (High - Low > S_Threshold * SizeOfElements)
    {
        _InsertionSort_Small(Low, Low + S_Threshold * SizeOfElements, SizeOfElements, Comparator);
        _Unguarded_InsertionSort(Low + S_Threshold * SizeOfElements, High, SizeOfElements, Comparator);
    }
    else _InsertionSort_Small(Low, High, SizeOfElements, Comparator);
}

static char* Unguarded_Partition(char* LeftP, char* RightP, char* Pivot, size_t SizeOfElements, CompareFunction Comparator)
{
    //使用的是Hoare partition scheme
    while (true)
    {
        while (Comparator(LeftP, Pivot) < 0) LeftP += SizeOfElements;
        RightP -= SizeOfElements;    //这里减一个是为了防止越界
        while (Comparator(RightP, Pivot) > 0) RightP -= SizeOfElements;
        if (!(LeftP < RightP)) return LeftP;
        SWAP(LeftP, RightP, SizeOfElements);
        LeftP += SizeOfElements;
    }
}

inline static char* Unguarded_PartitionPivot(char* Low, char* High, size_t SizeOfElements, CompareFunction Comparator)
{
    char* Mid = Low + SizeOfElements * (((High - Low) / SizeOfElements) / 2);
    MoveMidToFirst(Low, Low + SizeOfElements, Mid, High - SizeOfElements, SizeOfElements, Comparator);
    return Unguarded_Partition(Low + SizeOfElements, High, Low, SizeOfElements, Comparator);
}

static void IntroSort_Loop(char* Low, char* High, unsigned char DepthLimit, size_t SizeOfElements, CompareFunction Comparator)
{
    while ((High - Low) > S_Threshold * SizeOfElements)
    {
        if (DepthLimit == 0)
        {
            //使用堆排序
            _PartialSort(Low, High, High, SizeOfElements, Comparator);
            return;
        }
        --DepthLimit;
        char* Cut = Unguarded_PartitionPivot(Low, High, SizeOfElements, Comparator);
        IntroSort_Loop(Cut, High, DepthLimit, SizeOfElements, Comparator);
        High = Cut;
    }
}

/*
	此内省式排序则是几乎将libcstdc++库中的sort函数照搬过来的
*/
void IntrospectiveSort(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator)
{
    if (NumOfElements <= 1) return;
    IntroSort_Loop(Base, Base + NumOfElements * SizeOfElements, lg2(NumOfElements) * 2, SizeOfElements, Comparator);
    _Final_InsertionSort(Base, Base + NumOfElements * SizeOfElements, SizeOfElements, Comparator);
}

static char* GetPartition_glibc(char* LeftP, char* RightP, char* Pivot, size_t SizeOfElements, CompareFunction Comparator)
{
    //此处是glibc库中的qsort函数的实现划分的代码
    //使用的是Hoare partition scheme
    do {
        while (Comparator(LeftP, Pivot) < 0) LeftP += SizeOfElements;
        while (Comparator(Pivot, RightP) < 0) RightP -= SizeOfElements;
        if (LeftP < RightP)
        {
            SWAP(LeftP, RightP, SizeOfElements);
            if (Pivot == LeftP) Pivot = RightP;
            else if (Pivot == RightP) Pivot = LeftP;
            LeftP  += SizeOfElements;
            RightP -= SizeOfElements;
        }
        else if (LeftP == RightP) { return LeftP; }
    }
    while (LeftP <= RightP);
    //return LeftP;
    return RightP;
}

static char* GetPartitionPivot_glibc(char* Low, char* High, size_t SizeOfElements, CompareFunction Comparator)
{
    char* Mid = Low + SizeOfElements * ((High - Low) / SizeOfElements >> 1);
    //将Low,High,Mid排序
    if (Comparator(Mid, Low) < 0) SWAP(Mid, Low, SizeOfElements);
    if (Comparator(High, Mid) < 0) SWAP(Mid, High, SizeOfElements);
    else goto A;
    if (Comparator(Mid, Low) < 0) SWAP(Mid, Low, SizeOfElements);
A:
    return GetPartition_glibc(Low + SizeOfElements, High - SizeOfElements, Mid, SizeOfElements, Comparator);
}

void QuickSort_LTT_glibc_Loop(char* Low, char* High, size_t SizeOfElements, CompareFunction Comparator)
{
    //这样写可以减少一些函数的递归调用
    while ((High - Low) / SizeOfElements + 1 >= S_Threshold)
    {
        char* Cut = GetPartitionPivot_glibc(Low, High, SizeOfElements, Comparator);
        QuickSort_LTT_glibc_Loop(Cut + SizeOfElements, High, SizeOfElements, Comparator);    //Cut + SizeOfElements比Cut快一点
        High = Cut;                                                                          //Cut比Cut - SizeOfElements快一点
    }
}

/*
	此快速排序是使用了glibc库中的qsort函数的核心代码
	其中的QuickSort_LTT_glibc_Loop函数的循环则是借鉴了libstdc++的STL里面的sort的具体实现
*/
void QuickSort_LTT_glibc(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator)
{
    if (NumOfElements <= 1) return;
    QuickSort_LTT_glibc_Loop(Base, Base + (NumOfElements - 1) * SizeOfElements, SizeOfElements, Comparator);
    _Final_InsertionSort(Base, Base + SizeOfElements * NumOfElements, SizeOfElements, Comparator);
}

static char* GetPartition_libstdcpp(char* LeftP, char* RightP, char* Pivot, size_t SizeOfElements, CompareFunction Comparator)
{
    //此处是libstdc++库中的STL里面的sort的快速排序部分的实现划分的代码
    //使用的是Hoare partition scheme
    while (true)
    {
        while (Comparator(LeftP, Pivot) < 0) LeftP += SizeOfElements;
        while (Comparator(RightP, Pivot) > 0) RightP -= SizeOfElements;
        if (LeftP >= RightP) return LeftP;
        SWAP(LeftP, RightP, SizeOfElements);
        LeftP  += SizeOfElements;
        RightP -= SizeOfElements;
    }
}

static char* GetPartitionPivot_libstdcpp(char* Low, char* High, size_t SizeOfElements, CompareFunction Comparator)
{
    char* Mid = Low + SizeOfElements * (((High - Low) / SizeOfElements + 1) >> 1);
    MoveMidToFirst(Low, Low + SizeOfElements, Mid, High, SizeOfElements, Comparator);
    return GetPartition_libstdcpp(Low + SizeOfElements, High, Low, SizeOfElements, Comparator);
}

static void QuickSort_LTT_libstdcpp_Loop(char* Low, char* High, size_t SizeOfElements, CompareFunction Comparator)
{
    //这样写可以减少一些函数的递归调用
    while ((High - Low) / SizeOfElements + 1 >= S_Threshold)
    {
        char* Cut = GetPartitionPivot_libstdcpp(Low, High, SizeOfElements, Comparator);
        QuickSort_LTT_libstdcpp_Loop(Cut, High, SizeOfElements, Comparator);
        High = Cut - SizeOfElements;
    }
}

/*
	此快速排序是使用了libstdc++库中的STL里面的sort函数的快速排序的部分
	其中的QuickSort_LTT_glibc_Loop函数的循环则是借鉴了libstdc++的STL里面的sort的具体实现
*/
void QuickSort_LTT_libstdcpp(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator)
{
    if (NumOfElements <= 1) return;
    QuickSort_LTT_libstdcpp_Loop(Base, Base + (NumOfElements - 1) * SizeOfElements, SizeOfElements, Comparator);
    _Final_InsertionSort(Base, Base + SizeOfElements * NumOfElements, SizeOfElements, Comparator);
}

/*
	此快速排序则是几乎将glibc库中的qsort函数照搬过来的
*/
void QuickSort_glibc(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator)
{
    char*        Base_ptr   = (char*)Base;
    const size_t Max_Thresh = MAX_THRESH * SizeOfElements;    //每一个子数组的最大长度(4个元素大小)

    if (NumOfElements == 0) return;                           //空数组直接返回

    if (NumOfElements > MAX_THRESH)
    {
        char*       Low  = Base_ptr;
        char*       High = &Low[SizeOfElements * (NumOfElements - 1)];
        stack_node  Stack[STACK_SIZE];
        stack_node* Top = Stack;

        PUSH(NULL, NULL);

        while (STACK_NOT_EMPTY)
        {
            char* Left_ptr;
            char* Right_ptr;
            char* Mid = Low + SizeOfElements * ((High - Low) / SizeOfElements >> 1);
            //将Low,High,Mid排序
            if (Comparator((void*)Mid, (void*)Low) < 0) SWAP(Mid, Low, SizeOfElements);
            if (Comparator((void*)High, (void*)Mid) < 0) SWAP(Mid, High, SizeOfElements);
            else goto A;
            if (Comparator((void*)Mid, (void*)Low) < 0) SWAP(Mid, Low, SizeOfElements);
        A:
            Left_ptr  = Low + SizeOfElements;     //Left_ptr跳过第一个元素开始
            Right_ptr = High - SizeOfElements;    //Right_ptr跳过最后一个元素开始

            do {
                while (Comparator((void*)Left_ptr, (void*)Mid) < 0) Left_ptr += SizeOfElements;
                while (Comparator((void*)Mid, (void*)Right_ptr) < 0) Right_ptr -= SizeOfElements;
                if (Left_ptr < Right_ptr)
                {
                    SWAP(Left_ptr, Right_ptr, SizeOfElements);
                    if (Mid == Left_ptr) Mid = Right_ptr;
                    else if (Mid == Right_ptr) Mid = Left_ptr;
                    Left_ptr  += SizeOfElements;
                    Right_ptr -= SizeOfElements;
                }
                else if (Left_ptr == Right_ptr)
                {
                    Left_ptr  += SizeOfElements;
                    Right_ptr -= SizeOfElements;
                    break;
                }
            }
            while (Left_ptr <= Right_ptr);

            //将未排序的部分放入栈中
            if ((size_t)(Right_ptr - Low) <= Max_Thresh)                           //左边小
            {
                if ((size_t)(High - Left_ptr) <= Max_Thresh) POP(Low, High);       //左边小,右边小，直接弹出
                else Low = Left_ptr;                                               //左边小,右边大，从右边开始
            }                                                                      //左边大
            else if ((size_t)(High - Left_ptr) <= Max_Thresh) High = Right_ptr;    //左边大,右边小，从左边开始
            else if ((Right_ptr - Low) > (High - Left_ptr))                        //两边均大,左边大于右边
            {
                PUSH(Low, Right_ptr);                                              //将左边入栈，先排右边
                Low = Left_ptr;
            }
            else
            {
                PUSH(Left_ptr, High);                                              //将右边入栈，先排左边
                High = Right_ptr;
            }
        }
    }

#define MIN(x, y) ((x) < (y) ? (x) : (y))

    //将数组划分为4个元素一组的有序集合
    //用插入排序来进行最好的排序
    char* const End_ptr = &Base_ptr[SizeOfElements * (NumOfElements - 1)];                    //指向数组最后一个元素的指针
    char*       Tmp_ptr = Base_ptr;                                                           //指向数组第一个元素
    char*       Thresh  = MIN(End_ptr, Base_ptr + Max_Thresh);                                //第一组子数组的尾指针
    char*       Run_ptr;
    for (Run_ptr = Tmp_ptr + SizeOfElements; Run_ptr <= Thresh; Run_ptr += SizeOfElements)    //从第二个元素开始找最小的元素
    {
        if (Comparator((void*)Run_ptr, (void*)Tmp_ptr) < 0) Tmp_ptr = Run_ptr;                //找到比Tmp_ptr更小的元素,更新之
    }
    if (Tmp_ptr != Base_ptr) SWAP(Tmp_ptr, Base_ptr, SizeOfElements);                         //把子数组里面最小的元素放到第一个位置
    Run_ptr = Base_ptr + SizeOfElements;
    /*
        下面的作用是找到大于Run_ptr的元素组，将其向后移动
    */
    while ((Run_ptr += SizeOfElements) <= End_ptr)
    {
        Tmp_ptr = Run_ptr - SizeOfElements;
        while (Comparator((void*)Run_ptr, (void*)Tmp_ptr) < 0) Tmp_ptr -= SizeOfElements;
        Tmp_ptr += SizeOfElements;
        if (Tmp_ptr != Run_ptr)
        {
            char* Trav;
            Trav = Run_ptr + SizeOfElements;
            while (--Trav >= Run_ptr)
            {
                char  c = *Trav;
                char *High, *Low;
                for (High = Low = Trav; (Low -= SizeOfElements) >= Tmp_ptr; High = Low) *High = *Low;
                *High = c;
            }
        }
    }
}

void SimpleSelectionSort(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator)
{
    for (int i = 0; i < NumOfElements; ++i)
    {
        char* Min = Base + i * SizeOfElements;
        for (int j = i + 1; j < NumOfElements; ++j)
        {
            if (Comparator(Min, Base + j * SizeOfElements) > 0) Min = Base + j * SizeOfElements;
        }
        if (Base + i * SizeOfElements != Min) SWAP(Base + i * SizeOfElements, Min, SizeOfElements);
    }
}

static void Merge(char* First, char* Mid, char* Last, size_t SizeOfElements, CompareFunction Comparator)
{
    //如果自然有序则直接退出
    if (Comparator(Mid, Mid + SizeOfElements) <= 0) return;
    //开始归并
    char* i = First;
    char* j = Mid + SizeOfElements;
    char* k = Temp_Merge;
    while (i <= Mid && j <= Last)
    {
        if (Comparator(i, j) <= 0)
        {
            memcpy(k, i, SizeOfElements);
            i += SizeOfElements;
            k += SizeOfElements;
        }
        else
        {
            memcpy(k, j, SizeOfElements);
            j += SizeOfElements;
            k += SizeOfElements;
        }
    }
    if (i <= Mid) memcpy(k, i, Mid - i + SizeOfElements);
    if (j <= Last) memcpy(k, j, Last - j + SizeOfElements);
    memcpy(First, Temp_Merge, Last - First + SizeOfElements);
}

//空间复杂度减到了O(1),但是时间复杂度为O(n^2),不推荐！
static void Merge_Inplace(char* First, char* Mid, char* Last, size_t SizeOfElements, CompareFunction Comparator)
{
    char* First_2 = Mid + SizeOfElements;
    //如果自然有序则直接退出
    if (Comparator(Mid, First_2) <= 0) return;
    //开始归并
    while (First <= Mid && First_2 <= Last)
    {
        if (Comparator(First, First_2) <= 0) First += SizeOfElements;
        else
        {
            memcpy(Temp, First_2, SizeOfElements);
            char* index = First_2;
            //将 [First,First_2-SizeOfElements]中的元素向后移动
            while (index != First)
            {
                memcpy(index, index - SizeOfElements, SizeOfElements);
                index -= SizeOfElements;
            }
            memcpy(First, Temp, SizeOfElements);
            First   += SizeOfElements;
            Mid     += SizeOfElements;
            First_2 += SizeOfElements;
        }
    }
}

/*
	一种适用于int型数组的一种奇技淫巧
	其原理就是,对于两个数a,b,首先判断对Max取余的大小,选取小的一个数
	然后令a = a + (min(a % Max, b % max)) * Max
	这样在原本数所在的位置,对Max的商,就是排序好了的数值,而取余则是原本所在的数值
	但是在计算a = a + (min(a % Max, b % max)) * Max时要特别注意数值溢出!
*/
static void Merge_Inplace_Int(int* First, int* Mid, int* Last)
{
    //首先找到待归并的数组中最大的元素并加1
    int Max = *First;
    for (int* i = First; i <= Last; i += 1)
    {
        if (*i > Max) Max = *i;
    }
    ++Max;
    int i = 0;
    int j = Mid - First + 1;
    int k = 0;
    while (First + i <= Mid && First + j <= Last)
    {
        if (First[i] % Max <= First[j] % Max)
        {
            First[k] = First[k] + (First[i] % Max) * Max;
            k++;
            i++;
        }
        else
        {
            First[k] = First[k] + (First[j] % Max) * Max;
            k++;
            j++;
        }
    }
    while (First + i <= Mid)
    {
        First[k] = First[k] + (First[i] % Max) * Max;
        k++;
        i++;
    }
    while (First + j <= Last)
    {
        First[k] = First[k] + (First[j] % Max) * Max;
        k++;
        j++;
    }

    // 获取原始的有序序列
    for (int* i = First; i <= Last; i += 1) { *i = *i / Max; }
}

static void MergeSort_Recursion_Loop(char* First, char* Last, size_t SizeOfElements, CompareFunction Comparator)
{
    int NumOfElements = (Last - First) / SizeOfElements + 1;
    if (NumOfElements == 1) return;
    else if (NumOfElements >= S_Threshold)    //对小数组采用插入排序
    {
        char* Mid = First + ((NumOfElements >> 1) - 1) * SizeOfElements;
        MergeSort_Recursion_Loop(First, Mid, SizeOfElements, Comparator);
        MergeSort_Recursion_Loop(Mid + SizeOfElements, Last, SizeOfElements, Comparator);
        Merge(First, Mid, Last, SizeOfElements, Comparator);
    }
    else _InsertionSort_Small(First, First + SizeOfElements * NumOfElements, SizeOfElements, Comparator);
}

void MergeSort_Recursion(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator)
{
    Temp_Merge = (char*)malloc(NumOfElements * SizeOfElements);
    MergeSort_Recursion_Loop(Base, Base + (NumOfElements - 1) * SizeOfElements, SizeOfElements, Comparator);
    free(Temp_Merge);
}

void MergeSort_Iterative(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator)
{
    Temp_Merge = (char*)malloc(NumOfElements * SizeOfElements);
    int   CurrentSize;                                                                                                                                                                                                    //标识当前合并的子数组的大小，从1到NumOfElements/2
    char* First;                                                                                                                                                                                                          //标识当前要合并的子数组的起点
    for (CurrentSize = 1; CurrentSize <= NumOfElements - 1; CurrentSize = 2 * CurrentSize)
    {
        for (First = Base; First < (char*)Base + (NumOfElements - 1) * SizeOfElements; First += (2 * CurrentSize) * SizeOfElements)                                                                                       //First必须在最后一个元素的前面,这样后面或者还有一组或者直接就是最后一个元素了
        {
            char* Mid  = (First + (CurrentSize - 1) * SizeOfElements < (char*)Base + (NumOfElements - 1) * SizeOfElements) ? First + (CurrentSize - 1) * SizeOfElements : Base + (NumOfElements - 1) * SizeOfElements;    //找到第一组数组的最后一个元素
            char* Last = (Mid + CurrentSize * SizeOfElements < (char*)Base + (NumOfElements - 1) * SizeOfElements) ? Mid + CurrentSize * SizeOfElements : Base + (NumOfElements - 1) * SizeOfElements;                    //找到第二组数组的最后的元素
            Merge(First, Mid, Last, SizeOfElements, Comparator);
        }
    }
    free(Temp_Merge);
}

void MergeSort_Inplace_Iterative(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator)
{
    int   CurrentSize;                                                                                                                                                                                                    //标识当前合并的子数组的大小，从1到NumOfElements/2
    char* First;                                                                                                                                                                                                          //标识当前要合并的子数组的起点
    for (CurrentSize = 1; CurrentSize <= NumOfElements - 1; CurrentSize = 2 * CurrentSize)
    {
        for (First = Base; First < (char*)Base + (NumOfElements - 1) * SizeOfElements; First += (2 * CurrentSize) * SizeOfElements)                                                                                       //First必须在最后一个元素的前面,这样后面或者还有一组或者直接就是最后一个元素了
        {
            char* Mid  = (First + (CurrentSize - 1) * SizeOfElements < (char*)Base + (NumOfElements - 1) * SizeOfElements) ? First + (CurrentSize - 1) * SizeOfElements : Base + (NumOfElements - 1) * SizeOfElements;    //找到第一组数组的最后一个元素
            char* Last = (Mid + CurrentSize * SizeOfElements < (char*)Base + (NumOfElements - 1) * SizeOfElements) ? Mid + CurrentSize * SizeOfElements : Base + (NumOfElements - 1) * SizeOfElements;                    //找到第二组数组的最后的元素
            Merge_Inplace(First, Mid, Last, SizeOfElements, Comparator);
        }
    }
}

void MergeSort_Inplace_Iterative_For_Int(int* Base, size_t NumOfElements)
{
    int  CurrentSize;                                                                                                                 //标识当前合并的子数组的大小，从1到NumOfElements/2
    int* First;                                                                                                                       //标识当前要合并的子数组的起点
    for (CurrentSize = 1; CurrentSize <= NumOfElements - 1; CurrentSize = 2 * CurrentSize)
    {
        for (First = Base; First < &Base[NumOfElements - 1]; First += (2 * CurrentSize))                                              //First必须在最后一个元素的前面,这样后面或者还有一组或者直接就是最后一个元素了
        {
            int* Mid  = (&First[CurrentSize - 1] < &Base[NumOfElements - 1]) ? &First[CurrentSize - 1] : &Base[NumOfElements - 1];    //找到第一组数组的最后一个元素
            int* Last = (&Mid[CurrentSize] < &Base[NumOfElements - 1]) ? &Mid[CurrentSize] : &Base[NumOfElements - 1];                //找到第二组数组的最后的元素
            Merge_Inplace_Int(First, Mid, Last);
        }
    }
}

/*
	此排序相当于从最低位到最高位对int型关键值进行计数排序
	每一位最多10个不同的元素,就开了10个桶
*/
void RadixSort_LSD(void* Base, size_t NumOfElements, size_t SizeOfElements, ValueFunction GetValue)
{
    if (NumOfElements <= 1) return;
    //将每一个元素对应的数字先提取出来方便后续使用
    int* Value = (int*)malloc(NumOfElements * sizeof(int));
    if (Value == NULL)
    {
        printf("内存分配失败！\n");
        return;
    }
    for (int i = 0; i < NumOfElements; ++i) Value[i] = GetValue(Base + i * SizeOfElements);
    //创建统计数组并统计对应元素的个数
    size_t CountArray[10] = {0};
    void*  OrderedArray   = (void*)malloc(NumOfElements * SizeOfElements);
    if (OrderedArray == NULL)
    {
        printf("内存分配失败！\n");
        return;
    }
    //求出最大值
    int Max = Value[0];
    for (int i = 0; i < NumOfElements; ++i)
    {
        if (Value[i] > Max) Max = Value[i];
    }
    //求出所有数的最大位数
    int Digits = (Max == 0) ? 1 : (int)log10(Max) + 1;
    //最大位数的个数,进行几次计数排序
    for (int i = 1; i <= Digits; ++i)
    {
        //将每一个元素对应的数字先提取出来方便后续使用
        for (int i = 0; i < NumOfElements; ++i) Value[i] = GetValue(Base + i * SizeOfElements);
        //统计对应元素的个数
        for (int j = 0; j < NumOfElements; ++j) ++CountArray[(Value[j] / (int)pow(10, i - 1)) % 10];
        //统计数组做变形,后面的元素等于前面的元素之和
        for (int j = 1; j <= 9; ++j) CountArray[j] += CountArray[j - 1];
        //取出每个桶中的元素，赋值给数组
        for (int j = NumOfElements - 1; j >= 0; --j) memcpy(OrderedArray + (--CountArray[(Value[j] / (int)pow(10, i - 1)) % 10]) * SizeOfElements, Base + j * SizeOfElements, SizeOfElements);
        memcpy(Base, OrderedArray, NumOfElements * SizeOfElements);
        memset(CountArray, 0, 10 * sizeof(size_t));    //将计数数组归零
    }
    //排序完成
    free(Value);
    free(OrderedArray);
}

static void RadixSort_MSD_Loop(int* Base, size_t NumOfElements, int Radix)
{
    if (Radix <= 0) return;
    //10进制数,有10个桶,每个桶最多存NumOfElements个数
    int* Bucket[10];
    for (int i = 0; i < 10; ++i)
    {
        Bucket[i] = (int*)malloc(NumOfElements * sizeof(int));
        if (Bucket[i] == NULL)
        {
            printf("内存分配失败！\n");
            return;
        }
    }
    //用来计算,统计每个桶所存的元素的个数,每个桶对应一个元素
    int BucketElementCounts[10] = {0};
    for (int i = 0; i < NumOfElements; ++i)
    {
        //求的是第i个数对应基数是Radix的数码的值
        int Value                                 = Base[i] / Radix % 10;
        Bucket[Value][BucketElementCounts[Value]] = Base[i];
        ++BucketElementCounts[Value];
    }
    int index = 0;    //Base新的下标
    //遍历每个桶,如果桶里面有2个以上的元素,就把这个桶再一次RadixSort_MSD_Loop排序
    for (int i = 0; i < 10; ++i)
    {
        if (BucketElementCounts[i] > 1) RadixSort_MSD_Loop(Bucket[i], BucketElementCounts[i], Radix / 10);
        for (int j = 0; j < BucketElementCounts[i]; ++j)
        {
            Base[index] = Bucket[i][j];
            ++index;
        }
    }
    for (int i = 0; i < 10; ++i) free(Bucket[i]);
}

void RadixSort_MSD(int* Base, size_t NumOfElements)
{
    if (NumOfElements <= 1) return;
    //求出最大值
    int Max = Base[0];
    for (int i = 0; i < NumOfElements; ++i)
    {
        if (Base[i] > Max) Max = Base[i];
    }
    //求出最大值的位数
    int Digits = (Max == 0) ? 1 : (int)log10(Max) + 1;
    //计算最大值的基数
    int Radix  = (int)pow(10, Digits - 1);
    RadixSort_MSD_Loop(Base, NumOfElements, Radix);
}

void CountingSort(void* Base, size_t NumOfElements, size_t SizeOfElements, ValueFunction GetValue)
{
    if (NumOfElements <= 1) return;
    //将每一个元素对应的数字先提取出来方便后续使用
    int* Value = (int*)calloc(NumOfElements, sizeof(int));
    if (Value == NULL)
    {
        printf("内存分配失败！\n");
        return;
    }
    for (int i = 0; i < NumOfElements; ++i) Value[i] = GetValue(Base + i * SizeOfElements);
    //得到数列的最大值和最小值，并算出差值Delta
    int Max = Value[0];
    int Min = Value[0];
    for (int i = 0; i < NumOfElements; ++i)
    {
        if (Value[i] > Max) Max = Value[i];
        if (Value[i] < Min) Min = Value[i];
    }
    int Delta          = Max - Min;
    //创建统计数组并统计对应元素的个数
    size_t* CountArray = (size_t*)calloc((size_t)(Delta + 1), sizeof(size_t));
    if (CountArray == NULL)
    {
        printf("内存分配失败！\n");
        return;
    }
    for (int i = 0; i < NumOfElements; ++i) ++CountArray[Value[i] - Min];
    //统计数组做变形,后面的元素等于前面的元素之和
    for (int i = 1; i <= Delta; ++i) CountArray[i] += CountArray[i - 1];
    void* OrderedArray = (void*)malloc(NumOfElements * SizeOfElements);
    if (OrderedArray == NULL)
    {
        printf("内存分配失败！\n");
        return;
    }
    for (int i = NumOfElements - 1; i >= 0; --i) memcpy(OrderedArray + (--CountArray[Value[i] - Min]) * SizeOfElements, Base + i * SizeOfElements, SizeOfElements);
    memcpy(Base, OrderedArray, NumOfElements * SizeOfElements);
    free(Value);
    free(CountArray);
    free(OrderedArray);
}

void BucketSort(void* Base, size_t NumOfElements, size_t SizeOfElements, ValueFunction GetValue)
{
    if (NumOfElements <= 1) return;
    //将每一个元素对应的数字先提取出来方便后续使用
    int* Value = (int*)malloc(NumOfElements * sizeof(int));
    if (Value == NULL)
    {
        printf("内存分配失败！\n");
        return;
    }
    for (int i = 0; i < NumOfElements; ++i) Value[i] = GetValue(Base + i * SizeOfElements);
    //得到数列的最大值和最小值，并算出差值Delta
    int Max = Value[0];
    int Min = Value[0];
    for (int i = 0; i < NumOfElements; ++i)
    {
        if (Value[i] > Max) Max = Value[i];
        if (Value[i] < Min) Min = Value[i];
    }
    //建立桶,每个桶里面放入元素的范围是(Max - Min) / Bucket_NUM + 1
    void* Bucket[Bucket_NUM];
    for (int i = 0; i < Bucket_NUM; ++i)
    {
        Bucket[i] = (void*)malloc(NumOfElements * SizeOfElements);
        if (Bucket[i] == NULL)
        {
            printf("内存分配失败！\n");
            return;
        }
    }
    //用来记录每一个桶里面的元素个数
    size_t* CountArray = (size_t*)calloc(Bucket_NUM, sizeof(size_t));
    if (CountArray == NULL)
    {
        printf("内存分配失败！\n");
        return;
    }
    int BucketLength = ceil((double)(Max - Min + 1) / Bucket_NUM);    //向上取整
    //将Base数组里面的每一个元素映射到桶里面
    for (int i = 0; i < NumOfElements; ++i)
    {
        int index = (Value[i] - Min) / BucketLength;
        memcpy((char*)Bucket[index] + (++CountArray[index] - 1) * SizeOfElements, (char*)Base + i * SizeOfElements, SizeOfElements);
    }
    //对每一个桶进行计数排序(快上加快)
    int TotalNum = 0;
    for (int i = 0; i < Bucket_NUM; ++i)
    {
        CountingSort(Bucket[i], CountArray[i], SizeOfElements, GetValue);
        //把每个桶里面的元素放回Base数组里面
        memcpy(Base + TotalNum * SizeOfElements, Bucket[i], CountArray[i] * SizeOfElements);
        TotalNum += CountArray[i];
    }
    free(Value);
    for (int i = 0; i < Bucket_NUM; ++i) free(Bucket[i]);
}

/*
	注意与计数排序的区别, 此排序只考虑每一个元素出现的次数,并且只能排序int数组
	本来PigeonholeSort是直接将int值直接映射到数值的索引上,并在上面记录个数
	然后再历遍一次数组将数全部放到原数组里面就可以了
	但是为了全部开空间INT_MAX也开不了(16GB),于是有了下面的改进算法
	与计数排序的区别就是他不实际记录每一个元素的位置,而是个数,最后依次输出即可
*/

void PigeonholeSort(int* Base, size_t NumOfElements)
{
    if (NumOfElements <= 1) return;
    //得到数列的最大值和最小值，并算出差值Delta
    int Max = Base[0];
    int Min = Base[0];
    for (int i = 0; i < NumOfElements; ++i)
    {
        if (Base[i] > Max) Max = Base[i];
        if (Base[i] < Min) Min = Base[i];
    }
    int Delta       = Max - Min;
    //创建统计数组并统计对应元素的个数
    int* CountArray = (int*)calloc((Delta + 1), sizeof(int));
    if (CountArray == NULL)
    {
        printf("内存分配失败！\n");
        return;
    }
    for (int i = 0; i < NumOfElements; ++i) ++CountArray[Base[i] - Min];
    int index = 0;
    for (int i = 0; i <= Delta; ++i)
    {
        while (CountArray[i] != 0)
        {
            Base[index] = i + Min;
            --CountArray[i];
            ++index;
        }
    }
    free(CountArray);
}

void BigoSort(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator)
{
A:
    for (int i = 0; i < NumOfElements - 1; ++i)
    {
        if (Comparator(Base + i * SizeOfElements, Base + (i + 1) * SizeOfElements) > 0)
        {
            for (int j = NumOfElements - 1; j > 0; --j)
            {
                int ChangeIndex = rand() % j;
                SWAP(Base + j * SizeOfElements, Base + ChangeIndex * SizeOfElements, SizeOfElements);
            }
            goto A;
        }
    }
}