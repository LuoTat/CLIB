#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "LTT_BSTree.h"
#include "LTT_BinaryTree.h"

#define Mode       2
#define NodeNum    5
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
    /*
                 5
              /     \
              4     7
            /   \  /  \
            1   3  6   9
    */
    int int1                = 5;
    int int2                = 4;
    int int3                = 7;
    int int4                = 1;
    int int5                = 3;
    int int6                = 6;
    int int7                = 9;

    BSTree*         BS_Tree = LTT_BSTree_New(sizeof(int), cmp);
    BinaryTreeNode* Node1   = LTT_BiTreeNode_MakeNode(&int1);
    BinaryTreeNode* Node2   = LTT_BiTreeNode_MakeNode(&int2);
    BinaryTreeNode* Node3   = LTT_BiTreeNode_MakeNode(&int3);
    BinaryTreeNode* Node4   = LTT_BiTreeNode_MakeNode(&int4);
    BinaryTreeNode* Node5   = LTT_BiTreeNode_MakeNode(&int5);
    BinaryTreeNode* Node6   = LTT_BiTreeNode_MakeNode(&int6);
    BinaryTreeNode* Node7   = LTT_BiTreeNode_MakeNode(&int7);
    LTT_BiTree_InsertRoot((BinaryTree*)BS_Tree, Node1);
    LTT_BiTree_InsertNode(BS_Tree->BiTree.Root, Node2, 1);
    LTT_BiTree_InsertNode(BS_Tree->BiTree.Root, Node3, 0);
    LTT_BiTree_InsertNode(Node2, Node4, 1);
    LTT_BiTree_InsertNode(Node2, Node5, 0);
    LTT_BiTree_InsertNode(Node3, Node6, 1);
    LTT_BiTree_InsertNode(Node3, Node7, 0);

    //LTT_BiTree_DeleteNode(&BiTree->Root);
    LTT_BSTree_DeleteData(BS_Tree, &int2);

    printf("PreOrder: \n");
    LTT_BiTreeNode_PreOrder_Traverse_Recursive(BS_Tree->BiTree.Root, PRINT);
    printf("\n");
    LTT_BiTreeNode_PreOrder_Traverse_Stack(BS_Tree->BiTree.Root, PRINT);
    printf("\n");

    printf("InOrder: \n");
    LTT_BiTreeNode_InOrder_Traverse_Recursive(BS_Tree->BiTree.Root, PRINT);
    printf("\n");
    LTT_BiTreeNode_InOrder_Traverse_Stack(BS_Tree->BiTree.Root, PRINT);
    printf("\n");

    printf("PostOrder: \n");
    LTT_BiTreeNode_PostOrder_Traverse_Recursive(BS_Tree->BiTree.Root, PRINT);
    printf("\n");
    LTT_BiTreeNode_PostOrder_Traverse_Stack(BS_Tree->BiTree.Root, PRINT);
    printf("\n");

    printf("LevelOrder: \n");
    LTT_BiTreeNode_LevelOrder_Traverse_Queue(BS_Tree->BiTree.Root, PRINT);
    printf("\n");

    int NodeNumber = LTT_BiTreeNode_GetNodeNumber(BS_Tree->BiTree.Root);
    printf("NodeNumber: %d\n", NodeNumber);

    int LeafNum = LTT_BiTreeNode_GetLeafNumber(BS_Tree->BiTree.Root);
    printf("LeafNumber: %d\n", LeafNum);

    int Depth = LTT_BiTreeNode_GetDepth(BS_Tree->BiTree.Root);
    printf("Depth: %d\n", Depth);

    LTT_BSTree_Destroy(&BS_Tree);
}

// int main()
// {
// //     srand(RandomSend);
// //     int* Temp = (int*)malloc(NodeNum * sizeof(int));

// // #if Mode == 1      // 普通随机数组
// //     for (int i = 0; i < NodeNum; ++i) Temp[i] = rand() % NodeNum;
// // #elif Mode == 2    // 无相同元素的乱序数组
// //     GetTheRanddomNonRepeatingArray(Temp, NodeNum);
// // #endif


// //     int** intArray = (int**)malloc(NodeNum * sizeof(int*));
// //     for (int i = 0; i < NodeNum; i++)
// //     {
// //         intArray[i]  = (int*)malloc(sizeof(int));
// //         *intArray[i] = Temp[i];
// //     }

// //     BSTree* BS_Tree = LTT_BSTree_New(sizeof(int), cmp);

// //     clock_t Start, End;
// //     Start = clock();
// //     for (int i = 0; i < NodeNum; i++) { LTT_BSTree_InsertData(BS_Tree, intArray[i]); }
// //     End = clock();
// //     printf("Insert Time: %f ms\n", (double)(End - Start) / CLOCKS_PER_SEC * 1000);

// //     int NodeNumber = LTT_BiTreeNode_GetNodeNumber(BS_Tree->BiTree.Root);
// //     printf("NodeNumber: %d\n", NodeNumber);

// //     int LeafNum = LTT_BiTreeNode_GetLeafNumber(BS_Tree->BiTree.Root);
// //     printf("LeafNumber: %d\n", LeafNum);

// //     int Depth = LTT_BiTreeNode_GetDepth(BS_Tree->BiTree.Root);
// //     printf("Depth: %d\n", Depth);

// //     BinaryTreeNode* findnode = LTT_BSTreeNode_SearchData(BS_Tree, intArray[rand() % NodeNum]);
// //     printf("find %d\n", *(int*)findnode->Data);

// //     LTT_BSTree_DeleteData(BS_Tree, intArray[rand() % NodeNum]);
// //     NodeNumber = LTT_BiTreeNode_GetNodeNumber(BS_Tree->BiTree.Root);
// //     printf("NodeNumber: %d\n", NodeNumber);

// //     LTT_BSTree_Destroy(&BS_Tree);

// //     printf("Optimal_BST\n");

// //     double* random_num = (double*)malloc((NodeNum * 2 + 1) * sizeof(double));
// //     double  sum        = 0;
// //     for (int i = 0; i < NodeNum * 2 + 1; i++)
// //     {
// //         random_num[i]  = (double)rand() / RAND_MAX;
// //         sum           += random_num[i];
// //     }
// //     for (int i = 0; i < NodeNum * 2 + 1; i++) random_num[i] /= sum;

// //     int     count = 0;
// //     double* P     = (double*)malloc((NodeNum + 1) * sizeof(double));
// //     double* Q     = (double*)malloc((NodeNum + 1) * sizeof(double));
// //     for (int i = 1; i < NodeNum + 1; i++)
// //     {
// //         P[i] = random_num[count++];
// //         Q[i] = random_num[count++];
// //     }
// //     Q[0]        = random_num[count];

// //     int* Number = (int*)malloc((NodeNum + 1) * sizeof(int));
// //     for (int i = 0; i < NodeNum + 1; i++) Number[i] = i;
// //     int** Data = (int**)malloc((NodeNum + 1) * sizeof(int*));
// //     for (int i = 0; i < NodeNum + 1; i++) Data[i] = &Number[i];

// //     Start       = clock();
// //     BSTree* OPT = LTT_BSTree_Build_Optimal_BST((void**)Data, sizeof(int), P, Q, NodeNum);
// //     End         = clock();
// //     printf("Optimal BST Time: %f ms\n", (double)(End - Start) / CLOCKS_PER_SEC * 1000);

// //     LTT_BiTreeNode_PreOrder_Traverse_Recursive(OPT->BiTree.Root, PRINT);
// //     printf("\n");
// //     LTT_BiTreeNode_InOrder_Traverse_Recursive(OPT->BiTree.Root, PRINT);
// //     printf("\n");
// //     LTT_BiTreeNode_PostOrder_Traverse_Recursive(OPT->BiTree.Root, PRINT);
// //     printf("\n");
// //     LTT_BSTree_Destroy(&OPT);
// //     printf("Test Over!\n");
// }