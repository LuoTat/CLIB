# 输出UndigraphTest的可执行文件

add_executable(UndigraphTest UndigraphTest.c) # 生成可执行文件

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(UndigraphTest Undigraph_d) # 添加库文件路径
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_libraries(UndigraphTest Undigraph) # 添加库文件路径
endif()

set_target_properties(UndigraphTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})