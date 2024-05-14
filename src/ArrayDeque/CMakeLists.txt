# 添加源文件
list( APPEND ARRAYDEQUE_SRC ArrayDeque.c )

# 生成OBJ文件
add_library( ArrayDequeObj OBJECT ${ARRAYDEQUE_SRC} )

# 添加依赖OBJ文件
add_dependencies( ArrayDequeObj IteratorObj )

list( APPEND ARRAYDEQUE_OBJ
    $<TARGET_OBJECTS:ArrayDequeObj>
    ${ITERATOR_OBJ}
)

set( ARRAYDEQUE_OBJ ${ARRAYDEQUE_OBJ} PARENT_SCOPE )

# 生成静态库
add_library( ArrayDequeLib STATIC ${ARRAYDEQUE_OBJ} )