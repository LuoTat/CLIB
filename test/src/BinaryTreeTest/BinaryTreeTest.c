#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "LTT_BinaryTree.h"

#define Mode       2
#define NodeNum    1000
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

    /*
                 1
              /     \
              2     3
            /   \  /  \
            4   5  6   7
    */

    int** intArray = (int**)malloc(NodeNum * sizeof(int*));
    for (int i = 0; i < NodeNum; i++)
    {
        intArray[i]  = (int*)malloc(sizeof(int));
        *intArray[i] = Temp[i];
    }

    BinaryTree* BiTree = LTT_BiTree_New(intArray[0], sizeof(int));
    // BinaryTreeNode* Node2  = LTT_BiTree_Make_Node(int2, sizeof(int));
    // BinaryTreeNode* Node3  = LTT_BiTree_Make_Node(int3, sizeof(int));
    // BinaryTreeNode* Node4  = LTT_BiTree_Make_Node(int4, sizeof(int));
    // BinaryTreeNode* Node5  = LTT_BiTree_Make_Node(int5, sizeof(int));
    // BinaryTreeNode* Node6  = LTT_BiTree_Make_Node(int6, sizeof(int));
    // BinaryTreeNode* Node7  = LTT_BiTree_Make_Node(int7, sizeof(int));
    // LTT_BiTree_Insert_Node(BiTree->Root, Node2, 1);
    // LTT_BiTree_Insert_Node(BiTree->Root, Node3, 0);
    // LTT_BiTree_Insert_Node(Node2, Node4, 1);
    // LTT_BiTree_Insert_Node(Node2, Node5, 0);
    // LTT_BiTree_Insert_Node(Node3, Node6, 1);
    // LTT_BiTree_Insert_Node(Node3, Node7, 0);

    // LTT_BiTree_Insert_Data(BiTree->Root, int2, sizeof(int), cmp);
    // LTT_BiTree_Insert_Data(BiTree->Root, int3, sizeof(int), cmp);
    // LTT_BiTree_Insert_Data(BiTree->Root, int4, sizeof(int), cmp);
    // LTT_BiTree_Insert_Data(BiTree->Root, int5, sizeof(int), cmp);
    // LTT_BiTree_Insert_Data(BiTree->Root, int6, sizeof(int), cmp);
    // LTT_BiTree_Insert_Data(BiTree->Root, int7, sizeof(int), cmp);

    clock_t Start, End;
    Start = clock();
    for (int i = 1; i < NodeNum; i++) { LTT_BiTree_Insert_Data(BiTree->Root, intArray[i], sizeof(int), cmp); }
    End = clock();
    printf("Insert Time: %f ms\n", (double)(End - Start) / CLOCKS_PER_SEC * 1000);

    // printf("PreOrder: \n");
    // LTT_BiTree_PreOrder_Traverse_Recursive(BiTree->Root, PRINT);
    // printf("\n");
    // LTT_BiTree_PreOrder_Traverse_Stack(BiTree->Root, PRINT);
    // printf("\n");
    //
    // printf("InOrder: \n");
    // LTT_BiTree_InOrder_Traverse_Recursive(BiTree->Root, PRINT);
    // printf("\n");
    // LTT_BiTree_InOrder_Traverse_Stack(BiTree->Root, PRINT);
    // printf("\n");
    //
    // printf("PostOrder: \n");
    // LTT_BiTree_PostOrder_Traverse_Recursive(BiTree->Root, PRINT);
    // printf("\n");
    // LTT_BiTree_PostOrder_Traverse_Stack(BiTree->Root, PRINT);
    // printf("\n");
    //
    //
    // printf("LevelOrder: \n");
    // LTT_BiTree_LevelOrder_Traverse_Queue(BiTree->Root, PRINT);
    // printf("\n");

    int NodeNumber = LTT_BiTree_Get_NodeNumber(BiTree->Root);
    printf("NodeNumber: %d\n", NodeNumber);

    int LeafNum = LTT_BiTree_Get_LeafNumber(BiTree->Root);
    printf("LeafNumber: %d\n", LeafNum);

    int Depth = LTT_BiTree_Get_Depth(BiTree->Root);
    printf("Depth: %d\n", Depth);

    BinaryTreeNode* findnode = LTT_BiTree_Search_Data(BiTree->Root, intArray[rand() % NodeNum], cmp);
    printf("find %d\n", *(int*)findnode->Data);


    //LTT_Bitree_Destroy(BiTree);

    printf("Optimal_BST\n");

    double* random_num = (double*)malloc((NodeNum * 2 + 1) * sizeof(double));
    double  sum        = 0;
    for (int i = 0; i < NodeNum * 2 + 1; i++)
    {
        random_num[i]  = (double)rand() / RAND_MAX;
        sum           += random_num[i];
    }
    for (int i = 0; i < NodeNum * 2 + 1; i++) random_num[i] /= sum;

    int     count = 0;
    double* P     = (double*)malloc((NodeNum + 1) * sizeof(double));
    double* Q     = (double*)malloc((NodeNum + 1) * sizeof(double));
    for (int i = 1; i < NodeNum + 1; i++)
    {
        P[i] = random_num[count++];
        Q[i] = random_num[count++];
    }
    Q[0]        = random_num[count];

    int* Number = (int*)malloc((NodeNum + 1) * sizeof(int));
    for (int i = 0; i < NodeNum + 1; i++) Number[i] = i;
    int** Data = (int**)malloc((NodeNum + 1) * sizeof(int*));
    for (int i = 0; i < NodeNum + 1; i++) Data[i] = &Number[i];

    Start           = clock();
    BinaryTree* OPT = LTT_BiTree_Build_Optimal_BST((void**)Data, sizeof(int), P, Q, NodeNum);
    End             = clock();
    printf("Optimal BST Time: %f ms\n", (double)(End - Start) / CLOCKS_PER_SEC * 1000);

    LTT_BiTree_PreOrder_Traverse_Recursive(OPT->Root, PRINT);
    printf("\n");
    LTT_BiTree_InOrder_Traverse_Recursive(OPT->Root, PRINT);
    printf("\n");
    LTT_BiTree_PostOrder_Traverse_Recursive(OPT->Root, PRINT);
    printf("\n");
    printf("Test Over!\n");
}