//
// Created by user on 11/22/15.
//

#include <stdlib.h>
#include "neetoree_parser_valuestack.h"

static NEETOREE_FREE(valuestack) {
    neetoree_parser_value_t *value = item;
    value->freefunc(value->data);
    free(item);
}

neetoree_parser_valuestack_t *neetoree_parser_valuestack_new() {
    return neetoree_ptrstack_new(NEETOREE_FREE_NAME(valuestack));
}

void neetoree_parser_valuestack_free(neetoree_parser_valuestack_t *valuestack) {
    neetoree_ptrstack_free(valuestack);
}

void *neetoree_parser_valuestack_pop(neetoree_parser_valuestack_t *valuestack) {
    neetoree_parser_value_t *value = neetoree_ptrstack_pop(valuestack);
    void *data = value->data;
    free(value);
    return data;
}

void neetoree_parser_valuestack_push(neetoree_parser_valuestack_t *valuestack, void *data, neetoree_freefunc freefunc) {
    neetoree_parser_value_t *value = calloc(1, sizeof(neetoree_parser_value_t));
    value->data = data;
    value->freefunc = freefunc;
    neetoree_ptrstack_push(valuestack, value);
}

void *neetoree_parser_valuestack_peek(neetoree_parser_valuestack_t *valuestack, size_t n) {
    neetoree_parser_value_t *value = neetoree_ptrstack_peek(valuestack, n);
    return value->data;
}

void neetoree_parser_valuestack_drop(neetoree_parser_valuestack_t *valuestack, size_t limit) {
    while (valuestack->global->depth > limit) {
        neetoree_parser_value_t *value = neetoree_ptrstack_pop(valuestack);
        value->freefunc(value->data);
        free(value);
    }
}