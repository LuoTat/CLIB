# 生成Lib文件
add_library( HashMapLib STATIC )

# 添加源文件
target_sources( HashMapLib PRIVATE HashMap.c )

# 添加依赖
target_link_libraries( HashMapLib PUBLIC
    PredefinedHead
    IteratorLib
)

# 添加头文件
target_include_directories( HashMapLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )