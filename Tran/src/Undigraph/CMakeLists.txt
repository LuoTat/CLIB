# 生成Lib文件
add_library( UndigraphLib STATIC )

# 添加源文件
target_sources( UndigraphLib PRIVATE Undigraph.c )

# 添加依赖
target_link_libraries( UndigraphLib PUBLIC
    ArrayStackLib
    ArrayQueueLib
    HashMapLib
    UnionFindSetLib
)

# 添加头文件
target_include_directories( UndigraphLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )