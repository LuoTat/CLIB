#include "AVLTreeNode.h"
#include "../Predefined/Predefined.h"
#include "AVLTreeNode copy.h"

AVLTreeNode* LTT_AVLTreeNode_MakeNode(void* const Data)
{
    AVLTreeNode* RootNode = LTT_BiTreeNode_MakeNode(Data);
    if (RootNode == NULL)
    {
        printf("根节点制作失败!\n");
        return NULL;
    }
    Set_Balance_Factor(RootNode, EH);
    return RootNode;
}

static void LTT_AVLTreeNode_LeftRotate(AVLTreeNode** Root)
{
    AVLTreeNode* RightChildP   = Get_RightChild(*Root);
    Get_RightChild(*Root)      = Get_LeftChild(RightChildP);
    Get_LeftChild(RightChildP) = (*Root);
    (*Root)                    = RightChildP;
}

static void LTT_AVLTreeNode_RightRotate(AVLTreeNode** Root)
{
    AVLTreeNode* LeftChildP    = Get_LeftChild(*Root);
    Get_LeftChild(*Root)       = Get_RightChild(LeftChildP);
    Get_RightChild(LeftChildP) = (*Root);
    (*Root)                    = LeftChildP;
}

static void LTT_AVLTreeNode_LeftBalance_Insert(AVLTreeNode** Root)
{
    AVLTreeNode* LeftChildP = Get_LeftChild(*Root);
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
            AVLTreeNode* RightChildP = Get_RightChild(LeftChildP);
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
            LTT_AVLTreeNode_LeftRotate(&Get_LeftChild(*Root));
            LTT_AVLTreeNode_RightRotate(Root);
            break;
        }
    }
}

static void LTT_AVLTreeNode_RightBalance_Insert(AVLTreeNode** Root)
{
    AVLTreeNode* RightChildP = Get_RightChild(*Root);
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
            AVLTreeNode* LeftChildP = Get_LeftChild(RightChildP);
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
            LTT_AVLTreeNode_RightRotate(&Get_RightChild(*Root));
            LTT_AVLTreeNode_LeftRotate(Root);
            break;
        }
    }
}

static Status LTT_AVLTreeNode_InsertNode_Kernel(AVLTreeNode** BeInserted_Node, AVLTreeNode* const Inserted_Node, bool* const Taller, const CompareFunction Comparator)
{
    if ((*BeInserted_Node) == NODE_NULL)
    {
        *BeInserted_Node = Inserted_Node;
        Set_Balance_Factor(*BeInserted_Node, EH);
        *Taller = true;
    }
    else
    {
        int Delta = Comparator(Get_Data(Inserted_Node), Get_Data(*BeInserted_Node));
        if (Delta == 0)
        {
            *Taller = false;
            return ERROR;
        }
        if (Delta < 0)
        {
            if (LTT_AVLTreeNode_InsertNode_Kernel(&Get_LeftChild(*BeInserted_Node), Inserted_Node, Taller, Comparator) == ERROR) return ERROR;
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
            if (LTT_AVLTreeNode_InsertNode_Kernel(&Get_RightChild(*BeInserted_Node), Inserted_Node, Taller, Comparator) == ERROR) return ERROR;
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

Status LTT_AVLTreeNode_InsertNode(AVLTreeNode** Root, AVLTreeNode* const Inserted_Node, const CompareFunction Comparator)
{
    bool Taller = false;
    return LTT_AVLTreeNode_InsertNode_Kernel(Root, Inserted_Node, &Taller, Comparator);
}

static void LTT_AVLTreeNode_LeftBalance_Delete(AVLTreeNode** Root, bool* Lower)
{
    AVLTreeNodePointer RightChildP = (*RootNodePP)->RightChild;
    switch (RightChildP->BalanceFactor)
    {
        case LH :
        {
            AVLTreeNodePointer LeftChildP = RightChildP->LeftChild;
            switch (LeftChildP->BalanceFactor)
            {
                case LH :
                    (*Root)->BalanceFactor     = EH;
                    RightChildP->BalanceFactor = RH;
                    break;
                case EH : (*Root)->BalanceFactor = RightChildP->BalanceFactor = EH; break;
                case RH :
                    (*Root)->BalanceFactor     = LH;
                    RightChildP->BalanceFactor = EH;
                    break;
            }
            LeftChildP->BalanceFactor = EH;
            RightRotate(&(*Root)->RightChild);
            LeftRotate(Root);
            *Lower = true;
            break;    //高度-1
        }
        case EH :
        {
            (*Root)->BalanceFactor     = RH;
            RightChildP->BalanceFactor = LH;
            LeftRotate(Root);
            *Lower = false;
            break;    //高度未变
        }
        case RH :
        {
            (*Root)->BalanceFactor = RightChildP->BalanceFactor = EH;
            LeftRotate(Root);
            *Lower = true;
            break;    //高度-1
        }
    }
}

static void LTT_AVLTreeNode_RightBalance_Delete(AVLTreeNode** Root, bool* Lower)
{
    AVLTreeNodePointer LeftChildP = (*RootNodePP)->LeftChild;
    switch (LeftChildP->BalanceFactor)
    {
        case LH :
        {
            (*RootNodePP)->BalanceFactor = LeftChildP->BalanceFactor = EH;
            RightRotate(Root);
            *Lower = true;
            break;    //高度-1
        }
        case EH :
        {
            (*Root)->BalanceFactor    = LH;
            LeftChildP->BalanceFactor = RH;
            RightRotate(Root);
            *Lower = false;
            break;    //高度未变
        }
        case RH :
        {
            AVLTreeNodePointer RightChildP = LeftChildP->RightChild;
            switch (RightChildP->BalanceFactor)
            {
                case LH :
                    (*Root)->BalanceFactor    = RH;
                    LeftChildP->BalanceFactor = EH;
                    break;
                case EH : (*RootNodePP)->BalanceFactor = LeftChildP->BalanceFactor = EH; break;
                case RH :
                    (*Root)->BalanceFactor    = EH;
                    LeftChildP->BalanceFactor = LH;
                    break;
            }
            RightChildP->BalanceFactor = EH;
            LeftRotate(&(*Root)->LeftChild);
            RightRotate(Root);
            *Lower = true;
            break;    //高度-1
        }
    }
}

Status LTT_AVLTreeNode_DeleteData_Kernel(AVLTreeNode** Root, void* Data, bool* Lower, CompareFunction Comparator)
{
    if (*Root == NODE_NULL)
    {
        *Lower = false;
        return ERROR;
    }
    int Delta = Comparator(Data, Get_Data(*Root));
    if (Delta < 0)
    {
        if (LTT_AVLTreeNode_DeleteData_Kernel(&Get_LeftChild(*Root), Data, Lower, Comparator) == ERROR) return ERROR;
        if (*Lower)
        {
            switch (Get_Balance_Factor(*Root))
            {
                case LH :
                    Set_Balance_Factor(*Root, EH);
                    *Lower = true;
                    break;
                case EH :
                    Set_Balance_Factor(*Root, RH);
                    *Lower = false;
                    break;
                case RH : LTT_AVLTreeNode_LeftBalance_Delete(Root, Lower); break;    //要进行旋转操作
            }
        }
    }
    else if (Delta > 0)
    {
        if (LTT_AVLTreeNode_DeleteData_Kernel(&Get_RightChild(*Root), Data, Lower, Comparator) == ERROR) return ERROR;
        if (*Lower)
        {
            switch (Get_Balance_Factor(*Root))
            {
                case LH : LTT_AVLTreeNode_RightBalance_Delete(Root, Lower); break;    //要进行旋转操作
                case EH :
                    Set_Balance_Factor(*Root, LH);
                    *Lower = false;
                    break;
                case RH :
                    Set_Balance_Factor(*Root, EH);
                    *Lower = true;
                    break;
            }
        }
    }
    else    //找到节点，进行删除
    {
        //此节点为叶子节点
        if (Get_LeftChild(*Root) == Get_RightChild(*Root) == NODE_NULL)
        {
            LTT_BiTreeNode_DestroyNode(Root);
            *Lower = true;
        }
        //此节点只有左节点
        else if (Get_LeftChild(*Root) != NODE_NULL && Get_RightChild(*Root) == NODE_NULL)
        {
            void* TempData = (void*)malloc((*Root)->DataSize);
            memcpy(TempData, Get_Data(Get_LeftChild(*Root)), (*Root)->DataSize);
            LTT_AVLTreeNode_DeleteData_Kernel(RootNodePP, TempData, LowerP, Comparator);
            memcpy((*Root)->Data, TempData, (*Root)->DataSize);
            Set_Balance_Factor(*Root, EH);    // = EH;
            *Lower = true;
            free(TempData);
        }
        //此节点只有右节点
        else if (Get_LeftChild(*Root) == NODE_NULL && Get_RightChild(*Root) != NODE_NULL)
        {
            void* TempData = (void*)malloc((*Root)->DataSize);
            memcpy(TempData, (*Root)->RightChild->Data, (*Root)->DataSize);
            LTT_AVLTreeNode_DeleteData_Kernel(RootNodePP, TempData, LowerP, Comparator);
            memcpy((*Root)->Data, TempData, (*Root)->DataSize);
            (*Root)->BalanceFactor = EH;
            *Lower                 = true;
            free(TempData);
        }
        //此节点有左子树与右子树
        else
        {
            void* PrecursorData = (void*)malloc((*Root)->DataSize);
            memcpy(PrecursorData, GetThePrecursorData(*Root), (*Root)->DataSize);
            LTT_AVLTreeNode_DeleteData_Kernel(&(*RootNodePP)->LeftChild, PrecursorData, LowerP, Comparator);
            memcpy((*Root)->Data, PrecursorData, (*Root)->DataSize);
            free(PrecursorData);
            if (*Lower)
            {
                switch ((*Root)->BalanceFactor)
                {
                    case LH :
                        (*Root)->BalanceFactor = EH;
                        *Lower                 = true;
                        break;
                    case EH :
                        (*Root)->BalanceFactor = RH;
                        *Lower                 = false;
                        break;
                    case RH : LeftBalance_Delete(RootNodePP, LowerP); break;    //要进行旋转操作
                }
            }
        }
    }
    return OK;
}