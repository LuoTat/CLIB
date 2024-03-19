#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\LinearList\SqQueue.h"
#include "..\LinearList\SqStack.h"
#include "..\Predefined\Predefined.h"

typedef struct BinaryTreeNode
{
    void*                  Data;
    size_t                 DataSize;
    struct BinaryTreeNode* LeftChild;
    struct BinaryTreeNode* RightChild;
    struct BinaryTreeNode* Parent;
} BinaryTreeNode, *BinaryTreeNodePointer;

typedef struct
{
    BinaryTreeNode* RootNodePointer;
    int             NodeNumber;
} BinaryTree, *BinaryTreePointer;

Status                InitBiTree(BinaryTreePointer BiTreePointer);
Status                CreateBiTree_PreandInOrder(BinaryTree BiTree, char* PreOrder, char* InOrder);
void                  CreateBiTree_PreandInOrder_Kernel(BinaryTreeNodePointer* BiTreeNodePP, BinaryTreeNodePointer ParentBiTreeNodePointer, char* PreOrder, char* InOrder, int PStart, int IStart, int Length);
Status                MakeBinaryTreeNode(BinaryTreeNodePointer BiTreeNodePointer, void* Data, size_t DataSize);
bool                  BiTreeNodeExist(BinaryTreePointer BiTreePointer, BinaryTreeNodePointer BiTreeNodePointer);
Status                DestroyBiTree(BinaryTreePointer BiTreePointer);
Status                ClearBiTree(BinaryTreePointer BiTreePointer);
bool                  BiTreeEmpty(BinaryTree BiTree);
int                   BiTreeNodeNumber(BinaryTreeNodePointer BiTreeNodePointer);
int                   BiTreeNodeLeafNumber(BinaryTreeNodePointer BiTreeNodePointer);
int                   BiTreeNodeDepth(BinaryTreeNodePointer BiTreeNodePointer);
BinaryTreeNodePointer Root(BinaryTree BiTree);
void*                 Value(BinaryTreePointer BiTreePointer, BinaryTreeNodePointer BiTreeNodePointer);
Status                Assign(BinaryTreePointer BiTreePointer, BinaryTreeNodePointer BiTreeNodePointer, void* Data);
BinaryTreeNodePointer Parent(BinaryTreePointer BiTreePointer, BinaryTreeNodePointer BiTreeNodePointer);
BinaryTreeNodePointer LeftChild(BinaryTreePointer BiTreePointer, BinaryTreeNodePointer BiTreeNodePointer);
BinaryTreeNodePointer RightChild(BinaryTreePointer BiTreePointer, BinaryTreeNodePointer BiTreeNodePointer);
BinaryTreeNodePointer LeftSibling(BinaryTreePointer BiTreePointer, BinaryTreeNodePointer BiTreeNodePointer);
BinaryTreeNodePointer RightSibling(BinaryTreePointer BiTreePointer, BinaryTreeNodePointer BiTreeNodePointer);
Status                InsertBinaryTreeNode(BinaryTreePointer BiTreePointer, BinaryTreeNodePointer InsertedPositionNodePointer, bool LeftChild, BinaryTreeNodePointer InsertedBiTreeNode);
Status                DeleteChild(BinaryTreePointer BiTreePointer, BinaryTreeNodePointer DeletedBiTreeNodePointer, bool LeftChild);
Status                PreOrderTraverse_Recursive(BinaryTreeNodePointer BiTreeNodePointer, Status (*Visit)(BinaryTreeNodePointer));
Status                PreOrderTraverse_Stack(BinaryTreeNodePointer BiTreeNodePointer, Status (*Visit)(BinaryTreeNodePointer));
Status                InOrderTraverse_Recursive(BinaryTreeNodePointer BiTreeNodePointer, Status (*Visit)(BinaryTreeNodePointer));
Status                InOrderTraverse_Stack(BinaryTreeNodePointer BiTreeNodePointer, Status (*Visit)(BinaryTreeNodePointer));
Status                PostOrderTraverse_Recursive(BinaryTreeNodePointer BiTreeNodePointer, Status (*Visit)(BinaryTreeNodePointer));
Status                PostOrderTraverse_Stack(BinaryTreeNodePointer BiTreeNodePointer, Status (*Visit)(BinaryTreeNodePointer));
Status                LevelOrderTraverse(BinaryTreeNodePointer BiTreeNodePointer, Status (*Visit)(BinaryTreeNodePointer));
Status                ClearBiTreeVisit(BinaryTreeNodePointer BiTreeNodePointer);