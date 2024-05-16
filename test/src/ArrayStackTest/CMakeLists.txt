# 输出ArrayStackTest的可执行文件
add_executable( ArrayStackTest ArrayStackTest.cpp )

# 链接HashMapLib库
target_link_libraries( ArrayStackTest ArrayStackLib )