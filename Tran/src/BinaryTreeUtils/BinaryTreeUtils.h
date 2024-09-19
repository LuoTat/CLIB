/*
    这里是一些供二叉树使用的一些工具
*/

#pragma once
#include "Predefined.h"
#include "_PreBinaryTree.h"


#ifdef __cplusplus
extern "C"
{
#endif
    // 删除子二叉树
    void LTT_BiTreeUtils_DeleteSubTree(BinaryTreeNode** Root);
    // 获得子树的节点数
    int LTT_BiTreeUtils_GetNodeNumber(BinaryTreeNode* const Root);
    // 获得子树的叶子节点数
    int LTT_BiTreeUtils_GetLeafNumber(BinaryTreeNode* const Root);
    // 获得子树的深度
    int LTT_BiTreeUtils_GetDepth(BinaryTreeNode* const Root);
    // 前序遍历二叉子树
    Status LTT_BiTreeUtils_PreOrder_Traverse_Recursive(BinaryTreeNode* const Root, const VisitFunction Visit);
    Status LTT_BiTreeUtils_PreOrder_Traverse_Stack(BinaryTreeNode* const Root, const VisitFunction Visit);
    // 中序遍历二叉子树
    Status LTT_BiTreeUtils_InOrder_Traverse_Recursive(BinaryTreeNode* const Root, const VisitFunction Visit);
    Status LTT_BiTreeUtils_InOrder_Traverse_Stack(BinaryTreeNode* const Root, const VisitFunction Visit);
    // 后序遍历二叉子树
    Status LTT_BiTreeUtils_PostOrder_Traverse_Recursive(BinaryTreeNode* const Root, const VisitFunction Visit);
    Status LTT_BiTreeUtils_PostOrder_Traverse_Stack(BinaryTreeNode* const Root, const VisitFunction Visit);
    // 层序遍历二叉子树
    Status LTT_BiTreeUtils_LevelOrder_Traverse_Queue(BinaryTreeNode* const Root, const VisitFunction Visit);
#ifdef __cplusplus
}
#endif