# 生成Lib文件
add_library( AVLTreeLib STATIC )

# 添加源文件
target_sources( AVLTreeLib PRIVATE
    AVLTree.c
    _AVLTreeNode.c
)

# 添加依赖
target_link_libraries( AVLTreeLib PUBLIC
    BSTreeLib
    BSTreeUtilsLib
)

# 添加头文件
target_include_directories( AVLTreeLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )