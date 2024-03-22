# 输出ArrayStackTest的可执行文件

add_executable(ArrayStackTest ArrayStackTest.cpp) # 生成可执行文件

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(ArrayStackTest ArrayStack_d) # 添加库文件路径
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_libraries(ArrayStackTest ArrayStack) # 添加库文件路径
endif()