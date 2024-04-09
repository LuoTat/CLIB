#include "AVLTree.h"

Status InitAVLTree(AVLTreePointer AVLTreeP) { AVLTreeP->RootNode = NULL; }

Status InsertAVLTree(AVLTreePointer AVLTreeP, void* Data, size_t DataSize, CompareFunction Comparator)
{
    bool Taller = false;
    return InsertAVLTree_Kernel(&AVLTreeP->RootNode, Data, DataSize, &Taller, Comparator);
}

Status InsertAVLTree_Kernel(AVLTreeNodePointer* RootNodePP, void* Data, size_t DataSize, bool* TallerP, CompareFunction Comparator)
{
    if (!(*RootNodePP))
    {
        (*RootNodePP)       = (AVLTreeNodePointer)malloc(sizeof(AVLTreeNode));
        (*RootNodePP)->Data = (void*)malloc(DataSize);
        memcpy((*RootNodePP)->Data, Data, DataSize);
        (*RootNodePP)->DataSize  = DataSize;
        (*RootNodePP)->LeftChild = (*RootNodePP)->RightChild = NULL;
        (*RootNodePP)->BalanceFactor                         = EH;
        *TallerP                                             = true;
    }
    else
    {
        int Delta = Comparator(Data, (*RootNodePP)->Data);
        if (Delta == 0)
        {
            *TallerP = false;
            return 0;
        }
        if (Delta < 0)
        {
            if (!InsertAVLTree_Kernel(&(*RootNodePP)->LeftChild, Data, DataSize, TallerP, Comparator)) return 0;
            if (*TallerP)
            {
                switch ((*RootNodePP)->BalanceFactor)
                {
                    case LH :
                        LeftBalance_Insert(RootNodePP);
                        *TallerP = false;
                        break;
                    case EH :
                        (*RootNodePP)->BalanceFactor = LH;
                        *TallerP                     = true;
                        break;
                    case RH :
                        (*RootNodePP)->BalanceFactor = EH;
                        *TallerP                     = false;
                        break;
                }
            }
        }
        else
        {
            if (!InsertAVLTree_Kernel(&(*RootNodePP)->RightChild, Data, DataSize, TallerP, Comparator)) return 0;
            if (*TallerP)
            {
                switch ((*RootNodePP)->BalanceFactor)
                {
                    case LH :
                        (*RootNodePP)->BalanceFactor = EH;
                        *TallerP                     = false;
                        break;
                    case EH :
                        (*RootNodePP)->BalanceFactor = RH;
                        *TallerP                     = true;
                        break;
                    case RH :
                        RightBalance_Insert(RootNodePP);
                        *TallerP = false;
                        break;
                }
            }
        }
    }
    return OK;
}

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

Status LevelOrderTraverse_AVL(AVLTreeNodePointer RootNodeP, Status (*Visit)(AVLTreeNodePointer))
{
    SqQueue Queue;
    InitQueue(&Queue, sizeof(AVLTreeNodePointer));
    AVLTreeNodePointer Temp;
    if (RootNodeP) EnQueue(&Queue, &RootNodeP);
    while (!QueueEmpty(Queue))
    {
        DeQueue(&Queue, &Temp);
        (*Visit)(Temp);
        if (Temp->LeftChild) EnQueue(&Queue, &Temp->LeftChild);
        if (Temp->RightChild) EnQueue(&Queue, &Temp->RightChild);
    }
}

void LeftRotate(AVLTreeNodePointer* RootNodePP)
{
    AVLTreeNodePointer RightChildP = (*RootNodePP)->RightChild;
    (*RootNodePP)->RightChild      = RightChildP->LeftChild;
    RightChildP->LeftChild         = (*RootNodePP);
    (*RootNodePP)                  = RightChildP;
}

void RightRotate(AVLTreeNodePointer* RootNodePP)
{
    AVLTreeNodePointer LeftChildP = (*RootNodePP)->LeftChild;
    (*RootNodePP)->LeftChild      = LeftChildP->RightChild;
    LeftChildP->RightChild        = (*RootNodePP);
    (*RootNodePP)                 = LeftChildP;
}

void LeftBalance_Insert(AVLTreeNodePointer* RootNodePP)
{
    AVLTreeNodePointer LeftChildP = (*RootNodePP)->LeftChild;
    switch (LeftChildP->BalanceFactor)
    {
        case LH :
        {
            (*RootNodePP)->BalanceFactor = LeftChildP->BalanceFactor = EH;
            RightRotate(RootNodePP);
            break;
        }
        case RH :
        {
            AVLTreeNodePointer RightChildP = LeftChildP->RightChild;
            switch (RightChildP->BalanceFactor)
            {
                case LH :
                    (*RootNodePP)->BalanceFactor = RH;
                    LeftChildP->BalanceFactor    = EH;
                    break;
                case EH : (*RootNodePP)->BalanceFactor = LeftChildP->BalanceFactor = EH; break;
                case RH :
                    (*RootNodePP)->BalanceFactor = EH;
                    LeftChildP->BalanceFactor    = LH;
                    break;
            }
            RightChildP->BalanceFactor = EH;
            LeftRotate(&(*RootNodePP)->LeftChild);
            RightRotate(RootNodePP);
            break;
        }
    }
}

void RightBalance_Insert(AVLTreeNodePointer* RootNodePP)
{
    AVLTreeNodePointer RightChildP = (*RootNodePP)->RightChild;
    switch (RightChildP->BalanceFactor)
    {
        case RH :
        {
            (*RootNodePP)->BalanceFactor = RightChildP->BalanceFactor = EH;
            LeftRotate(RootNodePP);
            break;
        }
        case LH :
        {
            AVLTreeNodePointer LeftChildP = RightChildP->LeftChild;
            switch (LeftChildP->BalanceFactor)
            {
                case RH :
                    (*RootNodePP)->BalanceFactor = LH;
                    RightChildP->BalanceFactor   = EH;
                    break;
                case EH : (*RootNodePP)->BalanceFactor = RightChildP->BalanceFactor = EH; break;
                case LH :
                    (*RootNodePP)->BalanceFactor = EH;
                    RightChildP->BalanceFactor   = RH;
                    break;
            }
            LeftChildP->BalanceFactor = EH;
            RightRotate(&(*RootNodePP)->RightChild);
            LeftRotate(RootNodePP);
            break;
        }
    }
}

void LeftBalance_Delete(AVLTreeNodePointer* RootNodePP, bool* LowerP)
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
                    (*RootNodePP)->BalanceFactor = EH;
                    RightChildP->BalanceFactor   = RH;
                    break;
                case EH : (*RootNodePP)->BalanceFactor = RightChildP->BalanceFactor = EH; break;
                case RH :
                    (*RootNodePP)->BalanceFactor = LH;
                    RightChildP->BalanceFactor   = EH;
                    break;
            }
            LeftChildP->BalanceFactor = EH;
            RightRotate(&(*RootNodePP)->RightChild);
            LeftRotate(RootNodePP);
            *LowerP = true;
            break;    //高度-1
        }
        case EH :
        {
            (*RootNodePP)->BalanceFactor = RH;
            RightChildP->BalanceFactor   = LH;
            LeftRotate(RootNodePP);
            *LowerP = false;
            break;    //高度未变
        }
        case RH :
        {
            (*RootNodePP)->BalanceFactor = RightChildP->BalanceFactor = EH;
            LeftRotate(RootNodePP);
            *LowerP = true;
            break;    //高度-1
        }
    }
}

void RightBalance_Delete(AVLTreeNodePointer* RootNodePP, bool* LowerP)
{
    AVLTreeNodePointer LeftChildP = (*RootNodePP)->LeftChild;
    switch (LeftChildP->BalanceFactor)
    {
        case LH :
        {
            (*RootNodePP)->BalanceFactor = LeftChildP->BalanceFactor = EH;
            RightRotate(RootNodePP);
            *LowerP = true;
            break;    //高度-1
        }
        case EH :
        {
            (*RootNodePP)->BalanceFactor = LH;
            LeftChildP->BalanceFactor    = RH;
            RightRotate(RootNodePP);
            *LowerP = false;
            break;    //高度未变
        }
        case RH :
        {
            AVLTreeNodePointer RightChildP = LeftChildP->RightChild;
            switch (RightChildP->BalanceFactor)
            {
                case LH :
                    (*RootNodePP)->BalanceFactor = RH;
                    LeftChildP->BalanceFactor    = EH;
                    break;
                case EH : (*RootNodePP)->BalanceFactor = LeftChildP->BalanceFactor = EH; break;
                case RH :
                    (*RootNodePP)->BalanceFactor = EH;
                    LeftChildP->BalanceFactor    = LH;
                    break;
            }
            RightChildP->BalanceFactor = EH;
            LeftRotate(&(*RootNodePP)->LeftChild);
            RightRotate(RootNodePP);
            *LowerP = true;
            break;    //高度-1
        }
    }
}

void* GetThePrecursorData(AVLTreeNodePointer RootNodeP)
{
    AVLTreeNodePointer Iterator = RootNodeP->LeftChild;
    while (Iterator->RightChild) Iterator = Iterator->RightChild;
    return Iterator->Data;
}
