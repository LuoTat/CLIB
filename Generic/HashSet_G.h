#pragma once
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
#define DEFAULT_HASHSET_LOAD_FACTOR      0.75f

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


#define HASHSET_TYPE(NAME, TYPE)         \
    typedef struct HashSetNode_##NAME    \
    {                                    \
        TYPE                       Key;  \
        int                        Hash; \
        struct HashSetNode_##NAME* Next; \
    } HashSetNode_##NAME;                \
    typedef struct HashSet_##NAME        \
    {                                    \
        int                  Size;       \
        int                  Capacity;   \
        int                  Threshold;  \
        float                LoadFactor; \
        HashSetNode_##NAME** Table;      \
    } HashSet_##NAME

#define HASHSET_IMPL(NAME, TYPE, SCOPE, Hash_Function, Equals_Function)


/*! @function
  @abstract     Integer hash function
 */
#define LTT_Int_Hash_Function(key)                   ((unsigned int)(key))

/*! @function
  @abstract     Integer equality function
 */
#define LTT_Int_Equals_Function(key1, key2)          ((key1) == (key2))

/*! @function
  @abstract     64-bit integer hash function
 */
#define LTT_Int64_Hash_Function(key)                 ((unsigned int)((key) ^ ((key) >> 32)))

/*! @function
  @abstract     64-bit integer equality function
 */
#define LTT_Int64_Equals_Function(key1, key2)        ((key1) == (key2))

/*! @function
  @abstract     String hash function
 */
#define LTT_String_Hash_Function(key)                String_Hash_Function(key, strlen(key))

/*! @function
  @abstract     String equality function
 */
#define LTT_String_Equals_Function(key1, key2)       (strcmp((key1), (key2)) == 0)

/*! @function
  @abstract     Double hash function
 */
#define LTT_Double_Hash_Function(key)                ((unsigned int)(key))

/*! @function
  @abstract     Double equality function
 */
#define LTT_Double_Equals_Function(key1, key2)       ((key1) == (key2))

/*! @function
  @abstract     Pointer hash function
 */
#define LTT_Pointer_Hash_Function(key)               ((unsigned int)(uintptr_t)(key))

/*! @function
  @abstract     Pointer equality function
 */
#define LTT_Pointer_Equals_Function(key1, key2)      ((key1) == (key2))

/*! @function
  @abstract     Custom hash function
 */
#define LTT_Custom_Hash_Function(TYPE, key)          String_Hash_Function(key, sizeof(TYPE))

/*! @function
  @abstract     Custom equality function
 */
#define LTT_Custom_Equals_Function(TYPE, key1, key2) (memcmp((key1), (key2), sizeof(TYPE)) == 0)




#define HASHSET_INIT2(NAME, TYPE, SCOPE, Hash_Function, Equals_Function) \
    HASHSET_TYPE(NAME, TYPE)                                             \
    HASHSET_IMPL(NAME, TYPE, SCOPE, Hash_Function, Equals_Function)

#define HASHSET_INIT(NAME, TYPE, Hash_Function, Equals_Function)     HASHSET_INIT2(NAME, TYPE, static LTT_inline LTT_unused, Hash_Function, Equals_Function)

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
#define LTT_HASHSET_INIT_DEFAULT(NAME, TYPE)                         HASHSET_INIT(NAME, TYPE, VALUE_TYPE, LTT_Custom_Hash_Function, LTT_Custom_Equals_Function)