#pragma once
#include "../Predefined/Predefined.h"

#define NODE_NULL (&Node_NULL)

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct BinaryTreeNode
    {
        void*                  Data;
        struct BinaryTreeNode* LeftChild;
        struct BinaryTreeNode* RightChild;
    } BinaryTreeNode;

    extern BinaryTreeNode Node_NULL;

    // 制作二叉树节点
    BinaryTreeNode* LTT_BiTreeNode_MakeNode(void* const Data);
    // 删除二叉树节点
    void LTT_BiTreeNode_DeleteNode(BinaryTreeNode** Node);
    // 删除子二叉树
    void LTT_BiTreeNode_DeleteSubTree(BinaryTreeNode** Root);
    // 销毁二叉树节点
    void LTT_BiTreeNode_DestroyNode(BinaryTreeNode** Node);
    // 获得子树的节点数
    int LTT_BiTreeNode_GetNodeNumber(BinaryTreeNode* const Root);
    // 获得子树的叶子节点数
    int LTT_BiTreeNode_GetLeafNumber(BinaryTreeNode* const Root);
    // 获得子树的深度
    int LTT_BiTreeNode_GetDepth(BinaryTreeNode* const Root);
    // 前序遍历二叉子树
    Status LTT_BiTreeNode_PreOrder_Traverse_Recursive(BinaryTreeNode* const Root, const VisitFunction Visit);
    Status LTT_BiTreeNode_PreOrder_Traverse_Stack(BinaryTreeNode* const Root, const VisitFunction Visit);
    // 中序遍历二叉子树
    Status LTT_BiTreeNode_InOrder_Traverse_Recursive(BinaryTreeNode* const Root, const VisitFunction Visit);
    Status LTT_BiTreeNode_InOrder_Traverse_Stack(BinaryTreeNode* const Root, const VisitFunction Visit);
    // 后序遍历二叉子树
    Status LTT_BiTreeNode_PostOrder_Traverse_Recursive(BinaryTreeNode* const Root, const VisitFunction Visit);
    Status LTT_BiTreeNode_PostOrder_Traverse_Stack(BinaryTreeNode* const Root, const VisitFunction Visit);
    // 层序遍历二叉子树
    Status LTT_BiTreeNode_LevelOrder_Traverse_Queue(BinaryTreeNode* const Root, const VisitFunction Visit);
#ifdef __cplusplus
}
#endif