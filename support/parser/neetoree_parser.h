//
// Created by user on 11/22/15.
//

#ifndef NEETOREE_NEETOREE_PARSER_H
#define NEETOREE_NEETOREE_PARSER_H

#include <neetoree_ptrlist.h>
#include <neetoree_string.h>
#include <neetoree_stream.h>
#include <neetoree_common.h>
#include "neetoree_parser_valuestack.h"

struct neetoree_parser_grammar_t;

typedef enum {
    NEETOREE_PARSER_NODE_TYPE_NONE      = 0,
    NEETOREE_PARSER_NODE_TYPE_RULE      = 1,
    NEETOREE_PARSER_NODE_TYPE_LITERAL   = 2,
    NEETOREE_PARSER_NODE_TYPE_RANGE     = 3,
    NEETOREE_PARSER_NODE_TYPE_DOT       = 4,
    NEETOREE_PARSER_NODE_TYPE_SEQUENCE  = 5,
    NEETOREE_PARSER_NODE_TYPE_CHOICE    = 6,
    NEETOREE_PARSER_NODE_TYPE_REF       = 7,
    NEETOREE_PARSER_NODE_TYPE_PRED      = 8,
    NEETOREE_PARSER_NODE_TYPE_ACTION    = 9,
    NEETOREE_PARSER_NODE_TYPE_BEGIN     = 10,
    NEETOREE_PARSER_NODE_TYPE_END       = 11
} NeetoreeParserNodeType;

typedef enum {
    NEETOREE_PARSER_NODE_CARDINALITY_NONE       = 0,
    NEETOREE_PARSER_NODE_CARDINALITY_ONE        = 1,
    NEETOREE_PARSER_NODE_CARDINALITY_ONEORMORE  = 2,
    NEETOREE_PARSER_NODE_CARDINALITY_ZEROORMORE = 3,
    NEETOREE_PARSER_NODE_CARDINALITY_ZEROORONE  = 4
} NeetoreeParserNodeCardinality;

typedef enum {
    NEETOREE_PARSER_NODE_PREDICATE_NONE     = 0,
    NEETOREE_PARSER_NODE_PREDICATE_POSITIVE = 1,
    NEETOREE_PARSER_NODE_PREDICATE_NEGATIVE = 2,
} NeetoreeParserNodePredicate;

#define NEETOREE_PARSER_PRED_HANDLER_ARGS \
    struct neetoree_parser_grammar_t *grammar, \
    neetoree_string_t *match, \
    neetoree_ptrstack_t *valuestack, \
    void *context

#define NEETOREE_PARSER_PRED_HANDLER_NAME(name) neetoree_parser_## name ##_pred_handler
#define NEETOREE_PARSER_PRED_HANDLER(name) NeetoreeResult NEETOREE_PARSER_PRED_HANDLER_NAME(name)(NEETOREE_PARSER_PRED_HANDLER_ARGS)
typedef NeetoreeResult (*neetoree_parser_pred_handler)(NEETOREE_PARSER_PRED_HANDLER_ARGS);

#define NEETOREE_PARSER_ACTION_HANDLER_ARGS \
    struct neetoree_parser_grammar_t *grammar, \
    neetoree_string_t *match, \
    neetoree_ptrstack_t *valuestack, \
    void *context

#define NEETOREE_PARSER_ACTION_HANDLER_NAME(name) neetoree_parser_## name ##_action_handler
#define NEETOREE_PARSER_ACTION_HANDLER(name) void NEETOREE_PARSER_ACTION_HANDLER_NAME(name)(NEETOREE_PARSER_ACTION_HANDLER_ARGS)
typedef void (*neetoree_parser_action_handler)(NEETOREE_PARSER_ACTION_HANDLER_ARGS);

#define NEETOREE_PARSER_NODE(type, cardinality, pred, spec, children) { NEETOREE_PARSER_NODE_TYPE_ ## type, NEETOREE_PARSER_NODE_CARDINALITY_ ## cardinality, NEETOREE_PARSER_NODE_PREDICATE_ ## pred, spec, children }
#define NEETOREE_PARSER_NODE_BEGIN        NEETOREE_PARSER_NODE(BEGIN, NONE, NONE, NULL, NULL)
#define NEETOREE_PARSER_NODE_END          NEETOREE_PARSER_NODE(END, NONE, NONE, NULL, NULL)
#define NEETOREE_PARSER_NODE_ACTION(name) NEETOREE_PARSER_NODE(ACTION, NONE, NONE, NEETOREE_PARSER_ACTION_HANDLER_NAME(name), NULL)
#define NEETOREE_PARSER_NODE_PRED(name)   NEETOREE_PARSER_NODE(PRED, NONE, NONE, NEETOREE_PARSER_ACTION_HANDLER_NAME(name), NULL)
#define NEETOREE_PARSER_NODE_TERM         NEETOREE_PARSER_NODE(NONE, NONE, NONE, NULL, NULL)
typedef struct neetoree_parser_node_t {
    int                            type;
    int                            cardinality;
    int                            predicate;
    void                          *spec;
    struct neetoree_parser_node_t *children;
} neetoree_parser_node_t;

#define NEETOREE_PARSER_RULE(name, cont, children) { name, NEETOREE_PARSER_NODE_TYPE_ ## cont, children }
#define NEETOREE_PARSER_RULE_TERM NEETOREE_PARSER_RULE(NULL, NONE, NULL)
typedef struct {
    char                         *name;
    NeetoreeParserNodeType        containment;
    neetoree_parser_node_t       *children;
} neetoree_parser_rule_t;

struct neetoree_parser_compiled_node_t;

typedef struct neetoree_parser_grammar_t {
    neetoree_parser_valuestack_t           *valuestack;
    neetoree_ptrlist_t                     *rules;
    void                                   *context;
    struct neetoree_parser_compiled_node_t *entry;
    neetoree_string_t                      *match;
    neetoree_string_t                      *buffer;
    int                                     matchoz;
} neetoree_parser_grammar_t;

neetoree_string_t *neetoree_parser_peg(char *name, char *include, neetoree_stream_t *stream);
neetoree_parser_grammar_t *neetoree_parser_compile(neetoree_parser_rule_t *rules);
void neetoree_parser_free(neetoree_parser_grammar_t *grammar);
neetoree_parser_grammar_t *neetoree_parser_new();
NeetoreeResult neetoree_parser_run(neetoree_parser_grammar_t *grammar, neetoree_stream_t *stream, void *context);

#endif //NEETOREE_NEETOREE_PARSER_H