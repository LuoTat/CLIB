#pragma once
#include "..\Predefined\Predefined.h"
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int    Weight;
    int    Parent;
    int    LeftChild;
    int    RightChild;
    void*  Data;
    size_t DataSize;
} HuffmanTreeNode, *HuffmanTree;

typedef char** HuffmanCode;

void HuffmanCoding(HuffmanTree* HTree, HuffmanCode* HCode, int* WeightArray, void** DataArray, size_t DataSize, int Length);
void Select(HuffmanTree HTree, int Index, int* s1, int* s2);