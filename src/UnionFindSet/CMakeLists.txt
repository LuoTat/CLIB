# 输出UnionFindSet的.a文件

# 添加源文件
list(APPEND UNIONFINDSET_SRC UnionFindSet.c) 
list(APPEND UNIONFINDSET_SRC ../Iterator/Iterator.c)
list(APPEND UNIONFINDSET_SRC ../HashMap/HashMap.c)

# 生成静态库
add_library(UnionFindSetLib STATIC ${UNIONFINDSET_SRC})

set_target_properties(UnionFindSetLib PROPERTIES OUTPUT_NAME "UnionFindSet") # 修改lib文件的输出名字
