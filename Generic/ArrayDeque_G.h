#pragma once
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "Generic_tool.h"


// ------------------------------------------------------------------------------------------------
// Benchmark                                                      Time             CPU   Iterations
// ------------------------------------------------------------------------------------------------
// ArrayDeque_G_AddFirst_Test/100000                          50286 ns        50288 ns        12678
// deque_AddFirst_Test/100000                                 45831 ns        45833 ns        14588
// ArrayDeque_G_AddLast_Test/100000                           77435 ns        77408 ns         9004
// deque_AddLast_Test/100000                                  55905 ns        55846 ns        13702
// ArrayDeque_G_DeleteFirst_Test/100000/iterations:21474        214 ns          189 ns        21474
// deque_DeleteFirst_Test/100000                              38704 ns        38707 ns        17942
// ArrayDeque_G_DeleteLast_Test/100000/iterations:21474         245 ns          224 ns        21474
// deque_DeleteLast_Test/100000                               41931 ns        41931 ns        17466

#define DEFAULT_ARRAYDEQUE_CAPACITY  (16)
#define SOFT_MAX_ARRAYDEQUE_CAPACITY (INT_MAX - 8)
// #define INC(Head, Capacity)          (((Head) + 1) % (Capacity))                      // 同余类+1
// #define DEC(Head, Capacity)          (((Head) + ((Capacity) - 1)) % (Capacity))       // 同余类-1
// #define SUB(Head, Tail, Capacity)    (((Tail) - (Head) + (Capacity)) % (Capacity))    // 同余类减法


#define ARRAYDEQUE_TYPE(NAME, TYPE)       \
    typedef struct ArrayDeque_##NAME      \
    {                                     \
        /*Array of the elements*/         \
        TYPE* Array;                      \
        /*The capacity of the Array*/     \
        int Capacity;                     \
        /*The index of the head element*/ \
        int Head;                         \
        /*The index of the tail element*/ \
        int Tail;                         \
    } ArrayDeque_##NAME;


#define ARRAYDEQUE_PROTOTYPES(NAME, TYPE)                                                                    \
    extern CODE ArrayDeque_##NAME##_Init(ArrayDeque_##NAME* ArrayDeque);                                     \
    extern CODE ArrayDeque_##NAME##_AddFirst(ArrayDeque_##NAME* const ArrayDeque, const TYPE Data);          \
    extern CODE ArrayDeque_##NAME##_AddLast(ArrayDeque_##NAME* const ArrayDeque, const TYPE Data);           \
    extern CODE ArrayDeque_##NAME##_DeleteFirst(ArrayDeque_##NAME* const ArrayDeque, TYPE* const Result);    \
    extern CODE ArrayDeque_##NAME##_DeleteLast(ArrayDeque_##NAME* const ArrayDeque, TYPE* const Result);     \
    extern CODE ArrayDeque_##NAME##_GetFirst(const ArrayDeque_##NAME* const ArrayDeque, TYPE* const Result); \
    extern CODE ArrayDeque_##NAME##_GetLast(const ArrayDeque_##NAME* const ArrayDeque, TYPE* const Result);  \
    extern int  ArrayDeque_##NAME##_GetSize(const ArrayDeque_##NAME* const ArrayDeque);                      \
    extern bool ArrayDeque_##NAME##_IsEmpty(const ArrayDeque_##NAME* const ArrayDeque);                      \
    extern bool ArrayDeque_##NAME##_Contains(const ArrayDeque_##NAME* const ArrayDeque, const TYPE Data);    \
    extern void ArrayDeque_##NAME##_Clear(ArrayDeque_##NAME* const ArrayDeque);                              \
    extern void ArrayDeque_##NAME##_Destroy(ArrayDeque_##NAME* ArrayDeque);


#define ARRAYDEQUE_IMPL(NAME, TYPE, SCOPE, Equals_Function)                                                                                                     \
    SCOPE CODE ArrayDeque_##NAME##_Init(ArrayDeque_##NAME* ArrayDeque)                                                                                          \
    {                                                                                                                                                           \
        ArrayDeque->Array = (TYPE*)malloc((DEFAULT_ARRAYDEQUE_CAPACITY + 1) * sizeof(TYPE));                                                                    \
        if (unlikely(ArrayDeque->Array == NULL)) return MemoryAllocationError;                                                                                  \
        ArrayDeque->Capacity = DEFAULT_ARRAYDEQUE_CAPACITY + 1;                                                                                                 \
        ArrayDeque->Head = ArrayDeque->Tail = 0;                                                                                                                \
        return Success;                                                                                                                                         \
    }                                                                                                                                                           \
    SCOPE CODE ArrayDeque_##NAME##_newCapacity(const int OldCapacity, const int Needed, const int Jump, int* const Result)                                      \
    {                                                                                                                                                           \
        int MinCapacity;                                                                                                                                        \
        if ((MinCapacity = OldCapacity + Needed) - SOFT_MAX_ARRAYDEQUE_CAPACITY > 0)                                                                            \
        {                                                                                                                                                       \
            if (unlikely(MinCapacity < 0)) return MemoryOverflow; /*Overflow*/                                                                                  \
            else                                                                                                                                                \
            {                                                                                                                                                   \
                *Result = INT_MAX;                                                                                                                              \
                return Success;                                                                                                                                 \
            }                                                                                                                                                   \
        }                                                                                                                                                       \
        if (Needed > Jump)                                                                                                                                      \
        {                                                                                                                                                       \
            *Result = MinCapacity;                                                                                                                              \
            return Success;                                                                                                                                     \
        }                                                                                                                                                       \
        *Result = (OldCapacity + Jump - SOFT_MAX_ARRAYDEQUE_CAPACITY < 0) ? OldCapacity + Jump : SOFT_MAX_ARRAYDEQUE_CAPACITY;                                  \
        return Success;                                                                                                                                         \
    }                                                                                                                                                           \
    SCOPE CODE ArrayDeque_##NAME##_Resize(ArrayDeque_##NAME* const ArrayDeque, const int Needed)                                                                \
    {                                                                                                                                                           \
        int OldCapacity = ArrayDeque->Capacity;                                                                                                                 \
        int NewCapacity;                                                                                                                                        \
        int Jump = (OldCapacity < 64) ? (OldCapacity + 2) : (OldCapacity >> 1); /*Jump is the increment*/                                                       \
        if (Jump < Needed || (NewCapacity = OldCapacity + Jump) - SOFT_MAX_ARRAYDEQUE_CAPACITY > 0)                                                             \
        {                                                                                                                                                       \
            if (Success != ArrayDeque_##NAME##_newCapacity(OldCapacity, Needed, Jump, &NewCapacity)) return MemoryOverflow;                                     \
        }                                                                                                                                                       \
        TYPE* EA = (TYPE*)realloc(ArrayDeque->Array, NewCapacity * sizeof(TYPE));                                                                               \
        if (unlikely(EA == NULL)) return MemoryAllocationError;                                                                                                 \
        ArrayDeque->Array    = EA;                                                                                                                              \
        ArrayDeque->Capacity = NewCapacity;                                                                                                                     \
        int Head             = ArrayDeque->Head;                                                                                                                \
        int Tail             = ArrayDeque->Tail;                                                                                                                \
        /*if Tail<=Head, move the elements to the new space*/                                                                                                   \
        if (Tail <= Head)                                                                                                                                       \
        {                                                                                                                                                       \
            int NewSpace = NewCapacity - OldCapacity;                                                                                                           \
            memmove(&EA[Head + NewSpace], &EA[Head], (OldCapacity - Head) * sizeof(TYPE));                                                                      \
            ArrayDeque->Head += NewSpace;                                                                                                                       \
        }                                                                                                                                                       \
        return Success;                                                                                                                                         \
    }                                                                                                                                                           \
    SCOPE CODE ArrayDeque_##NAME##_AddFirst(ArrayDeque_##NAME* const ArrayDeque, const TYPE Data)                                                               \
    {                                                                                                                                                           \
        ArrayDeque->Array[ArrayDeque->Head = DEC(ArrayDeque->Head, ArrayDeque->Capacity)] = Data;                                                               \
        if (unlikely(ArrayDeque->Head == ArrayDeque->Tail))                                                                                                     \
        {                                                                                                                                                       \
            if (Success != ArrayDeque_##NAME##_Resize(ArrayDeque, 1)) return MemoryOverflow;                                                                    \
        }                                                                                                                                                       \
        return Success;                                                                                                                                         \
    }                                                                                                                                                           \
    SCOPE CODE ArrayDeque_##NAME##_AddLast(ArrayDeque_##NAME* const ArrayDeque, const TYPE Data)                                                                \
    {                                                                                                                                                           \
        ArrayDeque->Array[ArrayDeque->Tail] = Data;                                                                                                             \
        ArrayDeque->Tail                    = INC(ArrayDeque->Tail, ArrayDeque->Capacity);                                                                      \
        if (unlikely(ArrayDeque->Head == ArrayDeque->Tail))                                                                                                     \
        {                                                                                                                                                       \
            if (Success != ArrayDeque_##NAME##_Resize(ArrayDeque, 1)) return MemoryOverflow;                                                                    \
        }                                                                                                                                                       \
        return Success;                                                                                                                                         \
    }                                                                                                                                                           \
    SCOPE CODE ArrayDeque_##NAME##_DeleteFirst(ArrayDeque_##NAME* const ArrayDeque, TYPE* const Result)                                                         \
    {                                                                                                                                                           \
        if (unlikely(ArrayDeque->Head == ArrayDeque->Tail)) return NullPointerAccess;                                                                           \
        *Result          = ArrayDeque->Array[ArrayDeque->Head];                                                                                                 \
        ArrayDeque->Head = INC(ArrayDeque->Head, ArrayDeque->Capacity);                                                                                         \
        return Success;                                                                                                                                         \
    }                                                                                                                                                           \
    SCOPE CODE ArrayDeque_##NAME##_DeleteLast(ArrayDeque_##NAME* const ArrayDeque, TYPE* const Result)                                                          \
    {                                                                                                                                                           \
        if (unlikely(ArrayDeque->Head == ArrayDeque->Tail)) return NullPointerAccess;                                                                           \
        *Result = ArrayDeque->Array[ArrayDeque->Tail = DEC(ArrayDeque->Tail, ArrayDeque->Capacity)];                                                            \
        return Success;                                                                                                                                         \
    }                                                                                                                                                           \
    SCOPE CODE ArrayDeque_##NAME##_GetFirst(const ArrayDeque_##NAME* const ArrayDeque, TYPE* const Result)                                                      \
    {                                                                                                                                                           \
        if (unlikely(ArrayDeque->Head == ArrayDeque->Tail)) return NullPointerAccess;                                                                           \
        *Result = ArrayDeque->Array[ArrayDeque->Head];                                                                                                          \
        return Success;                                                                                                                                         \
    }                                                                                                                                                           \
    SCOPE CODE ArrayDeque_##NAME##_GetLast(const ArrayDeque_##NAME* const ArrayDeque, TYPE* const Result)                                                       \
    {                                                                                                                                                           \
        if (unlikely(ArrayDeque->Head == ArrayDeque->Tail)) return NullPointerAccess;                                                                           \
        *Result = ArrayDeque->Array[DEC(ArrayDeque->Tail, ArrayDeque->Capacity)];                                                                               \
        return Success;                                                                                                                                         \
    }                                                                                                                                                           \
    SCOPE int  ArrayDeque_##NAME##_GetSize(const ArrayDeque_##NAME* const ArrayDeque) { return SUB(ArrayDeque->Head, ArrayDeque->Tail, ArrayDeque->Capacity); } \
    SCOPE bool ArrayDeque_##NAME##_Contains(const ArrayDeque_##NAME* const ArrayDeque, const TYPE Data)                                                         \
    {                                                                                                                                                           \
        TYPE* EA = ArrayDeque->Array;                                                                                                                           \
        for (int i = ArrayDeque->Head, end = ArrayDeque->Tail, to = (i <= end) ? end : ArrayDeque->Capacity;; i = 0, to = end)                                  \
        {                                                                                                                                                       \
            for (; i < to; i++)                                                                                                                                 \
            {                                                                                                                                                   \
                if (Equals_Function(Data, EA[i])) return true;                                                                                                  \
            }                                                                                                                                                   \
            if (to == end) break;                                                                                                                               \
        }                                                                                                                                                       \
        return false;                                                                                                                                           \
    }                                                                                                                                                           \
    SCOPE void ArrayDeque_##NAME##_Clear(ArrayDeque_##NAME* const ArrayDeque) { ArrayDeque->Head = ArrayDeque->Tail = 0; }                                      \
    SCOPE void ArrayDeque_##NAME##_Destroy(ArrayDeque_##NAME* const ArrayDeque)                                                                                 \
    {                                                                                                                                                           \
        free(ArrayDeque->Array);                                                                                                                                \
        ArrayDeque->Array    = NULL;                                                                                                                            \
        ArrayDeque->Capacity = 0;                                                                                                                               \
        ArrayDeque->Head = ArrayDeque->Tail = 0;                                                                                                                \
    }


#define ARRAYDEQUE_DECLARE(NAME, TYPE) \
    ARRAYDEQUE_TYPE(NAME, TYPE)        \
    ARRAYDEQUE_PROTOTYPES(NAME, TYPE)


#define ARRAYDEQUE_INIT(NAME, TYPE, SCOPE, Equals_Function) \
    ARRAYDEQUE_TYPE(NAME, TYPE)                             \
    ARRAYDEQUE_IMPL(NAME, TYPE, SCOPE, Equals_Function)

// 内联函数
// 同余类+1
static LTT_inline LTT_unused int INC(int Head, int Capacity) { return (++Head >= Capacity) ? 0 : Head; }

// 同余类-1
static LTT_inline LTT_unused int DEC(int Head, int Capacity) { return (--Head < 0) ? Capacity - 1 : Head; }

// 同余类减法
static LTT_inline LTT_unused int SUB(int Head, int Tail, int Capacity) { return ((Tail -= Head) < 0) ? Tail + Capacity : Tail; }

/**
 * @brief Get the ArrayDeque_##NAME struct
 * @param NAME
 */
#define ArrayDeque(NAME)                                 ArrayDeque_##NAME

/**
 * @brief Initialize the ArrayDeque_##NAME struct
 * @param NAME The name of the function
 * @param ArrayDeque The ArrayDeque_##NAME struct
 * @return CODE The result of the function
 */
#define ArrayDeque_Init(NAME, ArrayDeque)                ArrayDeque_##NAME##_Init((ArrayDeque))

/**
 * @brief Add the Data to the first of the ArrayDeque_##NAME
 * @param NAME The name of the function
 * @param ArrayDeque The ArrayDeque_##NAME struct
 * @param Data The Data to add
 * @return CODE The result of the function
 * @note The function can be optimized compared to the deque
 */
#define ArrayDeque_AddFirst(NAME, ArrayDeque, Data)      ArrayDeque_##NAME##_AddFirst((ArrayDeque), (Data))

/**
 * @brief Add the Data to the last of the ArrayDeque_##NAME
 * @param NAME The name of the function
 * @param ArrayDeque The ArrayDeque_##NAME struct
 * @param Data The Data to add
 * @return CODE The result of the function
 */
#define ArrayDeque_AddLast(NAME, ArrayDeque, Data)       ArrayDeque_##NAME##_AddLast((ArrayDeque), (Data))

/**
 * @brief Delete the first element of the ArrayDeque_##NAME
 * @param NAME The name of the function
 * @param ArrayDeque The ArrayDeque_##NAME struct
 * @param Result To store the deleted element
 * @return CODE The result of the function
 * @note The function can be optimized compared to the deque
 */
#define ArrayDeque_DeleteFirst(NAME, ArrayDeque, Result) ArrayDeque_##NAME##_DeleteFirst((ArrayDeque), (Result))

/**
 * @brief Delete the last element of the ArrayDeque_##NAME
 * @param NAME The name of the function
 * @param ArrayDeque The ArrayDeque_##NAME struct
 * @param Result To store the deleted element
 * @return CODE The result of the function
 */
#define ArrayDeque_DeleteLast(NAME, ArrayDeque, Result)  ArrayDeque_##NAME##_DeleteLast((ArrayDeque), (Result))

/**
 * @brief Get the first element of the ArrayDeque_##NAME
 * @param NAME The name of the function
 * @param ArrayDeque The ArrayDeque_##NAME struct
 * @param Result To store the first element
 * @return CODE The result of the function
 */
#define ArrayDeque_GetFirst(NAME, ArrayDeque, Result)    ArrayDeque_##NAME##_GetFirst((ArrayDeque), (Result))

/**
 * @brief Get the last element of the ArrayDeque_##NAME
 * @param NAME The name of the function
 * @param ArrayDeque The ArrayDeque_##NAME struct
 * @param Result To store the last element
 * @return CODE The result of the function
 */
#define ArrayDeque_GetLast(NAME, ArrayDeque, Result)     ArrayDeque_##NAME##_GetLast((ArrayDeque), (Result))

/**
 * @brief Get the size of the ArrayDeque_##NAME
 * @param NAME The name of the function
 * @param ArrayDeque The ArrayDeque_##NAME struct
 * @return int The size of the ArrayDeque_##NAME
 */
#define ArrayDeque_GetSize(ArrayDeque)                   (SUB((ArrayDeque)->Head, (ArrayDeque)->Tail, (ArrayDeque)->Capacity))

/**
 * @brief Check the ArrayDeque_##NAME is empty
 * @param NAME The name of the function
 * @param ArrayDeque The ArrayDeque_##NAME struct
 * @retval true The ArrayDeque_##NAME is empty
 * @retval false The ArrayDeque_##NAME is not empty
 */
#define ArrayDeque_IsEmpty(ArrayDeque)                   ((ArrayDeque)->Head == (ArrayDeque)->Tail)

/**
 * @brief Check the ArrayDeque_##NAME contains the Data
 * @param NAME The name of the function
 * @param ArrayDeque The ArrayDeque_##NAME struct
 * @param Data The Data to check
 * @retval true The ArrayDeque_##NAME contains the Data
 * @retval false The ArrayDeque_##NAME does not contain the Data
 */
#define ArrayDeque_Contains(NAME, ArrayDeque, Data)      ArrayDeque_##NAME##_Contains((ArrayDeque), (Data))

/**
 * @brief Clear the ArrayDeque_##NAME, it will not free the memory
 * @param NAME The name of the function
 * @param ArrayDeque The ArrayDeque_##NAME struct
 */
#define ArrayDeque_Clear(ArrayDeque)                     ((ArrayDeque)->Head = (ArrayDeque)->Tail = 0)

/**
 * @brief Destroy the ArrayDeque_##NAME, it will free the memory
 * @param NAME The name of the function
 * @param ArrayDeque The ArrayDeque_##NAME struct
 */
#define ArrayDeque_Destroy(NAME, ArrayDeque)             ArrayDeque_##NAME##_Destroy((ArrayDeque))

// Function implementation
#define LTT_ARRAYDEQUE_INIT(NAME, TYPE, Equals_Function) ARRAYDEQUE_INIT(NAME, TYPE, static LTT_inline LTT_unused, Equals_Function)

// Function declaration
#define LTT_ARRAYSTACK_DECLARE(NAME, TYPE)               ARRAYDEQUE_DECLARE(NAME, TYPE)
