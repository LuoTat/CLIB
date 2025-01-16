# sudo sh -c "echo performance | tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor"

# 设置头文件路径
include(benchmark)
include_directories(${CMAKE_SOURCE_DIR}/Generic)

# 增加子目录
add_subdirectory(src/ArrayDequeBenchMark)
add_subdirectory(src/ArrayListBenchMark)
add_subdirectory(src/ArrayStackBenchMark)
add_subdirectory(src/HashSetBenchMark)
add_subdirectory(src/LinkedListBenchMark)
add_subdirectory(src/SortingAlgorithmBenchMark)
