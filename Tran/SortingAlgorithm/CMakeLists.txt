# 生成静态库
add_library( SortingAlgorithmLib STATIC SortingAlgorithm.c)

# 添加依赖
target_link_libraries( SortingAlgorithmLib PUBLIC
    PredefinedHead
)

# 添加头文件路径
target_include_directories( SortingAlgorithmLib PUBLIC .)