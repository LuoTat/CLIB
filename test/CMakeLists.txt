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

# 增加子目录
add_subdirectory(src/ArrayDequeTest)
# add_subdirectory(src/ArrayListTest)
# add_subdirectory(src/ArrayStackTest)

# # add_subdirectory( src/AVLTreeTest )
# # add_subdirectory( src/BinaryTreeTest )
# # add_subdirectory( src/BSTreeTest )
# # add_subdirectory(src/HashMapTest)
# add_subdirectory(src/HashSetTest)
# add_subdirectory(src/LinkedListTest)
# add_subdirectory(src/SortingAlgorithmTest)

# # add_subdirectory( src/UndigraphTest )
# # add_subdirectory( src/UnionFindSetTest )
