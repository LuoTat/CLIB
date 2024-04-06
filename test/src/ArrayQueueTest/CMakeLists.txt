# 输出ArrayQueueTest的可执行文件

add_executable( ArrayQueueTest ArrayQueueTest.cpp ) # 生成可执行文件

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    target_link_libraries( ArrayQueueTest ArrayQueue_d ) # 添加库文件路径
elseif( CMAKE_BUILD_TYPE STREQUAL "Release" )
    target_link_libraries( ArrayQueueTest ArrayQueue ) # 添加库文件路径
endif()