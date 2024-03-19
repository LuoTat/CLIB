#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../LinearList/SqQueue.h"
#include "..\Predefined\Predefined.h"


#define LH +1
#define EH 0
#define RH -1

typedef struct AVLTreeNode
{
    void*               Data;
    size_t              DataSize;
    int                 BalanceFactor;
    struct AVLTreeNode* LeftChild;
    struct AVLTreeNode* RightChild;
} AVLTreeNode, *AVLTreeNodePointer;

typedef struct
{
    AVLTreeNodePointer RootNode;
} AVLTree, *AVLTreePointer;

Status InitAVLTree(AVLTreePointer AVLTreeP);
Status InsertAVLTree(AVLTreePointer AVLTreeP, void* Data, size_t DataSize, CompareFunction Comparator);
Status InsertAVLTree_Kernel(AVLTreeNodePointer* RootNodePP, void* Data, size_t DataSize, bool* TallerP, CompareFunction Comparator);
Status DeleteAVLTree(AVLTreePointer AVLTreeP, void* Data, CompareFunction Comparator);
Status DeleteAVLTree_Kernel(AVLTreeNodePointer* RootNodePP, void* Data, bool* LowerP, CompareFunction Comparator);
void*  SearchAVLTree(AVLTreePointer AVLTreeP, void* Data, CompareFunction Comparator);
Status LevelOrderTraverse_AVL(AVLTreeNodePointer RootNodeP, Status (*Visit)(AVLTreeNodePointer));
void   LeftRotate(AVLTreeNodePointer* RootNodePP);
void   RightRotate(AVLTreeNodePointer* RootNodePP);
void   LeftBalance_Insert(AVLTreeNodePointer* RootNodePP);
void   RightBalance_Insert(AVLTreeNodePointer* RootNodePP);
void   LeftBalance_Delete(AVLTreeNodePointer* RootNodePP, bool* LowerP);
void   RightBalance_Delete(AVLTreeNodePointer* RootNodePP, bool* LowerP);
void*  GetThePrecursorData(AVLTreeNodePointer RootNodeP);