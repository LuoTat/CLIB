# 输出HashMap的.a文件

# 添加源文件
list( APPEND HASHMAP_SRC HashMap.c )
list( APPEND HASHMAP_SRC ../Iterator/Iterator.c )

# 生成静态库
add_library( HashMapLib STATIC ${HASHMAP_SRC} )

set_target_properties( HashMapLib PROPERTIES OUTPUT_NAME "HashMap" ) # 修改lib文件的输出名字
