#include "BinaryTree.h"
#include <float.h>
#include <stdio.h>
#include <stdlib.h>

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

Status LTT_BiTree_InsertRoot(BinaryTree* const BiTree, BinaryTreeNode* const Inserted_Node)
{
    if (BiTree->Root != NODE_NULL) return ERROR;    //如果根节点不为空，返回ERROR
    BiTree->Root = Inserted_Node;
    return OK;
}

Status LTT_BiTree_InsertNode(BinaryTreeNode* const BeInserted_Node, BinaryTreeNode* const Inserted_Node, const bool LeftChild)
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

Status LTT_BiTree_DeleteNode(BinaryTreeNode** Deleted_Node)
{
    LTT_BiTreeNode_DeleteNode(Deleted_Node);
    return OK;
}

void LTT_BiTree_Clear(BinaryTree* const BiTree) { LTT_BiTreeNode_DeleteSubTree(&(BiTree->Root)); }

void LTT_BiTree_Destroy(BinaryTree** BiTree)
{
    LTT_BiTree_Clear(*BiTree);
    free(*BiTree);
    *BiTree = NULL;
}