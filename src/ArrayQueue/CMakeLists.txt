# 生成Lib文件
add_library( ArrayQueueLib STATIC )

# 添加源文件
target_sources( ArrayQueueLib PRIVATE ArrayQueue.c )

# 添加依赖
target_link_libraries( ArrayQueueLib PUBLIC
    PredefinedHead
    IteratorLib
)

# 添加头文件
target_include_directories( ArrayQueueLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )