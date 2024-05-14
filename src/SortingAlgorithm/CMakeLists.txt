# 添加源文件
list( APPEND SORTINGALGORITHM_SRC SortingAlgorithm.c )

# 生成静态库
add_library( SortingAlgorithmLib STATIC ${SORTINGALGORITHM_SRC} )