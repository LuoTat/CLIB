#include "BinaryTree.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Predefined/Predefined.h"
#include "LTT_ArrayQueue.h"
#include "LTT_ArrayStack.h"


static BinaryTreeNode Node_NULL = {NULL, 0, NULL, NULL};
#define NODE_NULL (&Node_NULL)

//重载函数
BinaryTreeNode* LTT_BiTree_Make_Node(void* const Data, const size_t DataSize)
{
    BinaryTreeNode* RootNode = (BinaryTreeNode*)malloc(sizeof(BinaryTreeNode));
    if (RootNode == NULL)
    {
        printf("根节点制作失败!\n");
        return NULL;
    }
    RootNode->Data       = Data;
    RootNode->DataSize   = DataSize;
    RootNode->LeftChild  = NODE_NULL;
    RootNode->RightChild = NODE_NULL;
    return RootNode;
}

//重载函数
BinaryTree* LTT_BiTree_New(void* const Data, const size_t DataSize)
{
    BinaryTree* BiTree = (BinaryTree*)malloc(sizeof(BinaryTree));
    if (BiTree == NULL)
    {
        printf("BiTree制作失败!\n");
        return NULL;
    }

    //初始化虚函数表

    //初始化根节点

    BiTree->Root = LTT_BiTree_Make_Node(Data, DataSize);
    return BiTree;
}

Status LTT_BiTree_Insert_Node(BinaryTreeNode* BeInserted_Node, BinaryTreeNode* Inserted_Node, bool LeftChild)
{
    //把Inserted_Node插入到BeInserted_Node的左子树或者右子树
    if (LeftChild)
    {
        if (BeInserted_Node->LeftChild != NODE_NULL) return ERROR;     //如果左子树不为空，返回ERROR
        BeInserted_Node->LeftChild = Inserted_Node;
        return OK;
    }
    else
    {
        if (BeInserted_Node->RightChild != NODE_NULL) return ERROR;    //如果右子树不为空，返回ERROR
        BeInserted_Node->RightChild = Inserted_Node;
        return OK;
    }
}

Status LTT_BiTree_Insert_Node_byComparator(BinaryTreeNode* Root, BinaryTreeNode* InsertedNode, CompareFunction Comparator)
{
    BinaryTreeNode* Iterator = Root;
    if (Iterator == NODE_NULL)
    {
        printf("根节点为空!\n");
        return ERROR;
    }
    while (true)
    {
        int Delta = Comparator(InsertedNode->Data, Iterator->Data);
        if (Delta == 0) { return ERROR; }
        else if (Delta < 0)
        {
            if (Iterator->LeftChild != NODE_NULL) Iterator = Iterator->LeftChild;
            else
            {
                Iterator->LeftChild = InsertedNode;
                return OK;
            }
        }
        else
        {
            if (Iterator->RightChild != NODE_NULL) Iterator = Iterator->RightChild;
            else
            {
                Iterator->RightChild = InsertedNode;
                return OK;
            }
        }
    }
}

Status LTT_BiTree_Insert_Data(BinaryTreeNode* Root, void* Data, size_t DataSize, CompareFunction Comparator)
{
    BinaryTreeNode* InsertedNode = LTT_BiTree_Make_Node(Data, DataSize);
    if (LTT_BiTree_Insert_Node_byComparator(Root, InsertedNode, Comparator) == ERROR)
    {
        free(InsertedNode);
        return ERROR;
    }
    return OK;
}

Status LTT_BiTree_PreOrder_Traverse_Recursive(BinaryTreeNode* Root, VisitFunction Visit)
{
    if (Root != NODE_NULL)
    {
        if (Visit(Root) == ERROR) return ERROR;
        LTT_BiTree_PreOrder_Traverse_Recursive(Root->LeftChild, Visit);
        LTT_BiTree_PreOrder_Traverse_Recursive(Root->RightChild, Visit);
    }
    return OK;
}

Status LTT_BiTree_PreOrder_Traverse_Stack(BinaryTreeNode* Root, VisitFunction Visit)
{
    ArrayStack*     Stack = LTT_ArrayStack_New(sizeof(BinaryTreeNode), NULL);
    BinaryTreeNode* Temp;
    LTT_ArrayStack_Push(Stack, Root);
    while (!LTT_ArrayStack_IsEmpty(Stack))
    {
        Temp = LTT_ArrayStack_Pop(Stack);
        if (Temp != NODE_NULL)    //如果Temp不是空节点
        {
            if (Visit(Temp) == ERROR) return ERROR;
            LTT_ArrayStack_Push(Stack, Temp->RightChild);
            LTT_ArrayStack_Push(Stack, Temp->LeftChild);
        }
    }
    LTT_ArrayStack_Destroy(Stack);
    return OK;
}

Status LTT_BiTree_InOrder_Traverse_Recursive(BinaryTreeNode* Root, VisitFunction Visit)
{
    if (Root != NODE_NULL)
    {
        LTT_BiTree_InOrder_Traverse_Recursive(Root->LeftChild, Visit);
        if (Visit(Root) == ERROR) return ERROR;
        LTT_BiTree_InOrder_Traverse_Recursive(Root->RightChild, Visit);
    }
    return OK;
}

Status LTT_BiTree_InOrder_Traverse_Stack(BinaryTreeNode* Root, VisitFunction Visit)
{
    ArrayStack*     Stack = LTT_ArrayStack_New(sizeof(BinaryTreeNode), NULL);
    BinaryTreeNode* Temp;
    LTT_ArrayStack_Push(Stack, Root);
    while (!LTT_ArrayStack_IsEmpty(Stack))
    {
        while ((Temp = LTT_ArrayStack_Peek(Stack)) != NODE_NULL) LTT_ArrayStack_Push(Stack, Temp->LeftChild);
        Temp = LTT_ArrayStack_Pop(Stack);    //弹出上面压入栈的一个NULL
        if (!LTT_ArrayStack_IsEmpty(Stack))
        {
            Temp = LTT_ArrayStack_Pop(Stack);
            if (Visit(Temp) == ERROR) return ERROR;
            LTT_ArrayStack_Push(Stack, Temp->RightChild);
        }
    }
    LTT_ArrayStack_Destroy(Stack);
    return OK;
}

Status LTT_BiTree_PostOrder_Traverse_Recursive(BinaryTreeNode* Root, VisitFunction Visit)
{
    if (Root != NODE_NULL)
    {
        LTT_BiTree_PostOrder_Traverse_Recursive(Root->LeftChild, Visit);
        LTT_BiTree_PostOrder_Traverse_Recursive(Root->RightChild, Visit);
        if (Visit(Root) == ERROR) return ERROR;
    }
    return OK;
}

Status LTT_BiTree_PostOrder_Traverse_Stack(BinaryTreeNode* Root, VisitFunction Visit)
{
    ArrayStack*     Stack       = LTT_ArrayStack_New(sizeof(BinaryTreeNode), NULL);
    ArrayStack*     OutputStack = LTT_ArrayStack_New(sizeof(BinaryTreeNode), NULL);
    BinaryTreeNode* Temp;
    LTT_ArrayStack_Push(Stack, Root);
    while (!LTT_ArrayStack_IsEmpty(Stack))
    {
        Temp = LTT_ArrayStack_Pop(Stack);
        if (Temp != NODE_NULL)
        {
            LTT_ArrayStack_Push(OutputStack, Temp);
            LTT_ArrayStack_Push(Stack, Temp->LeftChild);
            LTT_ArrayStack_Push(Stack, Temp->RightChild);
        }
    }
    while (!LTT_ArrayStack_IsEmpty(OutputStack))
    {
        Temp = LTT_ArrayStack_Pop(OutputStack);
        if (Visit(Temp) == ERROR) return ERROR;
    }
    LTT_ArrayStack_Destroy(Stack);
    LTT_ArrayStack_Destroy(OutputStack);
    return OK;
}

Status LTT_BiTree_LevelOrder_Traverse_Queue(BinaryTreeNode* Root, VisitFunction Visit)
{
    ArrayQueue*     Queue = LTT_ArrayQueue_New(sizeof(BinaryTreeNode), NULL);
    BinaryTreeNode* Temp;
    if (Root != NODE_NULL) LTT_ArrayQueue_Push(Queue, Root);
    while (!LTT_ArrayQueue_IsEmpty(Queue))
    {
        Temp = LTT_ArrayQueue_Pop(Queue);
        if (Visit(Temp) == ERROR) return ERROR;
        if (Temp->LeftChild != NODE_NULL) LTT_ArrayQueue_Push(Queue, Temp->LeftChild);
        if (Temp->RightChild != NODE_NULL) LTT_ArrayQueue_Push(Queue, Temp->RightChild);
    }
    return OK;
}

int LTT_BiTree_Get_NodeNumber(BinaryTreeNode* Root)
{
    int             Num   = 0;
    ArrayQueue*     Queue = LTT_ArrayQueue_New(sizeof(BinaryTreeNode), NULL);
    BinaryTreeNode* Temp;
    if (Root != NODE_NULL) LTT_ArrayQueue_Push(Queue, Root);
    while (!LTT_ArrayQueue_IsEmpty(Queue))
    {
        Temp = LTT_ArrayQueue_Pop(Queue);
        ++Num;
        if (Temp->LeftChild != NODE_NULL) LTT_ArrayQueue_Push(Queue, Temp->LeftChild);
        if (Temp->RightChild != NODE_NULL) LTT_ArrayQueue_Push(Queue, Temp->RightChild);
    }
    return Num;
}

int LTT_BiTree_Get_LeafNumber(BinaryTreeNode* Root)
{
    int Num = 0;
    if (Root == NODE_NULL) return 0;
    if ((Root->LeftChild == NODE_NULL) && (Root->RightChild == NODE_NULL)) ++Num;
    Num += LTT_BiTree_Get_LeafNumber(Root->LeftChild);
    Num += LTT_BiTree_Get_LeafNumber(Root->RightChild);
    return Num;
}

int LTT_BiTree_Get_Depth(BinaryTreeNode* Root)
{
    int Depth = 0;
    if (Root == NODE_NULL) return 0;
    else
    {
        int DepthLeft  = LTT_BiTree_Get_Depth(Root->LeftChild);
        int DepthRight = LTT_BiTree_Get_Depth(Root->RightChild);
        Depth          = 1 + (DepthLeft > DepthRight ? DepthLeft : DepthRight);
    }
    return Depth;
}

BinaryTreeNode* LTT_BiTree_Search_Data(BinaryTreeNode* Root, void* Data, CompareFunction Comparator)
{
    BinaryTreeNode* Iterator = Root;
    if (Iterator == NODE_NULL) return NULL;
    while (Iterator != NODE_NULL)
    {
        int Delta = Comparator(Data, Iterator->Data);
        if (Delta == 0) return Iterator;
        else if (Delta < 0) Iterator = Iterator->LeftChild;
        else Iterator = Iterator->RightChild;
    }
    return NULL;
}

void LTT_BiTree_Clear(BinaryTreeNode* Root)
{
    //后序遍历,释放每个节点
    ArrayStack*     Stack       = LTT_ArrayStack_New(sizeof(BinaryTreeNode), NULL);
    ArrayStack*     OutputStack = LTT_ArrayStack_New(sizeof(BinaryTreeNode), NULL);
    BinaryTreeNode* Temp;
    LTT_ArrayStack_Push(Stack, Root);
    while (!LTT_ArrayStack_IsEmpty(Stack))
    {
        Temp = LTT_ArrayStack_Pop(Stack);
        if (Temp != NODE_NULL)
        {
            LTT_ArrayStack_Push(OutputStack, Temp);
            LTT_ArrayStack_Push(Stack, Temp->LeftChild);
            LTT_ArrayStack_Push(Stack, Temp->RightChild);
        }
    }
    while (!LTT_ArrayStack_IsEmpty(OutputStack))
    {
        Temp = LTT_ArrayStack_Pop(OutputStack);
        free(Temp->Data);
        free(Temp);
    }
    LTT_ArrayStack_Destroy(Stack);
    LTT_ArrayStack_Destroy(OutputStack);
}

void LTT_Bitree_Destroy(BinaryTree* BiTree)
{
    LTT_BiTree_Clear(BiTree->Root);
    free(BiTree->P_VTL);
    free(BiTree);
}

/*
    void** Data是一个指针数组
    DataSize是每个数据的大小
    P是查找概率
    Q是失败概率
    Length是数组长度
*/
BinaryTree* LTT_BiTree_Build_Optimal_BST(void** Data, size_t DataSize, double* P, double* Q, int Length)
{
    //使用动态规划算法构造最优二叉查找树
    // Data[1..n]是关键字，P[1..n]是查找概率，Q[0..n]是失败概率
    //
    //
    //
    //
    //构造三个二维数组e[1..n+1,0..n]和w[1..n+1,0..n]和root[1..n,1..n]
    //e[i,j]表示在包含关键字Data[i],Data[i+1],...,Data[j]的子树中进行一次查找的期望代价
    //w[i,j]表示在包含关键字Data[i],Data[i+1],...,Data[j]的子树成为另外一个节点的子树时增加的进行一次查找的期望代价
    //root[i,j]表示以i为根，j个节点的最优二叉查找树的根节点
    double** e    = (double**)malloc((Length + 2) * sizeof(double*));
    double** w    = (double**)malloc((Length + 2) * sizeof(double*));
    int**    root = (int**)malloc((Length + 1) * sizeof(int*));
    for (int i = 0; i < Length + 2; ++i)
    {
        e[i] = (double*)calloc((Length + 1), sizeof(double));
        w[i] = (double*)calloc((Length + 1), sizeof(double));
    }
    for (int i = 0; i < Length + 1; ++i) root[i] = (int*)calloc((Length + 1), sizeof(int));

    //初始化e和w
    for (int i = 1; i <= Length + 1; ++i)
    {
        e[i][i - 1] = Q[i - 1];
        w[i][i - 1] = Q[i - 1];
    }
    //开始动态规划
    for (int i = 1; i <= Length; ++i)
    {
        for (int j = 1; j <= Length - i + 1; ++j)
        {
            int Temp   = i + j - 1;
            e[j][Temp] = 10000;
            w[j][Temp] = w[j][Temp - 1] + P[Temp] + Q[Temp];
            for (int k = j; k <= Temp; ++k)
            {
                double t = e[j][k - 1] + e[k + 1][Temp] + w[j][Temp];
                if (t < e[j][Temp])
                {
                    e[j][Temp]    = t;
                    root[j][Temp] = k;
                }
            }
        }
    }

    for (int i = 1; i <= Length; ++i)
    {
        for (int j = i; j <= Length; ++j) { printf("%d ", root[i][j]); }
        printf("\n");
    }
    for (int i = 1; i <= Length; ++i)
    {
        for (int j = i; j <= Length; ++j) { printf("%lf ", e[i][j]); }
        printf("\n");
    }
    return NULL;
}