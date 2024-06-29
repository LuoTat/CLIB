#pragma once
#include "Iterator.h"
#include "Predefined.h"

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct _ArrayList ArrayList;

    
    // 初始化数组，采用赖加载，只有在第一次使用的时候才会初始化
    ArrayList* LTT_ArrayList_New(const size_t DataSize, Equals_Function Equals);
    // 尾部插入元素
    Status LTT_ArrayList_AddLast(ArrayList* const ArrayList, void* const Data);
    // 指定位置插入元素
    Status LTT_ArrayList_AddIndex(ArrayList* const ArrayList, const int Index, void* const Data);
    // 指定位置改变元素
    void* LTT_ArrayList_SetIndex(ArrayList* const ArrayList, const int Index, void* const Data);
    // 指定位置删除元素
    void* LTT_ArrayList_DeleteIndex(ArrayList* const ArrayList, const int Index);
    // 指定元素，删除第一个出现的位置
    bool LTT_ArrayList_DeleteData(ArrayList* const ArrayList, const void* const Data);
    // 获得指定位置元素
    void* LTT_ArrayList_GetData(const ArrayList* const ArrayList, const int Index);
    // 获得指定元素的最低位置
    int LTT_ArrayList_IndexOf(const ArrayList* const ArrayList, const void* const Data);
    // 获得指定元素的最高位置
    int LTT_ArrayList_LastIndexOf(const ArrayList* const ArrayList, const void* const Data);
    // 是否包含特定元素
    bool LTT_ArrayList_Contains(const ArrayList* const ArrayList, const void* const Data);
    // 判断是否为空
    bool LTT_ArrayList_IsEmpty(const ArrayList* const ArrayList);
    // 获得元素个数
    int LTT_ArrayList_GetSize(const ArrayList* const ArrayList);
    // 清空数组
    void LTT_ArrayList_Clear(ArrayList* const ArrayList);
    // 销毁数组
    void LTT_ArrayList_Destroy(ArrayList** ArrayList);
    // 获取迭代器
    Iterator LTT_ArrayList_GetIterator(const ArrayList* const ArrayList);
    // 获取迭代器当前元素
    void* LTT_ArrayList_GetCurrent(const Iterator* const IP);
#ifdef __cplusplus
}
#endif