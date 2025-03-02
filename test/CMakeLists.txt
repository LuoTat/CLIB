# 开启测试
enable_testing()

# 查找 GTest 库
find_package(GTest QUIET)

# 如果 GTest 库不存在，则使用 FetchContent 下载
if(NOT GTest_FOUND)
    message("GTest library not found, using FetchContent to download it.")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/googletest
    )

    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)
endif()

# 引入 GoogleTest 模块
include(GoogleTest)

# array_deque_test
add_executable(array_deque_test src/array_deque_test.cxx)
target_link_libraries(array_deque_test CLIB GTest::gtest_main)
gtest_discover_tests(array_deque_test)

# array_list_test
add_executable(array_list_test src/array_list_test.cxx)
target_link_libraries(array_list_test CLIB GTest::gtest_main)
gtest_discover_tests(array_list_test)

# array_stack_test
add_executable(array_stack_test src/array_stack_test.cxx)
target_link_libraries(array_stack_test CLIB GTest::gtest_main)
gtest_discover_tests(array_stack_test)

# hash_set_test
add_executable(hash_set_test src/hash_set_test.cxx)
target_link_libraries(hash_set_test CLIB GTest::gtest_main)
gtest_discover_tests(hash_set_test)

# linked_list_test
add_executable(linked_list_test src/linked_list_test.cxx)
target_link_libraries(linked_list_test CLIB GTest::gtest_main)
gtest_discover_tests(linked_list_test)

# sort_test
add_executable(sort_test src/sort_test.cxx)
target_link_libraries(sort_test CLIB GTest::gtest_main)
gtest_discover_tests(sort_test)