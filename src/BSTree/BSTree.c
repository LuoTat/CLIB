#include "BSTree.h"
#include <float.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "ArrayStack.h"
#include "BSTreeUtils.h"
#include "BinaryTree.h"
#include "_BinaryTree.h"

BSTree* LTT_BSTree_New(const size_t DataSize, const CompareFunction Comparator)
{
    BSTree*     BS_Tree = (BSTree*)malloc(sizeof(BSTree));
    BinaryTree* BiTree  = LTT_BiTree_New(DataSize);
    if (BS_Tree == NULL || BiTree == NULL)
    {
        printf("BSTree制作失败!\n");
        free(BS_Tree);
        return NULL;
    }
    BS_Tree->BiTree = *BiTree;
    free(BiTree);
    BS_Tree->Comparator = Comparator;
    return BS_Tree;
}

static Status LTT_BSTree_InsertNode(BSTree* BS_Tree, BinaryTreeNode* const Inserted_Node, const CompareFunction Comparator)
{
    // 双指针法，y指向x的父节点
    // x指向当前节点
    BinaryTreeNode* y     = NODE_NULL;
    BinaryTreeNode* x     = BS_Tree->BiTree.Root;
    int             Delta = 0;
    while (x != NODE_NULL)
    {
        y     = x;
        Delta = Comparator(Inserted_Node->Data, x->Data);
        if (Delta < 0) x = x->LeftChild;
        else if (Delta > 0) x = x->RightChild;
        else return ERROR;                                       // 如果相等，返回ERROR
    }
    Inserted_Node->Parent = y;
    if (y == NODE_NULL) BS_Tree->BiTree.Root = Inserted_Node;    //树为空
    else if (Comparator(Inserted_Node->Data, y->Data) < 0) y->LeftChild = Inserted_Node;
    else y->RightChild = Inserted_Node;
    return OK;
}

Status LTT_BSTree_Insert(BSTree* BS_Tree, void* const Data)
{
    BinaryTreeNode* BeInsertedNode = LTT_BiTree_MakeNode(Data);
    if (BeInsertedNode == NULL) return ERROR;
    if (LTT_BSTree_InsertNode(BS_Tree, BeInsertedNode, BS_Tree->Comparator) == ERROR)
    {
        free(BeInsertedNode);
        return ERROR;
    }
    return OK;
}

// 此函数用于将以V为根的子树替换为以U为根的子树
static void LTT_BSTree_Transplant(BSTree* const BS_Tree, BinaryTreeNode* const U, BinaryTreeNode* const V)
{
    if (U->Parent == NODE_NULL) BS_Tree->BiTree.Root = V;
    else if (U == U->Parent->LeftChild) U->Parent->LeftChild = V;
    else U->Parent->RightChild = V;
    if (V != NODE_NULL) V->Parent = U->Parent;
}

Status LTT_BSTree_Delete(BSTree* BS_Tree, void* const Data)
{
    BinaryTreeNode* FindNode = LTT_BSTreeUtils_SearchNode(BS_Tree->BiTree.Root, Data, BS_Tree->Comparator);
    if (FindNode == NULL) return ERROR;
    if (FindNode->LeftChild == NODE_NULL) LTT_BSTree_Transplant(BS_Tree, FindNode, FindNode->RightChild);
    else if (FindNode->RightChild == NODE_NULL) LTT_BSTree_Transplant(BS_Tree, FindNode, FindNode->LeftChild);
    else
    {
        BinaryTreeNode* Temp = LTT_BSTreeUtils_GetMinNode(FindNode->RightChild);
        if (Temp->Parent != FindNode)
        {
            LTT_BSTree_Transplant(BS_Tree, Temp, Temp->RightChild);
            Temp->RightChild         = FindNode->RightChild;
            Temp->RightChild->Parent = Temp;
        }
        LTT_BSTree_Transplant(BS_Tree, FindNode, Temp);
        Temp->LeftChild         = FindNode->LeftChild;
        Temp->LeftChild->Parent = Temp;
    }
    return OK;
}

BinaryTreeNode* LTT_BSTree_Search(const BSTree* const BS_Tree, const void* const Data) { return LTT_BSTreeUtils_SearchNode(BS_Tree->BiTree.Root, Data, BS_Tree->Comparator); }

void LTT_BSTree_Clear(BSTree* const BS_Tree) { LTT_BiTree_Clear(&BS_Tree->BiTree); }

void LTT_BSTree_Destroy(BSTree** BS_Tree)
{
    LTT_BSTree_Clear(*BS_Tree);
    free(*BS_Tree);
    *BS_Tree = NULL;
}

/*
    P是查找成功概率
    Q是查找失败概率
    N是节点个数
*/
void LTT_BSTree_Calculate_Optimal_BST_KNUTH(double** e, int** root, double* P, double* Q, int N)
{
    //使用动态规划算法构造最优二叉查找树
    //Data[1..N]是关键字，P[1..N]是查找概率，Q[0..N]是失败概率
    //构造三个二维数组e[1..N+1,0..N]和w[1..N+1,0..N]和root[1..N,1..N]
    //e[i,j]表示在包含关键字Data[i],Data[i+1],...,Data[j]的子树中进行一次查找的期望代价
    //w[i,j]表示在包含关键字Data[i],Data[i+1],...,Data[j]的子树成为另外一个节点的子树时增加的进行一次查找的期望代价
    //root[i,j]表示以i为根，j个节点的最优二叉查找树的根节点

    if (root == NULL)
    {
        root = (int**)malloc((N + 1) * sizeof(int*));
        for (int i = 0; i < N + 1; ++i) root[i] = (int*)calloc((N + 1), sizeof(int));
    }
    if (e == NULL)
    {
        e = (double**)malloc((N + 2) * sizeof(double*));
        for (int i = 0; i < N + 2; ++i) { e[i] = (double*)calloc((N + 1), sizeof(double)); }
    }

    double** w = (double**)malloc((N + 2) * sizeof(double*));
    for (int i = 0; i < N + 2; ++i) { w[i] = (double*)calloc((N + 1), sizeof(double)); }

    //初始化e和w
    for (int i = 1; i <= N + 1; ++i)
    {
        e[i][i - 1] = Q[i - 1];
        w[i][i - 1] = Q[i - 1];
    }
    //开始动态规划
    /*
        这里的i是控制迭代求解子树问题的长度,从0(e[i][i])到Length-1(e[1][N])
    */
    for (int i = 0; i <= N - 1; ++i)
    {
        /*
            这里的j是控制子树的起始位置，从1到N - i
            因为子树的长度是i+1，所以子树的起始位置最大是N - i
        */
        for (int j = 1; j <= N - i; ++j)
        {
            int Temp   = i + j;                                 //子树的终止位置
            e[j][Temp] = DBL_MAX;                               //初始化e[j][Temp]为无穷大
            w[j][Temp] = w[j][Temp - 1] + P[Temp] + Q[Temp];    //初始化w[j][Temp]

            // 使用性质root[i,j-1] <= root[i,j] <= root[i+1,j](1<=i<j<=N)来缩小搜索范围
            if (j < Temp)
            {
                for (int k = root[j][Temp - 1]; k <= root[j + 1][Temp]; ++k)
                {
                    double t = e[j][k - 1] + e[k + 1][Temp] + w[j][Temp];    //计算e[j][Temp]
                    if (t < e[j][Temp])                                      //如果t小于e[j][Temp]，更新e[j][Temp]和root[j][Temp]
                    {
                        e[j][Temp]    = t;
                        root[j][Temp] = k;
                    }
                }
            }
            else
            {
                e[j][Temp]    = e[j][Temp - 1] + e[j + 1][Temp] + w[j][Temp];    //计算e[j][Temp]
                root[j][Temp] = j;
            }
        }
    }

    if (root == NULL)
    {
        for (int i = 0; i < N + 1; ++i) free(root[i]);
        free(root);
    }
    if (e == NULL)
    {
        for (int i = 0; i < N + 2; ++i) { free(e[i]); }
        free(e);
    }
    for (int i = 0; i < N + 2; ++i) { free(w[i]); }
    free(w);
}

static BSTree* LTT_BSTree_Build_Optimal_BST_Kernel(void** Data, size_t DataSize, int** Root, int Length, CompareFunction Comparator)
{
    typedef struct
    {
        BinaryTreeNode* Node;
        bool            IsLeft;
        int             Left;
        int             Right;
    } NodeInfo;

    NodeInfo* NodeInfoArray = (NodeInfo*)malloc((Length * 2) * sizeof(NodeInfo));
    int       count         = 0;

    NodeInfo* Temp;
    NodeInfo* LeftInfo;
    NodeInfo* RightInfo;

    BinaryTreeNode* RootNode;
    int             RootIndex;

    ArrayStack* Stack = LTT_ArrayStack_New(sizeof(NodeInfo), NULL);

    RootIndex         = Root[1][Length];
    BSTree* BS_Tree   = LTT_BSTree_New(DataSize, Comparator);
    LTT_BSTree_Insert(BS_Tree, Data[RootIndex]);
    RootNode          = BS_Tree->BiTree.Root;

    LeftInfo          = &NodeInfoArray[count++];
    LeftInfo->Node    = RootNode;
    LeftInfo->Left    = 1;
    LeftInfo->Right   = RootIndex - 1;
    LeftInfo->IsLeft  = true;

    RightInfo         = &NodeInfoArray[count++];
    RightInfo->Node   = RootNode;
    RightInfo->Left   = RootIndex + 1;
    RightInfo->Right  = Length;
    RightInfo->IsLeft = false;

    if (RightInfo->Left <= RightInfo->Right) LTT_ArrayStack_Push(Stack, RightInfo);
    if (LeftInfo->Left <= LeftInfo->Right) LTT_ArrayStack_Push(Stack, LeftInfo);

    while (!LTT_ArrayStack_IsEmpty(Stack))
    {
        Temp      = LTT_ArrayStack_Pop(Stack);
        RootIndex = Root[Temp->Left][Temp->Right];

        RootNode  = LTT_BiTree_MakeNode(Data[RootIndex]);
        if (Temp->IsLeft == true) { Temp->Node->LeftChild = RootNode; }
        else { Temp->Node->RightChild = RootNode; }

        LeftInfo          = &NodeInfoArray[count++];
        LeftInfo->Node    = RootNode;
        LeftInfo->Left    = Temp->Left;
        LeftInfo->Right   = RootIndex - 1;
        LeftInfo->IsLeft  = true;

        RightInfo         = &NodeInfoArray[count++];
        RightInfo->Node   = RootNode;
        RightInfo->Left   = RootIndex + 1;
        RightInfo->Right  = Temp->Right;
        RightInfo->IsLeft = false;

        if (RightInfo->Left <= RightInfo->Right) LTT_ArrayStack_Push(Stack, RightInfo);
        if (LeftInfo->Left <= LeftInfo->Right) LTT_ArrayStack_Push(Stack, LeftInfo);
    }
    LTT_ArrayStack_Destroy(&Stack);
    free(NodeInfoArray);
    return BS_Tree;
}

BSTree* LTT_BSTree_Build_Optimal_BST(void** Data, size_t DataSize, double* P, double* Q, int Length, CompareFunction Comparator)
{
    int** Root = (int**)malloc((Length + 1) * sizeof(int*));
    for (int i = 0; i < Length + 1; ++i) Root[i] = (int*)calloc((Length + 1), sizeof(int));
    LTT_BSTree_Calculate_Optimal_BST_KNUTH(NULL, Root, P, Q, Length);
    BSTree* BS_Tree = LTT_BSTree_Build_Optimal_BST_Kernel(Data, DataSize, Root, Length, Comparator);
    for (int i = 0; i < Length + 1; ++i) free(Root[i]);
    free(Root);
    return BS_Tree;
}