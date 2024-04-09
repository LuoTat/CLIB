#include "BSTree.h"
#include <float.h>
#include "../../include/LTT_ArrayStack.h"
#include "../../include/LTT_BinaryTree.h"

BSTree* LTT_BSTree_New(const size_t DataSize, const CompareFunction Comparator)
{
    BSTree* BS_Tree = (BSTree*)malloc(sizeof(BSTree));
    if (BS_Tree == NULL)
    {
        printf("BSTree制作失败!\n");
        return NULL;
    }
    BinaryTree* BiTree = LTT_BiTree_New(DataSize);
    if (BiTree == NULL)
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

Status LTT_BSTree_Insert_Data(BSTree* BS_Tree, void* const Data)
{
    BinaryTreeNode* BeInsertedNode = LTT_BiTreeNode_Make_Node(Data);
    if (BeInsertedNode == NULL) return ERROR;
    if (BS_Tree->BiTree.Root == NODE_NULL)
    {
        BS_Tree->BiTree.Root = BeInsertedNode;
        return OK;
    }
    BinaryTreeNode* Iterator = BS_Tree->BiTree.Root;
    while (true)
    {
        int Delta = BS_Tree->Comparator(BeInsertedNode->Data, Iterator->Data);
        if (Delta == 0)
        {
            free(BeInsertedNode);
            return ERROR;
        }
        else if (Delta < 0)
        {
            if (Iterator->LeftChild != NODE_NULL) Iterator = Iterator->LeftChild;
            else
            {
                Iterator->LeftChild = BeInsertedNode;
                return OK;
            }
        }
        else
        {
            if (Iterator->RightChild != NODE_NULL) Iterator = Iterator->RightChild;
            else
            {
                Iterator->RightChild = BeInsertedNode;
                return OK;
            }
        }
    }
}

Status LTT_BSTree_Delete_Data(BSTree* BS_Tree, void* const Data);

BinaryTreeNode* LTT_BSTreeNode_Search_Data(const BSTree* const BS_Tree, const void* const Data)
{
    BinaryTreeNode* Iterator = BS_Tree->BiTree.Root;
    if (Iterator == NODE_NULL) return NULL;
    while (Iterator != NODE_NULL)
    {
        int Delta = BS_Tree->Comparator(Data, Iterator->Data);
        if (Delta == 0) return Iterator;
        else if (Delta < 0) Iterator = Iterator->LeftChild;
        else Iterator = Iterator->RightChild;
    }
    return NULL;
}

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
void LTT_BSTree_Calculate_Optimal_BST(double** e, int** root, double* P, double* Q, int N)
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
            int Temp   = i + j;                                          //子树的终止位置
            e[j][Temp] = DBL_MAX;                                        //初始化e[j][Temp]为无穷大
            w[j][Temp] = w[j][Temp - 1] + P[Temp] + Q[Temp];             //初始化w[j][Temp]
            for (int k = j; k <= Temp; ++k)
            {
                double t = e[j][k - 1] + e[k + 1][Temp] + w[j][Temp];    //计算e[j][Temp]
                if (t < e[j][Temp])                                      //如果t小于e[j][Temp]，更新e[j][Temp]和root[j][Temp]
                {
                    e[j][Temp]    = t;
                    root[j][Temp] = k;
                }
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
    LTT_BSTree_Insert_Data(BS_Tree, Data[RootIndex]);
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

        RootNode  = LTT_BiTreeNode_Make_Node(Data[RootIndex]);
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
    LTT_BSTree_Calculate_Optimal_BST(NULL, Root, P, Q, Length);
    BSTree* BS_Tree = LTT_BSTree_Build_Optimal_BST_Kernel(Data, DataSize, Root, Length, Comparator);
    for (int i = 0; i < Length + 1; ++i) free(Root[i]);
    free(Root);
    return BS_Tree;
}