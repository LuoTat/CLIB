# 输出ArrayDequeTest的可执行文件
add_executable(ArrayDequeTest ArrayDequeTest.cpp)

target_link_libraries(ArrayDequeTest GTest::gtest GTest::gtest_main)

gtest_discover_tests(ArrayDequeTest)