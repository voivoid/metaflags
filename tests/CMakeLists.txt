include(CheckCXXSourceCompiles)

add_executable(metaflags_tests tests.cpp msgbox.cpp)
target_link_libraries(metaflags_tests PRIVATE metaflags::lib)

get_target_property(metaflags_include_dirs metaflags INTERFACE_INCLUDE_DIRECTORIES)
set(CMAKE_REQUIRED_INCLUDES ${metaflags_include_dirs} ${CMAKE_CURRENT_SOURCE_DIR})