#include "HuffmanTree.h"

void HuffmanCoding(HuffmanTree* HTreeP, HuffmanCode* HCodeP, int* WeightArray, void** DataArray, size_t DataSize, int Length)
{
    if (Length < 1) return ERROR;
    int HTreeNodeNumber = 2 * Length;
    if (!((*HTreeP) = (HuffmanTree)malloc(HTreeNodeNumber * sizeof(HuffmanTreeNode)))) return OVERFLOW;
    for (int i = 0; i <= HTreeNodeNumber; ++i)
    {
        (*HTreeP)[i].Data       = NULL;
        (*HTreeP)[i].DataSize   = 0;
        (*HTreeP)[i].LeftChild  = 0;
        (*HTreeP)[i].RightChild = 0;
        (*HTreeP)[i].Parent     = 0;
        (*HTreeP)[i].Weight     = 0;
    }
    for (int i = 1; i <= Length; ++i)
    {
        (*HTreeP)[i].Data       = (void*)malloc(DataSize);
        //memcpy((*HTreeP)[i].Data, DataArray[Length -1], DataSize);
        (*HTreeP)[i].DataSize   = DataSize;
        (*HTreeP)[i].LeftChild  = 0;
        (*HTreeP)[i].RightChild = 0;
        (*HTreeP)[i].Parent     = 0;
        (*HTreeP)[i].Weight     = WeightArray[i - 1];
    }
    for (int i = Length + 1; i < HTreeNodeNumber; ++i)
    {
        int s1;
        int s2;
        Select(*HTreeP, i - 1, &s1, &s2);
        (*HTreeP)[s1].Parent    = i;
        (*HTreeP)[s2].Parent    = i;
        (*HTreeP)[i].LeftChild  = s1;
        (*HTreeP)[i].RightChild = s2;
        (*HTreeP)[i].Weight     = (*HTreeP)[s1].Weight + (*HTreeP)[s2].Weight;
    }
    (*HCodeP) = (HuffmanCode)malloc((Length + 1) * sizeof(char*));
    int Start = Length - 1;
    for (int i = 1; i <= Length; ++i)
    {
        char* TempString       = (char*)malloc(Length * sizeof(char));
        TempString[Length - 1] = '\0';
        for (int c = i, f = (*HTreeP)[i].Parent; f != 0; c = f, f = (*HTreeP)[f].Parent)
        {
            if ((*HTreeP)[f].LeftChild == c) TempString[--Start] = '0';
            else TempString[--Start] = '1';
        }
        (*HCodeP)[i] = (char*)malloc((Length - Start) * sizeof(char));
        strcpy_s((*HCodeP)[i], Length - Start, TempString + Start);
    }
}

void Select(HuffmanTree HTree, int Index, int* s1, int* s2)
{
    int a = INT_MAX;
    int b = INT_MAX;
    for (int i = 1; i <= Index; ++i)
    {
        if (HTree[i].Parent == 0)
        {
            if (HTree[i].Weight <= (a >= b) ? a : b)
            {
                if (a >= b)
                {
                    a   = HTree[i].Weight;
                    *s1 = i;
                }
                else
                {
                    b   = HTree[i].Weight;
                    *s2 = i;
                }
            }
        }
    }
}
