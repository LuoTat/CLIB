#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include "../Predefined/Predefined.h"


#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct BinaryTreeNode
    {
        void*                  Data;
        size_t                 DataSize;
        struct BinaryTreeNode* LeftChild;
        struct BinaryTreeNode* RightChild;
    } BinaryTreeNode;

    //制作二叉树节点
    BinaryTreeNode* LTT_BiTreeNode_Make_Node(void* const Data, const size_t DataSize);
    //插入二叉树节点
    Status LTT_BiTreeNode_Insert_Node(BinaryTreeNode* const BeInserted_Node, BinaryTreeNode* const Inserted_Node, const bool LeftChild);
    Status LTT_BiTreeNode_Insert_Node_byComparator(BinaryTreeNode* const Root, BinaryTreeNode* const Inserted_Node, const CompareFunction Comparator);
    Status LTT_BiTreeNode_Insert_Data(BinaryTreeNode* const Root, void* const Data, const size_t DataSize, const CompareFunction Comparator);
    //获得子树的节点数
    int LTT_BiTreeNode_Get_NodeNumber( BinaryTreeNode* const Root);
    //获得子树的叶子节点数
    int LTT_BiTreeNode_Get_LeafNumber( BinaryTreeNode* const Root);
    //获得子树的深度
    int LTT_BiTreeNode_Get_Depth( BinaryTreeNode* const Root);
    //查询子树中的数据
    BinaryTreeNode* LTT_BiTreeNode_Search_Data( BinaryTreeNode* const Root, const void* const Data, const CompareFunction Comparator);
    //清空二叉子树
    void LTT_BiTreeNode_Clear(BinaryTreeNode* const Root);
    //前序遍历二叉子树
    Status LTT_BiTreeNode_PreOrder_Traverse_Recursive(BinaryTreeNode* const Root, const VisitFunction Visit);
    Status LTT_BiTreeNode_PreOrder_Traverse_Stack(BinaryTreeNode* const Root, const VisitFunction Visit);
    //中序遍历二叉子树
    Status LTT_BiTreeNode_InOrder_Traverse_Recursive(BinaryTreeNode* const Root, const VisitFunction Visit);
    Status LTT_BiTreeNode_InOrder_Traverse_Stack(BinaryTreeNode* const Root, const VisitFunction Visit);
    //后序遍历二叉子树
    Status LTT_BiTreeNode_PostOrder_Traverse_Recursive(BinaryTreeNode* const Root, const VisitFunction Visit);
    Status LTT_BiTreeNode_PostOrder_Traverse_Stack(BinaryTreeNode* const Root, const VisitFunction Visit);
    //层序遍历二叉子树
    Status LTT_BiTreeNode_LevelOrder_Traverse_Queue(BinaryTreeNode* const Root, const VisitFunction Visit);
#ifdef __cplusplus
}
#endif