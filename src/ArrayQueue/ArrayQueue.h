#pragma once
#include "../Iterator/Iterator.h"
#include "../Predefined/Predefined.h"

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct _ArrayQueue ArrayQueue;

    
    // 初始化队列
    ArrayQueue* LTT_ArrayQueue_New(const size_t DataSize, Equals_Function Equals);
    // 插入元素
    Status LTT_ArrayQueue_Push(ArrayQueue* const ArrayQueue, void* const Data);
    // 弹出元素
    void* LTT_ArrayQueue_Pop(ArrayQueue* const ArrayQueue);
    // 查看队首元素
    void* LTT_ArrayQueue_Peek(const ArrayQueue* const ArrayQueue);
    // 是否包含特定元素
    bool LTT_ArrayQueue_Contains(const ArrayQueue* const ArrayQueue, void* const Data);
    // 判断是否为空
    bool LTT_ArrayQueue_IsEmpty(const ArrayQueue* const ArrayQueue);
    // 获得元素个数
    int LTT_ArrayQueue_GetSize(const ArrayQueue* const ArrayQueue);
    // 清空队列
    void LTT_ArrayQueue_Clear(ArrayQueue* const ArrayQueue);
    // 销毁队列
    void LTT_ArrayQueue_Destroy(ArrayQueue** ArrayQueue);
    // 获得迭代器
    Iterator LTT_ArrayQueue_GetIterator(const ArrayQueue* const ArrayQueue);
    // 获取迭代器当前元素
    void* LTT_ArrayQueue_GetCurrent(const Iterator* const IP);
#ifdef __cplusplus
}
#endif