//
// Created by user on 12/9/15.
//

#include <stdlib.h>
#include <neetoree_string.h>
#include "neetoree_parser_compile.h"

typedef struct {
    char start;
    char end;
} neetoree_parser_range_t;

typedef struct {
    neetoree_string_t  *singles;
    neetoree_ptrlist_t *ranges;
    int inverse;
} neetoree_parser_range_container_t;

typedef struct {
    char ch;
    NeetoreeResult result;
} neetoree_parser_range_match_t;

typedef enum {
    NEETOREE_PARSER_RANGE_STATE_FIRST,
    NEETOREE_PARSER_RANGE_STATE_ESCAPE,
    NEETOREE_PARSER_RANGE_STATE_RANGE,
} neetoree_parser_range_state;

static NEETOREE_PTRLIST_WALKER(range) {
    neetoree_parser_range_match_t *match = context;
    neetoree_parser_range_t *range = item;
    if (match->ch >= range->start && match->ch <= range->end) {
        match->result = NEETOREE_RESULT_SUCCESS;
        return NEETOREE_STATUS_STOP;
    }
    return NEETOREE_STATUS_CONT;
}

NEETOREE_PARSER_SPEC_NEW(range) {
    neetoree_parser_range_container_t *container = calloc(1, sizeof(neetoree_parser_range_container_t));
    container->singles = neetoree_string_new();
    container->ranges = neetoree_ptrlist_new(free);
    container->inverse = 0;
    neetoree_parser_range_state state = NEETOREE_PARSER_RANGE_STATE_FIRST;
    char *p = spec;
    char start = '\0';
    char end = '\0';
    char ch = '\0';
    while (p && *p) {
        switch (state) {
            case NEETOREE_PARSER_RANGE_STATE_FIRST:
                if (*p == '^' && container->inverse == 0) {
                    container->inverse = 1;
                } else if (*(p+1) == '-') {
                    start = *p;
                    p++;
                    state = NEETOREE_PARSER_RANGE_STATE_RANGE;
                } else {
                    if (*p == '\\') {
                        state = NEETOREE_PARSER_RANGE_STATE_ESCAPE;
                    } else {
                        neetoree_string_append_char(container->singles, (*p));
                    }
                }
                break;
            case NEETOREE_PARSER_RANGE_STATE_ESCAPE:
                switch (*p) {
                    case 'a':
                        ch = '\a';
                        break;
                    case 'b':
                        ch = '\b';
                        break;
                    case 'e':
                        ch = '\e';
                        break;
                    case 'f':
                        ch = '\f';
                        break;
                    case 'n':
                        ch = '\n';
                        break;
                    case 'r':
                        ch = '\r';
                        break;
                    case 't':
                        ch = '\t';
                        break;
                    case 'v':
                        ch = '\v';
                        break;
                    default:
                        ch = *p;
                        break;
                }
                neetoree_string_append_char(container->singles, ch);
                state = NEETOREE_PARSER_RANGE_STATE_FIRST;
                break;
            case NEETOREE_PARSER_RANGE_STATE_RANGE:
                end = *p;
                neetoree_parser_range_t *range = calloc(1, sizeof(neetoree_parser_range_t));
                range->start = start;
                range->end = end;
                neetoree_ptrlist_add(container->ranges, range);
                state = NEETOREE_PARSER_RANGE_STATE_FIRST;
                break;
        }
        p++;
    }

    return container;
}

NEETOREE_PARSER_SPEC_FREE(range) {
    neetoree_parser_range_container_t *container = data;
    NEETOREE_FREE_NAME(string)(container->singles);
    NEETOREE_FREE_NAME(ptrlist)(container->ranges);
    free(container);
}

static NeetoreeResult neetoree_parser_range_apply(neetoree_parser_range_container_t *container, neetoree_parser_applyctx_t *applyctx) {
    neetoree_string_reserve(applyctx->grammar->buffer, 1);
    applyctx->grammar->buffer->len = neetoree_stream_read(applyctx->stream, applyctx->grammar->buffer->data, 1);
    if (applyctx->grammar->buffer->len == 0) {
        return NEETOREE_RESULT_FAILURE;
    }
    for (size_t i = 0; i < container->singles->len; i++) {
        if (container->singles->data[i] == *applyctx->grammar->buffer->data) {
            return NEETOREE_RESULT_SUCCESS;
        }
    }

    neetoree_parser_range_match_t match;
    match.ch = *applyctx->grammar->buffer->data;
    match.result = NEETOREE_RESULT_FAILURE;

    neetoree_ptrlist_walk(container->ranges, NEETOREE_PTRLIST_WALKER_NAME(range), &match);
    return match.result;
}

NEETOREE_PARSER_SPEC_APPLY(range) {
    neetoree_parser_range_container_t *container = data;
    applyctx->result = neetoree_parser_range_apply(container, applyctx);
    if (container->inverse) {
        if (applyctx->result == NEETOREE_RESULT_SUCCESS) {
            applyctx->result = NEETOREE_RESULT_FAILURE;
        } else if (applyctx->result == NEETOREE_RESULT_FAILURE) {
            applyctx->result = NEETOREE_RESULT_SUCCESS;
        }
    }
    return applyctx->result;
}