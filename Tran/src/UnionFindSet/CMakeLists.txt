# 生成Lib文件
add_library( UnionFindSetLib STATIC )

# 添加源文件
target_sources( UnionFindSetLib PRIVATE UnionFindSet.c )

# 添加依赖
target_link_libraries( UnionFindSetLib PUBLIC
    HashMapLib
)

# 添加头文件
target_include_directories( UnionFindSetLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
