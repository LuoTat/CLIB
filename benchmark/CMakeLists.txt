# 设置可执行文件的输出路径
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin )

# 设置头文件路径
include_directories( ../Generic )

# 添加GoogleBenchmark库
find_package( benchmark REQUIRED )

# 增加子目录
add_subdirectory( src/ArrayDequeBenchMark )
add_subdirectory( src/ArrayListBenchMark )
add_subdirectory( src/ArrayStackBenchMark )
add_subdirectory( src/LinkedListBenchMark )
add_subdirectory( src/SortingAlgorithmBenchMark )
