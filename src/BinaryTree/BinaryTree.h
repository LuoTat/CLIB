#pragma once
#include "../BinaryTreeNode/BinaryTreeNode.h"

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct BinaryTree
    {
        BinaryTreeNode* Root;        //根节点
        size_t          DataSize;    //数据大小
    } BinaryTree;
#ifdef __cplusplus
}
#endif