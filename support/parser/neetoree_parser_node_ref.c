//
// Created by user on 12/8/15.
//

#include <stdlib.h>
#include <string.h>
#include "neetoree_parser_compile.h"

typedef struct {
    char *name;
    neetoree_parser_compiled_rule_t *rule;
} neetoree_parser_node_ref_search_t;

static NEETOREE_PTRLIST_WALKER(findmatch) {
    neetoree_parser_node_ref_search_t *search = context;
    neetoree_parser_compiled_rule_t *rule = item;
    if (strcmp(rule->origin->name, search->name) == 0) {
        search->rule = rule;
        return NEETOREE_STATUS_STOP;
    }
    return NEETOREE_STATUS_CONT;
}

NEETOREE_PARSER_SPEC_NEW(ref) {
    neetoree_parser_node_ref_search_t search;
    search.name = spec;
    search.rule = NULL;
    neetoree_ptrlist_walk(grammar->rules, NEETOREE_PTRLIST_WALKER_NAME(findmatch), &search);
    return search.rule;
}

NEETOREE_PARSER_SPEC_FREE(ref) {
    // do nothing
}

NEETOREE_PARSER_SPEC_APPLY(ref) {
    neetoree_parser_compiled_rule_t *rule = data;
    applyctx->type = rule->origin->containment;
    return rule->iface->applyfunc(rule->children, applyctx);
}