#include "AVLTree.h"
#include <stdlib.h>
#include "../../include/LTT_BSTree.h"
#include "../AVLTreeNode/AVLTreeNode.h"
#include "../BiTreeNodeUtil/BiTreeNodeUtil.h"
#include "../Predefined/Predefined.h"

AVLTree* LTT_AVLTree_New(const size_t DataSize, const CompareFunction Comparator) { return (AVLTree*)LTT_BSTree_New(DataSize, Comparator); }

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

static Status LTT_AVLTreeNode_InsertNode(AVLTreeNode** Root, AVLTreeNode* const Inserted_Node, const CompareFunction Comparator)
{
    bool Taller = false;
    return LTT_AVLTreeNode_InsertNode_Kernel(Root, Inserted_Node, &Taller, Comparator);
}

Status LTT_AVLTree_InsertData(AVLTree* AVL_Tree, void* const Data)
{
    AVLTreeNode* BeInsertedNode = LTT_AVLTreeNode_MakeNode(Data);
    if (BeInsertedNode == NULL) return ERROR;
    if (LTT_AVLTreeNode_InsertNode(&AVL_Tree->BiTree.Root, BeInsertedNode, AVL_Tree->Comparator) == ERROR)
    {
        LTT_BiTreeNode_DestroyNode(&BeInsertedNode);
        return ERROR;
    }
    return OK;
}

static Status LTT_AVLTreeNode_DeleteData_Kernel(AVLTreeNode** Root, void* Data, size_t DataSize, bool* Lower, CompareFunction Comparator)
{
    if (*Root == NODE_NULL)
    {
        *Lower = false;
        return ERROR;
    }
    int Delta = Comparator(Data, Get_Data(*Root));
    if (Delta < 0)
    {
        if (LTT_AVLTreeNode_DeleteData_Kernel(&Get_LeftChild(*Root), Data, DataSize, Lower, Comparator) == ERROR) return ERROR;
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
        if (LTT_AVLTreeNode_DeleteData_Kernel(&Get_RightChild(*Root), Data, DataSize, Lower, Comparator) == ERROR) return ERROR;
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
            void* TempData = (void*)malloc(DataSize);
            memcpy(TempData, Get_Data(Get_LeftChild(*Root)), DataSize);
            LTT_AVLTreeNode_DeleteData_Kernel(Root, TempData, DataSize, Lower, Comparator);
            memcpy(Get_Data(*Root), TempData, DataSize);
            Set_Balance_Factor(*Root, EH);    // = EH;
            *Lower = true;
            free(TempData);
        }
        //此节点只有右节点
        else if (Get_LeftChild(*Root) == NODE_NULL && Get_RightChild(*Root) != NODE_NULL)
        {
            void* TempData = (void*)malloc(DataSize);
            memcpy(TempData, Get_Data(Get_RightChild(*Root)), DataSize);
            LTT_AVLTreeNode_DeleteData_Kernel(Root, TempData, DataSize, Lower, Comparator);
            memcpy(Get_Data(*Root), TempData, DataSize);
            Set_Balance_Factor(*Root, EH);
            *Lower = true;
            free(TempData);
        }
        //此节点有左子树与右子树
        else
        {
            void* PrecursorData = (void*)malloc(DataSize);
            memcpy(PrecursorData, LTT_BiTreeNode_GetPredecessorNode((*Root), Comparator)->Data, DataSize);
            LTT_AVLTreeNode_DeleteData_Kernel(&Get_LeftChild(*Root), PrecursorData, DataSize, Lower, Comparator);
            memcpy(Get_Data(*Root), PrecursorData, DataSize);
            free(PrecursorData);
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
    }
    return OK;
}

Status LTT_AVLTree_DeleteData(AVLTree* AVL_Tree, void* const Data)
{
    bool Lower = false;
    return LTT_AVLTreeNode_DeleteData_Kernel(&AVL_Tree->BiTree.Root, Data, AVL_Tree->BiTree.DataSize, &Lower, AVL_Tree->Comparator);
}

void LTT_AVLTree_Clear(AVLTree* const AVL_Tree);

void LTT_AVLTree_Destroy(AVLTree** AVL_Tree);
