//
// Created by user on 12/9/15.
//

#include <stdlib.h>
#include <neetoree_string.h>
#include "neetoree_parser_compile.h"

static NEETOREE_PTRLIST_WALKER(choice) {
    neetoree_parser_applyctx_t *applyctx = context;
    neetoree_parser_compiled_node_t *node = item;
    applyctx->result = neetoree_parser_applywrap(node, applyctx);
    if (applyctx->result == NEETOREE_RESULT_SUCCESS) {
        return NEETOREE_STATUS_STOP;
    }
    return NEETOREE_STATUS_CONT;
}

static NEETOREE_PTRLIST_WALKER(sequence) {
    neetoree_parser_applyctx_t *applyctx = context;
    neetoree_parser_compiled_node_t *node = item;
    applyctx->result = neetoree_parser_applywrap(node, applyctx);
    if (applyctx->result == NEETOREE_RESULT_FAILURE) {
        return NEETOREE_STATUS_STOP;
    }
    return NEETOREE_STATUS_CONT;
}

static NEETOREE_FREE(node) {
    neetoree_parser_compiled_node_t *node = item;
    neetoree_avltree_free(node->cache);
    node->iface->freefunc(node->data);
    free(node);
}

NEETOREE_PARSER_SPEC_NEW(group) {
    neetoree_parser_node_t *current = origin->children;
    neetoree_parser_node_t *child = current;
    neetoree_ptrlist_t *children = neetoree_ptrlist_new(NEETOREE_FREE_NAME(node));
    while (child && child->type) {
        neetoree_parser_compile_node(child, children, grammar);
        child++;
    }
    return children;
}

NEETOREE_PARSER_SPEC_FREE(group) {
    NEETOREE_FREE_NAME(ptrlist)(data);
}

NEETOREE_PARSER_SPEC_APPLY(group) {
    void *state_before = applyctx->grammar->valuestack->next;
    size_t level = applyctx->grammar->valuestack->global->depth;
    neetoree_ptrlist_t *children = data;
    size_t matchlen = applyctx->grammar->match->len;
    if (applyctx->type == NEETOREE_PARSER_NODE_TYPE_CHOICE) {
        applyctx->result = NEETOREE_RESULT_FAILURE;
        neetoree_ptrlist_walk(children, NEETOREE_PTRLIST_WALKER_NAME(choice), applyctx);
        if (applyctx->result == NEETOREE_RESULT_NONE) {
            applyctx->result = NEETOREE_RESULT_FAILURE;
        }
    } else if (applyctx->type == NEETOREE_PARSER_NODE_TYPE_SEQUENCE) {
        applyctx->result = NEETOREE_RESULT_SUCCESS;
        neetoree_ptrlist_walk(children, NEETOREE_PTRLIST_WALKER_NAME(sequence), applyctx);
        if (applyctx->result == NEETOREE_RESULT_NONE) {
            applyctx->result = NEETOREE_RESULT_SUCCESS;
        }
    }

    if (applyctx->result == NEETOREE_RESULT_FAILURE && applyctx->grammar->match->len != matchlen) {
        applyctx->grammar->match->len = matchlen;
    }

    if (applyctx->result == NEETOREE_RESULT_FAILURE && applyctx->grammar->valuestack->global->depth != level) {
        printf("drop\n");
        neetoree_parser_valuestack_drop(applyctx->grammar->valuestack, level);
    }

    // guard
    void *state_after = applyctx->grammar->valuestack->next;
    if (state_before != state_after) {
        applyctx->error = neetoree_string_dup_new("valuestack mismatch");
        return NEETOREE_RESULT_ERROR;
    }

    return applyctx->result;
}