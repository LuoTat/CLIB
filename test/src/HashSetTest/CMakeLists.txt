# 输出HashSetTest的可执行文件
add_executable(HashSetTest HashSetTest.cpp)

target_link_libraries(HashSetTest GTest::gtest GTest::gtest_main)

gtest_discover_tests(HashSetTest)