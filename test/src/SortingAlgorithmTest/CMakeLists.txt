# 输出SortingAlgorithmTest的可执行文件
add_executable(SortingAlgorithmTest SortingAlgorithmTest.cpp)

target_link_libraries(SortingAlgorithmTest GTest::gtest GTest::gtest_main)

gtest_discover_tests(SortingAlgorithmTest)