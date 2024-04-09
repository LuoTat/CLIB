#include "UnionFindSet.h"
#include "../Iterator/Iterator.h"
#include "../Predefined/Predefined.h"
#include "LTT_HashMap.h"

typedef struct _UnionFindSet
{
    HashMap*        Set;
    size_t          DataSize;
    Equals_Function Equals;
} _UnionFindSet;

const void* LTT_UnionFindSet_GetCurrent(const Iterator* const IP) { return LTT_HashMap_GetCurrent_Key(IP); }

Iterator LTT_UnionFindSet_GetIterator(const UnionFindSet* const UnionFindSet)
{
    if (LTT_HashMap_IsEmpty(UnionFindSet->Set))
    {
        printf("并查集为空!\n");
        return Iterator_NULL;
    }
    return LTT_HashMap_GetIterator(UnionFindSet->Set);
}

UnionFindSet* LTT_UnionFindSet_New(const size_t DataSize, Equals_Function Equals)
{
    UnionFindSet* UFSP = (UnionFindSet*)malloc(sizeof(UnionFindSet));
    if (UFSP == NULL)
    {
        printf("内存分配失败!\n");
        return NULL;
    }
    UFSP->DataSize = DataSize;
    //用户可自定义的函数
    UFSP->Equals   = Equals ? Equals : LTT_DEFAULT_Equals;
    UFSP->Set      = LTT_HashMap_New(DataSize, DataSize, NULL, UFSP->Equals, UFSP->Equals);
    return UFSP;
}

Status LTT_UnionFindSet_Put(UnionFindSet* const UFS, void* const x)
{
    //每一个元素初始化的时候，父节点都是自己
    LTT_HashMap_PutIfAbsent(UFS->Set, x, x);
    return OK;
}

void* LTT_UnionFindSet_Find(UnionFindSet* const UFS, void* const x)
{
    //使用路径压缩来优化
    void* x_Parent = LTT_HashMap_Get(UFS->Set, x);
    if (x == x_Parent || UFS->Equals(x, x_Parent, UFS->DataSize)) return x;
    else
    {
        //将当前x的父节点设置为父节点的代表元素
        LTT_HashMap_Put(UFS->Set, x, LTT_UnionFindSet_Find(UFS, x_Parent));
        return LTT_HashMap_Get(UFS->Set, x);
    }
}

Status LTT_UnionFindSet_Unite(UnionFindSet* const UFS, void* const x, void* const y)
{
    void* x_Representative_Elements = LTT_UnionFindSet_Find(UFS, x);
    void* y_Representative_Elements = LTT_UnionFindSet_Find(UFS, y);
    if (x_Representative_Elements == y_Representative_Elements || UFS->Equals(x_Representative_Elements, y_Representative_Elements, UFS->DataSize)) { return OK; }
    //这里的合并是将y的代表元素的父节点设置为x的代表元素x_Representative_Elements
    LTT_HashMap_Put(UFS->Set, y_Representative_Elements, x_Representative_Elements);
    return OK;
}

bool LTT_UnionFindSet_IsUnite(UnionFindSet* const UFS, void* const x, void* const y)
{
    void* x_Representative_Elements = LTT_UnionFindSet_Find(UFS, x);
    void* y_Representative_Elements = LTT_UnionFindSet_Find(UFS, y);
    return (x_Representative_Elements == y_Representative_Elements || UFS->Equals(x_Representative_Elements, y_Representative_Elements, UFS->DataSize));
}

// 并查集的清空
void LTT_UnionFindSet_Clear(UnionFindSet* const UFS) { LTT_HashMap_Clear(UFS->Set); }

//并查集的销毁
void LTT_UnionFindSet_Destroy(UnionFindSet** UFS)
{
    LTT_HashMap_Destroy(&(*UFS)->Set);
    free(*UFS);
    *UFS = NULL;
}