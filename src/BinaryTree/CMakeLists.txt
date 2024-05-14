# 添加源文件
list( APPEND BINARYTREE_SRC BinaryTree.c )

# 生成OBJ文件
add_library( BinaryTreeObj OBJECT ${BINARYTREE_SRC} )

# 添加依赖OBJ文件
add_dependencies( BinaryTreeObj BinaryTreeUtilsObj )

list( APPEND BINARYTREE_OBJ
    $<TARGET_OBJECTS:BinaryTreeObj>
    ${BINARYTREEUTILS_OBJ}
)

set( BINARYTREE_OBJ ${BINARYTREE_OBJ} PARENT_SCOPE )

# 生成静态库
add_library( BinaryTreeLib STATIC ${BINARYTREE_OBJ} )