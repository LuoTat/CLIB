# 设置头文件路径
include_directories( ../Generic )

# 添加GoogleTest库
find_package( GTest REQUIRED )

# 增加子目录
# add_subdirectory( src/ArrayDequeTest )
add_subdirectory( src/ArrayListTest )
add_subdirectory( src/ArrayStackTest )

# add_subdirectory( src/AVLTreeTest )
# add_subdirectory( src/BinaryTreeTest )
# add_subdirectory( src/BSTreeTest )
# add_subdirectory( src/HashMapTest )
add_subdirectory( src/LinkedListTest )

 add_subdirectory( src/SortingAlgorithmTest )
# add_subdirectory( src/UndigraphTest )
# add_subdirectory( src/UnionFindSetTest )
