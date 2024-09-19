# 生成Lib文件
add_library( IteratorLib STATIC )

# 添加源文件
target_sources( IteratorLib PRIVATE Iterator.c )

# 添加头文件
target_include_directories( IteratorLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )