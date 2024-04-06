#pragma once
#include "../src/BinaryTree/BinaryTree.h"
#include "../src/BinaryTreeNode/BinaryTreeNode.h"    // IWYU pragma: export
#include "../src/Predefined/Predefined.h"


#ifdef __cplusplus
extern "C"
{
#endif
    //初始化二叉树
    BinaryTree* LTT_BiTree_New(void* const Data, const size_t DataSize);
    // 销毁二叉树
    void LTT_Bitree_Destroy(BinaryTree* BiTree);
    /*
        计算最优二叉搜索树的e[1..N+1,0..N]和root[1..N,1..N]
    */
    void LTT_BiTree_Calculate_Optimal_BST(double** e, int** root, double* P, double* Q, int N);
    //构造最优二叉搜索树
    BinaryTree* LTT_BiTree_Build_Optimal_BST(void** Data, size_t DataSize, double* P, double* Q, int Length);
#ifdef __cplusplus
}
#endif