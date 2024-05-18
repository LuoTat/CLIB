# 生成Lib文件
add_library( BSTreeLib STATIC )

# 添加源文件
target_sources( BSTreeLib PRIVATE BSTree.c )

# 添加依赖
target_link_libraries( BSTreeLib PUBLIC
    ArrayStackLib
    BinaryTreeLib
    BSTreeUtilsLib
)

# 添加头文件
target_include_directories( BSTreeLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )