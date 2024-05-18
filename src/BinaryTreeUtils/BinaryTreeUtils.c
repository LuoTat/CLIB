#include "BinaryTreeUtils.h"
#include <stdlib.h>
#include "ArrayQueue.h"
#include "ArrayStack.h"
#include "BinaryTree.h"

void LTT_BiTreeUtils_DeleteSubTree(BinaryTreeNode** Root)
{
    // 后序遍历,释放每个节点
    ArrayStack*      Stack       = LTT_ArrayStack_New(sizeof(BinaryTreeNode*), NULL);
    ArrayStack*      OutputStack = LTT_ArrayStack_New(sizeof(BinaryTreeNode*), NULL);
    BinaryTreeNode** Temp;
    LTT_ArrayStack_Push(Stack, Root);
    while (!LTT_ArrayStack_IsEmpty(Stack))
    {
        Temp = LTT_ArrayStack_Pop(Stack);
        if (*Temp != NODE_NULL)
        {
            LTT_ArrayStack_Push(OutputStack, Temp);
            LTT_ArrayStack_Push(Stack, &(*Temp)->LeftChild);
            LTT_ArrayStack_Push(Stack, &(*Temp)->RightChild);
        }
    }
    while (!LTT_ArrayStack_IsEmpty(OutputStack))
    {
        Temp = LTT_ArrayStack_Pop(OutputStack);
        LTT_BiTree_DestroyNode(Temp);
    }
    LTT_ArrayStack_Destroy(&Stack);
    LTT_ArrayStack_Destroy(&OutputStack);
}

int LTT_BiTreeUtils_GetNodeNumber(BinaryTreeNode* const Root)
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

int LTT_BiTreeUtils_GetLeafNumber(BinaryTreeNode* const Root)
{
    int Num = 0;
    if (Root == NODE_NULL) return 0;
    if ((Root->LeftChild == NODE_NULL) && (Root->RightChild == NODE_NULL)) ++Num;
    Num += LTT_BiTreeUtils_GetLeafNumber(Root->LeftChild);
    Num += LTT_BiTreeUtils_GetLeafNumber(Root->RightChild);
    return Num;
}

int LTT_BiTreeUtils_GetDepth(BinaryTreeNode* const Root)
{
    int Depth = 0;
    if (Root == NODE_NULL) return 0;
    else
    {
        int DepthLeft  = LTT_BiTreeUtils_GetDepth(Root->LeftChild);
        int DepthRight = LTT_BiTreeUtils_GetDepth(Root->RightChild);
        Depth          = 1 + (DepthLeft > DepthRight ? DepthLeft : DepthRight);
    }
    return Depth;
}

Status LTT_BiTreeUtils_PreOrder_Traverse_Recursive(BinaryTreeNode* const Root, const VisitFunction Visit)
{
    if (Root != NODE_NULL)
    {
        if (Visit(Root) == ERROR) return ERROR;
        LTT_BiTreeUtils_PreOrder_Traverse_Recursive(Root->LeftChild, Visit);
        LTT_BiTreeUtils_PreOrder_Traverse_Recursive(Root->RightChild, Visit);
    }
    return OK;
}

Status LTT_BiTreeUtils_PreOrder_Traverse_Stack(BinaryTreeNode* const Root, const VisitFunction Visit)
{
    ArrayStack*     Stack = LTT_ArrayStack_New(sizeof(BinaryTreeNode), NULL);
    BinaryTreeNode* Temp;
    LTT_ArrayStack_Push(Stack, Root);
    while (!LTT_ArrayStack_IsEmpty(Stack))
    {
        Temp = LTT_ArrayStack_Pop(Stack);
        if (Temp != NODE_NULL)    // 如果Temp不是空节点
        {
            if (Visit(Temp) == ERROR) return ERROR;
            LTT_ArrayStack_Push(Stack, Temp->RightChild);
            LTT_ArrayStack_Push(Stack, Temp->LeftChild);
        }
    }
    LTT_ArrayStack_Destroy(&Stack);
    return OK;
}

Status LTT_BiTreeUtils_InOrder_Traverse_Recursive(BinaryTreeNode* const Root, const VisitFunction Visit)
{
    if (Root != NODE_NULL)
    {
        LTT_BiTreeUtils_InOrder_Traverse_Recursive(Root->LeftChild, Visit);
        if (Visit(Root) == ERROR) return ERROR;
        LTT_BiTreeUtils_InOrder_Traverse_Recursive(Root->RightChild, Visit);
    }
    return OK;
}

Status LTT_BiTreeUtils_InOrder_Traverse_Stack(BinaryTreeNode* const Root, const VisitFunction Visit)
{
    ArrayStack*     Stack = LTT_ArrayStack_New(sizeof(BinaryTreeNode), NULL);
    BinaryTreeNode* Temp;
    LTT_ArrayStack_Push(Stack, Root);
    while (!LTT_ArrayStack_IsEmpty(Stack))
    {
        while ((Temp = LTT_ArrayStack_Peek(Stack)) != NODE_NULL) LTT_ArrayStack_Push(Stack, Temp->LeftChild);
        LTT_ArrayStack_Pop(Stack);    // 弹出上面压入栈的一个NODE_NULL
        if (!LTT_ArrayStack_IsEmpty(Stack))
        {
            Temp = LTT_ArrayStack_Pop(Stack);
            if (Visit(Temp) == ERROR) return ERROR;
            LTT_ArrayStack_Push(Stack, Temp->RightChild);
        }
    }
    LTT_ArrayStack_Destroy(&Stack);
    return OK;
}

Status LTT_BiTreeUtils_PostOrder_Traverse_Recursive(BinaryTreeNode* const Root, const VisitFunction Visit)
{
    if (Root != NODE_NULL)
    {
        LTT_BiTreeUtils_PostOrder_Traverse_Recursive(Root->LeftChild, Visit);
        LTT_BiTreeUtils_PostOrder_Traverse_Recursive(Root->RightChild, Visit);
        if (Visit(Root) == ERROR) return ERROR;
    }
    return OK;
}

Status LTT_BiTreeUtils_PostOrder_Traverse_Stack(BinaryTreeNode* const Root, const VisitFunction Visit)
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
    LTT_ArrayStack_Destroy(&Stack);
    LTT_ArrayStack_Destroy(&OutputStack);
    return OK;
}

Status LTT_BiTreeUtils_LevelOrder_Traverse_Queue(BinaryTreeNode* const Root, const VisitFunction Visit)
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
    LTT_ArrayQueue_Destroy(&Queue);
    return OK;
}
