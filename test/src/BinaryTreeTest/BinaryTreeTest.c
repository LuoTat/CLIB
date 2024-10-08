#include "BinaryTree.h"
#include <stdio.h>

Status PRINT(void* Data)
{
    BinaryTreeNode* Node = (BinaryTreeNode*)Data;
    printf("%d ", *(int*)Node->Data);
    return OK;
}

int main()
{
    /*
                 1
              /     \
              2     3
            /   \  /  \
            4   5  6   7
    */
    int int1           = 1;
    int int2           = 2;
    int int3           = 3;
    int int4           = 4;
    int int5           = 5;
    int int6           = 6;
    int int7           = 7;

    BinaryTree* BiTree = LTT_BiTree_New(sizeof(int));

    LTT_BiTree_Insert(BiTree, NULL, &int1, 0);
    LTT_BiTree_Insert(BiTree, BiTree->Root, &int2, 1);
    LTT_BiTree_Insert(BiTree, BiTree->Root, &int3, 0);
    LTT_BiTree_Insert(BiTree, BiTree->Root->LeftChild, &int4, 1);
    LTT_BiTree_Insert(BiTree, BiTree->Root->LeftChild, &int5, 0);
    LTT_BiTree_Insert(BiTree, BiTree->Root->RightChild, &int6, 1);
    LTT_BiTree_Insert(BiTree, BiTree->Root->RightChild, &int7, 0);

    //LTT_BiTreeNode_DeleteSubTree(&BiTree->Root);
    //(BiTree, BiTree->Root->RightChild->LeftChild);

    printf("PreOrder: \n");
    LTT_BiTreeUtils_PreOrder_Traverse_Recursive(BiTree->Root, PRINT);
    printf("\n");
    LTT_BiTreeUtils_PreOrder_Traverse_Stack(BiTree->Root, PRINT);
    printf("\n");

    printf("InOrder: \n");
    LTT_BiTreeUtils_InOrder_Traverse_Recursive(BiTree->Root, PRINT);
    printf("\n");
    LTT_BiTreeUtils_InOrder_Traverse_Stack(BiTree->Root, PRINT);
    printf("\n");

    printf("PostOrder: \n");
    LTT_BiTreeUtils_PostOrder_Traverse_Recursive(BiTree->Root, PRINT);
    printf("\n");
    LTT_BiTreeUtils_PostOrder_Traverse_Stack(BiTree->Root, PRINT);
    printf("\n");

    printf("LevelOrder: \n");
    LTT_BiTreeUtils_LevelOrder_Traverse_Queue(BiTree->Root, PRINT);
    printf("\n");

    int NodeNumber = LTT_BiTreeUtils_GetNodeNumber(BiTree->Root);
    printf("NodeNumber: %d\n", NodeNumber);

    int LeafNum = LTT_BiTreeUtils_GetLeafNumber(BiTree->Root);
    printf("LeafNumber: %d\n", LeafNum);

    int Depth = LTT_BiTreeUtils_GetDepth(BiTree->Root);
    printf("Depth: %d\n", Depth);

    LTT_BiTree_Destroy(&BiTree);
}