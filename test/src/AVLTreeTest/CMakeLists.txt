# 输出AVLTreeTest的可执行文件
add_executable( AVLTreeTest AVLTreeTest.c )

# 链接AVLTreeLib库
target_link_libraries( AVLTreeTest AVLTreeLib )