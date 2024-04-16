#include "BinaryTreeNode.h"
#include "../Predefined/Predefined.h"
#include "LTT_ArrayQueue.h"
#include "LTT_ArrayStack.h"

BinaryTreeNode Node_NULL = {NULL, 0, NULL};

BinaryTreeNode* LTT_BiTreeNode_MakeNode(void* const Data)
{
    BinaryTreeNode* RootNode = (BinaryTreeNode*)malloc(sizeof(BinaryTreeNode));
    if (RootNode == NULL)
    {
        printf("根节点制作失败!\n");
        return NULL;
    }
    RootNode->Data       = Data;
    RootNode->LeftChild  = NODE_NULL;
    RootNode->RightChild = NODE_NULL;
    RootNode->Parent     = NODE_NULL;
    return RootNode;
}

Status LTT_BiTreeNode_InsertNode(BinaryTreeNode* const BeInserted_Node, BinaryTreeNode* const Inserted_Node, const bool LeftChild)
{
    // 把Inserted_Node插入到BeInserted_Node的左子树或者右子树
    if (LeftChild)
    {
        if (BeInserted_Node->LeftChild != NODE_NULL) return ERROR;    //如果左子树不为空，返回ERROR
        BeInserted_Node->LeftChild = Inserted_Node;
        Inserted_Node->Parent      = BeInserted_Node;
        return OK;
    }
    else
    {
        if (BeInserted_Node->RightChild != NODE_NULL) return ERROR;    //如果右子树不为空，返回ERROR
        BeInserted_Node->RightChild = Inserted_Node;
        Inserted_Node->Parent       = BeInserted_Node;
        return OK;
    }
}

BinaryTreeNode* LTT_BiTreeNode_SearchNode(BinaryTreeNode* Root, const void* const Data, const CompareFunction Comparator)
{
    while (Root != NODE_NULL && Comparator(Data, Root->Data) != 0)
    {
        if (Comparator(Data, Root->Data) < 0) Root = Root->LeftChild;
        else Root = Root->RightChild;
    }
    return Root;
}

BinaryTreeNode* LTT_BiTreeNode_GetMaxNode(BinaryTreeNode* Root, const CompareFunction Comparator)
{
    while (Root->RightChild != NODE_NULL) Root = Root->RightChild;
    return Root;
}

BinaryTreeNode* LTT_BiTreeNode_GetMinNode(BinaryTreeNode* Root, const CompareFunction Comparator)
{
    while (Root->LeftChild != NODE_NULL) Root = Root->LeftChild;
    return Root;
}

BinaryTreeNode* LTT_BiTreeNode_GetPredecessorNode(BinaryTreeNode* Root, const CompareFunction Comparator)
{
    if (Root->LeftChild != NODE_NULL) return LTT_BiTreeNode_GetMaxNode(Root->LeftChild, Comparator);
    BinaryTreeNode* Temp = Root->Parent;
    while (Temp != NODE_NULL && Root == Temp->LeftChild)
    {
        Root = Temp;
        Temp = Temp->Parent;
    }
    return Temp;
}

BinaryTreeNode* LTT_BiTreeNode_GetSuccessorNode(BinaryTreeNode* Root, const CompareFunction Comparator)
{
    if (Root->RightChild != NODE_NULL) return LTT_BiTreeNode_GetMinNode(Root->RightChild, Comparator);
    BinaryTreeNode* Temp = Root->Parent;
    while (Temp != NODE_NULL && Root == Temp->RightChild)
    {
        Root = Temp;
        Temp = Temp->Parent;
    }
    return Temp;
}

void LTT_BiTreeNode_DeleteSubTree(BinaryTreeNode** Root)
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
        free(*Temp);
        *Temp = NODE_NULL;
    }
    LTT_ArrayStack_Destroy(&Stack);
    LTT_ArrayStack_Destroy(&OutputStack);
}

void LTT_BiTreeNode_DestroyNode(BinaryTreeNode** Root)
{
    free(*Root);
    *Root = NODE_NULL;
}

int LTT_BiTreeNode_GetNodeNumber(BinaryTreeNode* const Root)
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

int LTT_BiTreeNode_GetLeafNumber(BinaryTreeNode* const Root)
{
    int Num = 0;
    if (Root == NODE_NULL) return 0;
    if ((Root->LeftChild == NODE_NULL) && (Root->RightChild == NODE_NULL)) ++Num;
    Num += LTT_BiTreeNode_GetLeafNumber(Root->LeftChild);
    Num += LTT_BiTreeNode_GetLeafNumber(Root->RightChild);
    return Num;
}

int LTT_BiTreeNode_GetDepth(BinaryTreeNode* const Root)
{
    int Depth = 0;
    if (Root == NODE_NULL) return 0;
    else
    {
        int DepthLeft  = LTT_BiTreeNode_GetDepth(Root->LeftChild);
        int DepthRight = LTT_BiTreeNode_GetDepth(Root->RightChild);
        Depth          = 1 + (DepthLeft > DepthRight ? DepthLeft : DepthRight);
    }
    return Depth;
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
        Temp = LTT_ArrayStack_Pop(Stack);    // 弹出上面压入栈的一个NODE_NULL
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
    LTT_ArrayStack_Destroy(&Stack);
    LTT_ArrayStack_Destroy(&OutputStack);
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
    LTT_ArrayQueue_Destroy(&Queue);
    return OK;
}
