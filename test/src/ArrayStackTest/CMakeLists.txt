# 输出ArrayStackTest的可执行文件
add_executable(ArrayStackTest ArrayStackTest.cpp)

target_link_libraries(ArrayStackTest GTest::gtest GTest::gtest_main)

gtest_discover_tests(ArrayStackTest)