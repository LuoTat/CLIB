# 输出ArrayListTest的可执行文件
add_executable(ArrayListTest ArrayListTest.cpp)

target_link_libraries(ArrayListTest GTest::gtest GTest::gtest_main)

gtest_discover_tests(ArrayListTest)