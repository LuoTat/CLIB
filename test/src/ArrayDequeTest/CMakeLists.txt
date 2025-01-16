# 输出ArrayDequeTest的可执行文件
add_executable(ArrayDequeTest ArrayDequeTest.cpp)

target_link_libraries(ArrayDequeTest gtest gtest_main)

add_test(ArrayDequeTest ArrayDequeTest)