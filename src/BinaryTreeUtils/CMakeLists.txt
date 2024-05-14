# 添加源文件
list( APPEND BINARYTREEUTILS_SRC BinaryTreeUtils.c )

# 生成OBJ文件
add_library( BinaryTreeUtilsObj OBJECT ${BINARYTREEUTILS_SRC} )

set( BINARYTREEUTILS_OBJ $<TARGET_OBJECTS:BinaryTreeUtilsObj> PARENT_SCOPE )