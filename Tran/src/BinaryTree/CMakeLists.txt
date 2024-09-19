# 生成Lib文件
add_library( BinaryTreeLib STATIC )

# 添加源文件
target_sources( BinaryTreeLib PRIVATE BinaryTree.c )

# 添加依赖
target_link_libraries( BinaryTreeLib PUBLIC
    BinaryTreeUtilsLib
)

# 添加头文件
target_include_directories( BinaryTreeLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )