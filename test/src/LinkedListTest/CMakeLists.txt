# 输出LinkedListTest的可执行文件
add_executable(LinkedListTest LinkedListTest.cpp)

target_link_libraries(LinkedListTest GTest::gtest GTest::gtest_main)

gtest_discover_tests(LinkedListTest)