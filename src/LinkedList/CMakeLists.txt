# 添加源文件
list( APPEND LINKEDLIST_SRC LinkedList.c )

# 生成OBJ文件
add_library( LinkedListObj OBJECT ${LINKEDLIST_SRC} )

# 添加依赖OBJ文件
add_dependencies( LinkedListObj IteratorObj )

list( APPEND LINKEDLIST_OBJ
    $<TARGET_OBJECTS:LinkedListObj>
    ${ITERATOR_OBJ}
)

set( LINKEDLIST_OBJ ${LINKEDLIST_OBJ} PARENT_SCOPE )

# 生成静态库
add_library( LinkedListLib STATIC ${LINKEDLIST_OBJ} )