#pragma once
#include "../AVLTreeNode/AVLTreeNode.h"

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct AVLTree
    {
        AVLTreeNode* Root;    //根节点
    } AVLTree;
#ifdef __cplusplus
}
#endif