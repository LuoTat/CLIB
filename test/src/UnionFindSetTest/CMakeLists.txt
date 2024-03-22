# 输出UnionFindSetTest的可执行文件

add_executable(UnionFindSetTest UnionFindSetTest.c) # 生成可执行文件

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(UnionFindSetTest UnionFindSet_d) # 添加库文件路径
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_libraries(UnionFindSetTest UnionFindSet) # 添加库文件路径
endif()