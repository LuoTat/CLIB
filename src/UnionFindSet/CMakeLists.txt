# 添加源文件
list( APPEND UNIONFINDSET_SRC UnionFindSet.c )

# 生成OBJ文件
add_library( UnionFindSetObj OBJECT ${UNIONFINDSET_SRC} )

# 添加依赖OBJ文件
# add_dependencies( UnionFindSetObj HashMapObj )

# 生成静态库
list( APPEND UNIONFINDSET_OBJ
    $<TARGET_OBJECTS:UnionFindSetObj>
)
list( APPEND UNIONFINDSET_OBJ
    ${HASHMAP_OBJ}
)
set( UNIONFINDSET_OBJ ${UNIONFINDSET_OBJ} PARENT_SCOPE )

message( STATUS "Value of UNIONFINDSET_OBJ: ${UNIONFINDSET_OBJ}" )

add_library( UnionFindSetLib STATIC ${UNIONFINDSET_OBJ} )

add_dependencies( UnionFindSetLib UnionFindSetObj )