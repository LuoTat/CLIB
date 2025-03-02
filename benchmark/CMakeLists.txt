# sudo sh -c "echo performance | tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor"

# 查找 benchmark 库
find_package(benchmark QUIET)

# 如果 benchmark 库不存在，则使用 FetchContent 下载
if(NOT benchmark_FOUND)
    message("benchmark library not found, using FetchContent to download it.")
    include(FetchContent)
    FetchContent_Declare(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG main
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/benchmark
    )

    set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
    set(BENCHMARK_ENABLE_LTO ON)
    FetchContent_MakeAvailable(benchmark)
endif()

# array_deque_benchmark
add_executable(array_deque_benchmark src/array_deque_benchmark.cxx)
target_link_libraries(array_deque_benchmark CLIB benchmark::benchmark)

# array_list_benchmark
add_executable(array_list_benchmark src/array_list_benchmark.cxx)
target_link_libraries(array_list_benchmark CLIB benchmark::benchmark)

# array_stack_benchmark
add_executable(array_stack_benchmark src/array_stack_benchmark.cxx)
target_link_libraries(array_stack_benchmark CLIB benchmark::benchmark)

# hash_set_benchmark
add_executable(hash_set_benchmark src/hash_set_benchmark.cxx)
target_link_libraries(hash_set_benchmark CLIB benchmark::benchmark)

# linked_list_benchmark
add_executable(linked_list_benchmark src/linked_list_benchmark.cxx)
target_link_libraries(linked_list_benchmark CLIB benchmark::benchmark)

# sort_benchmark
add_executable(sort_benchmark src/sort_benchmark.cxx)
target_link_libraries(sort_benchmark CLIB benchmark::benchmark)