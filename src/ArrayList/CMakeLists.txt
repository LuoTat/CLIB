# 输出ArrayList的.a文件

# 添加源文件
list( APPEND ARRAYLIST_SRC ArrayList.c )
list( APPEND ARRAYLIST_SRC ../Iterator/Iterator.c )

# 生成静态库
add_library( ArrayListLib STATIC ${ARRAYLIST_SRC} )

set_target_properties( ArrayListLib PROPERTIES OUTPUT_NAME "ArrayList" ) # 修改lib文件的输出名字
