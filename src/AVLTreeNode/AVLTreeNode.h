#pragma once
#include "../BinaryTreeNode/BinaryTreeNode.h"
#include "../Predefined/Predefined.h"


#ifdef __cplusplus
extern "C"
{
#endif
    typedef BinaryTreeNode AVLTreeNode;
    //制作AVL树节点
    AVLTreeNode* LTT_AVLTreeNode_Make_Node(void* const Data, const size_t DataSize);
    //插入AVL树节点
    Status LTT_AVLTreeNode_Insert_Node_byComparator(AVLTreeNode** Root, AVLTreeNode* const Inserted_Node, const CompareFunction Comparator);
    Status LTT_AVLTreeNode_Insert_Data(AVLTreeNode** Root, void* const Data, const size_t DataSize, const CompareFunction Comparator);
#ifdef __cplusplus
}
#endif