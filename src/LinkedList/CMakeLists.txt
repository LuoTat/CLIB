# 输出LinkedList的.a文件

list( APPEND LINKEDLIST_SRC LinkedList.c ) # 添加源文件
list( APPEND LINKEDLIST_SRC ../Iterator/Iterator.c ) # 添加头文件

add_library( LinkedListLib STATIC ${LINKEDLIST_SRC} ) # 生成lib文件

set_target_properties( LinkedListLib PROPERTIES OUTPUT_NAME "LinkedList" ) # 修改lib文件的输出名字
