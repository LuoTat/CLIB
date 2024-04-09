#pragma once
#include "../BinaryTree/BinaryTree.h"

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct BSTree
    {
        BinaryTree      BiTree;        // 二叉树
        CompareFunction Comparator;    // 比较函数
    } BSTree;
#ifdef __cplusplus
}
#endif