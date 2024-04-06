#include "BinaryTreeNode.h"
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Predefined/Predefined.h"
#include "LTT_ArrayQueue.h"
#include "LTT_ArrayStack.h"

static BinaryTreeNode Node_NULL = {NULL, 0, NULL, NULL};
#define NODE_NULL (&Node_NULL)

BinaryTreeNode* LTT_BiTreeNode_Make_Node(void* const Data, const size_t DataSize)
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

Status LTT_BiTreeNode_Insert_Node(BinaryTreeNode* const BeInserted_Node, BinaryTreeNode* const Inserted_Node, const bool LeftChild)
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

Status LTT_BiTreeNode_Insert_Node_byComparator(BinaryTreeNode* const Root, BinaryTreeNode* const Inserted_Node, const CompareFunction Comparator)
{
    BinaryTreeNode* Iterator = Root;
    if (Iterator == NODE_NULL)
    {
        printf("根节点为空!\n");
        return ERROR;
    }
    while (true)
    {
        int Delta = Comparator(Inserted_Node->Data, Iterator->Data);
        if (Delta == 0) { return ERROR; }
        else if (Delta < 0)
        {
            if (Iterator->LeftChild != NODE_NULL) Iterator = Iterator->LeftChild;
            else
            {
                Iterator->LeftChild = Inserted_Node;
                return OK;
            }
        }
        else
        {
            if (Iterator->RightChild != NODE_NULL) Iterator = Iterator->RightChild;
            else
            {
                Iterator->RightChild = Inserted_Node;
                return OK;
            }
        }
    }
}

Status LTT_BiTreeNode_Insert_Data(BinaryTreeNode* const Root, void* const Data, const size_t DataSize, const CompareFunction Comparator)
{
    BinaryTreeNode* InsertedNode = LTT_BiTreeNode_Make_Node(Data, DataSize);
    if (LTT_BiTreeNode_Insert_Node_byComparator(Root, InsertedNode, Comparator) == ERROR)
    {
        free(InsertedNode);
        return ERROR;
    }
    return OK;
}

int LTT_BiTreeNode_Get_NodeNumber(BinaryTreeNode* const Root)
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

int LTT_BiTreeNode_Get_LeafNumber(BinaryTreeNode* const Root)
{
    int Num = 0;
    if (Root == NODE_NULL) return 0;
    if ((Root->LeftChild == NODE_NULL) && (Root->RightChild == NODE_NULL)) ++Num;
    Num += LTT_BiTreeNode_Get_LeafNumber(Root->LeftChild);
    Num += LTT_BiTreeNode_Get_LeafNumber(Root->RightChild);
    return Num;
}

int LTT_BiTreeNode_Get_Depth(BinaryTreeNode* const Root)
{
    int Depth = 0;
    if (Root == NODE_NULL) return 0;
    else
    {
        int DepthLeft  = LTT_BiTreeNode_Get_Depth(Root->LeftChild);
        int DepthRight = LTT_BiTreeNode_Get_Depth(Root->RightChild);
        Depth          = 1 + (DepthLeft > DepthRight ? DepthLeft : DepthRight);
    }
    return Depth;
}

BinaryTreeNode* LTT_BiTreeNode_Search_Data(BinaryTreeNode* const Root, const void* const Data, const CompareFunction Comparator)
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

void LTT_BiTreeNode_Clear(BinaryTreeNode* const Root)
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

Status LTT_BiTreeNode_PreOrder_Traverse_Recursive(BinaryTreeNode* const Root, const VisitFunction Visit)
{
    if (Root != NODE_NULL)
    {
        if (Visit(Root) == ERROR) return ERROR;
        LTT_BiTreeNode_PreOrder_Traverse_Recursive(Root->LeftChild, Visit);
        LTT_BiTreeNode_PreOrder_Traverse_Recursive(Root->RightChild, Visit);
    }
    return OK;
}

Status LTT_BiTreeNode_PreOrder_Traverse_Stack(BinaryTreeNode* const Root, const VisitFunction Visit)
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

Status LTT_BiTreeNode_InOrder_Traverse_Recursive(BinaryTreeNode* const Root, const VisitFunction Visit)
{
    if (Root != NODE_NULL)
    {
        LTT_BiTreeNode_InOrder_Traverse_Recursive(Root->LeftChild, Visit);
        if (Visit(Root) == ERROR) return ERROR;
        LTT_BiTreeNode_InOrder_Traverse_Recursive(Root->RightChild, Visit);
    }
    return OK;
}

Status LTT_BiTreeNode_InOrder_Traverse_Stack(BinaryTreeNode* const Root, const VisitFunction Visit)
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

Status LTT_BiTreeNode_PostOrder_Traverse_Recursive(BinaryTreeNode* const Root, const VisitFunction Visit)
{
    if (Root != NODE_NULL)
    {
        LTT_BiTreeNode_PostOrder_Traverse_Recursive(Root->LeftChild, Visit);
        LTT_BiTreeNode_PostOrder_Traverse_Recursive(Root->RightChild, Visit);
        if (Visit(Root) == ERROR) return ERROR;
    }
    return OK;
}

Status LTT_BiTreeNode_PostOrder_Traverse_Stack(BinaryTreeNode* const Root, const VisitFunction Visit)
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

Status LTT_BiTreeNode_LevelOrder_Traverse_Queue(BinaryTreeNode* const Root, const VisitFunction Visit)
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
