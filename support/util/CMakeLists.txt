set(NEETOREE_UTIL_SOURCES
        neetoree_avltree.c
        neetoree_ptrtree.c
        neetoree_ptrlist.c
        neetoree_ptrstack.c
        neetoree_stream.c
        neetoree_string.c
)
add_library(neetoree_util ${NEETOREE_UTIL_SOURCES})