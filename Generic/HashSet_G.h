#pragma once
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "GeneralHashFunctions.h"
#include "Generic_tool.h"

/*
 * The default initial capacity - MUST be a power of two.
*/
#define DEFAULT_HASHSET_INITIAL_CAPACITY (1 << 4)

/*
  * The maximum capacity, used if a higher value is implicitly specified
  * by either of the constructors with arguments.
  * MUST be a power of two <= 1<<30.
*/
#define MAXIMUM_HASHSET_CAPACITY         (1 << 30)
/*
  * The load factor used when none specified in constructor.
*/
#define DEFAULT_HASHSET_LOAD_FACTOR      (0.75f)

/*
    选择一个哈希函数
    RSHash
    JSHash
    PJWHash
    ELFHash
    BKDRHash
    SDBMHash
    DJBHash
    DEKHash
    BPHash
    FNVHash
    APHash
*/
#define String_Hash_Function             DJBHash


#define HASHSET_TYPE(NAME, TYPE)             \
    typedef struct HashSetNode_##NAME        \
    {                                        \
        TYPE                       Key;      \
        int                        HashCode; \
        struct HashSetNode_##NAME* Next;     \
    } HashSetNode_##NAME;                    \
    typedef struct HashSet_##NAME            \
    {                                        \
        int                  Size;           \
        int                  Capacity;       \
        int                  Threshold;      \
        float                LoadFactor;     \
        HashSetNode_##NAME** Table;          \
    } HashSet_##NAME;

#define HASHSET_IMPL(NAME, TYPE, SCOPE, Hash_Function, Equals_Function)                                                                                              \
    SCOPE CODE HashSet_##NAME##_Init(HashSet_##NAME* HashSet)                                                                                                        \
    {                                                                                                                                                                \
        HashSet->Size       = 0;                                                                                                                                     \
        HashSet->Capacity   = 0;                                                                                                                                     \
        HashSet->Threshold  = 0;                                                                                                                                     \
        HashSet->LoadFactor = DEFAULT_HASHSET_LOAD_FACTOR;                                                                                                           \
        HashSet->Table      = NULL;                                                                                                                                  \
        return Success;                                                                                                                                              \
    }                                                                                                                                                                \
    SCOPE int HashSet_##NAME##_HashCode(const TYPE Key)                                                                                                              \
    {                                                                                                                                                                \
        int h = Hash_Function(Key);                                                                                                                                  \
        return h ^ (h >> 16);                                                                                                                                        \
    }                                                                                                                                                                \
    SCOPE CODE HashSet_##NAME##_MakeNode(const int HashCode, const TYPE Key, HashSetNode_##NAME* Next, HashSetNode_##NAME** const Result)                            \
    {                                                                                                                                                                \
        *Result = (HashSetNode_##NAME*)malloc(sizeof(HashSetNode_##NAME));                                                                                           \
        if (unlikely(*Result == NULL)) return MemoryAllocationError;                                                                                                 \
        (*Result)->Key      = Key;                                                                                                                                   \
        (*Result)->HashCode = HashCode;                                                                                                                              \
        (*Result)->Next     = Next;                                                                                                                                  \
        return Success;                                                                                                                                              \
    }                                                                                                                                                                \
    SCOPE CODE HashSet_##NAME##_Resize(HashSet_##NAME* const HashSet)                                                                                                \
    {                                                                                                                                                                \
        HashSetNode_##NAME** OldTab       = HashSet->Table;                                                                                                          \
        int                  OldCapacity  = (OldTab == NULL) ? 0 : HashSet->Capacity;                                                                                \
        int                  OldThreshold = HashSet->Threshold;                                                                                                      \
        int                  NewCapacity  = 0;                                                                                                                       \
        int                  NewThreshold = 0;                                                                                                                       \
        if (OldCapacity > 0)                                                                                                                                         \
        {                                                                                                                                                            \
            if (OldCapacity >= MAXIMUM_HASHSET_CAPACITY)                                                                                                             \
            {                                                                                                                                                        \
                HashSet->Threshold = INT_MAX;                                                                                                                        \
                return Success;                                                                                                                                      \
            }                                                                                                                                                        \
            else if ((NewCapacity = OldCapacity << 1) < MAXIMUM_HASHSET_CAPACITY && OldCapacity >= DEFAULT_HASHSET_INITIAL_CAPACITY)                                 \
            {                                                                                                                                                        \
                HashSet->Capacity = NewCapacity;                                                                                                                     \
                NewThreshold      = OldThreshold << 1;                                                                                                               \
            }                                                                                                                                                        \
        }                                                                                                                                                            \
        else if (OldThreshold > 0) HashSet->Capacity = NewCapacity = OldThreshold;                                                                                   \
        else                                                                                                                                                         \
        {                                                                                                                                                            \
            HashSet->Capacity = NewCapacity = DEFAULT_HASHSET_INITIAL_CAPACITY;                                                                                      \
            NewThreshold                    = (int)(DEFAULT_HASHSET_LOAD_FACTOR * DEFAULT_HASHSET_INITIAL_CAPACITY);                                                 \
        }                                                                                                                                                            \
        if (NewThreshold == 0)                                                                                                                                       \
        {                                                                                                                                                            \
            float ft     = (float)NewCapacity * HashSet->LoadFactor;                                                                                                 \
            NewThreshold = (NewCapacity < MAXIMUM_HASHSET_CAPACITY && ft < (float)MAXIMUM_HASHSET_CAPACITY ? (int)ft : INT_MAX);                                     \
        }                                                                                                                                                            \
        HashSet->Threshold          = NewThreshold;                                                                                                                  \
        HashSetNode_##NAME** NewTab = (HashSetNode_##NAME**)calloc(NewCapacity, sizeof(HashSetNode_##NAME*));                                                        \
        if (unlikely(NewTab == NULL)) return MemoryAllocationError;                                                                                                  \
        HashSet->Table = NewTab;                                                                                                                                     \
        if (OldTab != NULL)                                                                                                                                          \
        {                                                                                                                                                            \
            for (int i = 0; i < OldCapacity; ++i)                                                                                                                    \
            {                                                                                                                                                        \
                HashSetNode_##NAME* Temp;                                                                                                                            \
                if ((Temp = OldTab[i]) != NULL)                                                                                                                      \
                {                                                                                                                                                    \
                    OldTab[i] = NULL;                                                                                                                                \
                    if (Temp->Next == NULL) { NewTab[Temp->HashCode & (NewCapacity - 1)] = Temp; }                                                                   \
                    else                                                                                                                                             \
                    {                                                                                                                                                \
                        HashSetNode_##NAME* UnChangeHead = NULL;                                                                                                     \
                        HashSetNode_##NAME* UnChangeTail = NULL;                                                                                                     \
                        HashSetNode_##NAME* ChangeHead   = NULL;                                                                                                     \
                        HashSetNode_##NAME* ChangeTail   = NULL;                                                                                                     \
                        HashSetNode_##NAME* Next;                                                                                                                    \
                        do {                                                                                                                                         \
                            Next = Temp->Next;                                                                                                                       \
                            if ((Temp->HashCode & OldCapacity) == 0)                                                                                                 \
                            {                                                                                                                                        \
                                if (UnChangeHead == NULL) UnChangeHead = Temp;                                                                                       \
                                else UnChangeTail->Next = Temp;                                                                                                      \
                                UnChangeTail = Temp;                                                                                                                 \
                            }                                                                                                                                        \
                            else                                                                                                                                     \
                            {                                                                                                                                        \
                                if (ChangeHead == NULL) ChangeHead = Temp;                                                                                           \
                                else ChangeTail->Next = Temp;                                                                                                        \
                                ChangeTail = Temp;                                                                                                                   \
                            }                                                                                                                                        \
                        }                                                                                                                                            \
                        while ((Temp = Next) != NULL);                                                                                                               \
                        if (UnChangeTail != NULL)                                                                                                                    \
                        {                                                                                                                                            \
                            UnChangeTail->Next = NULL;                                                                                                               \
                            NewTab[i]          = UnChangeHead;                                                                                                       \
                        }                                                                                                                                            \
                        if (ChangeTail != NULL)                                                                                                                      \
                        {                                                                                                                                            \
                            ChangeTail->Next        = NULL;                                                                                                          \
                            NewTab[i + OldCapacity] = ChangeHead;                                                                                                    \
                        }                                                                                                                                            \
                    }                                                                                                                                                \
                }                                                                                                                                                    \
            }                                                                                                                                                        \
        }                                                                                                                                                            \
        free(OldTab);                                                                                                                                                \
        return Success;                                                                                                                                              \
    }                                                                                                                                                                \
    SCOPE bool HashSet_##NAME##_Add(HashSet_##NAME* const HashSet, const TYPE Key)                                                                                   \
    {                                                                                                                                                                \
        HashSetNode_##NAME** Table;                                                                                                                                  \
        HashSetNode_##NAME** LocatedNode;                                                                                                                            \
        int                  Capacity;                                                                                                                               \
        int                  HashCode = HashSet_##NAME##_HashCode(Key);                                                                                              \
        if ((Table = HashSet->Table) == NULL || (Capacity = HashSet->Capacity) == 0)                                                                                 \
        {                                                                                                                                                            \
            HashSet_##NAME##_Resize(HashSet);                                                                                                                        \
            Table    = HashSet->Table;                                                                                                                               \
            Capacity = HashSet->Capacity;                                                                                                                            \
        }                                                                                                                                                            \
        LocatedNode = &Table[(Capacity - 1) & HashCode];                                                                                                             \
        while (*LocatedNode)                                                                                                                                         \
        {                                                                                                                                                            \
            if ((*LocatedNode)->HashCode == HashCode && Equals_Function(Key, (*LocatedNode)->Key)) return false;                                                     \
            LocatedNode = &(*LocatedNode)->Next;                                                                                                                     \
        }                                                                                                                                                            \
        if (Success != HashSet_##NAME##_MakeNode(HashCode, Key, NULL, LocatedNode)) return false;                                                                    \
        if (++HashSet->Size > HashSet->Threshold) HashSet_##NAME##_Resize(HashSet);                                                                                  \
        return true;                                                                                                                                                 \
    }                                                                                                                                                                \
    SCOPE bool HashSet_##NAME##_Delete(HashSet_##NAME* const HashSet, const TYPE Key)                                                                                \
    {                                                                                                                                                                \
        HashSetNode_##NAME** Table;                                                                                                                                  \
        HashSetNode_##NAME** Prev;                                                                                                                                   \
        HashSetNode_##NAME*  Curr;                                                                                                                                   \
        int                  Capacity;                                                                                                                               \
        int                  HashCode = HashSet_##NAME##_HashCode(Key);                                                                                              \
        if ((Table = HashSet->Table) && (Capacity = HashSet->Capacity) != 0 && *(Prev = &Table[(Capacity - 1) & HashCode]))                                          \
        {                                                                                                                                                            \
            do {                                                                                                                                                     \
                Curr = *Prev;                                                                                                                                        \
                if (Curr->HashCode == HashCode && Equals_Function(Key, Curr->Key))                                                                                   \
                {                                                                                                                                                    \
                    *Prev = Curr->Next;                                                                                                                              \
                    --HashSet->Size;                                                                                                                                 \
                    free(Curr);                                                                                                                                      \
                    return true;                                                                                                                                     \
                }                                                                                                                                                    \
                else Prev = &Curr->Next;                                                                                                                             \
            }                                                                                                                                                        \
            while (*Prev);                                                                                                                                           \
        }                                                                                                                                                            \
        return false;                                                                                                                                                \
    }                                                                                                                                                                \
    SCOPE bool HashSet_##NAME##_Contains(const HashSet_##NAME* const HashSet, const TYPE Key)                                                                        \
    {                                                                                                                                                                \
        HashSetNode_##NAME** Table;                                                                                                                                  \
        HashSetNode_##NAME** LocatedNode;                                                                                                                            \
        int                  HashCode;                                                                                                                               \
        int                  Capacity;                                                                                                                               \
        if ((Table = HashSet->Table) && (Capacity = HashSet->Capacity) > 0 && *(LocatedNode = &Table[(HashCode = HashSet_##NAME##_HashCode(Key)) & (Capacity - 1)])) \
        {                                                                                                                                                            \
            do {                                                                                                                                                     \
                if ((*LocatedNode)->HashCode == HashCode && Equals_Function(Key, (*LocatedNode)->Key)) return true;                                                  \
            }                                                                                                                                                        \
            while (*(LocatedNode = &(*LocatedNode)->Next));                                                                                                          \
        }                                                                                                                                                            \
        return false;                                                                                                                                                \
    }                                                                                                                                                                \
    SCOPE bool HashSet_##NAME##_IsEmpty(const HashSet_##NAME* const HashSet) { return HashSet->Size == 0; }                                                          \
    SCOPE int  HashSet_##NAME##_GetSize(const HashSet_##NAME* const HashSet) { return HashSet->Size; }                                                               \
    SCOPE void HashSet_##NAME##_Clear(HashSet_##NAME* const HashSet)                                                                                                 \
    {                                                                                                                                                                \
        HashSetNode_##NAME** Table;                                                                                                                                  \
        HashSetNode_##NAME*  LocatedNode;                                                                                                                            \
        if ((Table = HashSet->Table) && HashSet->Size > 0)                                                                                                           \
        {                                                                                                                                                            \
            HashSet->Size = 0;                                                                                                                                       \
            for (int i = 0; i < HashSet->Capacity; ++i)                                                                                                              \
            {                                                                                                                                                        \
                while (Table[i])                                                                                                                                     \
                {                                                                                                                                                    \
                    LocatedNode = Table[i];                                                                                                                          \
                    Table[i]    = LocatedNode->Next;                                                                                                                 \
                    free(LocatedNode);                                                                                                                               \
                }                                                                                                                                                    \
            }                                                                                                                                                        \
            memset(Table, 0, HashSet->Capacity);                                                                                                                     \
        }                                                                                                                                                            \
    }                                                                                                                                                                \
    SCOPE void HashSet_##NAME##_Destroy(HashSet_##NAME* const HashSet)                                                                                               \
    {                                                                                                                                                                \
        HashSet_##NAME##_Clear(HashSet);                                                                                                                             \
        free(HashSet->Table);                                                                                                                                        \
        HashSet->Table    = NULL;                                                                                                                                    \
        HashSet->Capacity = 0;                                                                                                                                       \
    }

/*! @function
  @abstract     Integer hash function
 */
#define LTT_Int_Hash_Function(key)              ((unsigned int)(key))

/*! @function
  @abstract     Integer equality function
 */
#define LTT_Int_Equals_Function(key1, key2)     ((key1) == (key2))

/*! @function
  @abstract     64-bit integer hash function
 */
#define LTT_Int64_Hash_Function(key)            ((unsigned int)((key) ^ ((key) >> 32)))

/*! @function
  @abstract     64-bit integer equality function
 */
#define LTT_Int64_Equals_Function(key1, key2)   ((key1) == (key2))

/*! @function
  @abstract     String hash function
 */
#define LTT_String_Hash_Function(key)           String_Hash_Function(key, strlen(key))

/*! @function
  @abstract     String equality function
 */
#define LTT_String_Equals_Function(key1, key2)  (strcmp((const char*)(key1), (const char*)(key2)) == 0)

/*! @function
  @abstract     Double hash function
 */
#define LTT_Double_Hash_Function(key)           ((unsigned int)(key))

/*! @function
  @abstract     Double equality function
 */
#define LTT_Double_Equals_Function(key1, key2)  ((key1) == (key2))

/*! @function
  @abstract     Pointer hash function
 */
#define LTT_Pointer_Hash_Function(key)          ((unsigned int)(uintptr_t)(key))

/*! @function
  @abstract     Pointer equality function
 */
#define LTT_Pointer_Equals_Function(key1, key2) ((key1) == (key2))

/*! @function
  @abstract     Custom hash function
 */
#define LTT_Custom_Hash_Function(key)           String_Hash_Function((const char*)&key, sizeof(key))

/*! @function
  @abstract     Custom equality function
 */
#define LTT_Custom_Equals_Function(key1, key2)  (memcmp((const void*)(&key1), (const void*)(&key2), sizeof(key1)) == 0)




#define HASHSET_INIT2(NAME, TYPE, SCOPE, Hash_Function, Equals_Function) \
    HASHSET_TYPE(NAME, TYPE)                                             \
    HASHSET_IMPL(NAME, TYPE, SCOPE, Hash_Function, Equals_Function)

#define HASHSET_INIT(NAME, TYPE, Hash_Function, Equals_Function)     HASHSET_INIT2(NAME, TYPE, static LTT_inline LTT_unused, Hash_Function, Equals_Function)

#define HashSet(NAME)                                                HashSet_##NAME
#define HashSet_Init(NAME, HashSet)                                  HashSet_##NAME##_Init((HashSet))
#define HashSet_Add(NAME, HashSet, Key)                              HashSet_##NAME##_Add((HashSet), (Key))
#define HashSet_Delete(NAME, HashSet, Key)                           HashSet_##NAME##_Delete((HashSet), (Key))
#define HashSet_Contains(NAME, HashSet, Key)                         HashSet_##NAME##_Contains((HashSet), (Key))
#define HashSet_IsEmpty(HashSet)                                     ((HashSet)->Size == 0)
#define HashSet_GetSize(HashSet)                                     ((HashSet)->Size)
#define HashSet_Clear(NAME, HashSet)                                 HashSet_##NAME##_Clear((HashSet))
#define HashSet_Destroy(NAME, HashSet)                               HashSet_##NAME##_Destroy((HashSet))

// 函数实现
/*
    @abstract     Instantiate a hash set containing integer keys
*/
#define LTT_HASHSET_INT_INIT(NAME)                                   HASHSET_INIT(NAME, unsigned int, LTT_Int_Hash_Function, LTT_Int_Equals_Function)

/*
    @abstract     Instantiate a hash set containing 64-bit integer keys
*/
#define LTT_HASHSET_INT64_INIT(NAME)                                 HASHSET_INIT(NAME, unsigned long, LTT_Int64_Hash_Function, LTT_Int64_Equals_Function)

/*
    @abstract     Instantiate a hash set containing char* keys
*/
#define LTT_HASHSET_STRING_INIT(NAME)                                HASHSET_INIT(NAME, char*, LTT_String_Hash_Function, LTT_String_Equals_Function)

/*
    @abstract     Instantiate a hash set containing double keys
*/
#define LTT_HASHSET_DOUBLE_INIT(NAME)                                HASHSET_INIT(NAME, double, LTT_Double_Hash_Function, LTT_Double_Equals_Function)

/*
    @abstract     Instantiate a hash set containing pointer keys
*/
#define LTT_HASHSET_POINTER_INIT(NAME)                               HASHSET_INIT(NAME, void*, LTT_Pointer_Hash_Function, LTT_Pointer_Equals_Function)

/*
    @abstract     Instantiate a hash set containing custom keys
*/
#define LTT_HASHSET_INIT(NAME, TYPE, Hash_Function, Equals_Function) HASHSET_INIT(NAME, TYPE, Hash_Function, Equals_Function)

/*
    @abstract     Instantiate a hash set containing custom keys with default hash function and equality function
*/
#define LTT_HASHSET_INIT_DEFAULT(NAME, TYPE)                         HASHSET_INIT(NAME, TYPE, LTT_Custom_Hash_Function, LTT_Custom_Equals_Function)