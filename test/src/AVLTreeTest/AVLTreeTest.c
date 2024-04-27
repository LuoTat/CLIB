#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "LTT_AVLTree.h"
#include "LTT_BinaryTree.h"

#define Mode       2
#define NodeNum    10
#define RandomSend (unsigned)time(NULL)

void SWAP(int* a, int* b)
{
    int temp = *a;
    *a       = *b;
    *b       = temp;
}

void GetTheRanddomNonRepeatingArray(int* Array, int NumOfElements)
{
    for (int i = 0; i < NumOfElements; ++i) Array[i] = i;
    for (int i = 0; i <= NodeNum - 2; ++i) { SWAP(&Array[i], &Array[i + (rand() % (NodeNum - i))]); }
}

Status PRINT(void* Data)
{
    BinaryTreeNode* Node = (BinaryTreeNode*)Data;
    printf("%d ", *(int*)Node->Data);
    return OK;
}

int cmp(const void* const Data1, const void* const Data2) { return *(int*)Data1 - *(int*)Data2; }

int main()
{
    srand(RandomSend);
    int* Temp = (int*)malloc(NodeNum * sizeof(int));

#if Mode == 1      // 普通随机数组
    for (int i = 0; i < NodeNum; ++i) Temp[i] = rand() % NodeNum;
#elif Mode == 2    // 无相同元素的乱序数组
    GetTheRanddomNonRepeatingArray(Temp, NodeNum);
#endif


    int** intArray = (int**)malloc(NodeNum * sizeof(int*));
    for (int i = 0; i < NodeNum; i++)
    {
        intArray[i]  = (int*)malloc(sizeof(int));
        *intArray[i] = Temp[i];
    }

    AVLTree* AVL_Tree = LTT_AVLTree_New(sizeof(int), cmp);

    clock_t Start, End;
    Start = clock();
    for (int i = 0; i < NodeNum; i++) { LTT_AVLTree_InsertData(AVL_Tree, intArray[i]); }
    End = clock();
    printf("Insert Time: %f ms\n", (double)(End - Start) / CLOCKS_PER_SEC * 1000);

    int NodeNumber = LTT_BiTreeNode_GetNodeNumber(AVL_Tree->BiTree.Root);
    printf("NodeNumber: %d\n", NodeNumber);

    int LeafNum = LTT_BiTreeNode_GetLeafNumber(AVL_Tree->BiTree.Root);
    printf("LeafNumber: %d\n", LeafNum);

    int Depth = LTT_BiTreeNode_GetDepth(AVL_Tree->BiTree.Root);
    printf("Depth: %d\n", Depth);

    BinaryTreeNode* findnode = LTT_BiTreeNode_SearchNode(AVL_Tree->BiTree.Root, intArray[rand() % NodeNum], AVL_Tree->Comparator);
    printf("find %d\n", *(int*)findnode->Data);

    LTT_AVLTree_DeleteData(AVL_Tree, intArray[rand() % NodeNum]);
    NodeNumber = LTT_BiTreeNode_GetNodeNumber(AVL_Tree->BiTree.Root);
    printf("NodeNumber: %d\n", NodeNumber);

    //LTT_BSTree_Destroy(&AVL_Tree);
    printf("Test Over!\n");
}