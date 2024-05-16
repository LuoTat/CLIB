/*
    此哈希表的实现是基于链地址法的哈希表
    哈希表内存储的是键值对，键和值都是void*类型的指针
    故用户在使用此哈希表时，需要自己实现哈希函数，以及判断两个键和值是否相等的函数
    否则，可以使用默认的哈希函数和判断两个键是否相等的函数
    使用时不能放入临时变量的地址，因为临时变量会在函数结束时被销毁
*/

#pragma once
#include "Iterator.h"
#include "Predefined.h"

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct _HashMap HashMap;


    // 哈希表的初始化
    HashMap* LTT_HashMap_New(const size_t KeySize, const size_t ValueSize, const GetHashCode_Function GetHashCode, const Equals_Function Equals_Key, const Equals_Function Equals_Value);
    // 放入键值对，返回键对应的旧值
    void* LTT_HashMap_Put(HashMap* const HashMap, void* const Key, void* const Value);
    // 获取键对应的值
    void* LTT_HashMap_Get(const HashMap* const HashMap, const void* const Key);
    // 放入键值对，当且仅当键对应的值不存在，返回键对应的旧值
    void* LTT_HashMap_PutIfAbsent(HashMap* const HashMap, void* const Key, void* const Value);
    // 替换已有键对应的值，返回键对应的旧值
    void* LTT_HashMap_Replace(HashMap* const HashMap, void* const Key, void* const Value);
    //判断哈希表是否为空
    bool LTT_HashMap_IsEmpty(const HashMap* const HashMap);
    // 获取哈希表元素的个数
    int LTT_HashMap_GetSize(const HashMap* const HashMap);
    // 判断哈希表是否包含键
    bool LTT_HashMap_ContainsKey(const HashMap* const HashMap, const void* const Key);
    // 判断哈希表是否包含值
    bool LTT_HashMap_ContainsValue(const HashMap* const HashMap, const void* const Value);
    // 删除键对应的值，返回键对应的旧值
    void* LTT_HashMap_DeleteKey(HashMap* const HashMap, const void* const Key);
    // 删除键值对，返回键对应的旧值
    void* LTT_HashMap_DeleteKVPair(HashMap* const HashMap, const void* const Key, const void* const Value);
    // 清空哈希表
    void LTT_HashMap_Clear(HashMap* const HashMap);
    // 销毁哈希表
    void LTT_HashMap_Destroy(HashMap** HashMap);
    // 获取迭代器
    Iterator LTT_HashMap_GetIterator(const HashMap* const HashMap);
    // 获取迭代器当前元素
    void* LTT_HashMap_GetCurrent_Key(const Iterator* const IP);
    void* LTT_HashMap_GetCurrent_Value(const Iterator* const IP);
#ifdef __cplusplus
}
#endif