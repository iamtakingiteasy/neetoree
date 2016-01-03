//
// Created by user on 12/19/15.
//

#include <stdlib.h>
#include <neetoree_string.h>
#include <neetoree_ptrlist.h>
#include "neetoree_parser_builder.h"

NEETOREE_FREE(builder_node) {
    neetoree_parser_builder_node_t *node = item;
    neetoree_string_free(node->spec);
    neetoree_string_free(node->reference);
    neetoree_ptrlist_free(node->children);
    free(node);
}

static char *neetoree_parser_builder_node_type_names[] = {
        "NONE,     ",
        "RULE,     ",
        "LITERAL,  ",
        "RANGE,    ",
        "DOT,      ",
        "SEQUENCE, ",
        "CHOICE,   ",
        "REF,      ",
        "PRED,     ",
        "ACTION,   ",
        "BEGIN,    ",
        "END,      "
};

static char *neetoree_parser_builder_node_cardinality_names[] = {
        "NONE,      ",
        "ONE,       ",
        "ONEORMORE, ",
        "ZEROORMORE,",
        "ZEROORONE, "
};

static char *neetoree_parser_builder_node_predicate_names[] = {
        "NONE,    ",
        "POSITIVE,",
        "NEGATIVE,"
};

neetoree_string_t * neetoree_parser_builder_name(neetoree_parser_builder_node_t *parent, size_t ord) {
    ord += 1;
    neetoree_string_t *name = neetoree_string_dup_new(parent->reference->data);
    int res = snprintf(NULL, 0, "_%ld", ord);
    if (res > 0) {
        size_t siz = (size_t) res;
        neetoree_string_reserve(name, name->len + siz + 1);
        name->len += snprintf(name->data + name->len, siz+1, "_%ld", ord);
    }
    return name;
}

neetoree_parser_builder_node_t *neetoree_parser_builder_node_new(NeetoreeParserBuilderType type) {
    neetoree_parser_builder_node_t *node = calloc(1, sizeof(neetoree_parser_builder_node_t));
    node->chain = NEETOREE_PARSER_NODE_TYPE_NONE;
    node->children = neetoree_ptrlist_new(NEETOREE_FREE_NAME(builder_node));
    node->cardinality = NEETOREE_PARSER_NODE_CARDINALITY_ONE;
    node->buildtype = type;
    return node;
}

neetoree_parser_builder_context_t *neetoree_parser_builder_new() {
    neetoree_parser_builder_context_t *builder = calloc(1, sizeof(neetoree_parser_builder_context_t));
    builder->actions = neetoree_ptrlist_new(NULL);
    builder->groups = neetoree_ptrlist_new(NULL);
    builder->rules  = neetoree_parser_builder_node_new(NEETOREE_PARSER_BUILDER_TYPE_ROOT);
    return builder;
}

void neetoree_parser_builder_free(neetoree_parser_builder_context_t *builder) {
    neetoree_ptrlist_free(builder->actions);
    neetoree_ptrlist_free(builder->groups);
    NEETOREE_FREE_NAME(builder_node)(builder->rules);
    free(builder);
}

static NEETOREE_PTRLIST_WALKER(builder_nodes) {
    neetoree_parser_builder_node_t *node = item;
    neetoree_parser_builder_render_t *render = context;
    neetoree_string_t *out = render->out;
    if (   node->type == NEETOREE_PARSER_NODE_TYPE_BEGIN
        || node->type == NEETOREE_PARSER_NODE_TYPE_END
        || node->type == NEETOREE_PARSER_NODE_TYPE_ACTION) {
        if (node->type == NEETOREE_PARSER_NODE_TYPE_BEGIN) {
            neetoree_string_append_c_str(out, "            NEETOREE_PARSER_NODE_BEGIN,\n");
        } else if (node->type == NEETOREE_PARSER_NODE_TYPE_END) {
            neetoree_string_append_c_str(out, "            NEETOREE_PARSER_NODE_END,\n");
        } else if (node->type == NEETOREE_PARSER_NODE_TYPE_ACTION) {
            neetoree_string_append_c_str(out, "            NEETOREE_PARSER_NODE_ACTION(");
            neetoree_string_append_str(out, node->spec);
            neetoree_string_append_c_str(out, "),\n");
        }
        return NEETOREE_STATUS_CONT;
    }
    neetoree_string_append_c_str(out, "            NEETOREE_PARSER_NODE(");
    if (node->chain == NEETOREE_PARSER_NODE_TYPE_NONE) {
        neetoree_string_append_c_str(out, neetoree_parser_builder_node_type_names[node->type]);
    } else {
        neetoree_string_append_c_str(out, neetoree_parser_builder_node_type_names[node->chain]);
    }
    neetoree_string_append_c_str(out, " ");
    neetoree_string_append_c_str(out, neetoree_parser_builder_node_cardinality_names[node->cardinality]);
    neetoree_string_append_c_str(out, " ");
    neetoree_string_append_c_str(out, neetoree_parser_builder_node_predicate_names[node->predicate]);
    neetoree_string_append_c_str(out, " ");
    size_t actualsiz = 0;
    if (node->spec && node->type != NEETOREE_PARSER_NODE_TYPE_ACTION) {
        neetoree_string_append_char(out, '"');
        for (size_t i = 0; i < node->spec->len; i++) {
            char zch = node->spec->data[i];
            if (zch == '"') {
                neetoree_string_append_char(out, '\\');
            } else if (node->type == NEETOREE_PARSER_NODE_TYPE_RANGE && zch == '\\') {
                neetoree_string_append_char(out, '\\');
            }
            neetoree_string_append_char(out, zch);
        }
        neetoree_string_append_char(out, '"');
        actualsiz = neetoree_parser_builder_spec_count(node->spec->data, node->type);
    } else {
        neetoree_string_append_c_str(out, "NULL");
        actualsiz = 2;
    }
    neetoree_string_append_c_str(out, ", ");
    for (size_t i = actualsiz; i < render->speclen; i++) {
        neetoree_string_append_char(out, ' ');
    }
    if (node->reference) {
        neetoree_string_append_c_str(out, "rule_");
        neetoree_string_append_c_str(out, node->reference->data);
        neetoree_string_append_c_str(out, "_nodes");
    } else {
        neetoree_string_append_c_str(out, "NULL");
    }
    neetoree_string_append_c_str(out, "),\n");

    return NEETOREE_STATUS_CONT;
}

static NEETOREE_PTRLIST_WALKER(builder_actions) {
    neetoree_parser_builder_node_t *node = item;
    neetoree_parser_builder_render_t *render = context;
    neetoree_string_t *out = render->out;

    neetoree_string_append_c_str(out, "static NEETOREE_PARSER_ACTION_HANDLER(");
    neetoree_string_append_str(out, node->reference);
    neetoree_string_append_c_str(out, ") {\n");
    neetoree_string_append_str(out, node->spec);
    neetoree_string_append_c_str(out, "\n}\n");
}

static NEETOREE_PTRLIST_WALKER(builder_groups) {
    neetoree_parser_builder_node_t *node = item;
    neetoree_parser_builder_render_t *render = context;
    neetoree_string_t *out = render->out;

    neetoree_string_append_c_str(out, "\n    neetoree_parser_node_t rule_");
    neetoree_string_append_c_str(out, node->reference->data);
    neetoree_string_append_c_str(out, "_nodes[] = {\n");
    render->speclen = node->speclength;
    neetoree_ptrlist_walk(node->children, NEETOREE_PTRLIST_WALKER_NAME(builder_nodes), render);
    neetoree_string_append_c_str(out, "            NEETOREE_PARSER_NODE_TERM\n    };\n");

    return NEETOREE_STATUS_CONT;
}

static NEETOREE_PTRLIST_WALKER(builder_rules) {
    neetoree_parser_builder_render_t *render = context;
    neetoree_string_t *out = render->out;
    neetoree_parser_builder_node_t *node = item;
    neetoree_string_append_c_str(out, "            NEETOREE_PARSER_RULE(\"");
    neetoree_string_append_str(out, node->reference);
    neetoree_string_append_c_str(out, "\", ");
    for (size_t i = node->reference->len; i < render->speclen; i++) {
        neetoree_string_append_char(out, ' ');
    }
    if (node->chain == NEETOREE_PARSER_NODE_TYPE_NONE) {
        neetoree_string_append_c_str(out, "NONE,     ");
    } else if (node->chain == NEETOREE_PARSER_NODE_TYPE_SEQUENCE) {
        neetoree_string_append_c_str(out, "SEQUENCE, ");
    } else if (node->chain == NEETOREE_PARSER_NODE_TYPE_CHOICE) {
        neetoree_string_append_c_str(out, "CHOICE,   ");
    }
    neetoree_string_append_c_str(out, "rule_");
    neetoree_string_append_str(out, node->reference);
    neetoree_string_append_c_str(out, "_nodes),\n");
    return NEETOREE_STATUS_CONT;
}

typedef struct {
    neetoree_parser_builder_context_t *builder;
    neetoree_parser_builder_node_t    *parent;
    size_t                             ord;
} neetoree_parser_builder_compact_context_t;

size_t neetoree_parser_builder_spec_count(char *str, NeetoreeParserNodeType type) {
    size_t len = 0;
    char *it = str;
    while (*it) {
        if (type == NEETOREE_PARSER_NODE_TYPE_RANGE && *it == '\\') {
            len++;
        } else if (*it == '"') {
            len++;
        }
        len++;
        it++;
    }
    return len;
}

void neetoree_parser_builder_spec_maxlen(neetoree_parser_builder_node_t *child, neetoree_parser_builder_node_t *parent) {
    if (child->spec || child->reference) {
        char *str = (child->spec) ? child->spec->data : child->reference->data;
        parent->speclength = MAXLEN(neetoree_parser_builder_spec_count(str, child->type), parent->speclength);
    } else {
        parent->speclength = MAXLEN(4, parent->speclength);
    }
}

static void neetoree_parser_builder_cleanse(neetoree_parser_builder_node_t *node, neetoree_ptrlist_t *groups) {
    for (size_t i = 0; i < groups->length; i++) {
        neetoree_parser_builder_node_t *cand = neetoree_ptrlist_access(groups, i);
        if (cand == node) {
            neetoree_ptrlist_remove(groups, i);
            break;
        }
    }
    NEETOREE_FREE_NAME(builder_node)(node);
}

static NEETOREE_PTRLIST_WALKER(builder_compact) {
    neetoree_parser_builder_compact_context_t *compact = context;
    neetoree_parser_builder_node_t *node = item;
    neetoree_parser_builder_node_t *parent = compact->parent;
    if (node->chain != NEETOREE_PARSER_NODE_TYPE_NONE) {
        neetoree_parser_builder_compact_context_t next;
        next.builder = compact->builder;
        next.parent = node;
        neetoree_ptrlist_walk(node->children, NEETOREE_PTRLIST_WALKER_NAME(builder_compact), &next);
        if (node->children->length == 0) {
            for (size_t i = 0; i < parent->children->length; i++) {
                neetoree_parser_builder_node_t *cand = neetoree_ptrlist_access(parent->children, i);
                if (cand == node) {
                    neetoree_ptrlist_remove(parent->children, i);
                    neetoree_parser_builder_cleanse(node, compact->builder->groups);
                    break;
                }
            }
        } else if (node->children->length == 1) {
            for (size_t i = 0; i < parent->children->length; i++) {
                neetoree_parser_builder_node_t *cand = neetoree_ptrlist_access(parent->children, i);
                if (cand == node) {
                    neetoree_parser_builder_node_t *child = neetoree_ptrlist_access(node->children, 0);
                    if ((child->cardinality != NEETOREE_PARSER_NODE_CARDINALITY_ONE && node->cardinality != NEETOREE_PARSER_NODE_CARDINALITY_ONE)
                        ||
                        (child->predicate != NEETOREE_PARSER_NODE_PREDICATE_NONE && node->predicate != NEETOREE_PARSER_NODE_PREDICATE_NONE)) {
                        break;
                    }

                    neetoree_ptrlist_remove(node->children, 0);

                    if (child->cardinality == NEETOREE_PARSER_NODE_CARDINALITY_ONE) {
                        child->cardinality = node->cardinality;
                    }

                    if (child->predicate == NEETOREE_PARSER_NODE_PREDICATE_NONE) {
                        child->predicate = node->predicate;
                    }

                    neetoree_parser_builder_spec_maxlen(child, parent);
                    neetoree_ptrlist_set(parent->children, i, child);
                    neetoree_parser_builder_cleanse(node, compact->builder->groups);
                    break;
                }
            }
        } else if (node->chain == NEETOREE_PARSER_NODE_TYPE_SEQUENCE && parent->chain == NEETOREE_PARSER_NODE_TYPE_SEQUENCE && node->cardinality == NEETOREE_PARSER_NODE_CARDINALITY_ONE && node->predicate == NEETOREE_PARSER_NODE_PREDICATE_NONE) {
            for (size_t i = 0; i < parent->children->length; i++) {
                neetoree_parser_builder_node_t *cand = neetoree_ptrlist_access(parent->children, i);
                if (cand == node) {
                    size_t off = 0;
                    neetoree_ptrlist_remove(parent->children, i);
                    while (node->children->length > 0) {
                        neetoree_parser_builder_node_t *child = neetoree_ptrlist_remove(node->children, 0);
                        neetoree_parser_builder_spec_maxlen(child, parent);
                        neetoree_ptrlist_insert(parent->children, i + off, child);
                        off++;
                    }
                    break;
                }
            }
            neetoree_parser_builder_cleanse(node, compact->builder->groups);
        }
    }
    return NEETOREE_STATUS_CONT;
}

static NEETOREE_PTRLIST_WALKER(builder_rename) {
    neetoree_parser_builder_compact_context_t *ctx = context;
    neetoree_parser_builder_node_t *child = item;
    neetoree_parser_builder_node_t *parent = ctx->parent;

    if (child->chain != NEETOREE_PARSER_NODE_TYPE_NONE) {
        child->reference = neetoree_parser_builder_name(parent, ctx->ord++);
        neetoree_parser_builder_compact_context_t next;
        next.parent = child;
        next.ord = 0;
        neetoree_ptrlist_walk(child->children, NEETOREE_PTRLIST_WALKER_NAME(builder_rename), &next);
    }

    return NEETOREE_STATUS_CONT;
}

void neetoree_parser_builder_compact(neetoree_parser_builder_node_t *rule, neetoree_parser_builder_context_t *ctx) {
    neetoree_parser_builder_compact_context_t context;
    context.builder = ctx;
    context.parent = rule;
    context.ord = 0;
    neetoree_ptrlist_walk(rule->children, NEETOREE_PTRLIST_WALKER_NAME(builder_compact), &context);
    neetoree_ptrlist_walk(rule->children, NEETOREE_PTRLIST_WALKER_NAME(builder_rename), &context);
}

neetoree_string_t *neetoree_parser_builder_render(char *name, char *include, neetoree_parser_builder_context_t *ctx) {
    neetoree_parser_builder_render_t render;
    render.out = neetoree_string_new();

    neetoree_string_append_c_str(render.out, "#include <neetoree_parser.h>\n");
    neetoree_string_append_c_str(render.out, "\nstatic NeetoreeResult ");
    neetoree_string_append_c_str(render.out, name);
    neetoree_string_append_c_str(render.out, "(void *context, void *init, neetoree_stream_t *stream);\n\n");
    if (include) {
        neetoree_string_append_c_str(render.out, "#include \"");
        neetoree_string_append_c_str(render.out, include);
        neetoree_string_append_c_str(render.out, "\"\n");
    }
    //neetoree_string_append_c_str(render.out, "\n");
    //neetoree_ptrlist_walk(ctx->actions, NEETOREE_PTRLIST_WALKER_NAME(builder_actions), &render);
    neetoree_string_append_c_str(render.out, "\nstatic NeetoreeResult ");
    neetoree_string_append_c_str(render.out, name);
    neetoree_string_append_c_str(render.out, "(void *context, void *init, neetoree_stream_t *stream) {\n");
    neetoree_ptrlist_walk(ctx->groups, NEETOREE_PTRLIST_WALKER_NAME(builder_groups), &render);
    neetoree_string_append_c_str(render.out, "\n    neetoree_parser_rule_t rules[] = {\n");
    render.speclen = ctx->rules->speclength;
    neetoree_ptrlist_walk(ctx->rules->children, NEETOREE_PTRLIST_WALKER_NAME(builder_rules), &render);
    neetoree_string_append_c_str(render.out, "            NEETOREE_PARSER_RULE_TERM\n    };\n");
    neetoree_string_append_c_str(render.out, "\n    neetoree_parser_grammar_t *compiled = neetoree_parser_compile(rules);\n");
    neetoree_string_append_c_str(render.out, "    if (compiled == NULL) {\n");
    neetoree_string_append_c_str(render.out, "        return NEETOREE_RESULT_FAILURE;\n");
    neetoree_string_append_c_str(render.out, "    }\n\n");
    neetoree_string_append_c_str(render.out, "    neetoree_parser_valuestack_push(compiled->valuestack, init, NULL);\n");
    neetoree_string_append_c_str(render.out, "    NeetoreeResult result = neetoree_parser_run(compiled, stream, context);\n");
    neetoree_string_append_c_str(render.out, "    neetoree_parser_valuestack_pop(compiled->valuestack);\n");
    neetoree_string_append_c_str(render.out, "    if (result != NEETOREE_RESULT_SUCCESS) {\n");
    neetoree_string_append_c_str(render.out, "        neetoree_parser_free(compiled);\n");
    neetoree_string_append_c_str(render.out, "        return NEETOREE_RESULT_FAILURE;\n");
    neetoree_string_append_c_str(render.out, "    }\n");
    neetoree_string_append_c_str(render.out, "    neetoree_parser_free(compiled);\n");
    neetoree_string_append_c_str(render.out, "    return NEETOREE_RESULT_SUCCESS;\n");

    neetoree_string_append_c_str(render.out, "}\n");
    return render.out;
}