# 输出ArrayListTest的可执行文件
add_executable( ArrayListTest ArrayListTest.cpp )

# 链接HashMapLib库
target_link_libraries( ArrayListTest ArrayListLib )