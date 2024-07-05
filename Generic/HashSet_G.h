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
 * The String hash function you can choose:
 * RSHash
 * JSHash
 * PJWHash
 * ELFHash
 * BKDRHash
 * SDBMHash
 * DJBHash
 * DEKHash
 * BPHash
 * FNV_1aHash
 * APHash
*/
#define String_Hash_Function             DJBHash


#define HASHSET_TYPE(NAME, TYPE)                        \
    typedef struct HashSetNode_##NAME                   \
    {                                                   \
        TYPE                       Key;                 \
        int                        HashCode;            \
        struct HashSetNode_##NAME* Next;                \
    } HashSetNode_##NAME;                               \
    typedef struct HashSet_##NAME                       \
    {                                                   \
        /* Array of the HashSetNode_##NAME* */          \
        HashSetNode_##NAME** Table;                     \
        /* The number of the elements in the HashSet */ \
        int Size;                                       \
        /* The capacity of the Table */                 \
        int Capacity;                                   \
        /* Threshold for expansion */                   \
        int   Threshold;                                \
        float LoadFactor;                               \
    } HashSet_##NAME;


#define HASHSET_PROTOTYPES(NAME, TYPE)                                                   \
    CODE HashSet_##NAME##_Init(HashSet_##NAME* HashSet);                                 \
    CODE HashSet_##NAME##_Add(HashSet_##NAME* const HashSet, const TYPE Key);            \
    bool HashSet_##NAME##_Delete(HashSet_##NAME* const HashSet, const TYPE Key);         \
    bool HashSet_##NAME##_Contains(const HashSet_##NAME* const HashSet, const TYPE Key); \
    bool HashSet_##NAME##_IsEmpty(const HashSet_##NAME* const HashSet);                  \
    int  HashSet_##NAME##_GetSize(const HashSet_##NAME* const HashSet);                  \
    void HashSet_##NAME##_Clear(HashSet_##NAME* const HashSet);                          \
    void HashSet_##NAME##_Destroy(HashSet_##NAME* const HashSet);


#define HASHSET_IMPL(NAME, TYPE, SCOPE, Hash_Function, Equals_Function)                                                                                                                                                 \
    SCOPE CODE HashSet_##NAME##_Init(HashSet_##NAME* HashSet)                                                                                                                                                           \
    {                                                                                                                                                                                                                   \
        HashSet->Size       = 0;                                                                                                                                                                                        \
        HashSet->Capacity   = 0;                                                                                                                                                                                        \
        HashSet->Threshold  = 0;                                                                                                                                                                                        \
        HashSet->LoadFactor = DEFAULT_HASHSET_LOAD_FACTOR;                                                                                                                                                              \
        HashSet->Table      = NULL;                                                                                                                                                                                     \
        return Success;                                                                                                                                                                                                 \
    }                                                                                                                                                                                                                   \
    SCOPE int HashSet_##NAME##_HashCode(const TYPE Key)                                                                                                                                                                 \
    {                                                                                                                                                                                                                   \
        int h = Hash_Function(Key);                                                                                                                                                                                     \
        return h ^ (h >> 16);                                                                                                                                                                                           \
    }                                                                                                                                                                                                                   \
    SCOPE CODE HashSet_##NAME##_MakeNode(const int HashCode, const TYPE Key, HashSetNode_##NAME* Next, HashSetNode_##NAME** const Result)                                                                               \
    {                                                                                                                                                                                                                   \
        *Result = (HashSetNode_##NAME*)malloc(sizeof(HashSetNode_##NAME));                                                                                                                                              \
        if (unlikely(*Result == NULL)) return MemoryAllocationError;                                                                                                                                                    \
        (*Result)->Key      = Key;                                                                                                                                                                                      \
        (*Result)->HashCode = HashCode;                                                                                                                                                                                 \
        (*Result)->Next     = Next;                                                                                                                                                                                     \
        return Success;                                                                                                                                                                                                 \
    }                                                                                                                                                                                                                   \
    SCOPE CODE HashSet_##NAME##_Resize(HashSet_##NAME* const HashSet)                                                                                                                                                   \
    {                                                                                                                                                                                                                   \
        HashSetNode_##NAME** OldTab       = HashSet->Table;                                                                                                                                                             \
        int                  OldCapacity  = (OldTab == NULL) ? 0 : HashSet->Capacity;                                                                                                                                   \
        int                  OldThreshold = HashSet->Threshold;                                                                                                                                                         \
        int                  NewCapacity  = 0;                                                                                                                                                                          \
        int                  NewThreshold = 0;                                                                                                                                                                          \
        /* If the previous array capacity is not zero */                                                                                                                                                                \
        if (OldCapacity > 0)                                                                                                                                                                                            \
        {                                                                                                                                                                                                               \
            /* If the current array capacity is greater than the maximum value */                                                                                                                                       \
            if (OldCapacity >= MAXIMUM_HASHSET_CAPACITY)                                                                                                                                                                \
            {                                                                                                                                                                                                           \
                /* The expansion threshold is set to the maximum value of the Int type, which rarely occurs */                                                                                                          \
                HashSet->Threshold = INT_MAX;                                                                                                                                                                           \
                return Success;                                                                                                                                                                                         \
            }                                                                                                                                                                                                           \
            /* If the OldTab array length is not that large, expand it to twice its size, and expand the threshold as well */                                                                                           \
            else if ((NewCapacity = OldCapacity << 1) < MAXIMUM_HASHSET_CAPACITY && OldCapacity >= DEFAULT_HASHSET_INITIAL_CAPACITY)                                                                                    \
            {                                                                                                                                                                                                           \
                HashSet->Capacity = NewCapacity;                                                                                                                                                                        \
                NewThreshold      = OldThreshold << 1;                                                                                                                                                                  \
            }                                                                                                                                                                                                           \
        }                                                                                                                                                                                                               \
        /* If the previous capacity is 0 but the previous threshold is greater than zero, the new hash table length equals the current threshold */                                                                     \
        else if (OldThreshold > 0) HashSet->Capacity = NewCapacity = OldThreshold;                                                                                                                                      \
        /* When initializing, use the default threshold = array length * load factor */                                                                                                                                 \
        else                                                                                                                                                                                                            \
        {                                                                                                                                                                                                               \
            HashSet->Capacity = NewCapacity = DEFAULT_HASHSET_INITIAL_CAPACITY;                                                                                                                                         \
            NewThreshold                    = (int)(DEFAULT_HASHSET_LOAD_FACTOR * DEFAULT_HASHSET_INITIAL_CAPACITY);                                                                                                    \
        }                                                                                                                                                                                                               \
        /* If the new threshold is 0, recalculate using the new capacity * load factor */                                                                                                                               \
        if (NewThreshold == 0)                                                                                                                                                                                          \
        {                                                                                                                                                                                                               \
            float ft     = (float)NewCapacity * HashSet->LoadFactor;                                                                                                                                                    \
            NewThreshold = (NewCapacity < MAXIMUM_HASHSET_CAPACITY && ft < (float)MAXIMUM_HASHSET_CAPACITY ? (int)ft : INT_MAX);                                                                                        \
        }                                                                                                                                                                                                               \
        HashSet->Threshold          = NewThreshold;                                                                                                                                                                     \
        /* Initialize the new hash table and set it to NULL */                                                                                                                                                          \
        HashSetNode_##NAME** NewTab = (HashSetNode_##NAME**)calloc(NewCapacity, sizeof(HashSetNode_##NAME*));                                                                                                           \
        if (unlikely(NewTab == NULL)) return MemoryAllocationError;                                                                                                                                                     \
        HashSet->Table = NewTab;                                                                                                                                                                                        \
                                                                                                                                                                                                                        \
                                                                                                                                                                                                                        \
        /* Below, start traversing the old hash table and move its elements to the new hash table */                                                                                                                    \
        /* There are many tricks here */                                                                                                                                                                                \
        /* First, the hash table length is a power of 2, so the binary representation of hash table (length - 1) is all 1 */                                                                                            \
        /* For example, if the hash table length is 16 before expansion, then the binary representation of (16 - 1) is */                                                                                               \
        /* 0000 0000 0000 0000 0000 0000 0000 1111 */                                                                                                                                                                   \
        /* When calculating the index, simply AND the hash value with the hash table (length - 1), because this effectively takes the lower 4 bits of the hash value, which is the result of the mod operation */       \
        /* After expansion, if the hash table length is 32, then the binary representation of (32 - 1) is */                                                                                                            \
        /* 0000 0000 0000 0000 0000 0000 0001 1111 */                                                                                                                                                                   \
        /* This effectively takes the lower 5 bits of the hash value */                                                                                                                                                 \
        /* After expansion, if the lower 5th bit of the hash value is 0, it means the index remains unchanged; if it is 1, the index becomes the original index + the original hash table length */                     \
        /* To determine whether the lower 5th bit of the hash value is 0, simply AND the hash value with the hash table length. If it is 0, it means the lower 5th bit is 0; if not, it means the lower 5th bit is 1 */ \
                                                                                                                                                                                                                        \
        if (OldTab != NULL)                                                                    /* If the old hash table is not NULL */                                                                                  \
        {                                                                                                                                                                                                               \
            for (int i = 0; i < OldCapacity; ++i)                                              /* Traverse the old hash table */                                                                                        \
            {                                                                                                                                                                                                           \
                HashSetNode_##NAME* Temp;                                                                                                                                                                               \
                if ((Temp = OldTab[i]) != NULL)                                                /* If OldTab[i] is not NULL */                                                                                           \
                {                                                                                                                                                                                                       \
                    OldTab[i] = NULL;                                                          /* Set OldTab[i] to NULL */                                                                                              \
                    if (Temp->Next == NULL) NewTab[Temp->HashCode & (NewCapacity - 1)] = Temp; /* If there is only one element, rehash and assign */                                                                    \
                    else                                                                       /* Handle the linked list, using the tail insertion method */                                                            \
                    {                                                                                                                                                                                                   \
                        HashSetNode_##NAME* UnChangeHead = NULL;                               /* Head node of the linked list with unchanged index */                                                                  \
                        HashSetNode_##NAME* UnChangeTail = NULL;                               /* Tail node of the linked list with unchanged index */                                                                  \
                        HashSetNode_##NAME* ChangeHead   = NULL;                               /* Head node of the linked list with changed index */                                                                    \
                        HashSetNode_##NAME* ChangeTail   = NULL;                               /* Tail node of the linked list with changed index */                                                                    \
                        HashSetNode_##NAME* Next;                                                                                                                                                                       \
                        do {                                                                                                                                                                                            \
                            Next = Temp->Next;                                                 /* Save the next node */                                                                                                 \
                            if ((Temp->HashCode & OldCapacity) == 0)                           /* The extra bit is 0, meaning the array index remains unchanged */                                                      \
                            {                                                                                                                                                                                           \
                                /* If there is no tail node yet, set the current node as the head node */                                                                                                               \
                                if (UnChangeHead == NULL) UnChangeHead = Temp;                                                                                                                                          \
                                /* If there is already a tail node, link the current node to the tail node */                                                                                                           \
                                else UnChangeTail->Next = Temp; /* Link the linked list */                                                                                                                              \
                                UnChangeTail = Temp;            /* Update the tail node */                                                                                                                              \
                            }                                                                                                                                                                                           \
                            else                                                                                                                                                                                        \
                            {                                                                                                                                                                                           \
                                if (ChangeHead == NULL) ChangeHead = Temp;                                                                                                                                              \
                                else ChangeTail->Next = Temp;   /* Link the linked list */                                                                                                                              \
                                ChangeTail = Temp;              /* Update the tail node */                                                                                                                              \
                            }                                                                                                                                                                                           \
                        }                                                                                                                                                                                               \
                        while ((Temp = Next) != NULL); /* Continue the loop if the next node is not NULL */                                                                                                             \
                        if (UnChangeTail != NULL)                                                                                                                                                                       \
                        {                                                                                                                                                                                               \
                            UnChangeTail->Next = NULL;                                                                                                                                                                  \
                            NewTab[i]          = UnChangeHead;                                                                                                                                                          \
                        }                                                                                                                                                                                               \
                        if (ChangeTail != NULL)                                                                                                                                                                         \
                        {                                                                                                                                                                                               \
                            ChangeTail->Next        = NULL;                                                                                                                                                             \
                            NewTab[i + OldCapacity] = ChangeHead;                                                                                                                                                       \
                        }                                                                                                                                                                                               \
                    }                                                                                                                                                                                                   \
                }                                                                                                                                                                                                       \
            }                                                                                                                                                                                                           \
        }                                                                                                                                                                                                               \
        free(OldTab);                                                                                                                                                                                                   \
        return Success;                                                                                                                                                                                                 \
    }                                                                                                                                                                                                                   \
    SCOPE CODE HashSet_##NAME##_Add(HashSet_##NAME* const HashSet, const TYPE Key)                                                                                                                                      \
    {                                                                                                                                                                                                                   \
        HashSetNode_##NAME** Table;                                                                                                                                                                                     \
        HashSetNode_##NAME** LocatedNode;                                                                                                                                                                               \
        int                  Capacity;                                                                                                                                                                                  \
        int                  HashCode = HashSet_##NAME##_HashCode(Key);                                                                                                                                                 \
        if ((Table = HashSet->Table) == NULL || (Capacity = HashSet->Capacity) == 0)                                                                                                                                    \
        {                                                                                                                                                                                                               \
            if (unlikely(Success != HashSet_##NAME##_Resize(HashSet))) return MemoryAllocationError;                                                                                                                    \
            Table    = HashSet->Table;                                                                                                                                                                                  \
            Capacity = HashSet->Capacity;                                                                                                                                                                               \
        }                                                                                                                                                                                                               \
        LocatedNode = &Table[(Capacity - 1) & HashCode];                                                                                                                                                                \
        while (*LocatedNode)                                                                                                                                                                                            \
        {                                                                                                                                                                                                               \
            if ((*LocatedNode)->HashCode == HashCode && Equals_Function(Key, (*LocatedNode)->Key)) return KeyDuplicated;                                                                                                \
            LocatedNode = &(*LocatedNode)->Next;                                                                                                                                                                        \
        }                                                                                                                                                                                                               \
        if (Success != HashSet_##NAME##_MakeNode(HashCode, Key, NULL, LocatedNode)) return MemoryAllocationError;                                                                                                       \
        if (++HashSet->Size > HashSet->Threshold)                                                                                                                                                                       \
        {                                                                                                                                                                                                               \
            if (unlikely(Success != HashSet_##NAME##_Resize(HashSet))) return MemoryAllocationError;                                                                                                                    \
        }                                                                                                                                                                                                               \
        return Success;                                                                                                                                                                                                 \
    }                                                                                                                                                                                                                   \
    SCOPE bool HashSet_##NAME##_Delete(HashSet_##NAME* const HashSet, const TYPE Key)                                                                                                                                   \
    {                                                                                                                                                                                                                   \
        HashSetNode_##NAME** Table;                                                                                                                                                                                     \
        HashSetNode_##NAME** Prev;                                                                                                                                                                                      \
        HashSetNode_##NAME*  Curr;                                                                                                                                                                                      \
        int                  Capacity;                                                                                                                                                                                  \
        int                  HashCode = HashSet_##NAME##_HashCode(Key);                                                                                                                                                 \
        if ((Table = HashSet->Table) && (Capacity = HashSet->Capacity) != 0 && *(Prev = &Table[(Capacity - 1) & HashCode]))                                                                                             \
        {                                                                                                                                                                                                               \
            do {                                                                                                                                                                                                        \
                Curr = *Prev;                                                                                                                                                                                           \
                if (Curr->HashCode == HashCode && Equals_Function(Key, Curr->Key))                                                                                                                                      \
                {                                                                                                                                                                                                       \
                    *Prev = Curr->Next;                                                                                                                                                                                 \
                    --HashSet->Size;                                                                                                                                                                                    \
                    free(Curr);                                                                                                                                                                                         \
                    return true;                                                                                                                                                                                        \
                }                                                                                                                                                                                                       \
                else Prev = &Curr->Next;                                                                                                                                                                                \
            }                                                                                                                                                                                                           \
            while (*Prev);                                                                                                                                                                                              \
        }                                                                                                                                                                                                               \
        return false;                                                                                                                                                                                                   \
    }                                                                                                                                                                                                                   \
    SCOPE bool HashSet_##NAME##_Contains(const HashSet_##NAME* const HashSet, const TYPE Key)                                                                                                                           \
    {                                                                                                                                                                                                                   \
        HashSetNode_##NAME** Table;                                                                                                                                                                                     \
        HashSetNode_##NAME** LocatedNode;                                                                                                                                                                               \
        int                  HashCode;                                                                                                                                                                                  \
        int                  Capacity;                                                                                                                                                                                  \
        if ((Table = HashSet->Table) && (Capacity = HashSet->Capacity) > 0 && *(LocatedNode = &Table[(HashCode = HashSet_##NAME##_HashCode(Key)) & (Capacity - 1)]))                                                    \
        {                                                                                                                                                                                                               \
            do {                                                                                                                                                                                                        \
                if ((*LocatedNode)->HashCode == HashCode && Equals_Function(Key, (*LocatedNode)->Key)) return true;                                                                                                     \
            }                                                                                                                                                                                                           \
            while (*(LocatedNode = &(*LocatedNode)->Next));                                                                                                                                                             \
        }                                                                                                                                                                                                               \
        return false;                                                                                                                                                                                                   \
    }                                                                                                                                                                                                                   \
    SCOPE bool HashSet_##NAME##_IsEmpty(const HashSet_##NAME* const HashSet) { return HashSet->Size == 0; }                                                                                                             \
    SCOPE int  HashSet_##NAME##_GetSize(const HashSet_##NAME* const HashSet) { return HashSet->Size; }                                                                                                                  \
    SCOPE void HashSet_##NAME##_Clear(HashSet_##NAME* const HashSet)                                                                                                                                                    \
    {                                                                                                                                                                                                                   \
        HashSetNode_##NAME** Table;                                                                                                                                                                                     \
        HashSetNode_##NAME*  LocatedNode;                                                                                                                                                                               \
        if ((Table = HashSet->Table) && HashSet->Size > 0)                                                                                                                                                              \
        {                                                                                                                                                                                                               \
            HashSet->Size = 0;                                                                                                                                                                                          \
            for (int i = 0; i < HashSet->Capacity; ++i)                                                                                                                                                                 \
            {                                                                                                                                                                                                           \
                while (Table[i])                                                                                                                                                                                        \
                {                                                                                                                                                                                                       \
                    LocatedNode = Table[i];                                                                                                                                                                             \
                    Table[i]    = LocatedNode->Next;                                                                                                                                                                    \
                    free(LocatedNode);                                                                                                                                                                                  \
                }                                                                                                                                                                                                       \
            }                                                                                                                                                                                                           \
            memset(Table, 0, HashSet->Capacity);                                                                                                                                                                        \
        }                                                                                                                                                                                                               \
    }                                                                                                                                                                                                                   \
    SCOPE void HashSet_##NAME##_Destroy(HashSet_##NAME* const HashSet)                                                                                                                                                  \
    {                                                                                                                                                                                                                   \
        HashSet_##NAME##_Clear(HashSet);                                                                                                                                                                                \
        free(HashSet->Table);                                                                                                                                                                                           \
        HashSet->Table    = NULL;                                                                                                                                                                                       \
        HashSet->Capacity = 0;                                                                                                                                                                                          \
    }


#define HASHSET_DECLARE(NAME, TYPE) \
    HASHSET_TYPE(NAME, TYPE)        \
    HASHSET_PROTOTYPES(NAME, TYPE)

#define HASHSET_INIT2(NAME, TYPE, SCOPE, Hash_Function, Equals_Function) \
    HASHSET_TYPE(NAME, TYPE)                                             \
    HASHSET_IMPL(NAME, TYPE, SCOPE, Hash_Function, Equals_Function)

#define HASHSET_INIT(NAME, TYPE, Hash_Function, Equals_Function)     HASHSET_INIT2(NAME, TYPE, static LTT_inline LTT_unused, Hash_Function, Equals_Function)


/* ------------------------------------------------------------------------ BEGIN OF HASH FUNCTIONS ------------------------------------------------------------------------ */

/**
 * @brief Integer hash function
 * @param key The key to hash
 * @return The hash value
 */
#define LTT_Int_Hash_Function(key)                                   ((unsigned int)(key))

/**
 * @brief Integer equality function
 * @param key1 The first key
 * @param key2 The second key
 * @return true if the keys are equal, false otherwise
 */
#define LTT_Int_Equals_Function(key1, key2)                          ((key1) == (key2))

/**
 * @brief 64-bit integer hash function
 * @param key The key to hash
 * @return The hash value
 */
#define LTT_Int64_Hash_Function(key)                                 ((unsigned int)((key) ^ ((key) >> 32)))

/**
 * @brief 64-bit integer equality function
 * @param key1 The first key
 * @param key2 The second key
 * @return true if the keys are equal, false otherwise
 */
#define LTT_Int64_Equals_Function(key1, key2)                        ((key1) == (key2))

/**
 * @brief String hash function
 * @param key The key to hash
 * @return The hash value
 */
#define LTT_String_Hash_Function(key)                                String_Hash_Function(key, strlen(key))

/**
 * @brief String equality function
 * @param key1 The first key
 * @param key2 The second key
 * @return true if the keys are equal, false otherwise
 */
#define LTT_String_Equals_Function(key1, key2)                       (strcmp((const char*)(key1), (const char*)(key2)) == 0)

/**
 * @brief Double hash function
 * @param key The key to hash
 * @return The hash value
 */
#define LTT_Double_Hash_Function(key)                                ((unsigned int)(key))

/**
 * @brief Double equality function
 * @param key1 The first key
 * @param key2 The second key
 * @return true if the keys are equal, false otherwise
 */
#define LTT_Double_Equals_Function(key1, key2)                       ((key1) == (key2))

/**
 * @brief Pointer hash function
 * @param key The key to hash
 * @return The hash value
 */
#define LTT_Pointer_Hash_Function(key)                               ((unsigned int)(uintptr_t)(key))

/**
 * @brief Pointer equality function
 * @param key1 The first key
 * @param key2 The second key
 * @return true if the keys are equal, false otherwise
 */
#define LTT_Pointer_Equals_Function(key1, key2)                      ((key1) == (key2))

/**
 * @brief Custom hash function
 * @param key The key to hash
 * @return The hash value
 */
#define LTT_Custom_Hash_Function(key)                                String_Hash_Function((const char*)&key, sizeof(key))

/**
 * @brief Custom equality function
 * @param key1 The first key
 * @param key2 The second key
 * @return true if the keys are equal, false otherwise
 */
#define LTT_Custom_Equals_Function(key1, key2)                       (memcmp((const void*)(&key1), (const void*)(&key2), sizeof(key1)) == 0)

/* ------------------------------------------------------------------------ END OF HASH FUNCTIONS ------------------------------------------------------------------------ */


/**
 * @brief Get the HashSet_##NAME struct
 * @param NAME
 */
#define HashSet(NAME)                                                HashSet_##NAME

/**
 * @brief Initialize the HashSet_##NAME struct
 * @param NAME The name of the HashSet_##NAME struct
 * @param HashSet The HashSet_##NAME struct
 * @return CODE The result of the function
 */
#define HashSet_Init(NAME, HashSet)                                  HashSet_##NAME##_Init((HashSet))

/**
 * @brief Add a key to the HashSet_##NAME struct
 * @param NAME The name of the HashSet_##NAME struct
 * @param HashSet The HashSet_##NAME struct
 * @param Key The key to add
 * @return CODE The result of the function
 */
#define HashSet_Add(NAME, HashSet, Key)                              HashSet_##NAME##_Add((HashSet), (Key))

/**
 * @brief Delete a key from the HashSet_##NAME struct
 * @param NAME The name of the HashSet_##NAME struct
 * @param HashSet The HashSet_##NAME struct
 * @param Key The key to delete
 * @retval true The key was deleted
 * @retval false The key was not deleted
 */
#define HashSet_Delete(NAME, HashSet, Key)                           HashSet_##NAME##_Delete((HashSet), (Key))

/**
 * @brief Check if the HashSet_##NAME struct contains a key
 * @param NAME The name of the HashSet_##NAME struct
 * @param HashSet The HashSet_##NAME struct
 * @param Key The key to check
 * @retval true The key is in the HashSet_##NAME struct
 * @retval false The key is not in the HashSet_##NAME struct
 */
#define HashSet_Contains(NAME, HashSet, Key)                         HashSet_##NAME##_Contains((HashSet), (Key))

/**
 * @brief Check if the HashSet_##NAME struct is empty
 * @param NAME The name of the HashSet_##NAME struct
 * @param HashSet The HashSet_##NAME struct
 * @retval true The HashSet_##NAME struct is empty
 * @retval false The HashSet_##NAME struct is not empty
 */
#define HashSet_IsEmpty(HashSet)                                     ((HashSet)->Size == 0)

/**
 * @brief Get the size of the HashSet_##NAME struct
 * @param NAME The name of the HashSet_##NAME struct
 * @param HashSet The HashSet_##NAME struct
 * @return int The size of the HashSet_##NAME struct
 */
#define HashSet_GetSize(HashSet)                                     ((HashSet)->Size)

/**
 * @brief Clear the HashSet_##NAME struct, it will not free the memory
 * @param NAME The name of the HashSet_##NAME struct
 * @param HashSet The HashSet_##NAME struct
 */
#define HashSet_Clear(NAME, HashSet)                                 HashSet_##NAME##_Clear((HashSet))

/**
 * @brief Destroy the HashSet_##NAME struct, it will free the memory
 * @param NAME The name of the HashSet_##NAME struct
 * @param HashSet The HashSet_##NAME struct
 */
#define HashSet_Destroy(NAME, HashSet)                               HashSet_##NAME##_Destroy((HashSet))

// Function implementation
#define LTT_HASHSET_INT_INIT(NAME)                                   HASHSET_INIT(NAME, unsigned int, LTT_Int_Hash_Function, LTT_Int_Equals_Function)
#define LTT_HASHSET_INT64_INIT(NAME)                                 HASHSET_INIT(NAME, unsigned long, LTT_Int64_Hash_Function, LTT_Int64_Equals_Function)
#define LTT_HASHSET_STRING_INIT(NAME)                                HASHSET_INIT(NAME, char*, LTT_String_Hash_Function, LTT_String_Equals_Function)
#define LTT_HASHSET_DOUBLE_INIT(NAME)                                HASHSET_INIT(NAME, double, LTT_Double_Hash_Function, LTT_Double_Equals_Function)
#define LTT_HASHSET_POINTER_INIT(NAME)                               HASHSET_INIT(NAME, void*, LTT_Pointer_Hash_Function, LTT_Pointer_Equals_Function)
#define LTT_HASHSET_INIT(NAME, TYPE, Hash_Function, Equals_Function) HASHSET_INIT(NAME, TYPE, Hash_Function, Equals_Function)
#define LTT_HASHSET_INIT_DEFAULT(NAME, TYPE)                         HASHSET_INIT(NAME, TYPE, LTT_Custom_Hash_Function, LTT_Custom_Equals_Function)

// Function declaration
#define LTT_HASHSET_DECLARE(NAME, TYPE)                              HASHSET_DECLARE(NAME, TYPE)