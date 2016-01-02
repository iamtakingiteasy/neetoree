//
// Created by user on 11/22/15.
//

#ifndef NEETOREE_NEETOREE_PARSER_COMPILE_H
#define NEETOREE_NEETOREE_PARSER_COMPILE_H

#include <neetoree_string.h>
#include <neetoree_avltree.h>
#include "neetoree_parser.h"

struct neetoree_parser_applyctx_t;
struct neetoree_parser_grammar_t;

#define NEETOREE_PARSER_SPEC_NEW_ARGS \
    void *spec, \
    neetoree_parser_node_t *origin, \
    struct neetoree_parser_grammar_t *grammar
#define NEETOREE_PARSER_SPEC_NEW_NAME(name) neetoree_parser_## name ##_spec_new
#define NEETOREE_PARSER_SPEC_NEW(name) void *NEETOREE_PARSER_SPEC_NEW_NAME(name)(NEETOREE_PARSER_SPEC_NEW_ARGS)

#define NEETOREE_PARSER_SPEC_FREE_ARGS \
    void *data
#define NEETOREE_PARSER_SPEC_FREE_NAME(name) neetoree_parser_## name ##_spec_free
#define NEETOREE_PARSER_SPEC_FREE(name) void NEETOREE_PARSER_SPEC_FREE_NAME(name)(NEETOREE_PARSER_SPEC_FREE_ARGS)

#define NEETOREE_PARSER_SPEC_APPLY_ARGS \
    void *data, \
    struct neetoree_parser_applyctx_t *applyctx
#define NEETOREE_PARSER_SPEC_APPLY_NAME(name) neetoree_parser_## name ##_spec_apply
#define NEETOREE_PARSER_SPEC_APPLY(name) NeetoreeResult NEETOREE_PARSER_SPEC_APPLY_NAME(name)(NEETOREE_PARSER_SPEC_APPLY_ARGS)

typedef void          *(*neetoree_parser_spec_new_func)(NEETOREE_PARSER_SPEC_NEW_ARGS);
typedef void           (*neetoree_parser_spec_free_func)(NEETOREE_PARSER_SPEC_FREE_ARGS);
typedef NeetoreeResult (*neetoree_parser_spec_apply_func)(NEETOREE_PARSER_SPEC_APPLY_ARGS);

#define NEETOREE_PARSER_SPEC_FUNC(name) \
    NEETOREE_PARSER_SPEC_NEW(name); \
    NEETOREE_PARSER_SPEC_FREE(name); \
    NEETOREE_PARSER_SPEC_APPLY(name)

NEETOREE_PARSER_SPEC_FUNC(none);
NEETOREE_PARSER_SPEC_FUNC(ref);
NEETOREE_PARSER_SPEC_FUNC(literal);
NEETOREE_PARSER_SPEC_FUNC(range);
NEETOREE_PARSER_SPEC_FUNC(dot);
NEETOREE_PARSER_SPEC_FUNC(group);
NEETOREE_PARSER_SPEC_FUNC(pred);
NEETOREE_PARSER_SPEC_FUNC(action);
NEETOREE_PARSER_SPEC_FUNC(begin);
NEETOREE_PARSER_SPEC_FUNC(end);

#undef NEETOREE_PARSER_SPEC_FUNC

typedef enum {
    NEETOREE_PARSER_ERROR_EOF         = 1,
    NEETOREE_PARSER_ERROR_CARDINALITY = 2,
    NEETOREE_PARSER_ERROR_NOCHILDREN  = 3
} NeetoreeParserError;

typedef struct {
    neetoree_parser_spec_new_func newfunc;
    neetoree_parser_spec_free_func freefunc;
    neetoree_parser_spec_apply_func applyfunc;
} neetoree_parser_compiled_iface_t;

typedef struct neetoree_parser_compiled_node_t {
    neetoree_parser_compiled_iface_t       *iface;
    neetoree_avltree_t                     *cache;
    void                                   *data;
    NeetoreeParserNodeCardinality           cardinality;
    NeetoreeParserNodeType                  type;
    NeetoreeParserNodePredicate             pred;
    // debug
    neetoree_parser_node_t                 *origin;
} neetoree_parser_compiled_node_t;

typedef struct neetoree_parser_compiled_rule_t {
    neetoree_parser_rule_t                 *origin;
    neetoree_ptrlist_t                     *children;
    neetoree_parser_compiled_iface_t       *iface;
} neetoree_parser_compiled_rule_t;

typedef struct {
    size_t             pos;
    neetoree_string_t *match;
    NeetoreeResult     result;
} neetoree_parser_compiled_cache_t;

typedef struct neetoree_parser_applyctx_t {
    neetoree_parser_grammar_t         *grammar;
    neetoree_stream_t                 *stream;
    NeetoreeStatus                     status;
    NeetoreeResult                     result;
    NeetoreeParserNodeType             type;
    struct neetoree_parser_applyctx_t *parent;
    neetoree_string_t                 *error;
    neetoree_string_t                 *match;
} neetoree_parser_applyctx_t;

neetoree_parser_compiled_node_t *neetoree_parser_compile_node(neetoree_parser_node_t *origin, neetoree_ptrlist_t *container, neetoree_parser_grammar_t *grammar);
NeetoreeResult neetoree_parser_applywrap(neetoree_parser_compiled_node_t *node, neetoree_parser_applyctx_t *parent);

#endif //NEETOREE_NEETOREE_PARSER_COMPILE_H