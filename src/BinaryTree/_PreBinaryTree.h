#pragma once
#include <stddef.h>

#define NODE_NULL ((BinaryTreeNode*)(&Node_NULL))

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct BinaryTreeNode
    {
        void*                  Data;
        struct BinaryTreeNode* LeftChild;
        struct BinaryTreeNode* RightChild;
        struct BinaryTreeNode* Parent;
    } BinaryTreeNode;

    typedef struct BinaryTree
    {
        BinaryTreeNode* Root;        //根节点
        size_t          DataSize;    //数据大小
    } BinaryTree;

    extern BinaryTreeNode Node_NULL;
#ifdef __cplusplus
}
#endif