cmake_minimum_required(VERSION 3.3)
project(neetoree)

add_definitions(-g)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

find_package(GLIB REQUIRED COMPONENTS gio gio-unix gobject)

include_directories(support/util support/parser src-gen ${GLIB_INCLUDE_DIRS} ${GLIB_GIO_UNIX_INCLUDE_DIR})

add_subdirectory(src-gen)
add_subdirectory(src)
add_subdirectory(support)