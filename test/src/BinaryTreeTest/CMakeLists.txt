# 输出BinaryTreeTes的可执行文件
add_executable( BinaryTreeTest BinaryTreeTest.c )

# 链接BinaryTreeLib库
target_link_libraries( BinaryTreeTest BinaryTreeLib )