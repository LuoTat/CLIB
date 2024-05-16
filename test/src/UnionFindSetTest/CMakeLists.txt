# 输出UnionFindSetTest的可执行文件

add_executable( UnionFindSetTest UnionFindSetTest.c ) # 生成可执行文件

# 添加UnionFindSetLib库
target_link_libraries( UnionFindSetTest UnionFindSetLib ) # 添加库文件路径
