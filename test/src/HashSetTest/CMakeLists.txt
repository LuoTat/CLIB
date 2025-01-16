# 输出HashSetTest的可执行文件
add_executable(HashSetTest HashSetTest.cpp)

target_link_libraries(HashSetTest gtest gtest_main)

add_test(HashSetTest HashSetTest)