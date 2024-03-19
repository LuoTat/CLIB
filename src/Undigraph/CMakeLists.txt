# 输出Undigraph的.a文件

# 添加源文件
list(APPEND UNDIGRAPH_SRC Undigraph.c)
list(APPEND UNDIGRAPH_SRC ../Iterator/Iterator.c)
list(APPEND UNDIGRAPH_SRC ../HashMap/HashMap.c)
list(APPEND UNDIGRAPH_SRC ../ArrayStack/ArrayStack.c)
list(APPEND UNDIGRAPH_SRC ../ArrayQueue/ArrayQueue.c)
list(APPEND UNDIGRAPH_SRC ../UnionFindSet/UnionFindSet.c)

# 生成静态库
add_library(UndigraphLib STATIC ${UNDIGRAPH_SRC}) # 生成lib文件

set_target_properties(UndigraphLib PROPERTIES OUTPUT_NAME "Undigraph") # 修改lib文件的输出名字
 