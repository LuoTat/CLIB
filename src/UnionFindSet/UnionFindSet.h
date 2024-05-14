#pragma once
#include "../Iterator/Iterator.h"
#include "../Predefined/Predefined.h"

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct _UnionFindSet UnionFindSet;

    
    //初始化并查集
    UnionFindSet* LTT_UnionFindSet_New(const size_t DataSize, Equals_Function Equals);
    //并查集的放入
    Status LTT_UnionFindSet_Put(UnionFindSet* const UFS, void* const x);
    //并查集的合并
    Status LTT_UnionFindSet_Unite(UnionFindSet* const UFS, void* const x, void* const y);
    //并查集的查找
    void* LTT_UnionFindSet_Find(UnionFindSet* const UFS, void* const x);
    //判断两个元素是否在同一个集合中
    bool LTT_UnionFindSet_IsUnite(UnionFindSet* const UFS, void* const x, void* const y);
    // 并查集的清空
    void LTT_UnionFindSet_Clear(UnionFindSet* const UFS);
    //并查集的销毁
    void LTT_UnionFindSet_Destroy(UnionFindSet** UFS);
    //获取迭代器
    Iterator LTT_UnionFindSet_GetIterator(const UnionFindSet* const UnionFindSet);
    // 获取迭代器当前元素
    const void* LTT_UnionFindSet_GetCurrent(const Iterator* const IP);
#ifdef __cplusplus
}
#endif