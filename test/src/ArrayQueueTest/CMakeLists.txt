# 输出ArrayQueueTest的可执行文件
add_executable( ArrayQueueTest ArrayQueueTest.cpp )

# 链接HashMapLib库
target_link_libraries( ArrayQueueTest ArrayQueueLib )