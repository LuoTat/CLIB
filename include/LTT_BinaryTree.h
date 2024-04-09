#pragma once
#include "../src/BinaryTree/BinaryTree.h"
#include "../src/Predefined/Predefined.h"


#ifdef __cplusplus
extern "C"
{
#endif
    // 初始化二叉树
    BinaryTree* LTT_BiTree_New(const size_t DataSize);
    Status      LTT_BiTree_Insert_Root(BinaryTree* const BiTree, BinaryTreeNode* const Inserted_Node);
    Status      LTT_BiTree_Insert_Node(BinaryTreeNode* const BeInserted_Node, BinaryTreeNode* const Inserted_Node, const bool LeftChild);
    // 清空二叉树
    void LTT_BiTree_Clear(BinaryTree* const BiTree);
    // 销毁二叉树
    void LTT_BiTree_Destroy(BinaryTree** BiTree);
#ifdef __cplusplus
}
#endif