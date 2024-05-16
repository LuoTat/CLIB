# 生成Lib文件
add_library( ArrayListLib STATIC )

# 添加源文件
target_sources( ArrayListLib PRIVATE ArrayList.c )

# 添加依赖
target_link_libraries( ArrayListLib PUBLIC
    PredefinedHead
    IteratorLib
)

# 添加头文件
target_include_directories( ArrayListLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )