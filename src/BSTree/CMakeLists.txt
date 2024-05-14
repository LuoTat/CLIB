# 输出BSTree的.a文件

# 添加源文件
list( APPEND BSTREE_SRC BSTree.c )
list( APPEND BSTREE_SRC ../BinaryTree/BinaryTree.c )
list( APPEND BSTREE_SRC ../BinaryTreeUtils/BinaryTreeUtils.c)
list( APPEND BSTREE_SRC ../BSTreeUtils/BSTreeUtils.c)
list( APPEND BSTREE_SRC ../ArrayStack/ArrayStack.c )
list( APPEND BSTREE_SRC ../ArrayQueue/ArrayQueue.c )
list( APPEND BSTREE_SRC ../Iterator/Iterator.c )

# 生成静态库
add_library( BSTreeLib STATIC ${BSTREE_SRC} )

set_target_properties( BSTreeLib PROPERTIES OUTPUT_NAME "BSTree" ) # 修改lib文件的输出名字
