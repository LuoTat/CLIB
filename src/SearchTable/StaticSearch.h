#pragma once
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "..\Predefined\Predefined.h"
#include "..\Tree\BinaryTree.h"

typedef struct
{
    char Data;
    int  Weight;
} Data_Weight;

void*              BinarySearch(const void* Key, const void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator);
void*              InterpolationSearch(const void* Key, const void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator);
Status             CreateNearlyOptimalSearchTree(BinaryTreePointer BiTree, const void* Data, size_t DataNum, size_t DataSize, int* Weight);
Status             CreateNearlyOptimalSearchTree_Kernel(BinaryTreeNodePointer* BiTreeNodePP, BinaryTreeNodePointer Parent, const void* Data, size_t DataSize, int* SW, int Low, int High);
Status             CreateLTTSearchTree(BinaryTreePointer BiTree, const void* Data, size_t DataNum, size_t DataSize, int* Weight);
Status             CreateLTTSearchTree_Kernel(BinaryTreeNodePointer* BiTreeNodePP, BinaryTreeNodePointer Parent, const void* Data, size_t DataSize, int* Weight, int Low, int High);
unsigned long long GetTheASLOfSearchTree(BinaryTree BiTree);
