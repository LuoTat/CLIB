# 输出ArrayStack的.a文件

# 添加源文件
list( APPEND ARRAYSTACK_SRC ArrayStack.c )
list( APPEND ARRAYSTACK_SRC ../Iterator/Iterator.c )

# 生成静态库
add_library( ArrayStackLib STATIC ${ARRAYSTACK_SRC} )

set_target_properties( ArrayStackLib PROPERTIES OUTPUT_NAME "ArrayStack" ) # 修改lib文件的输出名字
