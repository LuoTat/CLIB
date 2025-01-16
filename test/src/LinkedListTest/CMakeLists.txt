# 输出LinkedListTest的可执行文件
add_executable(LinkedListTest LinkedListTest.cpp)

target_link_libraries(LinkedListTest gtest gtest_main)

add_test(LinkedListTest LinkedListTest)