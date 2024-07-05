#pragma once
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "Generic_tool.h"


// ----------------------------------------------------------------------------------------------
// Benchmark                                                    Time             CPU   Iterations
// ----------------------------------------------------------------------------------------------
// ArrayList_G_AddFirst_Test/100000                     202540793 ns    202454596 ns            3
// vector_AddFirst_Test/100000                          203211509 ns    203190876 ns            3
// ArrayList_G_AddLast_Test/100000                          48800 ns        48796 ns        14471
// vector_AddLast_Test/100000                               23979 ns        23977 ns        29279
// ArrayList_G_DeleteFirst_Test/100000                  199706999 ns    199646427 ns            3
// vector_DeleteFirst_Test/100000                       209752662 ns    209680216 ns            3
// ArrayList_G_DeleteLast_Test/100000/iterations:21474        176 ns          158 ns        21474
// vector_DeleteLast_Test/100000/iterations:21474             156 ns          140 ns        21474

#define DEFAULT_ARRAYLIST_CAPACITY  (16)
#define SOFT_MAX_ARRAYLIST_CAPACITY (INT_MAX - 8)


#define ARRAYLIST_TYPE(NAME, TYPE)                    \
    typedef struct ArrayList_##NAME                   \
    {                                                 \
        /* Array of the elements */                   \
        TYPE* Array;                                  \
        /* The capacity of the Array */               \
        int Capacity;                                 \
        /* The number of the elements in the array */ \
        int Size;                                     \
    } ArrayList_##NAME;


#define ARRAYLIST_PROTOTYPES(NAME, TYPE)                                                                                        \
    extern void ArrayList_##NAME##_Init(ArrayList_##NAME* const ArrayList);                                                     \
    extern CODE ArrayList_##NAME##_AddFirst(ArrayList_##NAME* const ArrayList, const TYPE Data);                                \
    extern CODE ArrayList_##NAME##_AddLast(ArrayList_##NAME* const ArrayList, const TYPE Data);                                 \
    extern CODE ArrayList_##NAME##_AddIndex_Safe(ArrayList_##NAME* const ArrayList, const TYPE Data, const int Index);          \
    extern CODE ArrayList_##NAME##_DeleteFirst(ArrayList_##NAME* const ArrayList, TYPE* const Result);                          \
    extern CODE ArrayList_##NAME##_DeleteLast(ArrayList_##NAME* const ArrayList, TYPE* const Result);                           \
    extern CODE ArrayList_##NAME##_DeleteIndex(ArrayList_##NAME* const ArrayList, const int Index, TYPE* const Result);         \
    extern CODE ArrayList_##NAME##_SetIndex(ArrayList_##NAME* const ArrayList, TYPE Data, const int Index, TYPE* const Result); \
    extern CODE ArrayList_##NAME##_GetFirst(ArrayList_##NAME* const ArrayList, TYPE* const Result);                             \
    extern CODE ArrayList_##NAME##_GetLast(ArrayList_##NAME* const ArrayList, TYPE* const Result);                              \
    extern CODE ArrayList_##NAME##_GetIndex(ArrayList_##NAME* const ArrayList, const int Index, TYPE* const Result);            \
    extern int  ArrayList_##NAME##_IndexOf(const ArrayList_##NAME* const ArrayList, TYPE Data);                                 \
    extern int  ArrayList_##NAME##_LastIndexOf(const ArrayList_##NAME* const ArrayList, TYPE Data);                             \
    extern int  ArrayList_##NAME##_GetSize(const ArrayList_##NAME* const ArrayList);                                            \
    extern bool ArrayList_##NAME##_IsEmpty(const ArrayList_##NAME* const ArrayList);                                            \
    extern bool ArrayList_##NAME##_Contains(const ArrayList_##NAME* const ArrayList, const TYPE Data);                          \
    extern void ArrayList_##NAME##_Clear(ArrayList_##NAME* const ArrayList);                                                    \
    extern void ArrayList_##NAME##_Destroy(ArrayList_##NAME* const ArrayList);


#define ARRAYLIST_IMPL(NAME, TYPE, SCOPE, Equals_Function)                                                                                                        \
    SCOPE bool ArrayList_##NAME##_CheckIndex(const ArrayList_##NAME* const ArrayList, const int Index)                                                            \
    {                                                                                                                                                             \
        if (unlikely(Index < 0 || Index >= ArrayList->Size)) return false;                                                                                        \
        else return true;                                                                                                                                         \
    }                                                                                                                                                             \
    SCOPE void ArrayList_##NAME##_Init(ArrayList_##NAME* const ArrayList)                                                                                         \
    {                                                                                                                                                             \
        ArrayList->Array    = NULL;                                                                                                                               \
        ArrayList->Size     = 0;                                                                                                                                  \
        ArrayList->Capacity = 0;                                                                                                                                  \
    }                                                                                                                                                             \
    SCOPE CODE ArrayList_##NAME##_newCapacity(const int OldCapacity, const int MinGrowth, const int PrefGrowth, int* const Result)                                \
    {                                                                                                                                                             \
        int PrefCapacity = OldCapacity + ((MinGrowth > PrefGrowth) ? MinGrowth : PrefGrowth);                                                                     \
        if (PrefCapacity > 0 && PrefCapacity <= SOFT_MAX_ARRAYLIST_CAPACITY)                                                                                      \
        {                                                                                                                                                         \
            *Result = PrefCapacity;                                                                                                                               \
            return Success;                                                                                                                                       \
        }                                                                                                                                                         \
        else                                                                                                                                                      \
        {                                                                                                                                                         \
            int MinCapacity = OldCapacity + MinGrowth;                                                                                                            \
            if (MinCapacity < 0) return MemoryOverflow; /* Overflow */                                                                                            \
            else if (MinCapacity <= SOFT_MAX_ARRAYLIST_CAPACITY)                                                                                                  \
            {                                                                                                                                                     \
                *Result = SOFT_MAX_ARRAYLIST_CAPACITY;                                                                                                            \
                return Success;                                                                                                                                   \
            }                                                                                                                                                     \
            else                                                                                                                                                  \
            {                                                                                                                                                     \
                *Result = MinCapacity;                                                                                                                            \
                return Success;                                                                                                                                   \
            }                                                                                                                                                     \
        }                                                                                                                                                         \
    }                                                                                                                                                             \
    SCOPE CODE ArrayList_##NAME##_Resize(ArrayList_##NAME* const ArrayList, const int MinCapacity)                                                                \
    {                                                                                                                                                             \
        int OldCapacity = ArrayList->Capacity;                                                                                                                    \
        int NewCapacity;                                                                                                                                          \
        if (OldCapacity > 0 || ArrayList->Array != NULL)                                                                                                          \
        {                                                                                                                                                         \
            if (Success == ArrayList_##NAME##_newCapacity(OldCapacity, MinCapacity - OldCapacity, OldCapacity >> 1, &NewCapacity))                                \
            {                                                                                                                                                     \
                TYPE* EA = (TYPE*)realloc(ArrayList->Array, NewCapacity * sizeof(TYPE));                                                                          \
                if (unlikely(EA == NULL)) return MemoryAllocationError;                                                                                           \
                ArrayList->Array    = EA;                                                                                                                         \
                ArrayList->Capacity = NewCapacity;                                                                                                                \
                return Success;                                                                                                                                   \
            }                                                                                                                                                     \
            else return MemoryOverflow;                                                                                                                           \
        }                                                                                                                                                         \
        else                                                                                                                                                      \
        {                                                                                                                                                         \
            NewCapacity = (MinCapacity > DEFAULT_ARRAYLIST_CAPACITY) ? MinCapacity : DEFAULT_ARRAYLIST_CAPACITY;                                                  \
            TYPE* EA    = (TYPE*)malloc(NewCapacity * sizeof(TYPE));                                                                                              \
            if (unlikely(EA == NULL)) return MemoryAllocationError;                                                                                               \
            ArrayList->Array    = EA;                                                                                                                             \
            ArrayList->Capacity = NewCapacity;                                                                                                                    \
            return Success;                                                                                                                                       \
        }                                                                                                                                                         \
    }                                                                                                                                                             \
    SCOPE CODE ArrayList_##NAME##_AddIndex_UnSafe(ArrayList_##NAME* const ArrayList, const TYPE Data, const int Index)                                            \
    {                                                                                                                                                             \
        if (unlikely(ArrayList->Size == ArrayList->Capacity))                                                                                                     \
        {                                                                                                                                                         \
            if (Success != ArrayList_##NAME##_Resize(ArrayList, ArrayList->Size + 1)) return MemoryAllocationError;                                               \
        }                                                                                                                                                         \
        /* move all elements after Index one position to the right */                                                                                             \
        memmove(ArrayList->Array + Index + 1, ArrayList->Array + Index, (ArrayList->Size - Index) * sizeof(TYPE));                                                \
        ArrayList->Array[Index] = Data;                                                                                                                           \
        ++ArrayList->Size;                                                                                                                                        \
        return Success;                                                                                                                                           \
    }                                                                                                                                                             \
    SCOPE CODE ArrayList_##NAME##_AddFirst(ArrayList_##NAME* const ArrayList, const TYPE Data) { return ArrayList_##NAME##_AddIndex_UnSafe(ArrayList, Data, 0); } \
    SCOPE CODE ArrayList_##NAME##_AddLast(ArrayList_##NAME* const ArrayList, const TYPE Data)                                                                     \
    {                                                                                                                                                             \
        if (unlikely(ArrayList->Size == ArrayList->Capacity))                                                                                                     \
        {                                                                                                                                                         \
            if (Success != ArrayList_##NAME##_Resize(ArrayList, ArrayList->Size + 1)) return MemoryAllocationError;                                               \
        }                                                                                                                                                         \
        ArrayList->Array[ArrayList->Size++] = Data;                                                                                                               \
        return Success;                                                                                                                                           \
    }                                                                                                                                                             \
    SCOPE CODE ArrayList_##NAME##_AddIndex_Safe(ArrayList_##NAME* const ArrayList, const TYPE Data, const int Index)                                              \
    {                                                                                                                                                             \
        if (unlikely(!ArrayList_##NAME##_CheckIndex(ArrayList, Index))) return ArrayIndexOutOfRange;                                                              \
        return ArrayList_##NAME##_AddIndex_UnSafe(ArrayList, Data, Index);                                                                                        \
    }                                                                                                                                                             \
    SCOPE void ArrayList_##NAME##_FastDeleteIndex(ArrayList_##NAME* const ArrayList, const int Index)                                                             \
    {                                                                                                                                                             \
        int NewSize = ArrayList->Size - 1;                                                                                                                        \
        if (unlikely(NewSize > Index)) memmove(ArrayList->Array + Index, ArrayList->Array + Index + 1, (NewSize - Index) * sizeof(TYPE));                         \
        ArrayList->Size = NewSize;                                                                                                                                \
    }                                                                                                                                                             \
    SCOPE CODE ArrayList_##NAME##_DeleteFirst(ArrayList_##NAME* const ArrayList, TYPE* const Result)                                                              \
    {                                                                                                                                                             \
        if (unlikely(ArrayList->Size == 0)) return ArrayIndexOutOfRange;                                                                                          \
        *Result = ArrayList->Array[0];                                                                                                                            \
        ArrayList_##NAME##_FastDeleteIndex(ArrayList, 0);                                                                                                         \
        return Success;                                                                                                                                           \
    }                                                                                                                                                             \
    SCOPE CODE ArrayList_##NAME##_DeleteLast(ArrayList_##NAME* const ArrayList, TYPE* const Result)                                                               \
    {                                                                                                                                                             \
        if (unlikely(ArrayList->Size == 0)) return ArrayIndexOutOfRange;                                                                                          \
        *Result = ArrayList->Array[ArrayList->Size - 1];                                                                                                          \
        ArrayList_##NAME##_FastDeleteIndex(ArrayList, ArrayList->Size - 1);                                                                                       \
        return Success;                                                                                                                                           \
    }                                                                                                                                                             \
    SCOPE CODE ArrayList_##NAME##_DeleteIndex(ArrayList_##NAME* const ArrayList, const int Index, TYPE* const Result)                                             \
    {                                                                                                                                                             \
        if (unlikely(!ArrayList_##NAME##_CheckIndex(ArrayList, Index))) return ArrayIndexOutOfRange;                                                              \
        *Result = ArrayList->Array[Index];                                                                                                                        \
        ArrayList_##NAME##_FastDeleteIndex(ArrayList, Index);                                                                                                     \
        return Success;                                                                                                                                           \
    }                                                                                                                                                             \
    SCOPE CODE ArrayList_##NAME##_SetIndex(ArrayList_##NAME* const ArrayList, TYPE Data, const int Index, TYPE* const Result)                                     \
    {                                                                                                                                                             \
        if (unlikely(!ArrayList_##NAME##_CheckIndex(ArrayList, Index))) return ArrayIndexOutOfRange;                                                              \
        *Result                 = ArrayList->Array[Index];                                                                                                        \
        ArrayList->Array[Index] = Data;                                                                                                                           \
        return Success;                                                                                                                                           \
    }                                                                                                                                                             \
    SCOPE CODE ArrayList_##NAME##_GetFirst(ArrayList_##NAME* const ArrayList, TYPE* const Result)                                                                 \
    {                                                                                                                                                             \
        if (unlikely(ArrayList->Size == 0)) return ArrayIndexOutOfRange;                                                                                          \
        *Result = ArrayList->Array[0];                                                                                                                            \
        return Success;                                                                                                                                           \
    }                                                                                                                                                             \
    SCOPE CODE ArrayList_##NAME##_GetLast(ArrayList_##NAME* const ArrayList, TYPE* const Result)                                                                  \
    {                                                                                                                                                             \
        if (unlikely(ArrayList->Size == 0)) return ArrayIndexOutOfRange;                                                                                          \
        *Result = ArrayList->Array[ArrayList->Size - 1];                                                                                                          \
        return Success;                                                                                                                                           \
    }                                                                                                                                                             \
    SCOPE CODE ArrayList_##NAME##_GetIndex(ArrayList_##NAME* const ArrayList, const int Index, TYPE* const Result)                                                \
    {                                                                                                                                                             \
        if (unlikely(!ArrayList_##NAME##_CheckIndex(ArrayList, Index))) return ArrayIndexOutOfRange;                                                              \
        *Result = ArrayList->Array[Index];                                                                                                                        \
        return Success;                                                                                                                                           \
    }                                                                                                                                                             \
    SCOPE int ArrayList_##NAME##_IndexOf(const ArrayList_##NAME* const ArrayList, TYPE Data)                                                                      \
    {                                                                                                                                                             \
        for (int i = 0; i < ArrayList->Size; ++i)                                                                                                                 \
        {                                                                                                                                                         \
            if (Equals_Function(Data, ArrayList->Array[i])) return i;                                                                                             \
        }                                                                                                                                                         \
        return -1;                                                                                                                                                \
    }                                                                                                                                                             \
    SCOPE int ArrayList_##NAME##_LastIndexOf(const ArrayList_##NAME* const ArrayList, TYPE Data)                                                                  \
    {                                                                                                                                                             \
        for (int i = ArrayList->Size - 1; i >= 0; --i)                                                                                                            \
        {                                                                                                                                                         \
            if (Equals_Function(Data, ArrayList->Array[i])) return i;                                                                                             \
        }                                                                                                                                                         \
        return -1;                                                                                                                                                \
    }                                                                                                                                                             \
    SCOPE int  ArrayList_##NAME##_GetSize(const ArrayList_##NAME* const ArrayList) { return ArrayList->Size; }                                                    \
    SCOPE bool ArrayList_##NAME##_IsEmpty(const ArrayList_##NAME* const ArrayList) { return ArrayList->Size == 0; }                                               \
    SCOPE bool ArrayList_##NAME##_Contains(const ArrayList_##NAME* const ArrayList, const TYPE Data) { return ArrayList_##NAME##_IndexOf(ArrayList, Data) >= 0; } \
    SCOPE void ArrayList_##NAME##_Clear(ArrayList_##NAME* const ArrayList) { ArrayList->Size = 0; }                                                               \
    SCOPE void ArrayList_##NAME##_Destroy(ArrayList_##NAME* const ArrayList)                                                                                      \
    {                                                                                                                                                             \
        free(ArrayList->Array);                                                                                                                                   \
        ArrayList->Array    = NULL;                                                                                                                               \
        ArrayList->Size     = 0;                                                                                                                                  \
        ArrayList->Capacity = 0;                                                                                                                                  \
    }


#define ARRAYLIST_DECLARE(NAME, TYPE) \
    ARRAYLIST_TYPE(NAME, TYPE)        \
    ARRAYLIST_PROTOTYPES(NAME, TYPE)


#define ARRAYLIST_INIT(NAME, TYPE, SCOPE, Equals_Function) \
    ARRAYLIST_TYPE(NAME, TYPE)                             \
    ARRAYLIST_IMPL(NAME, TYPE, SCOPE, Equals_Function)

/**
 * @brief Get the ArrayList_##NAME struct
 * @param NAME
 */
#define ArrayList(NAME)                                          ArrayList_##NAME

/**
 * @brief Initialize the ArrayList_##NAME struct
 * @param NAME The name of the function
 * @param ArrayList The ArrayList_##NAME struct
 * @return CODE The result of the function
 */
#define ArrayList_Init(NAME, ArrayList)                          ArrayList_##NAME##_Init((ArrayList))

/**
 * @brief Add the Data to the first of the ArrayList_##NAME struct
 * @param NAME The name of the function
 * @param ArrayList The ArrayList_##NAME struct
 * @param Data The data to be added
 * @return CODE The result of the function
 */
#define ArrayList_AddFirst(NAME, ArrayList, Data)                ArrayList_##NAME##_AddFirst((ArrayList), (Data))

/**
 * @brief Add the Data to the last of the ArrayList_##NAME struct
 * @param NAME The name of the function
 * @param ArrayList The ArrayList_##NAME struct
 * @param Data The data to be added
 * @return CODE The result of the function
 */
#define ArrayList_AddLast(NAME, ArrayList, Data)                 ArrayList_##NAME##_AddLast((ArrayList), (Data))

/**
 * @brief Add the Data to the Index of the ArrayList_##NAME struct
 * @param NAME The name of the function
 * @param ArrayList The ArrayList_##NAME struct
 * @param Data The data to be added
 * @param Index The index of the data to be added
 * @return CODE The result of the function
 * @note The function can be optimized compared to the vector
 */
#define ArrayList_AddIndex(NAME, ArrayList, Data, Index)         ArrayList_##NAME##_AddIndex_Safe((ArrayList), (Data), (Index))

/**
 * @brief Delete the first data of the ArrayList_##NAME struct
 * @param NAME The name of the function
 * @param ArrayList The ArrayList_##NAME struct
 * @param Result To store the deleted element
 * @return CODE The result of the function
 */
#define ArrayList_DeleteFirst(NAME, ArrayList, Result)           ArrayList_##NAME##_DeleteFirst((ArrayList), (Result))

/**
 * @brief Delete the last data of the ArrayList_##NAME struct
 * @param NAME The name of the function
 * @param ArrayList The ArrayList_##NAME struct
 * @param Result To store the deleted element
 * @return CODE The result of the function
 */
#define ArrayList_DeleteLast(NAME, ArrayList, Result)            ArrayList_##NAME##_DeleteLast((ArrayList), (Result))

/**
 * @brief Delete the Index data of the ArrayList_##NAME struct
 * @param NAME The name of the function
 * @param ArrayList The ArrayList_##NAME struct
 * @param Index The index of the data to be deleted
 * @param Result To store the deleted element
 * @return CODE The result of the function
 * @note The function can be optimized compared to the vector
 */
#define ArrayList_DeleteIndex(NAME, ArrayList, Index, Result)    ArrayList_##NAME##_DeleteIndex((ArrayList), (Index), (Result))

/**
 * @brief Set the Index data of the ArrayList_##NAME struct
 * @param NAME The name of the function
 * @param ArrayList The ArrayList_##NAME struct
 * @param Data The data to be set
 * @param Index The index of the data to be set
 * @param Result To store the old element
 * @return CODE The result of the function
 */
#define ArrayList_SetIndex(NAME, ArrayList, Data, Index, Result) ArrayList_##NAME##_SetIndex((ArrayList), (Data), (Index), (Result))

/**
 * @brief Get the first data of the ArrayList_##NAME struct
 * @param NAME The name of the function
 * @param ArrayList The ArrayList_##NAME struct
 * @param Result To store the first element
 * @return CODE The result of the function
 */
#define ArrayList_GetFirst(NAME, ArrayList, Result)              ArrayList_##NAME##_GetFirst((ArrayList), (Result))

/**
 * @brief Get the last data of the ArrayList_##NAME struct
 * @param NAME The name of the function
 * @param ArrayList The ArrayList_##NAME struct
 * @param Result To store the last element
 * @return CODE The result of the function
 */
#define ArrayList_GetLast(NAME, ArrayList, Result)               ArrayList_##NAME##_GetLast((ArrayList), (Result))

/**
 * @brief Get the Index data of the ArrayList_##NAME struct
 * @param NAME The name of the function
 * @param ArrayList The ArrayList_##NAME struct
 * @param Index The index of the data to be get
 * @param Result To store the index of the element
 * @return CODE The result of the function
 */
#define ArrayList_GetIndex(NAME, ArrayList, Index, Result)       ArrayList_##NAME##_GetIndex((ArrayList), (Index), (Result))

/**
 * @brief Get the first Index of the Data in the ArrayList_##NAME struct
 * @param NAME The name of the function
 * @param ArrayList The ArrayList_##NAME struct
 * @param Data The data to be found
 * @return int The index of the data
 */
#define ArrayList_IndexOf(NAME, ArrayList, Data)                 ArrayList_##NAME##_IndexOf((ArrayList), (Data))

/**
 * @brief Get the last Index of the Data in the ArrayList_##NAME struct
 * @param NAME The name of the function
 * @param ArrayList The ArrayList_##NAME struct
 * @param Data The data to be found
 * @return int The index of the data
 */
#define ArrayList_LastIndexOf(NAME, ArrayList, Data)             ArrayList_##NAME##_LastIndexOf((ArrayList), (Data))

/**
 * @brief Get the size of the ArrayList_##NAME struct
 * @param NAME The name of the function
 * @param ArrayList The ArrayList_##NAME struct
 * @return int The size of the ArrayList_##NAME struct
 */
#define ArrayList_GetSize(ArrayList)                             ((ArrayList)->Size)

/**
 * @brief Check the ArrayList_##NAME struct is empty
 * @param NAME The name of the function
 * @param ArrayList The ArrayList_##NAME struct
 * @retval true The ArrayList_##NAME struct is empty
 * @retval false The ArrayList_##NAME struct is not empty
 */
#define ArrayList_IsEmpty(ArrayList)                             ((ArrayList)->Size == 0)

/**
 * @brief Check the ArrayList_##NAME struct contains the Data
 * @param NAME The name of the function
 * @param ArrayList The ArrayList_##NAME struct
 * @param Data The data to be found
 * @retval true The ArrayList_##NAME struct contains the Data
 * @retval false The ArrayList_##NAME struct does not contain the Data
 */
#define ArrayList_Contains(NAME, ArrayList, Data)                (ArrayList_##NAME##_IndexOf((ArrayList), (Data)) >= 0)

/**
 * @brief Clear the ArrayList_##NAME struct, it will not free the memory
 * @param NAME The name of the function
 * @param ArrayList The ArrayList_##NAME struct
 */
#define ArrayList_Clear(ArrayList)                               ((ArrayList)->Size = 0)

/**
 * @brief Destroy the ArrayList_##NAME struct, it will free the memory
 * @param NAME The name of the function
 * @param ArrayList The ArrayList_##NAME struct
 */
#define ArrayList_Destroy(NAME, ArrayList)                       ArrayList_##NAME##_Destroy((ArrayList))

// Function implementation
#define LTT_ARRAYLIST_INIT(NAME, TYPE, Equals_Function)          ARRAYLIST_INIT(NAME, TYPE, static LTT_inline LTT_unused, Equals_Function)

// Function declaration
#define LTT_ARRAYLIST_DECLARE(NAME, TYPE)                        ARRAYLIST_DECLARE(NAME, TYPE)