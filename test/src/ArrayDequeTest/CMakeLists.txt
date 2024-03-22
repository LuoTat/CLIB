# 输出ArrayDequeTest的可执行文件

add_executable(ArrayDequeTest ArrayDequeTest.cpp) # 生成可执行文件

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(ArrayDequeTest ArrayDeque_d) # 添加库文件路径
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_libraries(ArrayDequeTest ArrayDeque) # 添加库文件路径
endif()