#pragma once
#include "BinaryTreeUtils.h"    // IWYU pragma: export
#include "Predefined.h"
#include "_PreBinaryTree.h"


#ifdef __cplusplus
extern "C"
{
#endif
    // 初始化二叉树
    BinaryTree* LTT_BiTree_New(const size_t DataSize);
    // 插入节点
    Status LTT_BiTree_Insert(BinaryTree* const BiTree, BinaryTreeNode* const BeInserted_Node, void* const Inserted_Data, const bool LeftChild);
    // 删除节点
    Status LTT_BiTree_Delete(BinaryTree* const BiTree, BinaryTreeNode* BeDeleted_Node);
    // 清空二叉树
    void LTT_BiTree_Clear(BinaryTree* const BiTree);
    // 销毁节点
    void LTT_BiTree_DestroyNode(BinaryTreeNode** Node);
    // 销毁二叉树
    void LTT_BiTree_Destroy(BinaryTree** BiTree);
#ifdef __cplusplus
}
#endif