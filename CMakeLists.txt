# 设置 CMake 的最低版本
cmake_minimum_required(VERSION 3.30)

# 设置工程名称和版本
project(LTTLIB VERSION 1.0.0 LANGUAGES C CXX)

# 设置 C 标准
set(CMAKE_C_STANDARD 17)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)

# 设置编译器的参数
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra -Werror")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -Wextra -Werror")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Werror")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -Werror")

# 设置编译器显示详细信息
set(CMAKE_VERBOSE_MAKEFILE False)

# 设置默认编译类型
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message(STATUS "未指定编译类型,设置默认编译类型:Release")
endif()

# 显示编译信息
message(STATUS "Build type:               ${CMAKE_BUILD_TYPE}")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "C编译器参数:                ${CMAKE_C_FLAGS_DEBUG}")
    message(STATUS "CXX编译器参数:              ${CMAKE_CXX_FLAGS_DEBUG}")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "C编译器参数:                ${CMAKE_C_FLAGS_RELEASE}")
    message(STATUS "CXX编译器参数:              ${CMAKE_CXX_FLAGS_RELEASE}")
endif()

# 设置 CLIB 接口库
add_library(CLIB INTERFACE
    src/Allocator_G.h src/ArrayDeque_G.h src/ArrayList_G.h
    src/ArrayStack_G.h src/GeneralHashFunctions.h src/Generic_tool.h
    src/HashSet_G.h src/LinkedList_G.h src/SortingAlgorithm_G.h
)
target_include_directories(CLIB INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include/CLIB>
)

# 添加测试目录
add_subdirectory(test)

# 添加性能测试目录
# add_subdirectory(benchmark)