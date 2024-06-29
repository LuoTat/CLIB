# 生成Lib文件
add_library( ArrayStackLib STATIC )

# 添加源文件
target_sources( ArrayStackLib PRIVATE ArrayStack.c )

# 添加依赖
target_link_libraries( ArrayStackLib PUBLIC
    PredefinedHead
    IteratorLib
)

# 添加头文件
target_include_directories( ArrayStackLib PUBLIC .)