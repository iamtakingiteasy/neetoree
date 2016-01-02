set(NEETOREE_SOURCES
        ${CMAKE_SOURCE_DIR}/src-gen/DBUSAPI.c
)
add_library(neetoree ${NEETOREE_SOURCES})

add_executable(neetoreeapp main.c)
target_link_libraries(neetoreeapp
        neetoree
        ${GLIB_LIBRARIES}
        ${GLIB_GIO_LIBRARIES}
        ${GLIB_GIO_UNIX_LIBRARIES}
        ${GLIB_GOBJECT_LIBRARIES}
)