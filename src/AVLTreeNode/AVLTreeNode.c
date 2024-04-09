#include "AVLTreeNode.h"
#include "../Predefined/Predefined.h"

#define LH                                    0X2
#define EH                                    0X0
#define RH                                    0X1
#define BALANCE_MARK                          0X3

#define Get_Node(Node)                        ((AVLTreeNode*)((size_t)(Node) & (~BALANCE_MARK)))
#define Get_Balance_Factor(Node)              ((size_t)(Node) & BALANCE_MARK)
#define Set_Balance_Factor(Node, BalanceMark) ((Node) = (AVLTreeNode*)((size_t)(Node) | (BalanceMark)))

AVLTreeNode* LTT_AVLTreeNode_Make_Node(void* const Data, const size_t DataSize)
{
    AVLTreeNode* RootNode = (AVLTreeNode*)malloc(sizeof(AVLTreeNode));
    if (RootNode == NULL)
    {
        printf("根节点制作失败!\n");
        return NULL;
    }
    Set_Balance_Factor(RootNode, EH);
    return RootNode;
}

void LTT_AVLTreeNode_LeftRotate(AVLTreeNode** Root)
{
    AVLTreeNode* RightChildP = (*Root)->RightChild;
    (*Root)->RightChild      = RightChildP->LeftChild;
    RightChildP->LeftChild   = (*Root);
    (*Root)                  = RightChildP;
}

void LTT_AVLTreeNode_RightRotate(AVLTreeNode** Root)
{
    AVLTreeNode* LeftChildP = (*Root)->LeftChild;
    (*Root)->LeftChild      = LeftChildP->RightChild;
    LeftChildP->RightChild  = (*Root);
    (*Root)                 = LeftChildP;
}

void LTT_AVLTreeNode_LeftBalance_Insert(AVLTreeNode** Root)
{
    AVLTreeNode* LeftChildP = (*Root)->LeftChild;
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
            AVLTreeNode* RightChildP = LeftChildP->RightChild;
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
            LTT_AVLTreeNode_LeftRotate(&(*Root)->LeftChild);
            LTT_AVLTreeNode_RightRotate(Root);
            break;
        }
    }
}

void LTT_AVLTreeNode_RightBalance_Insert(AVLTreeNode** Root)
{
    AVLTreeNode* RightChildP = (*Root)->RightChild;
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
            AVLTreeNode* LeftChildP = RightChildP->LeftChild;
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
            LTT_AVLTreeNode_RightRotate(&(*Root)->RightChild);
            LTT_AVLTreeNode_LeftRotate(Root);
            break;
        }
    }
}

Status LTT_AVLTreeNode_Insert_Node_byComparator_Kernel(AVLTreeNode** BeInserted_Node, AVLTreeNode* const Inserted_Node, bool* const Taller, const CompareFunction Comparator)
{
    if ((*BeInserted_Node) == NODE_NULL)
    {
        *BeInserted_Node = Inserted_Node;
        Set_Balance_Factor(*BeInserted_Node, EH);
        *Taller = true;
    }
    else
    {
        int Delta = Comparator(Inserted_Node->Data, (*BeInserted_Node)->Data);
        if (Delta == 0)
        {
            *Taller = false;
            return 0;
        }
        if (Delta < 0)
        {
            if (LTT_AVLTreeNode_Insert_Node_byComparator_Kernel(&(*BeInserted_Node)->LeftChild, Inserted_Node, Taller, Comparator) == ERROR) return ERROR;
            if (*Taller)
            {
                switch (Get_Balance_Factor(*BeInserted_Node))
                {
                    case LH :
                        LTT_AVLTreeNode_LeftBalance_Insert(BeInserted_Node);
                        *Taller = false;
                        break;
                    case EH :

                        Set_Balance_Factor(*BeInserted_Node, LH);
                        *Taller = true;
                        break;
                    case RH :
                        Set_Balance_Factor(*BeInserted_Node, EH);
                        *Taller = false;
                        break;
                }
            }
        }
        else
        {
            if (LTT_AVLTreeNode_Insert_Node_byComparator_Kernel(&(*BeInserted_Node)->RightChild, Inserted_Node, Taller, Comparator) == ERROR) return ERROR;
            if (*Taller)
            {
                switch (Get_Balance_Factor(*BeInserted_Node))
                {
                    case LH :
                        Set_Balance_Factor(*BeInserted_Node, EH);
                        *Taller = false;
                        break;
                    case EH :
                        Set_Balance_Factor(*BeInserted_Node, RH);
                        *Taller = true;
                        break;
                    case RH :
                        LTT_AVLTreeNode_RightBalance_Insert(BeInserted_Node);
                        *Taller = false;
                        break;
                }
            }
        }
    }
    return OK;
}

Status LTT_AVLTreeNode_Insert_Node_byComparator(AVLTreeNode** Root, AVLTreeNode* const Inserted_Node, const CompareFunction Comparator)
{
    bool Taller = false;
    return LTT_AVLTreeNode_Insert_Node_byComparator_Kernel(Root, Inserted_Node, &Taller, Comparator);
}

Status LTT_AVLTreeNode_Insert_Data(AVLTreeNode** Root, void* const Data, const size_t DataSize, const CompareFunction Comparator)
{
    AVLTreeNode* BeInsertedNode = LTT_AVLTreeNode_Make_Node(Data, DataSize);
    if (BeInsertedNode == NULL) return ERROR;
    if (LTT_AVLTreeNode_Insert_Node_byComparator(Root, BeInsertedNode, Comparator) == ERROR)
    {
        free(BeInsertedNode);
        return ERROR;
    }
    return OK;
}
