# 添加源文件
list( APPEND UNDIGRAPH_SRC Undigraph.c )

# 生成OBJ文件
add_library( UndigraphObj OBJECT ${UNDIGRAPH_SRC} )

# 添加依赖OBJ文件
add_dependencies( UndigraphObj
    ArrayStackObj
    ArrayQueueObj
    HashMapObj
    UnionFindSetObj
)

list( APPEND UNDIGRAPH_OBJ
    $<TARGET_OBJECTS:UndigraphObj>
    ${ARRAYSTACK_OBJ}
    ${ARRAYQUEUE_OBJ}
    ${HASHMAP_OBJ}
)

set( UNDIGRAPH_OBJ ${UNDIGRAPH_OBJ} PARENT_SCOPE )

# 生成静态库
add_library( UndigraphLib STATIC ${UNDIGRAPH_OBJ} )