# 输出SortingAlgorithmTest的可执行文件

add_executable(SortingAlgorithmTest SortingAlgorithmTest.cpp) # 生成可执行文件

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(SortingAlgorithmTest SortingAlgorithm_d) # 添加库文件路径
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_libraries(SortingAlgorithmTest SortingAlgorithm) # 添加库文件路径
endif()

#target_link_libraries(SortingAlgorithmTest m)

set_target_properties(SortingAlgorithmTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})