#pragma once
#include "../src/BiTreeNodeUtil/BiTreeNodeUtil.h"    // IWYU pragma: export
#include "../src/BinaryTree/BinaryTree.h"
#include "../src/Predefined/Predefined.h"

#ifdef __cplusplus
extern "C"
{
#endif
    // 初始化二叉树
    BinaryTree* LTT_BiTree_New(const size_t DataSize);
    // 插入节点
    Status LTT_BiTree_InsertData(BinaryTree* const BiTree, BinaryTreeNode* const BeInserted_Node, void* const Inserted_Data, const bool LeftChild);
    // 删除节点
    Status LTT_BiTree_DeleteNode(BinaryTree* const BiTree, BinaryTreeNode* const BeDeleted_Node);
    // 清空二叉树
    void LTT_BiTree_Clear(BinaryTree* const BiTree);
    // 销毁二叉树
    void LTT_BiTree_Destroy(BinaryTree** BiTree);
#ifdef __cplusplus
}
#endif