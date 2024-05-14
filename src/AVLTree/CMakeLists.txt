# 输出AVLTree的.a文件

# 添加源文件
list( APPEND AVLTREE_SRC AVLTree.c )
list( APPEND AVLTREE_SRC _AVLTreeNode.c )
list( APPEND AVLTREE_SRC ../BSTree/BSTree.c )
list( APPEND AVLTREE_SRC ../BSTreeUtils/BSTreeUtils.c )
list( APPEND AVLTREE_SRC ../BinaryTreeUtils/BinaryTreeUtils.c )
list( APPEND AVLTREE_SRC ../BinaryTree/BinaryTree.c )
list( APPEND AVLTREE_SRC ../ArrayStack/ArrayStack.c )
list( APPEND AVLTREE_SRC ../ArrayQueue/ArrayQueue.c )
list( APPEND AVLTREE_SRC ../Iterator/Iterator.c )

# 生成静态库
add_library( AVLTreeLib STATIC ${AVLTREE_SRC} )

set_target_properties( AVLTreeLib PROPERTIES OUTPUT_NAME "AVLTree" ) # 修改lib文件的输出名字
