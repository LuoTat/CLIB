#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "..\Predefined\Predefined.h"
#include "..\Tree\BinaryTree.h"

Status InitBST(BinaryTreePointer BSTP);
void*  SearchBST(BinaryTreePointer BSTP, void* Data, CompareFunction Comparator);
Status InsertBST(BinaryTreePointer BSTP, void* Data, size_t DataSize, CompareFunction Comparator);
Status DeleteBST(BinaryTreePointer BSTP, void* Data, CompareFunction Comparator);
Status DeleteBST_Kernel(BinaryTreeNodePointer* BiTreeNodePP, void* Data, CompareFunction Comparator);