# 添加源文件
list( APPEND ARRAYSTACK_SRC ArrayStack.c )

# 生成OBJ文件
add_library( ArrayStackObj OBJECT ${ARRAYSTACK_SRC} )

# 添加依赖OBJ文件
add_dependencies( ArrayStackObj IteratorObj )

list( APPEND ARRAYSTACK_OBJ
    $<TARGET_OBJECTS:ArrayStackObj>
    ${ITERATOR_OBJ}
)

set( ARRAYSTACK_OBJ ${ARRAYSTACK_OBJ} PARENT_SCOPE )

# 生成静态库
add_library( ArrayStackLib STATIC ${ARRAYSTACK_OBJ} )