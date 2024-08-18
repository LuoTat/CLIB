#include "BinaryTree.h"
#include "BinaryTreeUtils.h"
#include <stdio.h>
#include <stdlib.h>

BinaryTreeNode Node_NULL = {NULL, 0, NULL};

BinaryTree* LTT_BiTree_New(const size_t DataSize)
{
    BinaryTree* BiTree = (BinaryTree*)malloc(sizeof(BinaryTree));
    if (BiTree == NULL)
    {
        printf("BiTree制作失败!\n");
        return NULL;
    }

    //初始化根节点
    BiTree->Root     = NODE_NULL;
    BiTree->DataSize = DataSize;
    return BiTree;
}

BinaryTreeNode* LTT_BiTree_MakeNode(void* const Data)
{
    BinaryTreeNode* Node = (BinaryTreeNode*)malloc(sizeof(BinaryTreeNode));
    if (Node == NULL)
    {
        printf("节点制作失败!\n");
        return NULL;
    }
    Node->Data       = Data;
    Node->LeftChild  = NODE_NULL;
    Node->RightChild = NODE_NULL;
    Node->Parent     = NODE_NULL;
    return Node;
}

Status LTT_BiTree_Insert(BinaryTree* const BiTree, BinaryTreeNode* const BeInserted_Node, void* const Inserted_Data, const bool LeftChild)
{
    BinaryTreeNode* Inserted_Node = LTT_BiTree_MakeNode(Inserted_Data);
    if (Inserted_Node == NULL) goto FREE;
    // 这里必须用NULL来判断
    if (BeInserted_Node == NULL)                                   // 如果BeInserted_Node为空，那么插入的节点就是根节点
    {
        BiTree->Root = Inserted_Node;
        return OK;
    }
    if (LeftChild)                                                 // 把Inserted_Node插入到BeInserted_Node的左子树或者右子树
    {
        if (BeInserted_Node->LeftChild != NODE_NULL) goto FREE;    //如果左子树不为空，返回ERROR
        BeInserted_Node->LeftChild = Inserted_Node;
        Inserted_Node->Parent      = BeInserted_Node;
        return OK;
    }
    else
    {
        if (BeInserted_Node->RightChild != NODE_NULL) goto FREE;    //如果右子树不为空，返回ERROR
        BeInserted_Node->RightChild = Inserted_Node;
        Inserted_Node->Parent       = BeInserted_Node;
        return OK;
    }
FREE:
    free(Inserted_Node);
    return ERROR;
}

static BinaryTreeNode* LTT_BiTree_GetRightMostNode(BinaryTreeNode* Root)
{
    while (Root->RightChild != NODE_NULL) Root = Root->RightChild;
    return Root;
}

Status LTT_BiTree_Delete(BinaryTree* const BiTree, BinaryTreeNode* BeDeleted_Node)
{
    bool isRootNode       = BeDeleted_Node == BiTree->Root;
    bool isLeftChild      = BeDeleted_Node->Parent->LeftChild == BeDeleted_Node;
    bool LeftChildIsNull  = BeDeleted_Node->LeftChild == NODE_NULL;
    bool RightChildIsNull = BeDeleted_Node->RightChild == NODE_NULL;

    // 如果BeDeleted_Node是根节点
    if (isRootNode)
    {
        // 如果左右子树都为空
        if (LeftChildIsNull && RightChildIsNull) { BiTree->Root = NODE_NULL; }
        // 如果左子树为空
        else if (LeftChildIsNull)
        {
            BiTree->Root                       = BeDeleted_Node->RightChild;
            BeDeleted_Node->RightChild->Parent = NODE_NULL;
        }
        // 如果右子树为空
        else if (RightChildIsNull)
        {
            BiTree->Root                      = BeDeleted_Node->LeftChild;
            BeDeleted_Node->LeftChild->Parent = NODE_NULL;
        }
        // 如果左右子树都不为空
        else
        {
            BinaryTreeNode* RightMostNode      = LTT_BiTree_GetRightMostNode(BeDeleted_Node->LeftChild);
            BiTree->Root                       = BeDeleted_Node->LeftChild;
            BeDeleted_Node->LeftChild->Parent  = NODE_NULL;
            RightMostNode->RightChild          = BeDeleted_Node->RightChild;
            BeDeleted_Node->RightChild->Parent = RightMostNode;
        }
    }
    // 如果BeDeleted_Node不是根节点
    else
    {
        // 如果左右子树都为空
        if (LeftChildIsNull && RightChildIsNull)
        {
            if (isLeftChild) { BeDeleted_Node->Parent->LeftChild = NODE_NULL; }
            else { BeDeleted_Node->Parent->RightChild = NODE_NULL; }
        }
        // 如果左子树为空
        else if (LeftChildIsNull)
        {
            if (isLeftChild)
            {
                BeDeleted_Node->Parent->LeftChild  = BeDeleted_Node->RightChild;
                BeDeleted_Node->RightChild->Parent = BeDeleted_Node->Parent;
            }
            else
            {
                BeDeleted_Node->Parent->RightChild = BeDeleted_Node->RightChild;
                BeDeleted_Node->RightChild->Parent = BeDeleted_Node->Parent;
            }
        }
        // 如果右子树为空
        else if (RightChildIsNull)
        {
            if (isLeftChild)
            {
                BeDeleted_Node->Parent->LeftChild = BeDeleted_Node->LeftChild;
                BeDeleted_Node->LeftChild->Parent = BeDeleted_Node->Parent;
            }
            else
            {
                BeDeleted_Node->Parent->RightChild = BeDeleted_Node->LeftChild;
                BeDeleted_Node->LeftChild->Parent  = BeDeleted_Node->Parent;
            }
        }
        // 如果左右子树都不为空
        else
        {
            BinaryTreeNode* RightMostNode = LTT_BiTree_GetRightMostNode(BeDeleted_Node->LeftChild);
            if (isLeftChild)
            {
                BeDeleted_Node->Parent->LeftChild  = BeDeleted_Node->LeftChild;
                BeDeleted_Node->LeftChild->Parent  = BeDeleted_Node->Parent;
                RightMostNode->RightChild          = BeDeleted_Node->RightChild;
                BeDeleted_Node->RightChild->Parent = RightMostNode;
            }
            else
            {
                BeDeleted_Node->Parent->RightChild = BeDeleted_Node->LeftChild;
                BeDeleted_Node->LeftChild->Parent  = BeDeleted_Node->Parent;
                RightMostNode->RightChild          = BeDeleted_Node->RightChild;
                BeDeleted_Node->RightChild->Parent = RightMostNode;
            }
        }
    }
    free(BeDeleted_Node);
    return OK;
}

void LTT_BiTree_Clear(BinaryTree* const BiTree) { LTT_BiTreeUtils_DeleteSubTree(&(BiTree->Root)); }

void LTT_BiTree_DestroyNode(BinaryTreeNode** Node)
{
    free(*Node);
    *Node = NODE_NULL;
}

void LTT_BiTree_Destroy(BinaryTree** BiTree)
{
    LTT_BiTree_Clear(*BiTree);
    free(*BiTree);
    *BiTree = NULL;
}