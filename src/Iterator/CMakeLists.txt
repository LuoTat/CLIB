# 添加源文件
list( APPEND ITERATOR_SRC Iterator.c )

# 生成OBJ文件
add_library( IteratorObj OBJECT ${ITERATOR_SRC} )

list( APPEND ITERATOR_OBJ
    $<TARGET_OBJECTS:IteratorObj>
)
set( ITERATOR_OBJ ${ITERATOR_OBJ} PARENT_SCOPE )
message( STATUS "Value of ITERATOR_OBJ1: ${ITERATOR_OBJ}" )
