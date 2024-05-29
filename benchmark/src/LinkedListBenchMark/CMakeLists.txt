# 输出LinkedListBenchMark的可执行文件
add_executable( LinkedListBenchMark LinkedListBenchMark.cpp )

target_link_libraries( LinkedListBenchMark LinkedListLib benchmark::benchmark )