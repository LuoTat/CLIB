# 添加源文件
list( APPEND HASHMAP_SRC HashMap.c )

# 生成OBJ文件
add_library( HashMapObj OBJECT ${HASHMAP_SRC} )

# 添加依赖OBJ文件
add_dependencies( HashMapObj IteratorObj )

list( APPEND HASHMAP_OBJ
    $<TARGET_OBJECTS:HashMapObj>
)
list( APPEND HASHMAP_OBJ
    ${ITERATOR_OBJ}
)

message( STATUS "Value of HASHMAP_OBJ: ${HASHMAP_OBJ}" )
message( STATUS "Value of ITERATOR_OBJ2: ${ITERATOR_OBJ}" )

set( HASHMAP_OBJ ${HASHMAP_OBJ} PARENT_SCOPE )

# 生成静态库
add_library( HashMapLib STATIC ${HASHMAP_OBJ} )