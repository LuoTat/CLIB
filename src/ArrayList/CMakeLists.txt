# 添加源文件
list( APPEND ARRAYLIST_SRC ArrayList.c )

# 生成OBJ文件
add_library( ArrayListObj OBJECT ${ARRAYLIST_SRC} )

# 添加依赖OBJ文件
add_dependencies( ArrayListObj IteratorObj )

list( APPEND ARRAYLIST_OBJ
    $<TARGET_OBJECTS:ArrayListObj>
    ${ITERATOR_OBJ}
)

set( ARRAYLIST_OBJ ${ARRAYLIST_OBJ} PARENT_SCOPE )

# 生成静态库
add_library( ArrayListLib STATIC ${ARRAYLIST_OBJ} )