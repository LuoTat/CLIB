# 输出ArrayDequeTest的可执行文件
add_executable( ArrayDequeTest ArrayDequeTest.cpp )

# 链接HashMapLib库
target_link_libraries( ArrayDequeTest ArrayDequeLib )