# 输出HashSetBenchMark的可执行文件
add_executable(HashSetBenchMark HashSetBenchMark.cpp)

target_link_libraries(HashSetBenchMark benchmark::benchmark)