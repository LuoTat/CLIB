# 输出LinkedListTest的可执行文件

add_executable(LinkedListTest LinkedListTest.cpp) # 生成可执行文件

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(LinkedListTest LinkedList_d) # 添加库文件路径
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_libraries(LinkedListTest LinkedList) # 添加库文件路径
endif()