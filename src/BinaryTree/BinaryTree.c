#include "BinaryTree.h"

Status InitBiTree(BinaryTreePointer BiTreePointer)
{
    BiTreePointer->NodeNumber      = 0;
    BiTreePointer->RootNodePointer = NULL;
    return OK;
}

Status CreateBiTree_PreandInOrder(BinaryTree BiTree, char* PreOrder, char* InOrder)
{
    if (strlen(PreOrder) != strlen(InOrder)) return ERROR;
    else CreateBiTree_PreandInOrder_Kernel(&(BiTree.RootNodePointer), NULL, PreOrder, InOrder, 0, 0, strlen(PreOrder));
    BiTree.NodeNumber = BiTreeNodeNumber(BiTree.RootNodePointer);
    return OK;
}

void CreateBiTree_PreandInOrder_Kernel(BinaryTreeNodePointer* BiTreeNodePP, BinaryTreeNodePointer ParentBiTreeNodePointer, char* PreOrder, char* InOrder, int PStart, int IStart, int Length)
{
    if (Length == 0) *BiTreeNodePP = NULL;
    else
    {
        char* RootChar = strchr(InOrder, PreOrder[PStart]);
        if (RootChar == NULL) *BiTreeNodePP = NULL;
        else
        {
            int index                 = (int)(RootChar - InOrder);
            (*BiTreeNodePP)           = (BinaryTreeNodePointer)malloc(sizeof(BinaryTreeNode));
            (*BiTreeNodePP)->Data     = (void*)malloc(sizeof(char));
            (*BiTreeNodePP)->Data     = memcpy((*BiTreeNodePP)->Data, PreOrder + PStart, sizeof(char));
            (*BiTreeNodePP)->DataSize = sizeof(char);
            (*BiTreeNodePP)->Parent   = ParentBiTreeNodePointer;
            if (index == IStart) (*BiTreeNodePP)->LeftChild = NULL;
            else CreateBiTree_PreandInOrder_Kernel(&((*BiTreeNodePP)->LeftChild), *BiTreeNodePP, PreOrder, InOrder, PStart + 1, IStart, index - IStart);
            if (index == IStart + Length - 1) (*BiTreeNodePP)->RightChild = NULL;
            else CreateBiTree_PreandInOrder_Kernel(&((*BiTreeNodePP)->RightChild), *BiTreeNodePP, PreOrder, InOrder, PStart + 1 + index - IStart, index + 1, Length - index + IStart - 1);
        }
    }
}

Status MakeBinaryTreeNode(BinaryTreeNodePointer BiTreeNodePointer, void* Data, size_t DataSize)
{
    BiTreeNodePointer->DataSize = DataSize;
    if (!(BiTreeNodePointer->Data = (void*)malloc(DataSize))) return OVERFLOW;
    memcpy(BiTreeNodePointer->Data, Data, DataSize);
    BiTreeNodePointer->LeftChild  = NULL;
    BiTreeNodePointer->Parent     = NULL;
    BiTreeNodePointer->RightChild = NULL;
}

bool BiTreeNodeExist(BinaryTreePointer BiTreePointer, BinaryTreeNodePointer BiTreeNodePointer)
{
    SqQueue Queue;
    InitQueue(&Queue, sizeof(BinaryTreeNodePointer));
    BinaryTreeNodePointer Temp;
    if (BiTreePointer->RootNodePointer) EnQueue(&Queue, &BiTreePointer->RootNodePointer);
    while (!QueueEmpty(Queue))
    {
        DeQueue(&Queue, &Temp);
        if (Temp == BiTreeNodePointer) return true;
        if (Temp->LeftChild) EnQueue(&Queue, &Temp->LeftChild);
        if (Temp->RightChild) EnQueue(&Queue, &Temp->RightChild);
    }
    return false;
}

Status DestroyBiTree(BinaryTreePointer BiTreePointer)
{
    ClearBiTree(BiTreePointer);
    free(BiTreePointer);
    BiTreePointer = NULL;
    return OK;
}

Status ClearBiTree(BinaryTreePointer BiTreePointer)
{
    PreOrderTraverse_Stack(BiTreePointer->RootNodePointer, ClearBiTreeVisit);
    BiTreePointer->NodeNumber      = 0;
    BiTreePointer->RootNodePointer = NULL;
    return OK;
}

bool BiTreeEmpty(BinaryTree BiTree)
{
    if (BiTree.RootNodePointer) return false;
    else return true;
}

int BiTreeNodeNumber(BinaryTreeNodePointer BiTreeNodePointer)
{
    int     Num = 0;
    SqQueue Queue;
    InitQueue(&Queue, sizeof(BinaryTreeNodePointer));
    BinaryTreeNodePointer Temp;
    if (BiTreeNodePointer) EnQueue(&Queue, &BiTreeNodePointer);
    while (!QueueEmpty(Queue))
    {
        DeQueue(&Queue, &Temp);
        ++Num;
        if (Temp->LeftChild) EnQueue(&Queue, &Temp->LeftChild);
        if (Temp->RightChild) EnQueue(&Queue, &Temp->RightChild);
    }
    return Num;
}

int BiTreeNodeLeafNumber(BinaryTreeNodePointer BiTreeNodePointer)
{
    int Num = 0;
    if (!BiTreeNodePointer) return 0;
    if ((!BiTreeNodePointer->LeftChild) && (!BiTreeNodePointer->RightChild)) Num++;
    Num += BiTreeNodeLeafNumber(BiTreeNodePointer->LeftChild);
    Num += BiTreeNodeLeafNumber(BiTreeNodePointer->RightChild);
    return Num;
}

int BiTreeNodeDepth(BinaryTreeNodePointer BiTreeNodePointer)
{
    int Depth = 0;
    if (!BiTreeNodePointer) return 0;
    else
    {
        int DepthLeft  = BiTreeNodeDepth(BiTreeNodePointer->LeftChild);
        int DepthRight = BiTreeNodeDepth(BiTreeNodePointer->RightChild);
        Depth          = 1 + (DepthLeft > DepthRight ? DepthLeft : DepthRight);
    }
    return Depth;
}

BinaryTreeNodePointer Root(BinaryTree BiTree) { return BiTree.RootNodePointer; }

void*                 Value(BinaryTreePointer BiTreePointer, BinaryTreeNodePointer BiTreeNodePointer)
{
    if (!BiTreeNodeExist(BiTreePointer, BiTreeNodePointer)) return NULL;
    return BiTreeNodePointer->Data;
}

Status Assign(BinaryTreePointer BiTreePointer, BinaryTreeNodePointer BiTreeNodePointer, void* Data)
{
    if (!BiTreeNodeExist(BiTreePointer, BiTreeNodePointer)) return ERROR;
    memcpy(BiTreeNodePointer->Data, Data, BiTreeNodePointer->DataSize);
    return OK;
}

BinaryTreeNodePointer Parent(BinaryTreePointer BiTreePointer, BinaryTreeNodePointer BiTreeNodePointer)
{
    if (!BiTreeNodeExist(BiTreePointer, BiTreeNodePointer)) return ERROR;
    return BiTreeNodePointer->Parent;
}

BinaryTreeNodePointer LeftChild(BinaryTreePointer BiTreePointer, BinaryTreeNodePointer BiTreeNodePointer)
{
    if (!BiTreeNodeExist(BiTreePointer, BiTreeNodePointer)) return ERROR;
    return BiTreeNodePointer->LeftChild;
}

BinaryTreeNodePointer RightChild(BinaryTreePointer BiTreePointer, BinaryTreeNodePointer BiTreeNodePointer)
{
    if (!BiTreeNodeExist(BiTreePointer, BiTreeNodePointer)) return ERROR;
    return BiTreeNodePointer->RightChild;
}

BinaryTreeNodePointer LeftSibling(BinaryTreePointer BiTreePointer, BinaryTreeNodePointer BiTreeNodePointer)
{
    if (!BiTreeNodeExist(BiTreePointer, BiTreeNodePointer)) return ERROR;
    if (BiTreeNodePointer == BiTreeNodePointer->Parent->RightChild) return BiTreeNodePointer->Parent->LeftChild;
    else return NULL;
}

BinaryTreeNodePointer RightSibling(BinaryTreePointer BiTreePointer, BinaryTreeNodePointer BiTreeNodePointer)
{
    if (!BiTreeNodeExist(BiTreePointer, BiTreeNodePointer)) return ERROR;
    if (BiTreeNodePointer == BiTreeNodePointer->Parent->LeftChild) return BiTreeNodePointer->Parent->RightChild;
    else return NULL;
}

Status InsertBinaryTreeNode(BinaryTreePointer BiTreePointer, BinaryTreeNodePointer InsertedPositionNodePointer, bool LeftChild, BinaryTreeNodePointer InsertedBiTreeNodePointer)
{
    if (!BiTreeNodeExist(BiTreePointer, InsertedPositionNodePointer)) return ERROR;
    if (LeftChild)
    {
        if (InsertedPositionNodePointer->LeftChild) return ERROR;
        BiTreePointer->NodeNumber              += BiTreeNodeNumber(InsertedBiTreeNodePointer);
        InsertedPositionNodePointer->LeftChild  = InsertedBiTreeNodePointer;
        InsertedBiTreeNodePointer->Parent       = InsertedPositionNodePointer;
        return OK;
    }
    else
    {
        if (InsertedPositionNodePointer->RightChild) return ERROR;
        BiTreePointer->NodeNumber               += BiTreeNodeNumber(InsertedBiTreeNodePointer);
        InsertedPositionNodePointer->RightChild  = InsertedBiTreeNodePointer;
        InsertedBiTreeNodePointer->Parent        = InsertedPositionNodePointer;
        ++BiTreePointer->NodeNumber;
        return OK;
    }
}

Status DeleteChild(BinaryTreePointer BiTreePointer, BinaryTreeNodePointer DeletedBiTreeNodePointer, bool LeftChild)
{
    if (!BiTreeNodeExist(BiTreePointer, DeletedBiTreeNodePointer)) return ERROR;
    if (LeftChild)
    {
        SqStack Stack;
        InitStack(&Stack, sizeof(BinaryTreeNodePointer));
        Push(&Stack, &DeletedBiTreeNodePointer->LeftChild);
        while (!StackEmpty(Stack))
        {
            BinaryTreeNodePointer Temp;
            Pop(&Stack, &Temp);
            if (Temp)
            {
                --BiTreePointer->NodeNumber;
                if (!(*ClearBiTreeVisit)(Temp)) return ERROR;
                Push(&Stack, &Temp->RightChild);
                Push(&Stack, &Temp->LeftChild);
            }
        }
        return OK;
    }
    else
    {
        SqStack               Stack;
        BinaryTreeNodePointer Temp;
        InitStack(&Stack, sizeof(BinaryTreeNodePointer));
        Push(&Stack, &DeletedBiTreeNodePointer->RightChild);
        while (!StackEmpty(Stack))
        {
            Pop(&Stack, &Temp);
            if (Temp)
            {
                --BiTreePointer->NodeNumber;
                if (!(*ClearBiTreeVisit)(Temp)) return ERROR;
                Push(&Stack, &Temp->RightChild);
                Push(&Stack, &Temp->LeftChild);
            }
        }
        return OK;
    }
}

Status PreOrderTraverse_Recursive(BinaryTreeNodePointer BiTreeNodePointer, Status (*Visit)(BinaryTreeNodePointer))
{
    if (BiTreeNodePointer)
    {
        (*Visit)(BiTreeNodePointer);
        PreOrderTraverse_Recursive(BiTreeNodePointer->LeftChild, Visit);
        PreOrderTraverse_Recursive(BiTreeNodePointer->RightChild, Visit);
        return OK;
    }
}

Status PreOrderTraverse_Stack(BinaryTreeNodePointer BiTreeNodePointer, Status (*Visit)(BinaryTreeNodePointer))
{
    SqStack               Stack;
    BinaryTreeNodePointer Temp;
    InitStack(&Stack, sizeof(BinaryTreeNodePointer));
    Push(&Stack, &BiTreeNodePointer);
    while (!StackEmpty(Stack))
    {
        Pop(&Stack, &Temp);
        if (Temp)
        {
            if (!(*Visit)(Temp)) return ERROR;
            Push(&Stack, &Temp->RightChild);
            Push(&Stack, &Temp->LeftChild);
        }
    }
    return OK;
}

Status InOrderTraverse_Recursive(BinaryTreeNodePointer BiTreeNodePointer, Status (*Visit)(BinaryTreeNodePointer))
{
    if (BiTreeNodePointer)
    {
        InOrderTraverse_Recursive(BiTreeNodePointer->LeftChild, Visit);
        (*Visit)(BiTreeNodePointer);
        InOrderTraverse_Recursive(BiTreeNodePointer->RightChild, Visit);
        return OK;
    }
}

Status InOrderTraverse_Stack(BinaryTreeNodePointer BiTreeNodePointer, Status (*Visit)(BinaryTreeNodePointer))
{
    SqStack               Stack;
    BinaryTreeNodePointer Temp;
    InitStack(&Stack, sizeof(BinaryTreeNodePointer));
    Push(&Stack, &BiTreeNodePointer);
    while (!StackEmpty(Stack))
    {
        while (GetTop(Stack, &Temp) && Temp) Push(&Stack, &Temp->LeftChild);
        Pop(&Stack, &Temp);    //弹出上面压入栈的一个NULL
        if (!StackEmpty(Stack))
        {
            Pop(&Stack, &Temp);
            if (!(*Visit)(Temp)) return ERROR;
            Push(&Stack, &Temp->RightChild);
        }
    }
    return OK;
}

Status PostOrderTraverse_Recursive(BinaryTreeNodePointer BiTreeNodePointer, Status (*Visit)(BinaryTreeNodePointer))
{
    if (BiTreeNodePointer)
    {
        PostOrderTraverse_Recursive(BiTreeNodePointer->LeftChild, Visit);
        PostOrderTraverse_Recursive(BiTreeNodePointer->RightChild, Visit);
        (*Visit)(BiTreeNodePointer);
        return OK;
    }
}

Status PostOrderTraverse_Stack(BinaryTreeNodePointer BiTreeNodePointer, Status (*Visit)(BinaryTreeNodePointer))
{
    SqStack               Stack;
    SqStack               OutputStack;
    BinaryTreeNodePointer Temp;
    InitStack(&Stack, sizeof(BinaryTreeNodePointer));
    InitStack(&OutputStack, sizeof(BinaryTreeNodePointer));
    Push(&Stack, &BiTreeNodePointer);
    while (!StackEmpty(Stack))
    {
        Pop(&Stack, &Temp);
        if (Temp)
        {
            Push(&OutputStack, &Temp);
            Push(&Stack, &Temp->LeftChild);
            Push(&Stack, &Temp->RightChild);
        }
    }
    while (!StackEmpty(OutputStack))
    {
        Pop(&OutputStack, &Temp);
        if (!(*Visit)(Temp)) return ERROR;
    }
    return OK;
}

Status LevelOrderTraverse(BinaryTreeNodePointer BiTreeNodePointer, Status (*Visit)(BinaryTreeNodePointer))
{
    SqQueue Queue;
    InitQueue(&Queue, sizeof(BinaryTreeNodePointer));
    BinaryTreeNodePointer Temp;
    if (BiTreeNodePointer) EnQueue(&Queue, &BiTreeNodePointer);
    while (!QueueEmpty(Queue))
    {
        DeQueue(&Queue, &Temp);
        (*Visit)(Temp);
        if (Temp->LeftChild) EnQueue(&Queue, &Temp->LeftChild);
        if (Temp->RightChild) EnQueue(&Queue, &Temp->RightChild);
    }
}

Status ClearBiTreeVisit(BinaryTreeNodePointer BiTreeNodePointer)
{
    free(BiTreeNodePointer->Data);
    free(BiTreeNodePointer);
    BiTreeNodePointer = NULL;
    return OK;
}