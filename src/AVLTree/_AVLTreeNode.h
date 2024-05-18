#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "_PreBinaryTree.h"

#define LH                                    0X2
#define EH                                    0X0
#define RH                                    0X1

#define Get_Data(Node)                        ((Node)->BiTreeNode.Data)
#define Get_Balance_Factor(Node)              ((Node)->Height)
#define Get_LeftChild(Node)                   ((Node)->BiTreeNode.LeftChild)
#define Get_RightChild(Node)                  ((Node)->BiTreeNode.RightChild)
#define Set_Balance_Factor(Node, BalanceMark) ((Node)->Height = (BalanceMark))


#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct AVLTreeNode
    {
        BinaryTreeNode BiTreeNode;
        short          Height;
    } AVLTreeNode;

    // 制作AVL树节点
    AVLTreeNode* LTT_AVLTreeNode_MakeNode(void* const Data);
    // 销毁AVL树节点
    void LTT_AVLTreeNode_Destroy(AVLTreeNode** Root);
    // 左旋
    void LTT_AVLTreeNode_LeftRotate(AVLTreeNode** Root);
    // 右旋
    void LTT_AVLTreeNode_RightRotate(AVLTreeNode** Root);
    // 插入节点
    void LTT_AVLTreeNode_LeftBalance_Insert(AVLTreeNode** Root);
    void LTT_AVLTreeNode_RightBalance_Insert(AVLTreeNode** Root);
    // 删除节点
    void LTT_AVLTreeNode_LeftBalance_Delete(AVLTreeNode** Root, bool* Lower);
    void LTT_AVLTreeNode_RightBalance_Delete(AVLTreeNode** Root, bool* Lower);
#ifdef __cplusplus
}
#endif