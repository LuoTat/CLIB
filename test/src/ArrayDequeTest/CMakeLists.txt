# 输出ArrayDequeTest的可执行文件
add_executable( ArrayDequeTest ArrayDequeTest.cpp )

target_link_libraries( ArrayDequeTest ArrayDequeLib benchmark::benchmark )