#设置可执行文件的输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

#增加子目录
add_subdirectory(src/ArrayDequeTest)
add_subdirectory(src/ArrayListTest)
add_subdirectory(src/ArrayQueueTest)
add_subdirectory(src/ArrayStackTest)
add_subdirectory(src/LinkedListTest)
add_subdirectory(src/HashMapTest)
add_subdirectory(src/UnionFindSetTest)
add_subdirectory(src/UndigraphTest)
add_subdirectory(src/SortingAlgorithmTest)
add_subdirectory(src/BinaryTreeTest)