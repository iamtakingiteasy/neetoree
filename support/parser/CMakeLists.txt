set(NEETOREE_PARSER_SOURCES
        neetoree_parser_main.c
        neetoree_parser_peg.c
        neetoree_parser_valuestack.c
        neetoree_parser_compile.c
        neetoree_parser_node_ref.c
        neetoree_parser_node_literal.c
        neetoree_parser_node_range.c
        neetoree_parser_node_dot.c
        neetoree_parser_node_group.c
        neetoree_parser_node_none.c
        neetoree_parser_node_pred.c
        neetoree_parser_node_action.c
        neetoree_parser_node_begin.c
        neetoree_parser_node_end.c
        neetoree_parser_builder.c)
add_executable(neetoree_parser ${NEETOREE_PARSER_SOURCES})
target_link_libraries(neetoree_parser neetoree_util)

if (${DEBUG})
    add_definitions(-DDEBUG)
endif(${DEBUG})
add_definitions(-DDEBUG)