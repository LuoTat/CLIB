# 输出SortingAlgorithm的.a文件

list(APPEND SORTINGALGORITHM_SRC SortingAlgorithm.c) # 添加源文件

add_library(SortingAlgorithmLib STATIC ${SORTINGALGORITHM_SRC}) # 生成lib文件

set_target_properties(SortingAlgorithmLib PROPERTIES OUTPUT_NAME "SortingAlgorithm") # 修改lib文件的输出名字
