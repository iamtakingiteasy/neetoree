//
// Created by user on 12/6/15.
//

#include <stdlib.h>
#include <string.h>
#ifdef DEBUG_TOKEN
#include <ctype.h>
#endif
#include <neetoree_ptrlist.h>
#include <neetoree_string.h>
#include "neetoree_parser_compile.h"

#define NEETOREE_PARSER_SPEC_FUNC_NAMES(name) \
    { \
        NEETOREE_PARSER_SPEC_NEW_NAME(name), \
        NEETOREE_PARSER_SPEC_FREE_NAME(name), \
        NEETOREE_PARSER_SPEC_APPLY_NAME(name)  \
    }
static neetoree_parser_compiled_iface_t ifaces[] = {
        NEETOREE_PARSER_SPEC_FUNC_NAMES(none),
        NEETOREE_PARSER_SPEC_FUNC_NAMES(none),
        NEETOREE_PARSER_SPEC_FUNC_NAMES(literal),
        NEETOREE_PARSER_SPEC_FUNC_NAMES(range),
        NEETOREE_PARSER_SPEC_FUNC_NAMES(dot),
        NEETOREE_PARSER_SPEC_FUNC_NAMES(group),
        NEETOREE_PARSER_SPEC_FUNC_NAMES(group),
        NEETOREE_PARSER_SPEC_FUNC_NAMES(ref),
        NEETOREE_PARSER_SPEC_FUNC_NAMES(pred),
        NEETOREE_PARSER_SPEC_FUNC_NAMES(action),
        NEETOREE_PARSER_SPEC_FUNC_NAMES(begin),
        NEETOREE_PARSER_SPEC_FUNC_NAMES(end)
};

#undef NEETOREE_PARSER_SPEC_FUNC_NAMES

static neetoree_parser_compiled_iface_t *neetoree_parser_lookup_iface(int type) {
    return &ifaces[type];
}

static NEETOREE_FREE(rule) {
    neetoree_parser_compiled_rule_t *rule = item;
    NEETOREE_FREE_NAME(ptrlist)(rule->children);
    free(rule);
}

static NEETOREE_FREE(node) {
    neetoree_parser_compiled_node_t *node = item;
    neetoree_avltree_free(node->cache);
    node->iface->freefunc(node->data);
    free(node);
}

static NEETOREE_FREE(cache) {
    neetoree_parser_compiled_cache_t *cache = item;
    neetoree_string_free(cache->match);
    free(cache);
}

static NEETOREE_AVLTREE_HASH(cache) {
    neetoree_parser_compiled_cache_t *cache = item;
    return cache->pos;
}

static NEETOREE_PTRLIST_WALKER(rule) {
    neetoree_parser_grammar_t *grammar = context;
    neetoree_parser_compiled_rule_t *rule = item;
    neetoree_parser_node_t *child = rule->origin->children;
    while (child && child->type) {
        neetoree_parser_compile_node(child, rule->children, grammar);
        child++;
    }
    return NEETOREE_STATUS_CONT;
}

static void neetoree_parser_compile_rule(neetoree_parser_rule_t *origin, neetoree_parser_grammar_t *grammar) {
    neetoree_parser_compiled_rule_t *rule = calloc(1, sizeof(neetoree_parser_compiled_rule_t));
    rule->origin = origin;
    rule->children = neetoree_ptrlist_new(NEETOREE_FREE_NAME(node));
    rule->iface = neetoree_parser_lookup_iface(origin->containment);
    neetoree_ptrlist_add(grammar->rules, rule);
}

neetoree_parser_compiled_node_t *neetoree_parser_compile_node(neetoree_parser_node_t *origin, neetoree_ptrlist_t *container, neetoree_parser_grammar_t *grammar) {
    neetoree_parser_compiled_node_t *node = calloc(1, sizeof(neetoree_parser_compiled_node_t));
    node->cache = neetoree_avltree_new(NEETOREE_AVLTREE_HASH_NAME(cache), NEETOREE_FREE_NAME(cache));
    node->iface = neetoree_parser_lookup_iface(origin->type);
    node->data = node->iface->newfunc(origin->spec, origin, grammar);
    node->cardinality = (NeetoreeParserNodeCardinality) origin->cardinality;
    node->pred = (NeetoreeParserNodePredicate) origin->predicate;
    node->type = (NeetoreeParserNodeType) origin->type;
    if (container) {
        neetoree_ptrlist_add(container, node);
    }
    //debug
    node->origin = origin;

    return node;
}

neetoree_parser_grammar_t *neetoree_parser_compile(neetoree_parser_rule_t *rules) {
    neetoree_parser_grammar_t *grammar = neetoree_parser_new();
    neetoree_parser_rule_t *rule = rules;
    while (rule && rule->name) {
        neetoree_parser_compile_rule(rule, grammar);
        rule++;
    }

    neetoree_ptrlist_walk(grammar->rules, NEETOREE_PTRLIST_WALKER_NAME(rule), grammar);

    if (grammar->rules->length > 0) {
        neetoree_parser_compiled_rule_t *entryrule = grammar->rules->ptrs[0];
        neetoree_parser_node_t *entryref = calloc(1, sizeof(neetoree_parser_node_t));

        entryref->type = NEETOREE_PARSER_NODE_TYPE_REF;
        entryref->spec = entryrule->origin->name;
        entryref->children = NULL;
        entryref->cardinality = NEETOREE_PARSER_NODE_CARDINALITY_ONE;
        entryref->predicate = NEETOREE_PARSER_NODE_PREDICATE_NONE;
        grammar->entry = neetoree_parser_compile_node(entryref, NULL, grammar);
    }

    return grammar;
}


void neetoree_parser_free(neetoree_parser_grammar_t *grammar) {
    if (!grammar) {
        return;
    }
    NEETOREE_FREE_NAME(ptrlist)(grammar->rules);
    neetoree_parser_valuestack_free(grammar->valuestack);
    if (grammar->entry) {
        //debug
        free(grammar->entry->origin);
        NEETOREE_FREE_NAME(node)(grammar->entry);
    }
    neetoree_string_free(grammar->match);
    neetoree_string_free(grammar->buffer);
    free(grammar);
}

neetoree_parser_grammar_t *neetoree_parser_new() {
    neetoree_parser_grammar_t *grammar = calloc(1, sizeof(neetoree_parser_grammar_t));
    grammar->valuestack = neetoree_parser_valuestack_new();
    grammar->rules = neetoree_ptrlist_new(NEETOREE_FREE_NAME(rule));
    grammar->match = neetoree_string_new();
    grammar->buffer = neetoree_string_new();
    grammar->matchoz = 0;
    return grammar;
}

#ifdef DEBUG_TOKEN
static char *escapepass(char *inp) {
    char *p = inp;
    int hex = 0;
    while (p && *p) {
        if (!isprint(*p)) {
            hex = 1;
        }
        p++;
    }

    size_t len = p - inp;

    if (hex) {
        char *out = calloc(1, 2 + (len * 5) + 1);
        char *w = out;
        p = inp;
        *w++ = '0';
        *w++ = 'x';
        int r = 0;
        while (p && *p && r++ < 3) {
            snprintf(w, 3, "%02x", *p & 0xFF);
            w += 2;
            p++;
        }
        return out;
    } else if (inp != NULL) {
        return strdup(inp);
    } else {
        return NULL;
    }
}
#endif

NeetoreeResult neetoree_parser_applywrap(neetoree_parser_compiled_node_t *node, neetoree_parser_applyctx_t *parent) {
#ifdef DEBUG_TOKEN
    static char *types[] = {
            "NONE",
            "RULE",
            "LITERAL",
            "RANGE",
            "DOT",
            "SEQUENCE",
            "CHOICE",
            "REF",
            "PRED",
            "ACTION",
            "BEGIN",
            "END"
    };

    static char *cardins[] = {
            "NONE",
            "ONE",
            "ONEORMORE",
            "ZEROORMORE",
            "ZEROORONE"
    };

    char zch[5];

    char val = (char) ((parent->stream->offset > 0 ) ? *(parent->stream->current->data + parent->stream->offset - 1) : '\0');
    snprintf(zch, 5, (isprint(val)) ? "'%c'  " : "0x%02x", val);

    printf("(%5ld %s) ", parent->stream->total, zch);
    neetoree_parser_applyctx_t *p = parent;
    size_t upt = 0;
    while (p) {
        upt++;
        printf("-");
        p = p->parent;
    }

    char *spec = escapepass(node->origin->spec);
    char *crdl = "";
    if (node->origin->type == NEETOREE_PARSER_NODE_TYPE_REF) {
        neetoree_parser_compiled_rule_t *rule = node->data;
        crdl = types[rule->origin->containment];
    }
    printf(" %3ld %8s %s @ %-10s : '%s'\n", upt, types[node->origin->type], crdl, cardins[node->origin->cardinality], spec);
#endif
    if (parent->status == NEETOREE_STATUS_STOP) {
        return parent->result;
    }

    neetoree_parser_applyctx_t applyctx;
    applyctx.grammar = parent->grammar;
    applyctx.stream = neetoree_stream_fork(parent->stream);
    applyctx.type = node->type;
    applyctx.parent = parent;
    applyctx.status = NEETOREE_STATUS_CONT;
    applyctx.trace = parent->trace;

    size_t prev = applyctx.stream->total;
    NeetoreeResult result = node->iface->applyfunc(node->data, &applyctx);
#ifdef DEBUG_TOKEN
    val = (char) ((parent->stream->offset > 0 ) ? *(parent->stream->current->data + parent->stream->offset - 1) : '\0');
    snprintf(zch, 5, (isprint(val)) ? "'%c'  " : "0x%02x", val);

    printf("(%5ld %s) ", parent->stream->total, zch);
    p = parent;
    while (p) {
        printf("-");
        p = p->parent;
    }
    char *neg = "";
    if (node->pred == NEETOREE_PARSER_NODE_PREDICATE_NEGATIVE) {
        neg = "!";
    }
    printf("%s%3ld %s('%s')\n", result == NEETOREE_RESULT_SUCCESS ? "+" : (result == NEETOREE_RESULT_NONE) ? "~" :"!", upt, neg, spec);
    free(spec);
#endif
    if (result != NEETOREE_RESULT_NONE && result != NEETOREE_RESULT_ERROR) {
        int skip = 0;
        int count = 0;
        switch (node->cardinality) {
            case NEETOREE_PARSER_NODE_CARDINALITY_NONE:
                break;
            case NEETOREE_PARSER_NODE_CARDINALITY_ONE:
                break;
            case NEETOREE_PARSER_NODE_CARDINALITY_ZEROORONE:
                if (result == NEETOREE_RESULT_FAILURE) {
                    skip = 1;
                    result = NEETOREE_RESULT_SUCCESS;
                }
                break;
            case NEETOREE_PARSER_NODE_CARDINALITY_ZEROORMORE:
                count = 0;
                while (result == NEETOREE_RESULT_SUCCESS && applyctx.stream->total - prev > 0) {

                    if (applyctx.grammar->matchoz && applyctx.grammar->buffer->len > 0) {
                        neetoree_string_append_str(applyctx.grammar->match, applyctx.grammar->buffer);
                    }
                    neetoree_stream_commit(applyctx.stream);

                    prev = applyctx.stream->total;
                    result = node->iface->applyfunc(node->data, &applyctx);
                    if (result == NEETOREE_RESULT_NONE) {
                        result = NEETOREE_RESULT_SUCCESS;
                    }
                    count++;
                }
                if (result == NEETOREE_RESULT_FAILURE) {
                    skip = 1;
                    result = NEETOREE_RESULT_SUCCESS;
                }
                break;
            case NEETOREE_PARSER_NODE_CARDINALITY_ONEORMORE:
                if (result != NEETOREE_RESULT_SUCCESS) {
                    break;
                }

                while (result == NEETOREE_RESULT_SUCCESS && applyctx.stream->total - prev > 0) {
                    prev = applyctx.stream->total;
                    result = node->iface->applyfunc(node->data, &applyctx);
                    if (result == NEETOREE_RESULT_NONE) {
                        result = NEETOREE_RESULT_SUCCESS;
                    }
                }

                if (result == NEETOREE_RESULT_FAILURE) {
                    result = NEETOREE_RESULT_SUCCESS;
                }
                break;
        }

        if (node->pred == NEETOREE_PARSER_NODE_PREDICATE_NEGATIVE) {
            if (result == NEETOREE_RESULT_SUCCESS) {
                result = NEETOREE_RESULT_FAILURE;
            } else if (result == NEETOREE_RESULT_FAILURE) {
                result = NEETOREE_RESULT_SUCCESS;
            }
        }

        if (node->pred != NEETOREE_PARSER_NODE_PREDICATE_NONE) {
            skip = 1;
        }

        if (result == NEETOREE_RESULT_SUCCESS && !skip) {
            if (applyctx.grammar->matchoz && applyctx.stream->total - prev > 0) {
                neetoree_string_append_str(applyctx.grammar->match, applyctx.grammar->buffer);
            }
            neetoree_stream_commit(applyctx.stream);
        }
        neetoree_string_reset(parent->grammar->buffer);
    } else if (result == NEETOREE_RESULT_ERROR) {
        if (applyctx.error) {
            printf("Error occurred: %s\n", applyctx.error->data);
            neetoree_string_free(applyctx.error);
        } else {
            printf("Unknown error occurred\n");
        }

    }

    neetoree_stream_free(applyctx.stream);

    return result;
}

static NEETOREE_PTRSTACK_WALKER(trace) {
    neetoree_string_t *str = item;
    neetoree_string_t *top = context;
    if (top != str) {
        printf(" <- ");
    }
    printf("%s\n", str->data);
    return NEETOREE_STATUS_CONT;
}

static NEETOREE_STREAM_HANDLER(trace) {
    neetoree_parser_location_t *location = context;
    for (size_t i = 0; i < len; i++) {
        char ch = data[i];
        if (ch == '\n') {
            location->line++;
            location->pos = 0;
        }
        location->pos++;
        location->total++;
    }
}

static NEETOREE_STREAM_HANDLER_CONTEXT_CLONE(trace) {
    neetoree_parser_location_t *loc = calloc(1, sizeof (neetoree_parser_location_t));
    neetoree_parser_location_t *p = parent;
    if (p) {
        loc->line = p->line;
        loc->pos = p->pos;
        loc->total = p->total;
    }
    return loc;
}

static NEETOREE_STREAM_HANDLER_CONTEXT_MERGE(trace) {
    neetoree_parser_location_t *loc = a;
    neetoree_parser_location_t *p   = b;
    if (loc) {
        loc->line = p->line;
        loc->pos = p->pos;
        loc->total = p->total;
    }
    free(p);
}

NeetoreeResult neetoree_parser_run(neetoree_parser_grammar_t *grammar, neetoree_stream_t *stream, void *context) {
    if (!grammar->entry) {
        return NEETOREE_RESULT_FAILURE;
    }

    grammar->context = context;

    neetoree_parser_applyctx_t applyctx;
    applyctx.grammar = grammar;
    applyctx.stream = neetoree_stream_fork(stream);
    applyctx.type = grammar->entry->type;
    applyctx.parent = NULL;
    applyctx.status = NEETOREE_STATUS_CONT;
    applyctx.trace = neetoree_ptrstack_new(NEETOREE_FREE_NAME(string));

    neetoree_stream_set_handler(applyctx.stream,
                                NEETOREE_STREAM_HANDLER_NAME(trace),
                                NEETOREE_STREAM_HANDLER_CONTEXT_CLONE_NAME(trace),
                                NEETOREE_STREAM_HANDLER_CONTEXT_MERGE_NAME(trace)
    );

    neetoree_parser_location_t location;
    memset(&location, 0, sizeof(neetoree_parser_location_t));
    NeetoreeResult result = neetoree_parser_applywrap(grammar->entry, &applyctx);
    if (result != NEETOREE_RESULT_SUCCESS) {
        neetoree_stream_commit_handler(applyctx.stream, &location);
        printf("Error at (%ld:%ld ~ %ld) from ",
               location.line + 1, location.pos + 1,
               location.total
        );

        neetoree_string_t *top = neetoree_ptrstack_peek(applyctx.trace, 0);
        neetoree_ptrstack_walk(applyctx.trace, NEETOREE_PTRSTACK_WALKER_NAME(trace), top);
        printf("\n");

        static size_t span = 20;
        static size_t totalspan = 20 * 2 + 2;
        char buf[totalspan];
        memset(buf, 0, totalspan);
        size_t start = 0;
        size_t offset = span;
        if (location.pos > span) {
            start = location.pos - span;
        } else {
            offset = location.pos;
        }

        neetoree_stream_advance(stream, start);
        neetoree_stream_read(stream, buf, totalspan - 1);
        fprintf(stderr, "\"%s\"\n", buf);
        fprintf(stderr, " ");
        for (size_t i = 0; i < offset; i++) {
            fprintf(stderr, " ");
        }
        fprintf(stderr, "^");
        fprintf(stderr, "\n");
    }

    neetoree_stream_commit(applyctx.stream);
    neetoree_stream_free(applyctx.stream);
    neetoree_ptrstack_free(applyctx.trace);
    return result;
}