# 开启测试
enable_testing()

# 设置头文件路径
include(googletest)
include_directories(${CMAKE_SOURCE_DIR}/Generic)

# 增加子目录
add_subdirectory(src/ArrayDequeTest)
add_subdirectory(src/ArrayListTest)
add_subdirectory(src/ArrayStackTest)

# add_subdirectory( src/AVLTreeTest )
# add_subdirectory( src/BinaryTreeTest )
# add_subdirectory( src/BSTreeTest )
# add_subdirectory(src/HashMapTest)
add_subdirectory(src/HashSetTest)
add_subdirectory(src/LinkedListTest)
add_subdirectory(src/SortingAlgorithmTest)

# add_subdirectory( src/UndigraphTest )
# add_subdirectory( src/UnionFindSetTest )
