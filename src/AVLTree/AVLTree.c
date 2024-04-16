#include "AVLTree.h"
#include "../../include/LTT_BSTree.h"
#include "../AVLTreeNode/AVLTreeNode.h"
#include "../AVLTreeNode/__AVLTreeNode.h"
#include "../Predefined/Predefined.h"

AVLTree* LTT_AVLTree_New(const size_t DataSize, const CompareFunction Comparator) { return (AVLTree*)LTT_BSTree_New(DataSize, Comparator); }

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

Status LTT_AVLTree_DeleteData(AVLTree* AVL_Tree, void* const Data);

void LTT_AVLTree_Clear(AVLTree* const AVL_Tree);

void LTT_AVLTree_Destroy(AVLTree** AVL_Tree);
