# 输出BSTreeTest的可执行文件

add_executable( BSTreeTest BSTreeTest.c ) # 生成可执行文件

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    target_link_libraries( BSTreeTest BSTree_d ) # 添加库文件路径
elseif( CMAKE_BUILD_TYPE STREQUAL "Release" )
    target_link_libraries( BSTreeTest BSTree ) # 添加库文件路径
endif()