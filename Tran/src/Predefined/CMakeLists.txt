add_library( PredefinedHead INTERFACE )
target_sources( PredefinedHead INTERFACE
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/Predefined.h
)