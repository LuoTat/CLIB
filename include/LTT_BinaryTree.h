#pragma once
#include "../src/BinaryTree/BinaryTree.h"
#include "../src/Predefined/Predefined.h"

#ifdef __cplusplus
extern "C"
{
#endif
    //初始化二叉树
    BinaryTree* LTT_BiTree_New(void* const Data, const size_t DataSize);
    //制作二叉树节点
    BinaryTreeNode* LTT_BiTree_Make_Node(void* const Data, const size_t DataSize);
    //插入二叉树节点
    Status LTT_BiTree_Insert_Node(BinaryTreeNode* const BeInserted_Node, const BinaryTreeNode* const Inserted_Node, const bool LeftChild);
    Status LTT_BiTree_Insert_Node_byComparator(BinaryTreeNode* Root, BinaryTreeNode* InsertedNode, CompareFunction Comparator);
    Status LTT_BiTree_Insert_Data(BinaryTreeNode* Root, void* Data, size_t DataSize, CompareFunction Comparator);
    //获得子树的节点数
    int LTT_BiTree_Get_NodeNumber(const BinaryTreeNode* const Root);
    //获得子树的叶子节点数
    int LTT_BiTree_Get_LeafNumber(const BinaryTreeNode* const Root);
    //获得子树的深度
    int LTT_BiTree_Get_Depth(const BinaryTreeNode* const Root);
    //查询子树中的数据
    BinaryTreeNode* LTT_BiTree_Search_Data(BinaryTreeNode* Root, void* Data, CompareFunction Comparator);
    // 清空二叉子树
    void LTT_BiTree_Clear(BinaryTreeNode* Root);
    // 销毁二叉树
    void LTT_Bitree_Destroy(BinaryTree* BiTree);
    //前序遍历
    Status LTT_BiTree_PreOrder_Traverse_Recursive(BinaryTreeNode* BiTreeNode, VisitFunction Visit);
    Status LTT_BiTree_PreOrder_Traverse_Stack(BinaryTreeNode* BiTreeNode, VisitFunction Visit);
    //中序遍历
    Status LTT_BiTree_InOrder_Traverse_Recursive(BinaryTreeNode* BiTreeNode, VisitFunction Visit);
    Status LTT_BiTree_InOrder_Traverse_Stack(BinaryTreeNode* BiTreeNode, VisitFunction Visit);
    //后序遍历
    Status LTT_BiTree_PostOrder_Traverse_Recursive(BinaryTreeNode* BiTreeNode, VisitFunction Visit);
    Status LTT_BiTree_PostOrder_Traverse_Stack(BinaryTreeNode* BiTreeNode, VisitFunction Visit);
    //层序遍历
    Status LTT_BiTree_LevelOrder_Traverse_Queue(BinaryTreeNode* BiTreeNode, VisitFunction Visit);
    /*
        计算最优二叉搜索树的e[1..N+1,0..N]和root[1..N,1..N]
    */
    void LTT_BiTree_Calculate_Optimal_BST(double** e, int** root, double* P, double* Q, int N);
    //构造最优二叉搜索树
    BinaryTree* LTT_BiTree_Build_Optimal_BST(void** Data, size_t DataSize, double* P, double* Q, int Length);
#ifdef __cplusplus
}
#endif