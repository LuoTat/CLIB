include(FetchContent)

FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG main
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/benchmark
)

FetchContent_MakeAvailable(benchmark)