# 输出LinkedListTest的可执行文件
add_executable( LinkedListTest LinkedListTest.cpp )

# 链接HashMapLib库
target_link_libraries( LinkedListTest LinkedListLib )