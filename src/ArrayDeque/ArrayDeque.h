#pragma once
#include "../Iterator/Iterator.h"
#include "../Predefined/Predefined.h"

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct _ArrayDeque ArrayDeque;


    // 初始化队列
    ArrayDeque* LTT_ArrayDeque_New(const size_t DataSize, Equals_Function Equals);
    // 头部添加元素
    Status LTT_ArrayDeque_AddFirst(ArrayDeque* const ArrayDeque, void* const Data);
    // 头部弹出元素
    void* LTT_ArrayDeque_DeleteFirst(ArrayDeque* const ArrayDeque);
    // 尾部添加元素
    Status LTT_ArrayDeque_AddLast(ArrayDeque* const ArrayDeque, void* const Data);
    // 尾部弹出元素
    void* LTT_ArrayDeque_DeleteLast(ArrayDeque* const ArrayDeque);
    // 获取头部元素
    void* LTT_ArrayDeque_GetFirst(const ArrayDeque* const ArrayDeque);
    // 获取尾部元素
    void* LTT_ArrayDeque_GetLast(const ArrayDeque* const ArrayDeque);
    // 是否包含特定元素
    bool LTT_ArrayDeque_Contains(const ArrayDeque* const ArrayDeque, void* const Data);
    // 判断是否为空
    bool LTT_ArrayDeque_IsEmpty(const ArrayDeque* const ArrayDeque);
    // 获得元素个数
    int LTT_ArrayDeque_GetSize(const ArrayDeque* const ArrayDeque);
    // 清空队列
    void LTT_ArrayDeque_Clear(ArrayDeque* const ArrayDeque);
    // 销毁队列
    void LTT_ArrayDeque_Destroy(ArrayDeque** ArrayDeque);
    // 获取迭代器
    Iterator LTT_ArrayDeque_GetIterator(const ArrayDeque* const ArrayDeque);
    // 获取迭代器当前元素
    void* LTT_ArrayDeque_GetCurrent(const Iterator* const IP);
#ifdef __cplusplus
}
#endif