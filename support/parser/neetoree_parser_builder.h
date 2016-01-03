//
// Created by user on 12/19/15.
//

#ifndef NEETOREE_NEETOREE_PARSER_BUILDER_H
#define NEETOREE_NEETOREE_PARSER_BUILDER_H

#include <neetoree_ptrlist.h>
#include <neetoree_string.h>
#include "neetoree_parser.h"

typedef enum {
    NEETOREE_PARSER_BUILDER_TYPE_ROOT,
    NEETOREE_PARSER_BUILDER_TYPE_NODE,
    NEETOREE_PARSER_BUILDER_TYPE_CHCE,
    NEETOREE_PARSER_BUILDER_TYPE_RULE
} NeetoreeParserBuilderType;

typedef struct {
    NeetoreeParserBuilderType      buildtype;
    neetoree_string_t             *spec;
    neetoree_string_t             *reference;
    NeetoreeParserNodePredicate    predicate;
    NeetoreeParserNodeType         type;
    NeetoreeParserNodeCardinality  cardinality;
    NeetoreeParserNodeType         chain;
    neetoree_ptrlist_t            *children;
    size_t                         speclength;
} neetoree_parser_builder_node_t;

typedef struct {
    neetoree_ptrlist_t             *actions;
    neetoree_ptrlist_t             *groups;
    neetoree_parser_builder_node_t *rules;
} neetoree_parser_builder_context_t;

typedef struct {
    neetoree_string_t *out;
    size_t             speclen;
} neetoree_parser_builder_render_t;

#define MAXLEN(a,b) ((a) > (b) ? (a) : (b))
NEETOREE_FREE(builder_node);
neetoree_parser_builder_context_t *neetoree_parser_builder_new();
void neetoree_parser_builder_free(neetoree_parser_builder_context_t *builder);
neetoree_parser_builder_node_t *neetoree_parser_builder_node_new(NeetoreeParserBuilderType type);
neetoree_string_t *neetoree_parser_builder_name(neetoree_parser_builder_node_t *parent, size_t ord);
neetoree_string_t *neetoree_parser_builder_render(char *name, char *include, neetoree_parser_builder_context_t *ctx);
void neetoree_parser_builder_spec_maxlen(neetoree_parser_builder_node_t *child, neetoree_parser_builder_node_t *parent);
size_t neetoree_parser_builder_spec_count(char *str, NeetoreeParserNodeType type);
void neetoree_parser_builder_compact(neetoree_parser_builder_node_t *rule, neetoree_parser_builder_context_t *ctx);

#endif //NEETOREE_NEETOREE_PARSER_BUILDER_H
