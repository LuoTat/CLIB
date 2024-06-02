# sudo sh -c "echo performance | tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor"

# 设置头文件路径
include_directories( ../Generic )

# 添加GoogleBenchmark库
find_package( benchmark REQUIRED )

# 增加子目录
add_subdirectory( src/ArrayDequeBenchMark )
add_subdirectory( src/ArrayListBenchMark )
add_subdirectory( src/ArrayStackBenchMark )
add_subdirectory( src/LinkedListBenchMark )

# add_subdirectory( src/SortingAlgorithmBenchMark )
