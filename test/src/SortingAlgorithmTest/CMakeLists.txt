# 输出SortingAlgorithmTest的可执行文件

add_executable( SortingAlgorithmTest SortingAlgorithmTest.cpp ) # 生成可执行文件

# 链接SortingAlgorithmLib库
target_link_libraries( SortingAlgorithmTest SortingAlgorithmLib ) # 链接库