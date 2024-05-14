#include "_AVLTreeNode.h"
#include <stdio.h>
#include <stdlib.h>
#include "../BinaryTree/_BinaryTree.h"

AVLTreeNode* LTT_AVLTreeNode_MakeNode(void* const Data)
{
    AVLTreeNode* RootNode = (AVLTreeNode*)malloc(sizeof(AVLTreeNode));
    if (RootNode == NULL)
    {
        printf("节点制作失败!\n");
        return NULL;
    }
    RootNode->BiTreeNode.Data       = Data;
    RootNode->BiTreeNode.LeftChild  = NODE_NULL;
    RootNode->BiTreeNode.RightChild = NODE_NULL;
    RootNode->BiTreeNode.Parent     = NODE_NULL;
    Set_Balance_Factor(RootNode, EH);
    return RootNode;
}

void LTT_AVLTreeNode_Destroy(AVLTreeNode** Root)
{
    free(*Root);
    *Root = NULL;
}

void LTT_AVLTreeNode_LeftRotate(AVLTreeNode** Root)
{
    AVLTreeNode* RightChildP   = (AVLTreeNode*)Get_RightChild(*Root);
    Get_RightChild(*Root)      = Get_LeftChild(RightChildP);
    Get_LeftChild(RightChildP) = (BinaryTreeNode*)(*Root);
    (*Root)                    = RightChildP;
}

void LTT_AVLTreeNode_RightRotate(AVLTreeNode** Root)
{
    AVLTreeNode* LeftChildP    = (AVLTreeNode*)Get_LeftChild(*Root);
    Get_LeftChild(*Root)       = Get_RightChild(LeftChildP);
    Get_RightChild(LeftChildP) = (BinaryTreeNode*)(*Root);
    (*Root)                    = LeftChildP;
}

void LTT_AVLTreeNode_LeftBalance_Insert(AVLTreeNode** Root)
{
    AVLTreeNode* LeftChildP = (AVLTreeNode*)Get_LeftChild(*Root);
    switch (Get_Balance_Factor(LeftChildP))
    {
        case LH :
        {
            Set_Balance_Factor(*Root, EH);
            Set_Balance_Factor(LeftChildP, EH);
            LTT_AVLTreeNode_RightRotate(Root);
            break;
        }
        case RH :
        {
            AVLTreeNode* RightChildP = (AVLTreeNode*)Get_RightChild(LeftChildP);
            switch (Get_Balance_Factor(RightChildP))
            {
                case LH :
                    Set_Balance_Factor(*Root, RH);
                    Set_Balance_Factor(LeftChildP, EH);
                    break;
                case EH :
                    Set_Balance_Factor(*Root, EH);
                    Set_Balance_Factor(LeftChildP, EH);
                    break;
                case RH :
                    Set_Balance_Factor(*Root, EH);
                    Set_Balance_Factor(LeftChildP, LH);
                    break;
            }
            Set_Balance_Factor(RightChildP, EH);
            LTT_AVLTreeNode_LeftRotate((AVLTreeNode**)&(Get_LeftChild(*Root)));
            LTT_AVLTreeNode_RightRotate(Root);
            break;
        }
    }
}

void LTT_AVLTreeNode_RightBalance_Insert(AVLTreeNode** Root)
{
    AVLTreeNode* RightChildP = (AVLTreeNode*)Get_RightChild(*Root);
    switch (Get_Balance_Factor(RightChildP))
    {
        case RH :
        {
            Set_Balance_Factor(*Root, EH);
            Set_Balance_Factor(RightChildP, EH);
            LTT_AVLTreeNode_LeftRotate(Root);
            break;
        }
        case LH :
        {
            AVLTreeNode* LeftChildP = (AVLTreeNode*)Get_LeftChild(RightChildP);
            switch (Get_Balance_Factor(LeftChildP))
            {
                case RH :
                    Set_Balance_Factor(*Root, LH);
                    Set_Balance_Factor(RightChildP, EH);
                    break;
                case EH :
                    Set_Balance_Factor(*Root, EH);
                    Set_Balance_Factor(RightChildP, EH);
                    break;
                case LH :
                    Set_Balance_Factor(*Root, EH);
                    Set_Balance_Factor(RightChildP, RH);
                    break;
            }
            Set_Balance_Factor(LeftChildP, EH);
            LTT_AVLTreeNode_RightRotate((AVLTreeNode**)&Get_RightChild(*Root));
            LTT_AVLTreeNode_LeftRotate(Root);
            break;
        }
    }
}

void LTT_AVLTreeNode_LeftBalance_Delete(AVLTreeNode** Root, bool* Lower)
{
    AVLTreeNode* RightChildP = (AVLTreeNode*)Get_RightChild(*Root);
    switch (Get_Balance_Factor(RightChildP))
    {
        case LH :
        {
            AVLTreeNode* LeftChildP = (AVLTreeNode*)Get_LeftChild(RightChildP);
            switch (Get_Balance_Factor(LeftChildP))
            {
                case LH :
                    Set_Balance_Factor(*Root, EH);
                    Set_Balance_Factor(RightChildP, RH);
                    break;
                case EH :
                    Set_Balance_Factor(*Root, EH);
                    Set_Balance_Factor(RightChildP, EH);
                    break;
                case RH :
                    Set_Balance_Factor(*Root, LH);
                    Set_Balance_Factor(RightChildP, EH);
                    break;
            }
            Set_Balance_Factor(LeftChildP, EH);
            LTT_AVLTreeNode_RightRotate((AVLTreeNode**)&Get_RightChild(*Root));
            LTT_AVLTreeNode_LeftRotate(Root);
            *Lower = true;
            break;    //高度-1
        }
        case EH :
        {
            Set_Balance_Factor(*Root, RH);
            Set_Balance_Factor(RightChildP, LH);
            LTT_AVLTreeNode_LeftRotate(Root);
            *Lower = false;
            break;    //高度未变
        }
        case RH :
        {
            Set_Balance_Factor(*Root, EH);
            Set_Balance_Factor(RightChildP, EH);
            LTT_AVLTreeNode_LeftRotate(Root);
            *Lower = true;
            break;    //高度-1
        }
    }
}

void LTT_AVLTreeNode_RightBalance_Delete(AVLTreeNode** Root, bool* Lower)
{
    AVLTreeNode* LeftChildP = (AVLTreeNode*)Get_LeftChild(*Root);
    switch (Get_Balance_Factor(LeftChildP))
    {
        case LH :
        {
            Set_Balance_Factor(*Root, EH);
            Set_Balance_Factor(LeftChildP, EH);
            LTT_AVLTreeNode_RightRotate(Root);
            *Lower = true;
            break;    //高度-1
        }
        case EH :
        {
            Set_Balance_Factor(*Root, LH);
            Set_Balance_Factor(LeftChildP, RH);
            LTT_AVLTreeNode_RightRotate(Root);
            *Lower = false;
            break;    //高度未变
        }
        case RH :
        {
            AVLTreeNode* RightChildP = (AVLTreeNode*)Get_RightChild(LeftChildP);
            switch (Get_Balance_Factor(RightChildP))
            {
                case LH :
                    Set_Balance_Factor(*Root, RH);
                    Set_Balance_Factor(LeftChildP, EH);
                    break;
                case EH :
                    Set_Balance_Factor(*Root, EH);
                    Set_Balance_Factor(LeftChildP, EH);
                    break;
                case RH :
                    Set_Balance_Factor(*Root, EH);
                    Set_Balance_Factor(LeftChildP, LH);
                    break;
            }
            Set_Balance_Factor(RightChildP, EH);
            LTT_AVLTreeNode_LeftRotate((AVLTreeNode**)&Get_LeftChild(*Root));
            LTT_AVLTreeNode_RightRotate(Root);
            *Lower = true;
            break;    //高度-1
        }
    }
}