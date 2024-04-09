#pragma once
#include "../src/BSTree/BSTree.h"
#include "../src/Predefined/Predefined.h"

#ifdef __cplusplus
extern "C"
{
#endif
    // 初始化二叉搜索树
    BSTree* LTT_BSTree_New(const size_t DataSize, const CompareFunction Comparator);
    // 插入数据
    Status LTT_BSTree_InsertData(BSTree* BS_Tree, void* const Data);
    // 删除数据
    Status LTT_BSTree_DeleteData(BSTree* BS_Tree, void* const Data);
    // 查询二叉搜索树中的数据
    BinaryTreeNode* LTT_BSTreeNode_SearchData(const BSTree* const BS_Tree, const void* const Data);
    // 清空二叉搜索树
    void LTT_BSTree_Clear(BSTree* const BS_Tree);
    // 销毁二叉搜索树
    void LTT_BSTree_Destroy(BSTree** BS_Tree);
    /*
        计算最优二叉搜索树的e[1..N+1,0..N]和root[1..N,1..N]
    */
    void LTT_BSTree_Calculate_Optimal_BST(double** e, int** root, double* P, double* Q, int N);
    // 构造最优二叉搜索树
    BSTree* LTT_BSTree_Build_Optimal_BST(void** Data, size_t DataSize, double* P, double* Q, int Length);
#ifdef __cplusplus
}
#endif