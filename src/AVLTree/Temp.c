#include "AVLTree.h"

Status DeleteAVLTree(AVLTreePointer AVLTreeP, void* Data, CompareFunction Comparator)
{
    bool Lower;
    if (!AVLTreeP) return ERROR;
    return DeleteAVLTree_Kernel(&AVLTreeP->RootNode, Data, &Lower, Comparator);
}

Status DeleteAVLTree_Kernel(AVLTreeNodePointer* RootNodePP, void* Data, bool* LowerP, CompareFunction Comparator)
{
    if (!*RootNodePP)
    {
        *LowerP = false;
        return 0;
    }
    int Delta = Comparator(Data, (*RootNodePP)->Data);
    if (Delta < 0)
    {
        if (!DeleteAVLTree_Kernel(&(*RootNodePP)->LeftChild, Data, LowerP, Comparator)) return 0;
        if (*LowerP)
        {
            switch ((*RootNodePP)->BalanceFactor)
            {
                case LH :
                    (*RootNodePP)->BalanceFactor = EH;
                    *LowerP                      = true;
                    break;
                case EH :
                    (*RootNodePP)->BalanceFactor = RH;
                    *LowerP                      = false;
                    break;
                case RH : LeftBalance_Delete(RootNodePP, LowerP); break;    //要进行旋转操作
            }
        }
    }
    else if (Delta > 0)
    {
        if (!DeleteAVLTree_Kernel(&(*RootNodePP)->RightChild, Data, LowerP, Comparator)) return 0;
        if (*LowerP)
        {
            switch ((*RootNodePP)->BalanceFactor)
            {
                case LH : RightBalance_Delete(RootNodePP, LowerP); break;    //要进行旋转操作
                case EH :
                    (*RootNodePP)->BalanceFactor = LH;
                    *LowerP                      = false;
                    break;
                case RH :
                    (*RootNodePP)->BalanceFactor = EH;
                    *LowerP                      = true;
                    break;
            }
        }
    }
    else    //找到节点，进行删除
    {
        //此节点为叶子节点
        if (!(*RootNodePP)->LeftChild && !(*RootNodePP)->RightChild)
        {
            AVLTreeNodePointer Temp = *RootNodePP;
            *RootNodePP             = NULL;
            free(Temp->Data);
            free(Temp);
            *LowerP = true;
        }
        //此节点只有左节点
        else if ((*RootNodePP)->LeftChild && !(*RootNodePP)->RightChild)
        {
            void* TempData = (void*)malloc((*RootNodePP)->DataSize);
            memcpy(TempData, (*RootNodePP)->LeftChild->Data, (*RootNodePP)->DataSize);
            DeleteAVLTree_Kernel(RootNodePP, TempData, LowerP, Comparator);
            memcpy((*RootNodePP)->Data, TempData, (*RootNodePP)->DataSize);
            (*RootNodePP)->BalanceFactor = EH;
            *LowerP                      = true;
            free(TempData);
        }
        //此节点只有右节点
        else if (!(*RootNodePP)->LeftChild && (*RootNodePP)->RightChild)
        {
            void* TempData = (void*)malloc((*RootNodePP)->DataSize);
            memcpy(TempData, (*RootNodePP)->RightChild->Data, (*RootNodePP)->DataSize);
            DeleteAVLTree_Kernel(RootNodePP, TempData, LowerP, Comparator);
            memcpy((*RootNodePP)->Data, TempData, (*RootNodePP)->DataSize);
            (*RootNodePP)->BalanceFactor = EH;
            *LowerP                      = true;
            free(TempData);
        }
        //此节点有左子树与右子树
        else
        {
            void* PrecursorData = (void*)malloc((*RootNodePP)->DataSize);
            memcpy(PrecursorData, GetThePrecursorData(*RootNodePP), (*RootNodePP)->DataSize);
            DeleteAVLTree_Kernel(&(*RootNodePP)->LeftChild, PrecursorData, LowerP, Comparator);
            memcpy((*RootNodePP)->Data, PrecursorData, (*RootNodePP)->DataSize);
            free(PrecursorData);
            if (*LowerP)
            {
                switch ((*RootNodePP)->BalanceFactor)
                {
                    case LH :
                        (*RootNodePP)->BalanceFactor = EH;
                        *LowerP                      = true;
                        break;
                    case EH :
                        (*RootNodePP)->BalanceFactor = RH;
                        *LowerP                      = false;
                        break;
                    case RH : LeftBalance_Delete(RootNodePP, LowerP); break;    //要进行旋转操作
                }
            }
        }
    }
    return OK;
}



void* GetThePrecursorData(AVLTreeNodePointer RootNodeP)
{
    AVLTreeNodePointer Iterator = RootNodeP->LeftChild;
    while (Iterator->RightChild) Iterator = Iterator->RightChild;
    return Iterator->Data;
}
