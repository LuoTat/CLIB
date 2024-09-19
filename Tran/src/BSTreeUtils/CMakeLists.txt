# 生成Lib文件
add_library( BSTreeUtilsLib STATIC )

# 添加源文件
target_sources( BSTreeUtilsLib PRIVATE BSTreeUtils.c )

# 添加依赖
target_link_libraries( BSTreeUtilsLib PUBLIC
    BinaryTreeLib
)

# 添加头文件
target_include_directories( BSTreeUtilsLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )