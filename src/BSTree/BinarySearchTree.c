#include "BinarySearchTree.h"

Status InitBST(BinaryTreePointer BSTP)
{
    BSTP->NodeNumber      = 0;
    BSTP->RootNodePointer = NULL;
    return OK;
}

void* SearchBST(BinaryTreePointer BSTP, void* Key, CompareFunction Comparator)
{
    BinaryTreeNodePointer Iterator = BSTP->RootNodePointer;
    if (!Iterator) return NULL;
    while (Iterator)
    {
        int Delta = Comparator(Key, Iterator->Data);
        if (Delta == 0) return Iterator->Data;
        else if (Delta < 0) Iterator = Iterator->LeftChild;
        else Iterator = Iterator->RightChild;
    }
    return NULL;
}

Status InsertBST(BinaryTreePointer BSTP, void* Data, size_t DataSize, CompareFunction Comparator)
{
    BinaryTreeNodePointer InsertedNodePointer = (BinaryTreeNodePointer)malloc(sizeof(BinaryTreeNode));
    MakeBinaryTreeNode(InsertedNodePointer, Data, DataSize);
    BinaryTreeNodePointer Iterator = BSTP->RootNodePointer;
    if (!Iterator)
    {
        BSTP->RootNodePointer = InsertedNodePointer;
        ++BSTP->NodeNumber;
        return OK;
    }
    while (true)
    {
        int Delta = Comparator(Data, Iterator->Data);
        if (Delta == 0)
        {
            ClearBiTreeVisit(InsertedNodePointer);
            return false;
        }
        else if (Delta < 0)
        {
            if (Iterator->LeftChild) Iterator = Iterator->LeftChild;
            else
            {
                Iterator->LeftChild         = InsertedNodePointer;
                InsertedNodePointer->Parent = Iterator;
                ++BSTP->NodeNumber;
                return OK;
            }
        }
        else
        {
            if (Iterator->RightChild) Iterator = Iterator->RightChild;
            else
            {
                Iterator->RightChild        = InsertedNodePointer;
                InsertedNodePointer->Parent = Iterator;
                ++BSTP->NodeNumber;
                return OK;
            }
        }
    }
}

Status DeleteBST(BinaryTreePointer BSTP, void* Data, CompareFunction Comparator)
{
    if (!BSTP) return ERROR;
    if (DeleteBST_Kernel(&BSTP->RootNodePointer, Data, Comparator))
    {
        --BSTP->NodeNumber;
        return OK;
    }
    else return ERROR;
}

Status DeleteBST_Kernel(BinaryTreeNodePointer* RootNodePP, void* Data, CompareFunction Comparator)
{
    if (!*RootNodePP) return ERROR;
    int Delta = Comparator(Data, (*RootNodePP)->Data);
    if (Delta < 0) { return DeleteBST_Kernel(&(*RootNodePP)->LeftChild, Data, Comparator); }
    else if (Delta > 0) { return DeleteBST_Kernel(&(*RootNodePP)->RightChild, Data, Comparator); }
    else                                   //找到节点
    {
        if (!(*RootNodePP)->RightChild)    //此节点无右节点
        {
            BinaryTreeNodePointer Temp = *RootNodePP;
            if ((*RootNodePP)->LeftChild) (*RootNodePP)->LeftChild->Parent = (*RootNodePP)->Parent;
            *RootNodePP = (*RootNodePP)->LeftChild;
            free(Temp->Data);
            free(Temp);
        }
        else if (!(*RootNodePP)->LeftChild)    //此节点无左节点
        {
            BinaryTreeNodePointer Temp = *RootNodePP;
            if ((*RootNodePP)->RightChild) (*RootNodePP)->RightChild->Parent = (*RootNodePP)->Parent;
            *RootNodePP = (*RootNodePP)->RightChild;
            free(Temp->Data);
            free(Temp);
        }
        else
        {
            BinaryTreeNodePointer s = (*RootNodePP)->LeftChild;
            while (s->RightChild) { s = s->RightChild; }
            void* PrecursorData = (void*)malloc((*RootNodePP)->DataSize);
            memcpy(PrecursorData, s->Data, (*RootNodePP)->DataSize);
            DeleteBST_Kernel(RootNodePP, s->Data, Comparator);
            memcpy((*RootNodePP)->Data, PrecursorData, (*RootNodePP)->DataSize);
            free(PrecursorData);
        }
    }
    return OK;
}