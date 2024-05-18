#pragma once
#include "BSTree.h"

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct BSTree AVLTree;


    // 初始化AVL树
    AVLTree* LTT_AVLTree_New(const size_t DataSize, const CompareFunction Comparator);
    // 插入数据
    Status LTT_AVLTree_InsertData(AVLTree* AVL_Tree, void* const Data);
    // 删除数据
    Status LTT_AVLTree_DeleteData(AVLTree* AVL_Tree, void* const Data);
    // 清空AVL树
    void LTT_AVLTree_Clear(AVLTree* const AVL_Tree);
    // 销毁AVL树
    void LTT_AVLTree_Destroy(AVLTree** AVL_Tree);
#ifdef __cplusplus
}
#endif