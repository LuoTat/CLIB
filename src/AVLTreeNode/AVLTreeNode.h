#pragma once
#include "../BinaryTreeNode/BinaryTreeNode.h"

#define LH                                    0X2
#define EH                                    0X0
#define RH                                    0X1
#define BALANCE_MARK                          0X3

#define Get_Data(Node)                        (Get_UMarkNode((Node))->Data)
#define Get_Balance_Factor(Node)              ((size_t)(Node) & BALANCE_MARK)
#define Get_LeftChild(Node)                   (Get_UMarkNode((Node))->LeftChild)
#define Get_RightChild(Node)                  (Get_UMarkNode((Node))->RightChild)
#define Get_UMarkNode(Node)                   ((AVLTreeNode*)((size_t)(Node) & (~BALANCE_MARK)))
#define Set_Balance_Factor(Node, BalanceMark) ((Node) = (AVLTreeNode*)((size_t)(Node) | (BalanceMark)))


#ifdef __cplusplus
extern "C"
{
#endif
    typedef BinaryTreeNode AVLTreeNode;
    // 制作AVL树节点
    AVLTreeNode* LTT_AVLTreeNode_MakeNode(void* const Data);
    // 删除二叉树节点
    void LTT_AVLTreeNode_DeleteNode(AVLTreeNode** Root);
#ifdef __cplusplus
}
#endif