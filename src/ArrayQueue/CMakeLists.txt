# 输出ArrayQueue的.a文件

# 添加源文件
list( APPEND ARRAYQUEUE_SRC ArrayQueue.c )
list( APPEND ARRAYQUEUE_SRC ../Iterator/Iterator.c )

# 生成静态库
add_library( ArrayQueueLib STATIC ${ARRAYQUEUE_SRC} )

set_target_properties( ArrayQueueLib PROPERTIES OUTPUT_NAME "ArrayQueue" ) # 修改lib文件的输出名字
