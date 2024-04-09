#pragma once
#include "../src/BinaryTree/BinaryTree.h"
#include "../src/Predefined/Predefined.h"


#ifdef __cplusplus
extern "C"
{
#endif
    // 初始化二叉树
    BinaryTree* LTT_BiTree_New(const size_t DataSize);
    // 插入根节点
    Status LTT_BiTree_InsertRoot(BinaryTree* const BiTree, BinaryTreeNode* const Inserted_Node);
    // 插入节点
    Status LTT_BiTree_InsertNode(BinaryTreeNode* const BeInserted_Node, BinaryTreeNode* const Inserted_Node, const bool LeftChild);
    // 删除节点
    Status LTT_BiTree_DeleteNode(BinaryTreeNode** Deleted_Node);
    // 清空二叉树
    void LTT_BiTree_Clear(BinaryTree* const BiTree);
    // 销毁二叉树
    void LTT_BiTree_Destroy(BinaryTree** BiTree);
#ifdef __cplusplus
}
#endif