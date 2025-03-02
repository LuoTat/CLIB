# 输出 ArrayDequeTest 的可执行文件
add_executable(ArrayDequeTest ArrayDequeTest.cpp)

target_link_libraries(ArrayDequeTest CLIB GTest::gtest_main)

gtest_discover_tests(ArrayDequeTest)

# add_test(ArrayDequeTest ArrayDequeTest)