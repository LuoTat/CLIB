#pragma once
#include "AVLTreeNode.h"

#ifdef __cplusplus
extern "C"
{
#endif
    // 插入AVL树节点
    Status LTT_AVLTreeNode_InsertNode(AVLTreeNode** Root, AVLTreeNode* const Inserted_Node, const CompareFunction Comparator);
#ifdef __cplusplus
}
#endif