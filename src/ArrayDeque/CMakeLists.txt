# 输出ArrayDeque的.a文件

# 添加源文件
list(APPEND ARRAYDEQUE_SRC ArrayDeque.c) 
list(APPEND ARRAYDEQUE_SRC ../Iterator/Iterator.c)

# 生成静态库
add_library(ArrayDequeLib STATIC ${ARRAYDEQUE_SRC}) 

set_target_properties(ArrayDequeLib PROPERTIES OUTPUT_NAME "ArrayDeque") # 修改lib文件的输出名字