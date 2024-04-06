# 输出BinaryTreeTest的可执行文件

add_executable( BinaryTreeTest BinaryTreeTest.c ) # 生成可执行文件

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    target_link_libraries( BinaryTreeTest BinaryTree_d ) # 添加库文件路径
elseif( CMAKE_BUILD_TYPE STREQUAL "Release" )
    target_link_libraries( BinaryTreeTest BinaryTree ) # 添加库文件路径
endif()