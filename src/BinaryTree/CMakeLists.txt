# 输出BinaryTree的.a文件

# 添加源文件
list(APPEND BINARYTREE_SRC BinaryTree.c) 
list(APPEND BINARYTREE_SRC ../ArrayStack/ArrayStack.c)
list(APPEND BINARYTREE_SRC ../ArrayQueue/ArrayQueue.c)
list(APPEND BINARYTREE_SRC ../Iterator/Iterator.c)


# 生成静态库
add_library(BinaryTreeLib STATIC ${BINARYTREE_SRC})

set_target_properties(BinaryTreeLib PROPERTIES OUTPUT_NAME "BinaryTree") # 修改lib文件的输出名字
