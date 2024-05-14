/*
这里是一些供二叉搜索树使用的一些工具
*/

#pragma once
#include "../BinaryTree/_PreBinaryTree.h"
#include "../Predefined/Predefined.h"

#ifdef __cplusplus
extern "C"
{
#endif
    // 查询节点
    BinaryTreeNode* LTT_BSTreeNode_SearchNode(BinaryTreeNode* Root, const void* const Data, const CompareFunction Comparator);
    // 获得子树最大节点
    BinaryTreeNode* LTT_BSTreeNode_GetMaxNode(BinaryTreeNode* Root);
    // 获得子树最小节点
    BinaryTreeNode* LTT_BSTreeNode_GetMinNode(BinaryTreeNode* Root);
    // 获得节点的前驱节点
    BinaryTreeNode* LTT_BSTreeNode_GetPredecessorNode(BinaryTreeNode* Root);
    // 获得节点的后继节点
    BinaryTreeNode* LTT_BSTreeNode_GetSuccessorNode(BinaryTreeNode* Root);
#ifdef __cplusplus
}
#endif
