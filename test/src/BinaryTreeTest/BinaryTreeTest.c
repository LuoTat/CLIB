#include <stdio.h>
#include "LTT_BinaryTree.h"

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
    int int1               = 1;
    int int2               = 2;
    int int3               = 3;
    int int4               = 4;
    int int5               = 5;
    int int6               = 6;
    int int7               = 7;

    BinaryTree*     BiTree = LTT_BiTree_New(sizeof(int));
    BinaryTreeNode* Node1  = LTT_BiTreeNode_MakeNode(&int1);
    BinaryTreeNode* Node2  = LTT_BiTreeNode_MakeNode(&int2);
    BinaryTreeNode* Node3  = LTT_BiTreeNode_MakeNode(&int3);
    BinaryTreeNode* Node4  = LTT_BiTreeNode_MakeNode(&int4);
    BinaryTreeNode* Node5  = LTT_BiTreeNode_MakeNode(&int5);
    BinaryTreeNode* Node6  = LTT_BiTreeNode_MakeNode(&int6);
    BinaryTreeNode* Node7  = LTT_BiTreeNode_MakeNode(&int7);
    LTT_BiTree_InsertRoot(BiTree, Node1);
    LTT_BiTree_InsertNode(BiTree->Root, Node2, 1);
    LTT_BiTree_InsertNode(BiTree->Root, Node3, 0);
    LTT_BiTree_InsertNode(Node2, Node4, 1);
    LTT_BiTree_InsertNode(Node2, Node5, 0);
    LTT_BiTree_InsertNode(Node3, Node6, 1);
    LTT_BiTree_InsertNode(Node3, Node7, 0);

    LTT_BiTreeNode_DeleteSubTree(&BiTree->Root->LeftChild);

    printf("PreOrder: \n");
    LTT_BiTreeNode_PreOrder_Traverse_Recursive(BiTree->Root, PRINT);
    printf("\n");
    LTT_BiTreeNode_PreOrder_Traverse_Stack(BiTree->Root, PRINT);
    printf("\n");

    printf("InOrder: \n");
    LTT_BiTreeNode_InOrder_Traverse_Recursive(BiTree->Root, PRINT);
    printf("\n");
    LTT_BiTreeNode_InOrder_Traverse_Stack(BiTree->Root, PRINT);
    printf("\n");

    printf("PostOrder: \n");
    LTT_BiTreeNode_PostOrder_Traverse_Recursive(BiTree->Root, PRINT);
    printf("\n");
    LTT_BiTreeNode_PostOrder_Traverse_Stack(BiTree->Root, PRINT);
    printf("\n");

    printf("LevelOrder: \n");
    LTT_BiTreeNode_LevelOrder_Traverse_Queue(BiTree->Root, PRINT);
    printf("\n");

    int NodeNumber = LTT_BiTreeNode_GetNodeNumber(BiTree->Root);
    printf("NodeNumber: %d\n", NodeNumber);

    int LeafNum = LTT_BiTreeNode_GetLeafNumber(BiTree->Root);
    printf("LeafNumber: %d\n", LeafNum);

    int Depth = LTT_BiTreeNode_GetDepth(BiTree->Root);
    printf("Depth: %d\n", Depth);

    LTT_BiTree_Destroy(&BiTree);
}