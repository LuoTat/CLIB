# 输出ArrayListTest的可执行文件

add_executable(ArrayListTest ArrayListTest.cpp) # 生成可执行文件

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(ArrayListTest ArrayList_d) # 添加库文件路径
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_libraries(ArrayListTest ArrayList) # 添加库文件路径
endif()