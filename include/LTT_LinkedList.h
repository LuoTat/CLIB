#pragma once
#include "../src/Iterator/Iterator.h"
#include "../src/LinkedList/LinkedList.h"
#include "../src/Predefined/Predefined.h"


#ifdef __cplusplus
extern "C"
{
#endif
    // 初始化链表
    LinkedList* LTT_LinkedList_New(const size_t DataSize, Equals_Function Equals);
    // 头部添加元素
    Status LTT_LinkedList_AddFirst(LinkedList* const LinkedList, void* const Data);
    // 头部删除元素
    void* LTT_LinkedList_DeleteFirst(LinkedList* const LinkedList);
    // 尾部添加元素
    Status LTT_LinkedList_AddLast(LinkedList* const LinkedList, void* const Data);
    // 尾部删除元素
    void* LTT_LinkedList_DeleteLast(LinkedList* const LinkedList);
    // 获取头部元素
    void* LTT_LinkedList_GetFirst(const LinkedList* const LinkedList);
    // 获取尾部元素
    void* LTT_LinkedList_GetLast(const LinkedList* const LinkedList);
    // 获得指定位置元素
    void* LTT_LinkedList_GetData(const LinkedList* const LinkedList, const int Index);
    // 获得指定元素的最低位置
    int LTT_LinkedList_IndexOf(const LinkedList* const LinkedList, const void* const Data);
    // 获得指定元素的最高位置
    int LTT_LinkedList_LastIndexOf(const LinkedList* const LinkedList, const void* const Data);
    // 是否包含特定元素
    bool LTT_LinkedList_Contains(const LinkedList* const LinkedList, void* const Data);
    // 判断是否为空
    bool LTT_LinkedList_IsEmpty(const LinkedList* const LinkedList);
    // 获得元素个数
    int LTT_LinkedList_GetSize(const LinkedList* const LinkedList);
    // 清空链表
    void LTT_LinkedList_Clear(LinkedList* const LinkedList);
    // 销毁链表
    void LTT_LinkedList_Destroy(LinkedList** LinkedList);
    // 获取迭代器
    Iterator LTT_LinkedList_GetIterator(const LinkedList* const LinkedList);
    // 获取迭代器当前元素
    void* LTT_LinkedList_GetCurrent(const Iterator* const IP);
#ifdef __cplusplus
}
#endif