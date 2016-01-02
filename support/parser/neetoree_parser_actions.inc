//
// Created by user on 1/1/16.
//

#include "neetoree_parser_builder.h"

#define MAXLEN(a,b) ((a) > (b) ? (a) : (b))
static NEETOREE_PARSER_ACTION_HANDLER(set_spec) {
#ifdef DEBUG
    printf("set '%s'\n", match->data);
#endif
    neetoree_parser_builder_node_t *node = neetoree_parser_valuestack_peek(valuestack, 0);
    neetoree_parser_builder_node_t *parent = neetoree_parser_valuestack_peek(valuestack, 1);
    node->spec = neetoree_string_dup_new(match->data);
    if (node->type == NEETOREE_PARSER_NODE_TYPE_ACTION) {
        parent->speclength = MAXLEN(4, parent->speclength);
    } else {
        parent->speclength = MAXLEN(node->spec->len, parent->speclength);
    }

}

static NEETOREE_PARSER_ACTION_HANDLER(type_ref) {
#ifdef DEBUG
    printf("type ref\n");
#endif
    neetoree_parser_builder_node_t *parent = neetoree_parser_valuestack_peek(valuestack, 0);
    parent->type = NEETOREE_PARSER_NODE_TYPE_REF;
}

static NEETOREE_PARSER_ACTION_HANDLER(type_literal) {
#ifdef DEBUG
    printf("type literal\n");
#endif
    neetoree_parser_builder_node_t *parent = neetoree_parser_valuestack_peek(valuestack, 0);
    parent->type = NEETOREE_PARSER_NODE_TYPE_LITERAL;
}

static NEETOREE_PARSER_ACTION_HANDLER(type_class) {
#ifdef DEBUG
    printf("type class\n");
#endif
    neetoree_parser_builder_node_t *parent = neetoree_parser_valuestack_peek(valuestack, 0);
    parent->type = NEETOREE_PARSER_NODE_TYPE_RANGE;
}

static NEETOREE_PARSER_ACTION_HANDLER(type_dot) {
#ifdef DEBUG
    printf("type dot\n");
#endif
    neetoree_parser_builder_node_t *parent = neetoree_parser_valuestack_peek(valuestack, 0);
    parent->type = NEETOREE_PARSER_NODE_TYPE_DOT;
}

static NEETOREE_PARSER_ACTION_HANDLER(type_action) {
#ifdef DEBUG
    printf("type action\n");
#endif
    neetoree_parser_builder_node_t *parent = neetoree_parser_valuestack_peek(valuestack, 0);
    parent->type = NEETOREE_PARSER_NODE_TYPE_ACTION;
}

static NEETOREE_PARSER_ACTION_HANDLER(type_begin) {
#ifdef DEBUG
    printf("type begin\n");
#endif
    neetoree_parser_builder_node_t *parent = neetoree_parser_valuestack_peek(valuestack, 0);
    parent->type = NEETOREE_PARSER_NODE_TYPE_BEGIN;
}

static NEETOREE_PARSER_ACTION_HANDLER(type_end) {
#ifdef DEBUG
    printf("type end\n");
#endif
    neetoree_parser_builder_node_t *parent = neetoree_parser_valuestack_peek(valuestack, 0);
    parent->type = NEETOREE_PARSER_NODE_TYPE_END;
}

static NEETOREE_PARSER_ACTION_HANDLER(group_start) {
#ifdef DEBUG
    printf("group start\n");
#endif
    neetoree_parser_builder_node_t *node = neetoree_parser_valuestack_peek(valuestack, 0);
    node->chain = NEETOREE_PARSER_NODE_TYPE_SEQUENCE;
    neetoree_parser_builder_node_t *parent = neetoree_parser_valuestack_peek(valuestack, 1);
    if (node->spec == NULL) {
        size_t ord = neetoree_parser_builder_count(parent);
        node->spec = neetoree_parser_builder_name(parent->spec->data, ord + 1);
    }
}

static NEETOREE_PARSER_ACTION_HANDLER(group_end) {
#ifdef DEBUG
    printf("group end\n");
#endif
}

static NEETOREE_PARSER_ACTION_HANDLER(cardinality_query) {
#ifdef DEBUG
    printf("cardinality query\n");
#endif
    neetoree_parser_builder_node_t *parent = neetoree_parser_valuestack_peek(valuestack, 0);
    parent->cardinality = NEETOREE_PARSER_NODE_CARDINALITY_ZEROORONE;
}

static NEETOREE_PARSER_ACTION_HANDLER(cardinality_star) {
#ifdef DEBUG
    printf("cardinality star\n");
#endif
    neetoree_parser_builder_node_t *parent = neetoree_parser_valuestack_peek(valuestack, 0);
    parent->cardinality = NEETOREE_PARSER_NODE_CARDINALITY_ZEROORMORE;
}

static NEETOREE_PARSER_ACTION_HANDLER(cardinality_plus) {
#ifdef DEBUG
    printf("cardinality plus\n");
#endif
    neetoree_parser_builder_node_t *parent = neetoree_parser_valuestack_peek(valuestack, 0);
    parent->cardinality = NEETOREE_PARSER_NODE_CARDINALITY_ONEORMORE;
}

static NEETOREE_PARSER_ACTION_HANDLER(pred_and) {
#ifdef DEBUG
    printf("pred and\n");
#endif
    neetoree_parser_builder_node_t *parent = neetoree_parser_valuestack_peek(valuestack, 0);
    parent->predicate = NEETOREE_PARSER_NODE_PREDICATE_POSITIVE;
}

static NEETOREE_PARSER_ACTION_HANDLER(pred_not) {
#ifdef DEBUG
    printf("pred not\n");
#endif
    neetoree_parser_builder_node_t *parent = neetoree_parser_valuestack_peek(valuestack, 0);
    parent->predicate = NEETOREE_PARSER_NODE_PREDICATE_NEGATIVE;
}

static NEETOREE_PARSER_ACTION_HANDLER(terminate_choice) {
#ifdef DEBUG
    printf("terminate choice\n");
#endif
    neetoree_parser_builder_node_t *parent = neetoree_parser_valuestack_peek(valuestack, 0);

    if (parent->chain != NEETOREE_PARSER_NODE_TYPE_CHOICE) {
        return;
    }

    neetoree_parser_builder_node_t *node =  NULL;
    for (size_t i = 0; i < parent->children->length; i++) {
        neetoree_parser_builder_node_t *n = neetoree_ptrlist_access(parent->children, i);

        if (n->buildtype == NEETOREE_PARSER_BUILDER_TYPE_CHCE) {
            continue;
        }

        if (node == NULL && (parent->children->length - i) == 1) {
            n->buildtype = NEETOREE_PARSER_BUILDER_TYPE_CHCE;
            continue;
        }

        if (node == NULL) {
            node = neetoree_parser_builder_node_new(NEETOREE_PARSER_BUILDER_TYPE_CHCE);
            node->chain = NEETOREE_PARSER_NODE_TYPE_SEQUENCE;
        }

        neetoree_ptrlist_add(node->children, n);
    }

    if (node != NULL) {
        neetoree_parser_builder_context_t *ctx = context;

        parent->children->length -= node->children->length;
        size_t ord = neetoree_parser_builder_count(parent);
        node->reference = neetoree_parser_builder_name(parent->spec->data, ord + 1);
        neetoree_ptrlist_add(parent->children, node);
        neetoree_ptrlist_add(ctx->groups, node);
    }
}

static NEETOREE_PARSER_ACTION_HANDLER(set_choice) {
#ifdef DEBUG
    printf("set choice\n");
#endif
    neetoree_parser_builder_node_t *node = neetoree_parser_valuestack_peek(valuestack, 0);
    neetoree_parser_builder_node_t *parent = neetoree_parser_valuestack_peek(valuestack, 1);
    node->chain = NEETOREE_PARSER_NODE_TYPE_CHOICE;

    if (node->spec == NULL) {
        size_t ord = neetoree_parser_builder_count(parent);
        node->spec = neetoree_parser_builder_name(parent->spec->data, ord + 1);
    }
}

static NEETOREE_PARSER_ACTION_HANDLER(node_new) {
#ifdef DEBUG
    printf("new node\n");
#endif
    neetoree_parser_builder_node_t *node = neetoree_parser_builder_node_new(NEETOREE_PARSER_BUILDER_TYPE_NODE);
    neetoree_parser_valuestack_push(valuestack, node, NEETOREE_FREE_NAME(builder_node));
}

static NEETOREE_PARSER_ACTION_HANDLER(node_commit) {
#ifdef DEBUG
    printf("commit node\n");
#endif
    neetoree_parser_builder_node_t *node = neetoree_parser_valuestack_pop(valuestack);
    neetoree_parser_builder_node_t *parent = neetoree_parser_valuestack_peek(valuestack, 0);
    neetoree_parser_builder_context_t *ctx = context;

    neetoree_ptrlist_add(parent->children, node);
    if (node->chain != NEETOREE_PARSER_NODE_TYPE_NONE) {
        node->reference = node->spec;
        node->spec = NULL;
        neetoree_ptrlist_add(ctx->groups, node);
    }

    if (node->type == NEETOREE_PARSER_NODE_TYPE_ACTION) {
        node->reference = neetoree_string_new();
        int siz = snprintf(NULL, 0, "%s_action_%ld", parent->spec->data, ctx->actions->length + 1);
        if (siz > 0) {
            size_t size = (size_t) siz;
            neetoree_string_reserve(node->reference, size+1);
            node->reference->len = (size_t) (snprintf(node->reference->data, size + 1, "%s_action_%ld", parent->spec->data, ctx->actions->length + 1));
            neetoree_ptrlist_add(ctx->actions, node);
        }
    }
}

static NEETOREE_PARSER_ACTION_HANDLER(rule_new) {
#ifdef DEBUG
    printf("new rule\n");
#endif
    neetoree_parser_builder_node_t *rule = neetoree_parser_builder_node_new(NEETOREE_PARSER_BUILDER_TYPE_RULE);
    rule->chain = NEETOREE_PARSER_NODE_TYPE_SEQUENCE;
    rule->type = NEETOREE_PARSER_NODE_TYPE_SEQUENCE;
    neetoree_parser_valuestack_push(valuestack, rule, NEETOREE_FREE_NAME(builder_node));
}

static NEETOREE_PARSER_ACTION_HANDLER(rule_commit) {
#ifdef DEBUG
    printf("commit rule\n");
#endif
    neetoree_parser_builder_node_t *rule = neetoree_parser_valuestack_pop(valuestack);
    neetoree_parser_builder_node_t *root = neetoree_parser_valuestack_peek(valuestack, 0);
    rule->reference = rule->spec;
    rule->spec = NULL;
    neetoree_parser_builder_context_t *ctx = context;
    neetoree_ptrlist_add(ctx->groups, rule);
    neetoree_ptrlist_add(root->children, rule);
}




neetoree_string_t *neetoree_parser_peg(char *name, char *include, neetoree_stream_t *stream) {
    neetoree_parser_builder_context_t *ctx = neetoree_parser_builder_new();
    NeetoreeResult result = neetoree_parser_peg_impl(ctx, ctx->rules, stream);
    if (result != NEETOREE_RESULT_SUCCESS) {
        neetoree_parser_builder_free(ctx);
        return NULL;
    }

    neetoree_string_t *code = neetoree_parser_builder_render(name, include, ctx);
    neetoree_parser_builder_free(ctx);
    return code;
}