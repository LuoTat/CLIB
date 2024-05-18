# 生成Lib文件
add_library( BinaryTreeUtilsLib STATIC )

# 添加源文件
target_sources( BinaryTreeUtilsLib PRIVATE BinaryTreeUtils.c )

# 添加依赖
target_link_libraries( BinaryTreeUtilsLib PUBLIC
    ArrayStackLib
    ArrayQueueLib
    BinaryTreeLib
)

# 添加头文件
target_include_directories( BinaryTreeUtilsLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )