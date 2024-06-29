# 生成Lib文件
add_library( ArrayDequeLib STATIC )

# 添加源文件
target_sources( ArrayDequeLib PRIVATE ArrayDeque.c )

# 添加依赖
target_link_libraries( ArrayDequeLib PUBLIC
    PredefinedHead
    IteratorLib
)

# 添加头文件
target_include_directories( ArrayDequeLib PUBLIC .)