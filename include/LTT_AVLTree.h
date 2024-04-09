#pragma once
#include "../src/AVLTree/AVLTree.h"
#include "../src/AVLTreeNode/AVLTreeNode.h"    // IWYU pragma: export
#include "../src/Predefined/Predefined.h"


#ifdef __cplusplus
extern "C"
{
#endif
    //初始化二叉树
    AVLTree* LTT_AVLTree_New(const size_t DataSize, const CompareFunction Comparator);
    Status   LTT_AVLTree_Insert_Data(AVLTree* AVLTree, void* const Data);
    // 销毁二叉树
    void LTT_AVLTree_Destroy(AVLTree* AVLTree);
#ifdef __cplusplus
}
#endif