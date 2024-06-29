#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "Generic_tool.h"


// The Macro to control the swap mode
#ifndef SWAPMODE
    #define SWAPMODE 1
#endif

// A simple macro to get the minimum value
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

// Maximum length of each sub-array (4 elements in size), used in the QuickSort_glibc function.
#define MAX_THRESH  16
#define S_Threshold 16
#define Bucket_NUM  8

// Stack node declarations used to store unfulfilled partition obligations.
#define SORT_TYPE(NAME, TYPE)        \
    typedef struct stack_node_##NAME \
    {                                \
        TYPE* Low;                   \
        TYPE* High;                  \
    } stack_node_##NAME;

// The next 4 #defines implement a very fast in-line stack abstraction.
/*
 * The stack needs log (total_elements) entries (we could even subtract log(MAX_THRESH)).
 * Since total_elements has type size_t, we get as upper bound for log (total_elements):
 * bits per byte (CHAR_BIT) * sizeof(size_t).
*/
#define STACK_SIZE          (CHAR_BIT * sizeof(size_t))
#define PUSH(LeftP, RightP) ((void)((Top->Low = (LeftP)), (Top->High = (RightP)), ++Top))
#define POP(LeftP, RightP)  ((void)(--Top, ((LeftP) = Top->Low), ((RightP) = Top->High)))
#define STACK_NOT_EMPTY     (Stack < Top)



#define SORT_IMPL(NAME, TYPE, SCOPE, Compare_Function)                                                                             \
    SCOPE void MoveMidToFirst_##NAME(TYPE* Result, TYPE* a, TYPE* b, TYPE* c)                                                      \
    {                                                                                                                              \
        if (Compare_Function(a, b) < 0)                                                                                            \
        {                                                                                                                          \
            if (Compare_Function(b, c) < 0) SWAP(Result, b, TYPE);                                                                 \
            else if (Compare_Function(a, c)) SWAP(Result, c, TYPE);                                                                \
            else SWAP(Result, a, TYPE);                                                                                            \
        }                                                                                                                          \
        else if (Compare_Function(a, c) < 0) SWAP(Result, a, TYPE);                                                                \
        else if (Compare_Function(b, c) < 0) SWAP(Result, c, TYPE);                                                                \
        else SWAP(Result, b, TYPE);                                                                                                \
    }                                                                                                                              \
    SCOPE void BinaryInsertionSort_##NAME(TYPE* Base, size_t NumOfElements)                                                        \
    {                                                                                                                              \
        if (NumOfElements <= 0) return;                                                                                            \
        TYPE Temp;                                                                                                                 \
        for (size_t i = 1; i < NumOfElements; ++i)                                                                                 \
        {                                                                                                                          \
            Temp = Base[i];                                                                                                        \
            int Position;                                                                                                          \
            int Low  = 0;                                                                                                          \
            int High = i - 1;                                                                                                      \
            while (Low <= High)                                                                                                    \
            {                                                                                                                      \
                int Mid   = (Low + High) >> 1;                                                                                     \
                int Delta = Compare_Function(&Temp, &Base[Mid]);                                                                   \
                if (Delta == 0)                                                                                                    \
                {                                                                                                                  \
                    Position = Mid + 1;                                                                                            \
                    goto A;                                                                                                        \
                }                                                                                                                  \
                else if (Delta < 0) High = Mid - 1;                                                                                \
                else Low = Mid + 1;                                                                                                \
            }                                                                                                                      \
            Position = High + 1;                                                                                                   \
        A:                                                                                                                         \
            for (int j = i - 1; j >= Position; --j) Base[j + 1] = Base[j];                                                         \
            Base[Position] = Temp;                                                                                                 \
        }                                                                                                                          \
    }                                                                                                                              \
    SCOPE void ShellInsertionSort_Hibbard_##NAME(TYPE* Base, size_t NumOfElements)                                                 \
    {                                                                                                                              \
        if (NumOfElements <= 0) return;                                                                                            \
        int* GapArray = GetHibbardStepArray(NumOfElements);                                                                        \
        TYPE Temp;                                                                                                                 \
        for (int i = 0;; ++i)                                                                                                      \
        {                                                                                                                          \
            int Gap = GapArray[i];                                                                                                 \
            for (size_t j = Gap; j < NumOfElements; ++j)                                                                           \
            {                                                                                                                      \
                Temp         = Base[j];                                                                                            \
                int Position = j - Gap;                                                                                            \
                while (Position >= 0 && Compare_Function(&Base[Position], &Temp) > 0)                                              \
                {                                                                                                                  \
                    Base[Position + Gap]  = Base[Position];                                                                        \
                    Position             -= Gap;                                                                                   \
                }                                                                                                                  \
                Base[Position + Gap] = Temp;                                                                                       \
            }                                                                                                                      \
            if (Gap == 1) break;                                                                                                   \
        }                                                                                                                          \
    }                                                                                                                              \
    SCOPE void ShellInsertionSort_Sedgewick_##NAME(TYPE* Base, size_t NumOfElements)                                               \
    {                                                                                                                              \
        if (NumOfElements <= 0) return;                                                                                            \
        int* GapArray = GetSedgewickStepArray(NumOfElements);                                                                      \
        TYPE Temp;                                                                                                                 \
        for (int i = 0;; ++i)                                                                                                      \
        {                                                                                                                          \
            int Gap = GapArray[i];                                                                                                 \
            for (size_t j = Gap; j < NumOfElements; ++j)                                                                           \
            {                                                                                                                      \
                Temp         = Base[j];                                                                                            \
                int Position = j - Gap;                                                                                            \
                while (Position >= 0 && Compare_Function(&Base[Position], &Temp) > 0)                                              \
                {                                                                                                                  \
                    Base[Position + Gap]  = Base[Position];                                                                        \
                    Position             -= Gap;                                                                                   \
                }                                                                                                                  \
                Base[Position + Gap] = Temp;                                                                                       \
            }                                                                                                                      \
            if (Gap == 1) break;                                                                                                   \
        }                                                                                                                          \
    }                                                                                                                              \
    SCOPE void BubbleSort_##NAME(TYPE* Base, size_t NumOfElements)                                                                 \
    {                                                                                                                              \
        if (NumOfElements <= 0) return;                                                                                            \
        /* Need to sort NumOfElements-1 times in total */                                                                          \
        for (size_t i = 0; i < NumOfElements - 1; ++i)                                                                             \
        {                                                                                                                          \
            bool Ordered = true;                                                                                                   \
            /* Select the largest value in this round of sorting and move it to the back */                                        \
            for (size_t j = 0; j < NumOfElements - 1 - i; j++)                                                                     \
            {                                                                                                                      \
                if (Compare_Function(&Base[j], &Base[j + 1]) > 0)                                                                  \
                {                                                                                                                  \
                    SWAP(&Base[j], &Base[j + 1], TYPE);                                                                            \
                    /* If any swap happens, set Ordered to false */                                                                \
                    Ordered = false;                                                                                               \
                }                                                                                                                  \
            }                                                                                                                      \
            if (Ordered) break;                                                                                                    \
        }                                                                                                                          \
    }                                                                                                                              \
    SCOPE void BubbleSort_Fast_##NAME(TYPE* Base, size_t NumOfElements)                                                            \
    {                                                                                                                              \
        if (NumOfElements <= 0) return;                                                                                            \
        bool Ordered;                                                                                                              \
        /* Used to record the position of the last swap of the largest number */                                                   \
        size_t HighPos = NumOfElements - 1;                                                                                        \
        /* Used to record the position of the last swap of the smallest number */                                                  \
        size_t LowPos  = 0;                                                                                                        \
        /* Need to sort NumOfElements-1 times in total */                                                                          \
        for (size_t i = 0; i < NumOfElements - 1; ++i)                                                                             \
        {                                                                                                                          \
            Ordered            = true;                                                                                             \
            /* Find the maximum value in the forward direction */                                                                  \
            size_t TempHighPos = HighPos;                                                                                          \
            for (size_t j = LowPos; j < HighPos; ++j)                                                                              \
            {                                                                                                                      \
                if (Compare_Function(&Base[j], &Base[j + 1]) > 0)                                                                  \
                {                                                                                                                  \
                    SWAP(&Base[j], &Base[j + 1], TYPE);                                                                            \
                    /* If any swap happens, set Ordered to false */                                                                \
                    Ordered     = false;                                                                                           \
                    /* Record the position of the last swap */                                                                     \
                    TempHighPos = j;                                                                                               \
                }                                                                                                                  \
            }                                                                                                                      \
            if (Ordered) break;                                                                                                    \
            HighPos           = TempHighPos;                                                                                       \
            size_t TempLowPos = LowPos;                                                                                            \
            for (size_t j = HighPos; j > LowPos; --j)                                                                              \
            {                                                                                                                      \
                if (Compare_Function(&Base[j], &Base[j - 1]) < 0)                                                                  \
                {                                                                                                                  \
                    SWAP(&Base[j], &Base[j - 1], TYPE);                                                                            \
                    /* If any swap happens, set Ordered to false */                                                                \
                    Ordered    = false;                                                                                            \
                    /* Record the position of the last swap */                                                                     \
                    TempLowPos = j;                                                                                                \
                }                                                                                                                  \
            }                                                                                                                      \
            if (Ordered) break;                                                                                                    \
            LowPos = TempLowPos;                                                                                                   \
        }                                                                                                                          \
    }                                                                                                                              \
    SCOPE void QuickSort_glibc_##NAME(TYPE* Base, size_t NumOfElements)                                                            \
    {                                                                                                                              \
        TYPE* Base_ptr = Base;                                                                                                     \
        if (NumOfElements == 0) return;                                                                                            \
        if (NumOfElements > MAX_THRESH)                                                                                            \
        {                                                                                                                          \
            TYPE*              Low  = Base_ptr;                                                                                    \
            TYPE*              High = &Low[NumOfElements - 1];                                                                     \
            stack_node_##NAME  Stack[STACK_SIZE];                                                                                  \
            stack_node_##NAME* Top = Stack;                                                                                        \
            PUSH(NULL, NULL);                                                                                                      \
            while (STACK_NOT_EMPTY)                                                                                                \
            {                                                                                                                      \
                TYPE* Left_ptr;                                                                                                    \
                TYPE* Right_ptr;                                                                                                   \
                TYPE* Mid = Low + ((High - Low) >> 1);                                                                             \
                /* Sort Low, High, Mid */                                                                                          \
                if (Compare_Function(Mid, Low) < 0) SWAP(Mid, Low, TYPE);                                                          \
                if (Compare_Function(High, Mid) < 0) SWAP(Mid, High, TYPE);                                                        \
                else goto A;                                                                                                       \
                if (Compare_Function(Mid, Low) < 0) SWAP(Mid, Low, TYPE);                                                          \
            A:                                                                                                                     \
                /* Skip the first element with Left_ptr */                                                                         \
                Left_ptr  = Low + 1;                                                                                               \
                /* Skip the last element with Right_ptr */                                                                         \
                Right_ptr = High - 1;                                                                                              \
                do {                                                                                                               \
                    while (Compare_Function(Left_ptr, Mid) < 0) ++Left_ptr;                                                        \
                    while (Compare_Function(Mid, Right_ptr) < 0) --Right_ptr;                                                      \
                    if (Left_ptr < Right_ptr)                                                                                      \
                    {                                                                                                              \
                        SWAP(Left_ptr, Right_ptr, TYPE);                                                                           \
                        if (Mid == Left_ptr) Mid = Right_ptr;                                                                      \
                        else if (Mid == Right_ptr) Mid = Left_ptr;                                                                 \
                        ++Left_ptr;                                                                                                \
                        --Right_ptr;                                                                                               \
                    }                                                                                                              \
                    else if (Left_ptr == Right_ptr)                                                                                \
                    {                                                                                                              \
                        ++Left_ptr;                                                                                                \
                        --Right_ptr;                                                                                               \
                        break;                                                                                                     \
                    }                                                                                                              \
                }                                                                                                                  \
                while (Left_ptr <= Right_ptr);                                                                                     \
                /* Push the unsorted parts onto the stack */                                                                       \
                /* Left side is small */                                                                                           \
                if ((size_t)(Right_ptr - Low) <= MAX_THRESH)                                                                       \
                {                                                                                                                  \
                    /* Left side is small, right side is small, directly pop */                                                    \
                    if ((size_t)(High - Left_ptr) <= MAX_THRESH) POP(Low, High);                                                   \
                    /* Left side is small, right side is large, start from the right side */                                       \
                    else Low = Left_ptr;                                                                                           \
                }                                                                                                                  \
                /* Left side is large, right side is small, start from the left side */                                            \
                else if ((size_t)(High - Left_ptr) <= MAX_THRESH) High = Right_ptr;                                                \
                /* Both sides are large, left side is greater than right side */                                                   \
                else if ((Right_ptr - Low) > (High - Left_ptr))                                                                    \
                {                                                                                                                  \
                    /* Push the left side onto the stack, sort the right side first */                                             \
                    PUSH(Low, Right_ptr);                                                                                          \
                    Low = Left_ptr;                                                                                                \
                }                                                                                                                  \
                else                                                                                                               \
                {                                                                                                                  \
                    /* Push the right side onto the stack, sort the left side first */                                             \
                    PUSH(Left_ptr, High);                                                                                          \
                    High = Right_ptr;                                                                                              \
                }                                                                                                                  \
            }                                                                                                                      \
        }                                                                                                                          \
        /* The following section moves elements greater than Run_ptr backwards */                                                  \
        TYPE* const End_ptr = &Base_ptr[NumOfElements - 1];                                                                        \
        TYPE*       Tmp_ptr = Base_ptr;                                                                                            \
        TYPE*       Thresh  = MIN(End_ptr, Base_ptr + MAX_THRESH);                                                                 \
        TYPE*       Run_ptr;                                                                                                       \
        for (Run_ptr = Tmp_ptr + 1; Run_ptr <= Thresh; ++Run_ptr)                                                                  \
        {                                                                                                                          \
            if (Compare_Function(Run_ptr, Tmp_ptr) < 0) Tmp_ptr = Run_ptr;                                                         \
        }                                                                                                                          \
        if (Tmp_ptr != Base_ptr) SWAP(Tmp_ptr, Base_ptr, TYPE);                                                                    \
        Run_ptr = Base_ptr + 1;                                                                                                    \
        while (++Run_ptr <= End_ptr)                                                                                               \
        {                                                                                                                          \
            Tmp_ptr = Run_ptr - 1;                                                                                                 \
            while (Compare_Function(Run_ptr, Tmp_ptr) < 0) --Tmp_ptr;                                                              \
            ++Tmp_ptr;                                                                                                             \
            if (Tmp_ptr != Run_ptr)                                                                                                \
            {                                                                                                                      \
                TYPE* Trav;                                                                                                        \
                Trav = Run_ptr + 1;                                                                                                \
                while (--Trav >= Run_ptr)                                                                                          \
                {                                                                                                                  \
                    TYPE  c = *Trav;                                                                                               \
                    TYPE *High, *Low;                                                                                              \
                    for (High = Low = Trav; (--Low) >= Tmp_ptr; High = Low) *High = *Low;                                          \
                    *High = c;                                                                                                     \
                }                                                                                                                  \
            }                                                                                                                      \
        }                                                                                                                          \
    }                                                                                                                              \
    SCOPE void _Unguarded_LinearInsert_##NAME(TYPE* Last)                                                                          \
    {                                                                                                                              \
        TYPE  Temp = *Last;                                                                                                        \
        TYPE* Next = Last;                                                                                                         \
        --Next;                                                                                                                    \
        while (Compare_Function(&Temp, Next) < 0)                                                                                  \
        {                                                                                                                          \
            *Last = *Next;                                                                                                         \
            Last  = Next;                                                                                                          \
            --Next;                                                                                                                \
        }                                                                                                                          \
        *Last = Temp;                                                                                                              \
    }                                                                                                                              \
    /* This insertion sort is specifically used in some recursive sorting algorithms */                                            \
    /* to sort small arrays with fewer than S_Threshold elements */                                                                \
    /* This insertion sort works by continuously moving elements smaller than the first element */                                 \
    /* to the position of the first element, thus avoiding boundary checks */                                                      \
    SCOPE void _InsertionSort_Small_##NAME(TYPE* Low, TYPE* High)                                                                  \
    {                                                                                                                              \
        if (Low == High) return;                                                                                                   \
        for (TYPE* i = Low + 1; i != High; ++i)                                                                                    \
        {                                                                                                                          \
            if (Compare_Function(i, Low) < 0)                                                                                      \
            {                                                                                                                      \
                TYPE Temp = *i;                                                                                                    \
                memmove(Low + 1, Low, (i - Low) * sizeof(TYPE));                                                                   \
                *Low = Temp;                                                                                                       \
            }                                                                                                                      \
            else _Unguarded_LinearInsert_##NAME(i);                                                                                \
        }                                                                                                                          \
    }                                                                                                                              \
    /* This insertion sort is a boundary-free insertion sort */                                                                    \
    SCOPE LTT_inline void _Unguarded_InsertionSort_##NAME(TYPE* Low, TYPE* High)                                                   \
    {                                                                                                                              \
        for (TYPE* i = Low; i != High; ++i) _Unguarded_LinearInsert_##NAME(i);                                                     \
    }                                                                                                                              \
    /* This insertion sort is used to sort the first S_Threshold elements of a long array, */                                      \
    /* which is already partially ordered in units of S_Threshold length */                                                        \
    /* After sorting the first S_Threshold elements, subsequent elements can benefit from */                                       \
    /* boundary-free insertion sort to improve performance */                                                                      \
    SCOPE void _Final_InsertionSort_##NAME(TYPE* Low, TYPE* High)                                                                  \
    {                                                                                                                              \
        if (High - Low > S_Threshold)                                                                                              \
        {                                                                                                                          \
            _InsertionSort_Small_##NAME(Low, Low + S_Threshold);                                                                   \
            _Unguarded_InsertionSort_##NAME(Low + S_Threshold, High);                                                              \
        }                                                                                                                          \
        else _InsertionSort_Small_##NAME(Low, High);                                                                               \
    }                                                                                                                              \
    SCOPE TYPE* GetPartition_libstdcpp_##NAME(TYPE* LeftP, TYPE* RightP, TYPE* Pivot)                                              \
    {                                                                                                                              \
        while (true)                                                                                                               \
        {                                                                                                                          \
            while (Compare_Function(LeftP, Pivot) < 0) ++LeftP;                                                                    \
            /* Subtract one here to prevent out-of-bounds access */                                                                \
            --RightP;                                                                                                              \
            while (Compare_Function(Pivot, RightP) < 0) --RightP;                                                                  \
            if (!(LeftP < RightP)) return LeftP;                                                                                   \
            SWAP(LeftP, RightP, TYPE);                                                                                             \
            ++LeftP;                                                                                                               \
        }                                                                                                                          \
    }                                                                                                                              \
    SCOPE LTT_inline TYPE* GetPartitionPivot_LTT_libstdcpp_##NAME(TYPE* Low, TYPE* High)                                           \
    {                                                                                                                              \
        TYPE* Mid = Low + ((High - Low) / 2);                                                                                      \
        MoveMidToFirst_##NAME(Low, Low + 1, Mid, High - 1);                                                                        \
        return GetPartition_libstdcpp_##NAME(Low + 1, High, Low);                                                                  \
    }                                                                                                                              \
    SCOPE void QuickSort_LTT_libstdcpp_Loop_##NAME(TYPE* Low, TYPE* High)                                                          \
    {                                                                                                                              \
        while (High - Low > S_Threshold)                                                                                           \
        {                                                                                                                          \
            TYPE* Cut = GetPartitionPivot_LTT_libstdcpp_##NAME(Low, High);                                                         \
            QuickSort_LTT_libstdcpp_Loop_##NAME(Cut, High);                                                                        \
            High = Cut;                                                                                                            \
        }                                                                                                                          \
    }                                                                                                                              \
    SCOPE void QuickSort_LTT_libstdcpp_##NAME(TYPE* Base, size_t NumOfElements)                                                    \
    {                                                                                                                              \
        if (NumOfElements <= 1) return;                                                                                            \
        QuickSort_LTT_libstdcpp_Loop_##NAME(Base, &Base[NumOfElements]);                                                           \
        _Final_InsertionSort_##NAME(Base, &Base[NumOfElements]);                                                                   \
    }                                                                                                                              \
    SCOPE void SimpleSelectionSort_##NAME(TYPE* Base, size_t NumOfElements)                                                        \
    {                                                                                                                              \
        if (NumOfElements <= 0) return;                                                                                            \
        for (size_t i = 0; i < NumOfElements; ++i)                                                                                 \
        {                                                                                                                          \
            TYPE* Min = &Base[i];                                                                                                  \
            for (size_t j = i + 1; j < NumOfElements; ++j)                                                                         \
            {                                                                                                                      \
                if (Compare_Function(Min, &Base[j]) > 0) Min = &Base[j];                                                           \
            }                                                                                                                      \
            if (&Base[i] != Min) SWAP(&Base[i], Min, TYPE);                                                                        \
        }                                                                                                                          \
    }                                                                                                                              \
    SCOPE void InsertionSort_##NAME(TYPE* Base, size_t NumOfElements) { _InsertionSort_Small_##NAME(Base, &Base[NumOfElements]); } \
    SCOPE void Merge_##NAME(TYPE* First, TYPE* Mid, TYPE* Last, TYPE* Temp)                                                        \
    {                                                                                                                              \
        if (Mid == Last || Compare_Function(Mid - 1, Mid) <= 0) return;                                                            \
        TYPE* i = First;                                                                                                           \
        TYPE* j = Mid;                                                                                                             \
        TYPE* k = Temp;                                                                                                            \
        while (i < Mid && j < Last)                                                                                                \
        {                                                                                                                          \
            if (Compare_Function(i, j) < 0) *k++ = *i++;                                                                           \
            else *k++ = *j++;                                                                                                      \
        }                                                                                                                          \
        if (i < Mid) memcpy(k, i, (Mid - i) * sizeof(TYPE));                                                                       \
        if (j < Last) memcpy(k, j, (Last - j) * sizeof(TYPE));                                                                     \
        memcpy(First, Temp, (Last - First) * sizeof(TYPE));                                                                        \
    }                                                                                                                              \
    SCOPE void MergeSort_Recursion_Loop_##NAME(TYPE* First, TYPE* Last, TYPE* Temp)                                                \
    {                                                                                                                              \
        size_t NumOfElements = Last - First;                                                                                       \
        if (NumOfElements == 1) return;                                                                                            \
        else if (NumOfElements > S_Threshold)                                                                                      \
        {                                                                                                                          \
            TYPE* Mid = First + (NumOfElements >> 1);                                                                              \
            MergeSort_Recursion_Loop_##NAME(First, Mid, Temp);                                                                     \
            MergeSort_Recursion_Loop_##NAME(Mid, Last, Temp);                                                                      \
            Merge_##NAME(First, Mid, Last, Temp);                                                                                  \
        }                                                                                                                          \
        else _InsertionSort_Small_##NAME(First, Last);                                                                             \
    }                                                                                                                              \
    SCOPE void MergeSort_Recursion_##NAME(TYPE* Base, size_t NumOfElements)                                                        \
    {                                                                                                                              \
        if (NumOfElements <= 0) return;                                                                                            \
        TYPE* Temp = (TYPE*)malloc(NumOfElements * sizeof(TYPE));                                                                  \
        MergeSort_Recursion_Loop_##NAME(Base, &Base[NumOfElements], Temp);                                                         \
        free(Temp);                                                                                                                \
    }                                                                                                                              \
    SCOPE void MergeSort_Iterative_##NAME(TYPE* Base, size_t NumOfElements)                                                        \
    {                                                                                                                              \
        TYPE*  Temp = (TYPE*)malloc(NumOfElements * sizeof(TYPE));                                                                 \
        size_t CurrentSize;                                                                                                        \
        TYPE*  First;                                                                                                              \
        for (CurrentSize = 1; CurrentSize < NumOfElements; CurrentSize <<= 1)                                                      \
        {                                                                                                                          \
            for (First = Base; First < &Base[NumOfElements]; First += (CurrentSize << 1))                                          \
            {                                                                                                                      \
                TYPE* Mid  = (&First[CurrentSize] < &Base[NumOfElements]) ? &First[CurrentSize] : &Base[NumOfElements];            \
                TYPE* Last = (&Mid[CurrentSize] < &Base[NumOfElements]) ? &Mid[CurrentSize] : &Base[NumOfElements];                \
                Merge_##NAME(First, Mid, Last, Temp);                                                                              \
            }                                                                                                                      \
        }                                                                                                                          \
        free(Temp);                                                                                                                \
    }                                                                                                                              \
    SCOPE void Merge_Inplace_##NAME(TYPE* First, TYPE* Mid, TYPE* Last)                                                            \
    {                                                                                                                              \
        if (Mid == Last || Compare_Function(Mid - 1, Mid) <= 0) return;                                                            \
        TYPE Temp;                                                                                                                 \
        while (First < Mid && Mid < Last)                                                                                          \
        {                                                                                                                          \
            if (Compare_Function(First, Mid) <= 0) ++First;                                                                        \
            else                                                                                                                   \
            {                                                                                                                      \
                Temp = *Mid;                                                                                                       \
                memmove(First + 1, First, (Mid - First) * sizeof(TYPE));                                                           \
                *First = Temp;                                                                                                     \
                ++First;                                                                                                           \
                ++Mid;                                                                                                             \
            }                                                                                                                      \
        }                                                                                                                          \
    }                                                                                                                              \
    SCOPE void MergeSort_Inplace_Iterative_##NAME(TYPE* Base, size_t NumOfElements)                                                \
    {                                                                                                                              \
        size_t CurrentSize;                                                                                                        \
        TYPE*  First;                                                                                                              \
        for (CurrentSize = 1; CurrentSize < NumOfElements; CurrentSize <<= 1)                                                      \
        {                                                                                                                          \
            for (First = Base; First < &Base[NumOfElements]; First += (CurrentSize << 1))                                          \
            {                                                                                                                      \
                TYPE* Mid  = (&First[CurrentSize] < &Base[NumOfElements]) ? &First[CurrentSize] : &Base[NumOfElements];            \
                TYPE* Last = (&Mid[CurrentSize] < &Base[NumOfElements]) ? &Mid[CurrentSize] : &Base[NumOfElements];                \
                Merge_Inplace_##NAME(First, Mid, Last);                                                                            \
            }                                                                                                                      \
        }                                                                                                                          \
    }

//SCOPE TYPE* GetPartition_LTT_glibc_##NAME(TYPE* LeftP, TYPE* RightP, TYPE* Pivot)
//{
//    do {
//        while (Compare_Function(LeftP, Pivot) < 0) ++LeftP;
//        while (Compare_Function(Pivot, RightP) < 0) --RightP;
//        if (LeftP < RightP)
//        {
//            SWAP(LeftP, RightP, TYPE);
//            if (Pivot == LeftP) Pivot = RightP;
//            else if (Pivot == RightP) Pivot = LeftP;
//            ++LeftP;
//            --RightP;
//        }
//        else if (LeftP == RightP) return LeftP;
//    }
//    while (LeftP <= RightP);
//    return RightP;
//}
//SCOPE TYPE* GetPartitionPivot_LTT_glibc_##NAME(TYPE* Low, TYPE* High)
//{
//    TYPE* Mid = Low + ((High - Low) >> 1);
//    if (Compare_Function(Mid, Low) < 0) SWAP(Mid, Low, TYPE);
//    if (Compare_Function(High, Mid) < 0) SWAP(Mid, High, TYPE);
//    else goto A;
//    if (Compare_Function(Mid, Low) < 0) SWAP(Mid, Low, TYPE);
//A:
//    return GetPartition_LTT_glibc_##NAME(Low + 1, High - 1, Mid);
//}
//SCOPE void QuickSort_LTT_glibc_Loop_##NAME(TYPE* Low, TYPE* High)
//{
//    while (High - Low > S_Threshold)
//    {
//        TYPE* Cut = GetPartitionPivot_LTT_glibc_##NAME(Low, High - 1);
//        QuickSort_LTT_glibc_Loop_##NAME(Cut, High);
//        High = Cut;
//    }
//}
//SCOPE void QuickSort_LTT_glibc_##NAME(TYPE* Base, size_t NumOfElements)
//{
//    if (NumOfElements <= 1) return;
//    QuickSort_LTT_glibc_Loop_##NAME(Base, &Base[NumOfElements]);
//    _Final_InsertionSort_##NAME(Base, &Base[NumOfElements]);
//}

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

// Global variables
static int HibbardStepArray[31]   = {2147483647, 1073741823, 536870911, 268435455, 134217727, 67108863, 33554431, 16777215, 8388607, 4194303, 2097151, 1048575, 524287, 262143, 131071, 65535, 32767, 16383, 8191, 4095, 2047, 1023, 511, 255, 127, 63, 31, 15, 7, 3, 1};
static int SedgewickStepArray[14] = {603906049, 150958081, 37730305, 9427969, 2354689, 587521, 146305, 36289, 8929, 2161, 505, 109, 19, 1};

// some static functions
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

static LTT_unused void Merge_Inplace_Int(int* First, int* Mid, int* Last)
{
    int Max = *First;
    for (int* i = First; i < Last; ++i)
    {
        if (*i > Max) Max = *i;
    }
    ++Max;
    int i = 0;
    int j = Mid - First;
    int k = 0;
    while (First + i < Mid && First + j < Last)
    {
        First[k] += (First[i] % Max <= First[j] % Max) ? (First[i++] % Max * Max) : (First[j++] % Max * Max);
        ++k;
    }
    while (First + i < Mid)
    {
        First[k] += First[i] % Max * Max;
        ++k;
        ++i;
    }
    while (First + j < Last)
    {
        First[k] += First[j] % Max * Max;
        ++k;
        ++j;
    }
    for (int* i = First; i < Last; ++i) *i /= Max;
}

static LTT_unused void MergeSort_Inplace_Iterative_Int(int* Base, size_t NumOfElements)
{
    size_t CurrentSize;
    int*   First;
    for (CurrentSize = 1; CurrentSize < NumOfElements; CurrentSize <<= 1)
    {
        for (First = Base; First < &Base[NumOfElements]; First += (CurrentSize << 1))
        {
            int* Mid  = (&First[CurrentSize] < &Base[NumOfElements]) ? &First[CurrentSize] : &Base[NumOfElements];
            int* Last = (&Mid[CurrentSize] < &Base[NumOfElements]) ? &Mid[CurrentSize] : &Base[NumOfElements];
            Merge_Inplace_Int(First, Mid, Last);
        }
    }
}

/*
	注意与计数排序的区别, 此排序只考虑每一个元素出现的次数,并且只能排序int数组
	本来PigeonholeSort是直接将int值直接映射到数值的索引上,并在上面记录个数
	然后再历遍一次数组将数全部放到原数组里面就可以了
	但是为了全部开空间INT_MAX也开不了(16GB),于是有了下面的改进算法
	与计数排序的区别就是他不实际记录每一个元素的位置,而是个数,最后依次输出即可
*/
static LTT_unused void PigeonholeSort_Int(int* Base, int Min, int Max, size_t NumOfElements)
{
    if (NumOfElements <= 1) return;
    int Delta       = Max - Min;
    // 创建统计数组并统计对应元素的个数
    int* CountArray = (int*)calloc((Delta + 1), sizeof(int));
    if (CountArray == NULL) return;
    for (size_t i = 0; i < NumOfElements; ++i) ++CountArray[Base[i] - Min];
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

#define SORT_INIT(NAME, TYPE, SCOPE, Compare_Function) \
    SORT_TYPE(NAME, TYPE)                              \
    SORT_IMPL(NAME, TYPE, SCOPE, Compare_Function)


//################################################### Based on Insertion ##################################################

/**
 * @brief Insertion Sort algorithm
 * @param NAME The name of the function
 * @param Array The array to be sorted
 * @param Number The number of elements in the array
 * @note Time complexity: O(n^2)
 */
#define InsertionSort(NAME, Array, Number)                InsertionSort_##NAME((Array), (Number))

/**
 * @brief Binary Insertion Sort algorithm
 * @param NAME The name of the function
 * @param Array The array to be sorted
 * @param Number The number of elements in the array
 * @note Time complexity: O(n^2)
 */
#define BinaryInsertionSort(NAME, Array, Number)          BinaryInsertionSort_##NAME((Array), (Number))

/**
 * @brief Shell Sort algorithm based on Hibbard's increment sequence
 * @param NAME The name of the function
 * @param Array The array to be sorted
 * @param Number The number of elements in the array
 * @note Time complexity: O(n^(3/2))
 */
#define ShellInsertionSort_Hibbard(NAME, Array, Number)   ShellInsertionSort_Hibbard_##NAME((Array), (Number))

/**
 * @brief Shell Sort algorithm based on Sedgewick's increment sequence
 * @param NAME The name of the function
 * @param Array The array to be sorted
 * @param Number The number of elements in the array
 * @note Time complexity: O(n^(4/3))
 */
#define ShellInsertionSort_Sedgewick(NAME, Array, Number) ShellInsertionSort_Sedgewick_##NAME((Array), (Number))

//################################################### Based on Exchange ###################################################

/**
 * @brief Bubble Sort algorithm
 * @param NAME The name of the function
 * @param Array The array to be sorted
 * @param Number The number of elements in the array
 * @note Time complexity: O(n^2)
 */
#define BubbleSort(NAME, Array, Number)                   BubbleSort_##NAME((Array), (Number))

/**
 * @brief Bubble Sort algorithm with fast version
 * @param NAME The name of the function
 * @param Array The array to be sorted
 * @param Number The number of elements in the array
 * @note Time complexity: O(n^2)
 */
#define BubbleSort_Fast(NAME, Array, Number)              BubbleSort_Fast_##NAME((Array), (Number))

/**
 * @brief Quick Sort algorithm \n
 * This Quick_Sort algorithm is taken directly from the original _qsort function in glibc. \n
 * After genericization, performance is doubled.
 * use the Hoare partition scheme partitioning
 * @param NAME The name of the function
 * @param Array The array to be sorted
 * @param Number The number of elements in the array
 * @note Time complexity: O(nlogn)
 */
#define QuickSort_glibc(NAME, Array, Number)              QuickSort_glibc_##NAME((Array), (Number))

/**
 * @brief Quick Sort algorithm \n
 * This Quick_Sort algorithm is taken directly from the sort part of GCC's libstdcpp. \n
 * Generally faster than QuickSort_glibc. \n
 * use the Hoare partition scheme partitioning \n
 * Essentially the same partitioning as in QuickSort_glibc
 * @param NAME The name of the function
 * @param Array The array to be sorted
 * @param Number The number of elements in the array
 * @note Time complexity: O(nlogn)
 */
#define QuickSort_LTT_libstdcpp(NAME, Array, Number)      QuickSort_LTT_libstdcpp_##NAME((Array), (Number))

//################################################### Based on Selection ###################################################

/**
 * @brief Simple Selection Sort algorithm
 * @param NAME The name of the function
 * @param Array The array to be sorted
 * @param Number The number of elements in the array
 * @note Time complexity: O(n^2)
 */
#define SimpleSelectionSort(NAME, Array, Number)          SimpleSelectionSort_##NAME((Array), (Number))
// TODO: Heap Sort
#define HeapSort(NAME, Array, Number)                     HeapSort_##NAME((Array), (Number))
#define PartialSort(NAME, Array, Number)                  PartialSort_##NAME((Array), (Number))
//################################################### Based on Merge ###################################################

/**
 * @brief Merge Sort algorithm based on recursion
 * @param NAME The name of the function
 * @param Array The array to be sorted
 * @param Number The number of elements in the array
 * @note Time complexity: O(nlogn)
 */
#define MergeSort_Recursion(NAME, Array, Number)          MergeSort_Recursion_##NAME((Array), (Number))

/**
 * @brief Merge Sort algorithm based on iteration
 * @param NAME The name of the function
 * @param Array The array to be sorted
 * @param Number The number of elements in the array
 * @note Time complexity: O(nlogn)
 */
#define MergeSort_Iterative(NAME, Array, Number)          MergeSort_Iterative_##NAME((Array), (Number))

/**
 * @brief In-place Merge Sort algorithm based on iteration \n
 * Space complexity reduced to O(1), but time complexity is O(n^2), not recommended!
 * @param NAME The name of the function
 * @param Array The array to be sorted
 * @param Number The number of elements in the array
 * @note Time complexity: O(n^2)
 */
#define MergeSort_Inplace_Iterative(NAME, Array, Number)  MergeSort_Inplace_Iterative_##NAME((Array), (Number))

/**
 * @brief In-place Merge Sort algorithm based on iteration only for int array \n
 * A trick technique suitable for int arrays.
 * Max is the value of the maximum element + 1.
 * The principle is to compare the remainders of two numbers a and b with Max,
 * select the smaller one, then set a = a + (min(a % Max, b % max)) * Max.
 * Thus, the quotient of the original position of the number by Max is the sorted value,
 * and the remainder is the original value.
 * Be particularly careful about overflow when computing a = a + (min(a % Max, b % max)) * Max!
 * @param Array The array to be sorted
 * @param Number The number of elements in the array
 * @note Time complexity: O(n^2)
 */
#define MergeSort_Inplace_Iterative_For_Int(Array, Number) MergeSort_Inplace_Iterative_Int((Array), (Number))

// Todo: k-way Merge Sort

// Todo: RadixSort_LSD Sort
/**
 * @brief Radix Sort algorithm based on LSD (Least Significant Digit)
 * @param NAME The name of the function
 * @param Array The array to be sorted
 * @param Number The number of elements in the array
 * @note Time complexity: O(n*k/d) \n
 * k: Number of digits in the largest number \n
 * d: Number of digits processed per pass.
 */
#define RadixSort_LSD(NAME, Array, Number)          RadixSort_LSD_##NAME((Array), (Number))

// Todo: RadixSort_MSD Sort
/**
 * @brief Radix Sort algorithm based on MSD (Most Significant Digit)
 * @param NAME The name of the function
 * @param Array The array to be sorted
 * @param Number The number of elements in the array
 * @note Time complexity: O(n*k/d) \n
 * k: Number of digits in the largest number \n
 * d: Number of digits processed per pass.
 */
#define RadixSort_MSD(NAME, Array, Number)          RadixSort_MSD_##NAME((Array), (Number))

// Todo: Bucket Sort
/**
 * @brief Bucket Sort algorithm
 * @param NAME The name of the function
 * @param Array The array to be sorted
 * @param Number The number of elements in the array
 * @note Time complexity: O(n+n^2/k+k) \n
 * k: Number of buckets \n
 * Note that if k is chosen to be k=O(n), then the time complexity becomes O(n)
 */
#define BucketSort(NAME, Array, Number)             BucketSort_##NAME((Array), (Number))

// Todo: Counting Sort
/**
 * @brief Counting Sort algorithm
 * @param NAME The name of the function
 * @param Array The array to be sorted
 * @param Number The number of elements in the array
 * @note Time complexity: O(n+k) \n
 * k:the range of the non-negative key values
 */
#define CountingSort(NAME, Array, Number)           CountingSort_##NAME((Array), (Number))

/**
 * @brief Pigeonhole Sort algorithm
 * @param Array The array to be sorted
 * @param Min The minimum value of the array
 * @param Max The maximum value of the array
 * @param Number The number of elements in the array
 * @note Time complexity: O(n)
 */
#define PigeonholeSort(Array, Min, Max, Number)     PigeonholeSort_Int((Array), (Min), (Max), (Number))

// TODO: Bigo Sort
/**
 * @brief Bigo Sort algorithm
 * @param NAME The name of the function
 * @param Array The array to be sorted
 * @param Number The number of elements in the array
 * @note Time complexity: O(n*n!)
 */
#define BigoSort(NAME, Array, Number)               BigoSort_##NAME((Array), (Number))

/**
 * @brief Introspective Sort algorithm
 * @param NAME The name of the function
 * @param Array The array to be sorted
 * @param Number The number of elements in the array
 * @note Time complexity: O(nlogn)
 */
#define IntrospectiveSort(NAME, Array, Number)      IntrospectiveSort_##NAME((Array), (Number))

// Function implementation
#define LTT_SORT_INIT(NAME, TYPE, Compare_Function) SORT_INIT(NAME, TYPE, static LTT_unused, Compare_Function)

// Function declaration
#define LTT_SORT_DECLARE(NAME, TYPE)                SORT_DECLARE(NAME, TYPE)
