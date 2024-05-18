# 输出BSTreeTest的可执行文件
add_executable( BSTreeTest BSTreeTest.c )

# 链接BSTreeLib库
target_link_libraries( BSTreeTest BSTreeLib )
