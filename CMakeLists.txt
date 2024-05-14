# 设置cmake的最低版本
cmake_minimum_required( VERSION 3.25 )

# 设置工程名称和版本
project( LTTLIB VERSION 1.0.0 LANGUAGES C CXX )

# # 编译器的参数
# SET( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -Wall -ggdb" )
# SET( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall" )
# SET( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall -ggdb" )
# SET( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall" )
if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Release )
    message( STATUS "未指定编译类型,设置默认编译类型:Release" )
endif()

message( STATUS "Build type:               ${CMAKE_BUILD_TYPE}" )

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    message( STATUS "C编译器参数:                ${CMAKE_C_FLAGS_DEBUG}" )
    message( STATUS "CXX编译器参数:                ${CMAKE_CXX_FLAGS_DEBUG}" )
elseif( CMAKE_BUILD_TYPE STREQUAL "Release" )
    message( STATUS "C编译器参数:                ${CMAKE_C_FLAGS_RELEASE}" )
    message( STATUS "CXX编译器参数:              ${CMAKE_CXX_FLAGS_RELEASE}" )
endif()

# CMAKE_ARCHIVE_OUTPUT_DIRECTORY：存放静态库的文件夹位置
# CMAKE_RUNTIME_OUTPUT_DIRECTORY：存放可执行软件的目录
# CMAKE_LIBRARY_OUTPUT_DIRECTORY：存放动态库的文件夹位置
# LIBRARY_OUTPUT_PATH：默认存放库文件的位置，如果产生的是静态库并且没有指定 CMAKE_ARCHIVE_OUTPUT_DIRECTORY 则存放在该目录下，动态库也类似
# EXECUTABLE_OUTPUT_PATH：默认存放可执行文件的位置，如果没有指定 CMAKE_RUNTIME_OUTPUT_DIRECTORY 则存放在该目录下

# link_directories( ${PROJECT_SOURCE_DIR}/LTTLIB/Debug/lib )
# link_directories( ${PROJECT_SOURCE_DIR}/LTTLIB/Release/lib )
add_subdirectory( src )

# add_subdirectory( test )