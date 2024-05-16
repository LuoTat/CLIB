# 输出UndigraphTest的可执行文件
add_executable( UndigraphTest UndigraphTest.c )

# 链接HashMapLib库
target_link_libraries( UndigraphTest UndigraphLib )