# 输出SortingAlgorithmBenchMark的可执行文件

add_executable(SortingAlgorithmBenchMark SortingAlgorithmBenchMark.cpp)

target_link_libraries(SortingAlgorithmBenchMark benchmark::benchmark)