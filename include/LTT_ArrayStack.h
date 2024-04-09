#pragma once
#include "../src/ArrayStack/ArrayStack.h"
#include "../src/Iterator/Iterator.h"
#include "../src/Predefined/Predefined.h"

#ifdef __cplusplus
extern "C"
{
#endif
    // 初始化队列
    ArrayStack* LTT_ArrayStack_New(const size_t DataSize, Equals_Function Equals);
    // 插入元素
    Status LTT_ArrayStack_Push(ArrayStack* const ArrayStack, void* const Data);
    // 弹出元素
    void* LTT_ArrayStack_Pop(ArrayStack* const ArrayStack);
    // 查看队首元素
    void* LTT_ArrayStack_Peek(const ArrayStack* const ArrayStack);
    // 是否包含特定元素
    bool LTT_ArrayStack_Contains(const ArrayStack* const ArrayStack, void* const Data);
    // 判断是否为空
    bool LTT_ArrayStack_IsEmpty(const ArrayStack* const ArrayStack);
    // 获得元素个数
    int LTT_ArrayStack_GetSize(const ArrayStack* const ArrayStack);
    // 清空队列
    void LTT_ArrayStack_Clear(ArrayStack* const ArrayStack);
    // 销毁队列
    void LTT_ArrayStack_Destroy(ArrayStack** ArrayStack);
    // 获得迭代器
    Iterator LTT_ArrayStack_GetIterator(const ArrayStack* const ArrayStack);
    // 获取迭代器当前元素
    void* LTT_ArrayStack_GetCurrent(const Iterator* const IP);
#ifdef __cplusplus
}
#endif