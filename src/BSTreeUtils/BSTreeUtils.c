#include "BSTreeUtils.h"

BinaryTreeNode* LTT_BSTreeNode_SearchNode(BinaryTreeNode* Root, const void* const Data, const CompareFunction Comparator)
{
    while (Root != NODE_NULL && Comparator(Data, Root->Data) != 0)
    {
        if (Comparator(Data, Root->Data) < 0) Root = Root->LeftChild;
        else Root = Root->RightChild;
    }
    return Root;
}

BinaryTreeNode* LTT_BSTreeNode_GetMaxNode(BinaryTreeNode* Root)
{
    while (Root->RightChild != NODE_NULL) Root = Root->RightChild;
    return Root;
}

BinaryTreeNode* LTT_BSTreeNode_GetMinNode(BinaryTreeNode* Root)
{
    while (Root->LeftChild != NODE_NULL) Root = Root->LeftChild;
    return Root;
}

BinaryTreeNode* LTT_BSTreeNode_GetPredecessorNode(BinaryTreeNode* Root)
{
    if (Root->LeftChild != NODE_NULL) return LTT_BSTreeNode_GetMaxNode(Root->LeftChild);
    BinaryTreeNode* Temp = Root->Parent;
    while (Temp != NODE_NULL && Root == Temp->LeftChild)
    {
        Root = Temp;
        Temp = Temp->Parent;
    }
    return Temp;
}

BinaryTreeNode* LTT_BSTreeNode_GetSuccessorNode(BinaryTreeNode* Root)
{
    if (Root->RightChild != NODE_NULL) return LTT_BSTreeNode_GetMinNode(Root->RightChild);
    BinaryTreeNode* Temp = Root->Parent;
    while (Temp != NODE_NULL && Root == Temp->RightChild)
    {
        Root = Temp;
        Temp = Temp->Parent;
    }
    return Temp;
}