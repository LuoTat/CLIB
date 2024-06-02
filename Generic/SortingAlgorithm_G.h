#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "Generic_tool.h"


// SwapModeMacro
#ifndef SWAPMODE
    #define SWAPMODE 1
#endif

// MAX_SIZEOFELEMENTS
#ifndef MAX_SIZEOFELEMENTS
    #define MAX_SIZEOFELEMENTS 1024
#endif

#define MIN(x, y) ((x) < (y) ? (x) : (y))


#if SWAPMODE == 1
    #define SWAP(a, b, TYPE)    \
        do {                    \
            TYPE __tmp = *(a);  \
            *(a)       = *(b);  \
            *(b)       = __tmp; \
        }                       \
        while (0)
#elif SWAPMODE == 2
    #define SWAP(a, b, TYPE)                            \
        do {                                            \
            size_t __DataSize = (sizeof(TYPE));         \
            char * __a = (char*)(a), *__b = (char*)(b); \
            do {                                        \
                char __tmp = *__a;                      \
                *__a++     = *__b;                      \
                *__b++     = __tmp;                     \
            }                                           \
            while (--__DataSize > 0);                   \
        }                                               \
        while (0)
#elif SWAPMODE == 3
    #define SWAP(a, b, TYPE)                           \
        do {                                           \
            size_t __DataSize = (sizeof(TYPE));        \
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

// 用来作为许多排序算法的临时存储空间
//static char  Temp[MAX_SIZEOFELEMENTS];
//static char* Temp_Merge;

#define MAX_THRESH          16
#define S_Threshold         16
#define Bucket_NUM          8

#define STACK_SIZE          (CHAR_BIT * sizeof(size_t))
#define PUSH(LeftP, RightP) ((void)((Top->Low = (LeftP)), (Top->High = (RightP)), ++Top))
#define POP(LeftP, RightP)  ((void)(--Top, ((LeftP) = Top->Low), ((RightP) = Top->High)))
#define STACK_NOT_EMPTY     (Stack < Top)


#define SORT_TYPE(NAME, TYPE)        \
    typedef struct stack_node_##NAME \
    {                                \
        TYPE* Low;                   \
        TYPE* High;                  \
    } stack_node_##NAME;


#define SORT_IMPL(NAME, TYPE, SCOPE, Compare_Function)                                       \
    SCOPE void MoveMidToFirst_##NAME(TYPE* Result, TYPE* a, TYPE* b, TYPE* c)                \
    {                                                                                        \
        if (Compare_Function(a, b) < 0)                                                      \
        {                                                                                    \
            if (Compare_Function(b, c) < 0) SWAP(Result, b, TYPE);                           \
            else if (Compare_Function(a, c)) SWAP(Result, c, TYPE);                          \
            else SWAP(Result, a, TYPE);                                                      \
        }                                                                                    \
        else if (Compare_Function(a, c) < 0) SWAP(Result, a, TYPE);                          \
        else if (Compare_Function(b, c) < 0) SWAP(Result, c, TYPE);                          \
        else SWAP(Result, b, TYPE);                                                          \
    }                                                                                        \
    SCOPE void BinaryInsertionSort_##NAME(TYPE* Base, size_t NumOfElements)                  \
    {                                                                                        \
        if (NumOfElements <= 0) return;                                                      \
        TYPE Temp;                                                                           \
        for (size_t i = 1; i < NumOfElements; ++i)                                           \
        {                                                                                    \
            Temp = Base[i];                                                                  \
            int Position;                                                                    \
            int Low  = 0;                                                                    \
            int High = i - 1;                                                                \
            while (Low <= High)                                                              \
            {                                                                                \
                int Mid   = (Low + High) >> 1;                                               \
                int Delta = Compare_Function(&Temp, &Base[Mid]);                             \
                if (Delta == 0)                                                              \
                {                                                                            \
                    Position = Mid + 1;                                                      \
                    goto A;                                                                  \
                }                                                                            \
                else if (Delta < 0) High = Mid - 1;                                          \
                else Low = Mid + 1;                                                          \
            }                                                                                \
            Position = High + 1;                                                             \
        A:                                                                                   \
            for (int j = i - 1; j >= Position; --j) Base[j + 1] = Base[j];                   \
            Base[Position] = Temp;                                                           \
        }                                                                                    \
    }                                                                                        \
    SCOPE void ShellInsertionSort_Hibbard_##NAME(TYPE* Base, size_t NumOfElements)           \
    {                                                                                        \
        int* GapArray = GetHibbardStepArray(NumOfElements);                                  \
        TYPE Temp;                                                                           \
        for (int i = 0;; ++i)                                                                \
        {                                                                                    \
            int Gap = GapArray[i];                                                           \
            for (size_t j = Gap; j < NumOfElements; ++j)                                     \
            {                                                                                \
                Temp         = Base[j];                                                      \
                int Position = j - Gap;                                                      \
                while (Position >= 0 && Compare_Function(&Base[Position], &Temp) > 0)        \
                {                                                                            \
                    Base[Position + Gap]  = Base[Position];                                  \
                    Position             -= Gap;                                             \
                }                                                                            \
                Base[Position + Gap] = Temp;                                                 \
            }                                                                                \
            if (Gap == 1) break;                                                             \
        }                                                                                    \
    }                                                                                        \
    SCOPE void ShellInsertionSort_Sedgewick_##NAME(TYPE* Base, size_t NumOfElements)         \
    {                                                                                        \
        int* GapArray = GetSedgewickStepArray(NumOfElements);                                \
        TYPE Temp;                                                                           \
        for (int i = 0;; ++i)                                                                \
        {                                                                                    \
            int Gap = GapArray[i];                                                           \
            for (size_t j = Gap; j < NumOfElements; ++j)                                     \
            {                                                                                \
                Temp         = Base[j];                                                      \
                int Position = j - Gap;                                                      \
                while (Position >= 0 && Compare_Function(&Base[Position], &Temp) > 0)        \
                {                                                                            \
                    Base[Position + Gap]  = Base[Position];                                  \
                    Position             -= Gap;                                             \
                }                                                                            \
                Base[Position + Gap] = Temp;                                                 \
            }                                                                                \
            if (Gap == 1) break;                                                             \
        }                                                                                    \
    }                                                                                        \
    SCOPE void BubbleSort_##NAME(TYPE* Base, size_t NumOfElements)                           \
    {                                                                                        \
        for (size_t i = 0; i < NumOfElements - 1; ++i)                                       \
        {                                                                                    \
            bool Ordered = true;                                                             \
            for (size_t j = 0; j < NumOfElements - 1 - i; j++)                               \
            {                                                                                \
                if (Compare_Function(&Base[j], &Base[j + 1]) > 0)                            \
                {                                                                            \
                    SWAP(&Base[j], &Base[j + 1], TYPE);                                      \
                    Ordered = false;                                                         \
                }                                                                            \
            }                                                                                \
            if (Ordered) break;                                                              \
        }                                                                                    \
    }                                                                                        \
    SCOPE void BubbleSort_Fast_##NAME(TYPE* Base, size_t NumOfElements)                      \
    {                                                                                        \
        bool   Ordered;                                                                      \
        size_t HighPos = NumOfElements - 1;                                                  \
        size_t LowPos  = 0;                                                                  \
        for (size_t i = 0; i < NumOfElements - 1; ++i)                                       \
        {                                                                                    \
            Ordered            = true;                                                       \
            size_t TempHighPos = HighPos;                                                    \
            for (size_t j = LowPos; j < HighPos; ++j)                                        \
            {                                                                                \
                if (Compare_Function(&Base[j], &Base[j + 1]) > 0)                            \
                {                                                                            \
                    SWAP(&Base[j], &Base[j + 1], TYPE);                                      \
                    Ordered     = false;                                                     \
                    TempHighPos = j;                                                         \
                }                                                                            \
            }                                                                                \
            if (Ordered) break;                                                              \
            HighPos           = TempHighPos;                                                 \
            size_t TempLowPos = LowPos;                                                      \
            for (size_t j = HighPos; j > LowPos; --j)                                        \
            {                                                                                \
                if (Compare_Function(&Base[j], &Base[j - 1]) < 0)                            \
                {                                                                            \
                    SWAP(&Base[j], &Base[j - 1], TYPE);                                      \
                    Ordered    = false;                                                      \
                    TempLowPos = j;                                                          \
                }                                                                            \
            }                                                                                \
            if (Ordered) break;                                                              \
            LowPos = TempLowPos;                                                             \
        }                                                                                    \
    }                                                                                        \
    SCOPE void QuickSort_glibc_##NAME(TYPE* Base, size_t NumOfElements)                      \
    {                                                                                        \
        TYPE*        Base_ptr   = Base;                                                      \
        const size_t Max_Thresh = MAX_THRESH * sizeof(TYPE);                                 \
        if (NumOfElements == 0) return;                                                      \
        if (NumOfElements > MAX_THRESH)                                                      \
        {                                                                                    \
            TYPE*              Low  = Base_ptr;                                              \
            TYPE*              High = Low + NumOfElements - 1;                               \
            stack_node_##NAME  Stack[STACK_SIZE];                                            \
            stack_node_##NAME* Top = Stack;                                                  \
            PUSH(NULL, NULL);                                                                \
            while (STACK_NOT_EMPTY)                                                          \
            {                                                                                \
                TYPE* Left_ptr;                                                              \
                TYPE* Right_ptr;                                                             \
                TYPE* Mid = Low + ((High - Low) >> 1);                                       \
                if (*Mid < *Low) SWAP(Mid, Low, TYPE);                                       \
                if (*High < *Mid) SWAP(Mid, High, TYPE);                                     \
                else goto A;                                                                 \
                if (*Mid < *Low) SWAP(Mid, Low, TYPE);                                       \
            A:                                                                               \
                Left_ptr  = Low + 1;                                                         \
                Right_ptr = High - 1;                                                        \
                do {                                                                         \
                    while (*Left_ptr < *Mid) ++Left_ptr;                                     \
                    while (*Mid < *Right_ptr) --Right_ptr;                                   \
                    if (Left_ptr < Right_ptr)                                                \
                    {                                                                        \
                        SWAP(Left_ptr, Right_ptr, TYPE);                                     \
                        if (Mid == Left_ptr) Mid = Right_ptr;                                \
                        else if (Mid == Right_ptr) Mid = Left_ptr;                           \
                        ++Left_ptr;                                                          \
                        --Right_ptr;                                                         \
                    }                                                                        \
                    else if (Left_ptr == Right_ptr)                                          \
                    {                                                                        \
                        ++Left_ptr;                                                          \
                        --Right_ptr;                                                         \
                        break;                                                               \
                    }                                                                        \
                }                                                                            \
                while (Left_ptr <= Right_ptr);                                               \
                if ((size_t)(Right_ptr - Low) <= Max_Thresh)                                 \
                {                                                                            \
                    if ((size_t)(High - Left_ptr) <= Max_Thresh) POP(Low, High);             \
                    else Low = Left_ptr;                                                     \
                }                                                                            \
                else if ((size_t)(High - Left_ptr) <= Max_Thresh) High = Right_ptr;          \
                else if ((Right_ptr - Low) > (High - Left_ptr))                              \
                {                                                                            \
                    PUSH(Low, Right_ptr);                                                    \
                    Low = Left_ptr;                                                          \
                }                                                                            \
                else                                                                         \
                {                                                                            \
                    PUSH(Left_ptr, High);                                                    \
                    High = Right_ptr;                                                        \
                }                                                                            \
            }                                                                                \
        }                                                                                    \
        TYPE* const End_ptr = Base_ptr + NumOfElements - 1;                                  \
        TYPE*       Tmp_ptr = Base_ptr;                                                      \
        TYPE*       Thresh  = MIN(End_ptr, Base_ptr + Max_Thresh);                           \
        TYPE*       Run_ptr;                                                                 \
        for (Run_ptr = Tmp_ptr + 1; Run_ptr <= Thresh; ++Run_ptr)                            \
        {                                                                                    \
            if (*Run_ptr < *Tmp_ptr) Tmp_ptr = Run_ptr;                                      \
        }                                                                                    \
        if (Tmp_ptr != Base_ptr) SWAP(Tmp_ptr, Base_ptr, TYPE);                              \
        Run_ptr = Base_ptr + 1;                                                              \
        while (++Run_ptr <= End_ptr)                                                         \
        {                                                                                    \
            Tmp_ptr = Run_ptr - 1;                                                           \
            while (*Run_ptr < *Tmp_ptr) --Tmp_ptr;                                           \
            ++Tmp_ptr;                                                                       \
            if (Tmp_ptr != Run_ptr)                                                          \
            {                                                                                \
                int* Trav;                                                                   \
                Trav = Run_ptr + 1;                                                          \
                while (--Trav >= Run_ptr)                                                    \
                {                                                                            \
                    TYPE  c = *Trav;                                                         \
                    TYPE *High, *Low;                                                        \
                    for (High = Low = Trav; (Low -= 1) >= Tmp_ptr; High = Low) *High = *Low; \
                    *High = c;                                                               \
                }                                                                            \
            }                                                                                \
        }                                                                                    \
    }                                                                                        \
    SCOPE void _Unguarded_LinearInsert_##NAME(TYPE* Last)                                    \
    {                                                                                        \
        TYPE  Temp = *Last;                                                                  \
        TYPE* Next = Last;                                                                   \
        --Next;                                                                              \
        while (Compare_Function(&Temp, Next) < 0)                                            \
        {                                                                                    \
            *Last = *Next;                                                                   \
            Last  = Next;                                                                    \
            --Next;                                                                          \
        }                                                                                    \
        *Last = Temp;                                                                        \
    }                                                                                        \
    SCOPE void _InsertionSort_Small_##NAME(TYPE* Low, TYPE* High)                            \
    {                                                                                        \
        if (Low == High) return;                                                             \
        for (TYPE* i = Low + 1; i != High; ++i)                                              \
        {                                                                                    \
            if (Compare_Function(i, Low) < 0)                                                \
            {                                                                                \
                TYPE Temp = *i;                                                              \
                memmove(Low + 1, Low, (i - Low) * sizeof(TYPE));                             \
                *Low = Temp;                                                                 \
            }                                                                                \
            else _Unguarded_LinearInsert_##NAME(i);                                          \
        }                                                                                    \
    }                                                                                        \
    SCOPE LTT_inline void _Unguarded_InsertionSort_##NAME(TYPE* Low, TYPE* High)             \
    {                                                                                        \
        for (TYPE* i = Low; i != High; ++i) _Unguarded_LinearInsert_##NAME(i);               \
    }                                                                                        \
    SCOPE void _Final_InsertionSort_##NAME(TYPE* Low, TYPE* High)                            \
    {                                                                                        \
        if (High - Low > S_Threshold)                                                        \
        {                                                                                    \
            _InsertionSort_Small_##NAME(Low, Low + S_Threshold);                             \
            _Unguarded_InsertionSort_##NAME(Low + S_Threshold, High);                        \
        }                                                                                    \
        else _InsertionSort_Small_##NAME(Low, High);                                         \
    }                                                                                        \
    SCOPE TYPE* GetPartition_LTT_glibc_##NAME(TYPE* LeftP, TYPE* RightP, TYPE* Pivot)        \
    {                                                                                        \
        do {                                                                                 \
            while (Compare_Function(LeftP, Pivot) < 0) ++LeftP;                              \
            while (Compare_Function(Pivot, RightP) < 0) --RightP;                            \
            if (LeftP < RightP)                                                              \
            {                                                                                \
                SWAP(LeftP, RightP, TYPE);                                                   \
                if (Pivot == LeftP) Pivot = RightP;                                          \
                else if (Pivot == RightP) Pivot = LeftP;                                     \
                ++LeftP;                                                                     \
                --RightP;                                                                    \
            }                                                                                \
            else if (LeftP == RightP) return LeftP;                                          \
        }                                                                                    \
        while (LeftP <= RightP);                                                             \
        return RightP;                                                                       \
    }                                                                                        \
    SCOPE TYPE* GetPartitionPivot_LTT_glibc_##NAME(TYPE* Low, TYPE* High)                    \
    {                                                                                        \
        TYPE* Mid = Low + ((High - Low) >> 1);                                               \
        if (Compare_Function(Mid, Low) < 0) SWAP(Mid, Low, TYPE);                            \
        if (Compare_Function(High, Mid) < 0) SWAP(Mid, High, TYPE);                          \
        else goto A;                                                                         \
        if (Compare_Function(Mid, Low) < 0) SWAP(Mid, Low, TYPE);                            \
    A:                                                                                       \
        return GetPartition_LTT_glibc_##NAME(Low + 1, High - 1, Mid);                        \
    }                                                                                        \
    SCOPE void QuickSort_LTT_glibc_Loop_##NAME(TYPE* Low, TYPE* High)                        \
    {                                                                                        \
        while (High - Low > S_Threshold)                                                     \
        {                                                                                    \
            TYPE* Cut = GetPartitionPivot_LTT_glibc_##NAME(Low, High - 1);                   \
            QuickSort_LTT_glibc_Loop_##NAME(Cut, High);                                      \
            High = Cut;                                                                      \
        }                                                                                    \
    }                                                                                        \
    SCOPE void QuickSort_LTT_glibc_##NAME(TYPE* Base, size_t NumOfElements)                  \
    {                                                                                        \
        if (NumOfElements <= 1) return;                                                      \
        QuickSort_LTT_glibc_Loop_##NAME(Base, &Base[NumOfElements]);                         \
        _Final_InsertionSort_##NAME(Base, &Base[NumOfElements]);                             \
    }                                                                                        \
    SCOPE TYPE* GetPartition_libstdcpp_##NAME(TYPE* LeftP, TYPE* RightP, TYPE* Pivot)        \
    {                                                                                        \
        while (true)                                                                         \
        {                                                                                    \
            while (Compare_Function(LeftP, Pivot) < 0) ++LeftP;                              \
            --RightP;                                                                        \
            while (Compare_Function(RightP, Pivot) > 0) --RightP;                            \
            if (!(LeftP < RightP)) return LeftP;                                             \
            SWAP(LeftP, RightP, TYPE);                                                       \
        }                                                                                    \
    }                                                                                        \
    SCOPE LTT_inline TYPE* GetPartitionPivot_LTT_libstdcpp_##NAME(TYPE* Low, TYPE* High)     \
    {                                                                                        \
        TYPE* Mid = Low + ((High - Low) >> 1);                                               \
        MoveMidToFirst_##NAME(Low, Low + 1, Mid, High - 1);                                  \
        return GetPartition_libstdcpp_##NAME(Low + 1, High, Low);                            \
    }                                                                                        \
    SCOPE void QuickSort_LTT_libstdcpp_Loop_##NAME(TYPE* Low, TYPE* High)                    \
    {                                                                                        \
        while (High - Low > S_Threshold)                                                     \
        {                                                                                    \
            TYPE* Cut = GetPartitionPivot_LTT_libstdcpp_##NAME(Low, High);                   \
            QuickSort_LTT_libstdcpp_Loop_##NAME(Cut, High);                                  \
            High = Cut;                                                                      \
        }                                                                                    \
    }                                                                                        \
    SCOPE void QuickSort_LTT_libstdcpp_##NAME(TYPE* Base, size_t NumOfElements)              \
    {                                                                                        \
        if (NumOfElements <= 1) return;                                                      \
        QuickSort_LTT_libstdcpp_Loop_##NAME(Base, &Base[NumOfElements]);                     \
        _Final_InsertionSort_##NAME(Base, &Base[NumOfElements]);                             \
    }


///*
//    这个AdjustHeap用来将以RootIndex为索引为根节点的子树的大顶堆化
//*/
//static void AdjustHeap_##NAME(char* Base, int RootIndex, int Length, void* TempValue, size_t SizeOfElements, CompareFunction Compare_Function)
//{
//    int SecondChild = RootIndex;                                                                                                    //记录子树的根的子节点
//    while (SecondChild < (Length - 1) / 2)                                                                                          //判断是否有右子节点
//    {
//        SecondChild = 2 * (SecondChild + 1);                                                                                        //更新SecondChild为当前节点的右子节点
//        if (Compare_Function(Base + (SecondChild - 1) * SizeOfElements, Base + SecondChild * SizeOfElements) > 0) --SecondChild;    //如果左子节点大于右子节点，更新SecondChild为左子节点,以确保选择较大的子节点
//        if (Compare_Function(TempValue, Base + SecondChild * SizeOfElements) > 0) { goto A; }                                       //如果其父节点大于其子节点，则直接返回
//        memcpy(Base + RootIndex * SizeOfElements, Base + SecondChild * SizeOfElements, SizeOfElements);                             //将更大的子节点放到父节点的位置
//        RootIndex = SecondChild;                                                                                                    //更新根节点的索引
//    }
//    if ((Length & 1) == 0 && SecondChild == (Length - 2) / 2)                                                                       //如果范围长度是偶数且SecondChild刚好为最后一个非叶子节点的索引
//    {
//        SecondChild = 2 * (SecondChild + 1) - 1;                                                                                    //更新SecondChild为最后一个节点的索引
//        if (Compare_Function(TempValue, Base + SecondChild * SizeOfElements) > 0) { goto A; };                                      //如果其父节点大于其子节点，则直接返回
//        memcpy(Base + RootIndex * SizeOfElements, Base + SecondChild * SizeOfElements, SizeOfElements);
//        RootIndex = SecondChild;
//        //将更大的子节点放到父节点的位置
//    }
//A:
//    memcpy(Base + RootIndex * SizeOfElements, TempValue, SizeOfElements);
//}

//inline static void PopHeap_##NAME(char* First, char* Last, char* Result, size_t SizeOfElements, CompareFunction Compare_Function)
//{
//    //此函数的作用是将Result与Last,然后将其大顶堆化
//    memcpy(Temp, Result, SizeOfElements);    //将Result存储起来
//    memcpy(Result, First, SizeOfElements);
//    //从将First节点，开始将整个树大顶堆化
//    AdjustHeap(First, 0, (Last - First) / SizeOfElements, Temp, SizeOfElements, Compare_Function);
//}

//static void SortHeap_##NAME(char* First, char* Last, size_t SizeOfElements, CompareFunction Compare_Function)
//{
//    while (Last - First > SizeOfElements)
//    {
//        Last -= SizeOfElements;
//        PopHeap(First, Last, Last, SizeOfElements, Compare_Function);
//    }
//}

//static void MakeHeap_##NAME(char* First, char* Last, size_t SizeOfElements, CompareFunction Compare_Function)
//{
//    int Length = (Last - First) / SizeOfElements;    //元素的个数
//    if (Length < 2) return;
//    int LastParent = (Length - 2) / 2;               //最后一个父节点的索引
//    //void* Temp       = (void*)malloc(SizeOfElements);
//    while (true)
//    {
//        memcpy(Temp, First + LastParent * SizeOfElements, SizeOfElements);
//        AdjustHeap(First, LastParent, Length, Temp, SizeOfElements, Compare_Function);    //将以LastParent顶点为根节点的子树大顶堆化
//        if (LastParent == 0) return;
//        LastParent--;
//    }
//    //free(Temp);
//}

//static void HeapSelect_##NAME(char* First, char* Middle, char* Last, size_t SizeOfElements, CompareFunction Compare_Function)
//{
//    //先将现有的前(First-Middle)项构建为一个大顶堆
//    MakeHeap(First, Middle, SizeOfElements, Compare_Function);
//    //然后历遍后面所有的节点，遇到比最大的First节点大的，就弹出First节点
//    for (char* i = Middle; i < Last; i += SizeOfElements)
//    {
//        if (Compare_Function(i, First) > 0) PopHeap(First, Middle, i, SizeOfElements, Compare_Function);
//    }
//}

////此函数是将数列的最大的前(First-Middle)项排序，并且放在前面
//void _PartialSort_##NAME(char* First, char* Middle, char* Last, size_t SizeOfElements, CompareFunction Compare_Function)
//{
//    HeapSelect(First, Middle, Last, SizeOfElements, Compare_Function);    //构建一个大顶堆
//    SortHeap(First, Middle, SizeOfElements, Compare_Function);            //将大顶堆排序
//}

//void HeapSort_##NAME(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Compare_Function) { _PartialSort(Base, Base + NumOfElements * SizeOfElements, Base + NumOfElements * SizeOfElements, SizeOfElements, Compare_Function); }
//
//void PartialSort_##NAME(void* Base, size_t k, size_t NumOfElements, size_t SizeOfElements, CompareFunction Compare_Function)
//{
//    if (k > 0 && k <= NumOfElements) _PartialSort(Base, Base + k * SizeOfElements, Base + NumOfElements * SizeOfElements, SizeOfElements, Compare_Function);
//}
//

///*
//	此插入排序是专门用于一些递归排序算法中用于给元素个数小于S_Threshold的小数组排序用的
//    此插入排序是一个通过每次都将比第一个元素小的元素移动到第一个的位置
//	从而避免边界检查
//*/

//void InsertionSort_##NAME(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Compare_Function) { _InsertionSort_Small(Base, Base + NumOfElements * SizeOfElements, SizeOfElements, Compare_Function); }

///*
//	此插入排序是用来给一个长数组,且已经以S_Threshold长度为单位局部有序
//	现在先将前S_Threshold个元素插入排序
//	后面的元素就可以使用无边界检查的插入排序来增加性能
//*/

//static char* Unguarded_Partition(char* LeftP, char* RightP, char* Pivot, size_t SizeOfElements, CompareFunction Compare_Function)
//{
//    //使用的是Hoare partition scheme
//    while (true)
//    {
//        while (Compare_Function(LeftP, Pivot) < 0) LeftP += SizeOfElements;
//        RightP -= SizeOfElements;    //这里减一个是为了防止越界
//        while (Compare_Function(RightP, Pivot) > 0) RightP -= SizeOfElements;
//        if (!(LeftP < RightP)) return LeftP;
//        SWAP(LeftP, RightP, SizeOfElements);
//        LeftP += SizeOfElements;
//    }
//}
//
//inline static char* Unguarded_PartitionPivot(char* Low, char* High, size_t SizeOfElements, CompareFunction Compare_Function)
//{
//    char* Mid = Low + SizeOfElements * (((High - Low) / SizeOfElements) / 2);
//    MoveMidToFirst(Low, Low + SizeOfElements, Mid, High - SizeOfElements, SizeOfElements, Compare_Function);
//    return Unguarded_Partition(Low + SizeOfElements, High, Low, SizeOfElements, Compare_Function);
//}
//
//static void IntroSort_Loop(char* Low, char* High, unsigned char DepthLimit, size_t SizeOfElements, CompareFunction Compare_Function)
//{
//    while ((High - Low) > S_Threshold * SizeOfElements)
//    {
//        if (DepthLimit == 0)
//        {
//            //使用堆排序
//            _PartialSort(Low, High, High, SizeOfElements, Compare_Function);
//            return;
//        }
//        --DepthLimit;
//        char* Cut = Unguarded_PartitionPivot(Low, High, SizeOfElements, Compare_Function);
//        IntroSort_Loop(Cut, High, DepthLimit, SizeOfElements, Compare_Function);
//        High = Cut;
//    }
//}
//
///*
//	此内省式排序则是几乎将libcstdc++库中的sort函数照搬过来的
//*/
//void IntrospectiveSort(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Compare_Function)
//{
//    if (NumOfElements <= 1) return;
//    IntroSort_Loop(Base, Base + NumOfElements * SizeOfElements, lg2(NumOfElements) * 2, SizeOfElements, Compare_Function);
//    _Final_InsertionSort(Base, Base + NumOfElements * SizeOfElements, SizeOfElements, Compare_Function);
//}

//void SimpleSelectionSort(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Compare_Function)
//{
//    for (int i = 0; i < NumOfElements; ++i)
//    {
//        char* Min = Base + i * SizeOfElements;
//        for (int j = i + 1; j < NumOfElements; ++j)
//        {
//            if (Compare_Function(Min, Base + j * SizeOfElements) > 0) Min = Base + j * SizeOfElements;
//        }
//        if (Base + i * SizeOfElements != Min) SWAP(Base + i * SizeOfElements, Min, SizeOfElements);
//    }
//}

//static void Merge(char* First, char* Mid, char* Last, size_t SizeOfElements, CompareFunction Compare_Function)
//{
//    //如果自然有序则直接退出
//    if (Compare_Function(Mid, Mid + SizeOfElements) <= 0) return;
//    //开始归并
//    char* i = First;
//    char* j = Mid + SizeOfElements;
//    char* k = Temp_Merge;
//    while (i <= Mid && j <= Last)
//    {
//        if (Compare_Function(i, j) <= 0)
//        {
//            memcpy(k, i, SizeOfElements);
//            i += SizeOfElements;
//            k += SizeOfElements;
//        }
//        else
//        {
//            memcpy(k, j, SizeOfElements);
//            j += SizeOfElements;
//            k += SizeOfElements;
//        }
//    }
//    if (i <= Mid) memcpy(k, i, Mid - i + SizeOfElements);
//    if (j <= Last) memcpy(k, j, Last - j + SizeOfElements);
//    memcpy(First, Temp_Merge, Last - First + SizeOfElements);
//}
//
////空间复杂度减到了O(1),但是时间复杂度为O(n^2),不推荐！
//static void Merge_Inplace(char* First, char* Mid, char* Last, size_t SizeOfElements, CompareFunction Compare_Function)
//{
//    char* First_2 = Mid + SizeOfElements;
//    //如果自然有序则直接退出
//    if (Compare_Function(Mid, First_2) <= 0) return;
//    //开始归并
//    while (First <= Mid && First_2 <= Last)
//    {
//        if (Compare_Function(First, First_2) <= 0) First += SizeOfElements;
//        else
//        {
//            memcpy(Temp, First_2, SizeOfElements);
//            char* index = First_2;
//            //将 [First,First_2-SizeOfElements]中的元素向后移动
//            while (index != First)
//            {
//                memcpy(index, index - SizeOfElements, SizeOfElements);
//                index -= SizeOfElements;
//            }
//            memcpy(First, Temp, SizeOfElements);
//            First   += SizeOfElements;
//            Mid     += SizeOfElements;
//            First_2 += SizeOfElements;
//        }
//    }
//}
//
///*
//	一种适用于int型数组的一种奇技淫巧
//	其原理就是,对于两个数a,b,首先判断对Max取余的大小,选取小的一个数
//	然后令a = a + (min(a % Max, b % max)) * Max
//	这样在原本数所在的位置,对Max的商,就是排序好了的数值,而取余则是原本所在的数值
//	但是在计算a = a + (min(a % Max, b % max)) * Max时要特别注意数值溢出!
//*/
//static void Merge_Inplace_Int(int* First, int* Mid, int* Last)
//{
//    //首先找到待归并的数组中最大的元素并加1
//    int Max = *First;
//    for (int* i = First; i <= Last; i += 1)
//    {
//        if (*i > Max) Max = *i;
//    }
//    ++Max;
//    int i = 0;
//    int j = Mid - First + 1;
//    int k = 0;
//    while (First + i <= Mid && First + j <= Last)
//    {
//        if (First[i] % Max <= First[j] % Max)
//        {
//            First[k] = First[k] + (First[i] % Max) * Max;
//            k++;
//            i++;
//        }
//        else
//        {
//            First[k] = First[k] + (First[j] % Max) * Max;
//            k++;
//            j++;
//        }
//    }
//    while (First + i <= Mid)
//    {
//        First[k] = First[k] + (First[i] % Max) * Max;
//        k++;
//        i++;
//    }
//    while (First + j <= Last)
//    {
//        First[k] = First[k] + (First[j] % Max) * Max;
//        k++;
//        j++;
//    }
//
//    // 获取原始的有序序列
//    for (int* i = First; i <= Last; i += 1) { *i = *i / Max; }
//}
//
//static void MergeSort_Recursion_Loop(char* First, char* Last, size_t SizeOfElements, CompareFunction Compare_Function)
//{
//    int NumOfElements = (Last - First) / SizeOfElements + 1;
//    if (NumOfElements == 1) return;
//    else if (NumOfElements >= S_Threshold)    //对小数组采用插入排序
//    {
//        char* Mid = First + ((NumOfElements >> 1) - 1) * SizeOfElements;
//        MergeSort_Recursion_Loop(First, Mid, SizeOfElements, Compare_Function);
//        MergeSort_Recursion_Loop(Mid + SizeOfElements, Last, SizeOfElements, Compare_Function);
//        Merge(First, Mid, Last, SizeOfElements, Compare_Function);
//    }
//    else _InsertionSort_Small(First, First + SizeOfElements * NumOfElements, SizeOfElements, Compare_Function);
//}
//
//void MergeSort_Recursion(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Compare_Function)
//{
//    Temp_Merge = (char*)malloc(NumOfElements * SizeOfElements);
//    MergeSort_Recursion_Loop(Base, Base + (NumOfElements - 1) * SizeOfElements, SizeOfElements, Compare_Function);
//    free(Temp_Merge);
//}
//
//void MergeSort_Iterative(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Compare_Function)
//{
//    Temp_Merge = (char*)malloc(NumOfElements * SizeOfElements);
//    int   CurrentSize;                                                                                                                                                                                                    //标识当前合并的子数组的大小，从1到NumOfElements/2
//    char* First;                                                                                                                                                                                                          //标识当前要合并的子数组的起点
//    for (CurrentSize = 1; CurrentSize <= NumOfElements - 1; CurrentSize = 2 * CurrentSize)
//    {
//        for (First = Base; First < (char*)Base + (NumOfElements - 1) * SizeOfElements; First += (2 * CurrentSize) * SizeOfElements)                                                                                       //First必须在最后一个元素的前面,这样后面或者还有一组或者直接就是最后一个元素了
//        {
//            char* Mid  = (First + (CurrentSize - 1) * SizeOfElements < (char*)Base + (NumOfElements - 1) * SizeOfElements) ? First + (CurrentSize - 1) * SizeOfElements : Base + (NumOfElements - 1) * SizeOfElements;    //找到第一组数组的最后一个元素
//            char* Last = (Mid + CurrentSize * SizeOfElements < (char*)Base + (NumOfElements - 1) * SizeOfElements) ? Mid + CurrentSize * SizeOfElements : Base + (NumOfElements - 1) * SizeOfElements;                    //找到第二组数组的最后的元素
//            Merge(First, Mid, Last, SizeOfElements, Compare_Function);
//        }
//    }
//    free(Temp_Merge);
//}
//
//void MergeSort_Inplace_Iterative(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Compare_Function)
//{
//    int   CurrentSize;                                                                                                                                                                                                    //标识当前合并的子数组的大小，从1到NumOfElements/2
//    char* First;                                                                                                                                                                                                          //标识当前要合并的子数组的起点
//    for (CurrentSize = 1; CurrentSize <= NumOfElements - 1; CurrentSize = 2 * CurrentSize)
//    {
//        for (First = Base; First < (char*)Base + (NumOfElements - 1) * SizeOfElements; First += (2 * CurrentSize) * SizeOfElements)                                                                                       //First必须在最后一个元素的前面,这样后面或者还有一组或者直接就是最后一个元素了
//        {
//            char* Mid  = (First + (CurrentSize - 1) * SizeOfElements < (char*)Base + (NumOfElements - 1) * SizeOfElements) ? First + (CurrentSize - 1) * SizeOfElements : Base + (NumOfElements - 1) * SizeOfElements;    //找到第一组数组的最后一个元素
//            char* Last = (Mid + CurrentSize * SizeOfElements < (char*)Base + (NumOfElements - 1) * SizeOfElements) ? Mid + CurrentSize * SizeOfElements : Base + (NumOfElements - 1) * SizeOfElements;                    //找到第二组数组的最后的元素
//            Merge_Inplace(First, Mid, Last, SizeOfElements, Compare_Function);
//        }
//    }
//}
//
//void MergeSort_Inplace_Iterative_For_Int(int* Base, size_t NumOfElements)
//{
//    int  CurrentSize;                                                                                                                 //标识当前合并的子数组的大小，从1到NumOfElements/2
//    int* First;                                                                                                                       //标识当前要合并的子数组的起点
//    for (CurrentSize = 1; CurrentSize <= NumOfElements - 1; CurrentSize = 2 * CurrentSize)
//    {
//        for (First = Base; First < &Base[NumOfElements - 1]; First += (2 * CurrentSize))                                              //First必须在最后一个元素的前面,这样后面或者还有一组或者直接就是最后一个元素了
//        {
//            int* Mid  = (&First[CurrentSize - 1] < &Base[NumOfElements - 1]) ? &First[CurrentSize - 1] : &Base[NumOfElements - 1];    //找到第一组数组的最后一个元素
//            int* Last = (&Mid[CurrentSize] < &Base[NumOfElements - 1]) ? &Mid[CurrentSize] : &Base[NumOfElements - 1];                //找到第二组数组的最后的元素
//            Merge_Inplace_Int(First, Mid, Last);
//        }
//    }
//}
//
///*
//	此排序相当于从最低位到最高位对int型关键值进行计数排序
//	每一位最多10个不同的元素,就开了10个桶
//*/
//void RadixSort_LSD(void* Base, size_t NumOfElements, size_t SizeOfElements, ValueFunction GetValue)
//{
//    if (NumOfElements <= 1) return;
//    //将每一个元素对应的数字先提取出来方便后续使用
//    int* Value = (int*)malloc(NumOfElements * sizeof(int));
//    if (Value == NULL)
//    {
//        printf("内存分配失败！\n");
//        return;
//    }
//    for (int i = 0; i < NumOfElements; ++i) Value[i] = GetValue(Base + i * SizeOfElements);
//    //创建统计数组并统计对应元素的个数
//    size_t CountArray[10] = {0};
//    void*  OrderedArray   = (void*)malloc(NumOfElements * SizeOfElements);
//    if (OrderedArray == NULL)
//    {
//        printf("内存分配失败！\n");
//        return;
//    }
//    //求出最大值
//    int Max = Value[0];
//    for (int i = 0; i < NumOfElements; ++i)
//    {
//        if (Value[i] > Max) Max = Value[i];
//    }
//    //求出所有数的最大位数
//    int Digits = (Max == 0) ? 1 : (int)log10(Max) + 1;
//    //最大位数的个数,进行几次计数排序
//    for (int i = 1; i <= Digits; ++i)
//    {
//        //将每一个元素对应的数字先提取出来方便后续使用
//        for (int i = 0; i < NumOfElements; ++i) Value[i] = GetValue(Base + i * SizeOfElements);
//        //统计对应元素的个数
//        for (int j = 0; j < NumOfElements; ++j) ++CountArray[(Value[j] / (int)pow(10, i - 1)) % 10];
//        //统计数组做变形,后面的元素等于前面的元素之和
//        for (int j = 1; j <= 9; ++j) CountArray[j] += CountArray[j - 1];
//        //取出每个桶中的元素，赋值给数组
//        for (int j = NumOfElements - 1; j >= 0; --j) memcpy(OrderedArray + (--CountArray[(Value[j] / (int)pow(10, i - 1)) % 10]) * SizeOfElements, Base + j * SizeOfElements, SizeOfElements);
//        memcpy(Base, OrderedArray, NumOfElements * SizeOfElements);
//        memset(CountArray, 0, 10 * sizeof(size_t));    //将计数数组归零
//    }
//    //排序完成
//    free(Value);
//    free(OrderedArray);
//}
//
//static void RadixSort_MSD_Loop(int* Base, size_t NumOfElements, int Radix)
//{
//    if (Radix <= 0) return;
//    //10进制数,有10个桶,每个桶最多存NumOfElements个数
//    int* Bucket[10];
//    for (int i = 0; i < 10; ++i)
//    {
//        Bucket[i] = (int*)malloc(NumOfElements * sizeof(int));
//        if (Bucket[i] == NULL)
//        {
//            printf("内存分配失败！\n");
//            return;
//        }
//    }
//    //用来计算,统计每个桶所存的元素的个数,每个桶对应一个元素
//    int BucketElementCounts[10] = {0};
//    for (int i = 0; i < NumOfElements; ++i)
//    {
//        //求的是第i个数对应基数是Radix的数码的值
//        int Value                                 = Base[i] / Radix % 10;
//        Bucket[Value][BucketElementCounts[Value]] = Base[i];
//        ++BucketElementCounts[Value];
//    }
//    int index = 0;    //Base新的下标
//    //遍历每个桶,如果桶里面有2个以上的元素,就把这个桶再一次RadixSort_MSD_Loop排序
//    for (int i = 0; i < 10; ++i)
//    {
//        if (BucketElementCounts[i] > 1) RadixSort_MSD_Loop(Bucket[i], BucketElementCounts[i], Radix / 10);
//        for (int j = 0; j < BucketElementCounts[i]; ++j)
//        {
//            Base[index] = Bucket[i][j];
//            ++index;
//        }
//    }
//    for (int i = 0; i < 10; ++i) free(Bucket[i]);
//}
//
//void RadixSort_MSD(int* Base, size_t NumOfElements)
//{
//    if (NumOfElements <= 1) return;
//    //求出最大值
//    int Max = Base[0];
//    for (int i = 0; i < NumOfElements; ++i)
//    {
//        if (Base[i] > Max) Max = Base[i];
//    }
//    //求出最大值的位数
//    int Digits = (Max == 0) ? 1 : (int)log10(Max) + 1;
//    //计算最大值的基数
//    int Radix  = (int)pow(10, Digits - 1);
//    RadixSort_MSD_Loop(Base, NumOfElements, Radix);
//}
//
//void CountingSort(void* Base, size_t NumOfElements, size_t SizeOfElements, ValueFunction GetValue)
//{
//    if (NumOfElements <= 1) return;
//    //将每一个元素对应的数字先提取出来方便后续使用
//    int* Value = (int*)calloc(NumOfElements, sizeof(int));
//    if (Value == NULL)
//    {
//        printf("内存分配失败！\n");
//        return;
//    }
//    for (int i = 0; i < NumOfElements; ++i) Value[i] = GetValue(Base + i * SizeOfElements);
//    //得到数列的最大值和最小值，并算出差值Delta
//    int Max = Value[0];
//    int Min = Value[0];
//    for (int i = 0; i < NumOfElements; ++i)
//    {
//        if (Value[i] > Max) Max = Value[i];
//        if (Value[i] < Min) Min = Value[i];
//    }
//    int Delta          = Max - Min;
//    //创建统计数组并统计对应元素的个数
//    size_t* CountArray = (size_t*)calloc((size_t)(Delta + 1), sizeof(size_t));
//    if (CountArray == NULL)
//    {
//        printf("内存分配失败！\n");
//        return;
//    }
//    for (int i = 0; i < NumOfElements; ++i) ++CountArray[Value[i] - Min];
//    //统计数组做变形,后面的元素等于前面的元素之和
//    for (int i = 1; i <= Delta; ++i) CountArray[i] += CountArray[i - 1];
//    void* OrderedArray = (void*)malloc(NumOfElements * SizeOfElements);
//    if (OrderedArray == NULL)
//    {
//        printf("内存分配失败！\n");
//        return;
//    }
//    for (int i = NumOfElements - 1; i >= 0; --i) memcpy(OrderedArray + (--CountArray[Value[i] - Min]) * SizeOfElements, Base + i * SizeOfElements, SizeOfElements);
//    memcpy(Base, OrderedArray, NumOfElements * SizeOfElements);
//    free(Value);
//    free(CountArray);
//    free(OrderedArray);
//}
//
//void BucketSort(void* Base, size_t NumOfElements, size_t SizeOfElements, ValueFunction GetValue)
//{
//    if (NumOfElements <= 1) return;
//    //将每一个元素对应的数字先提取出来方便后续使用
//    int* Value = (int*)malloc(NumOfElements * sizeof(int));
//    if (Value == NULL)
//    {
//        printf("内存分配失败！\n");
//        return;
//    }
//    for (int i = 0; i < NumOfElements; ++i) Value[i] = GetValue(Base + i * SizeOfElements);
//    //得到数列的最大值和最小值，并算出差值Delta
//    int Max = Value[0];
//    int Min = Value[0];
//    for (int i = 0; i < NumOfElements; ++i)
//    {
//        if (Value[i] > Max) Max = Value[i];
//        if (Value[i] < Min) Min = Value[i];
//    }
//    //建立桶,每个桶里面放入元素的范围是(Max - Min) / Bucket_NUM + 1
//    void* Bucket[Bucket_NUM];
//    for (int i = 0; i < Bucket_NUM; ++i)
//    {
//        Bucket[i] = (void*)malloc(NumOfElements * SizeOfElements);
//        if (Bucket[i] == NULL)
//        {
//            printf("内存分配失败！\n");
//            return;
//        }
//    }
//    //用来记录每一个桶里面的元素个数
//    size_t* CountArray = (size_t*)calloc(Bucket_NUM, sizeof(size_t));
//    if (CountArray == NULL)
//    {
//        printf("内存分配失败！\n");
//        return;
//    }
//    int BucketLength = ceil((double)(Max - Min + 1) / Bucket_NUM);    //向上取整
//    //将Base数组里面的每一个元素映射到桶里面
//    for (int i = 0; i < NumOfElements; ++i)
//    {
//        int index = (Value[i] - Min) / BucketLength;
//        memcpy((char*)Bucket[index] + (++CountArray[index] - 1) * SizeOfElements, (char*)Base + i * SizeOfElements, SizeOfElements);
//    }
//    //对每一个桶进行计数排序(快上加快)
//    int TotalNum = 0;
//    for (int i = 0; i < Bucket_NUM; ++i)
//    {
//        CountingSort(Bucket[i], CountArray[i], SizeOfElements, GetValue);
//        //把每个桶里面的元素放回Base数组里面
//        memcpy(Base + TotalNum * SizeOfElements, Bucket[i], CountArray[i] * SizeOfElements);
//        TotalNum += CountArray[i];
//    }
//    free(Value);
//    for (int i = 0; i < Bucket_NUM; ++i) free(Bucket[i]);
//}
//
///*
//	注意与计数排序的区别, 此排序只考虑每一个元素出现的次数,并且只能排序int数组
//	本来PigeonholeSort是直接将int值直接映射到数值的索引上,并在上面记录个数
//	然后再历遍一次数组将数全部放到原数组里面就可以了
//	但是为了全部开空间INT_MAX也开不了(16GB),于是有了下面的改进算法
//	与计数排序的区别就是他不实际记录每一个元素的位置,而是个数,最后依次输出即可
//*/
//
//void PigeonholeSort(int* Base, size_t NumOfElements)
//{
//    if (NumOfElements <= 1) return;
//    //得到数列的最大值和最小值，并算出差值Delta
//    int Max = Base[0];
//    int Min = Base[0];
//    for (int i = 0; i < NumOfElements; ++i)
//    {
//        if (Base[i] > Max) Max = Base[i];
//        if (Base[i] < Min) Min = Base[i];
//    }
//    int Delta       = Max - Min;
//    //创建统计数组并统计对应元素的个数
//    int* CountArray = (int*)calloc((Delta + 1), sizeof(int));
//    if (CountArray == NULL)
//    {
//        printf("内存分配失败！\n");
//        return;
//    }
//    for (int i = 0; i < NumOfElements; ++i) ++CountArray[Base[i] - Min];
//    int index = 0;
//    for (int i = 0; i <= Delta; ++i)
//    {
//        while (CountArray[i] != 0)
//        {
//            Base[index] = i + Min;
//            --CountArray[i];
//            ++index;
//        }
//    }
//    free(CountArray);
//}
//
//void BigoSort(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Compare_Function)
//{
//A:
//    for (int i = 0; i < NumOfElements - 1; ++i)
//    {
//        if (Compare_Function(Base + i * SizeOfElements, Base + (i + 1) * SizeOfElements) > 0)
//        {
//            for (int j = NumOfElements - 1; j > 0; --j)
//            {
//                int ChangeIndex = rand() % j;
//                SWAP(Base + j * SizeOfElements, Base + ChangeIndex * SizeOfElements, SizeOfElements);
//            }
//            goto A;
//        }
//    }
//}

// 全局变量
static int HibbardStepArray[31]   = {2147483647, 1073741823, 536870911, 268435455, 134217727, 67108863, 33554431, 16777215, 8388607, 4194303, 2097151, 1048575, 524287, 262143, 131071, 65535, 32767, 16383, 8191, 4095, 2047, 1023, 511, 255, 127, 63, 31, 15, 7, 3, 1};
static int SedgewickStepArray[14] = {603906049, 150958081, 37730305, 9427969, 2354689, 587521, 146305, 36289, 8929, 2161, 505, 109, 19, 1};

// 内联函数
static LTT_unused int* GetHibbardStepArray(int Length)
{
    for (int i = 0;; ++i)
    {
        if (HibbardStepArray[i] <= Length) { return &HibbardStepArray[i]; }
    }
}

static LTT_unused int* GetSedgewickStepArray(int Length)
{
    for (int i = 0;; ++i)
    {
        if (SedgewickStepArray[i] <= Length) { return &HibbardStepArray[i]; }
    }
}

static LTT_inline LTT_unused unsigned char lg2(size_t n)
{
    unsigned char log_val = 0;
    while (n >>= 1) { ++log_val; }
    return log_val;
}

#define SORT_INIT(NAME, TYPE, SCOPE, Compare_Function) \
    SORT_TYPE(NAME, TYPE)                              \
    SORT_IMPL(NAME, TYPE, SCOPE, Compare_Function)


//###########基于插入##########
//插入排序
#define InsertionSort(NAME, Array, Number)                       InsertionSort_##NAME((Array), (Number))
//折半插入
#define BinaryInsertionSort(NAME, Array, Number)                 BinaryInsertionSort_##NAME((Array), (Number))
//希尔排序
#define ShellInsertionSort_Hibbard(NAME, Array, Number)          ShellInsertionSort_Hibbard_##NAME((Array), (Number))
#define ShellInsertionSort_Sedgewick(NAME, Array, Number)        ShellInsertionSort_Sedgewick_##NAME((Array), (Number))
//###########基于交换##########
//冒泡排序
#define BubbleSort(NAME, Array, Number)                          BubbleSort_##NAME((Array), (Number))
#define BubbleSort_Fast(NAME, Array, Number)                     BubbleSort_Fast_##NAME((Array), (Number))
//快速排序
#define QuickSort_glibc(NAME, Array, Number)                     QuickSort_glibc_##NAME((Array), (Number))
#define QuickSort_LTT_glibc(NAME, Array, Number)                 QuickSort_LTT_glibc_##NAME((Array), (Number))
#define QuickSort_LTT_libstdcpp(NAME, Array, Number)             QuickSort_LTT_libstdcpp_##NAME((Array), (Number))
//###########基于选择##########
//简单选择排序
#define SimpleSelectionSort(NAME, Array, Number)                 SimpleSelectionSort_##NAME((Array), (Number))
//堆排序
#define HeapSort(NAME, Array, Number)                            HeapSort_##NAME((Array), (Number))
#define PartialSort(NAME, Array, Number)                         PartialSort_##NAME((Array), (Number))
//###########基于归并##########
//2-路归并排序
#define MergeSort_Recursion(NAME, Array, Number)                 MergeSort_Recursion_##NAME((Array), (Number))
#define MergeSort_Iterative(NAME, Array, Number)                 MergeSort_Iterative_##NAME((Array), (Number))
#define MergeSort_Inplace_Iterative(NAME, Array, Number)         MergeSort_Inplace_Iterative_##NAME((Array), (Number))
#define MergeSort_Inplace_Iterative_For_Int(NAME, Array, Number) MergeSort_Inplace_Iterative_For_Int_##NAME((Array), (Number))
// k路归并排序
//  Todo
//
////////////

//基数排序
#define RadixSort_LSD(NAME, Array, Number)                       RadixSort_LSD_##NAME((Array), (Number))
#define RadixSort_MSD(NAME, Array, Number)                       RadixSort_MSD_##NAME((Array), (Number))
//桶排序
#define BucketSort(NAME, Array, Number)                          BucketSort_##NAME((Array), (Number))
//计数排序
#define CountingSort(NAME, Array, Number)                        CountingSort_##NAME((Array), (Number))
//鸽巢排序
#define CountingSort(NAME, Array, Number)                        CountingSort_##NAME((Array), (Number))
//猴子排序
#define BigoSort(NAME, Array, Number)                            BigoSort_##NAME((Array), (Number))
//内省式排序
#define IntrospectiveSort(NAME, Array, Number)                   IntrospectiveSort_##NAME((Array), (Number))

// 函数实现
#define LTT_SORT_INIT(NAME, TYPE, Compare_Function)              SORT_INIT(NAME, TYPE, static LTT_unused, Compare_Function)

// 函数声明
#define LTT_SORT_DECLARE(NAME, TYPE)                             SORT_DECLARE(NAME, TYPE)