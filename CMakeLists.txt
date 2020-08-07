cmake_minimum_required(VERSION 3.18)

project(metaflags LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/hana.cmake)

add_library(metaflags INTERFACE)
add_library(metaflags::lib ALIAS metaflags)
target_include_directories(metaflags INTERFACE inc)
target_link_libraries(metaflags INTERFACE boost::hana)

target_sources(metaflags INTERFACE inc/metaflags/metaflags.hpp)

add_subdirectory(tests)

