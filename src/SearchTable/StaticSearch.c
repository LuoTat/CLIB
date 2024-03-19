#include "StaticSearch.h"

void* BinarySearch(const void* Key, const void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator)
{
    int Low  = 1;
    int High = NumOfElements;
    while (Low <= High)
    {
        int Mid = Low + High >> 1;
        if (Comparator(Key, (char*)Base + (Mid - 1) * SizeOfElements) == 0) return (char*)Base + (Mid - 1) * SizeOfElements;
        else if (Comparator(Key, (char*)Base + (Mid - 1) * SizeOfElements) < 0) High = Mid - 1;
        else Low = Mid + 1;
    }
    return NULL;
}

void* InterpolationSearch(const void* Key, const void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator)
{
    int Low  = 1;
    int High = NumOfElements;
    while (Low <= High)
    {
        int Mid = Low + (Comparator(Key, (char*)Base + (Low - 1) * SizeOfElements)) / (Comparator((char*)Base + (High - 1) * SizeOfElements, (char*)Base + (Low - 1) * SizeOfElements)) * (High - Low);
        if (Comparator(Key, (char*)Base + (Mid - 1) * SizeOfElements) == 0) return (char*)Base + (Mid - 1) * SizeOfElements;
        else if (Comparator(Key, (char*)Base + (Mid - 1) * SizeOfElements) < 0) High = Mid - 1;
        else Low = Mid + 1;
    }
    return NULL;
}

Status CreateNearlyOptimalSearchTree(BinaryTreePointer BiTree, const void* Data, size_t DataNum, size_t DataSize, int* Weight)
{
    BiTree->NodeNumber = DataNum;
    int* SW            = (int*)calloc(DataNum + 1, sizeof(int));
    SW[0]              = 0;
    for (int i = 1; i <= DataNum; ++i) SW[i] = SW[i - 1] + Weight[i - 1];
    if (!CreateNearlyOptimalSearchTree_Kernel(&(BiTree->RootNodePointer), NULL, Data, DataSize, SW, 1, DataNum)) return ERROR;
    free(SW);
    return OK;
}

Status CreateNearlyOptimalSearchTree_Kernel(BinaryTreeNodePointer* BiTreeNodePP, BinaryTreeNodePointer Parent, const void* Data, size_t DataSize, int* SW, int Low, int High)
{
    int RootNum = Low;
    int Min     = INT_MAX;
    for (int i = Low; i <= High; ++i)
    {
        if (Min > abs(SW[High] - SW[i] - SW[i - 1] + SW[Low - 1]))
        {
            Min     = abs(SW[High] - SW[i] - SW[i - 1] + SW[Low - 1]);
            RootNum = i;
        }
    }
    if (!((*BiTreeNodePP) = (BinaryTreeNodePointer)malloc(sizeof(BinaryTreeNode)))) return OVERFLOW;
    MakeBinaryTreeNode((*BiTreeNodePP), (char*)Data + DataSize * (RootNum - 1), DataSize);
    (*BiTreeNodePP)->Parent = Parent;
    if (RootNum == Low) (*BiTreeNodePP)->LeftChild = NULL;
    else CreateNearlyOptimalSearchTree_Kernel(&((*BiTreeNodePP)->LeftChild), (*BiTreeNodePP), Data, DataSize, SW, Low, RootNum - 1);
    if (RootNum == High) (*BiTreeNodePP)->RightChild = NULL;
    else CreateNearlyOptimalSearchTree_Kernel(&((*BiTreeNodePP)->RightChild), (*BiTreeNodePP), Data, DataSize, SW, RootNum + 1, High);
    return OK;
}

Status CreateLTTSearchTree(BinaryTreePointer BiTree, const void* Data, size_t DataNum, size_t DataSize, int* Weight)
{
    BiTree->NodeNumber = DataNum;
    if (!CreateLTTSearchTree_Kernel(&(BiTree->RootNodePointer), NULL, Data, DataSize, Weight, 0, DataNum - 1)) return ERROR;
    return OK;
}

Status CreateLTTSearchTree_Kernel(BinaryTreeNodePointer* BiTreeNodePP, BinaryTreeNodePointer Parent, const void* Data, size_t DataSize, int* Weight, int Low, int High)
{
    int RootNum = Low;
    int Max     = 0;
    for (int i = Low; i <= High; ++i)
    {
        if (Max < Weight[i])
        {
            Max     = Weight[i];
            RootNum = i;
        }
    }
    if (!((*BiTreeNodePP) = (BinaryTreeNodePointer)malloc(sizeof(BinaryTreeNode)))) return OVERFLOW;
    MakeBinaryTreeNode((*BiTreeNodePP), (char*)Data + DataSize * RootNum, DataSize);
    (*BiTreeNodePP)->Parent = Parent;
    if (RootNum == Low) (*BiTreeNodePP)->LeftChild = NULL;
    else CreateLTTSearchTree_Kernel(&((*BiTreeNodePP)->LeftChild), (*BiTreeNodePP), Data, DataSize, Weight, Low, RootNum - 1);
    if (RootNum == High) (*BiTreeNodePP)->RightChild = NULL;
    else CreateLTTSearchTree_Kernel(&((*BiTreeNodePP)->RightChild), (*BiTreeNodePP), Data, DataSize, Weight, RootNum + 1, High);
    return OK;
}

unsigned long long GetTheASLOfSearchTree(BinaryTree BiTree)
{
    unsigned long long ASL = 0;
    SqQueue            Queue;
    SqQueue            DepthQueue;
    InitQueue(&Queue, sizeof(BinaryTreeNodePointer));
    InitQueue(&DepthQueue, sizeof(int));
    int                   Depth = 0;
    BinaryTreeNodePointer Temp;
    if (BiTree.RootNodePointer)
    {
        EnQueue(&Queue, &BiTree.RootNodePointer);
        ++Depth;
        EnQueue(&DepthQueue, &Depth);
    }
    while (!QueueEmpty(Queue))
    {
        DeQueue(&Queue, &Temp);
        DeQueue(&DepthQueue, &Depth);
        ASL += ((Data_Weight*)Temp->Data)->Weight * Depth;
        if (Temp->LeftChild)
        {
            int Temp_Depth = Depth + 1;
            EnQueue(&Queue, &Temp->LeftChild);
            EnQueue(&DepthQueue, &Temp_Depth);
        }
        if (Temp->RightChild)
        {
            int Temp_Depth = Depth + 1;
            EnQueue(&Queue, &Temp->RightChild);
            EnQueue(&DepthQueue, &Temp_Depth);
        }
    }
    return ASL;
}