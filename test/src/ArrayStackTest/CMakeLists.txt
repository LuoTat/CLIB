# 输出ArrayStackTest的可执行文件
add_executable(ArrayStackTest ArrayStackTest.cpp)

target_link_libraries(ArrayStackTest gtest gtest_main)

add_test(ArrayStackTest ArrayStackTest)