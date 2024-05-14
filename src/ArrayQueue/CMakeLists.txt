# 添加源文件
list( APPEND ARRAYQUEUE_SRC ArrayQueue.c )

# 生成OBJ文件
add_library( ArrayQueueObj OBJECT ${ARRAYQUEUE_SRC} )

# 添加依赖OBJ文件
add_dependencies( ArrayQueueObj IteratorObj )

list( APPEND ARRAYQUEUE_OBJ
    $<TARGET_OBJECTS:ArrayQueueObj>
    ${ITERATOR_OBJ}
)

set( ARRAYQUEUE_OBJ ${ARRAYQUEUE_OBJ} PARENT_SCOPE )

# 生成静态库
add_library( ArrayQueueLib STATIC ${ARRAYQUEUE_OBJ} )