#pragma once
#include <stdlib.h>


//基类为BinaryTreeNode
//函数有初始化？
//makeBiTreeNode
//BiTreeNodeLeafNumber
//BiTreeNodeDepth
//几个遍历
//前继与后继
//插入与删除
//查找

#ifdef __cplusplus
extern "C"
{
#endif
    // Shape 的虚函数表
    typedef struct BinaryTree_VTL
    {
    } BinaryTree_VTL;

    typedef struct BinaryTreeNode
    {
        void*                  Data;
        size_t                 DataSize;
        struct BinaryTreeNode* LeftChild;
        struct BinaryTreeNode* RightChild;
    } BinaryTreeNode;

    typedef struct BinaryTree
    {
        struct BinaryTree_VTL* P_VTL;    //虚函数表
        struct BinaryTreeNode* Root;     //根节点
    } BinaryTree;
#ifdef __cplusplus
}
#endif